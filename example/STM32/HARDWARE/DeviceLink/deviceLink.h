#ifndef _DEVICELINK_H_
#define _DEVICELINK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "core.h"
#include "tim.h"

void deviceLink_init(void);
void deviceLink_loop(void);

#ifdef __cplusplus
}
#endif
#endif
