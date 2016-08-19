#pragma once

#include "stdafx.h"
//#include "Session.h"
#include "SocketDate.h"

class Session;
typedef boost::shared_ptr<Session> SessionPtr;
class Connection;
class ConnectionManager;
typedef boost::shared_ptr<Connection> ConnectionPtr;

class Connection:public boost::enable_shared_from_this<Connection>,
	private boost::noncopyable

{
public:
	//Connection(void);
	~Connection(void);
public:
	typedef std::vector<char> buffer_type;
	typedef boost::function<void (ConnectionPtr)> handle_type;
	enum{
		BEGIN       = 0x00,
		CONNECTING  ,
		CONNECTED   ,
		CLOSING     = 0xF0,
		CLOSED      = 0xFF
	};
public:
	/// Constructor.
	Connection(boost::asio::io_service& io_service, ConnectionManager& manager, SessionPtr handler);
	Connection(boost::asio::io_service& io_service, ConnectionManager& manager);
	
	inline boost::asio::ip::tcp::socket& socket(){ return socket_; }
	inline void set_handler(SessionPtr handler) { handler_ = handler; }
	inline void set_handler(SessionPtr handler, handle_type on_release) { 
		handler_ = handler; 
		release_handler_ = on_release;
	}
	inline void set_handler(SessionPtr handler, handle_type on_release, handle_type on_close) { 
		handler_ = handler; 
		release_handler_ = on_release;
		close_handler_ = on_close;
	}
	inline SessionPtr get_handler() const { return handler_; }
	inline void reset_handler() { handler_.reset(); }

	void close();
	void release();
	void start();
	void stop();

	void send_packet(const Header& header, const buffer_type& data);//发送数据
	void send_packet(const Header& header);

public:
	void handle_read_header(const boost::system::error_code& e, size_t length);//读socket头
	void handle_read_data(const boost::system::error_code& e, size_t length);//读socket内容
	void handle_connect();
	void handle_error(const boost::system::error_code& e);
	void handle_error(const char* where, const boost::system::error_code& e);
private:
	void async_read_header();
	void async_read_data();
	void sync_read_data();
	void async_write();
	void handle_write(const boost::system::error_code& e);

	bool is_valid() const { return (status_ < 0x80);}//连接是否可用
	inline void do_load(const char* buffer, size_t size, Header& header)
	{
		memcpy(&header,buffer,size);
	}
private:
	boost::asio::ip::tcp::socket socket_;  //这里封装一个boost socket 
	int status_;
	ConnectionManager& manager_; //添加一个 连接管理的引用
	SessionPtr handler_; //一个会话的指针
	handle_type release_handler_;
	handle_type close_handler_;

	/// The size of a fixed length header.
	enum { header_length = sizeof(Header) };

	/// Holds an outbound.
	buffer_type outbound_header_;	//typedef std::vector<char> buffer_type;
	buffer_type outbound_data_;

	/// Holds an inbound.
	Header header_;
	char inbound_header_[header_length];
	buffer_type inbound_data_;
};

class ConnectionManager : private boost::noncopyable{
public:
	/// Add the specified connection to the manager and start it.
	void start(ConnectionPtr c);

	/// Stop the specified connection.
	void stop(ConnectionPtr c);

	/// Stop all connections.
	void stop_all();

	void add(ConnectionPtr c);
	void erase(ConnectionPtr c);


	size_t size() const;
	bool exists(ConnectionPtr c);
	bool exists(ConnectionPtr c) const;
private:
	/// The managed connections.
	std::set<ConnectionPtr> connections_;
	boost::mutex mutex_;
};

//typedef boost::shared_ptr<Session> SessionPtr;


