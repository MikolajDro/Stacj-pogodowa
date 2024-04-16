/*
 * stm32.h
 *
 *  Created on: Apr 14, 2024
 *      Author: utsit
 */

#ifndef STM32_H_
#define STM32_H_
#include "stdint.h"
#define __vo volatile
/*
 * base adresses of Flash and Sram memory
 */
#define FLASH_BASEADDR							0x08000000U		/*explanation*/
#define SRAM1_BASEADDR							0x20000000U		/*explanation*/
#define SRAM2_BASEADDR							0x10000000U		/*explanation*/
#define ROM_BASSEADDR							0x1FFF0000U		/*explanation*/
#define SRAM 									SRAM1_BASEADDR

/*
* AHBx and APBx BUS Peripheral base addresses
*/
#define PERIPH_BASE								0x40000000U
#define APB1PERIPH_BASE							PERIPH_BASE
#define APB2PERIPH_BASE 						0x40010000U
#define AHB1PERIPH_BASE							0x40020000U
#define AHB2PERIPH_BASE							0x48000000U

/*
 *  base addresses of APB1 bus peripherals
 */
#define TIM2_BASEADDR									(APB1PERIPH_BASE + 0x0000)
#define TIM3_BASEADDR									(APB1PERIPH_BASE + 0x0400)
#define TIM4_BASEADDR									(APB1PERIPH_BASE + 0x0800)
#define TIM5_BASEADDR									(APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASEADDR									(APB1PERIPH_BASE + 0x1000)
#define TIM7_BASEADDR									(APB1PERIPH_BASE + 0x1400)
#define LCD_BASEADDR									(APB1PERIPH_BASE + 0x2400)
#define RTC_BASEADDR									(APB1PERIPH_BASE + 0x2800)
#define WWDG_BASEADDR									(APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASEADDR									(APB1PERIPH_BASE + 0x3000)
#define SPI2_BASEADDR									(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR									(APB1PERIPH_BASE + 0x3C00)
#define SPI3_BASEADDR									(APB1PERIPH_BASE + 0x3C00)
#define USART2_BASEADDR									(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR									(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR									(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR									(APB1PERIPH_BASE + 0x5000)

/*
 *  base addresses of APB2 bus peripherals
 */
#define  SYSCFG_BASEADDR								(APB2PERIPH_BASE)
#define VREFBUF_BASEADDR								(APB2PERIPH_BASE + 0x0030)
#define COMP_BASEADDR									(APB2PERIPH_BASE + 0x0200)
#define EXTI_BASEADDR									(APB2PERIPH_BASE + 0x0400)
#define FIREWALL_BASEADDR								(APB2PERIPH_BASE + 0x1C00)
#define SDMMC1_BASEADDR									(APB2PERIPH_BASE + 0x2800)
#define TIM1_BASEADDR									(APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASEADDR									(APB2PERIPH_BASE + 0x3000)
#define TIM8_BASEADDR									(APB2PERIPH_BASE + 0x3400)
#define USART1_BASEADDR									(APB2PERIPH_BASE + 0x3800)
#define TIM15_BASEADDR									(APB2PERIPH)BASE + 0x4000)
#define TIM16_BASEADDR									(APB2PERIPH_BASE + 0x4400)
#define TIM17_BASEADDR									(APB2PERIPH_BASE + 0x4800)
#define SAI1_BASEADDR									(APB2PERIPH_BASE + 0x5400)
#define SAI2_BASEADDR									(APB2PERIPH_BASE + 0x5800)
#define DFSDM1_BASEADDR									(APB2PERIPH_BASE + 0x6000)
/*
 *  base addresses of AHB1 bus peripherals
 */
#define DMA1_BASEADDR									(AHB1PERIPH_BASE)
#define DMA2_BASEADDR									(AHB1PERIPH_BASE + 0x0400)
#define RCC_BASEADDR									(AHB1PERIPH_BASE + 0x1000)
#define FLASH_REG_BASEADDR								(AHB1PERIPH_BASE + 0x2000)
#define CRC_BASEADDR									(AHB1PERIPH_BASE + 0x3000)
#define TSC_BASEADDR									(AHB1PERIPH_BASE + 0x4000)
#define DMA2D_BASEADDR									(AHB1PERIPH_BASE + 0xB000)

/*
 *  base addresses of AHB2 bus peripherals
 */
#define GPIOA_BASEADDR									(AHB2PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR									(AHB2PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR									(AHB2PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR									(AHB2PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR									(AHB2PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR									(AHB2PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR									(AHB2PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR									(AHB2PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR									(AHB2PERIPH_BASE + 0x2000)
#define OTGFS_BASEADDR									(AHB2PERIPH_BASE + 0x08000000)
#define ADC_BASEADDR									(AHB2PERIPH_BASE + 0x08040000)
#define DCMI_BASEADDR									(AHB2PERIPH_BASE + 0x08050000)
#define AES_BASEADDR									(AHB2PERIPH_BASE + 0x08060000)
#define HASH_BASEADDR									(AHB2PERIPH_BASE + 0x08060400)
#define RNG_BASEADDR									(AHB2PERIPH_BASE + 0x08060800)

/*
 * structure for GPIOx ports
*/
typedef struct {
	uint32_t MODER;				/*GPIO port mode register*/						/*offset 0x00*/
	uint32_t OTYPER; 			/*GPIO output type register*/					/*offset 0x04*/
	uint32_t OSPEEDR;			/*GPIO output speed register*/					/*offset 0x08*/
	uint32_t PUPUDR;			/*GPIO port pull-up/pull-down register*/		/*offset 0x0C*/
	uint32_t IDR;				/*GPIO input data register*/					/*offset 0x10*/
	uint32_t ODR;				/*GPIO output data register*/					/*offset 0x14*/
	uint32_t BSRR;				/*GPIO port bit set/reset */					/*offset 0x18*/
	uint32_t LCKR;				/*GPIO  port configuration lock register*/		/*offset 0x1C*/
	uint32_t AFRL;				/*GPIO alternate function low register*/		/*offset 0x20*/
	uint32_t AFHR;				/*GPIO alternate function high register*/		/*offset 0x24*/
	uint32_t BRR;				/*GPIO port bit reset register*/				/*offset 0x28*/
	uint32_t ASCR;				/*GPIO port analog switch control register*/	/*offset 0x2C*/
}GPIO_Reg_Def_t;


/*
 * Creating pointers for each GPIOx port
*/
GPIO_Reg_Def_t* pGPIOA = (GPIO_Reg_Def_t*)GPIOA_BASEADDR;
GPIO_Reg_Def_t* pGPIOB = (GPIO_Reg_Def_t*)GPIOB_BASEADDR;
GPIO_Reg_Def_t* pGPIOC = (GPIO_Reg_Def_t*)GPIOC_BASEADDR;
GPIO_Reg_Def_t* pGPIOD = (GPIO_Reg_Def_t*)GPIOD_BASEADDR;
GPIO_Reg_Def_t* pGPIOE = (GPIO_Reg_Def_t*)GPIOE_BASEADDR;
GPIO_Reg_Def_t* pGPIOF = (GPIO_Reg_Def_t*)GPIOF_BASEADDR;
GPIO_Reg_Def_t* pGPIOG = (GPIO_Reg_Def_t*)GPIOG_BASEADDR;
GPIO_Reg_Def_t* pGPIOH = (GPIO_Reg_Def_t*)GPIOH_BASEADDR;
GPIO_Reg_Def_t* pGPIOI = (GPIO_Reg_Def_t*)GPIOI_BASEADDR;


/*
 * structure for RCC registers
*/
typedef struct {
 __vo uint32_t RCC_CR; 				/*offset 0x00*/
 __vo uint32_t RCC_ICSCR;			/*offset 0x04*/
 __vo uint32_t RCC_CFGR;			/*offset 0x08*/
 __vo uint32_t RCC_PLLCFGR;			/*offset 0x0C*/
 __vo uint32_t RCC_PLLSAI1CFGR;		/*offset 0x10*/
 __vo uint32_t RCC_PLLSAI2CFGR;		/*offset 0x14*/
 __vo uint32_t RCC_CIER;			/*offset 0x18*/
 __vo uint32_t RCC_CIFR;			/*offset 0x1C*/
 __vo uint32_t RCC_CICR;			/*offset 0x20*/
 uint32_t RESERVED1;
 __vo uint32_t RCC_AHB1RSTR;		/*offset 0x28???*/
 __vo uint32_t RCC_AHB2RSTR;		/*offset 0x2C*/
 __vo uint32_t RCC_AHB3RSTR;		/*offset 0x30???*/
 uint32_t RESERVED2;
 __vo uint32_t RCC_APB1RSTR1; 		/*offset 0x38*/
 __vo uint32_t RCC_APB1RSTR2; 		/*offset 0x3C*/
 __vo uint32_t RCC_APB2RSTR; 		/*offset 0x40*/
 __vo uint32_t RCC_AHB1ENR;			/*offset 0x48*/
 __vo uint32_t RCC_AHB2ENR;			/*offset 0x4C*/
 __vo uint32_t RCC_AHB3ENR;			/*offset 0x50*/
 uint32_t RESERVED3;
 __vo uint32_t RCC_APB1ENR1;		/*offset 0x58???*/
 __vo uint32_t RCC_APB1ENR2;		/*offset 0x5C*/
 __vo uint32_t RCC_APB2ENR;			/*offset 0x60*/
 __vo uint32_t RCC_AHB1SMENR;		/*offset 0x68*/
 __vo uint32_t RCC_AHB2SMENR;		/*offset 0x6C*/
 __vo uint32_t RCC_AHB3SMENR;		/*offset 0x70*/
 uint32_t RESERVED4;
 __vo uint32_t RCC_APB1SMENR1;		/*offset 0x78*/
 __vo uint32_t RCC_APB1SMENR2;		/*offset 0x7C*/
 __vo uint32_t RCC_APB2SMENR;		/*offset 0x80*/
 uint32_t RESERVED5;
 __vo uint32_t RCC_CCIPR;			/*offset 0x88*/
 __vo uint32_t RCC_BDCR;			/*offset 0x90*/
 __vo uint32_t RCC_CSR;				/*offset 0x94*/
 __vo uint32_t RCC_CRRCR;			/*offset 0x98*/
 __vo uint32_t RCC_CRIPR2;			/*offset 0x9C*/


}RCC_RegDef_t;

#define RCC ((RCC_RegDef_t*)RCC_BASE_ADDR)


// some generic macros
#define ENABLE 				1
#define DISABLE 			0
#define SET     			ENABLE
#define RESET				DISABLE
#endif /* STM32_H_ */
