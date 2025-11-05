#include <LPC17xx.h>

// Connect your LED's anode (+) to P0.22 and cathode (-) to GND through a 220-330O resistor
#define LED_PIN (1 << 22) // P0.22

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 20000; j++);
}

int main(void) {
    SystemInit();

    LPC_GPIO0->FIODIR |= LED_PIN; // Set P0.22 as output

    while(1) {
        LPC_GPIO0->FIOSET = LED_PIN; // Turn LED ON
        delay_ms(500);               // Wait 500ms
        LPC_GPIO0->FIOCLR = LED_PIN; // Turn LED OFF
        delay_ms(500);               // Wait 500ms
    }
}
