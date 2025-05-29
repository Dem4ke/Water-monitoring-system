#include "LanguageComponent.h"

#include <QApplication>
#include <QLibraryInfo>

namespace Component {
LanguageComponent::LanguageComponent() {
    // Add supportable languages (English & Russian)
    languages_.insert(0, "en_EN");
    languages_.insert(1, "ru_RU");
}

LanguageComponent::~LanguageComponent() {
    delete appTranslator_;
    delete dialogTranslator_;
}

// Получение статического экземпляра класса
LanguageComponent& LanguageComponent::Instance() {
    static LanguageComponent languageComponent;
    return languageComponent;
}

// Изменение транслятора приложения, исходя из выбранного индекса
void LanguageComponent::changeTranlator(int languageIndex) {
    QApplication::removeTranslator(appTranslator_);
    QApplication::removeTranslator(dialogTranslator_);

    appTranslator_ = new QTranslator();
    dialogTranslator_ = new QTranslator();

    QString applicationLanguage = QApplication::applicationName() + "_" + languages_.value(languageIndex);

    // Загрузка текста, редактируемого в Qt linguist
    if (appTranslator_->load(applicationLanguage)) {
        QApplication::installTranslator(appTranslator_);
    }

    // Загрузка текста для стандратных диалогов, которые не переводятся в Qt Linguist
    if (dialogTranslator_->load("qt_" + languages_.value(languageIndex),
        QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {

        QApplication::installTranslator(dialogTranslator_);
    }
}
}
