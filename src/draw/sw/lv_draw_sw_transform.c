/**
 * @file lv_templ.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    struct {
        const void * src;           /*image source (array of pixels)*/
        lv_coord_t src_w;           /*width of the image source*/
        lv_coord_t src_h;           /*height of the image source*/
        lv_coord_t pivot_x;         /*pivot x*/
        lv_coord_t pivot_y;         /*pivot y*/
        int16_t angle;              /*angle to rotate*/
        uint16_t zoom;              /*256 no zoom, 128 half size, 512 double size*/
        lv_color_t color;           /*a color used for `LV_IMG_CF_INDEXED_1/2/4/8BIT` color formats*/
        lv_img_cf_t cf;             /*color format of the image to rotate*/
        bool antialias;
    } cfg;

    struct {
        lv_color_t color;
        lv_opa_t opa;
    } res;

    struct {
        lv_img_dsc_t img_dsc;
        int32_t pivot_x_256;
        int32_t pivot_y_256;
        int32_t sinma;
        int32_t cosma;

        uint8_t chroma_keyed : 1;
        uint8_t has_alpha : 1;
        uint8_t native_color : 1;

        uint32_t zoom_inv;

        /*Runtime data*/
        lv_coord_t xs;
        lv_coord_t ys;
        lv_coord_t xs_int;
        lv_coord_t ys_int;
        uint32_t pxi;
        uint8_t px_size;
    } tmp;
} lv_img_transform_dsc_t;


/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


void lv_draw_sw_transform(const lv_area_t * dest_area, const void * src_buf, lv_coord_t src_w, lv_coord_t src_h,
                          lv_coord_t src_stride, const lv_draw_img_dsc_t * draw_dsc, lv_img_cf_t cf, lv_color_t * cbuf, lv_opa_t * abuf)
{
    lv_img_transform_dsc_t trans_dsc;
    lv_memset_00(&trans_dsc, sizeof(lv_img_transform_dsc_t));
    trans_dsc.cfg.angle = draw_dsc->angle;
    trans_dsc.cfg.zoom = draw_dsc->zoom;
    trans_dsc.cfg.src = src_buf;
    trans_dsc.cfg.src_w = src_w;
    trans_dsc.cfg.src_h = src_h;
    trans_dsc.cfg.cf = cf;
    trans_dsc.cfg.pivot_x = draw_dsc->pivot.x;
    trans_dsc.cfg.pivot_y = draw_dsc->pivot.y;
    trans_dsc.cfg.color = draw_dsc->recolor;
    trans_dsc.cfg.antialias = draw_dsc->antialias;
    _lv_img_buf_transform_init(&trans_dsc);

    lv_coord_t dest_w = lv_area_get_width(dest_area);
    lv_coord_t dest_h = lv_area_get_height(dest_area);
    lv_coord_t y;
    bool has_alpha = lv_img_cf_has_alpha(cf);
    for(y = 0; y < dest_h; y++) {
        int32_t xs1_ups, ys1_ups, xs2_ups, ys2_ups;
        transform_point(&trans_dsc, dest_area->x1, dest_area->y1 + y, &xs1_ups, &ys1_ups);
        transform_point(&trans_dsc, dest_area->x2, dest_area->y1 + y, &xs2_ups, &ys2_ups);

        int32_t xs_step = (xs2_ups - xs1_ups) / dest_w;
        int32_t ys_step = (ys2_ups - ys1_ups) / dest_w;
        int32_t xs_ups = xs1_ups + xs_step / 2;     /*Init. + go the center of the pixel*/
        int32_t ys_ups = ys1_ups + ys_step / 2;

        lv_coord_t x;
        if(trans_dsc.cfg.antialias == 0) {
            for(x = 0; x < dest_w; x++) {
                int32_t xs_int = xs_ups >> 8;
                int32_t ys_int = ys_ups >> 8;
                if(xs_int < 0 || xs_int >= src_w || ys_int < 0 || ys_int >= src_h) {
                    abuf[x] = 0;
                    //                    printf("s\n");
                }
                else {
                    //                    printf("o\n");
                    const uint8_t * src_tmp;
                    src_tmp = trans_dsc.cfg.src;
                    src_tmp += (ys_int * src_stride * LV_IMG_PX_SIZE_ALPHA_BYTE) + xs_int * LV_IMG_PX_SIZE_ALPHA_BYTE;

#if LV_COLOR_DEPTH == 16
                    cbuf[x].full = src_tmp[0] + (src_tmp[1] << 8);
#elif LV_COLOR_DEPTH == 32
                    cbuf[x].full = *((uint32_t *)src_tmp);
#endif
                    //                    if(has_alpha)
                    abuf[x] = src_tmp[LV_IMG_PX_SIZE_ALPHA_BYTE - 1];
                }
                xs_ups += xs_step;
                ys_ups += ys_step;
            }
        }
        else {
            for(x = 0; x < dest_w; x++) {
                int32_t xs_int = xs_ups >> 8;
                int32_t ys_int = ys_ups >> 8;

                /*Fully out of the image*/
                if(ys_int < 0 || ys_int >= src_h) {
                    abuf[x] = 0x00;
                    ys_ups += ys_step;
                    xs_ups += xs_step;
                    continue;
                }

                /*Get the direction the hor and ver neighbor
                 *`fract` will be in range of 0x00..0xFF and `next` (+/-1) indicates the direction*/
                int32_t xs_fract = xs_ups & 0xFF;
                int32_t ys_fract = ys_ups & 0xFF;
                int32_t x_next;
                int32_t y_next;
                if(xs_fract < 0x80) {
                    x_next = -1;
                    xs_fract = (0x7F - xs_fract) * 2;
                }
                else {
                    x_next = 1;
                    xs_fract = (xs_fract - 0x80) * 2;
                }
                if(ys_fract < 0x80) {
                    y_next = -1;
                    ys_fract = (0x7F - ys_fract) * 2;
                }
                else {
                    y_next = 1;
                    ys_fract = (ys_fract - 0x80) * 2;
                }

                const uint8_t * src_tmp;
                src_tmp = trans_dsc.cfg.src;
                src_tmp += (ys_int * src_stride * LV_IMG_PX_SIZE_ALPHA_BYTE) + xs_int * LV_IMG_PX_SIZE_ALPHA_BYTE;

                if(xs_int + x_next >= 0 &&
                   xs_int + x_next <= src_w - 1 &&
                   ys_int + y_next >= 0 &&
                   ys_int + y_next <= src_h - 1) {

                    const uint8_t * px_base = NULL;
                    const uint8_t * px_hor = NULL;
                    const uint8_t * px_ver = NULL;
                    px_base = src_tmp;
                    px_hor = src_tmp + x_next * LV_IMG_PX_SIZE_ALPHA_BYTE;
                    px_ver = src_tmp + y_next * src_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;

                    lv_opa_t a_base = px_base[LV_IMG_PX_SIZE_ALPHA_BYTE - 1];
                    lv_opa_t a_ver = px_ver[LV_IMG_PX_SIZE_ALPHA_BYTE - 1];
                    lv_opa_t a_hor = px_hor[LV_IMG_PX_SIZE_ALPHA_BYTE - 1];

                    if(a_ver != a_base) a_ver = ((a_ver * ys_fract) + (a_base * (0x100 - ys_fract))) >> 8;
                    if(a_hor != a_base) a_hor = ((a_hor * xs_fract) + (a_base * (0x100 - xs_fract))) >> 8;
                    abuf[x] = (a_ver + a_hor) >> 1;

                    if(abuf[x]) {
                        lv_color_t c_base;
                        lv_color_t c_ver;
                        lv_color_t c_hor;
                        c_base.full = px_base[0] + (px_base[1] << 8);
                        c_ver.full = px_ver[0] + (px_ver[1] << 8);
                        c_hor.full = px_hor[0] + (px_hor[1] << 8);
                        c_ver = lv_color_mix(c_ver, c_base, ys_fract);
                        c_hor = lv_color_mix(c_hor, c_base, xs_fract);
                        cbuf[x] = lv_color_mix(c_hor, c_ver, LV_OPA_50);
                    }
                }
                else {
                    cbuf[x].full = src_tmp[0] + (src_tmp[1] << 8);
                    lv_opa_t a = src_tmp[LV_IMG_PX_SIZE_ALPHA_BYTE - 1];

                    if((xs_int == 0 && x_next < 0) || (xs_int == src_w - 1 && x_next > 0))  {
                        abuf[x] = (a * (0xFF - xs_fract)) >> 8;
                    }
                    else if((ys_int == 0 && y_next < 0) || (ys_int == src_h - 1 && y_next > 0))  {
                        abuf[x] = (a * (0xFF - ys_fract)) >> 8;
                    }
                    else {
                        abuf[x] = 0x00;
                    }
                }

                xs_ups += xs_step;
                ys_ups += ys_step;
            }
        }
        cbuf += dest_w;
        abuf += dest_w;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/



