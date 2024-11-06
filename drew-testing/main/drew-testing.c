#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "hal/adc_types.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali_scheme.h"  // Include the ADC calibration scheme header

static const char *TAG = "ADC_EXAMPLE";

static adc_oneshot_unit_handle_t adc_handle;     // ADC handle
//static adc_cali_handle_t cali_handle; //cali handle

void app_main(void)
{
    // GPIO setup
    gpio_set_direction(GPIO_NUM_1, GPIO_MODE_OUTPUT);

    // ADC configuration
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&unit_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,  // Use the default bit width
        .atten = ADC_ATTEN_DB_12,  // Using the updated attenuation constant
    };
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_5, &config);

    int adc_raw = 0;
    while (true) {
        // Read ADC value
        adc_oneshot_read(adc_handle, ADC_CHANNEL_5, &adc_raw);
        printf("RAW ADC = %d ", adc_raw);

        //adc_oneshot_get_calibrated_result(adc_handle, ADC_CHANNEL_5, &)

        // Check battery level
        if (adc_raw <= 1000) {
            ESP_LOGW(TAG, "Voltage lower than 1000mV");
            gpio_set_level(GPIO_NUM_1, 1);  // Turn on LED or alert
        } else {
            gpio_set_level(GPIO_NUM_1, 0);  // Turn off LED or alert
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for a second
    }

    // Clean up (won't be reached in this example but good practice in other cases)
    adc_oneshot_del_unit(adc_handle);
}
