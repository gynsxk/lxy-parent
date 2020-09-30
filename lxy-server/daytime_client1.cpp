#include "lxy-server.h"

#include "boost/asio.hpp"
#include "boost/array.hpp"

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;


int main() {
	try{
		std::string dst = "127.0.0.1";
		asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints = resolver.resolve(dst, "daytime");

		tcp::socket socket(io_context);
		auto itr = endpoints.cbegin();
		while (itr != endpoints.cend()) {
			std::cout << itr->host_name() <<":" << itr->service_name() << std::endl;
			itr++;
		}

		asio::connect(socket, endpoints);

		for (;;) {
			boost::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(asio::buffer(buf), error);
			if (error == asio::error::eof) {
				std::cout << "eof"<< std::endl;
				break;
			}
			else if (error) {
				std::cout << "exception" << error << std::endl;
				throw boost::system::system_error(error);
			}

			std::cout << len << std::endl;
			std::cout.write(buf.data(), len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}