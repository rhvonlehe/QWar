find_package(Qt6 COMPONENTS Core Gui Qml REQUIRED)
set(CMAKE_AUTOMOC ON)

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
