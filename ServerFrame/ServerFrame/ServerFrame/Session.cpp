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
		//�����ӷ��ص�����;
		if(curr_logic_)
		{
			curr_logic_->handle_write(c);
			//����Ҫ���������֮��Ĵ����Ǽ����������ǹر����ӣ�;
			//logic_finish();
		}else
		{
			delete curr_logic_;
			curr_logic_ = NULL;
			main_conn_->close();
		}
	}else
	{
		//�����Ӵ�����ʼ�������ӷ��ص�����;
		c->start();
	}
}

void Session::logic_finish()
{
	if(curr_logic_)
	{
		delete curr_logic_;
		curr_logic_ = NULL;//�����������һ�μ�������������ͷ�ָ��;
	}
	if(main_conn_)
	{main_conn_->start();}
	
}

int Session::handle_request( ConnectionPtr c, const Header& header, const buffer_type& data )
{
	//����;
	//
	boost::mutex::scoped_lock lock(mutex_);
	if(main_conn_ == c)
	{
		//�����ӣ���Ӧ�ͻ��˷�������������;
		if(NULL == curr_logic_)
		{
			//��ǰû��ҵ���߼�;
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
				//�߼����ֿ�ʼ;
				curr_logic_->start();
			}
		}
		else
		{
			//�ѽ�����ҵ���߼�;
			//�������Ҳ���������ܳ���;
			std::ostringstream os;
			os.str().reserve(1024);
			os << " Session::handle_request , has handler";
			Logger& slog(theapp.get_logger());
			slog.system(MYSTR("Connection::handle_read_data"+ os.str()));
			slog.err(MYSTR("Connection::handle_read_data"+ os.str()));
			return -1;//�������;
		}
	}
	else
	{
		//�����ӣ������������������;
		if(curr_logic_)
		{
			curr_logic_->handle_request(c, header, data);
		}else
		{
			//slog.error("Session::handle_request", "invalid request");
			//TODO: invalid request
		}
		//�������ͷŴ�����;
		c->release();
	}
	return 0;
	
}

void Session::reply_packet( Header& header, const buffer_type& buffer )
{
	//�������ӵĿͻ��˷�������;
	ConnectionPtr connection = main_conn_;
	if(connection) connection->send_packet(header, buffer);
}

void Session::reply_packet( const Header& header )
{
	//�������;
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
	//���ݷ���ʧ��;
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
