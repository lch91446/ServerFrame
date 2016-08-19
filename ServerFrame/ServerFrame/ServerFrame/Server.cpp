#include "stdafx.h"
#include "Server.h"
#include "logger.h"


Server::Server( boost::asio::io_service& io_service, unsigned short port, SessionManager& smgr ):
	io_service_(io_service), acceptor_(io_service), session_mgr_(smgr)
{
	Logger& slog(theapp.get_logger());
	slog.system(MYSTD string("Server::Server..."));
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();
	slog.system(MYSTD string("connection开始异步监听"));
	//这里创建一个connection开始异步监听;
	connection_.reset(new Connection(io_service, connection_mgr_));
	acceptor_.async_accept(connection_->socket(),
		boost::bind(&Server::handle_accept, this, boost::asio::placeholders::error)
		);
}


Server::~Server(void)
{
}

void Server::run()
{
	io_service_.run();
}

void Server::handle_accept( const boost::system::error_code& e )
{
	if(!e)
	{
		Logger& slog(theapp.get_logger());
		boost::asio::ip::tcp::socket& socket(connection_->socket());
		std::ostringstream os;
		os.str().reserve(128);
		os<<"Accept a new connection:	"<<socket.remote_endpoint()
			<<"	==>	"<<socket.local_endpoint();
		slog.system(os.str());
		std::cout<<os.str()<<"\n";
		SessionPtr session(session_mgr_.create_session());//通过session工厂创建一个session出来;
		connection_->set_handler(session);
		connection_mgr_.add(connection_);
		session->handle_connect(connection_);

		// Start an accept operation for a new connection.
		connection_.reset(new Connection(io_service_, connection_mgr_));
		acceptor_.async_accept(connection_->socket(),
			boost::bind(&Server::handle_accept, this, boost::asio::placeholders::error)
			);
	}else
	{
		//logging::slog_append(slog, logging::LogLevel::Error, __HERE__, e);
	}
}

void Server::handle_write( const boost::system::error_code& e, ConnectionPtr conn )
{
	return;
}

void Server::handle_stop()
{
	acceptor_.close();
	connection_mgr_.stop_all();
}
