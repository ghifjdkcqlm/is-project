#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

unsigned int dig_count = 0, temp1 = 0;
unsigned char array_dec[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
                              0x6D, 0x7D, 0x07, 0x7F, 0x6F,
                              0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

unsigned char one_sec_flag = 0;
unsigned int twenty_count = 0;

// Ring counter position (binary): one bit set and circulating
unsigned int ring = 0x0001;

void delay(void);
void display(void);

int main(void) {
    unsigned int i;
    
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 &= 0xFF0000FF; // P0.4-P0.11 GPIO
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF; // P1.23-P1.26 GPIO

    LPC_GPIO0->FIODIR |= 0x00000FF0;   // P0.4-P0.11 output (segments)
    LPC_GPIO1->FIODIR |= 0x07800000;   // P1.23-P1.26 output (digit enable)

    while (1) {
        delay();

        dig_count++;
        if (dig_count == 5) {
            dig_count = 0;
        }

        if (one_sec_flag) {
            one_sec_flag = 0;

            // Rotate ring counter one position left (4-bit ring)
            ring <<= 1;
            if (ring == 0x10) {
                ring = 0x01;
            }
        }
        display();
    }
}

void display(void) {
    unsigned int temp2;
    unsigned long int i;
    
    if (dig_count == 1) {
        temp1 = (ring & 0x01) ? 1 : 0; // Digit 1
        LPC_GPIO1->FIOPIN = FIRST_SEG;
    } else if (dig_count == 2) {
        temp1 = (ring & 0x02) ? 1 : 0; // Digit 2
        LPC_GPIO1->FIOPIN = SECOND_SEG;
    } else if (dig_count == 3) {
        temp1 = (ring & 0x04) ? 1 : 0; // Digit 3
        LPC_GPIO1->FIOPIN = THIRD_SEG;
    } else if (dig_count == 4) {
        temp1 = (ring & 0x08) ? 1 : 0; // Digit 4
        LPC_GPIO1->FIOPIN = FOURTH_SEG;
    }

    temp1 &= 0x0F;
    temp2 = array_dec[temp1] << 4;
    LPC_GPIO0->FIOPIN = temp2;

    for (i = 0; i < 500; i += 1);

    LPC_GPIO0->FIOCLR = 0x00000FF0;
}

void delay(void) {
    unsigned int i;
    for (i = 0; i < 10000; i += 1);

    if (twenty_count == 1000) {
        one_sec_flag = 1;
        twenty_count = 0;
    } else {
        twenty_count++;
    }
}
