#include "menu.h"
#include <iostream>


Menu::Menu(Predictor_Engine& eng, const Network& net) : engine(eng), network(net) {}

void Menu::startMenu(){
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "\tSubstation: " << network.getTransformer().getName() << "(" << network.getTransformer().getId() << ")" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "Feeders: " << network.feederCount() 
              << "\tService points: " << network.servicePointCount()
              << "\tCustomers: " << network.sdpCount() << std::endl;
    std::cout << "Open outages: "  
              << "\tOpen calls: " << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "1. Show network" << std::endl;
    std::cout << "2. New trouble call" << std::endl;
    std::cout << "3. Show open outages" << std::endl;
    std::cout << "4. Confirm outage" << std::endl;
    std::cout << "5. Restore power" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "Choice: " << std::endl;
}