#ifndef PERSON_H
#define PERSON_H

#include <optional>
#include <string>
#include <sys/types.h>

class Person {
	uint id = 0;
	std::string first_name;
	std::optional<std::string> middle_name;
	std::string last_name;
	char8_t age;

public:
	Person(std::string, std::string, char8_t);
	Person(std::string, std::string, std::string, char8_t);

	uint getId() const;

	std::string getFirstName() const;

	std::string getMiddleName() const;

	std::string getLastName() const;

	char8_t getAge() const;

	std::string getName() const;
};
#endif // !PERSON_H
