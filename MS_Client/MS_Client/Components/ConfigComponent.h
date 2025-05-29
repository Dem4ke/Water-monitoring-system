#pragma once

#include <QString>
#include <QColor>
#include <QMap>

namespace Component {
// Класс для конфигурирования проекта под пользовательские настройки
class ConfigComponent {
public:
    static ConfigComponent& Instance();

    void serialize();

    void setOutputType(int outputType);
    int getOutputType() const;

    void setOutputOpenFlag(bool isNeedToOpen);
    bool isOutputNeedToOpen() const;

    void setCurrentLanguage(int languageIndex);
    int getCurrentLanguage() const;

    void setColors(const QMap<QString, QColor>& colors);
    const QMap<QString, QColor>& getColors();
    const QColor& getColor(const QString& name);

private:
    ConfigComponent();
    ~ConfigComponent() = default;

    void initializeDefault();

    const QString path_ = "Config/User.conf";   // Путь до файла с конфигом

    int outputType_ = 0;                        // Тип файла для рассчитанных значений
    bool isOutputNeedToOpen_ = true;            // Флаг, показатель нужно ли открыть файл с расчетом

    int languageIndex_ = 0;                     // Индекс выбранного пользователем языка
    QMap<QString, QColor> colors_;              // Цвета, настраиваемые в диалоге настроек
};

#define CONFIG ConfigComponent::Instance()
}
