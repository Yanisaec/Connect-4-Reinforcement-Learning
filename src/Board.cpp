#include "Board.h"
#include <iostream>
#include <stdexcept>

// Constructor
Board::Board() {
    resetBoard();
}

void Board::resetBoard() {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            board[row][col] = 0;
        }
    }
}

void Board::makeMove(int player, int col) {
    int row_first_empty = getFirstZeroInCol(col);
    board[row_first_empty][col] = player;
}

int Board::getFirstZeroInCol(int col) {
    checkColumn(col);
    if (numberCheckersInCol(col) == 6) {
        throw std::invalid_argument("received full column as input");
    }
    for (int row = 5; row >= 0; row--) {
        if (board[row][col] == 0) {
            return row;
        }
    }
    return -1;
}

bool Board::isValidMove(int col) {
    checkColumn(col);
    return numberCheckersInCol(col) != 6;
}

int Board::numberCheckersInCol(int col) {
    checkColumn(col);
    int nbCheckers = 0;
    for (int row = 0; row < 6; row++) {
        if (board[row][col] != 0) {
            nbCheckers++;
        }
    }
    return nbCheckers;
}

void Board::checkColumn(int col) {
    if (col < 0 || col > 6) {
        throw std::invalid_argument("col out of bounds (0-6 allowed)");
    }
}

bool Board::isBoardFull() {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool Board::hasPlayerWon(int player) {
    // Horizontal win
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 6; i++) {
            if (board[i][j] == player && board[i][j + 1] == player && board[i][j + 2] == player && board[i][j + 3] == player) {
                return true;
            }
        }
    }

    // Vertical win
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 3; i++) {
            if (board[i][j] == player && board[i + 1][j] == player && board[i + 2][j] == player && board[i + 3][j] == player) {
                return true;
            }
        }
    }

    // Diagonal left-to-right
    for (int i = 3; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == player && board[i - 1][j + 1] == player && board[i - 2][j + 2] == player && board[i - 3][j + 3] == player) {
                return true;
            }
        }
    }

    // Diagonal right-to-left
    for (int i = 3; i < 6; i++) {
        for (int j = 3; j < 7; j++) {
            if (board[i][j] == player && board[i - 1][j - 1] == player && board[i - 2][j - 2] == player && board[i - 3][j - 3] == player) {
                return true;
            }
        }
    }

    return false;
}

void Board::displayBoard() {
    for (int row = 0; row < 6; row++) {
        std::cout << "|";
        for (int col = 0; col < 7; col++) {
            if (board[row][col] == 1) {
                std::cout << "[X]";
            } else if (board[row][col] == 2) {
                std::cout << "[O]";
            } else {
                std::cout << "[ ]";
            }

            if (col < 6) std::cout << ",";
        }
        std::cout << "|" << std::endl;
    }
}
