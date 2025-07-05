#include <iostream>

#include "person.h"

int main()
{
	Person peter("Peter", "Saan", 20);
	Person sebastian("Sebastian", "Pebsen", "Zachrau", 20);

	std::cout << peter.getName() << std::endl;
	std::cout << sebastian.getName() << std::endl;

	return 0;
}
