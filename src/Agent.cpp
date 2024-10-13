#include "Agent.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

double Agent::calculateReward(Board& board_before_playing, int col) { 
    return -1;
}

int Agent::chooseAction(Board& board) {
    string current_state = board.getStateRepresentation();
    vector<int> valid_actions = board.getValidActions();

    // Check if state has already been encoutered
    if (qTable.find(current_state) == qTable.end()) {
        // If it has not already been encoutered, set a random value
        qTable[current_state] = vector<double>(7, 0.0);
    }

    // Epsilon-greedy action to encourage discovery
    if ((rand() / static_cast<double>(RAND_MAX)) < exploration_rate) {
        // Chose random valid action
        int randomIndex = std::rand() % valid_actions.size();
        return valid_actions[randomIndex];
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