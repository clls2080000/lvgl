#include "../../lv_examples.h"
#if LV_USE_SLIDER && LV_BUILD_EXAMPLES

static void slider_event_cb(lv_event_t * e);
static lv_obj_t * slider_label;

/**
 * A default slider with a label displaying the current value
 */
void lv_example_slider_1(void)
{

	lv_obj_t * cont = lv_obj_create(lv_scr_act());
	lv_obj_set_size(cont, 300, 300);
    lv_obj_center(cont);
    lv_obj_set_style_transform_angle(cont, -100, 0);
    lv_obj_set_style_transform_zoom(cont, 150, 0);
//    lv_obj_add_flag(cont, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(cont);
//    lv_obj_add_flag(slider, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_center(slider);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_style_transform_angle(slider, 100, 0);
    lv_obj_set_style_transform_zoom(slider, 100, 0);
    /*Create a label below the slider*/
    slider_label = lv_label_create(cont);
    lv_label_set_text(slider_label, "123456789");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);
    lv_obj_allocate_spec_attr(slider);
    lv_obj_add_flag(slider, LV_OBJ_FLAG_SNAPSHOT);
    lv_obj_invalidate(slider);
    lv_obj_allocate_spec_attr(cont);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_SNAPSHOT);
    lv_obj_invalidate(cont);

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
