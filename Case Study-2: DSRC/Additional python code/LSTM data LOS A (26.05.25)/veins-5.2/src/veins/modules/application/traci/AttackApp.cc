#include "veins/modules/application/traci/AttackApp.h"
#include "veins/modules/application/traci/TraCIDemo11pRSUMessage_m.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <random>

using namespace veins;
using namespace std;

Define_Module(veins::AttackApp);

Coord vPos;
bool DoSAttack = false;
int msgflag;
double speedAtk;
const char *vehID;
double sP;  // Attack success probability
int attackPolicy;  // Attack mode (0 = Persistent, 1 = Periodic, 2 = Sporadic)
int nDos;  // Number of DoS messages
simtime_t beaconInterval, OrgBeacon; // Beacon interval (time between periodic beacon broadcasts)
cMessage *attackTimer = nullptr;  // Timer for periodic attacks
cMessage *beacon = nullptr;
int atkInt;
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> distribution(0.0, 1.0);
double falseSpeed;
int attack;

//   std::random_device rd;
std::mt19937 gen(rd());

// Define a small random variation around the offset (±2 m/s variation)
//std::uniform_real_distribution<> variation(-5.0, 5.0);

void AttackApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        OrgBeacon = par("beaconInterval");
    }

    if (stage == 1) {
        attackPolicy = par("attackPolicy");  // Get attack mode
        sP = par("successProbability");  // Probability of attack
        nDos = par("nDosMessages");
        beaconInterval = par("beaconInterval"); // Beacon interval (time between beacon broadcasts)

        messageReceiveVector.setName("Received Messages Timestamp");

        atkInt = par("attackInterval");
        speedAtk = par("speedAttackOffset");
        attack = par("attack");
        beacon = new cMessage("beacon");
        if(attack == 0){
            scheduleAt(simTime() + 1, beacon);
            DoSAttack = false;
        }

        if (attack == 1 || attack == 2) {
            if (attackPolicy == 0) {  // Persistent attack
                DoSAttack = true;
                scheduleAt(simTime() + beaconInterval, new cMessage("beacon"));
            } else if (attackPolicy == 1) {  // Periodic attack
                attackTimer = new cMessage("attackTimer"); // Unique timer for each node
                scheduleAt(simTime() + beaconInterval, attackTimer); // Schedule periodic attack based on beaconInterval
                scheduleAt(simTime() + beaconInterval, beacon);
            } else if (attackPolicy == 2) {  // Sporadic attack
                scheduleAt(simTime() + beaconInterval, beacon);

            }
        }
    }
}

void AttackApp::onWSM(BaseFrame1609_4 *frame) {

}

void AttackApp::handleSelfMsg(cMessage *msg) {
    if (attackPolicy == 0) {  // Persistent attack: Always active
        DoSAttack = true;
    } else if (attackPolicy == 1) {  // Periodic attack
        if (static_cast<int>(floor(simTime().dbl())) % atkInt == 0) { // Ensures integer-based interval checking
            DoSAttack = true;
        } else {
            DoSAttack = false;
        }
    } else if (attackPolicy == 2) {  // Sporadic attack (random probability)
        double randomValue = distribution(generator);
        if (randomValue <= sP) {
            DoSAttack = true;
        } else {
            DoSAttack = false;
        }
    }

    // Adjust beacon interval based on attack status
    if (simTime()>=920 && DoSAttack) {
        if (attack == 1) {
            beaconInterval.setRaw(OrgBeacon.raw() / nDos);

        }
        if (attack == 2) {
//            if (falseSpeed < 0){
//                           falseSpeed = 0;}
//            falseSpeed = falseSpeed-speedAtk;
            falseSpeed=speedAtk;
        }

        msgflag = 1;
         // Attack message
    } else {
//        if (attack == 1) {
        beaconInterval.setRaw(OrgBeacon.raw());
//    }
//        if (attack == 2) {
//        falseSpeed = -1;
//    }
        msgflag = 0;  // Normal message
    }



    // Generate and send a new beacon message
    if (msg->isName("beacon")) {
        vehID = mobility->getExternalId().c_str();
        vPos = mobility->getCurrentPosition();
        // Compute false speed inline

        TraCIDemo11pMessage *wsm = new TraCIDemo11pMessage();
        wsm->setDemoData("Attack Test");
        wsm->setVehicleId(vehID);
        wsm->setCarSenderPos(vPos);
        wsm->setDoSMsgFlag(msgflag);
        wsm->setLspeed(falseSpeed);
        wsm->setBeaInt(beaconInterval);
        populateWSM(wsm);
        sendDown(wsm);
    }

    // Reschedule only if the message was a beacon or attackTimer
    if (msg->isName("beacon") || msg->isName("attackTimer")) {
        scheduleAt(simTime() + beaconInterval, msg);
    }

}

void AttackApp::handlePositionUpdate(cObject *obj) {

}

void AttackApp::finish() {

    if (attackTimer) {
        cancelAndDelete(attackTimer);
        attackTimer = nullptr;
    }
//    if (beacon){
//        cancelAndDelete(beacon);
//        beacon = nullptr;
//    }

}
