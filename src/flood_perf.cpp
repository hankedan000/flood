#include <stdio.h>

#include "board.hpp"
#include "solvers.hpp"
#include "utils.hpp"

// method that i use to benchmark performance of the Board::flood() method
void benchmarkFloodPerf() {
    printf("======= BENCHMARKING FLOOD PERFORMANCE =======\n");
    srand(1000);// seed rng so we get same board each time
    Board board(100,100,"rgby");
    if (false) {
        // what i used to generate `moves` below
        printf("solving board randomly...\n");
        const std::string movesRand = solveRandBestN(board, 10);
        printf("moves: %ld '%s'\n", movesRand.length(), movesRand.c_str());
    }

    const std::string moves = "yrygyrbrbryrgrgbyrbybgygbyrbyrbybybyrgbgrbyrybrgbrgrygbyrgbgbgbybyryrbgygrybgbryrgryryrbgrybrygygbgyrybygyrybygrgybgbgbgrbyrgbry";
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
}

int main() {
    Board board(100,100,"rgby");
    printf("width: %ld, height: %ld\n", board.getWidth(), board.getHeight());

    {
        printf("======== solveSeq() ========\n");
        const unsigned long long startTime_us = get_time_us();
        const std::string moves = solveSeq(board);
        const float solveTime_ms = (get_time_us() - startTime_us) / 1000.0;
        printf("solve time: %.3fms\n", solveTime_ms);
        printf("moves: %ld '%s'\n", moves.length(), moves.c_str());
    }
    {
        const int N_SOLVES = 100;
        printf("======== solveRandBestN(%d) ========\n", N_SOLVES);
        const unsigned long long startTime_us = get_time_us();
        const std::string moves = solveRandBestN(board, N_SOLVES);
        const float solveTime_ms = (get_time_us() - startTime_us) / 1000.0;
        printf("solve time: %.3fms\n", solveTime_ms);
        printf("moves: %ld '%s'\n", moves.length(), moves.c_str());
    }
    {
        printf("======== solveMostCaptures() ========\n");
        const unsigned long long startTime_us = get_time_us();
        const std::string moves = solveMostCaptures(board);
        const float solveTime_ms = (get_time_us() - startTime_us) / 1000.0;
        printf("solve time: %.3fms\n", solveTime_ms);
        printf("moves: %ld '%s'\n", moves.length(), moves.c_str());
    }

    // benchmarkFloodPerf();

    return 0;
}