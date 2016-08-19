#pragma once

#include "Connection.h"
#include "Logic.h"
class SessionManager;
class Session:public boost::enable_shared_from_this<Session>,private boost::noncopyable
{
private:
	//Session(void);
public:
	typedef std::vector<char> buffer_type;
public:
	Session(SessionManager& smgr);
	~Session();
	//void stop();

	void handle_connect(ConnectionPtr c); 
	//void handle_connect_error(const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code& e);
	void handle_error(ConnectionPtr c, const boost::system::error_code& e);
	void handle_write(ConnectionPtr c);
	void handle_write_err(ConnectionPtr c,const boost::system::error_code& e);
	int handle_request(ConnectionPtr c, const Header& header, const buffer_type& data);

	void reply_packet( Header& header, const buffer_type& buffer);
	void reply_packet(const Header& header);

	void logic_finish();
private:
	boost::mutex mutex_;
	SessionManager& manager_;
	//ConnectionPtr curr_conn_;
	ConnectionPtr main_conn_;
	int a;
	Logic* curr_logic_;

};
typedef boost::shared_ptr<Session> SessionPtr;

class SessionManager : private boost::noncopyable
{
public:
	void insert(SessionPtr c);
	void erase(SessionPtr c);
	SessionPtr create_session();
private:
	/// The managed connections.
	std::set<SessionPtr> sessions_;
	boost::mutex mutex_;
};
