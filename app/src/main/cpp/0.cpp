#include <iostream>

#include "str.h"
#include "net.h"

int main() {
	auto http = Net::Http("music.badboy.institute");
	auto r = http.get("/rest/ping.view", "");
	std::cout << (*r).status << std::endl;
}
