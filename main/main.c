
#include <stdio.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "demos/lv_demos.h"

#include "capi_video.h"
#include "./init.h"
#include "./lv_conf.h"
/** Defines **/
static void IRAM_ATTR video_disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static void IRAM_ATTR video_monitor_cb(lv_disp_drv_t * disp_drv, uint32_t time_ms, uint32_t px_num);
static void lv_benchmark_loop();
static void init_all();

/** Entry **/
void app_main(void) {
    init_all();
    lv_benchmark_loop();
}

/** Implement **/

static void init_all() {
    // system: timer/ticker
    system_init();
    // composite video: PAL
    esp32_video_start(0);
    // LVGL
    lv_init();
    static lv_color_t lv_disp_buf[LV_HOR_RES_MAX*10];
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    lv_disp_draw_buf_init(&draw_buf_dsc_2, lv_disp_buf, NULL, LV_HOR_RES_MAX*10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = video_disp_flush;
    disp_drv.monitor_cb = video_monitor_cb;
    disp_drv.draw_buf = &draw_buf_dsc_2;
    lv_disp_drv_register(&disp_drv);
}

static void lv_benchmark_loop() {
    lv_demo_benchmark(LV_DEMO_BENCHMARK_MODE_RENDER_AND_DRIVER);
    while(1){
        lv_task_handler();
    }
}

static void IRAM_ATTR video_disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
    lv_color_t *color_p_dac = color_p;
    for(int y = area->y1; y <= area->y2; ++y) {
        for(int x = area->x1; x <= area->x2; ++x) {
            esp32_video_set_color(x, y, 
                lv_color_to8(*color_p_dac));
            ++color_p_dac;
        }
    }
    lv_disp_flush_ready(disp_drv);
}
static void IRAM_ATTR video_monitor_cb(lv_disp_drv_t * disp_drv, uint32_t time_ms, uint32_t px_num) {
    esp32_video_sync();
}