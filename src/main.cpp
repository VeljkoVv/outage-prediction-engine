#include "database.h"
#include "network.h"
#include "menu.h"
#include "predictor_engine.h"

int main(){
    Database db("db/outage-prediction.db");
    Network net;
    PredictorEngine p(net);

    net.loadNetwork(db);
    Menu m( p, net);
    m.runMenu();

    return 0;
}