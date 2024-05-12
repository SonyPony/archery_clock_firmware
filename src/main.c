#include "stm8s.h"
#include "stm8s_it.h"
#include <stm8s_gpio.h>
#include <stm8s_tim4.h>
#include <stm8s_itc.h>
#include <stm8s_clk.h>
#include <milis.h>

#define LED_GPIO_PORT (GPIOC)
#define LED_GPIO_PINS GPIO_PIN_5

void Delay(uint16_t nCount);

int main(void)
{
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz from internal RC oscillator
  init_milis();
  enableInterrupts();

  uint8_t i_var = 1;
  uint16_t last_time = 0;
  uint8_t io = 255;
  io++;

  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);

  while (1)
  {

    i_var++;
    if (milis() - last_time > 1000)
    {
      last_time = milis();
      GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    }
    // Delay(0xFFFF);
  }
}

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif