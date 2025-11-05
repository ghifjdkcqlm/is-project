#include <lpc17xx.h>

int i,j,x;

int main(){
  LPC_PINCON->PINSEL0=0;
	LPC_GPIO0->FIODIR =0XFF<<4;
	LPC_PINCON->PINSEL4=0;
	LPC_GPIO2->FIODIR &=~(1<<12);
	

    while (1) {
        // Read SW2 state (1 = pressed, 0 = released)
        x = (LPC_GPIO2->FIOPIN >> 12) & 1;

        // Display current count on LEDs
         
			if(x==1){
			for(i=8;i>0;i++){
				LPC_GPIO0->FIOCLR = 0x00000FF0;
			LPC_GPIO0->FIOSET = 1<<(i+3);
				for(j=0;j<100000;j++);
}
      
  }  }
	}
