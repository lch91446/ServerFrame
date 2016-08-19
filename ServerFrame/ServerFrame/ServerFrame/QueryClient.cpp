#include "StdAfx.h"
#include "App.h"
#include "QueryClient.h"



QueryClient::QueryClient( boost::asio::io_service& io_service ):io_service_(io_service)
{

}


QueryClient::~QueryClient(void)
{
}

void QueryClient::set_endpoint(const boost::asio::ip::tcp::endpoint& endpoint)
{
	endpoint_ = endpoint;
}

ConnectionPtr QueryClient::get_connection()
{
	boost::mutex::scoped_lock lck(mutex_);
	//获取一个新的可用连接;
	while(!ready_.empty())
	{
		//队列中有可用连接就拿来用，没有就新建一个连接;
		ConnectionPtr c(ready_.front());
		ready_.pop();
		if(manager_.exists(c))
		{
			return c;
		};
	}
	return ConnectionPtr();
}


void QueryClient::do_query(SessionPtr s, const Header& header, buffer_type& buffer)
{
	
	ConnectionPtr c(get_connection());
	if(c)
	{
		c->set_handler(s, boost::bind(&QueryClient::release, this, _1));
		c->send_packet(header, buffer);
	}else
	{
		Connection* connection = new Connection(io_service_, manager_);
		Callback* callback = new Callback(connection, header, buffer);
		connection->socket().async_connect(endpoint_,
			boost::bind(&QueryClient::handle_connect, this, 
						callback, s, boost::asio::placeholders::error
						)
			);
	}
}

void QueryClient::handle_connect(Callback *cb, SessionPtr s, 
	const boost::system::error_code& err)
{
	Logger& slog(theapp.get_logger());
	if(NULL == cb)
	{
		slog.err(MYSTR("QueryClient::handle_connect NULL callback."));
		return;
	}

	if(!err)
	{
		boost::asio::ip::tcp::socket& socket(cb->connection->socket());
		std::ostringstream os;
		os.str().reserve(128);
		os<<"Create a new connection to	DBServer:	"<<socket.local_endpoint()
			<<"	==>	"<<socket.remote_endpoint();
			//<<"	This is a common link, connection reusable and automatically";
		slog.system(os.str());
		ConnectionPtr c(cb->connection);
		c->set_handler(s, boost::bind(&QueryClient::release, this, _1));
		manager_.add(c);
		c->send_packet(cb->header, cb->buffer);
	}else
	{
		Connection* connection = cb->connection;
		delete connection;
	}
	delete cb;
}

void QueryClient::release(ConnectionPtr c)
{
	c->reset_handler();
	boost::mutex::scoped_lock lck(mutex_);
	ready_.push(c);	//断开连接的时候加入到队列中;
}