
#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "include.h"

#define U8_MAX_VAL ((const u8)0xFF)
#define U16_MAX_VAL ((const u16)0xFFFF)

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define USER_DEBUG_ENABLE 0     // TEST_ONLY 打开用户调试，包含使用demo板上的引脚
#define USER_DEBUG_PIN_ENABLE 0 // TEST_ONLY 是否使用demo板来调试，使用demo板的引脚

#if ((0 == USER_DEBUG_ENABLE) && USER_DEBUG_PIN_ENABLE)
#error USER_DEBUG_PIN_ENABLE need (USER_DEBUG_ENABLE == 1)
#endif

/*
    产品类型
    PRODUCT_TYPE_1 ，检测到低电平让电机正转，检测到高电平或者是悬空，让电机反转
    PRODUCT_TYPE_2 ，检测到低电平让电机反转，检测到高电平或者是悬空，让电机正转
 */
#define PRODUCT_TYPE_INVALID 0x00
#define PRODUCT_TYPE_1 0x01
#define PRODUCT_TYPE_2 0x02
#define PRODUCT_TYPE PRODUCT_TYPE_2

#if (PRODUCT_TYPE == PRODUCT_TYPE_INVALID)
#error "product type invalid"
#endif

#if USER_DEBUG_ENABLE
#include <stdio.h>
#endif

#endif