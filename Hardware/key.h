#ifndef __KEY_H__
#define __KEY_H__

#include "typedef.h"

#define KEY_0_PIN P31 // SW0 检测脚
// #define KEY_1_PIN P02 // SW2 检测脚 

enum
{
    KEY_VAL_INVALID = 0x00, // 无效键值
    KEY_VAL_VALID_LOW ,
    KEY_VAL_VALID_HIGH,
};
typedef u8 key_val_t;

void key_init(void);
void key0_scan_handle(void);
// void key1_scan_handle(void);

#endif
