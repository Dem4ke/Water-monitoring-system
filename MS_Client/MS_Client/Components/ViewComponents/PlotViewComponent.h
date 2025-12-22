#pragma once

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QDateTime>

class QQuickWidget;

namespace Component {
// Component that handle plot QML widget data
class PlotViewComponent : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariantList points READ getPoints NOTIFY redrawPlotRequest)
    Q_PROPERTY(QString name READ getName NOTIFY changeNameRequest)
    Q_PROPERTY(qint64 minTime READ minTime NOTIFY rangeChanged)
    Q_PROPERTY(qint64 maxTime READ maxTime NOTIFY rangeChanged)
    Q_PROPERTY(double minValue READ minValue NOTIFY rangeChanged)
    Q_PROPERTY(double maxValue READ maxValue NOTIFY rangeChanged)

public:
    PlotViewComponent(QObject* parent, QQuickWidget* plot, QString plotName);

    void setPoints(const QVector<QDateTime>& timePoints, const QVector<double>& plotValues);
    const QVariantList& getPoints();
    const QString& getName();

    qint64 minTime() const;
    qint64 maxTime() const;
    double minValue() const;
    double maxValue() const;

signals:
    void redrawPlotRequest();
    void changeNameRequest();
    void rangeChanged();

private:
    QQuickWidget* plot_;        // Plot QML

    QVariantList points_;
    QString plotName_;

    qint64 minTime_ = 0;
    qint64 maxTime_ = 0;
    double minValue_ = 0.0;
    double maxValue_ = 0.0;
};
}
