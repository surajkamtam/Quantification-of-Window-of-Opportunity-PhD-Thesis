#include "veins/modules/application/traci/TargetApp.h"
#include "veins/modules/application/traci/TraCIDemo11pRSUMessage_m.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <random>

using namespace veins;
using namespace std;

Define_Module(veins::TargetApp);

Coord targetvPos;
const char * AttackerID;
bool targetDoSAttack = false;
int targetmsgflag;
simtime_t targetBeaInt;
double targetspeed;
//
const char *targetvehID;
double targetsP;  // Attack success probability
simtime_t targetbeaconInterval;  // Beacon interval (time between periodic beacon broadcasts)
cMessage *targetattackTimer = nullptr;  // Timer for periodic attacks
//ofstream out("out.txt", ios::out | ios::trunc);
double oldspeed=0;
int persistattackstart=0;
int atackcount=0;
bool attackHandled = true;


void TargetApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
//        cout.rdbuf(out.rdbuf());
    }



    if (stage == 1) {
        targetsP = par("successProbability");  // Probability of attack
        targetmessageReceiveVector.setName("Target Received Messages Timestamp");
        beaconIntervalVector.setName("Beacon Intervals Timestamp");
//        oldspeed = mobility->getSpeed();
    }
}

void TargetApp::onWSM(BaseFrame1609_4* frame) {
    TraCIDemo11pMessage* wsm = dynamic_cast<TraCIDemo11pMessage*>(frame);


    if (wsm) {
        targetmsgflag = wsm->getDoSMsgFlag();
        AttackerID= wsm->getVehicleId();
        targetBeaInt= wsm->getBeaInt();
//        cout<<targetspeed<<endl;
        persistattackstart= wsm->getAttackstart();
//        if(simTime()>=920 && strcmp(AttackerID, "h0") == 0){
        if(simTime()>=persistattackstart && strcmp(AttackerID, "h0") == 0){
              targetspeed = wsm->getLspeed();
              traciVehicle->setDecel(1);
//              atackcount++;
//              attackHandled = true;
//              cout<<"Loop 1"<<endl;
        }


//        else if (simTime()>persistattackstart+20 && atackcount==1){
//            atackcount++;
//            traciVehicle->setSpeed(oldspeed+5);
//            attackHandled = false;
//            cout<<"Loop 2"<<endl;
//        }else if (!attackHandled){
////            traciVehicle->setMaxSpeed();
//            attackHandled = true;
//            cout<<"Loop 3"<<endl;
//        }
//              traciVehicle->slowDown(1, 20);


//                  traciVehicle->setSpeed(targetspeed);


//                  traciVehicle->setSpeed(oldspeed+2);
//                  traciVehicle->slowDown(oldspeed, 15);




        }


        targetmessageReceiveVector.record(targetmsgflag);  // Record the message reception for plotting
        beaconIntervalVector.record(targetBeaInt);
    }


void TargetApp::handleSelfMsg(cMessage* msg) {

}

void TargetApp::handlePositionUpdate(cObject* obj) {
    TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();




}


void TargetApp::finish() {

}
