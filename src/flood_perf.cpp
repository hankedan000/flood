#include <stdio.h>

#include "board.hpp"
#include "utils.hpp"

void play(Board &board, const std::string &moves) {
    for (char c : moves) {
        board.flood(c);
    }
}

unsigned long long playAndTime(Board &board, const std::string &moves) {
    board.reset();
    const auto startTime_us = get_time_us();
    play(board,moves);
    return get_time_us() - startTime_us;
}

std::string solveSeq(Board &board) {
    const std::string colors = board.getColors();
    const size_t nColors = colors.length();
    if (nColors == 0) {
        throw std::runtime_error("there are not colors. can't solve");
    }

    board.reset();

    // determine first color by picking the next one in the sequence
    // that isn't the current color.
    const auto &rootCell = board.getCell(0,0);
    size_t colorIdx = 0;
    for (size_t i=0; i<colors.length(); i++) {
        if (colors[i] == rootCell.color) {
            colorIdx = (i + 1) % nColors;
        }
    }

    std::string moves = "";
    while ( ! board.complete()) {
        char floodColor = colors[colorIdx];
        moves.push_back(floodColor);
        board.flood(floodColor);

        // flood with next color in the sequence
        colorIdx = (colorIdx + 1) % nColors;
    }
    return moves;
}

std::string solveRand(Board &board) {
    const std::string colors = board.getColors();
    const size_t nColors = colors.length();
    if (nColors == 0) {
        throw std::runtime_error("there are not colors. can't solve");
    }

    board.reset();

    std::string moves = "";
    while ( ! board.complete()) {
        const auto &rootCell = board.getCell(0,0);

        // randomly determine next flood color that isn't the current
        char nextColor = colors[rand() % nColors];
        while (nextColor == rootCell.color) {
            nextColor = colors[rand() % nColors];
        }

        moves.push_back(nextColor);
        board.flood(nextColor);
    }
    return moves;
}

int main() {
    Board board(8,8,"rgby");
    printf("width: %ld, height: %ld\n", board.getWidth(), board.getHeight());
    
    printf("solving board sequentially...\n");
    const std::string movesSeq = solveSeq(board);
    printf("moves: %ld '%s'\n", movesSeq.length(), movesSeq.c_str());
    printf("solving board randomly...\n");
    const std::string movesRand = solveRand(board);
    printf("moves: %ld '%s'\n", movesRand.length(), movesRand.c_str());

    const std::string moves = movesRand;
    const unsigned int N_ITRS = 100;
    unsigned long long minTime_us = -1;
    unsigned long long maxTime_us = 0;
    unsigned long long sumTime_us = 0;
    for (unsigned int i=0; i<N_ITRS; i++) {
        unsigned long long playTime_us = playAndTime(board, moves);
        minTime_us = std::min(minTime_us, playTime_us);
        maxTime_us = std::max(maxTime_us, playTime_us);
        sumTime_us += playTime_us;
        // printf("%2d: play time %4lldus\n", i, playTime_us);
    }
    printf("min time = %4lldus\n", minTime_us);
    printf("max time = %4lldus\n", maxTime_us);
    printf("avg time = %4lldus\n", sumTime_us / N_ITRS);

    return 0;
}