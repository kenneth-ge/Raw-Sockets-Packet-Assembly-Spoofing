#ifdef assemble_udp

#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include <exception>

#include <boost/asio.hpp>

#include "udp_header.h"
#include "raw.h"

int main() {
	udp_header udph;
	udph.source_port(200);
	udph.destination_port(5000);
	udph.3(11);
	udph.checksum(0x00); //optional for IPv4

	udph.to_string();

	std::string body("---"); //create a small data payload

	boost::asio::io_service io_service;

	boost::asio::basic_raw_socket<asio::ip::raw> socket_(io_service);

	boost::asio::streambuf request_buffer;
	std::ostream os(&request_buffer);
	os << udph << body;

	boost::asio::ip::address_v4::bytes_type b = { {127, 0, 0, 1} };

	asio::ip::raw::endpoint ep(boost::asio::ip::address_v4(b), 5000);

	try {
		socket_.open();
		socket_.send_to(request_buffer.data(), ep);
		std::cout << "Sent!" << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
#endif