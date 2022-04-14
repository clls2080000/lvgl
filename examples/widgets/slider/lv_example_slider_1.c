#include "../../lv_examples.h"
#if LV_USE_SLIDER && LV_BUILD_EXAMPLES

static void slider_event_cb(lv_event_t * e);
static lv_obj_t * slider_label;

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
	lv_obj_set_size(cont, 400, 400);
    lv_obj_center(cont);
//    lv_obj_set_pos(cont, 400, 100);

    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_dropdown_create(cont);
    lv_obj_center(slider);
//    lv_obj_set_pos(slider, 200, 200);
//    lv_obj_set_size(slider, 100, 20);

    /*Create a label below the slider*/
    slider_label = lv_label_create(cont);
    lv_label_set_text(slider_label,
            "a\nb\nc\nd\ne\nf\ng\nh\ni\nj\nk\nl\nj\nm\n"
            "n\no\np\nq\nr\ns\nt\nu\nu\nv\v\nw\nx\ny\nz");

    lv_obj_set_style_bg_color(cont, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_opa(cont, 200, 0);
    lv_obj_set_style_transform_angle(cont, 400, 0);
    lv_obj_set_style_opa(slider, 250, 0);
    lv_obj_set_style_transform_angle(slider, 1200, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, slider);
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
