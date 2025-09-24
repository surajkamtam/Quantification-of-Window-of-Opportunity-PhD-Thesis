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

#include <vasp/CSVWriter.h>
#include <vasp/connection/Manager.h>
#include <vasp/driver/CarApp.h>
#include <vasp/logging/TraceManager.h>
#include <vasp/messages/BasicSafetyMessage_m.h>

// V2X Applications
#include <vasp/safetyapps/EEBL.h>
#include <vasp/safetyapps/IMA.h>

// attacks
#include <vasp/attack/Type.h>
#include <vasp/attack/dimension/Type.h>
#include <vasp/attack/heading/Type.h>
// ghost vehicle based attacks
#include <vasp/attack/mobility/CommRangeBraking.h>
#include <vasp/attack/position/ghost_vehicle/SuddenAppearance.h>
#include <vasp/attack/position/ghost_vehicle/TargetedConstantPosition.h>
#include <vasp/attack/safetyapp/eebl/JustAttack.h>
#include <vasp/attack/safetyapp/eebl/StopAfterAttack.h>
// self telemetry based attacks
#include <vasp/attack/acceleration/Constant.h>
#include <vasp/attack/acceleration/ConstantOffset.h>
#include <vasp/attack/acceleration/High.h>
#include <vasp/attack/acceleration/Low.h>
#include <vasp/attack/acceleration/Random.h>
#include <vasp/attack/acceleration/RandomOffset.h>
#include <vasp/attack/channel/DenialOfService.h>
#include <vasp/attack/dimension/BadRatio.h>
#include <vasp/attack/dimension/ConstantOffset.h>
#include <vasp/attack/dimension/High.h>
#include <vasp/attack/dimension/Low.h>
#include <vasp/attack/dimension/Random.h>
#include <vasp/attack/dimension/RandomOffset.h>
#include <vasp/attack/heading/Constant.h>
#include <vasp/attack/heading/ConstantOffset.h>
#include <vasp/attack/heading/High.h>
#include <vasp/attack/heading/Low.h>
#include <vasp/attack/heading/Opposite.h>
#include <vasp/attack/heading/Perpendicular.h>
#include <vasp/attack/heading/Random.h>
#include <vasp/attack/heading/RandomOffset.h>
#include <vasp/attack/heading/Rotating.h>
#include <vasp/attack/position/self_telemetry/ConstantOffset.h>
#include <vasp/attack/position/self_telemetry/PlaygroundConstantPosition.h>
#include <vasp/attack/position/self_telemetry/Random.h>
#include <vasp/attack/position/self_telemetry/RandomOffset.h>
#include <vasp/attack/position/self_telemetry/SuddenDisappearance.h>
#include <vasp/attack/safetyapp/ima/HighAcceleration.h>
#include <vasp/attack/safetyapp/ima/HighSpeed.h>
#include <vasp/attack/safetyapp/ima/JunctionPosition.h>
#include <vasp/attack/safetyapp/ima/LowAcceleration.h>
#include <vasp/attack/safetyapp/ima/LowSpeed.h>
#include <vasp/attack/safetyapp/ima/PositionOffset.h>
#include <vasp/attack/speed/Constant.h>
#include <vasp/attack/speed/ConstantOffset.h>
#include <vasp/attack/speed/High.h>
#include <vasp/attack/speed/Low.h>
#include <vasp/attack/speed/Random.h>
#include <vasp/attack/speed/RandomOffset.h>

namespace vasp {
namespace driver {

Define_Module(CarApp);

void CarApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        attackType_ = par("attackType");
        maliciousProbability_ = attackType_ == attack::kAttackNo ? 0.0 : par("maliciousProbability");
        bsmData_ = par("bsmData").stdstringValue();
        simRunID_ = par("runID").stdstringValue();
        resultDir_ = par("resultDir").stdstringValue();
        mapFile_ = par("mapFile").stdstringValue();
    }

    if (stage == 1) {
        world_ = veins::FindModule<veins::BaseWorldUtility*>::findGlobalModule();
        connManager_ = veins::FindModule<connection::Manager*>::findGlobalModule();
        traceManager_ = veins::FindModule<logging::TraceManager*>::findGlobalModule();

        ghostVehicleDistance_ = connManager_->getInterfDist();

        // Load MAP
        std::ifstream mapFileStream{mapFile_};
        std::stringstream buffer{};
        if (mapFileStream) {
            buffer << mapFileStream.rdbuf();
            mapFileStream.close();
        }
        else {
            std::string errorMsg = "Unable to open map JSON file: \"" + mapFile_ + "\"";
            throw cRuntimeError(errorMsg.c_str());
        }
        mapJson_ = json::parse(buffer);

        // start IMA
        runIMA_ = std::make_shared<cMessage>("runIMA");
        scheduleAt(simTime() + 2, runIMA_.get());

        isMalicious_ = maliciousProbability_ >= dblrand();

        // only initialize attack if malicious
        if (!isMalicious_) {
            return;
        }
        attackPolicy_ = static_cast<attack::AttackPolicy>(par("attackPolicy").intValue());
        posAttackOffset_ = par("posAttackOffset");
        dimensionAttackOffset_ = par("dimensionAttackOffset");
        headingAttackOffset_ = par("headingAttackOffset");
        yawRateAttackOffset_ = par("yawRateAttackOffset");
        accelerationAttackOffset_ = par("accelerationAttackOffset");
        speedAttackOffset_ = par("speedAttackOffset");
        nDosMessages_ = par("nDosMessages");

        // handle random attack insertion
        sporadicInsertionRate_ = attackPolicy_ == attack::kAttackPolicySporadic ? par("sporadicInsertionRate") : 0.0;
        if (sporadicInsertionRate_ > 1 or sporadicInsertionRate_ < 0) {
            std::string const errorMsg{"sporadicInsertionRate should be within range [0, 1]; invalid input: " +
                std::to_string(sporadicInsertionRate_)};
            throw cRuntimeError(errorMsg.c_str());
        }

        // handle attack type selection if random attack selection
        if (attackType_ == attack::kAttackRandomlySelectedAttack) {
            attackType_ = static_cast<int>(uniform(attack::_kAttackMinValue + 1, attack::_kAttackMaxValue + 1));
        }
    }
}

void CarApp::finish()
{
    DemoBaseApplLayer::finish();
    cancelEvent(runIMA_.get());
}

void CarApp::handleSelfMsg(cMessage* msg)
{
    if (msg == runIMA_.get()) {
        runIMA();
        scheduleAt(simTime() + 2, runIMA_.get());
    }

    if (msg == sendBeaconEvt) {
        veins::BasicSafetyMessage* hvBsm = new veins::BasicSafetyMessage();
        populateWSM(hvBsm);

        if (isMalicious_) {
            int tmpAttackType{-1};
            if (attackType_ == attack::kAttackAlwaysRandomAttack) {
                tmpAttackType = attackType_;
                attackType_ = static_cast<int>(uniform(attack::_kAttackMinValue + 1, attack::_kAttackMaxValue + 1));
            }

            if ((attackPolicy_ == attack::kAttackPolicyPersistent) or // always attack
                (sporadicInsertionRate_ >= dblrand()) // sporadic attack
            ) {
                injectAttack(hvBsm);
            }

            prevBeaconTime_ = simTime();
            if (attackType_ != attack::kAttackSuddenDisappearance) {
                prevHvHeading_ = hvBsm->getHeading();
                sendDown(hvBsm);
            }
            attackType_ = tmpAttackType != -1 ? tmpAttackType : attackType_;
        }
        else {
            sendDown(hvBsm);
        }
        scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
    }

    if (msg == sendWSAEvt) {
        veins::DemoServiceAdvertisment* wsa = new veins::DemoServiceAdvertisment();
        populateWSM(wsa);
        sendDown(wsa);
        scheduleAt(simTime() + wsaInterval, sendWSAEvt);
    }
}

void CarApp::populateWSM(veins::BaseFrame1609_4* wsm, veins::LAddress::L2Type rcvId, int serial)
{
    DemoBaseApplLayer::populateWSM(wsm, rcvId, serial);

    if (veins::BasicSafetyMessage* bsm = dynamic_cast<veins::BasicSafetyMessage*>(wsm)) {
        bsm->setMsgCount(generatedBSMs % 128);
        bsm->setMsgGenerationTime(simTime().dbl());
        bsm->setAddress(myId);
        bsm->setRecipientId(rcvId);
        bsm->setAttackType("Genuine");
        bsm->setData(bsmData_.c_str());
        bsm->setHeading(mobility->getHeading());
        bsm->setYawRate(curYawRate_);
        bsm->setLength(traciVehicle->getLength());
        bsm->setWidth(traciVehicle->getWidth());
        bsm->setHeight(traciVehicle->getHeight());

        double const acceleration{traciVehicle->getAcceleration()};
        bsm->setAcceleration(acceleration);

        // AASHTO defines hard braking as a deceleration greater than 4.5 m/s^2
        double constexpr kDecelerationThreshold{-4.5}; // m/s^2
        bsm->setEventHardBraking(acceleration < kDecelerationThreshold);
    }
}

void CarApp::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

    if (lastUpdate_ == -1.0) {
        lastUpdate_ = simTime();
        return;
    }

    auto const updateInterval{simTime() - lastUpdate_};

    // calculate yaw rate
    auto const curAngleRad{mobility->getHeading().getRad()};
    if (lastAngleRad_ != -1.0) {
        curYawRate_ = (curAngleRad - lastAngleRad_) / updateInterval.dbl();
    }
    lastAngleRad_ = curAngleRad;
    lastUpdate_ = simTime();
}
void CarApp::injectAttack(veins::BasicSafetyMessage* hvBsm)
{
    using namespace vasp::attack;

    if (generatedBSMs == 0) {
        prevHvHeading_ = hvBsm->getHeading();
    }

    switch (attackType_) {
    case attack::kAttackPlaygroundConstantPosition: {
        attack_ = std::unique_ptr<position::PlaygroundConstantPosition>(
            new position::PlaygroundConstantPosition(world_));
        break;
    }
    case attack::kAttackConstantPositionOffset: {
        attack_ = std::unique_ptr<position::ConstantOffset>(
            new position::ConstantOffset(posAttackOffset_));
        break;
    }
    case attack::kAttackRandomPosition: {
        attack_ = std::unique_ptr<position::Random>(
            new position::Random(world_));
        break;
    }
    case attack::kAttackRandomPositionOffset: {
        attack_ = std::unique_ptr<position::RandomOffset>(
            new position::RandomOffset(posAttackOffset_));
        break;
    }
    case attack::kAttackSuddenDisappearance: {
        attack_ = std::unique_ptr<position::SuddenDisappearance>(
            new position::SuddenDisappearance());
        break;
    }
    case attack::kAttackDenialOfService: {
        attack_ = std::unique_ptr<channel::DenialOfService>(
            new channel::DenialOfService(beaconInterval, nDosMessages_));
        break;
    }
    case attack::kAttackIMAPosOffset: {
        attack_ = std::unique_ptr<safetyapp::ima::PositionOffset>(
            new safetyapp::ima::PositionOffset(approachingIntersection_));
        break;
    }
    case attack::kAttackIMAJunctionPos: {
        attack_ = std::unique_ptr<safetyapp::ima::JunctionPosition>(
            new safetyapp::ima::JunctionPosition(approachingIntersection_, junctionPos_));
        break;
    }
    case attack::kAttackIMAHighSpeed: {
        attack_ = std::unique_ptr<safetyapp::ima::HighSpeed>(
            new safetyapp::ima::HighSpeed(approachingIntersection_));
        break;
    }
    case attack::kAttackIMALowSpeed: {
        attack_ = std::unique_ptr<safetyapp::ima::LowSpeed>(
            new safetyapp::ima::LowSpeed(approachingIntersection_));
        break;
    }
    case attack::kAttackIMAHighAcceleration: {
        attack_ = std::unique_ptr<safetyapp::ima::HighAcceleration>(
            new safetyapp::ima::HighAcceleration(approachingIntersection_));
        break;
    }
    case attack::kAttackIMALowAcceleration: {
        attack_ = std::unique_ptr<safetyapp::ima::LowAcceleration>(
            new safetyapp::ima::LowAcceleration(approachingIntersection_));
        break;
    }
    }

    if (attack_) {
        attack_->attack(hvBsm);
    }
}


void CarApp::onBSM(veins::DemoSafetyMessage* dsm)
{
    auto rvBsm = dynamic_cast<veins::BasicSafetyMessage*>(dsm);
    if (rvBsm == nullptr) {
        return;
    }

    simtime_t const rvBsmReceiveTime{simTime()};

    if (isMalicious_) {
        // if a BSM is a ghost BSM then don't attack
        if (strcmp("ghost", rvBsm->getData()) == 0) {
            return;
        }

        if (attackPolicy_ == attack::kAttackPolicyPersistent or // always attack
            sporadicInsertionRate_ >= dblrand() // sporadic attack
        ) {
            injectGhostAttack(rvBsm);
        }

        return;
    }

    executeV2XApplications(rvBsm);
    writeTrace(rvBsm, rvBsmReceiveTime);
}

void CarApp::setUniqueGhostAddress(std::string const& key, veins::BasicSafetyMessage* ghostBsm)
{
    // set random, trackable and possibly unique ID for ghost
    if (ghostRvIdMap_.find(key) == ghostRvIdMap_.end()) {
        ghostRvIdMap_[key] = intrand(INT_MAX);
    }
    ghostBsm->setAddress(ghostRvIdMap_[key]);
}

void CarApp::setGhostMsgCount(std::string const& key, veins::BasicSafetyMessage* ghostBsm)
{
    // track message count per remote vehicle
    if (ghostMsgCountMap_.find(key) == ghostMsgCountMap_.end()) {
        ghostMsgCountMap_[key] = 0;
    }
    else {
        ghostMsgCountMap_[key] %= 128; // message count should not go beyond 127
    }
    ghostBsm->setMsgCount(ghostMsgCountMap_[key]);
    ghostMsgCountMap_[key]++;
}

void CarApp::injectGhostAttack(veins::BasicSafetyMessage const* rvBsm)
{
    using namespace vasp::attack;

    auto ghostBsm = new veins::BasicSafetyMessage();
    populateWSM(ghostBsm); // important to use this function so that receivers accept attack BSMs.
    ghostBsm->setRecipientId(rvBsm->getAddress());

    auto const mapKey{std::to_string(myId) + "-" + std::to_string(rvBsm->getAddress())};
    setUniqueGhostAddress(mapKey, ghostBsm);
    setGhostMsgCount(mapKey, ghostBsm);

    switch (attackType_) {
    case attack::kAttackSuddenAppearance: {
        ghostAttack_ = std::unique_ptr<position::SuddenAppearance>(new position::SuddenAppearance(rvBsm));
        break;
    }
    case attack::kAttackTargetedConstantPosition: {
        ghostAttack_ = std::unique_ptr<position::TargetedConstantPosition>(
            new position::TargetedConstantPosition(rvBsm, posAttackOffset_, ghostPos_, targetConstPosAttackFlag_));
        break;
    }
    case attack::kAttackCommRangeBraking: {
        ghostAttack_ = std::unique_ptr<mobility::CommRangeBraking>(
            new mobility::CommRangeBraking(rvBsm, ghostVehicleDistance_, curSpeed));
        break;
    }
    case attack::kAttackFakeEEBLJustAttack: {
        ghostAttack_ = std::unique_ptr<safetyapp::eebl::JustAttack>(new safetyapp::eebl::JustAttack(rvBsm));
        break;
    }
    case attack::kAttackFakeEEBLStopPositionUpdateAfterAttack: {
        ghostAttack_ = std::unique_ptr<safetyapp::eebl::StopAfterAttack>(new safetyapp::eebl::StopAfterAttack(rvBsm));
        break;
    }
    default: {
        delete ghostBsm;
        ghostBsm = nullptr;
    }
    }

    if (ghostAttack_) {
        ghostAttack_->attack(ghostBsm);
        sendDown(ghostBsm);
    }
}


void CarApp::writeTrace(veins::BasicSafetyMessage const* rvBsm, simtime_t_cref rvBsmReceiveTime)
{
    std::unique_ptr<veins::BasicSafetyMessage> hvBsm(new veins::BasicSafetyMessage());
    populateWSM(hvBsm.get());

    traceManager_->logTrace(rvBsm, hvBsm.get(), rvBsmReceiveTime, eeblWarning_, imaWarning_);
}


void CarApp::executeV2XApplications(veins::BasicSafetyMessage const* rvBsm)
{
    // EEBL
    vasp::safetyapps::EEBL eebl{};
    eeblWarning_ = eebl.warning(
        rvBsm,
        mobility->getPositionAt(simTime()),
        mobility->getHeading(),
        mobility->getHostSpeed(),
        myId);

    // IMA
    vasp::safetyapps::IMA ima{};
    imaWarning_ = approachingIntersection_ ? ima.warning(curPosition, curSpeed, rvBsm, junctionPos_) : false;
}

void CarApp::runIMA()
{
    auto currentRoad = mobility->getRoadId();

    for (auto& roadObj : mapJson_["roads"]) {
        auto road = roadObj["road"];
        // check if approaching an intersection
        approachingIntersection_ = road["id"] == currentRoad;
        if (approachingIntersection_) {
            // find junctionPos
            auto junction = road["junction"];
            junctionPos_ = veins::Coord(junction["x"], junction["y"]);
        }
    }
}

} // namespace driver
} // namespace vasp
