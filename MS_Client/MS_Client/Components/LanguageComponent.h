#pragma once

#include <QTranslator>
#include <QMap>

namespace Component {
class LanguageComponent {
public:
    static LanguageComponent& Instance();

    void changeTranlator(int languageIndex);

private:
    LanguageComponent();
    ~LanguageComponent();

    QTranslator* appTranslator_;        // Транслятор для перевода приложения средствами Qt
    QTranslator* dialogTranslator_;     // Транслятор для перевода стандартных диалоговых окон в Qt
    QMap<int, QString> languages_;      // Хэш таблица, хранящая языки, поддерживаемые приложением и ключи для их переключения
};

#define LANGUAGE LanguageComponent::Instance()
}
