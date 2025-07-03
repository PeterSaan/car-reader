#include <iostream>
#include "person.h"

int main()
{
	std::string ans = std::format("Hello, {}! This is my {}. time writing this.", "Peter", 1);

	std::cout << ans << std::endl;

	return 0;
}
