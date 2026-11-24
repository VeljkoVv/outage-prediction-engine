#include "menu.h"
#include "trouble_call.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>


Menu::Menu(PredictorEngine& eng, const Network& net) : engine(eng), network(net) {}

void Menu::runMenu(){
    while (true) {
        startMenu();
        std::cout << "Choice: ";
        int choice;
        std::cin >> choice;
        while(choice < 0 || choice > 5){
            std::cout << "Invalid option.\n";
            std::cout << "Choice: ";
            std::cin >> choice;
        }
        if (choice == 0) {
            break;
        }
        handleChoice(choice);
    }
}

void Menu::handleChoice(int choice){
    try {
        switch (choice) {
            case 1:
                showNetwork();
                break;
            case 2:
                troubleCall();
                break;
            case 3:
                showOutages();
                break;
            case 4:
                confirmOutage();
                break;
            case 5:
                restorePower();
                break;
            default:
                std::cout << "Invalid option.\n";
                break;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}

void Menu::showNetwork(){

}
void Menu::troubleCall(){
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "Trouble call: " << std::endl;
    int dp_id;
    std::cout << "\nDelivery point id: " << std::endl;
    std::cin >> dp_id;
    CallType t = callType();
    Trouble_Call::TimePoint tp = std::chrono::system_clock::now();
    Trouble_Call call(next_call_id++, dp_id, t, tp);
    engine.onTroubleCall(call);
}

CallType Menu::callType() {
    std::cout << "Call type:\n"
              << "1. No power\n"
              << "2. Partial power\n"
              << "3. Flickering\n"
              << "4. Hazard\n";

    int choice = 0;
    while (choice < 1 || choice > 4) {
        std::cout << "Choice: ";
        std::cin >> choice;
    }

    switch (choice) {
        case 1:
            return CallType::NoPower;
        case 2:
            return CallType::PartialPower;
        case 3:
            return CallType::Flickering;
        default:
            return CallType::Hazard;
    }
}

void Menu::showOutages(){
    if(engine.openOutageCount() == 0){
        std::cout << "No open outages" << std::endl;
        return;
    }
    std::cout << "--- Open outages( " << engine.openOutageCount() << ") ---" << std::endl;
    std::cout << "ID \tElement \t\tStatus \t\tCalls \tCustomers \tStarted at" << std::endl;
    for(const auto& [id, outage] : engine.getOutages()){
        if (!outage.isOpen()) {
            continue;
        }
        std::cout << outage.getId();
            switch(outage.getType()){
                case ElementType::ServicePoint:
                    std::cout << "\tService Point ";
                    break;
                case ElementType::Feeder:
                    std::cout << "\tFeeder ";
                    break;
                default:
                    std::cout << "\tTransformer ";
                    break;
            }
        std::cout << outage.getElementId();
        switch(outage.getStatus()){
            case OutageStatus::Cancelled:
                std::cout << "\tCancelled";
                break;
            case OutageStatus::Confirmed:
                std::cout << "\tConfirmed";
                break;
            case OutageStatus::Merged:
                std::cout << "\tMerged";
                break;
            case OutageStatus::Predicted:
                std::cout << "\tPredicted";
                break;
            default:
                std::cout << "\tRestored";
                break;
        }
        std::time_t start = std::chrono::system_clock::to_time_t(outage.getStartedAt());
        std::cout << "\t" << outage.getCallIds().size() 
                  << "\t" << network.customersUnder(outage) 
                  << "\t\t" << std::put_time(std::localtime(&start), "%H:%M") << std::endl;
    }
}
void Menu::confirmOutage(){

}
void Menu::restorePower(){

}

void Menu::startMenu(){
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "\tSubstation: " << network.getTransformer().getName() << "(" << network.getTransformer().getId() << ")" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "Feeders: " << network.feederCount() 
              << "\tService points: " << network.servicePointCount()
              << "\tCustomers: " << network.sdpCount() << std::endl;
    std::cout << "Open outages: " << engine.openOutageCount()
              << "\tOpen calls: " << engine.openCallCount() << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "1. Show network" << std::endl;
    std::cout << "2. New trouble call" << std::endl;
    std::cout << "3. Show open outages" << std::endl;
    std::cout << "4. Confirm outage" << std::endl;
    std::cout << "5. Restore power" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}