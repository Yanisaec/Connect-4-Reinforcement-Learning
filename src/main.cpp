#include "Board.h"
#include "Agent.h"
#include "Trainer.h"
#include "chrono"
#include <iostream>

using namespace std;

void play_game(Agent agent) {
    Board gameboard;
    while (!gameboard.isBoardFull()) {
        int action = agent.chooseAction(gameboard, false);
        gameboard.makeMove(1, action);
        if (gameboard.hasPlayerWon(1)) {
            cout << "AI won !" << endl;
            return;
        } 
        int col;
        gameboard.displayBoard();
        cout << "Chose a column to play (0-6)";
        cin >> col;
        gameboard.makeMove(2, col);

        if (gameboard.hasPlayerWon(2)) {
            cout << "Human won ! " << endl;
            return;
        } else if (gameboard.isBoardFull()) {
            cout << "Draw !" << endl;
            return;
        }
    }
}

int main() {
    srand(time(0)); 
    /////////////////////////////////////
    // Agent agent(1);
    // string file1 = "agent1_qtable.csv";
    // agent.loadQTable(file1);
    // play_game(agent);
    /////////////////////////////////////
    Trainer trainer;
    string file1 = "agent1_qtable.csv";
    string file2 = "agent2_qtable.csv";

    auto start = chrono::high_resolution_clock::now();
    trainer.train(10000000, file1, file2);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;

    cout << "Training took: " << duration.count() << " seconds" << endl;

    return 0;
}