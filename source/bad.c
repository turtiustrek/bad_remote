#include <stdio.h>
#include "lcd.h"
#include "delay.h"
#include "peripherals.h"
#include "sequence.h"
#include "frames.h"
static volatile uint8_t screenUpdateTick = 0;
static volatile uint32_t buzzerDuration = 0;

static uint8_t frameBuf[(LCD_WIDTH * LCD_HEIGHT) / 8] = { 0 };

void SysTick_Handler(void) {
	g_msTicks++;
	if ((g_msTicks % 3) == 0) {
		if (buzzerDuration) {
			buzzerDuration--;
		}
	}
	if ((g_msTicks % 75) == 0) {
		screenUpdateTick = 1;
	}
}
void resizePixels(uint8_t *src, uint8_t *dst,uint32_t w1, uint32_t h1, uint32_t w2, uint32_t h2,uint32_t x_ratio,uint32_t y_ratio) {
	//nearest neighbor scaling algorithm for vertical pages ONLY

	// int x2, y2;
	for (uint32_t i = 0; i < h2; i++) {
		for (uint32_t j = 0; j < w2; j += 4) {
			/*turtius: okay this looks scary but the main idea here is to index through a vertical buffer split into pages
			 * since the display works by indexing vertically(8 pixels) and moving on to the next page(128*8) for the horizontal axis.
			 * (y/8) * w2 + x returns the respective char which contains 8 pixels and the pixel itself is  found by i % 8
			 * and then use the respective ratio's to find the index of the destination.So to get a single pixel it looks something like this:
			 * ((i * y_ratio) >> 16) % 8)) & 0x1)
			 *also to increase speed, loop unrolling has been done but there a problem though, the pixel cannot be cleared with OR so ensure
			 *that the buffer is cleared on the next call
			 * tl;dr it should look something like this formally:
			 * x2 = ((j*x_ratio)>>16) ;
			 * y2 = ((i*y_ratio)>>16) ;
			 * dst[(i*w2)+j] = source[(y2*w1)+x2] ;
			 */
			dst[((i) / 8) * w2 + (j)] |= (((src[((((i * y_ratio) >> 16)) / 8) * w1 + (((j * x_ratio) >> 16))] >> (((i * y_ratio) >> 16) % 8)) & 0x1) << ((i) % 8));
			dst[((i) / 8) * w2 + (j + 1)] |= (((src[((((i * y_ratio) >> 16)) / 8) * w1 + ((((j + 1) * x_ratio) >> 16))] >> (((i * y_ratio) >> 16) % 8)) & 0x1) << ((i) % 8));
			dst[((i) / 8) * w2 + (j + 2)] |= (((src[((((i * y_ratio) >> 16)) / 8) * w1 + ((((j + 2) * x_ratio) >> 16))] >> (((i * y_ratio) >> 16) % 8)) & 0x1) << ((i) % 8));
			dst[((i) / 8) * w2 + (j + 3)] |= (((src[((((i * y_ratio) >> 16)) / 8) * w1 + ((((j + 3) * x_ratio) >> 16))] >> (((i * y_ratio) >> 16) % 8)) & 0x1) << ((i) % 8));
		}
	}
}

void bad_main(){
	/*
	 * turtius: This '880' constant is used since the  1ms ticks(1000) was not precise enough.
	 * This also means that delay functions are wrong!
	 * To calculate this constant just simply take the ratios of the durations, so in this instance it was
	 * expected_duration = 216 (s)
	 * actual_duration = 193 (s)
	 * actual_tick = 1000
	 *
	 * +-expected_tick_offset = ((expected_duration * actual_tick)/ actual_duration)
	 *
	 */
	if (SysTick_Config(SystemCoreClock / 880))
	{
		//ERROR
		return;
	}
	lcd_init();
	TPM_Type *base = TPM1_PERIPHERAL;
	uint32_t tpmClock = (TPM1_CLOCK_SOURCE/ (1U << (base->SC & TPM_SC_PS_MASK)));
	uint16_t data;
	uint16_t freq;
	uint32_t sequence = 0;

	uint32_t currentframe = 0;
	uint32_t x_ratio = (uint32_t) ((FRAME_WIDTH << 16) /  LCD_WIDTH);
	uint32_t y_ratio = (uint32_t) ((FRAME_HEIGHT << 16) / LCD_HEIGHT);
	while(1){
		if (buzzerDuration == 0) {

			if (sequence > Melody0_Length) {
				sequence = 0;
			}
			data = Melody0[sequence];
			if ((data & 0xF) == 0xF) {
				//base->CNT = base->MOD -1;
				//base->SC |= TPM_SC_CPWMS_MASK;
				base->MOD = 0;
			} else {
				freq = Freq8[data & 0xF] / (1 << (8 - (data >> 4 & 0xF)));
				//base->SC |= TPM_SC_CPWMS_MASK;
				base->MOD = tpmClock / (freq * 2);
			}
			buzzerDuration = data >> 8;
			sequence++;
		}
		if (screenUpdateTick) {
			if(currentframe > sizeof(multiple_frames_bin)){
				currentframe = 0;
			}
			resizePixels((uint8_t*)&multiple_frames_bin[currentframe],frameBuf,FRAME_HEIGHT,FRAME_WIDTH,LCD_WIDTH,LCD_HEIGHT,x_ratio,y_ratio);
			lcd_send_data(frameBuf);
			memset(frameBuf, 0, sizeof(frameBuf));
			currentframe += (FRAME_WIDTH * FRAME_HEIGHT) /8;
			screenUpdateTick = 0;
		}
	}
}

