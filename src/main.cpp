#include <fcntl.h>
#include <print>
#include <string>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

const std::string device = "/dev/ttyUSB0";
const int fd = open(device.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY);
struct termios initConfig, talkConfig;

std::string parseError(const int &code) {
	switch (code) {
		case 0: return "Successful";
		case -1: return "Error opening file";
		case -2: return "Specified file is not a TTY";
		case -3: return "Error getting the configuration of the serial interface";
		case -4: return "Error setting input and output speeds";
		case -5: return "Error setting configuration for the serial interface";
		case -6: return "Error sending init byte";
		case -7: return "Error setting input and output speeds after init";
		case -8: return "Error setting configuration for the serial interface after init";
		case -9: return "Error reading from ECU after init byte was sent";
		default: return "Unknown error";
	}
}

int init() {
	if (fd == -1) return fd;

	if (!isatty(fd)) {
		close(fd);
		return -2;
	}

	if (tcgetattr(fd, &initConfig) < 0) {
		close(fd);
		return -3;
	}

	// 7O1
	initConfig.c_cflag |= PARENB;
	initConfig.c_cflag |= PARODD;
	initConfig.c_cflag &= ~CSTOPB;
	initConfig.c_cflag &= ~CSIZE;
	initConfig.c_cflag |= CS7;

	if (cfsetspeed(&initConfig, 5) < 0) {
		close(fd);
		return -4;
	}

	if (tcsetattr(fd, TCSANOW, &initConfig) < 0) {
		close(fd);
		return -5;
	}

	int initByte[1];
	initByte[0] = 0x01;

	if (write(fd, initByte, 1) == -1) {
		close(fd);
		return -6;
	}

	/*if (cfsetspeed(&initConfig, 10400) < 0) {
		close(fd);
		return -7;
	}

	if (tcsetattr(fd, TCSANOW, &initConfig) < 0) {
		close(fd);
		return -8;
	}*/

	while (true) {
		int initResponseByte[1];
		int initResponse = read(fd, initResponseByte, 1);

		if (initResponse == -1) {
			close(fd);
			return -9;
		}

		std::println("Response: {} with a size of {} bytes", initResponseByte[0], initResponse);
		usleep(200000);
	}

	return 0;
}

int talk() {
	return 0;
}

int main() {
	const int initStatus = init();
	std::println("init() exited: {}. ({})", parseError(initStatus), initStatus);

	if (initStatus == 0) {
		const int talkStatus = talk();
		std::println("talk() exited: {}. ({})", parseError(talkStatus), talkStatus);
	}

	return 0;
}
