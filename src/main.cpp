#include "Board.h"
#include "Agent.h"
#include "Trainer.h"
#include "chrono"
#include <iostream>

using namespace std;

int main() {
    srand(time(0)); 

    Trainer trainer;
    string file1 = "agent1_qtable.csv";
    string file2 = "agent2_qtable.csv";

    auto start = chrono::high_resolution_clock::now();
    trainer.train(5000, file1, file2);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;

    cout << "Training took: " << duration.count() << " seconds" << endl;

    return 0;
}