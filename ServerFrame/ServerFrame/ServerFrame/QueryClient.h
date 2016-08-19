#pragma once

#include "stdafx.h"

#include <vector>
#include <queue>

#include "Session.h"
#include "Connection.h"


class QueryClient
{
	/************************************************************************/
	/* 
	QueryClient是链接其他服务器的基类。
	*/
	/************************************************************************/
public:
	typedef std::vector<char> buffer_type;
	struct Callback
	{
		Callback(Connection* c, const Header& hdr, buffer_type& b) 
			: connection(c), header(hdr)
		{
			buffer.swap(b);
		}
		Connection* connection;
		Header header;
		buffer_type buffer;
	};
	typedef std::queue<ConnectionPtr> PoolQueue;
public:
	QueryClient(boost::asio::io_service& io_service);
	~QueryClient();
	void set_endpoint(const boost::asio::ip::tcp::endpoint& endpoint);
	void do_query(SessionPtr s, const Header& hdr, buffer_type& buffer);
	void release(ConnectionPtr c);
private:
	void handle_connect(Callback* cb, SessionPtr s, const boost::system::error_code& err);
	//void handle_release(ConnectionPtr c);
	//void handle_close(ConnectionPtr c);
	ConnectionPtr get_connection();
private:
	ConnectionManager manager_;
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::endpoint endpoint_;
	PoolQueue ready_;
	boost::mutex mutex_;
};
