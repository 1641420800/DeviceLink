#ifndef _CIRCULARQUEUE_H_
#define _CIRCULARQUEUE_H_


#include <stdint.h>

#define Queue_Write(Name,data) Name##_Write(data)
#define Queue_Read(Name,data) Name##_Read(data)

#define NEW_queue(Name,Type,Size)			\
uint8_t Name##_buf[sizeof(Type) * (Size)];	\
uint8_t * Name##_read = Name##_buf;			\
uint8_t * Name##_write = Name##_buf;		\
void Name##_Write(Type c){					\
	uint8_t i;								\
	if(Name##_write + sizeof(Type) == Name##_read) Name##_read += sizeof(Type);	\
	for(i = 0; i < sizeof(Type); ++i){		\
		*Name##_write = ((uint8_t*)&c)[i];	\
		if(++Name##_write - Name##_buf >= sizeof(Name##_buf)) Name##_write = Name##_buf;						\
	}										\
}											\
char Name##_Read(Type * r){					\
	uint8_t i;								\
	if(Name##_write == Name##_read) return 0;	\
	for(i = 0; i < sizeof(Type); ++i){		\
		((uint8_t*)r)[i] = *Name##_read;	\
		if(++Name##_read - Name##_buf >= sizeof(Name##_buf)) Name##_read = Name##_buf;						\
	}										\
	return 1;								\
}


#endif
