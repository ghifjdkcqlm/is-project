#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF

unsigned int dig1 = 0x00, dig2 = 0x00, dig3 = 0x00, dig4 = 0x00;
unsigned int twenty_count = 0x00, dig_count = 0x00, temp1 = 0x00;
unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char one_sec_flag = 0x00;
unsigned long int temp2 = 0x00000000;
unsigned long int i = 0;

void delay(void);
void display(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    
    LPC_PINCON->PINSEL0 &= 0xFF0000FF; // P0.4 to P0.11 GPIO
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF; // P1.23 to P1.26 GPIO
    
    LPC_GPIO0->FIODIR |= 0x00000FF0;   // P0.4 to P0.11 output
    LPC_GPIO1->FIODIR |= 0x07800000;   // P1.23 to P1.26 output
    
    while (1) {
        delay();
        dig_count++;
        if (dig_count == 0x05) {
            dig_count = 0x00;
        }
        
        if (one_sec_flag == 0xFF) {
            one_sec_flag = 0x00;
            dig1++;
            if (dig1 == 0x0A) {
                dig1 = 0;
                dig2++;
                if (dig2 == 0x0A) {
                    dig2 = 0;
                    dig3++;
                    if (dig3 == 0x0A) {
                        dig3 = 0;
                        dig4++;
                        if (dig4 == 0x0A) {
                            dig4 = 0;
                        }
                    }
                }
            }
        }
        display();
    }
}

void display(void) {
    if (dig_count == 0x01) {
        temp1 = dig1;
        LPC_GPIO1->FIOPIN = FIRST_SEG;
    } else if (dig_count == 0x02) {
        temp1 = dig2;
        LPC_GPIO1->FIOPIN = SECOND_SEG;
    } else if (dig_count == 0x03) {
        temp1 = dig3;
        LPC_GPIO1->FIOPIN = THIRD_SEG;
    } else if (dig_count == 0x04) {
        temp1 = dig4;
        LPC_GPIO1->FIOPIN = FOURTH_SEG;
    }
    
    temp1 &= 0x0F;
    temp2 = array_dec[temp1]; // Decode digit to 7-segment
    temp2 = temp2 << 4;       // Shift to P0.4-P0.10 bits
    
    LPC_GPIO0->FIOPIN = temp2;
    
    for (i = 0; i < 500; i++); // Brief display duration
    
    LPC_GPIO0->FIOCLR = 0x00000FF0; // Clear segment bits
    // LPC_GPIO1->FIOPIN = DISABLE_ALL;  // Disable all digit outputs if required
}

void delay(void) {
    unsigned int i;
    for (i = 0; i < 1000; i++); // Short software delay
    if (twenty_count == 1000) { // Counts approx 1 second total when multiplied by display delay
        one_sec_flag = 0xFF;
        twenty_count = 0x00;
    } else {
        twenty_count++;
    }
}
