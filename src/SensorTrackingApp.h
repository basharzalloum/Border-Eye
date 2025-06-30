#ifndef SENSORTRACKINGAPP_H_
#define SENSORTRACKINGAPP_H_

#include "inet/applications/udpapp/UdpBasicApp.h"
#include <fstream>
#include <inet/mobility/contract/IMobility.h>

namespace inet {

class SensorTrackingApp : public UdpBasicApp
{
  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;

  private:
    simtime_t lastUpdateTime;
    Coord lastPosition;
    std::ofstream logFile;
};

} // namespace inet

#endif // SENSORTRACKINGAPP_H_
