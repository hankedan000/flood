#include <stdio.h>

#include "board.hpp"
#include "utils.hpp"

int main() {
    Board board(8,8,"rgby");

    unsigned int steps = 0;
    while ( ! board.complete()) {
        board.print();
        printf("Enter color (q to quit): ");
        char flood_c = getch();
        printf("%c\n", flood_c);
        if (flood_c == 'q') {
            printf("good bye!\n");
            exit(0);
        }
        if (board.flood(flood_c) > 0) {
            steps++;
        }
    }
    board.print();
    printf("You win!\n");
    printf("It took you %d steps to flood the board.\n", steps);
    return 0;
}