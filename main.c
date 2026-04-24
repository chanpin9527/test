#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/led/led.h"
#include "./BSP/exti/exti.h"
#include "./BSP/TIMER/time.h"
#include "./BSP/key/shan.h"
#include "./BSP/DAC/dac.h"
#include "./BSP/ADC/adc.h"


int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336,8,2,7);
    delay_init(168);
    uart_init(115200);
    led_init();
    exti_init();
    adc_init();
    timx_init(10000 - 1, 8400 - 1);
    shan_init();
    dac_init();
    const uint32_t best_speed = 600;                                                    /*定义最佳速度*/
    uint32_t gap = 0;
    

    while(1)
    {
        /*位置和速度的显示*/
        gap = best_speed - speed_get();                                      /*求得与最佳速度的差值*/
        printf("neolix");
        printf(" 当前的位置是：%u\r\n",location_get() ); 
        printf(" 当前的速度是：%u\r\n",speed_get() );                                    /*显示当前的速度(定时器定时1s中所行驶的单位长度)*/
        printf("当前速度差值是：%d\r\n",gap);
        
        /*基于当前速度确定车轮控制信号的电压*/
        //仅为演示服务，实际上DAC输出的电压值，可以通过收集其上的电流可以得到，这个地方只是为了表示DAC的输出值是正确的；
        HAL_DAC_SetValue(&g_dac1_handle,DAC_CHANNEL_1,DAC_ALIGN_12B_R,gap);             /*把当前速度与最佳速度的差值，输入到DAC转换器中*/
        printf("存到DAC寄存器上的gap值是：%d\r\n",gap);
        gap = HAL_DAC_GetValue(&g_dac1_handle,DAC_CHANNEL_1);                           /*获取DAC转换器的输出值*///感觉并不是输出值，而是获取其寄存器的值
        printf("从DAC寄存器上读取的gap值是：%d\r\n",gap);
        
        printf(" 需要给到车轮的电压值为：%fV\r\n",(float)((3.3/4096)*gap));                /*通过串口把电压值进行输出*/
        

        
        gap = adc_value_avr_get(ADC_ADCX_CHY,10);
        printf(" ADC采集到的数字值为：%d\r\n",gap);
        printf(" ADC采集到的电压值为：%fV\r\n",(float)((3.3/4096)*gap));
        
        
        delay_ms(1000);
    }
}
    




