#include "database.h"
#include "network.h"
#include "menu.h"
#include "predictor_engine.h"

int main(){
    Database db("db/outage-prediction.db");
    Network net;
    net.loadNetwork(db);
    Predictor_Engine p;

    Menu m( p, net);
    m.startMenu();

    return 0;
}