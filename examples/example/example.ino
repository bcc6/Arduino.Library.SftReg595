#include <sndlss_SftReg595.h>

#define LED_SH_PIN   10  // SRCLK
#define LED_ST_PIN   11  // RCLK
#define LED_DS_PIN   12  // DATA
SftReg595 leds(LED_SH_PIN, LED_ST_PIN, LED_DS_PIN);

void setup() {
}

void loop() {
	leds.write(0xFFFFFFFF);
    leds.update();
    delay(500);
	leds.write(0x00000000);
    leds.update();
    delay(500);
}
