#include <fstream>
#include <iostream>

#include "board.hpp"
#include "solvers.hpp"
#include "utils.hpp"

int main() {
    const size_t DIM = 20;
    const size_t N_BOARDS = 100;

    for (size_t bb=0; bb<N_BOARDS; bb++) {
        printf("%4ld: ", bb);
        Board board(DIM,DIM,"rgby");
        std::string bestMoves = "";
        std::string bestAlg = "";
        {
            const std::string moves = solveMostCaptures(board);
            printf("movesMostCap = %3ld; ", moves.length());
            board.reset();
            if (bestMoves.empty() || moves.length() < bestMoves.length()) {
                bestMoves = moves;
                bestAlg = "solveMostCaptures";
            }
        }
        {
            const std::string moves = solveSeq(board);
            printf("movesSeq = %3ld; ", moves.length());
            board.reset();
            if (bestMoves.empty() || moves.length() < bestMoves.length()) {
                bestMoves = moves;
                bestAlg = "solveSeq";
            }
        }
        {
            const std::string moves = solveRandBestN(board,20);
            printf("movesRandBestN = %3ld; ", moves.length());
            board.reset();
            if (bestMoves.empty() || moves.length() < bestMoves.length()) {
                bestMoves = moves;
                bestAlg = "solveRandBestN";
            }
        }

        printf("bestMoves = %3ld; bestAlg = '%s';\n", bestMoves.length(), bestAlg.c_str());
    }

    return 0;
}