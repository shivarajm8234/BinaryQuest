#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios original_termios;
static int terminal_initialized = 0;

// Linux implementation
int inputGet(){
    static struct termios new_termios;
    
    if (!terminal_initialized) {
        tcgetattr(STDIN_FILENO, &original_termios);
        new_termios = original_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
        terminal_initialized = 1;
    }
    
    int ch = getchar();
    if (ch == EOF) {
        return -1;  // No key pressed
    }
    return ch;
}

void inputCleanup(){
    if (terminal_initialized) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
        fcntl(STDIN_FILENO, F_SETFL, 0);
        terminal_initialized = 0;
    }
}