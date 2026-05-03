#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios oldt;

void reset_terminal(){
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void init_terminal(){
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}