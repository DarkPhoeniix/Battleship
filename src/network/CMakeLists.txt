add_executable(server_wrapper_test
        server_wrapper.h
        server_wrapper.cpp
        server_wrapper_test.cpp
        ../game/battleship/battleshipgame.cpp
        ../game/battleship/player.cpp
        ../game/battleship/ship.cpp
        ../game/battleship/map.cpp)
target_link_libraries(server_wrapper_test rpc Qt::Test)
add_test(NAME ServerWrapperTest COMMAND server_wrapper_test)
