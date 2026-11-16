#include "database.h"
#include "network.h"

int main(){
    Database db("db/outage-prediction.db");
    Network net;
    net.loadNetwork(db);
  

    return 0;
}