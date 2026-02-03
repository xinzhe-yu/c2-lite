
#include "terminal.h"


#include <termios.h>
#include <unistd.h>

struct termios saved_termios;

void set_raw_mode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &saved_termios);
    raw = saved_termios;
    
    // Disable canonical mode and echo
    raw.c_lflag &= ~(ECHO | ICANON);
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_termios);
}















