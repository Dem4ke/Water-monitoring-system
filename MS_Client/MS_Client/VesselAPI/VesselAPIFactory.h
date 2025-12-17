#pragma once

#include <memory>

namespace VesselAPI {
class IVesselAPI;

// Vessel API factories
class IVesselAPIFactory {
public:
    virtual ~IVesselAPIFactory() = default;
    virtual std::shared_ptr<IVesselAPI> createVesselAPI() = 0;
};

class NMEA2000VesselAPIFacrory : public IVesselAPIFactory {
public:
    ~NMEA2000VesselAPIFacrory() = default;
    std::shared_ptr<IVesselAPI> createVesselAPI() override;
};
}
