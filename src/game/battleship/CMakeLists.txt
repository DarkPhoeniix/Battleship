add_executable(ship_test
        ship.h
        ship.cpp
        ship_test.cpp)
target_link_libraries(ship_test Qt::Test)
add_test(NAME ShipTest COMMAND ship_test)
add_executable(map_test
        enumerations.h
        ship.h
        ship.cpp
        map.h
        map.cpp
        map_test.cpp)
target_link_libraries(map_test Qt::Test)
add_test(NAME MapTest COMMAND map_test)
add_executable(player_test
        enumerations.h
        ship.h
        ship.cpp
        map.h
        map.cpp
        player.h
        player.cpp
        player_test.cpp)
target_link_libraries(player_test Qt::Test)
add_test(NAME PlayerTest COMMAND player_test)
add_executable(battleshipgame_test
        enumerations.h
        ship.h
        ship.cpp
        map.h
        map.cpp
        player.h
        player.cpp
        battleshipgame.h
        battleshipgame.cpp
        battleshipgame_test.cpp)
target_link_libraries(battleshipgame_test Qt::Test)
add_test(NAME BattleshipGameTest COMMAND battleshipgame_test)
add_executable(AI_test
        enumerations.h
        ship.h
        ship.cpp
        map.h
        map.cpp
        AI/AI.h
        AI/AI.cpp
        AI/AI_test.cpp)
target_link_libraries(AI_test Qt::Test)
add_test(NAME AITest COMMAND AI_test)
