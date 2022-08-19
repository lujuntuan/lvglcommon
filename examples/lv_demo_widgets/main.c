#include "lv_custom/lv_custom.h"
#include "lv_demo_widgets.h"

int main(void)
{
#ifdef USE_OPENWFD
    //putenv("WFD_CLIENT_ID=0x78ff");
    //putenv("WFD_PORT_INDEX=1");
    //putenv("WFD_PORT_MODE_INDEX=0");
    //putenv("WFD_PIPELINE_ID=6");
#endif

    lv_custom_init(LV_CUSTOM_DISP | LV_CUSTOM_INDEV);

    lv_demo_widgets();

    lv_custom_exec();

    return 0;
}
