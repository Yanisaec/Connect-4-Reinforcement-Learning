#include "Trainer.h"
#include "Board.h"
#include "Agent.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Trainer::Trainer() {}

void Trainer::train(int nb_games, string filename1, string filename2) {
    Agent agent1(1);
    Agent agent2(2);
    int action1;
    int wins = 0;
    int draws = 0;
    int action2;
    int last_action2;
    double reward1;
    double reward2;
    string board_state;
    string board_state_after_1;
    string last_board_state_after_1;
    string board_state_after_2;
    for (int game=0; game<nb_games; game++) {
        if (game%50000 == 0) {
            cout << game << " games played yet" << endl;
        }
        Board gameBoard;
        bool stop = false;
        bool agent1_won = false;
        while (!gameBoard.isBoardFull() && !stop) {
            last_action2 = action2;
            last_board_state_after_1 = board_state_after_1;

            board_state = gameBoard.getStateRepresentation();

            action1 = agent1.chooseAction(gameBoard, true);
            reward1 = agent1.calculateReward(gameBoard, action1);
            gameBoard.makeMove(1, action1);
            board_state_after_1 = gameBoard.getStateRepresentation();

            if (gameBoard.hasPlayerWon(1)) {
                // cout << "Agent 1 has won" << endl;
                reward2 = -100;
                agent2.updateQTable(last_board_state_after_1, last_action2, reward2, board_state);
                agent1_won = true;
                stop = true;
            }

            action2 = agent2.chooseAction(gameBoard, true);
            reward2 = agent2.calculateReward(gameBoard, action2);
            gameBoard.makeMove(2, action2);
            board_state_after_2 = gameBoard.getStateRepresentation();

            if (gameBoard.hasPlayerWon(2)) {
                // cout << "Agent 2 has won" << endl;
                reward1 = -100;
                agent1.updateQTable(board_state, action1, reward1, board_state_after_1);
                stop = true;
            }

            if (!stop) {
                agent1.updateQTable(board_state, action1, reward1, board_state_after_1);
            }

            if (!agent1_won) {
            agent2.updateQTable(board_state_after_1, action2, reward2, board_state_after_2);
            } 
        }
        // gameBoard.displayBoard();
        if (agent1_won) {
            wins++;
        }
        else if (gameBoard.isBoardFull()) {
            draws++;
        }
    }
    agent1.saveQTable(filename1);
    agent2.saveQTable(filename2);
    cout << wins << endl;
    cout << draws << endl;
}