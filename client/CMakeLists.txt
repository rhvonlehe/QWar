find_package(Qt6 COMPONENTS Core Gui Qml REQUIRED)
qt_standard_project_setup()
qt_policy(SET QTP0001 OLD)

set(QML_FILES
    qml/Intro.qml
    qml/RootWindow.qml
)

set(CPP_FILES
    QWarClientApp.cpp
)

qt_add_executable(QWarClient
    main.cpp
)

qt_add_qml_module(QWarClient
    VERSION 1.0
    URI "QWarClient"
    QML_FILES ${QML_FILES}
    SOURCES ${CPP_FILES}
)

target_link_libraries(QWarClient
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
)

install(TARGETS QWarClient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#qt_finalize_target(QWarClient)

qt_generate_deploy_qml_app_script(
    TARGET QWarClient
    FILENAME_VARIABLE deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
)

message("deploy script name: ${deploy_script}")
message("qt_deploy_support: ${QT_DEPLOY_SUPPORT}")
install(SCRIPT ${deploy_script})


