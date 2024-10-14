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

    int potential_future_connectivities_generated = 0;
    int potential_future_connectivites_generated_for_opponent = 0;
    // check how many connectivities the agent will be able to generate by above the checker he played
    if (board_after_playing.isValidMove(col)) {
        potential_future_connectivities_generated = board_after_playing.numberOfConnectivitiesGenerated(player_ID, col);
        potential_future_connectivites_generated_for_opponent = board_after_playing.numberOfConnectivitiesGenerated(opponent_player, col);
    }
        
    int prevented_wins = board_before_playing.numberOfWinPossibilitiesPrevented(player_ID, col);
    int connectivities_prevented = board_before_playing.numberOfConnectivitiesPrevented(player_ID, col);
    int connectivities_generated = board_before_playing.numberOfConnectivitiesGenerated(player_ID, col);

    double prevented_wins_normalized = prevented_wins / 12.0;
    double connectivites_prevented_normalized = connectivities_prevented / 7.0;
    double connectivities_generated_normalized = connectivities_generated / 7.0;
    double potential_future_connectivities_generated_normalized = potential_future_connectivities_generated / 7.0;
    double potential_future_connectivites_generated_for_opponent_normalized = potential_future_connectivites_generated_for_opponent / 7.0;

    double reward = (2*connectivities_generated_normalized + connectivites_prevented_normalized + prevented_wins_normalized
    + 2*potential_future_connectivities_generated_normalized - potential_future_connectivites_generated_for_opponent_normalized) / 7;

    // Penalize unproductive moves
    // if (reward < 1) {  // maximum reward is 5
    //     reward = -1.0;
    // }

    return reward;
}


int Agent::chooseAction(Board& board) {
    string current_state = board.getStateRepresentation();
    vector<int> valid_actions = board.getValidActions();
    int random_index;

    // Check if state has already been encoutered
    if (qTable.find(current_state) == qTable.end()) {
        // If it has not already been encoutered, set a random value
        qTable[current_state] = vector<double>(7, 0.0);
        random_index = rand() % valid_actions.size();
        return valid_actions[random_index];
    }

    // Epsilon-greedy action to encourage discovery
    if ((rand() / static_cast<double>(RAND_MAX)) < exploration_rate) {
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

void Agent::updateQTable(string prevState, int action, double reward, string newState) {
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
            outFile << entry.first; 
            for (double qValue : entry.second) {
                outFile << "," << qValue; 
            }
            outFile << "\n"; 
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
            string state;
            getline(ss, state, ',');  

            vector<double> qValues;
            string qValue;
            while (getline(ss, qValue, ',')) {
                qValues.push_back(stod(qValue));
            }

            qTable[state] = qValues;
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading." << endl;
    }
}