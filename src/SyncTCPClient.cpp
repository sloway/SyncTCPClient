#include <iostream>
#include <boost/asio.hpp>

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main() {
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);
	boost::system::error_code connect_error;
	boost::asio::ip::tcp::socket socket(io_service);
	socket.connect(endpoint, connect_error);

	if(connect_error) {
		std::cout << " * Connection fail. error no : " << connect_error.value() << ", message : " << connect_error.message() << std::endl;
	} else {
		std::cout << " * Connected" << std::endl;
	}

	for(int i=0 ; i<7 ; ++i) {
		char szMessage[128] = {0,};
		snprintf(szMessage, 128-1, "%d - Send Message", i);
		int nMsgLen = strnlen(szMessage, 128-1);

		boost::system::error_code ignored_error;
		socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error);

		std::cout << " * Msg to server : " << szMessage << std::endl;

		std::array<char, 128> buf;
		buf.fill(0);
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if(error) {
			if(error == boost::asio::error::eof) {
				std::cout << " * Disconnected" << std::endl;
			} else {
				std::cout << " * error no : " << error.value() << "error messgae : " << error.message().c_str() << std::endl;
			}
			break;
		}

		std::cout << " * msg from server : " <<  &buf[0] << std::endl;
	}

	if(socket.is_open()) {
		socket.close();
	}

	// This is the test branch change.
	getchar();
	return 0;

}
