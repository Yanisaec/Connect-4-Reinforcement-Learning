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
    Agent(int id) : exploration_rate(0.33), learning_rate(0.5), discount_factor(0.5), player_ID(id), wins(0), losses(0), draws(0) {}
    int chooseAction(Board& board, bool training);
    void updateQTable(string prevState, int action, double reward, string newState);
    double calculateReward(Board& board_before_playing, int col); // col is the column in which the agent just played
    void saveQTable(const std::string& filename);
    void loadQTable(const std::string& filename);
};

#endif