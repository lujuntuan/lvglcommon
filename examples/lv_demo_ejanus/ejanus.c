#include "ejanus.h"
#include "lv_custom/lv_custom.h"
#include <math.h>
#include <stdlib.h>

LV_IMG_DECLARE(img_dial_panel)
LV_IMG_DECLARE(img_dial_gauge)
LV_IMG_DECLARE(img_dial_needle)
LV_IMG_DECLARE(img_dial_rpm)
LV_IMG_DECLARE(img_dial_oil_bgnorm)
LV_IMG_DECLARE(img_dial_oil_bgwarn)
LV_IMG_DECLARE(img_dial_water_bgnorm)
LV_IMG_DECLARE(img_dial_water_bgwarn)
LV_IMG_DECLARE(img_dial_media_tire)
LV_IMG_DECLARE(img_dial_media_tire_warn)
LV_IMG_DECLARE(img_dial_media_tire_error)
LV_IMG_DECLARE(img_dial_media_all)
LV_IMG_DECLARE(img_dial_v2x_bg)
LV_IMG_DECLARE(img_dial_v2x_lineleft)
LV_IMG_DECLARE(img_dial_v2x_lineright)
LV_IMG_DECLARE(img_dial_v2x_car)

LV_IMG_DECLARE(img_dial_num0)
LV_IMG_DECLARE(img_dial_num1)
LV_IMG_DECLARE(img_dial_num2)
LV_IMG_DECLARE(img_dial_num3)
LV_IMG_DECLARE(img_dial_num4)
LV_IMG_DECLARE(img_dial_num5)
LV_IMG_DECLARE(img_dial_num6)
LV_IMG_DECLARE(img_dial_num7)
LV_IMG_DECLARE(img_dial_num8)
LV_IMG_DECLARE(img_dial_num9)

LV_IMG_DECLARE(img_dial_gaugenum0)
LV_IMG_DECLARE(img_dial_gaugenum1)
LV_IMG_DECLARE(img_dial_gaugenum2)
LV_IMG_DECLARE(img_dial_gaugenum3)
LV_IMG_DECLARE(img_dial_gaugenum4)
LV_IMG_DECLARE(img_dial_gaugenum5)
LV_IMG_DECLARE(img_dial_gaugenum6)
LV_IMG_DECLARE(img_dial_gaugenum7)

LV_IMG_DECLARE(img_dial_oil_norm)
LV_IMG_DECLARE(img_dial_oil_warn)
LV_IMG_DECLARE(img_dial_water_norm)
LV_IMG_DECLARE(img_dial_water_warn)

LV_IMG_DECLARE(img_dial_sys_abs)
LV_IMG_DECLARE(img_dial_sys_ah)
LV_IMG_DECLARE(img_dial_sys_eco)
LV_IMG_DECLARE(img_dial_sys_egls)
LV_IMG_DECLARE(img_dial_sys_epc)
LV_IMG_DECLARE(img_dial_sys_espoff)
LV_IMG_DECLARE(img_dial_sys_hdc)
LV_IMG_DECLARE(img_dial_sys_light)
LV_IMG_DECLARE(img_dial_sys_lightf)
LV_IMG_DECLARE(img_dial_sys_lighth)
LV_IMG_DECLARE(img_dial_sys_lightl)
LV_IMG_DECLARE(img_dial_sys_sea1)
LV_IMG_DECLARE(img_dial_sys_sea2)
LV_IMG_DECLARE(img_dial_sys_tleft)
LV_IMG_DECLARE(img_dial_sys_tright)

#define MAX_OIL_VALUE 65
#define MAX_WATER_VALUE 120
#define MIN_WATER_VALUE -20

#define RPM_ANGLE_STEP 246
#define RPM_OFFSET_ANGLE -124

#define PI 3.141592654

#define AUTO_RUN_TEST 1

static lv_obj_t* panel = NULL;
static lv_obj_t* maincont = NULL;
static lv_obj_t* gauge = NULL;
static lv_obj_t* need_img = NULL;
static lv_obj_t* rpm_img = NULL;
static lv_obj_t* rpm_mask = NULL;
static lv_objmask_mask_t* rpm_mask_option = NULL;
static lv_obj_t* oilbg_img = NULL;
static lv_obj_t* oilbg_mask = NULL;
static lv_objmask_mask_t* oilbg_mask_option = NULL;
static lv_obj_t* waterbg_img = NULL;
static lv_obj_t* waterbg_mask = NULL;
static lv_objmask_mask_t* waterbg_mask_option = NULL;

static lv_obj_t* gaugenum0_img = NULL;
static lv_obj_t* gaugenum1_img = NULL;
static lv_obj_t* gaugenum2_img = NULL;
static lv_obj_t* gaugenum3_img = NULL;
static lv_obj_t* gaugenum4_img = NULL;
static lv_obj_t* gaugenum5_img = NULL;
static lv_obj_t* gaugenum6_img = NULL;
static lv_obj_t* gaugenum7_img = NULL;

static lv_obj_t* num1_img = NULL;
static lv_obj_t* num2_img = NULL;
static lv_obj_t* num3_img = NULL;
static lv_obj_t* oil_img = NULL;
static lv_obj_t* water_img = NULL;

static lv_obj_t* km_label = NULL;
static lv_obj_t* tem_label = NULL;
static lv_obj_t* trip_label = NULL;
static lv_obj_t* total_label = NULL;
static lv_obj_t* gear_label = NULL;

static lv_obj_t* tire_bg_img = NULL;
static lv_obj_t* tire1_img = NULL;
static lv_obj_t* tire2_img = NULL;
static lv_obj_t* tire3_img = NULL;
static lv_obj_t* tire4_img = NULL;
static lv_obj_t* tire1_label = NULL;
static lv_obj_t* tire2_label = NULL;
static lv_obj_t* tire3_label = NULL;
static lv_obj_t* tire4_label = NULL;
static lv_obj_t* allmedia_img = NULL;
static lv_obj_t* allmedia_mask = NULL;
static lv_obj_t* v2xbg_img = NULL;
static lv_obj_t* v2xlineleft_img = NULL;
static lv_obj_t* v2xlineright_img = NULL;
static lv_obj_t* v2xlineleft_mask = NULL;
static lv_obj_t* v2xlineright_mask = NULL;
static lv_obj_t* v2xcar_img = NULL;

static lv_obj_t* abs_img = NULL;
static lv_obj_t* ah_img = NULL;
static lv_obj_t* eco_img = NULL;
static lv_obj_t* egls_img = NULL;
static lv_obj_t* epc_img = NULL;
static lv_obj_t* espoff_img = NULL;
static lv_obj_t* hdc_img = NULL;
static lv_obj_t* light_img = NULL;
static lv_obj_t* lightf_img = NULL;
static lv_obj_t* lighth_img = NULL;
static lv_obj_t* lightl_img = NULL;
static lv_obj_t* sea1_img = NULL;
static lv_obj_t* sea2_img = NULL;
static lv_obj_t* tleft_img = NULL;
static lv_obj_t* tright_img = NULL;

static lv_coord_t hres = 0;
static lv_coord_t vres = 0;

static uint16_t rpm_value = 0;
static uint16_t speed_value = 0;
static uint16_t oil_value = 0;
static int16_t water_value = MIN_WATER_VALUE;
static uint32_t trip_value = 0;
static uint32_t total_value = 0;

static GEAR_STATE gear_state = GEAR_P;
static LIGHT_STATE light_state = LIGHT_CLOSE;
static TURN_STATE turn_state = TURN_ALL_OFF;
static TURN_STATE media_state = MEDIA_NONE;
static TIRE_PRESS_TEMP press_temp = { 0, 0, 0, 0, 0, 0, 0, 0 };
static SYS_PROPERTY sys_property = { C_OFF, C_OFF, C_OFF, C_OFF, C_OFF, C_OFF, C_OFF, C_OFF, C_OFF };
static int8_t turn_ani_hide = 0;

static lv_task_t* begin_task1 = NULL;
static lv_task_t* begin_task2 = NULL;
static lv_task_t* test_task = NULL;
static lv_task_t* turn_ani_task = NULL;
static lv_task_t* v2x_img_sprite_task = NULL;

static uint16_t rpm_ani = 0;
static uint16_t speed_ani = 0;
static uint16_t oil_ani = 0;
static uint16_t water_ani = 0;

static lv_anim_t start_ani_struct;
static lv_anim_t rpm_ani_struct;
static lv_anim_t speed_ani_struct;
static lv_anim_t oil_ani_struct;
static lv_anim_t water_ani_struct;
static lv_anim_t tire_ani_struct;
static lv_anim_t allmedia_ani_struct;

static uint16_t full_oil_km = 750;

static uint8_t has_inited = 0;

static uint8_t media_test_count = 0;
static uint8_t v2x_img_sprite = 0;

static uint16_t nums_area = 1250;
static uint8_t nums_size_qz = 175;
static uint8_t nums_opacity_qz = 60;

static uint8_t start_ani_type = 0;

static void begin_ani1(lv_task_t* task);
static void begin_ani2(lv_task_t* task);
static void begin();
static void create_main();
static void create_media();
static void create_text();
static void create_ico();
static void create_ani();
static lv_anim_value_t custom_anim_path_ease_out(const lv_anim_path_t* path, const lv_anim_t* a);
static void paint_dial_num(lv_obj_t* img_obj, uint16_t num);
static void deal_turn_state(lv_task_t* task);
static void deal_start_ani(void* _ani, lv_anim_value_t value);
static void deal_rpm_ani(void* _ani, lv_anim_value_t value);
static void deal_speed_ani(void* _ani, lv_anim_value_t value);
static void deal_oil_ani(void* _ani, lv_anim_value_t value);
static void deal_water_ani(void* _ani, lv_anim_value_t value);
static void deal_tire_ani(void* _ani, lv_anim_value_t value);
static void deal_allmedia_ani(void* _ani, lv_anim_value_t value);
static void deal_v2x_img_sprite(lv_task_t* task);
static void run_test(lv_task_t* task);

static void begin_ani1(lv_task_t* task)
{
    LV_UNUSED(task);
    if (begin_task1) {
        lv_task_del(begin_task1);
    }
    lv_anim_set_values(&start_ani_struct, 0, 7000);
    lv_anim_start(&start_ani_struct);
    start_ani_type = 0;
}

static void begin_ani2(lv_task_t* task)
{
    LV_UNUSED(task);
    if (begin_task2) {
        lv_task_del(begin_task2);
    }
    lv_anim_set_values(&start_ani_struct, 7000, 0);
    lv_anim_start(&start_ani_struct);
    start_ani_type = 1;
}

static void begin()
{
    has_inited = 1;
    lv_obj_set_hidden(maincont, 0);
    lv_anim_del(&start_ani_struct, deal_start_ani);
    if (AUTO_RUN_TEST) {
        if (test_task == NULL) {
            test_task = lv_task_create(run_test, 2000, LV_TASK_PRIO_LOW, NULL);
        }
        if (v2x_img_sprite_task == NULL) {
            lv_obj_set_hidden(v2xlineleft_mask, 0);
            lv_obj_set_hidden(v2xlineright_mask, 0);
            lv_obj_set_hidden(v2xcar_img, 0);
            v2x_img_sprite_task = lv_task_create(deal_v2x_img_sprite, 20, LV_TASK_PRIO_MID, NULL);
        }
    }
}

void ejanus_create()
{
    hres = lv_disp_get_hor_res(NULL);
    vres = lv_disp_get_ver_res(NULL);

    lv_custom_print("create_main");
    create_main();
    lv_custom_print("create_media");
    create_media();
    lv_custom_print("create_text");
    create_text();
    lv_custom_print("create_ico");
    create_ico();
    lv_custom_print("create_ani");
    create_ani();

    lv_scr_load(panel);
}

static void create_main()
{

    panel = lv_img_create(NULL, NULL);

    lv_obj_set_width(panel, hres);
    lv_obj_set_height(panel, vres);
    lv_img_set_src(panel, &img_dial_panel);
    //lv_obj_set_style_local_image_opa(panel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 50);

    maincont = lv_cont_create(panel, NULL);
    lv_obj_set_width(maincont, hres);
    lv_obj_set_height(maincont, vres);
    lv_obj_set_style_local_bg_color(maincont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_bg_opa(maincont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(maincont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_opa(maincont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_align(maincont, panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_opa_scale(maincont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    rpm_mask = lv_objmask_create(panel, NULL);
    lv_obj_set_width(rpm_mask, img_dial_rpm.header.w);
    lv_obj_set_height(rpm_mask, img_dial_rpm.header.h);
    lv_obj_align(rpm_mask, panel, LV_ALIGN_CENTER, 0, 0);
    rpm_img = lv_img_create(rpm_mask, NULL);
    lv_img_set_src(rpm_img, &img_dial_rpm);
    lv_obj_set_hidden(rpm_mask, 1);

    oilbg_mask = lv_objmask_create(panel, NULL);
    lv_obj_set_width(oilbg_mask, img_dial_oil_bgnorm.header.w);
    lv_obj_set_height(oilbg_mask, img_dial_oil_bgnorm.header.h);
    lv_obj_align(oilbg_mask, panel, LV_ALIGN_CENTER, -300, 0);
    oilbg_img = lv_img_create(oilbg_mask, NULL);
    lv_img_set_src(oilbg_img, &img_dial_oil_bgnorm);
    lv_obj_set_hidden(oilbg_mask, 1);

    waterbg_mask = lv_objmask_create(panel, NULL);
    lv_obj_set_width(waterbg_mask, img_dial_water_bgnorm.header.w);
    lv_obj_set_height(waterbg_mask, img_dial_water_bgnorm.header.h);
    lv_obj_align(waterbg_mask, panel, LV_ALIGN_CENTER, 300, 0);
    waterbg_img = lv_img_create(waterbg_mask, NULL);
    lv_img_set_src(waterbg_img, &img_dial_water_bgnorm);
    lv_obj_set_hidden(waterbg_mask, 1);

    //

    gauge = lv_img_create(panel, NULL);
    lv_img_set_src(gauge, &img_dial_gauge);
    lv_obj_align(gauge, panel, LV_ALIGN_CENTER, 0, 0);

    need_img = lv_img_create(panel, NULL);
    lv_img_set_src(need_img, &img_dial_needle);
    lv_obj_align(need_img, panel, LV_ALIGN_IN_TOP_MID, 0, 40);
    lv_img_set_pivot(need_img, img_dial_needle.header.w / 2, 318);
    lv_img_set_angle(need_img, RPM_ANGLE_STEP * 10 * rpm_ani / 7000 + RPM_OFFSET_ANGLE * 10);

    num1_img = lv_img_create(panel, NULL);
    lv_img_set_src(num1_img, &img_dial_num0);
    lv_obj_set_hidden(num1_img, 0);
    lv_obj_set_x(num1_img, hres / 2 - img_dial_num0.header.w / 2);
    lv_obj_set_y(num1_img, vres / 2 - img_dial_num0.header.h / 2 - 45);

    num2_img = lv_img_create(panel, num1_img);
    lv_obj_set_hidden(num2_img, 1);

    num3_img = lv_img_create(panel, num1_img);
    lv_obj_set_hidden(num3_img, 1);

    int16_t gauge_angle = 0;
    int16_t gauge_x = 0;
    int16_t gauge_y = 0;
    int16_t gauge_r = 221;
    int16_t gauge_cx = hres / 2 - img_dial_gaugenum0.header.w / 2;
    int16_t gauge_cy = vres / 2 - img_dial_gaugenum0.header.h / 2;
    gaugenum0_img = lv_img_create(panel, NULL);
    lv_img_set_src(gaugenum0_img, &img_dial_gaugenum0);
    gauge_angle = RPM_ANGLE_STEP * 0 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum0_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum0_img, gauge_x, gauge_y);

    lv_img_set_pivot(gaugenum0_img, img_dial_gaugenum1.header.w / 2, img_dial_gaugenum0.header.h / 2);
    lv_img_set_zoom(gaugenum0_img, nums_size_qz);
    lv_obj_set_style_local_image_opa(gaugenum0_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, nums_opacity_qz);

    gaugenum1_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum1_img, &img_dial_gaugenum1);
    gauge_angle = RPM_ANGLE_STEP * 1000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum1_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum1_img, gauge_x, gauge_y);

    gaugenum2_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum2_img, &img_dial_gaugenum2);
    gauge_angle = RPM_ANGLE_STEP * 2000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum2_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum2_img, gauge_x, gauge_y);

    gaugenum3_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum3_img, &img_dial_gaugenum3);
    gauge_angle = RPM_ANGLE_STEP * 3000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum3_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum3_img, gauge_x, gauge_y);

    gaugenum4_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum4_img, &img_dial_gaugenum4);
    gauge_angle = RPM_ANGLE_STEP * 4000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum4_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum4_img, gauge_x, gauge_y);

    gaugenum5_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum5_img, &img_dial_gaugenum5);
    gauge_angle = RPM_ANGLE_STEP * 5000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum5_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum5_img, gauge_x, gauge_y);

    gaugenum6_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum6_img, &img_dial_gaugenum6);
    gauge_angle = RPM_ANGLE_STEP * 6000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum6_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum6_img, gauge_x, gauge_y);

    gaugenum7_img = lv_img_create(panel, gaugenum0_img);
    lv_img_set_src(gaugenum7_img, &img_dial_gaugenum7);
    gauge_angle = RPM_ANGLE_STEP * 7000 / 7000 + RPM_OFFSET_ANGLE;
    gauge_x = gauge_cx + (int16_t)(sin(gauge_angle * PI / 180) * gauge_r);
    gauge_y = gauge_cy - (int16_t)(cos(gauge_angle * PI / 180) * gauge_r);
    lv_img_set_angle(gaugenum7_img, gauge_angle * 10);
    lv_obj_set_pos(gaugenum7_img, gauge_x, gauge_y);

    oil_img = lv_img_create(panel, NULL);
    lv_img_set_src(oil_img, &img_dial_oil_norm);
    lv_obj_set_x(oil_img, hres / 2 - 268);
    lv_obj_set_y(oil_img, vres / 2 + 183);

    water_img = lv_img_create(panel, oil_img);
    lv_img_set_src(water_img, &img_dial_water_norm);
    lv_obj_set_x(water_img, hres / 2 + 268 - 36);
    lv_obj_set_y(water_img, vres / 2 + 183);
}

static void create_media()
{
    v2xbg_img = lv_img_create(maincont, NULL);
    lv_img_set_src(v2xbg_img, &img_dial_v2x_bg);
    lv_obj_align(v2xbg_img, maincont, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    //
    tire_bg_img = lv_img_create(maincont, NULL);
    lv_img_set_src(tire_bg_img, &img_dial_media_tire);
    lv_obj_align(tire_bg_img, maincont, LV_ALIGN_IN_LEFT_MID, 240, -25);

    tire1_img = lv_img_create(tire_bg_img, NULL);
    lv_img_set_src(tire1_img, &img_dial_media_tire_warn);
    lv_obj_align(tire1_img, tire_bg_img, LV_ALIGN_IN_TOP_LEFT, 1, 38);

    tire2_img = lv_img_create(tire_bg_img, tire1_img);
    lv_img_set_src(tire2_img, &img_dial_media_tire_warn);
    lv_obj_align(tire2_img, tire_bg_img, LV_ALIGN_IN_TOP_RIGHT, 1, 38);

    tire3_img = lv_img_create(tire_bg_img, tire1_img);
    lv_img_set_src(tire3_img, &img_dial_media_tire_warn);
    lv_obj_align(tire3_img, tire_bg_img, LV_ALIGN_IN_BOTTOM_LEFT, 1, -25);

    tire4_img = lv_img_create(tire_bg_img, tire1_img);
    lv_img_set_src(tire4_img, &img_dial_media_tire_warn);
    lv_obj_align(tire4_img, tire_bg_img, LV_ALIGN_IN_BOTTOM_RIGHT, 1, -25);

    tire1_label = lv_label_create(maincont, NULL);
    lv_obj_set_style_local_text_font(tire1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_custom_font_18);
    lv_obj_set_style_local_text_color(tire1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_text_opa(tire1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);
    lv_label_set_long_mode(tire1_label, LV_LABEL_LONG_SROLL);
    lv_obj_set_width(tire1_label, 150);
    lv_obj_set_height(tire1_label, lv_custom_font_18.line_height * 2 + 5);
    //
    lv_label_set_align(tire1_label, LV_LABEL_ALIGN_RIGHT);
    lv_obj_align(tire1_label, tire1_img, LV_ALIGN_OUT_LEFT_MID, -20, 0);
    lv_label_set_text(tire1_label, "UNKNOWN");

    tire2_label = lv_label_create(maincont, tire1_label);
    lv_label_set_align(tire2_label, LV_LABEL_ALIGN_LEFT);
    lv_obj_align(tire2_label, tire2_img, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_label_set_text(tire2_label, "UNKNOWN");

    tire3_label = lv_label_create(maincont, tire1_label);
    lv_label_set_align(tire3_label, LV_LABEL_ALIGN_RIGHT);
    lv_obj_align(tire3_label, tire3_img, LV_ALIGN_OUT_LEFT_MID, -20, 0);
    lv_label_set_text(tire3_label, "UNKNOWN");

    tire4_label = lv_label_create(maincont, tire1_label);
    lv_label_set_align(tire4_label, LV_LABEL_ALIGN_LEFT);
    lv_obj_align(tire4_label, tire4_img, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_label_set_text(tire4_label, "UNKNOWN");
    //
    allmedia_mask = lv_objmask_create(maincont, NULL);
    lv_obj_set_width(allmedia_mask, img_dial_media_all.header.w / 5);
    lv_obj_set_height(allmedia_mask, img_dial_media_all.header.h);
    lv_obj_align(allmedia_mask, panel, LV_ALIGN_IN_RIGHT_MID, -120, 0);
    allmedia_img = lv_img_create(allmedia_mask, NULL);
    lv_img_set_src(allmedia_img, &img_dial_media_all);
    lv_obj_set_x(allmedia_img, 0);

    v2xlineleft_mask = lv_objmask_create(v2xbg_img, NULL);
    lv_obj_set_width(v2xlineleft_mask, img_dial_v2x_lineleft.header.w / 10);
    lv_obj_set_height(v2xlineleft_mask, img_dial_v2x_lineleft.header.h / 2);
    lv_obj_align(v2xlineleft_mask, v2xbg_img, LV_ALIGN_CENTER, -105, 0);
    v2xlineleft_img = lv_img_create(v2xlineleft_mask, NULL);
    lv_img_set_src(v2xlineleft_img, &img_dial_v2x_lineleft);
    lv_obj_set_hidden(v2xlineleft_mask, 1);

    v2xlineright_mask = lv_objmask_create(v2xbg_img, NULL);
    lv_obj_set_width(v2xlineright_mask, img_dial_v2x_lineleft.header.w / 10);
    lv_obj_set_height(v2xlineright_mask, img_dial_v2x_lineleft.header.h / 2);
    lv_obj_align(v2xlineright_mask, v2xbg_img, LV_ALIGN_CENTER, 105, 0);
    v2xlineright_img = lv_img_create(v2xlineright_mask, NULL);
    lv_img_set_src(v2xlineright_img, &img_dial_v2x_lineright);
    lv_obj_set_hidden(v2xlineright_mask, 1);

    v2xcar_img = lv_img_create(v2xbg_img, NULL);
    lv_img_set_src(v2xcar_img, &img_dial_v2x_car);
    lv_obj_align(v2xcar_img, v2xbg_img, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_hidden(v2xcar_img, 1);
}

static void create_text()
{
    km_label = lv_label_create(v2xbg_img, NULL);

    lv_label_set_text(km_label, "-- km");
    lv_obj_set_style_local_text_font(km_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_custom_font_24);
    lv_obj_set_style_local_text_color(km_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_opa(km_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
    lv_label_set_long_mode(km_label, LV_LABEL_LONG_CROP);
    lv_label_set_align(km_label, LV_LABEL_ALIGN_RIGHT);
    lv_obj_set_width(km_label, 130);
    lv_obj_align(km_label, v2xbg_img, LV_ALIGN_IN_BOTTOM_MID, -400, -145);

    tem_label = lv_label_create(v2xbg_img, km_label);
    lv_label_set_text(tem_label, "-- °C");
    lv_label_set_align(tem_label, LV_LABEL_ALIGN_RIGHT);
    lv_obj_align(tem_label, v2xbg_img, LV_ALIGN_IN_BOTTOM_MID, 355, -145);
    //
    trip_label = lv_label_create(v2xbg_img, km_label);
    lv_obj_set_style_local_text_font(trip_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_custom_font_24);
    lv_obj_set_style_local_text_opa(trip_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
    lv_label_set_text(trip_label, "--");
    lv_label_set_align(trip_label, LV_LABEL_ALIGN_RIGHT);
    lv_obj_align(trip_label, v2xbg_img, LV_ALIGN_IN_BOTTOM_MID, -185, -25);

    total_label = lv_label_create(v2xbg_img, trip_label);
    lv_label_set_text(total_label, "--");
    lv_label_set_align(total_label, LV_LABEL_ALIGN_RIGHT);
    lv_obj_align(total_label, v2xbg_img, LV_ALIGN_IN_BOTTOM_MID, 235, -25);

    gear_label = lv_label_create(v2xbg_img, km_label);
    lv_obj_set_style_local_text_font(gear_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_custom_font_36);
    lv_obj_set_style_local_text_opa(gear_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);
    lv_label_set_text(gear_label, "--");
    lv_label_set_align(gear_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_height(gear_label, 60);
    lv_obj_align(gear_label, v2xbg_img, LV_ALIGN_IN_BOTTOM_MID, 1, -6);
}

static void create_ico()
{
    abs_img = lv_img_create(maincont, NULL);
    lv_img_set_src(abs_img, &img_dial_sys_abs);
    lv_obj_align(abs_img, maincont, LV_ALIGN_IN_BOTTOM_LEFT, 120, -20);
    lv_obj_set_hidden(abs_img, 1);

    ah_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(ah_img, &img_dial_sys_ah);
    lv_obj_align(ah_img, maincont, LV_ALIGN_IN_TOP_MID, 370, 140);

    eco_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(eco_img, &img_dial_sys_eco);
    lv_obj_align(eco_img, maincont, LV_ALIGN_IN_BOTTOM_RIGHT, -120, -20);

    egls_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(egls_img, &img_dial_sys_egls);
    lv_obj_align(egls_img, maincont, LV_ALIGN_IN_TOP_LEFT, 120, 20);

    epc_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(epc_img, &img_dial_sys_epc);
    lv_obj_align(epc_img, maincont, LV_ALIGN_IN_BOTTOM_LEFT, 190, -20);

    espoff_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(espoff_img, &img_dial_sys_espoff);
    lv_obj_align(espoff_img, maincont, LV_ALIGN_IN_BOTTOM_LEFT, 260, -20);

    hdc_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(hdc_img, &img_dial_sys_hdc);
    lv_obj_align(hdc_img, maincont, LV_ALIGN_IN_BOTTOM_RIGHT, -190, -20);

    light_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(light_img, &img_dial_sys_light);
    lv_obj_align(light_img, maincont, LV_ALIGN_IN_TOP_MID, -120, 20);

    lightf_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(lightf_img, &img_dial_sys_lightf);
    lv_obj_align(lightf_img, maincont, LV_ALIGN_IN_TOP_MID, 50, 20);

    lighth_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(lighth_img, &img_dial_sys_lighth);
    lv_obj_align(lighth_img, maincont, LV_ALIGN_IN_TOP_MID, 120, 20);

    lightl_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(lightl_img, &img_dial_sys_lightl);
    lv_obj_align(lightl_img, maincont, LV_ALIGN_IN_TOP_MID, 190, 20);

    sea1_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(sea1_img, &img_dial_sys_sea1);
    lv_obj_align(sea1_img, maincont, LV_ALIGN_IN_TOP_RIGHT, -190, 20);

    sea2_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(sea2_img, &img_dial_sys_sea2);
    lv_obj_align(sea2_img, maincont, LV_ALIGN_IN_TOP_RIGHT, -120, 20);

    tleft_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(tleft_img, &img_dial_sys_tleft);
    lv_obj_align(tleft_img, maincont, LV_ALIGN_IN_TOP_MID, -300, 20);

    tright_img = lv_img_create(maincont, abs_img);
    lv_img_set_src(tright_img, &img_dial_sys_tright);
    lv_obj_align(tright_img, maincont, LV_ALIGN_IN_TOP_MID, 300, 20);
}

static void create_ani()
{
    static lv_anim_path_t linear_ani_path;
    lv_anim_path_init(&linear_ani_path);
    lv_anim_path_set_cb(&linear_ani_path, lv_anim_path_linear);

    static lv_anim_path_t out_ani_path;
    lv_anim_path_init(&out_ani_path);
    lv_anim_path_set_cb(&out_ani_path, custom_anim_path_ease_out);

    lv_anim_init(&start_ani_struct);
    lv_anim_set_exec_cb(&start_ani_struct, deal_start_ani);
    lv_anim_set_time(&start_ani_struct, 950);
    lv_anim_set_path(&start_ani_struct, &linear_ani_path);

    lv_anim_init(&rpm_ani_struct);
    lv_anim_set_exec_cb(&rpm_ani_struct, deal_rpm_ani);
    lv_anim_set_time(&rpm_ani_struct, 800);
    lv_anim_set_path(&rpm_ani_struct, &out_ani_path);

    lv_anim_init(&speed_ani_struct);
    lv_anim_set_exec_cb(&speed_ani_struct, deal_speed_ani);
    lv_anim_set_time(&speed_ani_struct, 800);
    lv_anim_set_path(&speed_ani_struct, &out_ani_path);

    lv_anim_init(&oil_ani_struct);
    lv_anim_set_exec_cb(&oil_ani_struct, deal_oil_ani);
    lv_anim_set_time(&oil_ani_struct, 800);
    lv_anim_set_path(&oil_ani_struct, &out_ani_path);

    lv_anim_init(&water_ani_struct);
    lv_anim_set_exec_cb(&water_ani_struct, deal_water_ani);
    lv_anim_set_time(&water_ani_struct, 800);
    lv_anim_set_path(&water_ani_struct, &out_ani_path);

    lv_anim_init(&tire_ani_struct);
    lv_anim_set_exec_cb(&tire_ani_struct, deal_tire_ani);
    lv_anim_set_time(&tire_ani_struct, 400);
    lv_anim_set_path(&tire_ani_struct, &out_ani_path);

    lv_anim_init(&allmedia_ani_struct);
    lv_anim_set_exec_cb(&allmedia_ani_struct, deal_allmedia_ani);
    lv_anim_set_time(&allmedia_ani_struct, 400);
    lv_anim_set_path(&allmedia_ani_struct, &out_ani_path);
}

static lv_anim_value_t custom_anim_path_ease_out(const lv_anim_path_t* path, const lv_anim_t* a)
{
    LV_UNUSED(path);

    uint32_t t;
    if (a->time == a->act_time) {
        t = 1024;
    } else {
        t = (uint32_t)((uint32_t)a->act_time * 1024) / a->time;
    }

    int32_t step = _lv_bezier3(t, 0, 768, 960, 1024);

    int32_t new_value;
    new_value = (int32_t)step * (a->end - a->start);
    new_value = new_value >> 10;
    new_value += a->start;

    return (lv_anim_value_t)new_value;
}

static void paint_dial_num(lv_obj_t* img_obj, uint16_t num)
{
    switch (num) {
    case 0:
        lv_img_set_src(img_obj, &img_dial_num0);
        break;
    case 1:
        lv_img_set_src(img_obj, &img_dial_num1);
        break;
    case 2:
        lv_img_set_src(img_obj, &img_dial_num2);
        break;
    case 3:
        lv_img_set_src(img_obj, &img_dial_num3);
        break;
    case 4:
        lv_img_set_src(img_obj, &img_dial_num4);
        break;
    case 5:
        lv_img_set_src(img_obj, &img_dial_num5);
        break;
    case 6:
        lv_img_set_src(img_obj, &img_dial_num6);
        break;
    case 7:
        lv_img_set_src(img_obj, &img_dial_num7);
        break;
    case 8:
        lv_img_set_src(img_obj, &img_dial_num8);
        break;
    case 9:
        lv_img_set_src(img_obj, &img_dial_num9);
        break;
    default:
        break;
    }
}

static void deal_turn_state(lv_task_t* task)
{
    if (task != turn_ani_task) {
        return;
    }
    if (has_inited == 0) {
        return;
    }
    if (turn_ani_hide == 0) {
        turn_ani_hide = 1;
    } else {
        turn_ani_hide = 0;
    }
    switch (turn_state) {
    case TURN_ALL_OFF:
        lv_obj_set_hidden(tleft_img, 1);
        lv_obj_set_hidden(tright_img, 1);
        break;
    case TURN_LEFT:
        lv_obj_set_hidden(tleft_img, turn_ani_hide);
        lv_obj_set_hidden(tright_img, 1);
        break;
    case TURN_RIGHT:
        lv_obj_set_hidden(tleft_img, 1);
        lv_obj_set_hidden(tright_img, turn_ani_hide);
        break;
    case TURN_ALL_ON:
        lv_obj_set_hidden(tleft_img, turn_ani_hide);
        lv_obj_set_hidden(tright_img, turn_ani_hide);
        break;
    default:
        break;
    }
}

static void deal_start_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    if (start_ani_type == 2) {
        lv_obj_set_style_local_opa_scale(maincont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)value);
        if (start_ani_struct.act_time <= 0) {
            begin();
        }
    } else {
        deal_speed_ani(NULL, value * 240 / 7000);
        deal_rpm_ani(NULL, value);
        deal_oil_ani(NULL, value * MAX_OIL_VALUE * 10 / 7000);
        deal_water_ani(NULL, value * (MAX_WATER_VALUE - MIN_WATER_VALUE) / 7000);
        if (value <= 0 && start_ani_type == 1) {
            lv_anim_set_time(&start_ani_struct, 500);
            lv_anim_set_values(&start_ani_struct, 0, 255);
            lv_anim_start(&start_ani_struct);
            start_ani_type = 2;
        } else if (value >= 7000 && start_ani_type == 0) {
            begin_task2 = lv_task_create(begin_ani2, 100, LV_TASK_PRIO_MID, NULL);
        }
    }
}

static void deal_rpm_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    if (rpm_ani != value) {
        rpm_ani = value;
        lv_img_set_angle(need_img, RPM_ANGLE_STEP * 10 * rpm_ani / 7000 + RPM_OFFSET_ANGLE * 10);
        lv_draw_mask_angle_param_t rpm_param;
        lv_draw_mask_angle_init(&rpm_param, img_dial_rpm.header.w / 2, img_dial_rpm.header.h / 2, 142, (RPM_ANGLE_STEP * rpm_ani / 7000 + 142 + 5) % 360);
        if (rpm_mask_option == NULL) {
            rpm_mask_option = lv_objmask_add_mask(rpm_mask, &rpm_param);
            lv_obj_set_hidden(rpm_mask, 0);
        } else {
            lv_objmask_update_mask(rpm_mask, rpm_mask_option, &rpm_param);
        }

        int16_t num0_size_step = (nums_area - abs(rpm_ani - 0)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num1_size_step = (nums_area - abs(rpm_ani - 1000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num2_size_step = (nums_area - abs(rpm_ani - 2000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num3_size_step = (nums_area - abs(rpm_ani - 3000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num4_size_step = (nums_area - abs(rpm_ani - 4000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num5_size_step = (nums_area - abs(rpm_ani - 5000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num6_size_step = (nums_area - abs(rpm_ani - 6000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        int16_t num7_size_step = (nums_area - abs(rpm_ani - 7000)) * (255 - nums_size_qz) / 1000 + nums_size_qz;
        if (num0_size_step < nums_size_qz)
            num0_size_step = nums_size_qz;
        if (num1_size_step < nums_size_qz)
            num1_size_step = nums_size_qz;
        if (num2_size_step < nums_size_qz)
            num2_size_step = nums_size_qz;
        if (num3_size_step < nums_size_qz)
            num3_size_step = nums_size_qz;
        if (num4_size_step < nums_size_qz)
            num4_size_step = nums_size_qz;
        if (num5_size_step < nums_size_qz)
            num5_size_step = nums_size_qz;
        if (num6_size_step < nums_size_qz)
            num6_size_step = nums_size_qz;
        if (num7_size_step < nums_size_qz)
            num7_size_step = nums_size_qz;

        int16_t num0_opacity_step = (nums_area + rpm_ani - 0) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num1_opacity_step = (nums_area + rpm_ani - 1000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num2_opacity_step = (nums_area + rpm_ani - 2000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num3_opacity_step = (nums_area + rpm_ani - 3000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num4_opacity_step = (nums_area + rpm_ani - 4000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num5_opacity_step = (nums_area + rpm_ani - 5000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num6_opacity_step = (nums_area + rpm_ani - 6000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        int16_t num7_opacity_step = (nums_area + rpm_ani - 7000) * (255 - nums_opacity_qz) / 1000 + nums_opacity_qz;
        if (num0_opacity_step < nums_opacity_qz)
            num0_opacity_step = nums_opacity_qz;
        if (num1_opacity_step < nums_opacity_qz)
            num1_opacity_step = nums_opacity_qz;
        if (num2_opacity_step < nums_opacity_qz)
            num2_opacity_step = nums_opacity_qz;
        if (num3_opacity_step < nums_opacity_qz)
            num3_opacity_step = nums_opacity_qz;
        if (num4_opacity_step < nums_opacity_qz)
            num4_opacity_step = nums_opacity_qz;
        if (num5_opacity_step < nums_opacity_qz)
            num5_opacity_step = nums_opacity_qz;
        if (num6_opacity_step < nums_opacity_qz)
            num6_opacity_step = nums_opacity_qz;
        if (num7_opacity_step < nums_opacity_qz)
            num7_opacity_step = nums_opacity_qz;
        if (num0_opacity_step > 255)
            num0_opacity_step = 255;
        if (num1_opacity_step > 255)
            num1_opacity_step = 255;
        if (num2_opacity_step > 255)
            num2_opacity_step = 255;
        if (num3_opacity_step > 255)
            num3_opacity_step = 255;
        if (num4_opacity_step > 255)
            num4_opacity_step = 255;
        if (num5_opacity_step > 255)
            num5_opacity_step = 255;
        if (num6_opacity_step > 255)
            num6_opacity_step = 255;
        if (num7_opacity_step > 255)
            num7_opacity_step = 255;

        lv_img_set_zoom(gaugenum0_img, num0_size_step);
        lv_obj_set_style_local_image_opa(gaugenum0_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num0_opacity_step);
        lv_img_set_zoom(gaugenum1_img, num1_size_step);
        lv_obj_set_style_local_image_opa(gaugenum1_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num1_opacity_step);
        lv_img_set_zoom(gaugenum2_img, num2_size_step);
        lv_obj_set_style_local_image_opa(gaugenum2_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num2_opacity_step);
        lv_img_set_zoom(gaugenum3_img, num3_size_step);
        lv_obj_set_style_local_image_opa(gaugenum3_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num3_opacity_step);
        lv_img_set_zoom(gaugenum4_img, num4_size_step);
        lv_obj_set_style_local_image_opa(gaugenum4_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num4_opacity_step);
        lv_img_set_zoom(gaugenum5_img, num5_size_step);
        lv_obj_set_style_local_image_opa(gaugenum5_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num5_opacity_step);
        lv_img_set_zoom(gaugenum6_img, num6_size_step);
        lv_obj_set_style_local_image_opa(gaugenum6_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num6_opacity_step);
        lv_img_set_zoom(gaugenum7_img, num7_size_step);
        lv_obj_set_style_local_image_opa(gaugenum7_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (lv_opa_t)num7_opacity_step);
    }
}

static void deal_speed_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    if (speed_ani != value) {
        speed_ani = value;
        if (speed_ani < 10) {
            lv_obj_set_hidden(num1_img, 0);
            lv_obj_set_x(num1_img, hres / 2 - img_dial_num0.header.w / 2);
            lv_obj_set_hidden(num2_img, 1);
            lv_obj_set_x(num2_img, hres / 2 - img_dial_num0.header.w / 2);
            lv_obj_set_hidden(num3_img, 1);
            lv_obj_set_x(num3_img, hres / 2 - img_dial_num0.header.w / 2);
            paint_dial_num(num1_img, speed_ani % 10);
        } else if (speed_ani < 100) {
            lv_obj_set_hidden(num1_img, 0);
            lv_obj_set_x(num1_img, hres / 2);
            lv_obj_set_hidden(num2_img, 0);
            lv_obj_set_x(num2_img, hres / 2 - img_dial_num0.header.w);
            lv_obj_set_hidden(num3_img, 1);
            lv_obj_set_x(num3_img, hres / 2 - img_dial_num0.header.w / 2);
            paint_dial_num(num1_img, speed_ani % 10);
            paint_dial_num(num2_img, (speed_ani / 10) % 10);
        } else if (speed_ani < 1000) {
            lv_obj_set_hidden(num1_img, 0);
            lv_obj_set_x(num1_img, hres / 2 + img_dial_num0.header.w / 2);
            lv_obj_set_hidden(num2_img, 0);
            lv_obj_set_x(num2_img, hres / 2 - img_dial_num0.header.w / 2);
            lv_obj_set_hidden(num3_img, 0);
            lv_obj_set_x(num3_img, hres / 2 - img_dial_num0.header.w * 3 / 2);
            paint_dial_num(num1_img, speed_ani % 10);
            paint_dial_num(num2_img, (speed_ani / 10) % 10);
            paint_dial_num(num3_img, (speed_ani / 100) % 10);
        } else {
        }
    }
}

static void deal_oil_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    if (oil_ani != value) {
        oil_ani = value;
        if (has_inited == 1) {
            const void* p = NULL;
            if (oil_ani < 100) {
                p = lv_img_get_src(oil_img);
                if (p != &img_dial_oil_warn) {
                    lv_img_set_src(oil_img, &img_dial_oil_warn);
                    lv_img_set_src(oilbg_img, &img_dial_oil_bgwarn);
                }
            } else {
                p = lv_img_get_src(oil_img);
                if (p != &img_dial_oil_norm) {
                    lv_img_set_src(oil_img, &img_dial_oil_norm);
                    lv_img_set_src(oilbg_img, &img_dial_oil_bgnorm);
                }
            }
        }
        lv_draw_mask_angle_param_t oil_param;
        lv_draw_mask_angle_init(&oil_param, img_dial_rpm.header.w / 2, img_dial_oil_bgnorm.header.h / 2, 140, (86 * oil_ani / 10 / MAX_OIL_VALUE + 140) % 360);
        if (oilbg_mask_option == NULL) {
            oilbg_mask_option = lv_objmask_add_mask(oilbg_mask, &oil_param);
            lv_obj_set_hidden(oilbg_mask, 0);
        } else {
            lv_objmask_update_mask(oilbg_mask, oilbg_mask_option, &oil_param);
        }
        if (has_inited == 1) {
            lv_label_set_text_fmt(km_label, "%d km", full_oil_km * oil_ani / 10 / MAX_OIL_VALUE);
        }
    }
}

static void deal_water_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    if (water_ani != value) {
        water_ani = value;
        if (has_inited == 1) {
            const void* p = NULL;
            if (water_ani > 120) {
                p = lv_img_get_src(water_img);
                if (p != &img_dial_water_warn) {
                    lv_img_set_src(water_img, &img_dial_water_warn);
                    lv_img_set_src(waterbg_img, &img_dial_water_bgwarn);
                }
            } else {
                p = lv_img_get_src(water_img);
                if (p != &img_dial_water_norm) {
                    lv_img_set_src(water_img, &img_dial_water_norm);
                    lv_img_set_src(waterbg_img, &img_dial_water_bgnorm);
                }
            }
        }
        lv_draw_mask_angle_param_t water_param;
        lv_draw_mask_angle_init(&water_param, img_dial_water_bgnorm.header.w - img_dial_rpm.header.w / 2, img_dial_water_bgnorm.header.h / 2, (400 - 86 * water_ani / (MAX_WATER_VALUE - MIN_WATER_VALUE)) % 360, 400 - 360);
        if (waterbg_mask_option == NULL) {
            waterbg_mask_option = lv_objmask_add_mask(waterbg_mask, &water_param);
            lv_obj_set_hidden(waterbg_mask, 0);
        } else {
            lv_objmask_update_mask(waterbg_mask, waterbg_mask_option, &water_param);
        }
        if (has_inited == 1) {
            lv_label_set_text_fmt(tem_label, "%d °C", water_ani + MIN_WATER_VALUE);
        }
    }
}

static void deal_tire_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    uint8_t tire1_hide = lv_obj_get_hidden(tire1_img);
    uint8_t tire2_hide = lv_obj_get_hidden(tire2_img);
    uint8_t tire3_hide = lv_obj_get_hidden(tire3_img);
    uint8_t tire4_hide = lv_obj_get_hidden(tire4_img);
    if (tire1_hide == 0) {
        lv_obj_set_style_local_image_opa(tire1_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (uint8_t)value);
    }
    if (tire2_hide == 0) {
        lv_obj_set_style_local_image_opa(tire2_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (uint8_t)value);
    }
    if (tire3_hide == 0) {
        lv_obj_set_style_local_image_opa(tire3_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (uint8_t)value);
    }
    if (tire4_hide == 0) {
        lv_obj_set_style_local_image_opa(tire4_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, (uint8_t)value);
    }
    if (value >= 255) {
        lv_anim_set_values(&tire_ani_struct, 255, 50);
        lv_anim_start(&tire_ani_struct);
    } else if (value <= 50) {
        lv_anim_set_values(&tire_ani_struct, 50, 255);
        lv_anim_start(&tire_ani_struct);
    }
}

static void deal_allmedia_ani(void* _ani, lv_anim_value_t value)
{
    LV_UNUSED(_ani);
    lv_obj_set_x(allmedia_img, (value + 5) / 10);
    if (value <= -((int32_t)img_dial_media_all.header.w * 40 / 5)) {
        lv_obj_set_x(allmedia_img, 0);
    }
}

static void deal_v2x_img_sprite(lv_task_t* task)
{
    if (task != v2x_img_sprite_task) {
        return;
    }
    int16_t px = -((int32_t)img_dial_v2x_lineleft.header.w * (v2x_img_sprite % 10) / 10);
    if (v2x_img_sprite < 10) {
        lv_obj_set_pos(v2xlineleft_img, px, 0);
        lv_obj_set_pos(v2xlineright_img, px, 0);
    } else {
        lv_obj_set_pos(v2xlineleft_img, px, -((int32_t)img_dial_v2x_lineleft.header.h / 2));
        lv_obj_set_pos(v2xlineright_img, px, -((int32_t)img_dial_v2x_lineleft.header.h / 2));
    }
    if (speed_ani > 0) {
        uint8_t sprite_step = speed_ani / 30 + 1;
        v2x_img_sprite += sprite_step;
    }
    if (v2x_img_sprite >= 20) {
        v2x_img_sprite = 0;
    }
}

static void run_test(lv_task_t* task)
{
    if (task != test_task) {
        return;
    }

    uint16_t new_rpm = rand() % 50;
    new_rpm *= 100;
    new_rpm += 1200;
    set_rpm(new_rpm);

    uint16_t new_speed = (new_rpm - 1200) * 240 / 5000;
    set_speed(new_speed);

    uint16_t new_oil = rand() % MAX_OIL_VALUE;
    set_oil(new_oil);

    int16_t new_water = rand() % (MAX_WATER_VALUE - MIN_WATER_VALUE);
    new_water += MIN_WATER_VALUE;
    set_water(new_water);

    int8_t new_gear_state = gear_state;
    switch (new_gear_state) {
    case GEAR_P:
        new_gear_state = GEAR_R;
        break;
    case GEAR_R:
        new_gear_state = GEAR_N;
        break;
    case GEAR_N:
        new_gear_state = GEAR_D;
        break;
    case GEAR_D:
        new_gear_state = GEAR_P;
        break;
    default:
        break;
    }
    set_gear_state(new_gear_state);

    media_test_count++;
    if (media_test_count > 200) {
        media_test_count = 0;
    }
    if (media_test_count % 5 == 0) {
        int8_t new_media_state = media_state;
        switch (media_state) {
        case MEDIA_NONE:
            new_media_state = MEDIA_FM;
            break;
        case MEDIA_FM:
            new_media_state = MEDIA_AM;
            break;
        case MEDIA_AM:
            new_media_state = MEDIA_MUSIC;
            break;
        case MEDIA_MUSIC:
            new_media_state = MEDIA_NONE;
            break;
        default:
            new_media_state = MEDIA_NONE;
            break;
        }
        set_media_state(new_media_state);
    }
}

void set_rpm(uint16_t rpm)
{
    rpm_value = rpm;
    lv_anim_set_values(&rpm_ani_struct, rpm_ani, rpm_value);
    lv_anim_start(&rpm_ani_struct);
}

void set_speed(uint16_t speed)
{
    speed_value = speed;
    lv_anim_set_values(&speed_ani_struct, speed_ani, speed_value);
    lv_anim_start(&speed_ani_struct);
}

void set_oil(uint16_t oil)
{
    oil_value = oil;
    lv_anim_set_values(&oil_ani_struct, oil_ani, oil_value * 10);
    lv_anim_start(&oil_ani_struct);
}

void set_water(int16_t water)
{
    water_value = water;
    int16_t fwater = water_value - MIN_WATER_VALUE;
    lv_anim_set_values(&water_ani_struct, water_ani, fwater < 0 ? 0 : fwater);
    lv_anim_start(&water_ani_struct);
}

void set_trip_value(uint32_t trip)
{
    trip_value = trip;
    lv_label_set_text_fmt(trip_label, "%d.%d", trip_value / 1000, (trip_value % 1000) / 100);
}

void set_total_value(uint32_t total)
{
    total_value = total;
    lv_label_set_text_fmt(total_label, "%d.%d", total_value / 1000, (total_value % 1000) / 100);
}

void set_gear_state(GEAR_STATE state)
{
    gear_state = state;
    switch (gear_state) {
    case GEAR_P:
        lv_label_set_text(gear_label, "P");
        break;
    case GEAR_R:
        lv_label_set_text(gear_label, "R");
        break;
    case GEAR_N:
        lv_label_set_text(gear_label, "N");
        break;
    case GEAR_D:
        lv_label_set_text(gear_label, "D");
        break;
    default:
        break;
    }
    lv_obj_set_hidden(ah_img, !(sys_property.ah == C_ON && gear_state == GEAR_P));
}

void set_light_state(LIGHT_STATE state)
{
    light_state = state;
    switch (state) {
    case LIGHT_CLOSE:
        lv_obj_set_hidden(light_img, 1);
        lv_obj_set_hidden(lightf_img, 1);
        lv_obj_set_hidden(lighth_img, 1);
        lv_obj_set_hidden(lightl_img, 1);
        break;
    case LIGHT_HIGH:
        lv_obj_set_hidden(light_img, 0);
        lv_obj_set_hidden(lightf_img, 1);
        lv_obj_set_hidden(lighth_img, 0);
        lv_obj_set_hidden(lightl_img, 1);
        break;
    case LIGHT_LOW:
        lv_obj_set_hidden(light_img, 0);
        lv_obj_set_hidden(lightf_img, 1);
        lv_obj_set_hidden(lighth_img, 1);
        lv_obj_set_hidden(lightl_img, 0);
        break;
    case LIGHT_FOG:
        lv_obj_set_hidden(light_img, 0);
        lv_obj_set_hidden(lightf_img, 0);
        lv_obj_set_hidden(lighth_img, 1);
        lv_obj_set_hidden(lightl_img, 1);
        break;
    default:
        break;
    }
}

void set_turn_state(TURN_STATE state)
{
    turn_state = state;
    if (turn_ani_task) {
        lv_task_del(turn_ani_task);
        turn_ani_task = NULL;
    }
    switch (state) {
    case TURN_ALL_OFF:
        lv_obj_set_hidden(tleft_img, 1);
        lv_obj_set_hidden(tright_img, 1);
        break;
    case TURN_LEFT:
        turn_ani_hide = 0;
        lv_obj_set_hidden(tleft_img, 0);
        lv_obj_set_hidden(tright_img, 1);
        turn_ani_task = lv_task_create(deal_turn_state, 400, LV_TASK_PRIO_MID, NULL);
        break;
    case TURN_RIGHT:
        turn_ani_hide = 0;
        lv_obj_set_hidden(tleft_img, 1);
        lv_obj_set_hidden(tright_img, 0);
        turn_ani_task = lv_task_create(deal_turn_state, 400, LV_TASK_PRIO_MID, NULL);
        break;
    case TURN_ALL_ON:
        turn_ani_hide = 0;
        lv_obj_set_hidden(tleft_img, 0);
        lv_obj_set_hidden(tright_img, 0);
        turn_ani_task = lv_task_create(deal_turn_state, 600, LV_TASK_PRIO_MID, NULL);
        break;
    default:
        break;
    }
}

void set_media_state(MEDIA_STATE state)
{
    TURN_STATE old_media_state = media_state;
    media_state = state;

    int16_t old_value = 0;

    switch (old_media_state) {
    case MEDIA_NONE:
        old_value = 0;
        break;
    case MEDIA_FM:
        old_value = -((int32_t)img_dial_media_all.header.w * 10 / 5);
        break;
    case MEDIA_AM:
        old_value = -((int32_t)img_dial_media_all.header.w * 20 / 5);
        break;
    case MEDIA_MUSIC:
        old_value = -((int32_t)img_dial_media_all.header.w * 30 / 5);
        break;
    case MEDIA_PHONE:
        old_value = 0;
        break;
    case MEDIA_NAVI:
        old_value = 0;
        break;
    default:
        old_value = 0;
        break;
    }

    switch (media_state) {
    case MEDIA_NONE:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 40 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    case MEDIA_FM:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 10 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    case MEDIA_AM:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 20 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    case MEDIA_MUSIC:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 30 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    case MEDIA_PHONE:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 40 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    case MEDIA_NAVI:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 40 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    default:
        lv_anim_set_values(&allmedia_ani_struct, old_value, -((int32_t)img_dial_media_all.header.w * 40 / 5));
        lv_anim_start(&allmedia_ani_struct);
        break;
    }
}

void set_press_temp_value(TIRE_PRESS_TEMP property)
{
    press_temp = property;
    lv_label_set_text_fmt(tire1_label, "%d.%d bar\n%d °C", press_temp.left_front_press / 100, (press_temp.left_front_press + 5) % 100 / 10, press_temp.left_front_temp);
    lv_label_set_text_fmt(tire2_label, "%d.%d bar\n%d °C", press_temp.right_front_press / 100, (press_temp.right_front_press + 5) % 100 / 10, press_temp.right_front_temp);
    lv_label_set_text_fmt(tire3_label, "%d.%d bar\n%d °C", press_temp.left_behind_press / 100, (press_temp.left_behind_press + 5) % 100 / 10, press_temp.left_behind_temp);
    lv_label_set_text_fmt(tire4_label, "%d.%d bar\n%d °C", press_temp.right_behind_press / 100, (press_temp.right_behind_press + 5) % 100 / 10, press_temp.right_behind_temp);
    uint8_t has_ani = 0;
    if ((press_temp.left_front_press < 150 || press_temp.left_front_press > 300) || (press_temp.left_front_temp < -10 || press_temp.left_front_temp > 60)) {
        has_ani = 1;
        lv_obj_set_hidden(tire1_img, 0);
        if ((press_temp.left_front_press < 100 || press_temp.left_front_press > 350) || (press_temp.left_front_temp < -20 || press_temp.left_front_temp > 80)) {
            lv_img_set_src(tire1_img, &img_dial_media_tire_error);
            lv_obj_set_style_local_text_color(tire1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        } else {
            lv_img_set_src(tire1_img, &img_dial_media_tire_warn);
            lv_obj_set_style_local_text_color(tire1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
        }
    } else {
        lv_obj_set_hidden(tire1_img, 1);
        //lv_img_set_src(tire1_img, NULL);
        lv_obj_set_style_local_text_color(tire1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    }
    if ((press_temp.right_front_press < 150 || press_temp.right_front_press > 300) || (press_temp.right_front_temp < -10 || press_temp.right_front_temp > 60)) {
        has_ani = 1;
        lv_obj_set_hidden(tire2_img, 0);
        if ((press_temp.right_front_press < 100 || press_temp.right_front_press > 350) || (press_temp.right_front_temp < -20 || press_temp.right_front_temp > 80)) {
            lv_img_set_src(tire2_img, &img_dial_media_tire_error);
            lv_obj_set_style_local_text_color(tire2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        } else {
            lv_img_set_src(tire2_img, &img_dial_media_tire_warn);
            lv_obj_set_style_local_text_color(tire2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
        }
    } else {
        lv_obj_set_hidden(tire2_img, 1);
        //lv_img_set_src(tire2_img, NULL);
        lv_obj_set_style_local_text_color(tire2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    }
    if ((press_temp.left_behind_press < 150 || press_temp.left_behind_press > 300) || (press_temp.left_behind_temp < -10 || press_temp.left_behind_temp > 60)) {
        has_ani = 1;
        lv_obj_set_hidden(tire3_img, 0);
        if ((press_temp.left_behind_press < 100 || press_temp.left_behind_press > 350) || (press_temp.left_behind_temp < -20 || press_temp.left_behind_temp > 80)) {
            lv_img_set_src(tire3_img, &img_dial_media_tire_error);
            lv_obj_set_style_local_text_color(tire3_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        } else {
            lv_img_set_src(tire3_img, &img_dial_media_tire_warn);
            lv_obj_set_style_local_text_color(tire3_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
        }
    } else {
        lv_obj_set_hidden(tire3_img, 1);
        //lv_img_set_src(tire3_img, NULL);
        lv_obj_set_style_local_text_color(tire3_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    }
    if ((press_temp.right_behind_press < 150 || press_temp.right_behind_press > 300) || (press_temp.right_behind_temp < -10 || press_temp.right_behind_temp > 60)) {
        has_ani = 1;
        lv_obj_set_hidden(tire4_img, 0);
        if ((press_temp.right_behind_press < 100 || press_temp.right_behind_press > 350) || (press_temp.right_behind_temp < -20 || press_temp.right_behind_temp > 80)) {
            lv_img_set_src(tire4_img, &img_dial_media_tire_error);
            lv_obj_set_style_local_text_color(tire4_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        } else {
            lv_img_set_src(tire4_img, &img_dial_media_tire_warn);
            lv_obj_set_style_local_text_color(tire4_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
        }
    } else {
        lv_obj_set_hidden(tire4_img, 1);
        //lv_img_set_src(tire4_img, NULL);
        lv_obj_set_style_local_text_color(tire4_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    }

    lv_anim_del(&tire_ani_struct, deal_tire_ani);
    lv_obj_set_style_local_image_opa(tire1_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_image_opa(tire1_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_image_opa(tire1_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_image_opa(tire1_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 1);
    if (has_ani == 1) {
        lv_anim_set_values(&tire_ani_struct, 50, 255);
        lv_anim_start(&tire_ani_struct);
    }
}

void set_sys_property_value(SYS_PROPERTY property)
{
    sys_property = property;
    lv_obj_set_hidden(abs_img, !sys_property.abs);
    lv_obj_set_hidden(eco_img, !sys_property.eco);
    lv_obj_set_hidden(egls_img, !sys_property.egls);
    lv_obj_set_hidden(epc_img, !sys_property.epc);
    lv_obj_set_hidden(espoff_img, !sys_property.espoff);
    lv_obj_set_hidden(hdc_img, !sys_property.hdc);
    lv_obj_set_hidden(sea1_img, !sys_property.sea1);
    lv_obj_set_hidden(sea2_img, !sys_property.sea2);

    lv_obj_set_hidden(ah_img, !(sys_property.ah == C_ON && gear_state == GEAR_P));
}

void ejanus_start(uint32_t delayms)
{
    has_inited = 0;
    if (delayms <= 0) {
        begin_ani1(NULL);
    } else {
        begin_task1 = lv_task_create(begin_ani1, delayms, LV_TASK_PRIO_MID, NULL);
        lv_task_set_repeat_count(begin_task1, 1);
    }
}
