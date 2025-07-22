#include <asm-generic/termbits.h>
#include <fcntl.h>
#include <print>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

const std::string device = "/dev/ttyUSB0";
const int fd = open(device.c_str(), O_RDWR | O_DSYNC | O_NOCTTY | O_NONBLOCK | O_NOFOLLOW);
struct termios2 initConfig, talkConfig;

std::string parseError(const int &code) {
	switch (code) {
		case 0: return "Successful";
		case -1: return "Error opening file";
		case -2: return "Specified file is not a TTY";
		case -3: return "Error getting the configuration of the serial interface";
		case -4: return "Error setting input and output speeds to 5 baud";
		case -5: return "Error setting input and output speeds to 10400 baud";
		case -6: return "Error setting configuration for the serial interface";
		case -7: return "Error sending init byte";
		case -8: return "Error setting configuration for the serial interface after init";
		case -9: return "Read data after init was of size 0";
		case -10: return "Error reading from ECU after init byte was sent";
		default: return "Unknown error";
	}
}

int init() {
	if (fd == -1) return fd;
	std::println("Opened device");

	if (!isatty(fd)) {
		close(fd);
		return -2;
	}
	std::println("Confirmed it's a TTY");

	ioctl(fd, TCFLSH, TCIOFLUSH);
	
	if (ioctl(fd, TCGETS2, &initConfig) < 0 && ioctl(fd, TCGETS2, &talkConfig) < 0) {
		close(fd);
		return -3;
	}
	std::println("Got it's attributes for termios configs");

	talkConfig.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CBAUD);
	talkConfig.c_cflag |= CS8;
	talkConfig.c_cflag |= BOTHER;
	talkConfig.c_iflag &= ~(IXON | IXOFF | ICRNL);
	talkConfig.c_lflag &= ~(ECHO | ICANON);
	talkConfig.c_oflag &= ~(ONLCR | OPOST);
	talkConfig.c_ispeed = 10400;
	talkConfig.c_ospeed = 10400;

	std::println("Finished talking config");

	initConfig.c_cflag &= ~(CSTOPB | CSIZE | CBAUD);
	initConfig.c_cflag |= PARENB;
	initConfig.c_cflag |= PARODD;
	initConfig.c_cflag |= BOTHER;
	initConfig.c_cflag |= CS7;
	initConfig.c_ispeed = 5;
	initConfig.c_ospeed = 5;

	if (ioctl(fd, TCSETS2, &initConfig) < 0) {
		close(fd);
		return -4;
	}
	std::println("Finished and set init config");

	int initByte[1];
	initByte[0] = 0x01;

	if (write(fd, initByte, 1) == -1) {
		close(fd);
		return -7;
	}
	std::println("Wrote 0x01 to the ECU");

	if (ioctl(fd, TCSETS2, &talkConfig) < 0) {
		close(fd);
		return -5;
	}
	std::println("Switched TTY's config to talkConfig");

	usleep(50000);

	int responseByte[1];
	int initResponse = read(fd, responseByte, 1);

	if (initResponse == 0) {
		close(fd);
		return -9;
	}

	if (initResponse == -1) {
		close(fd);
		return -10;
	}

	std::println("\nInit response: {:x} with a size of {} bytes", responseByte[0], initResponse);

	//while(responseByte[0] != 0x55) {
	//	usleep(500000);
	//	read(fd, responseByte, 1);
	//	std::println("Read didn't get 0x55. Instead got {}", responseByte[0]);
	//}
	ioctl(fd, TCFLSH, TCIOFLUSH);

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
