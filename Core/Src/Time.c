#include "Time.h"
#include "stm32f4xx_hal.h"
uint32_t getCurrentTime(){
	return HAL_GetTick();
}
