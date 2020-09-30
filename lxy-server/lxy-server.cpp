// lxy-server.cpp: 定义应用程序的入口点。
//

#include "lxy-server.h"
#include "boost/bind/bind.hpp"
#include "boost/thread/thread.hpp"
#include "boost/filesystem.hpp"
#include "boost/asio.hpp"

using namespace std;
using namespace boost;

void print(const system::error_code& /*e*/)
{
	std::cout << "Hello, asio!" << std::endl;
}

void print2(const system::error_code&, asio::steady_timer* t, int* count) {
	if (*count < 10) {
		std::cout << *count << std::endl;
		++(*count);

		t->expires_at(t->expiry() + asio::chrono::milliseconds(500));
		t->async_wait(bind(print2, asio::placeholders::error, t, count));
	}
}


class printer {
	asio::steady_timer timer_;
	int count_ = 0;
public:
	printer(asio::io_context& io) : timer_(io, asio::chrono::seconds(1)), count_(0) {
		timer_.async_wait(boost::bind(&printer::print, this));
	}
	~printer() {
		std::cout << "Final count is " << count_ << std::endl;
	}

	void print() {
		if (count_ < 5) {
			std::cout << "c:" << count_ << std::endl;
			++count_;

			timer_.expires_at(timer_.expiry() + asio::chrono::seconds(1));
			timer_.async_wait(boost::bind(&printer::print, this));
		}
	}
};

class printer2 {
private:
private:
	asio::strand<boost::asio::io_context::executor_type> strand_;
	asio::steady_timer timer1_;
	asio::steady_timer timer2_;
	int count_;
public:
	printer2(asio::io_context& io):strand_(asio::make_strand(io)),
		timer1_(io, asio::chrono::seconds(1)),
		timer2_(io, asio::chrono::seconds(1)) ,
		count_(0)
	{

		timer1_.async_wait(asio::bind_executor(strand_, boost::bind(&printer2::print1, this)));
		timer2_.async_wait(asio::bind_executor(strand_, boost::bind(&printer2::print2, this)));
	}

	~printer2() {
		std::cout << "Final count is " << count_ << std::endl;
	}

	void print1() {
		if (count_ < 10)
		{
			std::cout << "Timer 1: " << count_ << std::endl;
			++count_;

			timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

			timer1_.async_wait(asio::bind_executor(strand_,
				boost::bind(&printer2::print1, this)));
		}
	}
	void print2() {
		if (count_ < 10)
		{
			std::cout << "Timer 2: " << count_ << std::endl;
			++count_;

			timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

			timer2_.async_wait(asio::bind_executor(strand_,
				boost::bind(&printer2::print2, this)));
		}
	}
	
};

int main(int argc, char* argv[])
{
	std::cout << "文件大小: " << filesystem::file_size("D:\\local\\boost_1_74_0\\lib64-msvc-14.2\\boost_atomic-vc142-mt-gd-x64-1_74.dll") << '\n';

	{
		int count = 0;
		asio::io_context io;
		asio::steady_timer t(io, asio::chrono::seconds(1));
		t.async_wait(bind(print2, asio::placeholders::error, &t, &count));

		printer p(io);
		io.run();
	}
	{
		boost::asio::io_context io;
		printer2 p(io);
		boost::thread t(boost::bind(&asio::io_context::run, &io));
		boost::thread t2(boost::bind(&asio::io_context::run, &io));
		io.run();
		t.join();
	}

	return 0;
}

