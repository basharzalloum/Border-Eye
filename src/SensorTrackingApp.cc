#include "SensorTrackingApp.h"
#include <inet/mobility/contract/IMobility.h>  // Needed for mobility access

namespace inet {

Define_Module(SensorTrackingApp);

void SensorTrackingApp::initialize(int stage)
{
    UdpBasicApp::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        lastUpdateTime = simTime();

        IMobility *mobility = check_and_cast<IMobility *>(getParentModule()->getSubmodule("mobility"));
        lastPosition = mobility->getCurrentPosition();

        logFile.open("tracking_log.csv", std::ios::out | std::ios::app);
        if (logFile.tellp() == 0)
            logFile << "time,posX,posY,speed,suspicious\n";
    }
}

void SensorTrackingApp::handleMessageWhenUp(cMessage *msg)
{
    UdpBasicApp::handleMessageWhenUp(msg);

    IMobility *mobility = check_and_cast<IMobility *>(getParentModule()->getSubmodule("mobility"));

    Coord currPos = mobility->getCurrentPosition();
    simtime_t currTime = simTime();

    double distance = currPos.distance(lastPosition);
    double timeDiff = (currTime - lastUpdateTime).dbl();
    double speed = (timeDiff > 0) ? distance / timeDiff : 0;

    int suspicious = (speed > 4.0) ? 1 : 0;

    logFile << currTime << ","
            << currPos.x << ","
            << currPos.y << ","
            << speed << ","
            << suspicious << "\n";

    lastPosition = currPos;
    lastUpdateTime = currTime;
}

void SensorTrackingApp::finish()
{
    if (logFile.is_open()) {
        logFile.flush();
        logFile.close();
    }

    UdpBasicApp::finish();
}

} // namespace inet
