#include "Board.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Constructor
Board::Board() {
    resetBoard();
}

Board::Board(const Board& other) {
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 7; col++) {
                board[row][col] = other.board[row][col];
            }
        }
    }

// Method to clone the board
Board Board::clone() const {
    return Board(*this);
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
        throw invalid_argument("received full column as input");
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

vector<int> Board::getValidActions() {
    vector<int> valid_actions;
    for (int col=0; col<7; col++) {
        if (isValidMove(col)) {
            valid_actions.push_back(col);
        }
    }
    return valid_actions;
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
        throw invalid_argument("col out of bounds (0-6 allowed)");
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

int Board::numberWinPossibilities(int player) {
    int opponent_player = player%2+1;
    int number_of_possibilites = 69; // Total number of winning checkers position
    // Check for all possible winning conditions if there is a checker of the opponent that would prevent winning this way
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 6; i++) {
            if (board[i][j] == opponent_player || board[i][j + 1] == opponent_player || board[i][j + 2] == opponent_player || board[i][j + 3] == opponent_player) {
                number_of_possibilites--;
            }
        }
    }
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 3; i++) {
            if (board[i][j] == opponent_player || board[i + 1][j] == opponent_player || board[i + 2][j] == opponent_player || board[i + 3][j] == opponent_player) {
                number_of_possibilites--;
            }
        }
    }
    for (int i = 3; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == opponent_player || board[i - 1][j + 1] == opponent_player || board[i - 2][j + 2] == opponent_player || board[i - 3][j + 3] == opponent_player) {
                number_of_possibilites--;
            }
        }
    }
    for (int i = 3; i < 6; i++) {
        for (int j = 3; j < 7; j++) {
            if (board[i][j] == opponent_player || board[i - 1][j - 1] == opponent_player || board[i - 2][j - 2] == opponent_player || board[i - 3][j - 3] == opponent_player) {
                number_of_possibilites--;
            }
        }
    }
    return number_of_possibilites;
}

int Board::numberOfWinPossibilitiesPrevented(int player, int col) {
    // The maximum value is 12, the minimum is 0
    int opponent_player = player%2+1; 
    int current_number_wins_possibilities_for_opponent = numberWinPossibilities(opponent_player);
    Board board_for_future = clone();
    board_for_future.makeMove(player, col);
    int potential_number_wins_for_opponent = board_for_future.numberWinPossibilities(opponent_player);
    int number_of_wins_removed = current_number_wins_possibilities_for_opponent - potential_number_wins_for_opponent;
    return number_of_wins_removed;
}

int Board::numberOfConnectivitiesGenerated(int player, int col) {
    // The maximum value is 7, the minimum is 0
    int row = getFirstZeroInCol(col);
    int number_of_connectivities = 0;
    for (int r=row-1; r<row+2; r++) {
        for (int c=col-1; c<col+2; c++) {
            if (r>0 && r<6 && c>0 && c<7) {
                if (board[r][c] == player) {
                    number_of_connectivities++;
                }
            }
        }
    }
    return number_of_connectivities;
}

int Board::numberOfConnectivitiesPrevented(int player, int col) {
    // The maximum value is 7, the minimum is 0
    int opponent_player = player%2+1;
    int row = getFirstZeroInCol(col);
    int number_of_connectivities_prevented = 0;
    for (int r=row-1; r<row+2; r++) {
        for (int c=col-1; c<col+2; c++) {
            if (r>0 && r<6 && c>0 && c<7) {
                if (board[r][c] == opponent_player) {
                    number_of_connectivities_prevented++;
                }
            }
        }
    }
    return number_of_connectivities_prevented;
}

void Board::displayBoard() {
    for (int row = 0; row < 6; row++) {
        cout << "|";
        for (int col = 0; col < 7; col++) {
            if (board[row][col] == 1) {
                cout << "[X]";
            } else if (board[row][col] == 2) {
                cout << "[O]";
            } else {
                cout << "[ ]";
            }

            if (col < 6) cout << ",";
        }
        cout << "|" << endl;
    }
}

int Board::numberConnect4Prevented(int player, int col) {
    int opponent = player % 2 + 1;
    int nbPrevented = 0;
    int row = getFirstZeroInCol(col);

    // Diagonal: bottom-left to top-right (\ direction)
    for (int start = -3; start <= 0; start++) {
        int count_opponent = 0;
        for (int i = 0; i < 4; i++) {
            int r = row + start + i;
            int c = col + start + i;
            if (r >= 0 && r < 6 && c >= 0 && c < 7) {
                if (board[r][c] == opponent) {
                    count_opponent++;
                } else if (r == row && c == col) {
                    // This is the new move position; ignore it for the count
                    continue;
                } else {
                    break; // Not continuous
                }
            }
        }
        if (count_opponent == 3) {
            nbPrevented++;
        }
    }

    // Diagonal: top-left to bottom-right (/ direction)
    for (int start = -3; start <= 0; start++) {
        int count_opponent = 0;
        for (int i = 0; i < 4; i++) {
            int r = row - start - i;
            int c = col + start + i;
            if (r >= 0 && r < 6 && c >= 0 && c < 7) {
                if (board[r][c] == opponent) {
                    count_opponent++;
                } else if (r == row && c == col) {
                    // This is the new move position; ignore it for the count
                    continue;
                } else {
                    break; // Not continuous
                }
            }
        }
        if (count_opponent == 3) {
            nbPrevented++;
        }
    }

    // Horizontal: left to right (--- direction)
    for (int start = -3; start <= 0; start++) {
        int count_opponent = 0;
        for (int i = 0; i < 4; i++) {
            int c = col + start + i;
            if (c >= 0 && c < 7) {
                if (board[row][c] == opponent) {
                    count_opponent++;
                } else if (c == col) {
                    continue; // Ignore the current move
                } else {
                    break; // Not continuous
                }
            }
        }
        if (count_opponent == 3) {
            nbPrevented++;
        }
    }

    // Vertical: top to bottom (| direction)
    for (int start = -3; start <= 0; start++) {
        int count_opponent = 0;
        for (int i = 0; i < 4; i++) {
            int r = row + start + i;
            if (r >= 0 && r < 6) {
                if (board[r][col] == opponent) {
                    count_opponent++;
                } else if (r == row) {
                    continue; // Ignore the current move
                } else {
                    break; // Not continuous
                }
            }
        }
        if (count_opponent == 3) {
            nbPrevented++;
        }
    }

    return nbPrevented;
}


string Board::getStateRepresentation() {
    stringstream ss;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            ss << board[row][col]; // Append the player ID (0, 1, or 2)
        }
    }
    return ss.str();
}