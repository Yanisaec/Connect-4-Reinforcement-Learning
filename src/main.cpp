#include "Board.h"
#include <iostream>

using namespace std;

int main() {
    Board gameBoard;
    
    gameBoard.makeMove(1, 0);
    gameBoard.makeMove(2, 1);
    gameBoard.makeMove(1, 0);
    gameBoard.makeMove(2, 1);
    gameBoard.makeMove(1, 2);
    gameBoard.makeMove(1, 2);
    gameBoard.makeMove(1, 2);
    gameBoard.makeMove(1, 2);
    gameBoard.makeMove(1, 3);
    gameBoard.makeMove(1, 4);
    gameBoard.makeMove(2, 4);
    gameBoard.makeMove(1, 4);
    gameBoard.makeMove(2, 6);
    gameBoard.displayBoard();

    int i = gameBoard.numberOfConnectivitiesGenerated(2, 3);
    cout << i << endl;    
    
    int j = gameBoard.numberOfConnectivitiesPrevented(2, 3);
    cout << j << endl;

    int number_of_possibilites = gameBoard.numberWinPossibilities(2);
    cout << number_of_possibilites << endl;

    int number_destroyed = gameBoard.numberOfWinPossibilitiesPrevented(2, 4);    
    cout << number_destroyed << endl;
    return 0;
}