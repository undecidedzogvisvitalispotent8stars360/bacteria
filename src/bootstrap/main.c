#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZEOFOPCODE 5
#define OPCODEDEBUG

#define INITOPCODE(a,b,c,d,rfun)\
	{.o[0]=a, .o[1]=b, .o[2]=c, .o[3]=d, .o[4]=0x0, .fun=rfun}

#define OPCODEEQUAL(op,n,d,i) op[n].o[i] == d[i] 

#define FOPCODEEQUAL(op,n,d) OPCODEEQUAL(op,i,data,0) \
	&& OPCODEEQUAL(op,i,d,1) \
	&& OPCODEEQUAL(op,i,d,2) \
	&& OPCODEEQUAL(op,i,d,3) 


typedef void(*eventfun)(const char params[], ...);

typedef struct{
	char o[SIZEOFOPCODE];
	eventfun fun;
}opcode;

extern opcode create_opcode(unsigned char a, unsigned char b, unsigned char c, unsigned char d, eventfun fun);
extern void event0(const char params[], ...);

static const opcode opcodes[]={ 
	INITOPCODE(0x01,0x00,0x0,0x01, event0),
	{"",NULL}
};
void event0(const char params[], ...){
	puts("event0");
}

void run_opcode(const char data[]){
	for(unsigned int i = 0;i< sizeof(opcodes);i++){
		if(opcodes[i].fun == NULL) break;
		if( FOPCODEEQUAL(opcodes,i,data) ){
			printf("Found Opcode. start func\n");
			opcodes[i].fun("");
			return;
		}
	}			
	puts("opcode not found");
}


//#define INITDATAOPCODE(n, __VA_ARGS__) 
int
main(void)
{
	char data[4] = {0x01, 0x00, 0x0, 0x01};
	char data1[4] = {0x01, 0x00, 0x0, 0x00};
 	run_opcode(data);	
 	run_opcode(data1);	
}
