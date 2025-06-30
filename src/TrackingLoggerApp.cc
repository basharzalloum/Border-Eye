#include "TrackingLoggerApp.h"

#include <inet/common/packet/Packet.h>
#include <inet/common/INETUtils.h>
#include <inet/common/packet/chunk/BytesChunk.h>

using namespace inet;
using namespace omnetpp;

Define_Module(TrackingLoggerApp);

void TrackingLoggerApp::initialize(int stage)
{
    UdpSink::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        std::string fname = par("logFile").stdstringValue();
        if (!fname.empty()) {
            logFile.open(fname);
            if (logFile.is_open()) {
                logFile << "timestamp,targetId,posX,posY,speed,label\n";
            }
        }
    }
}

void TrackingLoggerApp::processPacket(Packet *pkt)
{
    const auto& payload = pkt->peekData<BytesChunk>();
    std::string dataStr((const char*)payload->getBytes().data(), payload->getLength().get());

    EV_INFO << "Received tracking data: " << dataStr << endl;

    int targetId = -1;
    double posX = 0, posY = 0, speed = 0;

    std::istringstream ss(dataStr);
    std::string token;

    while (std::getline(ss, token, ',')) {
        size_t pos = token.find(":");
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string val = token.substr(pos + 1);
            if (key == "TID") targetId = std::stoi(val);
            else if (key == "X") posX = std::stod(val);
            else if (key == "Y") posY = std::stod(val);
            else if (key == "SPEED") speed = std::stod(val);
        }
    }

    std::string label = (speed > 4.0) ? "suspicious" : "normal";

    if (targetId != -1 && logFile.is_open()) {
        logFile << simTime().dbl() << ","
                << targetId << ","
                << posX << ","
                << posY << ","
                << speed << ","
                << label << "\n";
    }

    delete pkt;
}
