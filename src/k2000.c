#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define perr_exit(msg)                                                         \
	do {                                                                   \
		perror(msg);                                                   \
		exit(EXIT_FAILURE);                                            \
	} while (0)

#define LED1 "/sys/class/leds/beaglebone:green:heartbeat"
#define LED2 "/sys/class/leds/beaglebone:green:mmc0"
#define LED3 "/sys/class/leds/beaglebone:green:usr2"
#define LED4 "/sys/class/leds/beaglebone:green:usr3"

#define TRIGGER "/trigger"
#define BACKLIGHT "/brightness" //typo sorry
#define DEL_ON "/delay_on"
#define DEL_OFF "/delay_off"

#define TIMER "timer"
#define ON "1"
#define OFF "0"

#define LED_FILE(LED, FILE) LED FILE

void trigger_timer(char *filename)
{
	int fd;

	if ((fd = open(filename, O_WRONLY)) == -1)
		perr_exit("open");

	// sizeof(TIMER) will return 6 for write
	if (write(fd, TIMER, sizeof(TIMER)) != sizeof(TIMER))
		perr_exit("write");

	if (close(fd) == -1)
		perr_exit("close");
}

void turn_backlight(char *filename, bool state)
{
	int fd;

	if ((fd = open(filename, O_WRONLY)) == -1)
		perr_exit("open");

	if (state) {
		if (write(fd, ON, sizeof(ON)) != sizeof(ON))
			perr_exit("write");
	} else {
		if (write(fd, OFF, sizeof(OFF)) != sizeof(OFF))
			perr_exit("write");
	}

	if (close(fd) == -1)
		perr_exit("close");
}

int main(int argc, char **argv)
{
	int fd;
	int speed;

	if (argc != 2)
		errx(EXIT_FAILURE, "%s [speed in ms (1 to 999)]\n", argv[0]);

	speed = atoi(argv[1]);
	if ((speed > 800) || (speed < 50))
		errx(EXIT_FAILURE, "Please give a speed between 50 and 800");
	else
		printf("speed chosen : %d ms\n", speed);
	speed *= 1000;

	// Timings in delay_off & delay_on are expressed in milliseconds
	// usleep is in microsecond so : * 1000
	while (true) {
		turn_backlight(LED_FILE(LED2, BACKLIGHT), true);
		turn_backlight(LED_FILE(LED1, BACKLIGHT), false);
		usleep(speed);
		turn_backlight(LED_FILE(LED3, BACKLIGHT), true);
		turn_backlight(LED_FILE(LED2, BACKLIGHT), false);
		usleep(speed);
		turn_backlight(LED_FILE(LED4, BACKLIGHT), true);
		turn_backlight(LED_FILE(LED3, BACKLIGHT), false);
		usleep(speed);
		turn_backlight(LED_FILE(LED3, BACKLIGHT), true);
		turn_backlight(LED_FILE(LED4, BACKLIGHT), false);
		usleep(speed);
		turn_backlight(LED_FILE(LED2, BACKLIGHT), true);
		turn_backlight(LED_FILE(LED3, BACKLIGHT), false);
		usleep(speed);
		turn_backlight(LED_FILE(LED1, BACKLIGHT), true);
		turn_backlight(LED_FILE(LED2, BACKLIGHT), false);
		usleep(speed);
	}

	return EXIT_SUCCESS;
}
