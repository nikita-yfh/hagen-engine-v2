#include <cstdarg>
#include <cstdint>

#define b2_lengthUnitsPerMeter 1.0f
#define b2_maxPolygonVertices	4

namespace luabridge{
	class LuaRef;
};
struct B2_API b2BodyUserData {
};

struct B2_API b2FixtureUserData {
};

struct B2_API b2JointUserData {
};

B2_API void* b2Alloc_Default(int32 size);
B2_API void b2Free_Default(void* mem);

inline void* b2Alloc(int32 size) {
	return b2Alloc_Default(size);
}

inline void b2Free(void* mem) {
	b2Free_Default(mem);
}

B2_API void b2Log_Default(const char* string, va_list args);

inline void b2Log(const char* string, ...) {
	va_list args;
	va_start(args, string);

	b2Log_Default(string, args);
	va_end(args);
}
