#include <optional>
#include <string>

#include "person.h"

Person::Person(
	std::string &first_n,
	std::string &last_n,
	char8_t &age
) : first_name(first_n), last_name(last_n), age(age) {}

Person::Person(
	std::string &first_n,
	std::string &middle_n,
	std::string &last_n,
	char8_t &age
) : first_name(first_n), middle_name(middle_n), last_name(last_n), age(age) {}

uint Person::getId(Person &p) {
	return p.id;
}

std::string Person::getFirstName(Person &p) {
	return p.first_name;
}

std::string Person::getMiddleName(Person &p) {
	if (!p.middle_name.has_value()) return "";
	return p.middle_name.value();
}

std::string Person::getLastName(Person &p) {
	return p.last_name;
}

char8_t Person::getAge(Person &p) {
	return p.age;
}
