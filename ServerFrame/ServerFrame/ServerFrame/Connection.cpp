#include "stdafx.h"
#include "Connection.h"
#include "Session.h"

#include "App.h"
// Connection::Connection(void)
// {
// }

Connection::Connection( boost::asio::io_service& io_service, ConnectionManager& manager, SessionPtr handler )
	: socket_(io_service), status_(BEGIN), manager_(manager), handler_(handler)
{
	memset(inbound_header_,0,header_length);
}

Connection::Connection( boost::asio::io_service& io_service, ConnectionManager& manager )
	: socket_(io_service), status_(BEGIN), manager_(manager)
{
	memset(inbound_header_,0,header_length);
}


Connection::~Connection(void)
{
}

void Connection::start()
{
	if(handler_)
	{
		async_read_header();
	}
}

void Connection::release()
{
	handler_.reset();
	if(release_handler_) release_handler_(shared_from_this());
}

void Connection::stop()
{
	Logger& slog(theapp.get_logger());
	std::ostringstream os;
	os.str().reserve(128);
	os<<"Connection stop	"<<socket_.remote_endpoint()
		<<"	=\\=>	"<<socket_.local_endpoint();
	slog.system(os.str());
	std::cout<<os.str()<<"\n";
	socket_.close();
	status_ = CLOSED;
	handler_.reset();
}

void Connection::close()
{
	status_ = CLOSING;
	if(close_handler_) close_handler_(shared_from_this());
	manager_.stop(shared_from_this());
}

void Connection::handle_read_header( const boost::system::error_code& e, size_t length )
{
	SessionPtr handler=handler_;	
	if (!e)
	{
		async_read_data();//正确的读数据;
	}else
	{
		handle_error("Connection::handle_read_header", e);
	}
}

void Connection::handle_read_data( const boost::system::error_code& e, size_t length )
{
	SessionPtr handler=handler_;
	if (!e && length==header_.length)
	{
		if(handler)
		{
			try
			{
				int ret = handler->handle_request(shared_from_this(), header_, inbound_data_);
				//在这里开始做数据处理;
				if(0 != ret) 
					{
						close();
					}
			} 
			catch(std::exception& le) 
			{
				std::ostringstream os;
				os.str().reserve(1024);
				os << "Loader error:" << le.what()
					<< "\nreceive: " << length << " bytes"
					<< ", command = " << header_.command 
					<< ", length = " << header_.length
					<< ", err_code = " << std::hex << header_.err_code << "\n";
				Logger& slog(theapp.get_logger());
				slog.err(MYSTR("Connection::handle_read_data"+os.str()));
				boost::system::error_code ec(0x01910003, e.category());
				handler->handle_error(shared_from_this(),ec);
				close();
			}
		}else{
			std::ostringstream os;
			os.str().reserve(1024);
			os << "receive " << length << " bytes, but no handler";
			Logger& slog(theapp.get_logger());
			slog.err(MYSTR("Connection::handle_read_data"+ os.str()));
		}
	}else
	{
		handle_error("Connection::handle_read_data", e);
	}
}

void Connection::handle_connect()
{
	SessionPtr handler=handler_;
	if(handler){
		handler->handle_connect(shared_from_this());
	}
	status_ = CONNECTED;
}

void Connection::handle_error( const boost::system::error_code& e )
{
	if(is_valid())
	{
		SessionPtr handler = handler_;
		if(handler) handler->handle_error(shared_from_this(), e);
	}
	close();
}

void Connection::handle_error( const char* where, const boost::system::error_code& e )
{
	if(is_valid())
	{
		SessionPtr handler = handler_;
		if(handler) handler->handle_error(shared_from_this(), e);
	}
	close();
}

void Connection::async_read_header()
{
	boost::asio::async_read(socket_, 
		boost::asio::buffer(inbound_header_),
		boost::bind(&Connection::handle_read_header, this, 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred
			)
		);
}

void Connection::async_read_data()
{
	inbound_data_.resize(header_.length);//数据缓冲区 ;
	boost::asio::socket_base::bytes_readable command(true);
	socket_.io_control(command);
	std::size_t bytes_readable = command.get();
	if(bytes_readable == header_.length) 
	{
		boost::system::error_code ec;
		std::size_t length=socket_.read_some(boost::asio::buffer(inbound_data_), ec);//同步读取;
		handle_read_data(ec,length);
	}else
	{
		boost::asio::async_read(socket_, 
			boost::asio::buffer(inbound_data_),
			boost::bind(&Connection::handle_read_data, this, 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred
				)
			);
	}
}

void Connection::sync_read_data()
{
	boost::system::error_code ec;
	inbound_data_.resize(header_.length);
	boost::asio::read(socket_, 
		boost::asio::buffer(inbound_data_),
		boost::asio::transfer_all(), ec
		);
}

void Connection::async_write()
{

}

void Connection::handle_write( const boost::system::error_code& e )
{
	SessionPtr handler = handler_;
	if(!e)
	{
		handler->handle_write(shared_from_this());
	}else
	{
		handle_error("Connection::handle_write", e);
		handler->handle_write_err(shared_from_this(),e);
	}
}

void Connection::send_packet( const Header& header, const buffer_type& data )
{
	outbound_header_.clear();
	//myserialize::do_save(outbound_header_, header);
	//在这里要把包头转换成网络字节序;
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(boost::asio::buffer(outbound_header_));

	outbound_data_.resize(data.size());
	std::copy(data.begin(), data.end(), outbound_data_.begin());
	buffers.push_back(boost::asio::buffer(outbound_data_));
	
		
	boost::asio::async_write(socket_, buffers, 
		boost::bind(&Connection::handle_write, this, 
		boost::asio::placeholders::error)
		);
}

void Connection::send_packet( const Header& header )
{
	outbound_header_.clear();
	myserialize::do_save(outbound_header_, header);
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(boost::asio::buffer(outbound_header_));

	boost::asio::async_write(socket_, buffers, 
		boost::bind(&Connection::handle_write, this, 
		boost::asio::placeholders::error)
		);	
}

void ConnectionManager::start( ConnectionPtr c )
{
	{
		boost::mutex::scoped_lock lck(mutex_);
		connections_.insert(c);
	}
	c->start();
}

void ConnectionManager::stop( ConnectionPtr c )
{
	{
		boost::mutex::scoped_lock lck(mutex_);
		connections_.erase(c);
	}
	c->stop();
}

void ConnectionManager::stop_all()
{
	boost::mutex::scoped_lock lck(mutex_);
	std::for_each(connections_.begin(), connections_.end(), boost::bind(&Connection::stop, _1));
	connections_.clear();
}

void ConnectionManager::add( ConnectionPtr c )
{
	boost::mutex::scoped_lock lck(mutex_);
	connections_.insert(c);
}

void ConnectionManager::erase( ConnectionPtr c )
{
	boost::mutex::scoped_lock lck(mutex_);
	connections_.erase(c);
}

size_t ConnectionManager::size() const
{
	return connections_.size();
}

bool ConnectionManager::exists( ConnectionPtr c )
{
	return (connections_.end() != connections_.find(c));
}

bool ConnectionManager::exists( ConnectionPtr c ) const
{
	return (connections_.end() != connections_.find(c));
}
