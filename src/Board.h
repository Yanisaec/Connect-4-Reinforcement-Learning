#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <vector>

using namespace std;

class Board {
private:
    int board[6][7];

public:
    Board();
    Board(const Board& other);
    Board clone() const;
    void resetBoard(); // resets the board to empty state
    void makeMove(int player, int col); // place a checker in the column at the lowest possible point
    int getFirstZeroInCol(int col); // get the row where a checker would fall if played in the column
    bool isValidMove(int col);
    int numberCheckersInCol(int col);
    void checkColumn(int col);
    bool isBoardFull();
    bool hasPlayerWon(int player); // checks if the player has 4 checkers connected
    void displayBoard();
    string getStateRepresentation(); // puts the state of the board in a string
    vector<int> getValidActions(); // returns the list of indices of columns where a player can still play
    int numberWinPossibilities(int opponent_player); // number of winning situations for the player that isn't opponent_player
    int numberOfWinPossibilitiesDestroyed(int player, int col); // returns the number of possibilities of winning that playing in col would take away
    int numberOfConnectivitiesGenerated(int player, int col);
};

#endif // BOARD_H