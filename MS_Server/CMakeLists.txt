cmake_minimum_required(VERSION 3.19)
project(MS_Server LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network Sql)

qt_standard_project_setup()

qt_add_executable(MS_Server
    MS_Server/main.cpp
    MS_Server/Server.h
    MS_Server/Server.cpp
)

target_link_libraries(MS_Server
    PRIVATE
        Qt::Core
        Qt6::Network
        Qt6::Sql
)


include(GNUInstallDirs)

install(TARGETS MS_Server
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MS_Server
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
