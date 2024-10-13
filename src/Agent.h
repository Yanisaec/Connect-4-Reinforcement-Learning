#ifndef AGENT_H
#define AGENT_H
#include "Board.h"
#include <unordered_map>
#include <vector>

using namespace std;

class Agent {
private:
    unordered_map<string, vector<double>> qTable;
    double exploration_rate;
    double learning_rate;
    double discount_factor;
    int player_ID;
    int wins;
    int losses;
    int draws;

public:
    Agent(int id) : exploration_rate(0.1), learning_rate(0.5), discount_factor(0.9), wins(0), losses(0), draws(0), player_ID(id) {}
    int chooseAction(Board& board);
    void updateQTable(string prevState, int action, double reward, string newState);
    double calculateReward(Board& board_before_playing, int col); // col is the column in which the agent just played
};

#endif