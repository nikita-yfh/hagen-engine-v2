set(ENGINE "engine")
set(ENGINE_SOURCES
	"AudioManager.cpp"
	"Color.cpp"
	"Camera.cpp"
	"Controls.cpp"
	"Engine.cpp"
	"GameConfig.cpp"
	"GUIConsole.cpp"
	"GUISettings.cpp"
	"Interface.cpp"
	"main.cpp"
	"RenderManager.cpp"
	"Settings.cpp"
)
set(ENGINE_HEADERS
	"AudioManager.hpp"
	"b2_user_settings.h"
	"Color.hpp"
	"Camera.hpp"
	"Controls.hpp"
	"Engine.hpp"
	"GameConfig.hpp"
	"GUIConsole.hpp"
	"GUISettings.hpp"
	"Interface.hpp"
	"RenderManager.hpp"
	"Settings.hpp"
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
	
