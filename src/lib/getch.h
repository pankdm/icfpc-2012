#pragma once

#ifdef unix
#include <termios.h>
#include <unistd.h>
inline int getch()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt= oldt;
	newt.c_lflag &= ~(ICANON| ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch= getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#else
#include <conio.h>
inline int getch() {
    return _getch();
}
#endif
