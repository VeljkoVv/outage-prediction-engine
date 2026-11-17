#pragma once
#include "predictor_engine.h"
#include "network.h"

class Menu{
    public:
        Menu(Predictor_Engine& eng, const Network& net);

        void startMenu();
    private:
        Predictor_Engine& engine;
        const Network& network;
};