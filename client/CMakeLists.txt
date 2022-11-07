find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(QWarClient)

target_sources(QWarClient
    PRIVATE
    main.cpp
    QWarClientApp.cpp
)

target_link_libraries(QWarClient
    PRIVATE
    Qt5::Core
    Qt5::Gui
)
