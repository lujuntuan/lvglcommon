#include "ejanus2.h"
#include "lv_custom/lv_custom.h"

int main(void)
{
#ifdef USE_OPENWFD
    //putenv("WFD_CLIENT_ID=0x7812");
    //putenv("WFD_PORT_INDEX=0");
    //putenv("WFD_PORT_MODE_INDEX=0");
    //putenv("WFD_PIPELINE_ID=15");
#endif

    lv_custom_init(LV_CUSTOM_DISP);

    ejanus2_create();

    set_trip_value(99998000);
    set_total_value(99998000);
    set_gear_state(GEAR_P);
    set_turn_state(TURN_ALL_ON);
    set_light_state(LIGHT_LOW);

    TIRE_PRESS_TEMP press_temp;
    press_temp.left_front_press = 145;
    press_temp.left_front_temp = 27;

    press_temp.right_front_press = 240;
    press_temp.right_front_temp = 26;

    press_temp.left_behind_press = 250;
    press_temp.left_behind_temp = 27;

    press_temp.right_behind_press = 80;
    press_temp.right_behind_temp = 27;

    set_press_temp_value(press_temp);

    SYS_PROPERTY sys_property;
    sys_property.abs = C_ON;
    sys_property.ah = C_ON;
    sys_property.eco = C_ON;
    sys_property.egls = C_ON;
    sys_property.epc = C_ON;
    sys_property.espoff = C_ON;
    sys_property.hdc = C_ON;
    sys_property.sea1 = C_ON;
    sys_property.sea2 = C_ON;
    set_sys_property_value(sys_property);

    ejanus2_start(300);

    lv_custom_exec();

    return 0;
}
