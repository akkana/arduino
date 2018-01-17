int led_init(int addr);
void led_fini(int fd);

void led_printf (int fd, const char *fmt, ...);

void led_brightness_set (int fd, uint8_t val);
void led_sleep_set (int fd, int val);
uint8_t led_status_get (int fd);

void led_version_print (int fd);
void led_addr_set (int fd, uint8_t newaddr);
void led_test (int fd);
void led_reset (int fd);

#define LED_ADDR_FACTORY	0x27
#define LED_ADDR_ADDRMODE	0x51
