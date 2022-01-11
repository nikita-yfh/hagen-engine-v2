set(IMGUI_SOURCES
	"imgui.cpp"
	"imgui_draw.cpp"
	"imgui_tables.cpp"
	"imgui_widgets.cpp"
	"backends/imgui_impl_sdl.cpp"
	"backends/imgui_impl_opengl3.cpp"
)
set(IMGUI_HEADERS
	"imgui.h"
	"imconfig.h"
	"imgui_internal.h"
	"imstb_rectpack.h"
	"imstb_textedit.h"
	"imstb_truetype.h"
	"backends/imgui_impl_sdl.h"
	"backends/imgui_impl_opengl3.h"
)
find_package(OpenGL REQUIRED)

include_directories(
	${OpenGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
)

add_library("imgui" SHARED ${IMGUI_SOURCES} ${IMGUI_HEADERS})

