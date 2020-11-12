#include "server.h"

#include "request.h"
#include "../Frontend/GUI/gui.h"
#include "statics.h"
//https://www.boost.org/doc/libs/1_70_0/libs/beast/example/http/server/fast/http_server_fast.cpp

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
	http_connection(tcp::socket socket)
		: socket_(std::move(socket))
	{
	}

	// Initiate the asynchronous operations associated with the connection.
	void
		start()
	{
		read_request();
		check_deadline();
	}

private:
	// The socket for the currently connected client.
	tcp::socket socket_;

	// The buffer for performing reads.
	beast::flat_buffer buffer_{ 8192 };

	// The request message.
	http::request<http::string_body> req;

	// The response message.
	http::response<http::dynamic_body> res;

	// The timer for putting a deadline on connection processing.
	net::basic_waitable_timer<std::chrono::steady_clock> deadline_{
		socket_.get_executor(), std::chrono::seconds(60)
	};

	// Asynchronously receive a complete request message.
	void
		read_request()
	{
		auto self = shared_from_this();

		http::async_read(
			socket_,
			buffer_,
			req,
			[self](beast::error_code ec,
				std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);
				if (!ec) self->process_request();
			});
	}

	// Determine what needs to be done with the request message.
	void
		process_request()
	{
		res.version(req.version());
		res.keep_alive(false);
		res.result(http::status::ok);
		res.set(http::field::server, "Neonite++");
		res = handle_request(req, res);
		write_response();
	}

	// Asynchronously transmit the response message.
	void
		write_response()
	{
		auto self = shared_from_this();

		res.set(http::field::content_length, res.body().size());

		http::async_write(
			socket_,
			res,
			[self](beast::error_code ec, std::size_t)
			{
				self->socket_.shutdown(tcp::socket::shutdown_send, ec);
				self->deadline_.cancel();
			});
	}

	// Check whether we have spent enough time on this connection.
	void
		check_deadline()
	{
		auto self = shared_from_this();

		deadline_.async_wait(
			[self](beast::error_code ec)
			{
				if (!ec)
				{
					// Close socket to cancel any outstanding operation.
					self->socket_.close(ec);
				}
			});
	}
};

// "Loop" forever accepting new connections.
void http_server(tcp::acceptor& acceptor, tcp::socket& socket)
{
	acceptor.async_accept(socket,
		[&](beast::error_code ec)
		{
			if (!ec)
				std::make_shared<http_connection>(std::move(socket))
				->start();
			http_server(acceptor, socket);
		});
}

void server()
{
	try
	{
		auto const address = net::ip::make_address(ip);

		net::io_context ioc{ 1 };
		tcp::acceptor acceptor{ ioc, {address, port} };
		tcp::socket socket{ ioc };

		http_server(acceptor, socket);
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&registerStatics,
			nullptr, NULL, nullptr);

		console.AddLog("[=]Neonite server is listening on port %i", port);
		ioc.run();
	}
	catch (std::exception const& e)
	{
		std::string error = e.what();
		error += "\nPlease screenshot this and send it in the server for support.";
		MessageBoxA(nullptr, error.c_str(), "Neonite++", ERROR);
	}
}