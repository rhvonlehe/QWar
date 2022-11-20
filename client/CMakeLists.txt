find_package(Qt6 COMPONENTS Core Gui Qml REQUIRED)

add_executable(QWarClient)

target_sources(QWarClient
    PRIVATE
    main.cpp
    QWarClientApp.cpp
)

target_link_libraries(QWarClient
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
)
