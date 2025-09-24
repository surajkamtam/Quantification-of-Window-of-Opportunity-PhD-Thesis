import veins.base.utils.Coord;
import veins.modules.messages.BaseFrame1609_4;
import veins.base.utils.SimpleAddress;

namespace veins;

packet TraCIDemo11pMessage extends BaseFrame1609_4 {
    string demoData;
    LAddress::L2Type senderAddress = -1;
    string vehicleId;
    int serial = 0;
    double msgTime;
    Coord senderDist;
    int RFlag;
    Coord carSenderPos;
    double lspeed;
}