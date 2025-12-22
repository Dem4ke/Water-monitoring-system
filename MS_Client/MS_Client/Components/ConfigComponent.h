#pragma once

#include <QString>

namespace Component {
// Class for configuring application by the user's choices
class ConfigComponent {
public:
    static ConfigComponent& Instance();

    void serialize();

    void setCurrentLanguage(int index);
    int getCurrentLanguage() const;

    void setVesselMonSysIndex(int index);
    int getVesselMonSysIndex() const;

    void setSearchTime(int seconds);
    int getSearchTime() const;

    void setSearchRadius(float radius);
    int getSearchRadius() const;

private:
    ConfigComponent();
    ~ConfigComponent() = default;

    void initializeDefault();

    const QString path_ = "Config/User.conf";   // Path to the configuration file
    int languageIndex_ = 0;                     // Index of current language used by the application
    int vesselMonSysIndex_ = 0;                 // Index of vessel's monitoring system in use
    int searchTimeSec_ = 3600;                  // Time in seconds that used to download data of chosen vessel
    float searchRadius_ = 100.f;                // Radius that used to find nearest vessels in kilometres
};

#define CONFIG ConfigComponent::Instance()
}
