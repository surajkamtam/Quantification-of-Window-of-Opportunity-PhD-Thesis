//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"
#include "veins/modules/application/traci/TraCIDemo11pRSUMessage_m.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
#include <string>

using namespace veins;
using namespace std;

Define_Module(veins::TraCIDemoRSU11p);
int malmsgcounter = 0;
int normalmsgcounter = 0;
int delayTime = 10;
int dt = 1;
int rsuCapacity;
int rsuValue=50;
int rsuFull=0;
bool isDelay = false;
Coord rPos2 = Coord(1025, 1525, 3);
double vdistance1;
Coord vPos1;

void TraCIDemoRSU11p::initialize(int stage)

{

    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        rsuCapacity = 0;
        scheduleAt(simTime() +uniform(0.01, 0.2) + dt, new cMessage("SendRSUMessages"));

    }

}

void TraCIDemoRSU11p::onWSM(BaseFrame1609_4 *frame) {

    char s2[] = "h";

    TraCIDemo11pMessage *wsm = dynamic_cast<TraCIDemo11pMessage*>(frame);
    vPos1=wsm->getSenderDist();
    vdistance1 = vPos1.distance(rPos2);
    if (!isDelay) {
    if (rsuCapacity < rsuValue) {
        // Process the received Malicious message
        int result = strncmp(wsm->getVehicleId(), s2, 1);
//cout<<"Vehicle ID and Change in results"<<wsm->getVehicleId()<<"\t"<<"Result 0 attacker:"<<result<<endl;
        if (result == 0) {

           cout<<wsm->getVehicleId()<<"\t"<<vdistance1<<"\t"<<simTime().dbl()<<" 1"<<endl;
            rsuCapacity++;
            malmsgcounter++;
            //        cout<<"malicious vehicle Mcounter: "<<malmsgcounter<<"\t"<<simTime().dbl()<<endl;
        }

        else {
            normalmsgcounter++;
            //        cout<<"malicious vehicle Ccounter: "<<normalmsgcounter<<"\t"<<simTime().dbl()<<endl;

        }

//            double pNonMalMsg = static_cast<double>(normalmsgcounter)
//                    / (normalmsgcounter + malmsgcounter);
//            double pMalMsg = 1 - pNonMalMsg;

// If the RSU's capacity reaches 50, initiate the pause
        if (rsuCapacity == rsuValue) {
            rsuFull++;
            // Disable message reception and transmission and Schedule the pause
            scheduleAt(simTime()+ uniform(0.01, 0.2) + 1, new cMessage("PauseRSUOperations"));


        }

    }
    }
//3 Communication output
//    cout<<"Number of Malicious Messages:"<<malmsgcounter<<"\t"<<"Number of Non-Malicious Messages:"<<normalmsgcounter<<"\t"<< "Number of times RSU Capacity Full:"<<rsuFull<<endl;

//        cout<<malmsgcounter<<"\t"<<simTime()<<endl;
//        cerr<<normalmsgcounter<<"\t"<<simTime()<<endl;

//    For RSU capacity graph
//        cout<<50-rsuCapacity<<"\t"<<simTime()<<endl;

    //    For RSU capacity graph red circles
//    if(50-rsuCapacity==0)
//    cout<<simTime()<<endl;

//    For Delay and time output
//    if (dt==1){
//        cerr<<0<<"\t"<<simTime()<<endl;
//    }
//    else{
//        cerr<<1<<"\t"<<simTime()<<endl;
//    }


    //For Malcious messages output
//    int result = strncmp(wsm->getVehicleId(), s2, 1);
//    if (result == 0){
//            cout<<1<<"\t"<<simTime()<<endl;
//        }
//        else{
//            cout<<0<<"\t"<<simTime()<<endl;
//        }
}

void TraCIDemoRSU11p::handleSelfMsg(cMessage *msg) {
    if (msg->isName("SendRSUMessages")) {
        TraCIDemo11pRSUMessage *rsm = new TraCIDemo11pRSUMessage();
        Coord pos =
                check_and_cast<BaseMobility*>(
                        getSimulation()->getModuleByPath("rsu[0].mobility"))->getCurrentPosition();
        populateWSM(rsm);
        rsm->setDemoData("Weather Warning");
        rsm->setRsuSenderPos(pos);
        rsm->setRSUId("1");
//            rsm->setNonMalMP(pNonMalMsg);
        rsm->setNonMalCounter(normalmsgcounter);
        rsm->setMalCounter(malmsgcounter);
//            rsm->setMalMP(pMalMsg);
        sendDown(rsm);





        // Schedule the next message transmission
        scheduleAt(simTime() + uniform(0.01, 0.2) + dt, msg);
    } else if (msg->isName("PauseRSUOperations")) {
        // Enable message reception and transmission after the pause
        // Schedule the next period of message transmission
        scheduleAt(simTime() + uniform(0.01, 0.2) + delayTime, new cMessage("DelayMessage"));
        dt = delayTime;

        // Reset RSU capacity after the pause
        rsuCapacity = 0;

        delete msg;

        isDelay = true;
    } else {
//        cout << "reached delay message: " << simTime() << endl;
        delete msg;
        dt = 1;
        isDelay = false;
    }

}

TraCIDemoRSU11p::~TraCIDemoRSU11p() {

}

