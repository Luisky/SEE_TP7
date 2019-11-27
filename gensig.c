#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>

int main(int argc, char **argv)
{
	int pid;
	int period;

	if (argc != 3)
		errx(EXIT_FAILURE, "./%s [period in seconds] [pid]", argv[0]);

	//TODO: check the result of atoi
	period = atoi(argv[1]);
	pid    = atoi(argv[2]);

	while (true) {
		kill(pid, SIGUSR1);
		sleep(period);
	}

	return EXIT_SUCCESS;
}