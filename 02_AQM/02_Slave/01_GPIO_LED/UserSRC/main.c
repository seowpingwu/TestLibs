#include <stm32f10x_conf.h>

int main(void){
	
  SystemInit();
	Systick_Config(72);
	Sys_LEDUserConfig();
	
	while (1){
		
		LED_Runing_Low; 
		Systick_Delayms(1000);
		LED_Runing_High; 
		Systick_Delayms(1000);

   }
}

