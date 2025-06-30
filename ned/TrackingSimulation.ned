package ned;

// Base imports from INET
import inet.node.inet.WirelessHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.applications.udpapp.UdpBasicApp;
import inet.applications.udpapp.UdpSink;
import inet.mobility.static.StationaryMobility;
import inet.mobility.single.RandomWaypointMobility;

// Custom applications

simple SensorTrackingApp extends UdpBasicApp
{
    parameters:
        string destAddress;  // IP address of the server/logger
        @display("i=block/app");
}

simple TrackingLoggerApp extends UdpSink
{
    parameters:
        string interfaceTable;
        @display("i=block/app");
}

simple MonitoringCenterApp extends UdpSink
{
    parameters:
        @display("i=block/app");

    gates:
        input in;
        output out;
}

module ServerWithLogger extends WirelessHost
{
}

network TrackingSimulation
{
    parameters:
        int numSensors = default(5);
        int numTargets = default(10);
        @display("bgi=background/streetmap;bgb=980,344;i=background/green");

    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,50");
        }

        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=258,25");
        }

        client: WirelessHost {
            @networkNode();
            @display("p=139,150;i=device/antennatower");
        }

        server: ServerWithLogger {
            @networkNode();
            @display("p=95,269;i=device/server");
        }

        sensor[numSensors]: WirelessHost {
            @networkNode();
            @display("i=misc/sensor;p=638,252");
        }

        target[numTargets]: WirelessHost {
            @networkNode();
            @display("i=misc/device;p=536,64");
        }

        monitoringCenter: WirelessHost {
            @networkNode();
            @display("p=299,180;i=block/network");
        }

    connections allowunconnected:
        // No explicit connections needed for wireless communication as radioMedium handles that
}
