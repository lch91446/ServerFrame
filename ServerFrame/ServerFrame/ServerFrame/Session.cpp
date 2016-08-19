#include "stdafx.h"

#include "Session.h"
//#include "UserbillLogic.h"

// Session::Session(void)
// {
// }

Session::Session( SessionManager& smgr )
	: manager_(smgr), curr_logic_(NULL)
{
	
}


Session::~Session(void)
{
}

void Session::handle_connect( ConnectionPtr c )
{
	main_conn_ = c;
	c->start();
}

void Session::handle_write( ConnectionPtr c )
{
	boost::mutex::scoped_lock lck(mutex_);
	if(main_conn_ == c)
	{ 
		//主链接发回的数据;
		if(curr_logic_)
		{
			curr_logic_->handle_write(c);
			//这里要添加主链接之后的处理是继续监听还是关闭链接？;
			//logic_finish();
		}else
		{
			delete curr_logic_;
			curr_logic_ = NULL;
			main_conn_->close();
		}
	}else
	{
		//次连接创建后开始侦听连接发回的数据;
		c->start();
	}
}

void Session::logic_finish()
{
	if(curr_logic_)
	{
		delete curr_logic_;
		curr_logic_ = NULL;//在这里完成了一次计算操作，正常释放指针;
	}
	if(main_conn_)
	{main_conn_->start();}
	
}

int Session::handle_request( ConnectionPtr c, const Header& header, const buffer_type& data )
{
	//加锁;
	//
	boost::mutex::scoped_lock lock(mutex_);
	if(main_conn_ == c)
	{
		//主链接，响应客户端发来的链接请求;
		if(NULL == curr_logic_)
		{
			//当前没有业务逻辑;
			switch(header.command)
			{
			case 1:
				break;

			default:
				main_conn_.reset();
				manager_.erase(shared_from_this());
				break;
			}
			if(curr_logic_) 
			{
				//逻辑部分开始;
				curr_logic_->start();
			}
		}
		else
		{
			//已近存在业务逻辑;
			//这种情况也基本不可能出现;
			std::ostringstream os;
			os.str().reserve(1024);
			os << " Session::handle_request , has handler";
			Logger& slog(theapp.get_logger());
			slog.system(MYSTR("Connection::handle_read_data"+ os.str()));
			slog.err(MYSTR("Connection::handle_read_data"+ os.str()));
			return -1;//错误情况;
		}
	}
	else
	{
		//次连接，主动发起的链接请求;
		if(curr_logic_)
		{
			curr_logic_->handle_request(c, header, data);
		}else
		{
			//slog.error("Session::handle_request", "invalid request");
			//TODO: invalid request
		}
		//在这里释放次连接;
		c->release();
	}
	return 0;
	
}

void Session::reply_packet( Header& header, const buffer_type& buffer )
{
	//向发起链接的客户端返回数据;
	ConnectionPtr connection = main_conn_;
	if(connection) connection->send_packet(header, buffer);
}

void Session::reply_packet( const Header& header )
{
	//出错情况;
	ConnectionPtr connection = main_conn_;
	if(connection) connection->send_packet(header);
}

void Session::handle_error( ConnectionPtr c, const boost::system::error_code& e )
{
	Logger& slog(theapp.get_logger());
	std::ostringstream os;
	os << "error: " << e;  
	slog.err(MYSTR("Session::handle_error "+os.str()));
	boost::mutex::scoped_lock lck(mutex_);
	ConnectionPtr main_conn=main_conn_;
	if(c != main_conn)
	{
		if(curr_logic_)
		{
			curr_logic_->handle_socket_error(c, e);
		}
	}
	else
	{

		if(curr_logic_)
		{
			delete curr_logic_;
			curr_logic_ = NULL;
		}

		//main_conn_->reset_handler();	
		main_conn_.reset();
		manager_.erase(shared_from_this());
	}
}

void Session::handle_write_err( ConnectionPtr c, const boost::system::error_code& e)
{
	//数据发送失败;
	handle_error(c,e);
	return;
}

void SessionManager::insert( SessionPtr c )
{
	boost::mutex::scoped_lock lck(mutex_);
	sessions_.insert(c);
}

void SessionManager::erase( SessionPtr c )
{
	boost::mutex::scoped_lock lck(mutex_);
	sessions_.erase(c);
}

SessionPtr SessionManager::create_session()
{
	SessionPtr session(new Session(*this));
	{
		boost::mutex::scoped_lock lck(mutex_);
		sessions_.insert(session);
	}
	return session;
}
