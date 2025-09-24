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



#include "veins/modules/application/DoS/AttackSim.h"
#include "veins/modules/application/traci/TraCIDemo11pRSUMessage_m.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
#include <fstream>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <iostream>
#include <unordered_map>

using namespace veins;
using namespace std;



Define_Module(veins::AttackSim);

//std::ofstream out("attack.txt");
//int vTime [200], n;
//std::vector<double> vect;
//std::vector<std::string> vect1;






void AttackSim::initialize(int stage)

{

    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
                sentMessage = false;
                lastDroveAt = simTime();
                currentSubscribedServiceId = -1;
//                cout.rdbuf(out.rdbuf());
//                std::string name= getFullName();
//                std::string path= getFullPath();
//                std::cout<<"Name:"<<name<<std::endl;
//                std::cout<<"Path:"<<path<<std::endl;



    }


}

void AttackSim::onWSA(DemoServiceAdvertisment* wsa)
{
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
        currentSubscribedServiceId = wsa->getPsid();

        if (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService(static_cast<Channel>(wsa->getTargetChannel()), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }

}

void AttackSim::onWSM(BaseFrame1609_4* frame)

{
    TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);
    findHost()->getDisplayString().setTagArg("i", 1, "green");
    if (mobility->getRoadId()[0] != ':') traciVehicle->changeRoute(wsm ->getDemoData(), 9999);
    // End testing code
    if (!sentMessage) {
    sentMessage = true;
    // repeat the received traffic update once in 2 seconds plus some random delay
    wsm->setSenderAddress(myId);
    wsm->setSerial(3);
    scheduleAt(simTime() + 2 + uniform(0.01, 0.2), wsm->dup());
    }
}


void AttackSim::handleSelfMsg(cMessage* msg)

{
    if (TraCIDemo11pMessage* wsm = dynamic_cast<TraCIDemo11pMessage*>( msg)) {
    // send this message on the service channel until the counter is 3 or higher.
    // this code only runs when channel switching is enabled
    sendDown(wsm->dup());
    wsm->setSerial(wsm->getSerial() + 1);
    if (wsm->getSerial() >= 3) {
    // stop service advertisements
    stopService();
    delete (wsm);
    }
    else {
    scheduleAt(simTime() + 1, wsm);
    }
    }
    else {
    DemoBaseApplLayer::handleSelfMsg(msg);
    }

}


// Handle Position Update contains the attack.
void AttackSim::handlePositionUpdate(cObject* obj)

{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    if((simTime() >= 50 && simTime() < 74) ||
    (simTime() >= 210 && simTime() < 224)
    ) {
    for (int i = 0; i < 25000; i++){
    sentMessage = true;
    TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();

    populateWSM(wsm);
    wsm->setDemoData(mobility->getRoadId().c_str());
    sendDown(wsm);
    }
    }
    else {
    lastDroveAt = simTime();
    }

}







