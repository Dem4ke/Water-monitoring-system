#include "PlotViewComponent.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QDir>

namespace Component {
PlotViewComponent::PlotViewComponent(QObject* parent, QQuickWidget* plot, QString plotName)
    : QObject(parent)
    , plot_(plot)
    , plotName_(plotName) {

    // QML widget start
    plot_->rootContext()->setContextProperty("plotData", this);
    plot_->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/MS_Client_QML/QML/Plot.qml"));

    emit changeNameRequest();
}

// Set plot points to draw
void PlotViewComponent::setPoints(const QVector<QDateTime>& timePoints, const QVector<double>& plotValues)
{
    points_.clear();

    if (timePoints.isEmpty() || plotValues.isEmpty())
        return;

    const int size = qMin(timePoints.size(), plotValues.size());

    minTime_ = timePoints.first().toMSecsSinceEpoch();
    maxTime_ = minTime_;

    minValue_ = plotValues.first();
    maxValue_ = minValue_;

    for (int i = 0; i < size; ++i) {
        const qint64 t = timePoints[i].toMSecsSinceEpoch();
        const double v = plotValues[i];

        minTime_ = qMin(minTime_, t);
        maxTime_ = qMax(maxTime_, t);
        minValue_ = qMin(minValue_, v);
        maxValue_ = qMax(maxValue_, v);

        QVariantMap point;
        point["x"] = t;
        point["y"] = v;
        points_.append(point);
    }

    emit redrawPlotRequest();
    emit rangeChanged();
}

const QVariantList& PlotViewComponent::getPoints() {
    return points_;
}

const QString& PlotViewComponent::getName() {
    return plotName_;
}

qint64 PlotViewComponent::minTime() const {
    return minTime_;
}

qint64 PlotViewComponent::maxTime() const {
    return maxTime_;
}

double PlotViewComponent::minValue() const {
    return minValue_;
}

double PlotViewComponent::maxValue() const {
    return maxValue_;
}
}
