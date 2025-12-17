#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace Component {
class ProjectComponent;
}

namespace MS {
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateLocations(const QVector<QString>& locations);

private:
    void getNearVesselLocation(const QPointF& location, float radius);

    Ui::MainWindow* ui_;                                        // Graphical user interface
    Component::ProjectComponent* projectComponent_ = nullptr;   // Project component
};
}

