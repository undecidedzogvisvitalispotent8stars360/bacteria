#ifndef OPCODES_H
#define OPCODES_H
#include"events.h"
//extern void event1(const char params[],...);
namespace opcode{
	constexpr short opcode_size = 4;
	constexpr auto ignorebyte = 0x0E;
	constexpr auto splitbyte = 0x0F;
	using eventfun = events::fun;
	using opcode_data = std::array<char, opcode_size>;

class opcode{
	protected:

	protected:
		opcode_data m_data;
		events::event m_event;
	public:
		bool operator==(opcode& a) noexcept{
			for(unsigned short i = 0; i < opcode_size; i++){
				if(a.m_data[i] != m_data[i]) return false;
			}
			return true;
		}

		template<typename T>
		bool operator==(T const a) noexcept{
			for(unsigned short i = 0; i < opcode_size; i++){
				if(a[i] != m_data[i]) return false;
			}
			return true;
		}
		template<typename T>
		opcode& operator=(T a[]) noexcept{
			unsigned long i =0;
			for(auto b : a){
				m_data[i++]=b;
			}
			return *this;
		}
		opcode(opcode_data data, eventfun fun):
		m_data(data), m_event(fun)
		{}
		events::event& getEvent(void) { return m_event; }



};

}

// events
namespace opcode{
	void event1(lua_State * L, int sock, const char * uIp, uint16_t uPort, char* buf, ...);
	void event0(lua_State * L, int sock, const char * uIp, uint16_t uPort, char* buf, ...);
};
#endif
