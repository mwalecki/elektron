#include "clock.h"


//void RCC_Configuration(void)
//{
//  ErrorStatus HSEStartUpStatus;
//  /* RCC system reset(for debug purpose) */
//  RCC_DeInit();
//  /* Enable HSE */
//  RCC_HSEConfig(RCC_HSE_ON);
//  /* Wait till HSE is ready */
//  HSEStartUpStatus = RCC_WaitForHSEStartUp();
//  if(HSEStartUpStatus == SUCCESS)
//  {
//    /* Enable Prefetch Buffer */
//    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//    /* Flash 2 wait state */
//    FLASH_SetLatency(FLASH_Latency_2);
//    /* HCLK = SYSCLK */
//    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
//    /* PCLK2 = HCLK */
//    RCC_PCLK2Config(RCC_HCLK_Div1);
//    /* PCLK1 = HCLK/2 */
//    RCC_PCLK1Config(RCC_HCLK_Div4);
//    /* PLLCLK = 8MHz * 9 = 72 MHz */
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
//    /* Enable PLL */ 
//    RCC_PLLCmd(ENABLE);
//    /* Wait till PLL is ready */
//    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//    {
//    }
//    /* Select PLL as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//    /* Wait till PLL is used as system clock source */
//    while(RCC_GetSYSCLKSource() != 0x08)
//    {
//    }
//  }
//}

void STM32EU_CL_RCC_Configuration(void)
{
	u16 StartUpCounter=0;
	uint32_t HSEStatus;

	/* W≥πcz HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Czekaj, aø HSE sie uruchomi lub zostanie przekroczony dozwolony czas */
	do
	{
		StartUpCounter++;
	} while((RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) &&
						(StartUpCounter != HSEStartUp_TimeOut));

	if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == SET)
	{
		HSEStatus = SUCCESS;
	}
	else
	{
		HSEStatus = ERROR;
	}

	if (HSEStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* 2x "waitstate" */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PREDIV2: PREDIV2CLK = HSE / 4 = 4 MHz */
		RCC_PREDIV2Config(RCC_PREDIV2_Div4 );

		/* PLL3: PLL3CLK = (HSE / 4) * 12 = 48 MHz */
		RCC_PLL3Config(RCC_PLL3Mul_12);
		/* W≥acz PLL3 */
		RCC_PLL3Cmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET);

		/* MCO: MCO_OUT = PLL3 / 2 = 24 MHz*/
		RCC_MCOConfig(RCC_MCO_PLL3CLK_Div2);

		/* PLL2: PLL2CLK = (HSE / 4) * 8 = 32 MHz */
		RCC_PLL2Config(RCC_PLL2Mul_8);
		RCC_PLL2Cmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);

		/* PREDIV1: PREDIV1CLK = PLL2 / 4 = 8 MHz */
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div4);

		/* PLL: PLLCLK = PREDIV1 * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

		/* Wlasz PLL */
		RCC_PLLCmd (ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		/* PLL ürÛdlem SYSCLK */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08);

		/* èrÛd≥o taktowania USB (PLLVCO = 2*PLLCLK) PLLVCO / 3 = 48MHz */
		RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

		/* W≥πczenie taktowania ETHERNET i USB */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
							RCC_AHBPeriph_ETH_MAC_Rx | RCC_AHBPeriph_OTG_FS, ENABLE);
	}

	/* Wlπcznie taktowania GPIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
						   RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
}
