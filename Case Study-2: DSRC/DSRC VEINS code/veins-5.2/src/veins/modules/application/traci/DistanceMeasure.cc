//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <veins/modules/application/traci/DistanceMeasure.h>
#include "veins/modules/application/traci/DistanceMeasureApp_m.h"
#include <cstring>

using namespace veins;

Define_Module(veins::DistanceMeasure);

//double firstVehX, firstVehY, firstVehZ;
//double firstVehSpeed, secVehSpeed;


void DistanceMeasure::initialize(int stage)

{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {

    // Schedule the first distance measurement after 10 seconds

    scheduleAt(simTime() + 10, new cMessage("measurement"));
    }

}


void DistanceMeasure::handleMsg(cMessage *msg)

{
    int f=0;
           f++;
           std::cout<<"Code outside "<<f<< endl;

    if (strcmp(msg->getName(), "measurement") == 0)

    {
       int c=0;
       c++;
       std::cout<<"Code inside "<<c<< endl;
        // Get the position of the car

        DistanceMeasureApp *carMobility = check_and_cast<DistanceMeasureApp *>(getModuleByPath("^.^.car[0].mobility"));

//        Coord carPos = mobility->getCurrentPosition();
        Coord carPos = carMobility->getSenderPos();
        std::cout<<"SenderPos "<<carPos<< " meters." << endl;



        // Get the position of the RSU

        DistanceMeasureApp *rsuMobility = check_and_cast<DistanceMeasureApp *>(getModuleByPath("^.^.rsu[0].mobility"));

        Coord rsuPos = rsuMobility->getRecipientPos();



        // Calculate the Euclidean distance between the car and the RSU

        double distance = carPos.distance(rsuPos);
        std::cout<<"The current distance between the car and the RSU is "<<distance<< " meters." << endl;

        EV << "The current distance between the car and the RSU is " << distance << " meters." << endl;



        // Schedule the next distance measurement

        scheduleAt(simTime() + 10, msg);

    }

}

void DistanceMeasure::handlePositionUpdate(cObject* obj, cMessage *msg)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

    int f=0;
              f++;
              std::cout<<"Code outside "<<f<< endl;

       if (strcmp(msg->getName(), "measurement") == 0)

       {
          int c=0;
          c++;
          std::cout<<"Code inside "<<c<< endl;
           // Get the position of the car

           DistanceMeasureApp *carMobility = check_and_cast<DistanceMeasureApp *>(getModuleByPath("^.^.car[0].mobility"));

   //        Coord carPos = mobility->getCurrentPosition();
           Coord carPos = carMobility->getSenderPos();
           std::cout<<"SenderPos "<<carPos<< " meters." << endl;



           // Get the position of the RSU

           DistanceMeasureApp *rsuMobility = check_and_cast<DistanceMeasureApp *>(getModuleByPath("^.^.rsu[0].mobility"));

           Coord rsuPos = rsuMobility->getRecipientPos();



           // Calculate the Euclidean distance between the car and the RSU

           double distance = carPos.distance(rsuPos);
           std::cout<<"The current distance between the car and the RSU is "<<distance<< " meters." << endl;

           EV << "The current distance between the car and the RSU is " << distance << " meters." << endl;



           // Schedule the next distance measurement

           scheduleAt(simTime() + 10, msg);

       }

//    DistanceMeasureApp* wsm = new DistanceMeasureApp();
//               populateWSM(wsm);
////               wsm->setDemoData(mobility->getRoadId().c_str());
//
//    std::string vehID = mobility->getExternalId().c_str();
//    std::cout<<"vehID:"<<vehID<<std::endl;
//    if (vehID == "vehicle_0"){
//             firstVehX = mobility->getCurrentPosition().x;
//             firstVehY = mobility->getCurrentPosition().y;
//             firstVehZ = mobility->getCurrentPosition().z;
//             firstVehSpeed = mobility->getSpeed();
//             std::cout<<"Position x:"<<firstVehX<<std::endl;
//             std::cout<<"Position y:"<<firstVehY<<std::endl;
//             std::cout<<"Position z:"<<firstVehZ<<std::endl;
//             std::cout<<"Vehicle 0 Speed:"<<firstVehSpeed<<std::endl;
//
//    }


}


//DistanceMeasure::DistanceMeasure() {
//    // TODO Auto-generated constructor stub
//
//}
//
//DistanceMeasure::~DistanceMeasure() {
//    // TODO Auto-generated destructor stub
//}

