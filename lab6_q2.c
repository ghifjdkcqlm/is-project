#include <LPC17xx.h>

#define LED_PIN     (1 << 22)   // P0.22 for LED
#define SWITCH_PIN  (1 << 12)   // P2.12 for switch

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 20000; j++);
}

int main(void) {
    SystemInit();

    // Configure directions
    LPC_GPIO0->FIODIR |= LED_PIN;     // P0.22 as output (LED)
    LPC_GPIO2->FIODIR &= ~SWITCH_PIN; // P2.12 as input  (Switch)

    while(1) {
        // Active LOW: button press connects pin to GND
        if((LPC_GPIO2->FIOPIN & SWITCH_PIN) == 0) {
            LPC_GPIO0->FIOSET = LED_PIN; // LED ON
        } else {
            LPC_GPIO0->FIOCLR = LED_PIN; // LED OFF
        }
        delay_ms(10); // Small debounce
    }
}
