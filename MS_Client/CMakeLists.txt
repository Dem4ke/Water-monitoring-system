cmake_minimum_required(VERSION 3.19)
project(MS_Client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools Network)

qt_standard_project_setup()

set(app_icon "Resources/MS_Client.rc")

qt_add_executable(MS_Client
    WIN32 MACOSX_BUNDLE
    ${app_icon}

    MS_Client/main.cpp

    MS_Client/Dialogs/UserDataHandler/UserLoginDlg.cpp
    MS_Client/Dialogs/UserDataHandler/UserLoginDlg.h
    MS_Client/Dialogs/UserDataHandler/UserLoginDlg.ui

    MS_Client/Components/ConfigComponent.h
    MS_Client/Components/ConfigComponent.cpp
    MS_Client/Components/LanguageComponent.h
    MS_Client/Components/LanguageComponent.cpp
    MS_Client/Components/SocketComponent.h MS_Client/Components/SocketComponent.cpp
    MS_Client/Dialogs/UserDataHandler/AccountCreationDlg.h MS_Client/Dialogs/UserDataHandler/AccountCreationDlg.cpp MS_Client/Dialogs/UserDataHandler/AccountCreationDlg.ui
)

qt_add_translations(
    TARGETS MS_Client
    TS_FILES MS_Client_ru_RU.ts
)

qt_create_translation(
    TARGETS MS_Client
    QM_FILES Languages/MS_Client_ru_RU.qm
)

add_dependencies(MS_Client update_translations)

target_link_libraries(MS_Client
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

include(GNUInstallDirs)

install(TARGETS MS_Client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MS_Client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
