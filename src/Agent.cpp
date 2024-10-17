#include "Agent.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <cmath>

using namespace std;

double Agent::calculateReward(Board& board_before_playing, int col) { 
    Board board_after_playing = board_before_playing.clone();
    int opponent_player = player_ID%2+1;

    if (board_after_playing.hasPlayerWon(player_ID)) {
        return 100;  // High positive reward for winning
    }

    for (int i=0; i<7; i++){
        Board test_board_opponent = board_before_playing.clone();
        Board test_board_agent = board_before_playing.clone();
        if (test_board_opponent.isValidMove(i)){
            test_board_agent.makeMove(player_ID, i);
            if (test_board_agent.hasPlayerWon(player_ID)) {
                return -100; // missed an opportunity to win
            }
            if (i!=col) {
                test_board_opponent.makeMove(opponent_player, i);
                if (test_board_opponent.hasPlayerWon(opponent_player)) {
                    return -100; // missed the opportunity to block a connect 4
                }
            } else {
                test_board_opponent.makeMove(opponent_player, i);
                if (test_board_opponent.hasPlayerWon(opponent_player)) {
                    return 50; // blocked a connect 4
                }
            }
        }
    }

    int potential_future_connectivities_generated = 0;
    int potential_future_connectivites_generated_for_opponent = 0;
    int potential_winning_offered = 0;
    // check how many connectivities the agent will be able to generate by above the checker he played
    if (board_after_playing.isValidMove(col)) {
        potential_future_connectivities_generated = board_after_playing.numberOfConnectivitiesGenerated(player_ID, col);
        potential_future_connectivites_generated_for_opponent = board_after_playing.numberOfConnectivitiesGenerated(opponent_player, col);
        potential_winning_offered = board_after_playing.numberConnect4Prevented(player_ID, col);
    }
    if (potential_winning_offered !=0) {
        return -50;
    }
        
    int prevented_wins_placements = board_before_playing.numberOfWinPossibilitiesPrevented(player_ID, col);
    int connectivities_prevented = board_before_playing.numberOfConnectivitiesPrevented(player_ID, col);
    int connectivities_generated = board_before_playing.numberOfConnectivitiesGenerated(player_ID, col);
    int number_connect4_prevented = board_before_playing.numberConnect4Prevented(player_ID, col);
    if (number_connect4_prevented !=0) {
        return 50;
    }

    double prevented_wins_placements_normalized = prevented_wins_placements / 12.0;
    double connectivites_prevented_normalized = connectivities_prevented / 7.0;
    double connectivities_generated_normalized = connectivities_generated / 7.0;
    double potential_future_connectivities_generated_normalized = potential_future_connectivities_generated / 7.0;
    double potential_future_connectivites_generated_for_opponent_normalized = potential_future_connectivites_generated_for_opponent / 7.0;

    double reward = (connectivities_generated_normalized + connectivites_prevented_normalized + prevented_wins_placements_normalized
    + potential_future_connectivities_generated_normalized - potential_future_connectivites_generated_for_opponent_normalized)*10;

    // Penalize unproductive moves
    // if (reward < 1) {  // maximum reward is 5
    //     reward = -1.0;
    // }

    return reward;
}


int Agent::chooseAction(Board& board, bool training) {
    uint64_t current_state = board.getStateRepresentation();
    vector<int> valid_actions = board.getValidActions();
    int random_index;

    // Check if state has already been encoutered
    if (qTable.find(current_state) == qTable.end()) {
        // If it has not already been encoutered, set a random value
        qTable[current_state] = vector<double>(7, 0.0);
        if (rand() / static_cast<double>(RAND_MAX) > 0.5) {
            vector<int> top_moves = board.getTopMoves(player_ID);
            if (top_moves.size() > 0) {
                random_index = rand() % top_moves.size();
                return top_moves[random_index];
            }
        }
        random_index = rand() % valid_actions.size();
        return valid_actions[random_index];
    }

    // Epsilon-greedy action to encourage discovery
    if (training && (rand() / static_cast<double>(RAND_MAX)) < exploration_rate) {
        // Chose random valid action
        random_index = rand() % valid_actions.size();
        return valid_actions[random_index];
    }

    // Chose the best action according to the qTable
    double maxQ = -1e9;
    int best_action = valid_actions[0];
    for (int action : valid_actions) {
        double qValue = qTable[current_state][action];
        if (qValue > maxQ) {
            maxQ = qValue;
            best_action = action;
        }
    }
    return best_action;
}

void Agent::updateQTable(uint64_t prevState, int action, double reward, uint64_t newState) {
    double currentQvalue = qTable[prevState][action];
    double maxFutureQvalue;
    if (qTable.find(newState) == qTable.end()) {
        maxFutureQvalue = 0;
    } else {
        maxFutureQvalue = *max_element(qTable[newState].begin(), qTable[newState].end());    
    }
    
    double newQvalue = (1 - learning_rate)*currentQvalue + learning_rate*(reward + discount_factor*maxFutureQvalue);
    qTable[prevState][action] = newQvalue;
}

void Agent::saveQTable(const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& entry : qTable) {
            outFile << entry.first;  // Save the uint64_t state as is (numeric representation)
            for (double qValue : entry.second) {
                outFile << "," << qValue;  // Save Q-values separated by commas
            }
            outFile << "\n";  // Newline after each state entry
        }
        outFile.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
}


void Agent::loadQTable(const string& filename) {
    ifstream inFile(filename);
    string line;
    
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            stringstream ss(line);
            uint64_t state;
            ss >> state;  // Read the uint64_t state (no comma before the first element)

            if (ss.peek() == ',') ss.ignore();  // Skip the comma before Q-values

            vector<double> qValues;
            string qValue;
            while (getline(ss, qValue, ',')) {
                qValues.push_back(stod(qValue));  // Convert and store each Q-value
            }

            qTable[state] = qValues;  // Insert the state and corresponding Q-values into the map
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading." << endl;
    }
}

// Save the Q-table in binary format
void Agent::saveQTableBinary(const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (outFile.is_open()) {
        for (const auto& entry : qTable) {
            outFile.write(reinterpret_cast<const char*>(&entry.first), sizeof(entry.first));  // Write uint64_t state
            for (double qValue : entry.second) {
                outFile.write(reinterpret_cast<const char*>(&qValue), sizeof(qValue));  // Write Q-value
            }
        }
        outFile.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
}

// Load the Q-table from binary format
void Agent::loadQTableBinary(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (inFile.is_open()) {
        uint64_t state;
        vector<double> qValues(7);  // Assuming 7 possible actions (adjust if necessary)
        while (inFile.read(reinterpret_cast<char*>(&state), sizeof(state))) {
            for (int i = 0; i < 7; i++) {
                inFile.read(reinterpret_cast<char*>(&qValues[i]), sizeof(double));
            }
            qTable[state] = qValues;
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading." << endl;
    }
}