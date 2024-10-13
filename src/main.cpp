#include "Board.h"
#include <iostream>

int main() {
    Board gameBoard;
    
    gameBoard.makeMove(1, 0);
    gameBoard.makeMove(2, 1);
    gameBoard.makeMove(1, 0);
    gameBoard.makeMove(2, 1);
    gameBoard.makeMove(1, 2);
    gameBoard.displayBoard();

    return 0;
}