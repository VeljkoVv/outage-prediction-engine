#include "menu.h"
#include "trouble_call.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdexcept>


Menu::Menu(PredictorEngine& eng, const Network& net, std::istream& in, std::ostream& out)
    : engine(eng), network(net), in(in), out(out) {
}

void Menu::runMenu(){
    while (true) {
        startMenu();
        out << "Choice: ";
        int choice;
        in >> choice;
        while(choice < 0 || choice > 5){
            out << "Invalid option.\n";
            out << "Choice: ";
            in >> choice;
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
                out << "Invalid option.\n";
                break;
        }
    } catch (const std::exception& e) {
        out << "Error: " << e.what() << '\n';
    }
}

std::string Menu::outageTag(ElementType type, int id) const {
    if (auto o = engine.findOpenOutage(type, id)) {
        return "  [OUTAGE #" + std::to_string(*o) + "]";
    }
    return "";
}

void Menu::showNetwork(){
    out << network.getTransformer().getName() << " (" << network.getTransformer().getId() << ")" 
                                                    << outageTag(ElementType::Transformer, network.getTransformer().getId()) << std::endl;

    for (const auto& [feederId, feeder] : network.getFeeders()) {
        out << " \t" << feeder.getName() << " (" << feederId << ")" 
                           << outageTag(ElementType::Feeder, feederId) << std::endl;

        for (int spId : network.servicePointsOf(feederId)) {
            out << "\t\t" << network.getServicePoint(spId).getName() << " (" << spId << ")" 
                                << outageTag(ElementType::ServicePoint, spId) << std::endl;
            for (int sdpId : network.sdpsOf(spId)) {
                out << "\t\t\t" << network.getSdp(sdpId).getName() << " (" << sdpId << ")" << std::endl;
            }
        }
    }
}
void Menu::troubleCall(){
    out << "-------------------------------------------------------" << std::endl;
    out << "Trouble call: " << std::endl;
    int dp_id;
    out << "\nDelivery point id: " << std::endl;
    in >> dp_id;
    CallType t = callType();
    Trouble_Call::TimePoint tp = std::chrono::system_clock::now();
    Trouble_Call call(next_call_id++, dp_id, t, tp);
    engine.onTroubleCall(call);
}

CallType Menu::callType() {
    out << "\nCall type:\n"
              << "1. No power\n"
              << "2. Partial power\n"
              << "3. Flickering\n"
              << "4. Hazard\n";

    int choice = 0;
    while (choice < 1 || choice > 4) {
        out << "Choice: ";
        if (!(in >> choice)) {
            throw std::runtime_error("Invalid input");
        }
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
        out << "\nNo open outages" << std::endl;
        return;
    }
    printOutages(false);
}

void Menu::printOutages(bool isPredicted){
    int count;
    if(isPredicted){
        count = engine.predictedOutageCount();
    } else{
        count = engine.openOutageCount();
    }
    out << "--- Outages (" << count << ") ---" << std::endl;
    out << "ID \tElement \t\tStatus \t\tCalls \tCustomers \tStarted at" << std::endl;
    for(const auto& [id, outage] : engine.getOutages()){
        if (!outage.isOpen()) {
            continue;
        }
        if (isPredicted && outage.getStatus() != OutageStatus::Predicted) {
            continue;
        }
        out << outage.getId();
            switch(outage.getType()){
                case ElementType::ServicePoint:
                    out << "\tService Point ";
                    break;
                case ElementType::Feeder:
                    out << "\tFeeder ";
                    break;
                default:
                    out << "\tTransformer ";
                    break;
            }
        out << outage.getElementId();
        switch(outage.getStatus()){
            case OutageStatus::Cancelled:
                out << "\tCancelled";
                break;
            case OutageStatus::Confirmed:
                out << "\tConfirmed";
                break;
            case OutageStatus::Merged:
                out << "\tMerged";
                break;
            case OutageStatus::Predicted:
                out << "\tPredicted";
                break;
            default:
                out << "\tRestored";
                break;
        }
        std::time_t start = std::chrono::system_clock::to_time_t(outage.getStartedAt());
        out << "\t" << outage.getCallIds().size() 
                  << "\t" << network.customersUnder(outage) 
                  << "\t\t" << std::put_time(std::localtime(&start), "%H:%M") << std::endl;
    }
}

void Menu::confirmOutage(){
    if (engine.predictedOutageCount() == 0) {
        out << "\nNo outages to confirm" << std::endl;
        return;
    }
    printOutages(true);
    
    out << "Type ID to confirm (0 to go back): ";
    int choice;
    in >> choice;
    if(choice == 0){
        return;
    }
    engine.confirmOutage(choice);
    out << "Outage " << choice << " confirmed." << std::endl;
}

void Menu::restorePower(){
    if (engine.openOutageCount() == 0) {
        out << "\nNo outages to restore" << std::endl;
        return;
    }
    printOutages(false);
    
    out << "Type ID to restore (0 to go back): ";
    int choice;
    in >> choice;
    if(choice == 0){
        return;
    }
    engine.restoreOutage(choice, std::chrono::system_clock::now());
    out << "\nOutage " << choice << " restored." << std::endl;
}

void Menu::startMenu(){
    out << "\n=======================================================" << std::endl;
    out << "\tSubstation: " << network.getTransformer().getName() << "(" << network.getTransformer().getId() << ")" << std::endl;
    out << "=======================================================" << std::endl;
    out << "Feeders: " << network.feederCount() 
              << "\tService points: " << network.servicePointCount()
              << "\tCustomers: " << network.sdpCount() << std::endl;
    out << "Open outages: " << engine.openOutageCount()
              << "\tOpen calls: " << engine.openCallCount() << std::endl;
    out << "-------------------------------------------------------" << std::endl;
    out << "1. Show network" << std::endl;
    out << "2. New trouble call" << std::endl;
    out << "3. Show open outages" << std::endl;
    out << "4. Confirm outage" << std::endl;
    out << "5. Restore power" << std::endl;
    out << "0. Exit" << std::endl;
    out << "-------------------------------------------------------" << std::endl;
}