#pragma once

#include "UdpSink.h"
#include <fstream>

namespace inet {

class TrackingLoggerApp : public UdpSink
{
  protected:
    std::ofstream logFile;

    virtual void initialize(int stage) override;
    virtual void processPacket(Packet *packet) override;
    virtual void finish() override;

};

} // namespace inet
