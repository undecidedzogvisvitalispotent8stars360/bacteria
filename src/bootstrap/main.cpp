#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<array>

using eventfun =  void(*)(const char params[], ...);
constexpr short opcode_size = 4;

using opcode_data = std::array<char, opcode_size>;

typedef struct{
	opcode_data data;
	eventfun fun;
}opcode;

extern opcode create_opcode(unsigned char a, unsigned char b, unsigned char c, unsigned char d, eventfun fun);
extern void event0(const char params[], ...);
extern void event1(const char params[],...);


static const opcode opcodes[]={ 
	{	{0x01,0x00,0x00,0x00}, event0	},
	{	{0x04,0x06,0x00,0x07}, event1  },
	{"",NULL}
};

void event1(const char params[],...){
	puts("event1");
}

void event0(const char params[], ...){
	puts("event0");
}


template <typename T, typename T1, size_t sarr> 
bool equal_array(std::array<T,sarr> arr, T1 data[]){
	for(unsigned short i = 0; i < sarr; i++){
		if(arr[i] != data[i]) return false;
	}
	return true;
}

template <typename T, typename T1, size_t sarr, size_t sdata> 
bool equal_array(std::array<T,sarr> arr, std::array<T,sdata> data){
	if(sdata != sarr) return false;
	equal_array( arr, data.to_array() );
}

void run_opcode(const char data[opcode_size]){
	for(auto op : opcodes){
		if(op.fun == NULL) break;
		if( equal_array(op.data, data) ){
			op.fun("");
			return;
		}

	}
	puts("opcode not found");
}

void run_opcode(opcode_data data){
	char d[opcode_size];
	for(unsigned short el =0; el < data.size(); el++)
		d[el] = data[el];
	run_opcode( d );
}


//#define INITDATAOPCODE(n, __VA_ARGS__) 
int
main(void)
{
	char data[4] = {0x01, 0x00, 0x0, 0x01};
 	opcode_data data1 = {0x01,0x00,0x00,0x00};
 	run_opcode(data);	
 	run_opcode(data1);	
	run_opcode(opcode_data{0x04,0x06,0x00,0x07});
}
