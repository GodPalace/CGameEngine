cmake_minimum_required(VERSION 4.2)
project(CGameEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(CGameEngine SHARED
        ccontrol/Geometry.cpp
        ccontrol/Geometry.h
        cgutils/FastRandom.h
        cgutils/FastRandom.cpp
        ccontrol/Panel.cpp
        ccontrol/Panel.h
        cgame/console.h
        cgame/console.cpp
        cevents/IEvent.h
        cevents/Events.cpp
        cevents/Events.h
        cevents/EventBus.cpp
        cevents/EventBus.h
        ccontrol/Control.h
        ccontrol/Control.cpp
        ccontrol/Border.cpp
        ccontrol/Border.h
        cgutils/Logger.cpp
        cgutils/Logger.h
        ccontrol/Text.cpp
        ccontrol/Text.h
        ccontrol/Graphics.cpp
        ccontrol/Graphics.h
        ccontrol/FlowPanel.cpp
        ccontrol/FlowPanel.h
)

target_compile_definitions(CGameEngine PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

add_executable(CGameEngineTest test/main.cpp)
target_link_libraries(CGameEngineTest PRIVATE CGameEngine)
