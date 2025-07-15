#include <print>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

std::string parseError(const int &code) {
	switch (code) {
		case 0: return "Successful";
		case -1: return "Error opening file";
		case -2: return "Specified file is not a TTY";
		case -3: return "Error getting the configuration of the serial interface";
		case -4: return "Error setting input and output speeds";
		case -5: return "Error setting new configuration for the serial interface";
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

	if (tcgetattr(fd, &tconfig) < 0) {
		close(fd);
		return -3;
	}

	tconfig.c_iflag &= ~(INPCK | ISTRIP | IXON | BRKINT);
	tconfig.c_oflag &= OPOST;
	tconfig.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
	tconfig.c_cflag &= ~(CSIZE | PARENB);
	tconfig.c_cflag |= CS8;
	tconfig.c_cc[VMIN] = 1;
	tconfig.c_cc[VTIME] = 0;

	if (cfsetspeed(&tconfig, B9600) < 0) {
		close(fd);
		return -4;
	}

	if (tcsetattr(fd, TCSANOW, &tconfig) < 0) {
		close(fd);
		return -5;
	}

	close(fd);
	return 0;
}

int main() {
	const int connectStatus = connect();

	std::println("connect() exited: {}", parseError(connectStatus));

	return 0;
}
