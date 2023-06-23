#pragma once

#include <list>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

struct CellState {
    char color;
    bool flooded;
    bool explored;
};

class Board {
public:
    Board(size_t width, size_t height, const std::string &colors)
     : origState_()
     , currState_()
     , stateStack_()
     , colors_(colors)
     , width_(width)
     , height_(height) {
        generate();
    }

    // generates a new board with current dimensions
    void generate() {
        if (width_ < 2 || height_ < 2) {
            throw std::runtime_error("board must be at at least 2x2");
        }

        // randomly generate a board
        currState_.cells.resize(width_ * height_);
        for (auto &cell : currState_.cells) {
            cell.color = colors_[rand() % colors_.length()];
            cell.flooded = false;
            cell.explored = false;
        }
        // init board by flooding upper left cell with current color
        currState_.flood_total = flood(getCell(0,0).color);
        origState_ = currState_;
    }

    // resets board back to its original unsolved state
    void reset() {
        currState_ = origState_;
        stateStack_.clear();
    }

    // push board state onto stack. can be restored with restoreState()
    void pushState() {
        stateStack_.push_back(currState_);
    }

    // restore board state from top of stack (doesn't remove it)
    void restoreState() {
        currState_ = stateStack_.back();
    }

    // removes the top stack entry
    void popState() {
        stateStack_.pop_back();
    }

    // the number of states in the stack
    size_t getNumStates() const {
        return stateStack_.size();
    }

    const size_t getWidth() const {
        return width_;
    }

    const size_t getHeight() const {
        return height_;
    }

    const CellState & getCell(size_t x, size_t y) const {
        return currState_.cells.at(x + width_ * y);
    }

    const std::string & getColors() const {
        return colors_;
    }

    void print() const {
        for (unsigned int y=0; y<height_; y++) {
            for (unsigned int x=0; x<width_; x++) {
                printf("%c ", getCell(x,y).color);
            }
            printf("\n");
        }
    }

    void printState() const {
        for (unsigned int y=0; y<height_; y++) {
            for (unsigned int x=0; x<width_; x++) {
                printf("%c ", getCell(x,y).color);
            }
            printf("  ");
            for (unsigned int x=0; x<width_; x++) {
                printf("%c ", (getCell(x,y).flooded ? 'F' : '-'));
            }
            printf("  ");
            for (unsigned int x=0; x<width_; x++) {
                printf("%c ", (getCell(x,y).explored ? 'E' : '-'));
            }
            printf("\n");
        }
    }

    int flood(char flood_c) {
        if (colors_.find(flood_c) != std::string::npos) {
            clearExplored();
            currState_.flood_total = floodInternal(0, 0, 0, 0, flood_c);
            return currState_.flood_total;
        } else {
            printf("must use one of these colors: \"%s\"\n", colors_.c_str());
            return 0;
        }
    }

    bool complete() const {
        return currState_.flood_total >= (width_ * height_);
    }

private:
    void clearExplored() {
        for (auto &cell : currState_.cells) {
            cell.explored = false;
        }
    }

    int floodInternal(size_t prevX, size_t prevY, size_t x, size_t y, char flood_c) {
        int flood_total = 0;
        CellState &this_cell = _getCell(x,y);
        if (this_cell.explored) {
            return flood_total;
        }
        this_cell.explored = true;

        // capture unflooded cells that match our flood color
        bool explore = false;
        if ( ! this_cell.flooded && this_cell.color == flood_c) {
            this_cell.flooded = true;
            flood_total++;
            explore = true;
        } else if (this_cell.flooded && this_cell.color != flood_c) {
            this_cell.color = flood_c;
            flood_total++;
            explore = true;
        }

        if (explore) {
            if (x > 0 && (x-1) != prevX) {
                flood_total += floodInternal(x, y, x-1, y, flood_c);// explore left
            }
            if (y > 0 && (y-1) != prevY) {
                flood_total += floodInternal(x, y, x, y-1, flood_c);// explore up
            }
            if ((x+1) < width_ && (x+1) != prevX) {
                flood_total += floodInternal(x, y, x+1, y, flood_c);// explore right
            }
            if ((y+1) < height_ && (y+1) != prevY) {
                flood_total += floodInternal(x, y, x, y+1, flood_c);// explore down
            }
        }

        return flood_total;
    }

    CellState & _getCell(size_t x, size_t y) {
        return currState_.cells.at(x + width_ * y);
    }

private:
    using CellStates = std::vector<CellState>;
    struct BoardState {
        CellStates cells;
        size_t flood_total;
    };
    BoardState origState_;
    BoardState currState_;
    std::list<BoardState> stateStack_;
    std::string colors_;
    size_t width_;
    size_t height_;

};