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

#pragma once

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/utility/TimerManager.h"

//#include "veins/base/utils/FindModule.h"//added
//#include "veins/modules/mac/ieee80211p/Mac1609_4.h"//added

//#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;
   using veins::TimerManager;

namespace veins {

/**
 * Small RSU Demo using 11p
 */
class VEINS_API TraCIDemoRSU11p : public DemoBaseApplLayer {
public:
    ~TraCIDemoRSU11p();
    void initialize(int stage) override;
//    Mac1609_4* mac;//added
protected:
  void onWSM(BaseFrame1609_4* wsm) override;
  void handleSelfMsg(cMessage* msg) override;
  TimerManager timerManager{this};
     TimerManager::TimerHandle handle;
//    void onWSA(DemoServiceAdvertisment* wsa) override;
//    virtual void handleMessage(BaseFrame1609_4* wsm);
};

} // namespace veins
