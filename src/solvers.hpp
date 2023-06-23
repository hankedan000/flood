#pragma once

#include <stdexcept>

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

std::string solveRandBestN(Board &board, size_t maxSolves) {
    std::string bestMoves = "";
    for (size_t i=0; i<maxSolves; i++) {
        std::string thisMoves = solveRand(board);
        if (bestMoves.empty() || thisMoves.length() < bestMoves.length()) {
            bestMoves = thisMoves;
        }
    }
    return bestMoves;
}
