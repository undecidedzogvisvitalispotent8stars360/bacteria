#include "opcodes.h"
#include <iostream>
int main(void) {

#ifndef DISABLELUA
  lua_State *L = start_lua();
  lua::pushval(L, true, 1, 2, "string", "fs", true, false, 1.0, 3.14);

  lua_close(L);
#endif
  opcode::opcode_data data1 = {0x01, opcode::ignorebyte, opcode::ignorebyte,
                               0x01};

  opcode::opcode op{{0x01, opcode::ignorebyte, opcode::ignorebyte, 0x01},
                    opcode::event0};
  opcode::opcode op1{{0x02, opcode::ignorebyte, opcode::ignorebyte, 0x01},
                     NULL};
  opcode::opcode op2{{0x01, opcode::ignorebyte, opcode::ignorebyte, 0x01},
                     NULL};
  std::cout << (op == op2) << std::endl;
  std::cout << (op == data1) << std::endl;
  op.getEvent().run(NULL, "", NULL);

  return 0;
}
