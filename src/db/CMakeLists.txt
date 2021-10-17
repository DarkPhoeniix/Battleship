add_executable(database_test
        database.h
        database.cpp
        database_test.cpp)
target_link_libraries(database_test
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
        Qt::Test)
add_test(NAME DatabaseTest COMMAND database_test)
