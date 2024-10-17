#include "Agent.h"
#include "Trainer.h"
#include "Board.h"
#include "chrono"
#include <iostream>

using namespace std;

void play_game(Agent agent) {
    bool keep_going = true;
    while (keep_going) {
        Board gameboard;
        bool play = true;
        while (play) {
            int action = agent.chooseAction(gameboard, false);
            gameboard.makeMove(1, action);
            if (gameboard.hasPlayerWon(1)) {
                gameboard.displayBoard();
                cout << "AI won !" << endl;
                play = false;
                break;
            } 
            int col;
            gameboard.displayBoard();
            cout << "Choose a column to play (0-6)";
            cin >> col;
            gameboard.makeMove(2, col);

            if (gameboard.hasPlayerWon(2)) {
                gameboard.displayBoard();
                cout << "Human won ! " << endl;
                play = false;
                break;
            } else if (gameboard.isBoardFull()) {
                gameboard.displayBoard();
                cout << "Draw !" << endl;
                play = false;
                break;
            }
        }
        cout << "Keep playing ? (y/n)" << endl;
        string answer;
        cin >> answer;
        if (answer == "n") {
            keep_going = false;
        }
    }
}

int main() {
    srand(time(0)); 
    /////////////////////////////////////
    Agent agent(1);
    string file1 = "agent1_qtable.bin";
    agent.loadQTableBinary(file1);
    play_game(agent);
    /////////////////////////////////////
    // Trainer trainer;
    // string file1 = "agent1_qtable.bin";
    // string file2 = "agent2_qtable.bin";

    // auto start = chrono::high_resolution_clock::now();
    // trainer.train(5000000, file1, file2);
    // auto end = chrono::high_resolution_clock::now();

    // chrono::duration<double> duration = end - start;

    // cout << "Training took: " << duration.count() << " seconds" << endl;

    return 0;
}