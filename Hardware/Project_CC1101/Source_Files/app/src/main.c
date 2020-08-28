#include "main.h"

/**
 * @brief Actually we do not use that in this edition
 * 
 */
const char *testword = "srtptest";
/**
 * @brief Data, TX to RX CC1101, here is the structure
 * +----------------------+----------------------+----------------------+----------------------+
 * |       data[0]        |       data[1]        |       data[2]        |       data[3]        |
 * +----------------------+----------------------+----------------------+----------------------+
 * |  (package>>24)&0xff  |  (package>>16)&0xff  |  (package>> 8)&0xff  |  (package>> 0)&0xff  |
 * +----------------------+----------------------+----------------------+----------------------+
 * 
 */
uint8_t data[4];
/**
 * @brief Data, RX CC1101 to PC, here is the structure
 * +---------------------------------+---------------+
 * |  rxBuffer[0:4] (not include 4)  |  rxBuffer[4]  |
 * +---------------------------------+---------------+
 * |      data[0:4] (not include 4)  |      RSSI     |
 * +---------------------------------+---------------+
 * 
 */
uint8_t rxBuffer[5] = {0};
uint8_t rssi = 0;
/**
 * @brief Use this to synchonise more than 1 CC1101, it equals to
 * package index.
 * 
 */
uint32_t packCnt = 0;

int main(void) {
    int i;
    uint32_t j;
    uint8_t lenth = 0;
    // Init UART
    drv_uart_init(9600);
    // Init LED
    drv_led_init();
    led_switch(0b01);

    // Init SPI
    drv_spi_init();
    // Init CC1101
    CC1101_Init();
    led_switch(0b10);
    drv_delay_ms(500);
    led_switch(0b00);

// Macro __CC1101_TX_TEST__ is defined in "main.h"

#ifdef __CC1101_TX_TEST__
    // Init button, actually we do not use that
    drv_button_init();
    while (1) {
        j = packCnt;
        // Divide uint32_t into 4*uint8_t and record them
        for (i = 0; i < 4; ++i) {
            data[3-i] = j & 0xff;
            j >>= 8;
        }
        ++packCnt;
        CC1101_Tx_Packet((uint8_t *)data, 4, ADDRESS_CHECK);
        drv_delay_ms(20);
        led_red_flashing();
    }
#else
    while (1) {
        CC1101_Clear_RxBuffer();
        CC1101_Set_Mode(RX_MODE);
        lenth = CC1101_Rx_Packet(rxBuffer);
        // FIXME: Dangerous operation, if lenth != 4, it will send wrong data to PC
        rxBuffer[4] = CC1101_Read_Status(CC1101_RSSI);
        if (0 != lenth) {
            led_green_flashing();
            drv_uart_tx_bytes(rxBuffer, lenth+1);
        }
    }
#endif
    return 0;
}
