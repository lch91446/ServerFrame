#pragma once

#include "stdafx.h"
#include "Connection.h"
#include "Session.h"
#include "App.h"

class Server
{
private:
	//Server(void);
public:
	Server(boost::asio::io_service& io_service, unsigned short port, SessionManager& smgr);
	~Server();
	void handle_stop();
	void run();
	void handle_accept(const boost::system::error_code& e);
	void handle_write(const boost::system::error_code& e, ConnectionPtr conn);
private:
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	ConnectionManager connection_mgr_;
	SessionManager& session_mgr_;
	ConnectionPtr connection_;
};

