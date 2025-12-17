#pragma once

#include <QObject>

class QTimer;

namespace MS {
class IProject;
}

namespace VesselAPI {
class IVesselAPI;
}

namespace Component {
// Component that contais project and all methods to handle it
class ProjectComponent : public QObject {
    Q_OBJECT

public:
    explicit ProjectComponent(QObject* parent = nullptr);

signals:

    // Здесь будет сбор информации от судового АПИ, таймер и сигнал для отправки информации на сервер

private slots:
    void dataUpdate();

private:
    std::shared_ptr<MS::IProject> project_ = nullptr;               // Project (vessel) that contains information
    std::shared_ptr<VesselAPI::IVesselAPI> vesselAPI_ = nullptr;    // Interface of vessel's monitoring system

    QTimer* timer_;                                                 // Timer that helps collect information
};
}
