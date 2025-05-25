cmake_minimum_required(VERSION 3.19)
project(LeviosaSQL LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(LeviosaSQL
    WIN32 MACOSX_BUNDLE
    main.cpp
    leviosa.cpp
    leviosa.h
    databaseloader.h databaseloader.cpp
    input.h input.cpp

)

target_link_libraries(LeviosaSQL
    PRIVATE
        Qt::Core
        Qt::Widgets
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
