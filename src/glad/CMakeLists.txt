set(GLAD "glad")
set(GLAD_HEADERS
	"glad.h"
	"khrplatform.h"
)
set(GLAD_SOURCES
	"glad.c"
)
find_package(OpenGL REQUIRED)
include_directories(
	${OpenGL_INCLUDE_DIR}
)
add_library(${GLAD} STATIC ${GLAD_SOURCES} ${GLAD_HEADERS})
target_link_libraries(${GLAD} ${CMAKE_DL_LIBS})
