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
#include <signal.h>

#define perr_exit(msg)                                                         \
	do {                                                                   \
		perror(msg);                                                   \
		exit(EXIT_FAILURE);                                            \
	} while (0)

#define LED1 "/sys/class/leds/beaglebone:green:heartbeat"
#define LED2 "/sys/class/leds/beaglebone:green:mmc0"
#define LED3 "/sys/class/leds/beaglebone:green:usr2"
#define LED4 "/sys/class/leds/beaglebone:green:usr3"

#define LED1_MASK 0b0001
#define LED2_MASK 0b0010
#define LED3_MASK 0b0100
#define LED4_MASK 0b1000

#define TRIGGER "/trigger"
#define BACKLIGHT "/brightness" //typo sorry
#define DEL_ON "/delay_on"
#define DEL_OFF "/delay_off"

#define TIMER "timer"
#define ON "1"
#define OFF "0"

#define LED_FILE(LED, FILE) LED FILE

void int_to_led(int nb);

static void handler(int sig, siginfo_t *si, void *uc)
{
	static int count = 0;

	count++;
	int_to_led(count);
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

void int_to_led(int nb)
{
	turn_backlight(LED_FILE(LED1, BACKLIGHT), nb & LED1_MASK);
	turn_backlight(LED_FILE(LED2, BACKLIGHT), nb & LED2_MASK);
	turn_backlight(LED_FILE(LED3, BACKLIGHT), nb & LED3_MASK);
	turn_backlight(LED_FILE(LED4, BACKLIGHT), nb & LED4_MASK);
}

int main(void)
{
	printf("counter program pid : %d\n", getpid());

	struct sigaction sa;
	sa.sa_flags	= SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		perr_exit("sigaction");

	turn_backlight(LED_FILE(LED1, BACKLIGHT), false);
	turn_backlight(LED_FILE(LED2, BACKLIGHT), false);
	turn_backlight(LED_FILE(LED3, BACKLIGHT), false);
	turn_backlight(LED_FILE(LED4, BACKLIGHT), false);

	while (true) {
		/*The pause() function shall suspend the calling thread until delivery  of  a
       	signal  whose  action is either to execute a signal-catching function or to
       	terminate the process.*/
		pause();
	}

	return EXIT_SUCCESS;
}