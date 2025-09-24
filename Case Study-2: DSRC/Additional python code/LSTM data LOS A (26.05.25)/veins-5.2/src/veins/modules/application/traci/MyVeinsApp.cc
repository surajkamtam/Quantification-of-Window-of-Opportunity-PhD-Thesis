#include "veins/modules/application/traci/MyVeinsApp.h"
#include "veins/modules/application/traci/TraCIDemo11pRSUMessage_m.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <random>

using namespace veins;
using namespace std;

Define_Module(veins::MyVeinsApp);

Coord normalvPos;

cMessage *beacon1 = nullptr;
const char *nvehID;


void MyVeinsApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
    }
//    if (stage == 1) {
    beacon1 = new cMessage("beacon");
    scheduleAt(simTime() + beaconInterval, beacon1);
//}
}

void MyVeinsApp::onWSM(BaseFrame1609_4 *frame) {
    TraCIDemo11pMessage *wsm = dynamic_cast<TraCIDemo11pMessage*>(frame);


}

void MyVeinsApp::handleSelfMsg(cMessage *msg) {
    TraCIDemo11pMessage *wsm = new TraCIDemo11pMessage();
    if (msg->isName("beacon")) {
      nvehID = mobility->getExternalId().c_str();
      normalvPos = mobility->getCurrentPosition();

      wsm->setDemoData("V2X Msg");
      wsm->setVehicleId(nvehID);
      wsm->setCarSenderPos(normalvPos);
      populateWSM(wsm);
      sendDown(wsm);
    }

}

void MyVeinsApp::handlePositionUpdate(cObject *obj) {



}

void MyVeinsApp::finish() {
//    if (beacon1){
//           cancelAndDelete(beacon1);
//           beacon1 = nullptr;
//       }

}
