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
    int opponent_player = player_ID%2+1;

    Board board_after_playing = board_before_playing.clone();
    board_after_playing.makeMove(player_ID, col);

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

    double reward = (connectivities_generated_normalized + connectivites_prevented_normalized + prevented_wins_normalized
    + potential_future_connectivities_generated - potential_future_connectivites_generated_for_opponent) / 5;

    // Penalize unproductive moves
    if (reward < 2) {  // maximum reward is 5
        reward = -1.0;
    }

    return reward;
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

void Agent::updateQTable(string prevState, int action, double reward, string newState) {
    
}