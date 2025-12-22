#include <QFile>
#include <QDebug>
#include "ConfigComponent.h"

namespace Component {
ConfigComponent::ConfigComponent() {
    QFile readFromData;
    readFromData.setFileName(path_);

    if (!readFromData.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open" << path_;
        return;
    }

    // If the file is empty, it must be filled and serialized again
    if (readFromData.size() == 0) {
        initializeDefault();
    }

    QDataStream readStream(&readFromData);

    readStream >> languageIndex_;
    readStream >> vesselMonSysIndex_;
    readStream >> searchRadius_;

    readFromData.close();
}

// Static instance getter
ConfigComponent& ConfigComponent::Instance() {
    static ConfigComponent configComponent;
    return configComponent;
}

// Serialize the config file in a binary format
void ConfigComponent::serialize() {
    QFile writeInData;
    writeInData.setFileName(path_);

    if (!writeInData.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Cannot open" << path_;
        return;
    }

    writeInData.resize(0);

    QDataStream writeStream(&writeInData);

    writeStream << languageIndex_;
    writeStream << vesselMonSysIndex_;
    writeStream << searchRadius_;

    writeInData.close();
}


// Set index of current language used by the application
void ConfigComponent::setCurrentLanguage(int index) {
    languageIndex_ = index;
}

// Get index of current language used by the application
int ConfigComponent::getCurrentLanguage() const {
    return languageIndex_;
}

void ConfigComponent::setVesselMonSysIndex(int index) {
    vesselMonSysIndex_ = index;
}

int ConfigComponent::getVesselMonSysIndex() const {
    return vesselMonSysIndex_;
}

void ConfigComponent::setSearchTime(int seconds) {
    searchTimeSec_ = seconds;
}

int ConfigComponent::getSearchTime() const {
    return searchTimeSec_;
}

void ConfigComponent::setSearchRadius(float radius) {
    searchRadius_ = radius;
}

int ConfigComponent::getSearchRadius() const {
    return searchRadius_;
}

// Initialize default values
void ConfigComponent::initializeDefault() {
    languageIndex_ = 0;
    vesselMonSysIndex_ = 0;
    searchRadius_ = 100.f;

    serialize();
}
}
