#ifndef TRAINER_H
#define TRAINER_H
#include <string>

using namespace std;

class Trainer {
public:
    Trainer();
    void train(int nb_games, string filename1, string filename2);
};

#endif // TRAINER_H