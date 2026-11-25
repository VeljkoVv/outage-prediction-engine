#pragma once
#include "predictor_engine.h"
#include "network.h"
#include "trouble_call.h"
#include <iostream>
#include <string>

class Menu{
    public:
        Menu(PredictorEngine& eng, const Network& net, std::istream& in = std::cin, std::ostream& out = std::cout);
        void runMenu();
        void handleChoice(int choice);
        void startMenu();

        void showNetwork();
        void troubleCall();
        void showOutages();
        void confirmOutage();
        void restorePower();
        std::string outageTag(ElementType type, int id) const;

        void printOutages(bool isPredicted);
        CallType callType();
    private:
        PredictorEngine& engine;
        const Network& network;
        int next_call_id = 1;
        std::istream& in;
        std::ostream& out;
};