#pragma once

#include <fstream>
#include <stdexcept>
#include <termios.h>
#include <time.h>
#include <unistd.h>

char getch() {
    struct termios tty_opts_backup, tty_opts_raw;

    if ( ! isatty(STDIN_FILENO)) {
      std::runtime_error("Error: stdin is not a TTY");
    }

    // Back up current TTY settings
    tcgetattr(STDIN_FILENO, &tty_opts_backup);

    // Change TTY settings to raw mode
    cfmakeraw(&tty_opts_raw);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty_opts_raw);

    // Read a char from stdin
    char c = getchar();

    // Restore previous TTY settings
    tcsetattr(STDIN_FILENO, TCSANOW, &tty_opts_backup);

    return c;
}

unsigned long long get_time_us() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}
