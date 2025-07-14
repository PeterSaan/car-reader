#include <print>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

std::string parseError(const int &code) {
	switch (code) {
		case 0: return "";
		case -1: return "Couldn't open file";
		case -2: return "Specified file is not a TTY";
		case -3: return "Couldn't get the configuration of the serial interface";
		default: return "Unknown error";
	}
}

int connect() {
	struct termios tconfig;
	const std::string device = "/dev/ttyUSB0";
	const int fd = open(device.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY);

	if (fd == -1) return fd;

	if (!isatty(fd)) {
		close(fd);
		return -2;
	}

	if (tcgetattr(fd, &config) < 0) {
		close(fd);
		return -3;
	}

	config

	close(fd);
	return 0;
}

int main() {
	const int connectStatus = connect();

	while (!connectStatus) {
		std::println("Connected to serial device");
	}

	if (connectStatus != 0) {
		std::println("connect() exited with error: {}", parseError(connectStatus));
	}

	std::println("Compilation successful!");
	return 0;
}
