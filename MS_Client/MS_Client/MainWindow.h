#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace Component {
class ProjectComponent;
class MapViewComponent;
class PlotViewComponent;
}

namespace MS {
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent, int vesselId);
    ~MainWindow();

private:
    Ui::MainWindow* ui_;                                                // Graphical user interface
    Component::ProjectComponent* projectComponent_ = nullptr;           // Project component
    Component::MapViewComponent* mapViewComponent_ = nullptr;           // Map view component
    Component::PlotViewComponent* windPlotViewComponent_ = nullptr;     // Plot view component that contais wind force plot
    Component::PlotViewComponent* wavePlotViewComponent_ = nullptr;     // Plot view component that contais wave height plot
};
}

