#ifndef __LED_H__
#define __LED_H__

#define LED_CTL_PIN P17

#define LED_POWER_ON()   \
    do                   \
    {                    \
        LED_CTL_PIN = 1; \
    } while (0)
#define LED_POWER_OFF()  \
    do                   \
    {                    \
        LED_CTL_PIN = 0; \
    } while (0)

void led_init(void);

#endif