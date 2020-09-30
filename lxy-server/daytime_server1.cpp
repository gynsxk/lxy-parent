#include "lxy-server.h"

#include <ctime>
#include <string>
#include "boost/asio.hpp"
#include "boost/array.hpp"

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

std::string make_daytime_string() {
	time_t now = time(0);
	return ctime(&now);
}

int main() {
	
	try {
		asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 18888));

		for (;;) {
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			for (;;) {
				boost::array<char, 256> buf;
				boost::system::error_code error;

				size_t len = socket.read_some(asio::buffer(buf), error);
				if (error == asio::error::eof) {
					std::cout << "eof" << std::endl;
					break;
				}
				else if (error) {
					std::cout << "exception" << error << std::endl;
					throw boost::system::system_error(error);
				}

				//std::cout << len << std::endl;
				std::cout.write(buf.data(), len);
			}

			std::cout << "接到链接请求" << std::endl;
			std::string message = make_daytime_string();
			std::cout << message << std::endl;
			system::error_code ignored_error;
			asio::write(socket, asio::buffer(message), ignored_error);
			std::cout << ignored_error << "end" << std::endl;
		}
	}catch (std::exception& e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}