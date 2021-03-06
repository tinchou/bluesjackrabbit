project(common)

set(SOURCES
        Configuration.cpp
        Configuration.h
        Constants.h
        CreateGameMessage.cpp
        CreateGameMessage.h
        DirectoryException.cpp
        DirectoryException.h
        DirectoryReader.cpp
        DirectoryReader.h
        GameFinishedMessage.cpp
        GameFinishedMessage.h
        GameInitMessage.cpp
        GameInitMessage.h
        GameObjectMessage.cpp
        GameObjectMessage.h
        GamesMessage.cpp
        GamesMessage.h
        InvalidMessageException.cpp
        InvalidMessageException.h
        JoinGameMessage.cpp
        JoinGameMessage.h
        LevelFinishedMessage.cpp
        LevelFinishedMessage.h
        Lock.cpp
        Lock.h
        Logger.cpp
        Logger.h
        LoggerScope.cpp
        LoggerScope.h
        MapLoader.cpp
        MapLoader.h
        MapsMessage.cpp
        MapsMessage.h
        Message.cpp
        Message.h
        MessageReader.cpp
        MessageReader.h
        MessageWriter.cpp
        MessageWriter.h
        Mutex.cpp
        Mutex.h
        PlayerInfoMessage.cpp
        PlayerInfoMessage.h
        Socket.cpp
        Socket.h
        Thread.cpp
        Thread.h
)
include(CppLint)

add_library(${PROJECT_NAME} ${SOURCES})
add_style_check_target(${PROJECT_NAME}-lint "${SOURCES}")
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-lint)
add_dependencies(${PROJECT_NAME} engine)

target_link_libraries(${PROJECT_NAME} ${LIBXML_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LOG4CPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} engine)


