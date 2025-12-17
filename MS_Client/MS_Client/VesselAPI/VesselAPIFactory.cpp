#include "VesselAPIFactory.h"
#include "NMEA2000VesselAPI.h"

namespace VesselAPI {
// Create a NMEA2000VesselAPI pointer
std::shared_ptr<IVesselAPI> NMEA2000VesselAPIFacrory::createVesselAPI() {
    return std::make_shared<NMEA2000VesselAPI>();
}
}
