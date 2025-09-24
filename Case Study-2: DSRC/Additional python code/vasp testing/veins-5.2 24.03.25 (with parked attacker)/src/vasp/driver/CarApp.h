/*
 * MIT License
 *
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc., SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Project: V2X Application Spoofing Platform (VASP)
 * Author: Raashid Ansari
 * Email: quic_ransari@quicinc.com
 */

#pragma once

//#include <vasp/json.h>
#include <memory>
#include <omnetpp/simtime_t.h>
#include <string>
#include <vasp/attack/AttackPolicy.h>
#include <veins/modules/application/ieee80211p/DemoBaseApplLayer.h>

// forward declarations

namespace vasp {
namespace attack {
class Interface;
} // namespace attack

namespace logging {
class TraceManager;
} // namespace logging

namespace connection {
class Manager;
} // namespace connection
} // namespace vasp

//using json = nlohmann::json;

namespace veins {
class BasicSafetyMessage;
} // namespace veins

namespace vasp {
namespace driver {
class CarApp final : public veins::DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    void finish() override;

protected:
    void handleSelfMsg(cMessage* msg) override;
    void handlePositionUpdate(cObject* obj) override;
    void onBSM(veins::DemoSafetyMessage* dsm) override;
    void populateWSM(veins::BaseFrame1609_4* wsm, veins::LAddress::L2Type rcvId = veins::LAddress::L2BROADCAST(), int serial = 0) override;

private:
    void writeTrace(veins::BasicSafetyMessage const* rvBsm, simtime_t_cref rvBsmReceiveTime);
//    void runIMA();
    void executeV2XApplications(veins::BasicSafetyMessage const* rvBsm);
    void injectGhostAttack(veins::BasicSafetyMessage const* bsm);
    void injectAttack(veins::BasicSafetyMessage* bsm);
    void setUniqueGhostAddress(std::string const& key, veins::BasicSafetyMessage* ghostBsm);
    void setGhostMsgCount(std::string const& key, veins::BasicSafetyMessage* ghostBsm);

private:
    vasp::logging::TraceManager* traceManager_;
    veins::BaseWorldUtility* world_;
    vasp::connection::Manager* connManager_;

    std::string resultDir_;
    std::string simRunID_;
    std::string bsmData_;

    // IMA related
//    std::shared_ptr<cMessage> runIMA_{nullptr};
//    bool approachingIntersection_{false};
//    veins::Coord junctionPos_;
//    std::string mapFile_;
//    json mapJson_;

    // attack related
    int attackType_;
    std::shared_ptr<vasp::attack::Interface> attack_{nullptr};
    std::shared_ptr<vasp::attack::Interface> ghostAttack_{nullptr};
    int nDosMessages_;
    vasp::attack::AttackPolicy attackPolicy_;
    double sporadicInsertionRate_;
    double maliciousProbability_;
    bool isMalicious_;
    double posAttackOffset_{};
    double dimensionAttackOffset_{};
    double headingAttackOffset_{};
    double yawRateAttackOffset_{};
    double accelerationAttackOffset_{};
    double speedAttackOffset_{};

    // V2X apps related
    bool eeblWarning_{};
//    bool imaWarning_{};

    // yaw-rate calculation related
    simtime_t prevBeaconTime_{-1};
    veins::Heading prevHvHeading_{INFINITY};
    double curYawRate_{-1.0};
    simtime_t lastUpdate_{-1.0};
    double lastAngleRad_{-1.0};

    // ghost vehicle related
    std::map<std::string, int> ghostMsgCountMap_;
    std::map<std::string, long> ghostRvIdMap_;
    double ghostVehicleDistance_{0.0};
    veins::Coord ghostPos_;
    bool targetConstPosAttackFlag_{true};
};
} // namespace driver
} // namespace vasp
