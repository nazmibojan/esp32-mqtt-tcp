#include "adl_ntp.h"

static const char *NTP_FILE = "adl_ntp.c";

/**
 * @brief Initialize NTP client in order to obtain current datetime
 */
void ntp_init(void) {
	static bool init_state = false;

	// only do these once
	if (!init_state) {
		ESP_LOGI(NTP_FILE, "Initializing SNTP");
		sntp_setoperatingmode(SNTP_OPMODE_POLL);
		sntp_setservername(0, "pool.ntp.org");
		init_state = true;
	}

    sntp_init();
}

/**
 * @brief Fetch current datetime from a ntp server to Real-Time Clock peripheral.
 *
 * 
 * Function will retry to obtain ntp datetime 5 times. If failed, current
 * datetime in RTC equals to 1 January 1970. Please refer to ``struct tm`` members for
 * more information about datetime format.
 * 
 * @return True if success, false otherwise.
 */
bool ntp_set_datetime(void) {
    ntp_init();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 5;
    while(timeinfo.tm_year < (2018 - 1900) && ++retry < retry_count) {
        ESP_LOGI(NTP_FILE, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
    return (timeinfo.tm_year >= (2018 - 1900)) && (retry < retry_count);
}

/**
 * @brief Obtain current datetime from RTC.
 *
 * Function will automatically detect outdated datetime and try to call ntp_set_datetime()
 * if local RTC datetime is correct, function will return proper data.
 * 
 * @return Returns current datetime in ``struct tm`` structure.
 */
struct tm ntp_obtain_datetime(void) {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    if (timeinfo.tm_year < (2018 - 1900)) {
        ESP_LOGI(NTP_FILE, "Time is not set yet. Getting time over NTP.");
        ntp_set_datetime();
        time(&now);
    }

    setenv("TZ", "UTC-7", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    return timeinfo;
}
