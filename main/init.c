#include "./init.h"
static void timer_periodic_cb(void *arg) {
    lv_tick_inc(1);
}
void system_init() {
    static esp_timer_handle_t esp_timer_tick = 0;

    static  esp_timer_create_args_t periodic_arg = { .callback =
            &timer_periodic_cb, 
            .arg = NULL, 
            .name = "LVGL_TICK_TIMER" 
    };
    esp_err_t err;
    err = esp_timer_create(&periodic_arg, &esp_timer_tick);
    ESP_ERROR_CHECK(err);
    err = esp_timer_start_periodic(esp_timer_tick, 1 * 1000);
    ESP_ERROR_CHECK(err);

}