#ifndef SYS_H_
#define SYS_H_

typedef unsigned char	u8;

volatile typedef struct {
	unsigned char bit0 : 1;
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
}bit_type;					  	//Structure definition
typedef	union	{
	bit_type	bits;
		u8		byte;
}byte_type;

//Define data type: bit and byte share the same type
typedef	union {
	struct {
		unsigned char bit0:1;
		unsigned char bit1:1;
		unsigned char bit2:1;
		unsigned char bit3:1;
		unsigned char bit4:1;
		unsigned char bit5:1;
		unsigned char bit6:1;
		unsigned char bit7:1;
	}	bits;
	u8	byte;
}_byte_type;

#endif