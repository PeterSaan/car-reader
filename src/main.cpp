#include <print>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

void read()
{
	const char device[] = "/dev/random";
	int fd = open(device, O_RDONLY | O_NONBLOCK | O_NOCTTY);
	if (fd == -1) {
		std::println("Error: Couldn't open file {}", device);
		return;
	}

	if (!isatty(fd)) {
		std::println("Error: {} is not a TTY", device);
		close(fd);
		return;
	}

	close(fd);
}

int main()
{
	read();
	std::println("Compilation successful!");

	return 0;
}
