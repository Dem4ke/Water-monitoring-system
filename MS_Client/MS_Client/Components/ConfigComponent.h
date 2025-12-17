#pragma once

#include <QString>

namespace Component {
// Class for configuring application by the user's choices
class ConfigComponent {
public:
    static ConfigComponent& Instance();

    void serialize();

    void setSearchRadius(float radius);
    int getSearchRadius() const;

    void setCurrentLanguage(int languageIndex);
    int getCurrentLanguage() const;

    void setVesselMonSysIndex();
    int getVesselMonSysIndex() const;

private:
    ConfigComponent();
    ~ConfigComponent() = default;

    void initializeDefault();

    const QString path_ = "Config/User.conf";   // Path to the configuration file
    int languageIndex_ = 0;                     // Index of current language used by the application
    int vesselMonSysIndex_ = 0;                 // Index of vessel's monitoring system in use
    float searchRadius_ = 100.f;                // Radius that used to find nearest vessels in kilometres
};

#define CONFIG ConfigComponent::Instance()
}
