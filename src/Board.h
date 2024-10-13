#ifndef BOARD_H
#define BOARD_H
#include <string>

class Board {
private:
    int board[6][7];

public:
    Board();
    void resetBoard();
    void makeMove(int player, int col);
    int getFirstZeroInCol(int col);
    bool isValidMove(int col);
    int numberCheckersInCol(int col);
    void checkColumn(int col);
    bool isBoardFull();
    bool hasPlayerWon(int player);
    void displayBoard();
    std::string getStateRepresentation();
};

#endif // BOARD_H