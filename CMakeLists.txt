cmake_minimum_required(VERSION 3.19)
project(LeviosaSQL LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSSH REQUIRED libssh)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(LeviosaSQL
    WIN32 MACOSX_BUNDLE
    main.cpp
    leviosa.cpp
    leviosa.h
    databaseloader.h databaseloader.cpp
    input.h input.cpp
    sessionmanagerdialog.h sessionmanagerdialog.cpp
    sessionmanagercore.h sessionmanagercore.cpp
    sessionlistwidget.h sessionlistwidget.cpp
    multitabcodeeditor.h multitabcodeeditor.cpp
    simplecodeeditor.h simplecodeeditor.cpp
    syntaxhighlighter.h syntaxhighlighter.cpp
    resources.qrc
    sshcore.h sshcore.cpp
    sshworker.h sshworker.cpp
)

target_link_libraries(LeviosaSQL
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${LIBSSH_LIBRARIES}
)

include(GNUInstallDirs)

install(TARGETS LeviosaSQL
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LeviosaSQL
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
