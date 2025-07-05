#include <optional>
#include <string>

#include "person.h"

Person::Person(std::string first_n, std::string last_n, char8_t age) {
	id++;
	this->first_name = first_n;
	this->last_name = last_n;
	this->age = age;
}

Person::Person(std::string first_n, std::string middle_n, std::string last_n, char8_t age) {
	id++;
	this->first_name = first_n;
	this->middle_name = middle_n;
	this->last_name = last_n;
	this->age = age;
}

uint Person::getId() const {
	return id;
}

std::string Person::getFirstName() const {
	return first_name;
}

std::string Person::getMiddleName() const {
	if (middle_name.has_value()) return "";
	return middle_name.value();
}

std::string Person::getLastName() const {
	return last_name;
}

char8_t Person::getAge() const {
	return age;
}

std::string Person::getName() const {
	if (!middle_name.has_value()) {
		return first_name + " " + last_name;
	}
	return first_name + " " + middle_name.value() + " " + last_name;
}
