#include <stdio.h>

#include "board.hpp"
#include "utils.hpp"

int main() {
    Board board(8,8,"rgby");

    std::string moves = "";
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
            moves.push_back(flood_c);
        }
    }
    board.print();
    printf("You win!\n");
    printf("Moves: '%s' (%ld steps)\n", moves.c_str(), moves.length());
    return 0;
}