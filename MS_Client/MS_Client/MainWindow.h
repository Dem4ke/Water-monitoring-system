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

private slots:
    void mapUpdate(const QPointF& vesselLocation, const QVector<QPointF>& locations);

private:
    Ui::MainWindow* ui_;                                        // Graphical user interface
    Component::ProjectComponent* projectComponent_ = nullptr;   // Project component
};
}

