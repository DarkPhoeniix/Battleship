add_executable(battleship
                main.cpp
                game/generic_interfaces.h
                game/battleship/abstract_game.h
                game/battleship/enumerations.h
                widgets/fieldwidget.h
                widgets/fieldwidget.cpp
                windows/mainwindow.h
                windows/mainwindow.cpp
                windows/mainwindow.ui
                db/database.h
                db/database.cpp)
include_directories(.)
target_link_libraries(battleship
		Qt::Core
		Qt::Gui
		Qt::Widgets
                Qt::Sql
)

add_executable(database_test
                db/database.h
                db/database.cpp
                db/database_test.cpp)
add_executable(ship_test
                game/battleship/ship.h
                game/battleship/ship.cpp
                game/battleship/ship_test.cpp)
add_executable(map_test
                game/battleship/enumerations.h
                game/battleship/ship.h
                game/battleship/ship.cpp
                game/battleship/map.h
                game/battleship/map.cpp
                game/battleship/map_test.cpp)
add_executable(player_test
                game/battleship/enumerations.h
                game/battleship/ship.h
                game/battleship/ship.cpp
                game/battleship/map.h
                game/battleship/map.cpp
                game/battleship/player.h
                game/battleship/player.cpp
                game/battleship/player_test.cpp)

target_link_libraries(database_test
                Qt::Core
                Qt::Gui
                Qt::Widgets
                Qt::Sql
                Qt::Test
)
target_link_libraries(ship_test
                Qt::Test
)
target_link_libraries(map_test
                Qt::Test
)
target_link_libraries(player_test
                Qt::Test
)

add_test(NAME DatabaseTest COMMAND database_test)
add_test(NAME ShipTest COMMAND ship_test)
add_test(NAME MapTest COMMAND map_test)
add_test(NAME PlayerTest COMMAND player_test)
