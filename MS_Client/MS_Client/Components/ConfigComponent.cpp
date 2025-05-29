#include <QFile>
#include <QDebug>
#include "ConfigComponent.h"

namespace Component {
ConfigComponent::ConfigComponent() {
    // Открытие файла
    QFile readFromData;
    readFromData.setFileName(path_);

    if (!readFromData.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open" << path_;
        return;
    }

    // Если файл пустой, загрузить базовые значения
    if (readFromData.size() == 0) {
        initializeDefault();
    }

    QDataStream readStream(&readFromData);

    readStream >> outputType_;          // Тип файла для рассчитанных значений
    readStream >> isOutputNeedToOpen_;  // Флаг, показатель нужно ли открыть файл с расчетом
    readStream >> languageIndex_;       // Индекс выбранного пользователем языка
    readStream >> colors_;              // Цвета, настраиваемые в диалоге настроек

    readFromData.close();
}

// Получение статического экземпляра класса
ConfigComponent& ConfigComponent::Instance() {
    static ConfigComponent configComponent;
    return configComponent;
}

// Сохранение конфига в бинарном файле
void ConfigComponent::serialize() {
    // Открытие файла
    QFile writeInData;
    writeInData.setFileName(path_);

    if (!writeInData.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open" << path_;
        return;
    }

    QDataStream writeStream(&writeInData);

    writeStream << outputType_;          // Тип файла для рассчитанных значений
    writeStream << isOutputNeedToOpen_;  // Флаг, показатель нужно ли открыть файл с расчетом
    writeStream << languageIndex_;       // Индекс выбранного пользователем языка
    writeStream << colors_;              // Цвета, настраиваемые в диалоге настроек

    writeInData.close();
}

// Задание индекса используемого типа вывода расчетов
void ConfigComponent::setOutputType(int outputType) {
    outputType_ = outputType;
}

// Получение индекса типа файла для вывода расчетов
int ConfigComponent::getOutputType() const {
    return outputType_;
}

// Задание стандартного положения флага-указателя, показывающего необходимость открыть расчет
void ConfigComponent::setOutputOpenFlag(bool isNeedToOpen) {
    isOutputNeedToOpen_ = isNeedToOpen;
}

// Получение стандартного положения флага-указателя, показывающего необходимость открыть расчет
bool ConfigComponent::isOutputNeedToOpen() const {
    return isOutputNeedToOpen_;
}

// Задание индекса используемого языка
void ConfigComponent::setCurrentLanguage(int languageIndex) {
    languageIndex_ = languageIndex;
}

// Получение индекса используемого языка
int ConfigComponent::getCurrentLanguage() const {
    return languageIndex_;
}

// Задание цветов из настроек
void ConfigComponent::setColors(const QMap<QString, QColor>& colors) {
    colors_ = colors;
}

// Получение структуры, хранящей цвета
const QMap<QString, QColor>& ConfigComponent::getColors() {
    return colors_;
}

// Получение цвета по его названию
const QColor &ConfigComponent::getColor(const QString &name) {
    return colors_[name];
}

// Инициализация стандартных значений
void ConfigComponent::initializeDefault() {
    outputType_ = 0;
    isOutputNeedToOpen_ = true;
    languageIndex_ = 0;

    colors_.clear();
    colors_.insert("Draft", QColor(33, 44, 255));
    colors_.insert("Background", QColor(0, 0, 0));

    serialize();
}
}
