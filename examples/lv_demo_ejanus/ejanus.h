#ifndef EJANUS_H
#define EJANUS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

enum {
    C_OFF = 0x00,
    C_ON = 0x01,
    C_UNKNOWN = 0x02,
};
typedef uint8_t C_STATE;

enum {
    GEAR_P = 0x10,
    GEAR_R = 0x11,
    GEAR_N = 0x12,
    GEAR_D = 0x13,
    GEAR_UNKNOWN = 0x14
};
typedef uint8_t GEAR_STATE;

enum {
    LIGHT_CLOSE = 0x20,
    LIGHT_HIGH = 0x21,
    LIGHT_LOW = 0x22,
    LIGHT_FOG = 0x23,
    LIGHT_UNKNOWN = 0x24
};
typedef uint8_t LIGHT_STATE;

enum {
    TURN_ALL_OFF = 0x30,
    TURN_LEFT = 0x31,
    TURN_RIGHT = 0x32,
    TURN_ALL_ON = 0x33,
    TURN_UNKNOWN = 0x34
};
typedef uint8_t TURN_STATE;

enum {
    MEDIA_NONE = 0x40,
    MEDIA_FM = 0x41,
    MEDIA_AM = 0x42,
    MEDIA_MUSIC = 0x43,
    MEDIA_PHONE = 0x44,
    MEDIA_NAVI = 0x45,
    MEDIA_UNKNOWN = 0x46
};
typedef uint8_t MEDIA_STATE;

typedef struct {
    uint16_t left_front_press;
    uint16_t right_front_press;
    uint16_t left_behind_press;
    uint16_t right_behind_press;
    int8_t left_front_temp;
    int8_t right_front_temp;
    int8_t left_behind_temp;
    int8_t right_behind_temp;
} TIRE_PRESS_TEMP;

typedef struct {
    C_STATE abs;
    C_STATE ah;
    C_STATE eco;
    C_STATE egls;
    C_STATE epc;
    C_STATE espoff;
    C_STATE hdc;
    C_STATE sea1;
    C_STATE sea2;
} SYS_PROPERTY;

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a EJANUS application
 */
void ejanus_create();
void set_rpm(uint16_t rpm);
void set_speed(uint16_t speed);
void set_oil(uint16_t oil);
void set_water(int16_t water);

void set_trip_value(uint32_t trip);
void set_total_value(uint32_t total);

void set_gear_state(GEAR_STATE state);
void set_light_state(LIGHT_STATE state);
void set_turn_state(TURN_STATE state);
void set_media_state(MEDIA_STATE state);

void set_press_temp_value(TIRE_PRESS_TEMP property);

void set_sys_property_value(SYS_PROPERTY property);

void ejanus_start(uint32_t delayms);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //EJANUS_H
