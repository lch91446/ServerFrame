#pragma once

#include "stdafx.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "SocketDate.h"
//#include "Define.h"
//#include "App.h"

class Connection;
class Session;

typedef boost::shared_ptr<Connection> ConnectionPtr;
typedef boost::shared_ptr<Session> SessionPtr;
class Logic
{
public:
	typedef std::vector<char> buffer_type;
public:
	Logic(){};
	Logic(SessionPtr sess, const Header& header);
	virtual ~Logic();
	virtual void start() = 0;
	virtual void handle_request(ConnectionPtr c, const Header& header, const buffer_type& data) = 0;//业务逻辑处理;
	virtual void handle_socket_error(ConnectionPtr c, const boost::system::error_code& e) = 0;
	virtual void handle_connect_error(const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code& e) = 0;
	virtual void handle_write(ConnectionPtr c) = 0;
protected:
	//void slog_debug(const std::string& where);
	//void slog_debug(const std::string& where, const std::string& what);
	//void slog_debug(const std::string& where, const long& user_id, const std::string& what);
	//void slog_info(const std::string& where, const std::string& what);
	//void slog_notice(const std::string& where, const std::string& what);
	//void slog_warning(const std::string& where, const std::string& what);
	//void slog_error(const std::string& where, const std::string& what);
	//void slog_critical(const std::string& where, const std::string& what);
	//void slog_fatal(const std::string& where, const std::string& what);
protected:
	SessionPtr session_;
	Header header_;
};


