cmake_minimum_required(VERSION 4.2)
project(CGameEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(CGameEngine SHARED
        ccontrol/Geometry.cpp
        ccontrol/Geometry.h
        cgutil/FastRandom.h
        cgutil/FastRandom.cpp
        ccontrol/Panel.h
        cgame/console.h
        cgame/console.cpp
        cevent/IEvent.h
        cevent/Events.h
        cevent/EventBus.cpp
        cevent/EventBus.h
        ccontrol/Control.h
        ccontrol/Control.cpp
        ccontrol/Border.cpp
        ccontrol/Border.h
        cgutil/Logger.cpp
        cgutil/Logger.h
        ccontrol/Text.cpp
        ccontrol/Text.h
        ccontrol/Graphics.cpp
        ccontrol/Graphics.h
        ccontrol/FlowPanel.cpp
        ccontrol/FlowPanel.h
        ccontrol/GridPanel.cpp
        ccontrol/GridPanel.h
        ccontrol/VPanel.cpp
        ccontrol/VPanel.h
)

target_compile_definitions(CGameEngine PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

add_executable(CGameEngineTest test/main.cpp)
target_link_libraries(CGameEngineTest PRIVATE CGameEngine)
