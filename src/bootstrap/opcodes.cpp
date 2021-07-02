// extern void event1(const char params[],...);
#include "lua.h"
#include <iostream>

namespace opcode {

void event1(lua_State *L, ...) { puts("event1 -"); }

void event0(lua_State *L, ...) { puts("event0 - "); }

} // namespace opcode
