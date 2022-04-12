set(ENGINE "engine")
set(ENGINE_SOURCES
	"Camera.cpp"
	"Engine.cpp"
	"Level.cpp"
	"Input.cpp"
	"GameConfig.cpp"
	"Settings.cpp"
	"Interface.cpp"
	"Style.cpp"
	"GUIConsole.cpp"
	"GUISettings.cpp"
	"main.cpp"
)
set(ENGINE_HEADERS
	"b2_user_settings.h"
	"Camera.hpp"
	"Engine.hpp"
	"Level.hpp"
	"Input.hpp"
	"GameConfig.hpp"
	"Settings.hpp"
	"Interface.hpp"
	"Style.hpp"
	"GUIConsole.hpp"
	"GUISettings.hpp"
)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
	${SDL2_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
	${BOX2D_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
)
add_executable(${ENGINE} ${ENGINE_SOURCES} ${ENGINE_HEADERS})
target_link_libraries(${ENGINE}
	${LUA_LIBRARIES}
	${SDL2_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	"glad"
	"imgui"
	"box2d"
	"core"
)
	
