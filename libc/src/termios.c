#include <stdio.h>
#include <termios.h>

int tcgetattr(int filedes, struct termios *termios_p) {
	(void)filedes;
	(void)termios_p;
	puts("tcgetattr: not implemented");
	return -1;
}

int tcsetattr(int filedes, int optional_actions, const struct termios *termios_p) {
	(void)filedes;
	(void)optional_actions;
	(void)termios_p;
	puts("tcgetattr: not implemented");
	return -1;
}
