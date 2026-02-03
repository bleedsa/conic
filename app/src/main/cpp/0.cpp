#include <iostream>

#include "str.h"
#include "net.h"

int main() {
	try {
		auto http = Net::Http("music.badboy.institute");
		auto r = http.get("/rest/ping.view", "");
		auto c = (*r).c_str();
		std::cout << c << std::endl;
		delete[] c;
	} catch (std::string &e) {
		std::cerr << e << std::endl;
	}
}
