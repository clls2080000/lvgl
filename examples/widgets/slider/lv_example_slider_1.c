#include "../../lv_examples.h"
#if LV_USE_SLIDER && LV_BUILD_EXAMPLES

static void slider_event_cb(lv_event_t * e);
static lv_obj_t * label;

static void set_angle(void * img, int32_t v)
{
    lv_obj_invalidate(img);
    lv_obj_set_style_transform_angle(img, v, 0);
}
/**
 * A default slider with a label displaying the current value
 */
void lv_example_slider_1(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x00ff00), 0);

	lv_obj_t * cont = lv_obj_create(lv_scr_act());
	lv_obj_set_style_radius(cont, 0, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_opa(cont, 128, 0);
//    lv_obj_set_style_shadow_width(cont, 50, 0);
//    lv_obj_set_style_transform_angle(cont, 500, 0);
//    lv_obj_set_style_transform_zoom(cont, 256*3, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(cont, 30, 0);
    lv_obj_set_style_pad_left(cont, 50, 0);
	lv_obj_set_size(cont, 400, 400);
    lv_obj_align(cont, LV_ALIGN_CENTER, 500, -50);
    static const char * cont_name = "Cont";
    lv_obj_set_user_data(cont, cont_name);
    lv_obj_t * cont2 = lv_obj_create(cont);
    lv_obj_set_size(cont2, lv_pct(100), 200);
    lv_obj_set_flex_flow(cont2, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(cont2, 30, 0);
    lv_obj_set_style_pad_column(cont2, 15, 0);
//    lv_obj_set_style_transform_angle(cont2, -98, 0);
//    lv_obj_set_style_opa(cont2, 150, 0);
    static const char * cont2_name = "Cont2";
    lv_obj_set_user_data(cont2, cont2_name);

    uint32_t i;
    for(i = 0; i < 36; i++) {
        lv_obj_t * sw = lv_switch_create(cont2);
//        lv_obj_set_style_transform_angle(sw, 100 * i, 0);

        char * name = lv_mem_alloc(32);
        lv_snprintf(name, 32, "Switch %d", i);
        lv_obj_set_user_data(sw, name);
    }

    /*Create a slider in the center of the display*/
    lv_obj_t * slider1 = lv_slider_create(cont);
    lv_obj_t * slider2 = lv_slider_create(cont);
    lv_obj_t * slider3 = lv_slider_create(cont);
    lv_obj_t * slider4 = lv_slider_create(cont);

//    lv_obj_set_style_transform_angle(slider2, 200, 0);
//    lv_obj_set_style_transform_zoom(slider3, 128, 0);
//    lv_obj_set_style_transform_angle(slider4, 200, 0);
//    lv_obj_set_style_transform_zoom(slider4, 128, 0);

    static const char * slider1_name = "Slider1";
    lv_obj_set_user_data(slider1, slider1_name);
    static const char * slider2_name = "Slider2";
    lv_obj_set_user_data(slider2, slider2_name);
    static const char * slider3_name = "Slider3";
    lv_obj_set_user_data(slider3, slider3_name);
    static const char * slider4_name = "Slider4";
    lv_obj_set_user_data(slider4, slider4_name);

    label = lv_label_create(cont);
    lv_obj_set_x(label, -20);
    lv_obj_add_flag(label, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(label,"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//    lv_obj_set_style_transform_angle(label, 900, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, slider1);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_values(&a, 0, 3600);
    lv_anim_set_time(&a, 15000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//    lv_anim_start(&a);


}
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
//    lv_label_set_text(slider_label, buf);
//    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

}

#endif
