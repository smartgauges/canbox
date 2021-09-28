/**************************************************************************//**
 * @file     NUC131.h
 * @version  V3.0
 * $Revision: 62 $
 * $Date: 15/05/22 9:06a $
 * @brief    NUC131 Series Peripheral Access Layer Header File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/



/**
  \mainpage Introduction
  *
  *
  * This user manual describes the usage of NUC131 Series MCU device driver
  *
  * <b>Disclaimer</b>
  *
  * The Software is furnished "AS IS", without warranty as to performance or results, and
  * the entire risk as to performance or results is assumed by YOU. Nuvoton disclaims all
  * warranties, express, implied or otherwise, with regard to the Software, its use, or
  * operation, including without limitation any and all warranties of merchantability, fitness
  * for a particular purpose, and non-infringement of intellectual property rights.
  *
  * <b>Copyright Notice</b>
  *
  * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
  */


/**
  * \page PG_REV Revision History
  *
  * <b>Revision 3.00.002</b>
  * \li Fix PWM driver bug for output low when duty is 100%
  * \li Fix BPWM driver bug for output low when duty is 100%
  * \li Fix CLK driver bug in CLK_SetCoreClock():
  * \li Fix CLK driver constant definitions error of (B)PWM0/1_MODULE clock source selection.
  * \li Fix GPIO_ENABLE_DOUT_MASK() and GPIO_DISABLE_DOUT_MASK() bug of GPIO driver.
  * \li Fix PWM driver bug of PWM_MASK_OUTPUT() to remove redundant parenthesis.
  * \li Fix BPWM driver bug of BPWM_MASK_OUTPUT() to remove redundant parenthesis.
  * \li Fix UART driver clear flag bug in UART_ClearIntFlag().
  * \li Fix I2C driver module reset bug of I2C_Close().
  * \li Fix clear RS-485 address byte detection flag bug in UART_RS485_CLEAR_ADDR_FLAG() of UART driver.
  * \li Fix SYS_IS_SYSTEM_RST() bug, it is "SYS_RSTSRC_RSTS_SYS_Msk" but "SYS_RSTSRC_RSTS_MCU_Msk".
  * \li Fix clear RS-485 address byte detection flag clear bug in RS485_HANDLE() of UART_RS485_Slave sample code.
  * \li Fix UART RS485 RTS active level to high level active in RS485_9bitModeMaster() of UART RS485 Sample code.
  * \li Fix NVIC_EnableIRQ() to NVIC_DisableIRQ() after CHIP wake-up in I2C_Wakeup_Slave sample code
  * \li Add PWM_EnableLoadMode() and PWM_DisableLoadMode() functions to PWM driver
  * \li Add PWM_SetBrakePinSource() function to PWM driver
  * \li Add CLK_GetPCLKFreq() function to CLK driver
  * \li Add new macro PWM_SET_DEADZONE_CLK_SRC() to PWM driver
  * \li Add new macro SYS_IS_LVR_RST() to SYS driver.
  * \li Add non-blocking printf implementation and use predefine compiler option to enable/disable it.
  *
  * <b>Revision 3.00.001</b>
  * \li Updated to support new API
*/



#ifndef __NUC131_H__
#define __NUC131_H__


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Definitions Definitions for CMSIS
  This file defines all structures and symbols:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
*/

/**
 * @details  Interrupt Number Definition. The maximum of 32 Specific Interrupts are possible.
 */

typedef enum IRQn
{
    /******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
    HardFault_IRQn              = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
    SVCall_IRQn                 = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
    PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
    SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

    /******  ARMIKMCU Swift specific Interrupt Numbers ************************************************/
    BOD_IRQn                  = 0,        /*!< Brown-Out Low Voltage Detected Interrupt             */
    WDT_IRQn                  = 1,        /*!< Watch Dog Timer Interrupt                            */
    EINT0_IRQn                = 2,        /*!< EINT0 Interrupt                                      */
    EINT1_IRQn                = 3,        /*!< EINT1 Interrupt                                      */
    GPAB_IRQn                 = 4,        /*!< GPIO_PA/PB Interrupt                                 */
    GPCDEF_IRQn               = 5,        /*!< GPIO_PC/PD/PE/PF Interrupt                           */
    TMR0_IRQn                 = 8,        /*!< TIMER0 Interrupt                                     */
    TMR1_IRQn                 = 9,        /*!< TIMER1 Interrupt                                     */
    TMR2_IRQn                 = 10,       /*!< TIMER2 Interrupt                                     */
    TMR3_IRQn                 = 11,       /*!< TIMER3 Interrupt                                     */
    UART02_IRQn               = 12,       /*!< UART0/UART2 Interrupt                                */
    UART1_IRQn                = 13,       /*!< UART1 Interrupt                                      */
    SPI0_IRQn                 = 14,       /*!< SPI0 Interrupt                                       */
    UART3_IRQn                = 15,       /*!< UART3 Interrupt                                      */
    UART4_IRQn                = 16,       /*!< UART4 Interrupt                                      */
    UART5_IRQn                = 17,       /*!< UART5 Interrupt                                      */
    I2C0_IRQn                 = 18,       /*!< I2C0 Interrupt                                       */
    I2C1_IRQn                 = 19,       /*!< I2C1 Interrupt                                       */
    CAN0_IRQn                 = 20,       /*!< CAN0 Interrupt                                       */
    CAN1_IRQn                 = 21,       /*!< CAN1 Interrupt                                       */
    PWM0_IRQn                 = 22,       /*!< PWM0 Interrupt                                       */
    PWM1_IRQn                 = 23,       /*!< PWM1 Interrupt                                       */
    BPWM0_IRQn                = 24,       /*!< BPWM0 Interrupt                                      */
    BPWM1_IRQn                = 25,       /*!< BPWM1 Interrupt                                      */
    BRAKE0_IRQn               = 26,       /*!< PWM0 Brake Interrupt                                 */
    BRAKE1_IRQn               = 27,       /*!< PWM1 Brake Interrupt                                 */
    PWRWU_IRQn                = 28,       /*!< Power Down Wake Up Interrupt                         */
    ADC_IRQn                  = 29,       /*!< ADC Interrupt                                        */
    CKD_IRQn                  = 30,       /*!< Clock detection Interrupt                            */
} IRQn_Type;                                            
                                                        
                                                        
/*                                                      
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT           0       /*!< armikcmu does not provide a MPU present or not       */
#define __NVIC_PRIO_BITS        2       /*!< armikcmu Supports 2 Bits for the Priority Levels     */
#define __Vendor_SysTickConfig  0       /*!< Set to 1 if different SysTick Config is used         */


/*@}*/ /* end of group CMSIS */


#include "core_cm0.h"                   /* Cortex-M0 processor and core peripherals               */
#include "system_NUC131.h"               /* NUC131 System                                           */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

/** @addtogroup REGISTER Control Register

  @{

*/


/*---------------------- Analog to Digital Converter -------------------------*/
/**
    @addtogroup ADC Analog to Digital Converter (ADC)
    Memory Mapped Structure for ADC Controller
@{ */
typedef struct
{

    /**
     * ADDR
     * ===================================================================================================
     * Offset: 0x00-0x1C  ADC Data Register 0~7
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RSLT      |A/D Conversion Result
     * |        |          |This field contains conversion result of ADC.
     * |        |          |When DMOF bit (ADCR[31]) set to 0, 12-bit ADC conversion result with unsigned format will be filled in RSLT (ADDRx[11:0], x=0~7) and zero will be filled in RSLT (ADDRx[15:12], x=0~7).
     * |        |          |When DMOF bit (ADCR[31]) set to 1, 12-bit ADC conversion result with 2'complement format will be filled in RSLT(ADDRx[11:0], x=0~7) and signed bits to will be filled in RSLT (ADDRx[15:12], x=0~7).
     * |[16]    |OVERRUN   |Overrun Flag
     * |        |          |0 = Data in RSLT (ADDRx[15:0], x=0~7) is recent conversion result.
     * |        |          |1 = Data in RSLT (ADDRx[15:0], x=0~7) is overwritten.
     * |        |          |If converted data in RSLT has not been read before new conversion result is loaded to this register, OVERRUN is set to 1 and previous conversion result is gone.
     * |        |          |It is cleared by hardware after ADDR register is read.
     * |        |          |This is a read only bit.
     * |[17]    |VALID     |Valid Flag
     * |        |          |0 = Data in RSLT bits (ADDRx[15:0], x=0~7) is not valid.
     * |        |          |1 = Data in RSLT bits (ADDRx[15:0], x=0~7) is valid.
     * |        |          |This bit is set to 1 when corresponding channel analog input conversion is completed and cleared by hardware after ADDR register is read.
     * |        |          |This is a read only bit
     */
    __I  uint32_t ADDR[8];

    /**
     * ADCR
     * ===================================================================================================
     * Offset: 0x20  ADC Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ADEN      |A/D Converter Enable
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |        |          |Before starting A/D conversion function, this bit should be set to 1.
     * |        |          |Clear it to 0 to disable A/D converter analog circuit for saving power consumption.
     * |[1]     |ADIE      |A/D Interrupt Enable
     * |        |          |0 = A/D interrupt function Disabled.
     * |        |          |1 = A/D interrupt function Enabled.
     * |        |          |A/D conversion end interrupt request is generated if ADIE bit (ADCR[1]) is set to 1.
     * |[3:2]   |ADMD      |A/D Converter Operation Mode
     * |        |          |00 = Single conversion.
     * |        |          |01 = Reserved.
     * |        |          |10 = Single-cycle scan.
     * |        |          |11 = Continuous scan.
     * |        |          |When changing the operation mode, software should disable ADST bit (ADCR[11]) firstly.
     * |[5:4]   |TRGS      |Hardware Trigger Source
     * |        |          |00 = A/D conversion is started by external STADC pin.
     * |        |          |11 = A/D conversion is started by PWM Center-aligned trigger.
     * |        |          |Others = Reserved.
     * |        |          |Software should disable TRGEN (ADCR[8]) and ADST (ADCR[11]) before change TRGS.
     * |[7:6]   |TRGCOND   |External Trigger Condition
     * |        |          |These two bits decide external pin STADC trigger event is level or edge.
     * |        |          |The signal must be kept at stable state at least 8 PCLKs for level trigger and 4 PCLKs at high and low state for edge trigger.
     * |        |          |00 = Low level.
     * |        |          |01 = High level.
     * |        |          |10 = Falling edge.
     * |        |          |11 = Rising edge.
     * |[8]     |TRGEN     |Hardware Trigger Enable
     * |        |          |Enable or disable triggering of A/D conversion by hardware (external STADC pin or PWM Center-aligned trigger).
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |        |          |ADC hardware trigger function is only supported in single-cycle scan mode.
     * |        |          |If hardware trigger mode, the ADST bit (ADCR[11]) can be set to 1 by the selected hardware trigger source.
     * |[10]    |DIFFEN    |Differential Input Mode Enable
     * |        |          |0 = Single-end analog input mode.
     * |        |          |1 = Differential analog input mode.
     * |        |          |Differential   input Paired Channel
     * |        |          |Differential input voltage (Vdiff) = Vplus - Vminus, where Vplus
     * |        |          |is the analog input; Vminus is the inverted analog input.
     * |        |          |In differential input mode, only the even number of the two corresponding channels needs to be enabled in ADCHER.
     * |        |          |The conversion result will be placed to the corresponding data register of the enabled channel.
     * |[11]    |ADST      |A/D Conversion Start
     * |        |          |0 = Conversion stops and A/D converter enter idle state.
     * |        |          |1 = Conversion starts.
     * |        |          |ADST bit can be set to 1 from three sources: software, PWM Center-aligned trigger and external pin STADC.
     * |        |          |ADST will be cleared to 0 by hardware automatically at the ends of single mode and single-cycle scan mode.
     * |        |          |In continuous scan mode, A/D conversion is continuously performed until software writes 0 to this bit or chip reset.
     * |[31]    |DMOF      |A/D Differential Input Mode Output Format
     * |        |          |0 = A/D Conversion result will be filled in RSLT at ADDRx registers with unsigned format.
     * |        |          |1 = A/D Conversion result will be filled in RSLT at ADDRx registers with 2'complement format.
     */
    __IO uint32_t ADCR;

    /**
     * ADCHER
     * ===================================================================================================
     * Offset: 0x24  ADC Channel Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |CHEN      |Analog Input Channel Enable
     * |        |          |Set CHEN[7:0] to enable the corresponding analog input channel 7 ~ 0.
     * |        |          |If DIFFEN bit (ADCR[10]) is set to 1, only the even number channels need to be enabled.
     * |        |          |0 = ADC input channel Disabled.
     * |        |          |1 = ADC input channel Enabled.
     * |[9:8]   |PRESEL    |Analog Input Channel 7 Select
     * |        |          |00 = External analog input.
     * |        |          |01 = Internal band-gap voltage.
     * |        |          |10 = Internal temperature sensor.
     * |        |          |11 = Reserved.
     * |        |          |Note:
     * |        |          |When software select the band-gap voltage as the analog input source of ADC channel 7, ADC clock rate needs to be limited to slower than 300 kHz.
     */
    __IO uint32_t ADCHER;

    /**
     * ADCMPR
     * ===================================================================================================
     * Offset: 0x28, 0x2C  ADC Compare Register 0 & 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CMPEN     |Compare Enable
     * |        |          |0 = Compare function Disabled.
     * |        |          |1 = Compare function Enabled.
     * |        |          |Set this bit to 1 to enable ADC controller to compare CMPD (ADCMPR0/1[27:16]) with specified channel conversion result when converted data is loaded into ADDR register.
     * |[1]     |CMPIE     |Compare Interrupt Enable
     * |        |          |0 = Compare function interrupt Disabled.
     * |        |          |1 = Compare function interrupt Enabled.
     * |        |          |If the compare function is enabled and the compare condition matches the setting of CMPCOND (ADCMPR0/1[2]) and CMPMATCNT (ADCMPR0/1[11:8]), CMPF0/1 bit (ADSR[1]/[2]) will be asserted, in the meanwhile, if CMPIE (ADCMPR0/1[1]) is set to 1, a compare interrupt request is generated.
     * |[2]     |CMPCOND   |Compare Condition
     * |        |          |0 = Set the compare condition as that when a 12-bit A/D conversion result is less than the 12-bit CMPD (ADCMPR0/1[27:16]), the internal match counter will increase one.
     * |        |          |1 = Set the compare condition as that when a 12-bit A/D conversion result is greater or equal to the 12-bit CMPD (ADCMPR0/1[27:16]), the internal match counter will increase one.
     * |        |          |Note: When the internal counter reaches the value to (CMPMATCNT (ADCMPR0/1[11:8])+1), the CMPF0/1 bit (ADSR[1]/[2]) will be set.
     * |[5:3]   |CMPCH     |Compare Channel Selection
     * |        |          |000 = Channel 0 conversion result is selected to be compared.
     * |        |          |001 = Channel 1 conversion result is selected to be compared.
     * |        |          |010 = Channel 2 conversion result is selected to be compared.
     * |        |          |011 = Channel 3 conversion result is selected to be compared.
     * |        |          |100 = Channel 4 conversion result is selected to be compared.
     * |        |          |101 = Channel 5 conversion result is selected to be compared.
     * |        |          |110 = Channel 6 conversion result is selected to be compared.
     * |        |          |111 = Channel 7 conversion result is selected to be compared.
     * |[11:8]  |CMPMATCNT |Compare Match Count
     * |        |          |When the specified A/D channel analog conversion result matches the compare condition defined by CMPCOND (ADCMPR0/1[2]), the internal match counter will increase 1.
     * |        |          |When the internal counter reaches the value to (CMPMATCNT (ADCMPR0/1[11:8]) +1), the CMPF0/1 bit (ADSR[1]/[2]) will be set.
     * |[27:16] |CMPD      |Comparison Data
     * |        |          |The 12-bit data is used to compare with conversion result of specified channel.
     * |        |          |When DMOF bit (ADCR[31]) is set to 0, ADC comparator compares CMPD with conversion result with unsigned format.
     * |        |          |CMPD should be filled in unsigned format.
     * |        |          |When DMOF bit (ADCR[31]) is set to 1, ADC comparator compares CMPD with conversion result with 2'complement format.
     * |        |          |CMPD should be filled in 2'complement format.
     */
    __IO uint32_t ADCMPR[2];

    /**
     * ADSR
     * ===================================================================================================
     * Offset: 0x30  ADC Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ADF       |A/D Conversion End Flag
     * |        |          |A status flag that indicates the end of A/D conversion.
     * |        |          |ADF is set to 1 at these two conditions:
     * |        |          |1. When A/D conversion ends in Single mode.
     * |        |          |2. When A/D conversion ends on all specified channels in Scan mode.
     * |        |          |This flag can be cleared by writing 1 to itself.
     * |[1]     |CMPF0     |Compare Flag
     * |        |          |When the selected channel A/D conversion result meets setting condition in ADCMPR0 then this bit is set to 1.
     * |        |          |And it is cleared by writing 1 to self.
     * |        |          |0 = Conversion result in ADDR does not meet ADCMPR0 setting.
     * |        |          |1 = Conversion result in ADDR meets ADCMPR0 setting.
     * |[2]     |CMPF1     |Compare Flag
     * |        |          |When the selected channel A/D conversion result meets setting condition in ADCMPR1 then this bit is set to 1.
     * |        |          |And it is cleared by writing 1 to self.
     * |        |          |0 = Conversion result in ADDR does not meet ADCMPR1 setting.
     * |        |          |1 = Conversion result in ADDR meets ADCMPR1 setting.
     * |[3]     |BUSY      |BUSY/IDLE
     * |        |          |0 = A/D converter is in idle state.
     * |        |          |1 = A/D converter is busy at conversion.
     * |        |          |This bit is mirror of as ADST bit (ADCR[11]).
     * |        |          |It is read only.
     * |[6:4]   |CHANNEL   |Current Conversion Channel
     * |        |          |This field reflects the current conversion channel when BUSY = 1 (ADSR[3]).
     * |        |          |When BUSY = 0, it shows the number of the next converted channel.
     * |        |          |It is read only.
     * |[15:8]  |VALID     |Data Valid Flag
     * |        |          |It is a mirror of VALID bit (ADDR0~7[17]).
     * |        |          |It is read only.
     * |[23:16] |OVERRUN   |Overrun Flag
     * |        |          |It is a mirror to OVERRUN bit (ADDR0~7[16]).
     * |        |          |It is read only.
     */
    __IO uint32_t ADSR;

} ADC_T;

/**
    @addtogroup ADC_CONST ADC Bit Field Definition
    Constant Definitions for ADC Controller
@{ */


/* ADDR Bit Field Definitions */
#define ADC_ADDR_VALID_Pos      17                                /*!< ADC_T::ADDR: VALID Position */
#define ADC_ADDR_VALID_Msk      (1ul << ADC_ADDR_VALID_Pos)       /*!< ADC_T::ADDR: VALID Mask */

#define ADC_ADDR_OVERRUN_Pos    16                                /*!< ADC_T::ADDR: OVERRUN Position */
#define ADC_ADDR_OVERRUN_Msk    (1ul << ADC_ADDR_OVERRUN_Pos)     /*!< ADC_T::ADDR: OVERRUN Mask */

#define ADC_ADDR_RSLT_Pos       0                                 /*!< ADC_T::ADDR: RSLT Position */
#define ADC_ADDR_RSLT_Msk       (0xFFFFul << ADC_ADDR_RSLT_Pos)   /*!< ADC_T::ADDR: RSLT Mask */

/* ADCR Bit Field Definitions */
#define ADC_ADCR_DMOF_Pos       31                                /*!< ADC_T::ADCR: DMOF Position */
#define ADC_ADCR_DMOF_Msk       (1ul << ADC_ADCR_DMOF_Pos)        /*!< ADC_T::ADCR: DMOF Mask */

#define ADC_ADCR_ADST_Pos       11                                /*!< ADC_T::ADCR: ADST Position */
#define ADC_ADCR_ADST_Msk       (1ul << ADC_ADCR_ADST_Pos)        /*!< ADC_T::ADCR: ADST Mask */

#define ADC_ADCR_DIFFEN_Pos     10                                /*!< ADC_T::ADCR: DIFFEN Position */
#define ADC_ADCR_DIFFEN_Msk     (1ul << ADC_ADCR_DIFFEN_Pos)      /*!< ADC_T::ADCR: DIFFEN Mask */

#define ADC_ADCR_TRGEN_Pos      8                                 /*!< ADC_T::ADCR: TRGEN Position */
#define ADC_ADCR_TRGEN_Msk      (1ul << ADC_ADCR_TRGEN_Pos)       /*!< ADC_T::ADCR: TRGEN Mask */

#define ADC_ADCR_TRGCOND_Pos    6                                 /*!< ADC_T::ADCR: TRGCOND Position */
#define ADC_ADCR_TRGCOND_Msk    (3ul << ADC_ADCR_TRGCOND_Pos)     /*!< ADC_T::ADCR: TRGCOND Mask */

#define ADC_ADCR_TRGS_Pos       4                                 /*!< ADC_T::ADCR: TRGS Position */
#define ADC_ADCR_TRGS_Msk       (3ul << ADC_ADCR_TRGS_Pos)        /*!< ADC_T::ADCR: TRGS Mask */

#define ADC_ADCR_ADMD_Pos       2                                 /*!< ADC_T::ADCR: ADMD Position */
#define ADC_ADCR_ADMD_Msk       (3ul << ADC_ADCR_ADMD_Pos)        /*!< ADC_T::ADCR: ADMD Mask */

#define ADC_ADCR_ADIE_Pos       1                                 /*!< ADC_T::ADCR: ADIE Position */
#define ADC_ADCR_ADIE_Msk       (1ul << ADC_ADCR_ADIE_Pos)        /*!< ADC_T::ADCR: ADIE Mask */

#define ADC_ADCR_ADEN_Pos       0                                 /*!< ADC_T::ADCR: ADEN Position */
#define ADC_ADCR_ADEN_Msk       (1ul << ADC_ADCR_ADEN_Pos)        /*!< ADC_T::ADCR: ADEN Mask */

/* ADCHER Bit Field Definitions */
#define ADC_ADCHER_PRESEL_Pos   8                                 /*!< ADC_T::ADCHER: PRESEL Position */
#define ADC_ADCHER_PRESEL_Msk   (3ul << ADC_ADCHER_PRESEL_Pos)    /*!< ADC_T::ADCHER: PRESEL Mask */

#define ADC_ADCHER_CHEN_Pos     0                                 /*!< ADC_T::ADCHER: CHEN Position */
#define ADC_ADCHER_CHEN_Msk     (0xFFul << ADC_ADCHER_CHEN_Pos)   /*!< ADC_T::ADCHER: CHEN Mask */

/* ADCMPR Bit Field Definitions */
#define ADC_ADCMPR_CMPD_Pos        16                                    /*!< ADC_T::ADCMPR: CMPD Position */
#define ADC_ADCMPR_CMPD_Msk        (0xFFFul << ADC_ADCMPR_CMPD_Pos)      /*!< ADC_T::ADCMPR: CMPD Mask */

#define ADC_ADCMPR_CMPMATCNT_Pos   8                                     /*!< ADC_T::ADCMPR: CMPMATCNT Position */
#define ADC_ADCMPR_CMPMATCNT_Msk   (0xFul << ADC_ADCMPR_CMPMATCNT_Pos)   /*!< ADC_T::ADCMPR: CMPMATCNT Mask */

#define ADC_ADCMPR_CMPCH_Pos       3                                     /*!< ADC_T::ADCMPR: CMPCH Position */
#define ADC_ADCMPR_CMPCH_Msk       (7ul << ADC_ADCMPR_CMPCH_Pos)         /*!< ADC_T::ADCMPR: CMPCH Mask */

#define ADC_ADCMPR_CMPCOND_Pos     2                                     /*!< ADC_T::ADCMPR: CMPCOND Position */
#define ADC_ADCMPR_CMPCOND_Msk     (1ul << ADC_ADCMPR_CMPCOND_Pos)       /*!< ADC_T::ADCMPR: CMPCOND Mask */

#define ADC_ADCMPR_CMPIE_Pos       1                                     /*!< ADC_T::ADCMPR: CMPIE Position */
#define ADC_ADCMPR_CMPIE_Msk       (1ul << ADC_ADCMPR_CMPIE_Pos)         /*!< ADC_T::ADCMPR: CMPIE Mask */

#define ADC_ADCMPR_CMPEN_Pos       0                                     /*!< ADC_T::ADCMPR: CMPEN Position */
#define ADC_ADCMPR_CMPEN_Msk       (1ul << ADC_ADCMPR_CMPEN_Pos)         /*!< ADC_T::ADCMPR: CMPEN Mask */

/* ADSR Bit Field Definitions */
#define ADC_ADSR_OVERRUN_Pos       16                                    /*!< ADC_T::ADSR: OVERRUN Position */
#define ADC_ADSR_OVERRUN_Msk       (0xFFul << ADC_ADSR_OVERRUN_Pos)      /*!< ADC_T::ADSR: OVERRUN Mask */

#define ADC_ADSR_VALID_Pos         8                                     /*!< ADC_T::ADSR: VALID Position */
#define ADC_ADSR_VALID_Msk         (0xFFul << ADC_ADSR_VALID_Pos)        /*!< ADC_T::ADSR: VALID Mask */

#define ADC_ADSR_CHANNEL_Pos       4                                     /*!< ADC_T::ADSR: CHANNEL Position */
#define ADC_ADSR_CHANNEL_Msk       (7ul << ADC_ADSR_CHANNEL_Pos)         /*!< ADC_T::ADSR: CHANNEL Mask */

#define ADC_ADSR_BUSY_Pos          3                                     /*!< ADC_T::ADSR: BUSY Position */
#define ADC_ADSR_BUSY_Msk          (1ul << ADC_ADSR_BUSY_Pos)            /*!< ADC_T::ADSR: BUSY Mask */

#define ADC_ADSR_CMPF1_Pos         2                                     /*!< ADC_T::ADSR: CMPF1 Position */
#define ADC_ADSR_CMPF1_Msk         (1ul << ADC_ADSR_CMPF1_Pos)           /*!< ADC_T::ADSR: CMPF1 Mask */

#define ADC_ADSR_CMPF0_Pos         1                                     /*!< ADC_T::ADSR: CMPF0 Position */
#define ADC_ADSR_CMPF0_Msk         (1ul << ADC_ADSR_CMPF0_Pos)           /*!< ADC_T::ADSR: CMPF0 Mask */

#define ADC_ADSR_ADF_Pos           0                                     /*!< ADC_T::ADSR: ADF Position */
#define ADC_ADSR_ADF_Msk           (1ul << ADC_ADSR_ADF_Pos)             /*!< ADC_T::ADSR: ADF Mask */

/*@}*/ /* end of group ADC_CONST */
/*@}*/ /* end of group ADC */



/*---------------------- Basic Pulse Width Modulation Controller -------------------------*/
/**
    @addtogroup BPWM Basic Pulse Width Modulation Controller (BPWM)
    Memory Mapped Structure for BPWM Controller
@{ */
typedef struct
{
    /**
     * BPWM_CTL0
     * ===================================================================================================
     * Offset: 0x00  BPWM Control Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CTRLDn    |Center Re-Load
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |In up-down counter type, PERIOD will load to PBUF at the end point of each period. CMPDAT
     * |        |          |will load to CMPBUF at the center point of a period.
     * |[21:16] |IMMLDENn  |Immediately Load Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = PERIOD will load to PBUF at the end point of each period. CMPDAT will load to CMPBUF
     * |        |          |at the end point or center point of each period by setting CTRLD bit.
     * |        |          |1 = PERIOD/CMPDAT will load to PBUF and CMPBUF immediately when software update PERIOD/CMPDAT.
     * |        |          |Note: If IMMLDENn is enabled, WINLDENn and CTRLDn will be invalid.
     * |[30]    |DBGHALT   |ICE Debug Mode Counter Halt (Write Protect)
     * |        |          |If counter halt is enabled, BPWM all counters will keep current value until exit
     * |        |          |ICE debug mode.
     * |        |          |0 = ICE debug mode counter halt disable.
     * |        |          |1 = ICE debug mode counter halt enable.
     * |        |          |Note: This register is write protected. Refer toSYS_REGLCTL register.
     * |[31]    |DBGTRIOFF |ICE Debug Mode Acknowledge Disable (Write Protect)
     * |        |          |0 = ICE debug mode acknowledgment effects BPWM output.
     * |        |          |BPWM pin will be forced as tri-state while ICE debug mode acknowledged.
     * |        |          |1 = ICE debug mode acknowledgment disabled.
     * |        |          |BPWM pin will keep output no matter ICE debug mode acknowledged or not.
     * |        |          |Note: This register is write protected. Refer toSYS_REGLCTL register.
     */
    __IO uint32_t CTL0;

    /**
     * BPWM_CTL1
     * ===================================================================================================
     * Offset: 0x04  BPWM Control Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |CNTTYPE0  |BPWM Counter Behavior Type 0
     * |        |          |Each bit n controls corresponding BPWM channel n.
     * |        |          |00 = Up counter type (supports in capture mode).
     * |        |          |01 = Down count type (supports in capture mode).
     * |        |          |10 = Up-down counter type.
     * |        |          |11 = Reserved.
     */
    __IO uint32_t CTL1;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED0
     * ===================================================================================================
     * Offset: 0x0008 ~ 0x000C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED0[2];
    /**
     * @endcond
     */

    /**
     * BPWM_CLKSRC
     * ===================================================================================================
     * Offset: 0x10  BPWM Clock Source Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2:0]   |ECLKSRC0  |BPWM_CH01 External Clock Source Select
     * |        |          |000 = BPWMx_CLK, x denotes 0 or 1.
     * |        |          |001 = TIMER0 overflow.
     * |        |          |010 = TIMER1 overflow.
     * |        |          |011 = TIMER2 overflow.
     * |        |          |100 = TIMER3 overflow.
     * |        |          |Others = Reserved.
     */
    __IO uint32_t CLKSRC;

    /**
     * BPWM_CLKPSC
     * ===================================================================================================
     * Offset: 0x14  BPWM Clock Pre-scale Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |CLKPSC    |BPWM Counter Clock Pre-Scale
     * |        |          |The clock of BPWM counter is decided by clock prescaler. Each BPWM pair share one BPWM
     * |        |          |counter clock prescaler. The clock of BPWM counter is divided by (CLKPSC+ 1).
     */
    __IO uint32_t CLKPSC;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED1
     * ===================================================================================================
     * Offset: 0x0018 ~ 0x001C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED1[2];
    /**
     * @endcond
     */

    /**
     * BPWM_CNTEN
     * ===================================================================================================
     * Offset: 0x20  BPWM Counter Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTEN0    |BPWM Counter Enable 0
     * |        |          |0 = BPWM Counter and clock prescaler Stop Running.
     * |        |          |1 = BPWM Counter and clock prescaler Start Running.
     */
    __IO uint32_t CNTEN;

    /**
     * BPWM_CNTCLR
     * ===================================================================================================
     * Offset: 0x24  BPWM Clear Counter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTCLR0   |Clear BPWM Counter Control Bit 0
     * |        |          |It is automatically cleared by hardware.
     * |        |          |0 = No effect.
     * |        |          |1 = Clear 16-bit BPWM counter to 0000H.
     */
    __IO uint32_t CNTCLR;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED2
     * ===================================================================================================
     * Offset: 0x0028 ~ 0x002C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED2[2];
    /**
     * @endcond
     */

    /**
     * BPWM_PERIOD
     * ===================================================================================================
     * Offset: 0x30  BPWM Period Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |PERIOD    |BPWM Period Register
     * |        |          |Up-Count mode: In this mode, BPWM counter counts from 0 to PERIOD, and restarts from 0.
     * |        |          |Down-Count mode: In this mode, BPWM counter counts from PERIOD to 0, and restarts from PERIOD.
     * |        |          |BPWM period time = (PERIOD+1) * BPWM_CLK period.
     * |        |          |Up-Down-Count mode: In this mode, BPWM counter counts from 0 to PERIOD, then decrements to 0
     * |        |          |and repeats again.
     * |        |          |BPWM period time = 2 * PERIOD * BPWM_CLK period.
     */
    __IO uint32_t PERIOD;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED3
     * ===================================================================================================
     * Offset: 0x0034 ~ 0x004C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED3[7];
    /**
     * @endcond
     */

    /**
     * BPWM_CMPDAT0~5
     * ===================================================================================================
     * Offset: 0x50~0x64  BPWM Comparator Register 0~5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |CMPDAT    |BPWM Comparator Register
     * |        |          |CMPDAT use to compare with CNT to generate BPWM waveform, interrupt and trigger ADC.
     * |        |          |In independent mode, CMPDAT0~5 denote as 6 independent BPWM_CH0~5 compared point.
     * |        |          |In complementary mode, CMPDAT0, 2, 4 denote as first compared point, and CMPDAT1,
     * |        |          |3, 5 denote as second compared point for the corresponding 3 complementary
     * |        |          |pairs BPWM_CH0 and BPWM_CH1, BPWM_CH2 and BPWM_CH3, BPWM_CH4 and BPWM_CH5.
     */
    __IO uint32_t CMPDAT[6];

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED4
     * ===================================================================================================
     * Offset: 0x0068 ~ 0x008C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED4[10];
    /**
     * @endcond
     */

    /**
     * BPWM_CNT
     * ===================================================================================================
     * Offset: 0x90  BPWM Counter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |CNT       |BPWM Data Register (Read Only)
     * |        |          |User can monitor CNT to know the current value in 16-bit period counter.
     * |[16]    |DIRF      |BPWM Direction Indicator Flag (Read Only)
     * |        |          |0 = Counter is Down count.
     * |        |          |1 = Counter is UP count.
     */
    __IO uint32_t CNT;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED5
     * ===================================================================================================
     * Offset: 0x0094 ~ 0x00AC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED5[7];
    /**
     * @endcond
     */

    /**
     * BPWM_WGCTL0
     * ===================================================================================================
     * Offset: 0xB0  BPWM Generation Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |ZPCTLn    |BPWM Zero Point Control
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = BPWM zero point output Low.
     * |        |          |10 = BPWM zero point output High.
     * |        |          |11 = BPWM zero point output Toggle.
     * |        |          |BPWM can control output level when BPWM counter count to zero.
     * |[27:16] |PRDPCTLn  |BPWM Period (Center) Point Control
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = BPWM period (center) point output Low.
     * |        |          |10 = BPWM period (center) point output High.
     * |        |          |11 = BPWM period (center) point output Toggle.
     * |        |          |BPWM can control output level when BPWM counter count to (PERIODn+1).
     * |        |          |Note: This bit is center point control when BPWM counter operating in up-down counter type.
     */
    __IO uint32_t WGCTL0;


    /**
     * BPWM_WGCTL1
     * ===================================================================================================
     * Offset: 0xB4  BPWM Generation Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |CMPUCTLn  |BPWM Compare Up Point Control
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = BPWM compare up point output Low.
     * |        |          |10 = BPWM compare up point output High.
     * |        |          |11 = BPWM compare up point output Toggle.
     * |        |          |BPWM can control output level when BPWM counter up count to CMPDAT.
     * |        |          |Note: In complementary mode, CMPUCTL1, 3, 5 use as another CMPUCTL for channel 0, 2, 4.
     * |[27:16] |CMPDCTLn  |BPWM Compare Down Point Control
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = BPWM compare down point output Low.
     * |        |          |10 = BPWM compare down point output High.
     * |        |          |11 = BPWM compare down point output Toggle.
     * |        |          |BPWM can control output level when BPWM counter down count to CMPDAT.
     * |        |          |Note: In complementary mode, CMPDCTL1, 3, 5 use as another CMPDCTL for channel 0, 2, 4.
     */
    __IO uint32_t WGCTL1;

    /**
     * BPWM_MSKEN
     * ===================================================================================================
     * Offset: 0xB8  BPWM Mask Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |MSKENn    |BPWM Mask Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |The BPWM output signal will be masked when this bit is enabled. The corresponding BPWM
     * |        |          |channel n will output MSKDATn (BPWM_MSK[5:0]) data.
     * |        |          |0 = BPWM output signal is non-masked.
     * |        |          |1 = BPWM output signal is masked and output MSKDATn data.
     */
    __IO uint32_t MSKEN;

    /**
     * BPWM_MSK
     * ===================================================================================================
     * Offset: 0xBC  BPWM Mask Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |MSKDATn   |BPWM Mask Data Bit
     * |        |          |This data bit control the state of BPWMn output pin, if corresponding mask function
     * |        |          |is enabled. Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Output logic low to BPWMn.
     * |        |          |1 = Output logic high to BPWMn.
     */
    __IO uint32_t MSK;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED6
     * ===================================================================================================
     * Offset: 0x00C0 ~ 0x00D0
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED6[5];
    /**
     * @endcond
     */

    /**
     * BPWM_POLCTL
     * ===================================================================================================
     * Offset: 0xD4  BPWM Pin Polar Inverse Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |PINVn     |BPWM PIN Polar Inverse Control
     * |        |          |The register controls polarity state of BPWM output. Each bit n controls the
     * |        |          |corresponding BPWM channel n.
     * |        |          |0 = BPWM output polar inverse Disabled.
     * |        |          |1 = BPWM output polar inverse Enabled.
     */
    __IO uint32_t POLCTL;

    /**
     * BPWM_POEN
     * ===================================================================================================
     * Offset: 0xD8  BPWM Output Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |POENn     |BPWM Pin Output Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = BPWM pin at tri-state.
     * |        |          |1 = BPWM pin in output mode.
     */
    __IO uint32_t POEN;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED7
     * ===================================================================================================
     * Offset: 0x00DC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED7[1];
    /**
     * @endcond
     */

    /**
     * BPWM_INTEN
     * ===================================================================================================
     * Offset: 0xE0  BPWM Interrupt Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ZIEN0     |BPWM Zero Point Interrupt Enable 0
     * |        |          |0 = Zero point interrupt Disabled.
     * |        |          |1 = Zero point interrupt Enabled.
     * |        |          |Note: Odd channels will read always 0 at complementary mode.
     * |[8]     |PIEN0     |BPWM Period Point Interrupt Enable 0
     * |        |          |0 = Period point interrupt Disabled.
     * |        |          |1 = Period point interrupt Enabled.
     * |        |          |Note: When up-down counter type period point means center point.
     * |[21:16] |CMPUIENn  |BPWM Compare Up Count Interrupt Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Compare up count interrupt Disabled.
     * |        |          |1 = Compare up count interrupt Enabled.
     * |        |          |Note: In complementary mode, CMPUIEN1, 3, 5 use as another CMPUIEN for channel 0, 2, 4.
     * |[29:24] |CMPDIENn  |BPWM Compare Down Count Interrupt Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Compare down count interrupt Disabled.
     * |        |          |1 = Compare down count interrupt Enabled.
     * |        |          |Note: In complementary mode, CMPDIEN1, 3, 5 use as another CMPDIEN for channel 0, 2, 4.
     */
    __IO uint32_t INTEN;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED8
     * ===================================================================================================
     * Offset: 0x00E4
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED8[1];
    /**
     * @endcond
     */

    /**
     * BPWM_INTSTS
     * ===================================================================================================
     * Offset: 0xE8  BPWM Interrupt Flag Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ZIF0      |BPWM Zero Point Interrupt Flag 0
     * |        |          |This bit is set by hardware when BPWM_CH0 counter reaches zero, software can write 1
     * |        |          |to clear this bit to zero.
     * |[8]     |PIF0      |BPWM Period Point Interrupt Flag 0
     * |        |          |This bit is set by hardware when BPWM_CH0 counter reaches BPWM_PERIOD0, software can
     * |        |          |write 1 to clear this bit to zero.
     * |[21:16] |CMPUIFn   |BPWM Compare Up Count Interrupt Flag
     * |        |          |Flag is set by hardware when BPWM counter up count and reaches BPWM_CMPDATn,
     * |        |          |software can clear this bit by writing 1 to it. Each bit n controls the
     * |        |          |corresponding BPWM channel n.
     * |        |          |Note1: If CMPDAT equal to PERIOD, this flag is not working in up counter type selection.
     * |        |          |Note2: In complementary mode, CMPUIF1, 3, 5 use as another CMPUIF for channel 0, 2, 4.
     * |[29:24] |CMPDIFn   |BPWM Compare Down Count Interrupt Flag
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |Flag is set by hardware when BPWM counter down count and reaches BPWM_CMPDATn,
     * |        |          |software can clear this bit by writing 1 to it.
     * |        |          |Note1: If CMPDAT equal to PERIOD, this flag is not working in down counter type selection.
     * |        |          |Note2: In complementary mode, CMPDIF1, 3, 5 use as another CMPDIF for channel 0, 2, 4.
     */
    __IO uint32_t INTSTS;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED9
     * ===================================================================================================
     * Offset: 0x00EC ~ 0x00F4
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED9[3];
    /**
     * @endcond
     */

    /**
     * BPWM_ADCTS0
     * ===================================================================================================
     * Offset: 0xF8  BPWM Trigger ADC Source Select Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |TRGSEL0   |BPWM_CH0 Trigger ADC Source Select
     * |        |          |0000 = BPWM_CH0 zero point.
     * |        |          |0001 = BPWM_CH0 period point.
     * |        |          |0010 = BPWM_CH0 zero or period point.
     * |        |          |0011 = BPWM_CH0 up-count CMPDAT point.
     * |        |          |0100 = BPWM_CH0 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = BPWM_CH1 up-count CMPDAT point.
     * |        |          |1001 = BPWM_CH1 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[7]     |TRGEN0    |BPWM_CH0 Trigger ADC Enable Bit
     * |[11:8]  |TRGSEL1   |BPWM_CH1 Trigger ADC Source Select
     * |        |          |0000 = BPWM_CH0 zero point.
     * |        |          |0001 = BPWM_CH0 period point.
     * |        |          |0010 = BPWM_CH0 zero or period point.
     * |        |          |0011 = BPWM_CH0 up-count CMPDAT point.
     * |        |          |0100 = BPWM_CH0 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = BPWM_CH1 up-count CMPDAT point.
     * |        |          |1001 = BPWM_CH1 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[15]    |TRGEN1    |BPWM_CH1 Trigger ADC Enable Bit
     * |[19:16] |TRGSEL2   |BPWM_CH2 Trigger ADC Source Select
     * |        |          |0000 = BPWM_CH2 zero point.
     * |        |          |0001 = BPWM_CH2 period point.
     * |        |          |0010 = BPWM_CH2 zero or period point.
     * |        |          |0011 = BPWM_CH2 up-count CMPDAT point.
     * |        |          |0100 = BPWM_CH2 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = BPWM_CH3 up-count CMPDAT point.
     * |        |          |1001 = BPWM_CH3 down-count CMPDAT point.
     * |        |          |Others = Reserved.
     * |[23]    |TRGEN2    |BPWM_CH2 Trigger ADC Enable Bit
     * |[27:24] |TRGSEL3   |BPWM_CH3 Trigger ADC Source Select
     * |        |          |0000 = BPWM_CH2 zero point.
     * |        |          |0001 = BPWM_CH2 period point.
     * |        |          |0010 = BPWM_CH2 zero or period point.
     * |        |          |0011 = BPWM_CH2 up-count CMPDAT point.
     * |        |          |0100 = BPWM_CH2 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = BPWM_CH3 up-count CMPDAT point.
     * |        |          |1001 = BPWM_CH3 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[31]    |TRGEN3    |BPWM_CH3 Trigger ADC Enable Bit
     */
    __IO uint32_t ADCTS0;

    /**
     * BPWM_ADCTS1
     * ===================================================================================================
     * Offset: 0xFC  BPWM Trigger ADC Source Select Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |TRGSEL4   |BPWM_CH4 Trigger ADC Source Select
     * |        |          |0000 = BPWM_CH4 zero point.
     * |        |          |0001 = BPWM_CH4 period point.
     * |        |          |0010 = BPWM_CH4 zero or period point.
     * |        |          |0011 = BPWM_CH4 up-count CMPDAT point.
     * |        |          |0100 = BPWM_CH4 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = BPWM_CH5 up-count CMPDAT point.
     * |        |          |1001 = BPWM_CH5 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[7]     |TRGEN4    |BPWM_CH4 Trigger ADC Enable Bit
     * |[11:8]  |TRGSEL5   |BPWM_CH5 Trigger ADC Source Select
     * |        |          |0000 = BPWM_CH4 zero point.
     * |        |          |0001 = BPWM_CH4 period point.
     * |        |          |0010 = BPWM_CH4 zero or period point.
     * |        |          |0011 = BPWM_CH4 up-count CMPDAT point.
     * |        |          |0100 = BPWM_CH4 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = BPWM_CH5 up-count CMPDAT point.
     * |        |          |1001 = BPWM_CH5 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[15]    |TRGEN5    |BPWM_CH5 Trigger ADC Enable Bit
     */
    __IO uint32_t ADCTS1;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED10
     * ===================================================================================================
     * Offset: 0x0100 ~ 0x010C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED10[4];
    /**
     * @endcond
     */

    /**
     * BPWM_SSCTL
     * ===================================================================================================
     * Offset: 0x110  BPWM Synchronous Start Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |SSEN0     |BPWM Synchronous Start Function Enable 0
     * |        |          |When synchronous start function is enabled, the BPWM_CH0 counter enable bit (CNTEN0)
     * |        |          |can be enabled by writing BPWM synchronous start trigger bit (CNTSEN).
     * |        |          |0 = BPWM synchronous start function Disabled.
     * |        |          |1 = BPWM synchronous start function Enabled.
     * |[9:8]   |SSRC      |BPWM Synchronous Start Source Select
     * |        |          |00 = Synchronous start source come from BPWM0.
     * |        |          |01 = Synchronous start source come from BPWM1.
     * |        |          |10 = Synchronous start source come from BPWM0.
     * |        |          |11 = Synchronous start source come from BPWM1.
     */
    __IO uint32_t SSCTL;

    /**
     * BPWM_SSTRG
     * ===================================================================================================
     * Offset: 0x114  BPWM Synchronous Start Trigger Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTSEN    |BPWM Counter Synchronous Start Enable (Write Only)
     * |        |          |PMW counter synchronous enable function is used to make selected BPWM channels
     * |        |          |(include BPWM0_CHx and BPWM1_CHx) start counting at the same time.
     * |        |          |Writing this bit to 1 will also set the counter enable bit (CNTENn, n denotes channel 0
     * |        |          |to 5) if correlated BPWM channel counter synchronous start function is enabled.
     * |        |          |Note: This bit only present in BPWM0_BA.
     */
    __IO uint32_t SSTRG;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED11
     * ===================================================================================================
     * Offset: 0x0118 ~ 0x011C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED11[2];
    /**
     * @endcond
     */

    /**
     * BPWM_STATUS
     * ===================================================================================================
     * Offset: 0x120  BPWM Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTMAX0   |Time-Base Counter 0 Equal To 0xFFFF Latched Status
     * |        |          |0 = Indicates the time-base counter never reached its maximum value 0xFFFF.
     * |        |          |1 = Indicates the time-base counter reached its maximum value, software can write 1 to clear
     * |        |          |this bit.
     * |[21:16] |ADCTRGn   |ADC Start Of Conversion Status
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Indicates no ADC start of conversion trigger event has occurred.
     * |        |          |1 = Indicates an ADC start of conversion trigger event has occurred, software can
     * |        |          |write 1 to clear this bit.
     */
    __IO uint32_t STATUS;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED12
     * ===================================================================================================
     * Offset: 0x0124 ~ 0x01FC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED12[55];
    /**
     * @endcond
     */

    /**
     * BPWM_CAPINEN
     * ===================================================================================================
     * Offset: 0x200  BPWM Capture Input Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPINENn  |Capture Input Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = BPWM Channel capture input path Disabled. The input of BPWM channel capture function
     * |        |          |is always regarded as 0.
     * |        |          |1 = BPWM Channel capture input path Enabled. The input of BPWM channel capture function
     * |        |          |comes from correlative multifunction pin.
     */
    __IO uint32_t CAPINEN;

    /**
     * BPWM_CAPCTL
     * ===================================================================================================
     * Offset: 0x204  BPWM Capture Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPENn    |Capture Function Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Capture function Disabled. RCAPDAT/FCAPDAT register will not be updated.
     * |        |          |1 = Capture function Enabled. Capture latched the BPWM counter value when detected
     * |        |          |rising or falling edge of input signal and saved to RCAPDAT (Rising latch) and FCAPDAT
     * |        |          |(Falling latch).
     * |[13:8]  |CAPINVn   |Capture Inverter Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Capture source inverter Disabled.
     * |        |          |1 = Capture source inverter Enabled. Reverse the input signal from GPIO.
     * |[21:16] |RCRLDENn  |Rising Capture Reload Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Rising capture reload counter Disabled.
     * |        |          |1 = Rising capture reload counter Enabled.
     * |[29:24] |FCRLDENn  |Falling Capture Reload Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Falling capture reload counter Disabled.
     * |        |          |1 = Falling capture reload counter Enabled.
     */
    __IO uint32_t CAPCTL;

    /**
     * BPWM_CAPSTS
     * ===================================================================================================
     * Offset: 0x208  BPWM Capture Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CRIFOVn   |Capture Rising Interrupt Flag Overrun Status (Read Only)
     * |        |          |This flag indicates if rising latch happened when the corresponding CAPRIF is 1.
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |Note: This bit will be cleared automatically when user clear corresponding CAPRIF.
     * |[13:8]  |CFIFOVn   |Capture Falling Interrupt Flag Overrun Status (Read Only)
     * |        |          |This flag indicates if falling latch happened when the corresponding CAPFIF is 1.
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |Note: This bit will be cleared automatically when user clear corresponding CAPFIF.
     */
    __IO uint32_t CAPSTS;

    /**
     * BPWM_RCAPDAT0
     * ===================================================================================================
     * Offset: 0x20C  BPWM Rising Capture Data Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |BPWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT0;

    /**
     * BPWM_FCAPDAT0
     * ===================================================================================================
     * Offset: 0x210  BPWM Falling Capture Data Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |BPWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT0;

    /**
     * BPWM_RCAPDAT1
     * ===================================================================================================
     * Offset: 0x214  BPWM Rising Capture Data Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |BPWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT1;

    /**
     * BPWM_FCAPDAT1
     * ===================================================================================================
     * Offset: 0x218  BPWM Falling Capture Data Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |BPWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT1;

    /**
     * BPWM_RCAPDAT2
     * ===================================================================================================
     * Offset: 0x21C  BPWM Rising Capture Data Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |BPWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT2;

    /**
     * BPWM_FCAPDAT2
     * ===================================================================================================
     * Offset: 0x220  BPWM Falling Capture Data Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |BPWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT2;

    /**
     * BPWM_RCAPDAT3
     * ===================================================================================================
     * Offset: 0x224  BPWM Rising Capture Data Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |BPWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT3;

    /**
     * BPWM_FCAPDAT3
     * ===================================================================================================
     * Offset: 0x228  BPWM Falling Capture Data Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |BPWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT3;


    /**
     * BPWM_RCAPDAT4
     * ===================================================================================================
     * Offset: 0x22C  BPWM Rising Capture Data Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |BPWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT4;

    /**
     * BPWM_FCAPDAT4
     * ===================================================================================================
     * Offset: 0x230  BPWM Falling Capture Data Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |BPWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT4;

    /**
     * BPWM_RCAPDAT5
     * ===================================================================================================
     * Offset: 0x234  BPWM Rising Capture Data Register 5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |BPWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT5;

    /**
     * BPWM_FCAPDAT5
     * ===================================================================================================
     * Offset: 0x238  BPWM Falling Capture Data Register 5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |BPWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the BPWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT5;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED13
     * ===================================================================================================
     * Offset: 0x023C ~ 0x024C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED13[5];
    /**
     * @endcond
     */

    /**
     * BPWM_CAPIEN
     * ===================================================================================================
     * Offset: 0x250  BPWM Capture Interrupt Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPRIENn  |BPWM Capture Rising Latch Interrupt Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Capture rising edge latch interrupt Disabled.
     * |        |          |1 = Capture rising edge latch interrupt Enabled.
     * |[13:8]  |CAPFIENn  |BPWM Capture Falling Latch Interrupt Enable
     * |        |          |Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = Capture falling edge latch interrupt Disabled.
     * |        |          |1 = Capture falling edge latch interrupt Enabled.
     */
    __IO uint32_t CAPIEN;


    /**
     * BPWM_CAPIF
     * ===================================================================================================
     * Offset: 0x254  BPWM Capture Interrupt Flag Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPRIFn   |BPWM Capture Rising Latch Interrupt Flag
     * |        |          |This bit is writing 1 to clear. Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = No capture rising latch condition happened.
     * |        |          |1 = Capture rising latch condition happened, this flag will be set to high.
     * |[13:8]  |CAPFIFn   |BPWM Capture Falling Latch Interrupt Flag
     * |        |          |This bit is writing 1 to clear. Each bit n controls the corresponding BPWM channel n.
     * |        |          |0 = No capture falling latch condition happened.
     * |        |          |1 = Capture falling latch condition happened, this flag will be set to high.
     */
    __IO uint32_t CAPIF;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED14
     * ===================================================================================================
     * Offset: 0x0258 ~ 0x0300
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED14[43];
    /**
     * @endcond
     */

    /**
     * BPWM_PBUF
     * ===================================================================================================
     * Offset: 0x304  BPWM PERIOD Buffer
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |PBUF      |BPWM Period Register Buffer (Read Only)
     * |        |          |Used as PERIOD active register.
     */
    __I  uint32_t PBUF;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED15
     * ===================================================================================================
     * Offset: 0x0308 ~ 0x0318
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED15[5];
    /**
     * @endcond
     */

    /**
     * BPWM_CMPBUF0~5
     * ===================================================================================================
     * Offset: 0x31C~0x330  BPWM CMP0~5 Buffer
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |CMPBUF    |BPWM Comparator Register Buffer (Read Only)
     * |        |          |Used as CMP active register.
     */
    __I  uint32_t CMPBUF[6];

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED16
     * ===================================================================================================
     * Offset: 0x0334 ~ 0x0FFC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED16[819];
    /**
     * @endcond
     */
} BPWM_T;

/**
    @addtogroup BPWM_CONST BPWM Bit Field Definition
    Constant Definitions for BPWM Controller
@{ */

#define BPWM_CTL0_CTRLDn_Pos             (0)                                                /*!< BPWM_T::CTL0: CTRLDn Position              */
#define BPWM_CTL0_CTRLDn_Msk             (0x3ful << BPWM_CTL0_CTRLDn_Pos)                   /*!< BPWM_T::CTL0: CTRLDn Mask                  */

#define BPWM_CTL0_CTRLD0_Pos             (0)                                                /*!< BPWM_T::CTL0: CTRLD0 Position              */
#define BPWM_CTL0_CTRLD0_Msk             (0x1ul << BPWM_CTL0_CTRLD0_Pos)                    /*!< BPWM_T::CTL0: CTRLD0 Mask                  */

#define BPWM_CTL0_CTRLD1_Pos             (1)                                                /*!< BPWM_T::CTL0: CTRLD1 Position              */
#define BPWM_CTL0_CTRLD1_Msk             (0x1ul << BPWM_CTL0_CTRLD1_Pos)                    /*!< BPWM_T::CTL0: CTRLD1 Mask                  */

#define BPWM_CTL0_CTRLD2_Pos             (2)                                                /*!< BPWM_T::CTL0: CTRLD2 Position              */
#define BPWM_CTL0_CTRLD2_Msk             (0x1ul << BPWM_CTL0_CTRLD2_Pos)                    /*!< BPWM_T::CTL0: CTRLD2 Mask                  */

#define BPWM_CTL0_CTRLD3_Pos             (3)                                                /*!< BPWM_T::CTL0: CTRLD3 Position              */
#define BPWM_CTL0_CTRLD3_Msk             (0x1ul << BPWM_CTL0_CTRLD3_Pos)                    /*!< BPWM_T::CTL0: CTRLD3 Mask                  */

#define BPWM_CTL0_CTRLD4_Pos             (4)                                                /*!< BPWM_T::CTL0: CTRLD4 Position              */
#define BPWM_CTL0_CTRLD4_Msk             (0x1ul << BPWM_CTL0_CTRLD4_Pos)                    /*!< BPWM_T::CTL0: CTRLD4 Mask                  */

#define BPWM_CTL0_CTRLD5_Pos             (5)                                                /*!< BPWM_T::CTL0: CTRLD5 Position              */
#define BPWM_CTL0_CTRLD5_Msk             (0x1ul << BPWM_CTL0_CTRLD5_Pos)                    /*!< BPWM_T::CTL0: CTRLD5 Mask                  */

#define BPWM_CTL0_IMMLDENn_Pos           (16)                                               /*!< BPWM_T::CTL0: IMMLDENn Position            */
#define BPWM_CTL0_IMMLDENn_Msk           (0x3ful << BPWM_CTL0_IMMLDENn_Pos)                 /*!< BPWM_T::CTL0: IMMLDENn Mask                */

#define BPWM_CTL0_IMMLDEN0_Pos           (16)                                               /*!< BPWM_T::CTL0: IMMLDEN0 Position            */
#define BPWM_CTL0_IMMLDEN0_Msk           (0x1ul << BPWM_CTL0_IMMLDEN0_Pos)                  /*!< BPWM_T::CTL0: IMMLDEN0 Mask                */

#define BPWM_CTL0_IMMLDEN1_Pos           (17)                                               /*!< BPWM_T::CTL0: IMMLDEN1 Position            */
#define BPWM_CTL0_IMMLDEN1_Msk           (0x1ul << BPWM_CTL0_IMMLDEN1_Pos)                  /*!< BPWM_T::CTL0: IMMLDEN1 Mask                */

#define BPWM_CTL0_IMMLDEN2_Pos           (18)                                               /*!< BPWM_T::CTL0: IMMLDEN2 Position            */
#define BPWM_CTL0_IMMLDEN2_Msk           (0x1ul << BPWM_CTL0_IMMLDEN2_Pos)                  /*!< BPWM_T::CTL0: IMMLDEN2 Mask                */

#define BPWM_CTL0_IMMLDEN3_Pos           (19)                                               /*!< BPWM_T::CTL0: IMMLDEN3 Position            */
#define BPWM_CTL0_IMMLDEN3_Msk           (0x1ul << BPWM_CTL0_IMMLDEN3_Pos)                  /*!< BPWM_T::CTL0: IMMLDEN3 Mask                */

#define BPWM_CTL0_IMMLDEN4_Pos           (20)                                               /*!< BPWM_T::CTL0: IMMLDEN4 Position            */
#define BPWM_CTL0_IMMLDEN4_Msk           (0x1ul << BPWM_CTL0_IMMLDEN4_Pos)                  /*!< BPWM_T::CTL0: IMMLDEN4 Mask                */

#define BPWM_CTL0_IMMLDEN5_Pos           (21)                                               /*!< BPWM_T::CTL0: IMMLDEN5 Position            */
#define BPWM_CTL0_IMMLDEN5_Msk           (0x1ul << BPWM_CTL0_IMMLDEN5_Pos)                  /*!< BPWM_T::CTL0: IMMLDEN5 Mask                */

#define BPWM_CTL0_DBGHALT_Pos            (30)                                               /*!< BPWM_T::CTL0: DBGHALT Position             */
#define BPWM_CTL0_DBGHALT_Msk            (0x1ul << BPWM_CTL0_DBGHALT_Pos)                   /*!< BPWM_T::CTL0: DBGHALT Mask                 */

#define BPWM_CTL0_DBGTRIOFF_Pos          (31)                                               /*!< BPWM_T::CTL0: DBGTRIOFF Position           */
#define BPWM_CTL0_DBGTRIOFF_Msk          (0x1ul << BPWM_CTL0_DBGTRIOFF_Pos)                 /*!< BPWM_T::CTL0: DBGTRIOFF Mask               */

#define BPWM_CTL1_CNTTYPE0_Pos           (0)                                                /*!< BPWM_T::CTL1: CNTTYPE0 Position            */
#define BPWM_CTL1_CNTTYPE0_Msk           (0x3ul << BPWM_CTL1_CNTTYPE0_Pos)                  /*!< BPWM_T::CTL1: CNTTYPE0 Mask                */

#define BPWM_CLKSRC_ECLKSRC0_Pos         (0)                                                /*!< BPWM_T::CLKSRC: ECLKSRC0 Position          */
#define BPWM_CLKSRC_ECLKSRC0_Msk         (0x7ul << BPWM_CLKSRC_ECLKSRC0_Pos)                /*!< BPWM_T::CLKSRC: ECLKSRC0 Mask              */

#define BPWM_CLKPSC_CLKPSC_Pos           (0)                                                /*!< BPWM_T::CLKSRC: CLKPSC Position            */
#define BPWM_CLKPSC_CLKPSC_Msk           (0xffful << BPWM_CLKPSC_CLKPSC_Pos)                /*!< BPWM_T::CLKSRC: CLKPSC Mask                */

#define BPWM_CNTEN_CNTEN0_Pos            (0)                                                /*!< BPWM_T::CNTEN: CNTEN0 Position             */
#define BPWM_CNTEN_CNTEN0_Msk            (0x1ul << BPWM_CNTEN_CNTEN0_Pos)                   /*!< BPWM_T::CNTEN: CNTEN0 Mask                 */

#define BPWM_CNTCLR_CNTCLR0_Pos          (0)                                                /*!< BPWM_T::CNTCLR: CNTCLR0 Position           */
#define BPWM_CNTCLR_CNTCLR0_Msk          (0x1ul << BPWM_CNTCLR_CNTCLR0_Pos)                 /*!< BPWM_T::CNTCLR: CNTCLR0 Mask               */

#define BPWM_PERIOD_PERIOD_Pos           (0)                                                /*!< BPWM_T::PERIOD: PERIOD Position            */
#define BPWM_PERIOD_PERIOD_Msk           (0xfffful << BPWM_PERIOD_PERIOD_Pos)               /*!< BPWM_T::PERIOD: PERIOD Mask                */

#define BPWM_CMPDAT_CMP_Pos              (0)                                                /*!< BPWM_T::CMPDAT: CMP Position               */
#define BPWM_CMPDAT_CMP_Msk              (0xfffful << BPWM_CMPDAT_CMP_Pos)                  /*!< BPWM_T::CMPDAT: CMP Mask                   */

#define BPWM_CNT_CNT_Pos                 (0)                                                /*!< BPWM_T::CNT: CNT Position                  */
#define BPWM_CNT_CNT_Msk                 (0xfffful << BPWM_CNT_CNT_Pos)                     /*!< BPWM_T::CNT: CNT Mask                      */

#define BPWM_CNT_DIRF_Pos                (16)                                               /*!< BPWM_T::CNT: DIRF Position                 */
#define BPWM_CNT_DIRF_Msk                (0x1ul << BPWM_CNT_DIRF_Pos)                       /*!< BPWM_T::CNT: DIRF Mask                     */

#define BPWM_WGCTL0_ZPCTLn_Pos           (0)                                                /*!< BPWM_T::WGCTL0: ZPCTLn Position            */
#define BPWM_WGCTL0_ZPCTLn_Msk           (0xffful << BPWM_WGCTL0_ZPCTLn_Pos)                /*!< BPWM_T::WGCTL0: ZPCTLn Mask                */

#define BPWM_WGCTL0_ZPCTL0_Pos           (0)                                                /*!< BPWM_T::WGCTL0: ZPCTL0 Position            */
#define BPWM_WGCTL0_ZPCTL0_Msk           (0x3ul << BPWM_WGCTL0_ZPCTL0_Pos)                  /*!< BPWM_T::WGCTL0: ZPCTL0 Mask                */

#define BPWM_WGCTL0_ZPCTL1_Pos           (2)                                                /*!< BPWM_T::WGCTL0: ZPCTL1 Position            */
#define BPWM_WGCTL0_ZPCTL1_Msk           (0x3ul << BPWM_WGCTL0_ZPCTL1_Pos)                  /*!< BPWM_T::WGCTL0: ZPCTL1 Mask                */

#define BPWM_WGCTL0_ZPCTL2_Pos           (4)                                                /*!< BPWM_T::WGCTL0: ZPCTL2 Position            */
#define BPWM_WGCTL0_ZPCTL2_Msk           (0x3ul << BPWM_WGCTL0_ZPCTL2_Pos)                  /*!< BPWM_T::WGCTL0: ZPCTL2 Mask                */

#define BPWM_WGCTL0_ZPCTL3_Pos           (6)                                                /*!< BPWM_T::WGCTL0: ZPCTL3 Position            */
#define BPWM_WGCTL0_ZPCTL3_Msk           (0x3ul << BPWM_WGCTL0_ZPCTL3_Pos)                  /*!< BPWM_T::WGCTL0: ZPCTL3 Mask                */

#define BPWM_WGCTL0_ZPCTL4_Pos           (8)                                                /*!< BPWM_T::WGCTL0: ZPCTL4 Position            */
#define BPWM_WGCTL0_ZPCTL4_Msk           (0x3ul << BPWM_WGCTL0_ZPCTL4_Pos)                  /*!< BPWM_T::WGCTL0: ZPCTL4 Mask                */

#define BPWM_WGCTL0_ZPCTL5_Pos           (10)                                               /*!< BPWM_T::WGCTL0: ZPCTL5 Position            */
#define BPWM_WGCTL0_ZPCTL5_Msk           (0x3ul << BPWM_WGCTL0_ZPCTL5_Pos)                  /*!< BPWM_T::WGCTL0: ZPCTL5 Mask                */

#define BPWM_WGCTL0_PRDPCTLn_Pos         (16)                                               /*!< BPWM_T::WGCTL0: PRDPCTLn Position          */
#define BPWM_WGCTL0_PRDPCTLn_Msk         (0xffful << BPWM_WGCTL0_PRDPCTLn_Pos)              /*!< BPWM_T::WGCTL0: PRDPCTLn Mask              */

#define BPWM_WGCTL0_PRDPCTL0_Pos         (16)                                               /*!< BPWM_T::WGCTL0: PRDPCTL0 Position          */
#define BPWM_WGCTL0_PRDPCTL0_Msk         (0x3ul << BPWM_WGCTL0_PRDPCTL0_Pos)                /*!< BPWM_T::WGCTL0: PRDPCTL0 Mask              */

#define BPWM_WGCTL0_PRDPCTL1_Pos         (18)                                               /*!< BPWM_T::WGCTL0: PRDPCTL1 Position          */
#define BPWM_WGCTL0_PRDPCTL1_Msk         (0x3ul << BPWM_WGCTL0_PRDPCTL1_Pos)                /*!< BPWM_T::WGCTL0: PRDPCTL1 Mask              */

#define BPWM_WGCTL0_PRDPCTL2_Pos         (20)                                               /*!< BPWM_T::WGCTL0: PRDPCTL2 Position          */
#define BPWM_WGCTL0_PRDPCTL2_Msk         (0x3ul << BPWM_WGCTL0_PRDPCTL2_Pos)                /*!< BPWM_T::WGCTL0: PRDPCTL2 Mask              */

#define BPWM_WGCTL0_PRDPCTL3_Pos         (22)                                               /*!< BPWM_T::WGCTL0: PRDPCTL3 Position          */
#define BPWM_WGCTL0_PRDPCTL3_Msk         (0x3ul << BPWM_WGCTL0_PRDPCTL3_Pos)                /*!< BPWM_T::WGCTL0: PRDPCTL3 Mask              */

#define BPWM_WGCTL0_PRDPCTL4_Pos         (24)                                               /*!< BPWM_T::WGCTL0: PRDPCTL4 Position          */
#define BPWM_WGCTL0_PRDPCTL4_Msk         (0x3ul << BPWM_WGCTL0_PRDPCTL4_Pos)                /*!< BPWM_T::WGCTL0: PRDPCTL4 Mask              */

#define BPWM_WGCTL0_PRDPCTL5_Pos         (26)                                               /*!< BPWM_T::WGCTL0: PRDPCTL5 Position          */
#define BPWM_WGCTL0_PRDPCTL5_Msk         (0x3ul << BPWM_WGCTL0_PRDPCTL5_Pos)                /*!< BPWM_T::WGCTL0: PRDPCTL5 Mask              */

#define BPWM_WGCTL1_CMPUCTLn_Pos         (0)                                                /*!< BPWM_T::WGCTL1: CMPUCTLn Position          */
#define BPWM_WGCTL1_CMPUCTLn_Msk         (0xffful << BPWM_WGCTL1_CMPUCTLn_Pos)              /*!< BPWM_T::WGCTL1: CMPUCTLn Mask              */

#define BPWM_WGCTL1_CMPUCTL0_Pos         (0)                                                /*!< BPWM_T::WGCTL1: CMPUCTL0 Position          */
#define BPWM_WGCTL1_CMPUCTL0_Msk         (0x3ul << BPWM_WGCTL1_CMPUCTL0_Pos)                /*!< BPWM_T::WGCTL1: CMPUCTL0 Mask              */

#define BPWM_WGCTL1_CMPUCTL1_Pos         (2)                                                /*!< BPWM_T::WGCTL1: CMPUCTL1 Position          */
#define BPWM_WGCTL1_CMPUCTL1_Msk         (0x3ul << BPWM_WGCTL1_CMPUCTL1_Pos)                /*!< BPWM_T::WGCTL1: CMPUCTL1 Mask              */

#define BPWM_WGCTL1_CMPUCTL2_Pos         (4)                                                /*!< BPWM_T::WGCTL1: CMPUCTL2 Position          */
#define BPWM_WGCTL1_CMPUCTL2_Msk         (0x3ul << BPWM_WGCTL1_CMPUCTL2_Pos)                /*!< BPWM_T::WGCTL1: CMPUCTL2 Mask              */

#define BPWM_WGCTL1_CMPUCTL3_Pos         (6)                                                /*!< BPWM_T::WGCTL1: CMPUCTL3 Position          */
#define BPWM_WGCTL1_CMPUCTL3_Msk         (0x3ul << BPWM_WGCTL1_CMPUCTL3_Pos)                /*!< BPWM_T::WGCTL1: CMPUCTL3 Mask              */

#define BPWM_WGCTL1_CMPUCTL4_Pos         (8)                                                /*!< BPWM_T::WGCTL1: CMPUCTL4 Position          */
#define BPWM_WGCTL1_CMPUCTL4_Msk         (0x3ul << BPWM_WGCTL1_CMPUCTL4_Pos)                /*!< BPWM_T::WGCTL1: CMPUCTL4 Mask              */

#define BPWM_WGCTL1_CMPUCTL5_Pos         (10)                                               /*!< BPWM_T::WGCTL1: CMPUCTL5 Position          */
#define BPWM_WGCTL1_CMPUCTL5_Msk         (0x3ul << BPWM_WGCTL1_CMPUCTL5_Pos)                /*!< BPWM_T::WGCTL1: CMPUCTL5 Mask              */

#define BPWM_WGCTL1_CMPDCTLn_Pos         (16)                                               /*!< BPWM_T::WGCTL1: CMPDCTLn Position          */
#define BPWM_WGCTL1_CMPDCTLn_Msk         (0xffful << BPWM_WGCTL1_CMPDCTLn_Pos)              /*!< BPWM_T::WGCTL1: CMPDCTLn Mask              */

#define BPWM_WGCTL1_CMPDCTL0_Pos         (16)                                               /*!< BPWM_T::WGCTL1: CMPDCTL0 Position          */
#define BPWM_WGCTL1_CMPDCTL0_Msk         (0x3ul << BPWM_WGCTL1_CMPDCTL0_Pos)                /*!< BPWM_T::WGCTL1: CMPDCTL0 Mask              */

#define BPWM_WGCTL1_CMPDCTL1_Pos         (18)                                               /*!< BPWM_T::WGCTL1: CMPDCTL1 Position          */
#define BPWM_WGCTL1_CMPDCTL1_Msk         (0x3ul << BPWM_WGCTL1_CMPDCTL1_Pos)                /*!< BPWM_T::WGCTL1: CMPDCTL1 Mask              */

#define BPWM_WGCTL1_CMPDCTL2_Pos         (20)                                               /*!< BPWM_T::WGCTL1: CMPDCTL2 Position          */
#define BPWM_WGCTL1_CMPDCTL2_Msk         (0x3ul << BPWM_WGCTL1_CMPDCTL2_Pos)                /*!< BPWM_T::WGCTL1: CMPDCTL2 Mask              */

#define BPWM_WGCTL1_CMPDCTL3_Pos         (22)                                               /*!< BPWM_T::WGCTL1: CMPDCTL3 Position          */
#define BPWM_WGCTL1_CMPDCTL3_Msk         (0x3ul << BPWM_WGCTL1_CMPDCTL3_Pos)                /*!< BPWM_T::WGCTL1: CMPDCTL3 Mask              */

#define BPWM_WGCTL1_CMPDCTL4_Pos         (24)                                               /*!< BPWM_T::WGCTL1: CMPDCTL4 Position          */
#define BPWM_WGCTL1_CMPDCTL4_Msk         (0x3ul << BPWM_WGCTL1_CMPDCTL4_Pos)                /*!< BPWM_T::WGCTL1: CMPDCTL4 Mask              */

#define BPWM_WGCTL1_CMPDCTL5_Pos         (26)                                               /*!< BPWM_T::WGCTL1: CMPDCTL5 Position          */
#define BPWM_WGCTL1_CMPDCTL5_Msk         (0x3ul << BPWM_WGCTL1_CMPDCTL5_Pos)                /*!< BPWM_T::WGCTL1: CMPDCTL5 Mask              */

#define BPWM_MSKEN_MSKENn_Pos            (0)                                                /*!< BPWM_T::MSKEN: MSKENn Position             */
#define BPWM_MSKEN_MSKENn_Msk            (0x3ful << BPWM_MSKEN_MSKENn_Pos)                  /*!< BPWM_T::MSKEN: MSKENn Mask                 */

#define BPWM_MSKEN_MSKEN0_Pos            (0)                                                /*!< BPWM_T::MSKEN: MSKEN0 Position             */
#define BPWM_MSKEN_MSKEN0_Msk            (0x1ul << BPWM_MSKEN_MSKEN0_Pos)                   /*!< BPWM_T::MSKEN: MSKEN0 Mask                 */

#define BPWM_MSKEN_MSKEN1_Pos            (1)                                                /*!< BPWM_T::MSKEN: MSKEN1 Position             */
#define BPWM_MSKEN_MSKEN1_Msk            (0x1ul << BPWM_MSKEN_MSKEN1_Pos)                   /*!< BPWM_T::MSKEN: MSKEN1 Mask                 */

#define BPWM_MSKEN_MSKEN2_Pos            (2)                                                /*!< BPWM_T::MSKEN: MSKEN2 Position             */
#define BPWM_MSKEN_MSKEN2_Msk            (0x1ul << BPWM_MSKEN_MSKEN2_Pos)                   /*!< BPWM_T::MSKEN: MSKEN2 Mask                 */

#define BPWM_MSKEN_MSKEN3_Pos            (3)                                                /*!< BPWM_T::MSKEN: MSKEN3 Position             */
#define BPWM_MSKEN_MSKEN3_Msk            (0x1ul << BPWM_MSKEN_MSKEN3_Pos)                   /*!< BPWM_T::MSKEN: MSKEN3 Mask                 */

#define BPWM_MSKEN_MSKEN4_Pos            (4)                                                /*!< BPWM_T::MSKEN: MSKEN4 Position             */
#define BPWM_MSKEN_MSKEN4_Msk            (0x1ul << BPWM_MSKEN_MSKEN4_Pos)                   /*!< BPWM_T::MSKEN: MSKEN4 Mask                 */

#define BPWM_MSKEN_MSKEN5_Pos            (5)                                                /*!< BPWM_T::MSKEN: MSKEN5 Position             */
#define BPWM_MSKEN_MSKEN5_Msk            (0x1ul << BPWM_MSKEN_MSKEN5_Pos)                   /*!< BPWM_T::MSKEN: MSKEN5 Mask                 */

#define BPWM_MSK_MSKDATn_Pos             (0)                                                /*!< BPWM_T::MSK: MSKDATn Position              */
#define BPWM_MSK_MSKDATn_Msk             (0x3ful << BPWM_MSK_MSKDATn_Pos)                   /*!< BPWM_T::MSK: MSKDATn Mask                  */

#define BPWM_MSK_MSKDAT0_Pos             (0)                                                /*!< BPWM_T::MSK: MSKDAT0 Position              */
#define BPWM_MSK_MSKDAT0_Msk             (0x1ul << BPWM_MSK_MSKDAT0_Pos)                    /*!< BPWM_T::MSK: MSKDAT0 Mask                  */

#define BPWM_MSK_MSKDAT1_Pos             (1)                                                /*!< BPWM_T::MSK: MSKDAT1 Position              */
#define BPWM_MSK_MSKDAT1_Msk             (0x1ul << BPWM_MSK_MSKDAT1_Pos)                    /*!< BPWM_T::MSK: MSKDAT1 Mask                  */

#define BPWM_MSK_MSKDAT2_Pos             (2)                                                /*!< BPWM_T::MSK: MSKDAT2 Position              */
#define BPWM_MSK_MSKDAT2_Msk             (0x1ul << BPWM_MSK_MSKDAT2_Pos)                    /*!< BPWM_T::MSK: MSKDAT2 Mask                  */

#define BPWM_MSK_MSKDAT3_Pos             (3)                                                /*!< BPWM_T::MSK: MSKDAT3 Position              */
#define BPWM_MSK_MSKDAT3_Msk             (0x1ul << BPWM_MSK_MSKDAT3_Pos)                    /*!< BPWM_T::MSK: MSKDAT3 Mask                  */

#define BPWM_MSK_MSKDAT4_Pos             (4)                                                /*!< BPWM_T::MSK: MSKDAT4 Position              */
#define BPWM_MSK_MSKDAT4_Msk             (0x1ul << BPWM_MSK_MSKDAT4_Pos)                    /*!< BPWM_T::MSK: MSKDAT4 Mask                  */

#define BPWM_MSK_MSKDAT5_Pos             (5)                                                /*!< BPWM_T::MSK: MSKDAT5 Position              */
#define BPWM_MSK_MSKDAT5_Msk             (0x1ul << BPWM_MSK_MSKDAT5_Pos)                    /*!< BPWM_T::MSK: MSKDAT5 Mask                  */

#define BPWM_POLCTL_PINVn_Pos            (0)                                                /*!< BPWM_T::POLCTL: PINVn Position             */
#define BPWM_POLCTL_PINVn_Msk            (0x3ful << BPWM_POLCTL_PINVn_Pos)                  /*!< BPWM_T::POLCTL: PINVn Mask                 */

#define BPWM_POLCTL_PINV0_Pos            (0)                                                /*!< BPWM_T::POLCTL: PINV0 Position             */
#define BPWM_POLCTL_PINV0_Msk            (0x1ul << BPWM_POLCTL_PINV0_Pos)                   /*!< BPWM_T::POLCTL: PINV0 Mask                 */

#define BPWM_POLCTL_PINV1_Pos            (1)                                                /*!< BPWM_T::POLCTL: PINV1 Position             */
#define BPWM_POLCTL_PINV1_Msk            (0x1ul << BPWM_POLCTL_PINV1_Pos)                   /*!< BPWM_T::POLCTL: PINV1 Mask                 */

#define BPWM_POLCTL_PINV2_Pos            (2)                                                /*!< BPWM_T::POLCTL: PINV2 Position             */
#define BPWM_POLCTL_PINV2_Msk            (0x1ul << BPWM_POLCTL_PINV2_Pos)                   /*!< BPWM_T::POLCTL: PINV2 Mask                 */

#define BPWM_POLCTL_PINV3_Pos            (3)                                                /*!< BPWM_T::POLCTL: PINV3 Position             */
#define BPWM_POLCTL_PINV3_Msk            (0x1ul << BPWM_POLCTL_PINV3_Pos)                   /*!< BPWM_T::POLCTL: PINV3 Mask                 */

#define BPWM_POLCTL_PINV4_Pos            (4)                                                /*!< BPWM_T::POLCTL: PINV4 Position             */
#define BPWM_POLCTL_PINV4_Msk            (0x1ul << BPWM_POLCTL_PINV4_Pos)                   /*!< BPWM_T::POLCTL: PINV4 Mask                 */

#define BPWM_POLCTL_PINV5_Pos            (5)                                                /*!< BPWM_T::POLCTL: PINV5 Position             */
#define BPWM_POLCTL_PINV5_Msk            (0x1ul << BPWM_POLCTL_PINV5_Pos)                   /*!< BPWM_T::POLCTL: PINV5 Mask                 */

#define BPWM_POEN_POENn_Pos              (0)                                                /*!< BPWM_T::POEN: POENn Position               */
#define BPWM_POEN_POENn_Msk              (0x3ful << BPWM_POEN_POENn_Pos)                    /*!< BPWM_T::POEN: POENn Mask                   */

#define BPWM_POEN_POEN0_Pos              (0)                                                /*!< BPWM_T::POEN: POEN0 Position               */
#define BPWM_POEN_POEN0_Msk              (0x1ul << BPWM_POEN_POEN0_Pos)                     /*!< BPWM_T::POEN: POEN0 Mask                   */

#define BPWM_POEN_POEN1_Pos              (1)                                                /*!< BPWM_T::POEN: POEN1 Position               */
#define BPWM_POEN_POEN1_Msk              (0x1ul << BPWM_POEN_POEN1_Pos)                     /*!< BPWM_T::POEN: POEN1 Mask                   */

#define BPWM_POEN_POEN2_Pos              (2)                                                /*!< BPWM_T::POEN: POEN2 Position               */
#define BPWM_POEN_POEN2_Msk              (0x1ul << BPWM_POEN_POEN2_Pos)                     /*!< BPWM_T::POEN: POEN2 Mask                   */

#define BPWM_POEN_POEN3_Pos              (3)                                                /*!< BPWM_T::POEN: POEN3 Position               */
#define BPWM_POEN_POEN3_Msk              (0x1ul << BPWM_POEN_POEN3_Pos)                     /*!< BPWM_T::POEN: POEN3 Mask                   */

#define BPWM_POEN_POEN4_Pos              (4)                                                /*!< BPWM_T::POEN: POEN4 Position               */
#define BPWM_POEN_POEN4_Msk              (0x1ul << BPWM_POEN_POEN4_Pos)                     /*!< BPWM_T::POEN: POEN4 Mask                   */

#define BPWM_POEN_POEN5_Pos              (5)                                                /*!< BPWM_T::POEN: POEN5 Position               */
#define BPWM_POEN_POEN5_Msk              (0x1ul << BPWM_POEN_POEN5_Pos)                     /*!< BPWM_T::POEN: POEN5 Mask                   */

#define BPWM_INTEN_ZIEN0_Pos             (0)                                                /*!< BPWM_T::INTEN: ZIEN0 Position              */
#define BPWM_INTEN_ZIEN0_Msk             (0x1ul << BPWM_INTEN_ZIEN0_Pos)                    /*!< BPWM_T::INTEN: ZIEN0 Mask                  */

#define BPWM_INTEN_PIEN0_Pos             (8)                                                /*!< BPWM_T::INTEN: PIEN0 Position              */
#define BPWM_INTEN_PIEN0_Msk             (0x1ul << BPWM_INTEN_PIEN0_Pos)                    /*!< BPWM_T::INTEN: PIEN0 Mask                  */

#define BPWM_INTEN_CMPUIENn_Pos          (16)                                               /*!< BPWM_T::INTEN: CMPUIENn Position           */
#define BPWM_INTEN_CMPUIENn_Msk          (0x3ful << BPWM_INTEN_CMPUIENn_Pos)                /*!< BPWM_T::INTEN: CMPUIENn Mask               */

#define BPWM_INTEN_CMPUIEN0_Pos          (16)                                               /*!< BPWM_T::INTEN: CMPUIEN0 Position           */
#define BPWM_INTEN_CMPUIEN0_Msk          (0x1ul << BPWM_INTEN_CMPUIEN0_Pos)                 /*!< BPWM_T::INTEN: CMPUIEN0 Mask               */

#define BPWM_INTEN_CMPUIEN1_Pos          (17)                                               /*!< BPWM_T::INTEN: CMPUIEN1 Position           */
#define BPWM_INTEN_CMPUIEN1_Msk          (0x1ul << BPWM_INTEN_CMPUIEN1_Pos)                 /*!< BPWM_T::INTEN: CMPUIEN1 Mask               */

#define BPWM_INTEN_CMPUIEN2_Pos          (18)                                               /*!< BPWM_T::INTEN: CMPUIEN2 Position           */
#define BPWM_INTEN_CMPUIEN2_Msk          (0x1ul << BPWM_INTEN_CMPUIEN2_Pos)                 /*!< BPWM_T::INTEN: CMPUIEN2 Mask               */

#define BPWM_INTEN_CMPUIEN3_Pos          (19)                                               /*!< BPWM_T::INTEN: CMPUIEN3 Position           */
#define BPWM_INTEN_CMPUIEN3_Msk          (0x1ul << BPWM_INTEN_CMPUIEN3_Pos)                 /*!< BPWM_T::INTEN: CMPUIEN3 Mask               */

#define BPWM_INTEN_CMPUIEN4_Pos          (20)                                               /*!< BPWM_T::INTEN: CMPUIEN4 Position           */
#define BPWM_INTEN_CMPUIEN4_Msk          (0x1ul << BPWM_INTEN_CMPUIEN4_Pos)                 /*!< BPWM_T::INTEN: CMPUIEN4 Mask               */

#define BPWM_INTEN_CMPUIEN5_Pos          (21)                                               /*!< BPWM_T::INTEN: CMPUIEN5 Position           */
#define BPWM_INTEN_CMPUIEN5_Msk          (0x1ul << BPWM_INTEN_CMPUIEN5_Pos)                 /*!< BPWM_T::INTEN: CMPUIEN5 Mask               */

#define BPWM_INTEN_CMPDIENn_Pos          (24)                                               /*!< BPWM_T::INTEN: CMPDIENn Position           */
#define BPWM_INTEN_CMPDIENn_Msk          (0x3ful << BPWM_INTEN_CMPDIENn_Pos)                /*!< BPWM_T::INTEN: CMPDIENn Mask               */

#define BPWM_INTEN_CMPDIEN0_Pos          (24)                                               /*!< BPWM_T::INTEN: CMPDIEN0 Position           */
#define BPWM_INTEN_CMPDIEN0_Msk          (0x1ul << BPWM_INTEN_CMPDIEN0_Pos)                 /*!< BPWM_T::INTEN: CMPDIEN0 Mask               */

#define BPWM_INTEN_CMPDIEN1_Pos          (25)                                               /*!< BPWM_T::INTEN: CMPDIEN1 Position           */
#define BPWM_INTEN_CMPDIEN1_Msk          (0x1ul << BPWM_INTEN_CMPDIEN1_Pos)                 /*!< BPWM_T::INTEN: CMPDIEN1 Mask               */

#define BPWM_INTEN_CMPDIEN2_Pos          (26)                                               /*!< BPWM_T::INTEN: CMPDIEN2 Position           */
#define BPWM_INTEN_CMPDIEN2_Msk          (0x1ul << BPWM_INTEN_CMPDIEN2_Pos)                 /*!< BPWM_T::INTEN: CMPDIEN2 Mask               */

#define BPWM_INTEN_CMPDIEN3_Pos          (27)                                               /*!< BPWM_T::INTEN: CMPDIEN3 Position           */
#define BPWM_INTEN_CMPDIEN3_Msk          (0x1ul << BPWM_INTEN_CMPDIEN3_Pos)                 /*!< BPWM_T::INTEN: CMPDIEN3 Mask               */

#define BPWM_INTEN_CMPDIEN4_Pos          (28)                                               /*!< BPWM_T::INTEN: CMPDIEN4 Position           */
#define BPWM_INTEN_CMPDIEN4_Msk          (0x1ul << BPWM_INTEN_CMPDIEN4_Pos)                 /*!< BPWM_T::INTEN: CMPDIEN4 Mask               */

#define BPWM_INTEN_CMPDIEN5_Pos          (29)                                               /*!< BPWM_T::INTEN: CMPDIEN5 Position           */
#define BPWM_INTEN_CMPDIEN5_Msk          (0x1ul << BPWM_INTEN_CMPDIEN5_Pos)                 /*!< BPWM_T::INTEN: CMPDIEN5 Mask               */

#define BPWM_INTSTS_ZIF0_Pos             (0)                                                /*!< BPWM_T::INTSTS: ZIF0 Position              */
#define BPWM_INTSTS_ZIF0_Msk             (0x1ul << BPWM_INTSTS_ZIF0_Pos)                    /*!< BPWM_T::INTSTS: ZIF0 Mask                  */

#define BPWM_INTSTS_PIF0_Pos             (8)                                                /*!< BPWM_T::INTSTS: PIF0 Position              */
#define BPWM_INTSTS_PIF0_Msk             (0x1ul << BPWM_INTSTS_PIF0_Pos)                    /*!< BPWM_T::INTSTS: PIF0 Mask                  */

#define BPWM_INTSTS_CMPUIFn_Pos          (16)                                               /*!< BPWM_T::INTSTS: CMPUIFn Position           */
#define BPWM_INTSTS_CMPUIFn_Msk          (0x3ful << BPWM_INTSTS_CMPUIFn_Pos)                /*!< BPWM_T::INTSTS: CMPUIFn Mask               */

#define BPWM_INTSTS_CMPUIF0_Pos          (16)                                               /*!< BPWM_T::INTSTS: CMPUIF0 Position           */
#define BPWM_INTSTS_CMPUIF0_Msk          (0x1ul << BPWM_INTSTS_CMPUIF0_Pos)                 /*!< BPWM_T::INTSTS: CMPUIF0 Mask               */

#define BPWM_INTSTS_CMPUIF1_Pos          (17)                                               /*!< BPWM_T::INTSTS: CMPUIF1 Position           */
#define BPWM_INTSTS_CMPUIF1_Msk          (0x1ul << BPWM_INTSTS_CMPUIF1_Pos)                 /*!< BPWM_T::INTSTS: CMPUIF1 Mask               */

#define BPWM_INTSTS_CMPUIF2_Pos          (18)                                               /*!< BPWM_T::INTSTS: CMPUIF2 Position           */
#define BPWM_INTSTS_CMPUIF2_Msk          (0x1ul << BPWM_INTSTS_CMPUIF2_Pos)                 /*!< BPWM_T::INTSTS: CMPUIF2 Mask               */

#define BPWM_INTSTS_CMPUIF3_Pos          (19)                                               /*!< BPWM_T::INTSTS: CMPUIF3 Position           */
#define BPWM_INTSTS_CMPUIF3_Msk          (0x1ul << BPWM_INTSTS_CMPUIF3_Pos)                 /*!< BPWM_T::INTSTS: CMPUIF3 Mask               */

#define BPWM_INTSTS_CMPUIF4_Pos          (20)                                               /*!< BPWM_T::INTSTS: CMPUIF4 Position           */
#define BPWM_INTSTS_CMPUIF4_Msk          (0x1ul << BPWM_INTSTS_CMPUIF4_Pos)                 /*!< BPWM_T::INTSTS: CMPUIF4 Mask               */

#define BPWM_INTSTS_CMPUIF5_Pos          (21)                                               /*!< BPWM_T::INTSTS: CMPUIF5 Position           */
#define BPWM_INTSTS_CMPUIF5_Msk          (0x1ul << BPWM_INTSTS_CMPUIF5_Pos)                 /*!< BPWM_T::INTSTS: CMPUIF5 Mask               */

#define BPWM_INTSTS_CMPDIFn_Pos          (24)                                               /*!< BPWM_T::INTSTS: CMPDIFn Position           */
#define BPWM_INTSTS_CMPDIFn_Msk          (0x3ful << BPWM_INTSTS_CMPDIFn_Pos)                /*!< BPWM_T::INTSTS: CMPDIFn Mask               */

#define BPWM_INTSTS_CMPDIF0_Pos          (24)                                               /*!< BPWM_T::INTSTS: CMPDIF0 Position           */
#define BPWM_INTSTS_CMPDIF0_Msk          (0x1ul << BPWM_INTSTS_CMPDIF0_Pos)                 /*!< BPWM_T::INTSTS: CMPDIF0 Mask               */

#define BPWM_INTSTS_CMPDIF1_Pos          (25)                                               /*!< BPWM_T::INTSTS: CMPDIF1 Position           */
#define BPWM_INTSTS_CMPDIF1_Msk          (0x1ul << BPWM_INTSTS_CMPDIF1_Pos)                 /*!< BPWM_T::INTSTS: CMPDIF1 Mask               */

#define BPWM_INTSTS_CMPDIF2_Pos          (26)                                               /*!< BPWM_T::INTSTS: CMPDIF2 Position           */
#define BPWM_INTSTS_CMPDIF2_Msk          (0x1ul << BPWM_INTSTS_CMPDIF2_Pos)                 /*!< BPWM_T::INTSTS: CMPDIF2 Mask               */

#define BPWM_INTSTS_CMPDIF3_Pos          (27)                                               /*!< BPWM_T::INTSTS: CMPDIF3 Position           */
#define BPWM_INTSTS_CMPDIF3_Msk          (0x1ul << BPWM_INTSTS_CMPDIF3_Pos)                 /*!< BPWM_T::INTSTS: CMPDIF3 Mask               */

#define BPWM_INTSTS_CMPDIF4_Pos          (28)                                               /*!< BPWM_T::INTSTS: CMPDIF4 Position           */
#define BPWM_INTSTS_CMPDIF4_Msk          (0x1ul << BPWM_INTSTS_CMPDIF4_Pos)                 /*!< BPWM_T::INTSTS: CMPDIF4 Mask               */

#define BPWM_INTSTS_CMPDIF5_Pos          (29)                                               /*!< BPWM_T::INTSTS: CMPDIF5 Position           */
#define BPWM_INTSTS_CMPDIF5_Msk          (0x1ul << BPWM_INTSTS_CMPDIF5_Pos)                 /*!< BPWM_T::INTSTS: CMPDIF5 Mask               */

#define BPWM_ADCTS0_TRGSEL0_Pos          (0)                                                /*!< BPWM_T::ADCTS0: TRGSEL0 Position           */
#define BPWM_ADCTS0_TRGSEL0_Msk          (0xful << BPWM_ADCTS0_TRGSEL0_Pos)                 /*!< BPWM_T::ADCTS0: TRGSEL0 Mask               */

#define BPWM_ADCTS0_TRGEN0_Pos           (7)                                                /*!< BPWM_T::ADCTS0: TRGEN0 Position            */
#define BPWM_ADCTS0_TRGEN0_Msk           (0x1ul << BPWM_ADCTS0_TRGEN0_Pos)                  /*!< BPWM_T::ADCTS0: TRGEN0 Mask                */

#define BPWM_ADCTS0_TRGSEL1_Pos          (8)                                                /*!< BPWM_T::ADCTS0: TRGSEL1 Position           */
#define BPWM_ADCTS0_TRGSEL1_Msk          (0xful << BPWM_ADCTS0_TRGSEL1_Pos)                 /*!< BPWM_T::ADCTS0: TRGSEL1 Mask               */

#define BPWM_ADCTS0_TRGEN1_Pos           (15)                                               /*!< BPWM_T::ADCTS0: TRGEN1 Position            */
#define BPWM_ADCTS0_TRGEN1_Msk           (0x1ul << BPWM_ADCTS0_TRGEN1_Pos)                  /*!< BPWM_T::ADCTS0: TRGEN1 Mask                */

#define BPWM_ADCTS0_TRGSEL2_Pos          (16)                                               /*!< BPWM_T::ADCTS0: TRGSEL2 Position           */
#define BPWM_ADCTS0_TRGSEL2_Msk          (0xful << BPWM_ADCTS0_TRGSEL2_Pos)                 /*!< BPWM_T::ADCTS0: TRGSEL2 Mask               */

#define BPWM_ADCTS0_TRGEN2_Pos           (23)                                               /*!< BPWM_T::ADCTS0: TRGEN2 Position            */
#define BPWM_ADCTS0_TRGEN2_Msk           (0x1ul << BPWM_ADCTS0_TRGEN2_Pos)                  /*!< BPWM_T::ADCTS0: TRGEN2 Mask                */

#define BPWM_ADCTS0_TRGSEL3_Pos          (24)                                               /*!< BPWM_T::ADCTS0: TRGSEL3 Position           */
#define BPWM_ADCTS0_TRGSEL3_Msk          (0xful << BPWM_ADCTS0_TRGSEL3_Pos)                 /*!< BPWM_T::ADCTS0: TRGSEL3 Mask               */

#define BPWM_ADCTS0_TRGEN3_Pos           (31)                                               /*!< BPWM_T::ADCTS0: TRGEN3 Position            */
#define BPWM_ADCTS0_TRGEN3_Msk           (0x1ul << BPWM_ADCTS0_TRGEN3_Pos)                  /*!< BPWM_T::ADCTS0: TRGEN3 Mask                */

#define BPWM_ADCTS1_TRGSEL4_Pos          (0)                                                /*!< BPWM_T::ADCTS1: TRGSEL4 Position           */
#define BPWM_ADCTS1_TRGSEL4_Msk          (0xful << BPWM_ADCTS1_TRGSEL4_Pos)                 /*!< BPWM_T::ADCTS1: TRGSEL4 Mask               */

#define BPWM_ADCTS1_TRGEN4_Pos           (7)                                                /*!< BPWM_T::ADCTS1: TRGEN4 Position            */
#define BPWM_ADCTS1_TRGEN4_Msk           (0x1ul << BPWM_ADCTS1_TRGEN4_Pos)                  /*!< BPWM_T::ADCTS1: TRGEN4 Mask                */

#define BPWM_ADCTS1_TRGSEL5_Pos          (8)                                                /*!< BPWM_T::ADCTS1: TRGSEL5 Position           */
#define BPWM_ADCTS1_TRGSEL5_Msk          (0xful << BPWM_ADCTS1_TRGSEL5_Pos)                 /*!< BPWM_T::ADCTS1: TRGSEL5 Mask               */

#define BPWM_ADCTS1_TRGEN5_Pos           (15)                                               /*!< BPWM_T::ADCTS1: TRGEN5 Position            */
#define BPWM_ADCTS1_TRGEN5_Msk           (0x1ul << BPWM_ADCTS1_TRGEN5_Pos)                  /*!< BPWM_T::ADCTS1: TRGEN5 Mask                */

#define BPWM_SSCTL_SSEN0_Pos             (0)                                                /*!< BPWM_T::SSCTL: SSEN0 Position              */
#define BPWM_SSCTL_SSEN0_Msk             (0x1ul << BPWM_SSCTL_SSEN0_Pos)                    /*!< BPWM_T::SSCTL: SSEN0 Mask                  */

#define BPWM_SSCTL_SSRC_Pos              (8)                                                /*!< BPWM_T::SSCTL: SSRC Position               */
#define BPWM_SSCTL_SSRC_Msk              (0x3ul << BPWM_SSCTL_SSRC_Pos)                     /*!< BPWM_T::SSCTL: SSRC Mask                   */

#define BPWM_SSTRG_CNTSEN_Pos            (0)                                                /*!< BPWM_T::SSTRG: CNTSEN Position             */
#define BPWM_SSTRG_CNTSEN_Msk            (0x1ul << BPWM_SSTRG_CNTSEN_Pos)                   /*!< BPWM_T::SSTRG: CNTSEN Mask                 */

#define BPWM_STATUS_CNTMAXF0_Pos         (0)                                                /*!< BPWM_T::STATUS: CNTMAXF0 Position          */
#define BPWM_STATUS_CNTMAXF0_Msk         (0x1ul << BPWM_STATUS_CNTMAXF0_Pos)                /*!< BPWM_T::STATUS: CNTMAXF0 Mask              */

#define BPWM_STATUS_ADCTRGFn_Pos         (16)                                               /*!< BPWM_T::STATUS: ADCTRGFn Position          */
#define BPWM_STATUS_ADCTRGFn_Msk         (0x3ful << BPWM_STATUS_ADCTRGFn_Pos)               /*!< BPWM_T::STATUS: ADCTRGFn Mask              */

#define BPWM_STATUS_ADCTRGF0_Pos         (16)                                               /*!< BPWM_T::STATUS: ADCTRGF0 Position          */
#define BPWM_STATUS_ADCTRGF0_Msk         (0x1ul << BPWM_STATUS_ADCTRGF0_Pos)                /*!< BPWM_T::STATUS: ADCTRGF0 Mask              */

#define BPWM_STATUS_ADCTRGF1_Pos         (17)                                               /*!< BPWM_T::STATUS: ADCTRGF1 Position          */
#define BPWM_STATUS_ADCTRGF1_Msk         (0x1ul << BPWM_STATUS_ADCTRGF1_Pos)                /*!< BPWM_T::STATUS: ADCTRGF1 Mask              */

#define BPWM_STATUS_ADCTRGF2_Pos         (18)                                               /*!< BPWM_T::STATUS: ADCTRGF2 Position          */
#define BPWM_STATUS_ADCTRGF2_Msk         (0x1ul << BPWM_STATUS_ADCTRGF2_Pos)                /*!< BPWM_T::STATUS: ADCTRGF2 Mask              */

#define BPWM_STATUS_ADCTRGF3_Pos         (19)                                               /*!< BPWM_T::STATUS: ADCTRGF3 Position          */
#define BPWM_STATUS_ADCTRGF3_Msk         (0x1ul << BPWM_STATUS_ADCTRGF3_Pos)                /*!< BPWM_T::STATUS: ADCTRGF3 Mask              */

#define BPWM_STATUS_ADCTRGF4_Pos         (20)                                               /*!< BPWM_T::STATUS: ADCTRGF4 Position          */
#define BPWM_STATUS_ADCTRGF4_Msk         (0x1ul << BPWM_STATUS_ADCTRGF4_Pos)                /*!< BPWM_T::STATUS: ADCTRGF4 Mask              */

#define BPWM_STATUS_ADCTRGF5_Pos         (21)                                               /*!< BPWM_T::STATUS: ADCTRGF5 Position          */
#define BPWM_STATUS_ADCTRGF5_Msk         (0x1ul << BPWM_STATUS_ADCTRGF5_Pos)                /*!< BPWM_T::STATUS: ADCTRGF5 Mask              */

#define BPWM_CAPINEN_CAPINENn_Pos        (0)                                                /*!< BPWM_T::CAPINEN: CAPINENn Position         */
#define BPWM_CAPINEN_CAPINENn_Msk        (0x3ful << BPWM_CAPINEN_CAPINENn_Pos)              /*!< BPWM_T::CAPINEN: CAPINENn Mask             */

#define BPWM_CAPINEN_CAPINEN0_Pos        (0)                                                /*!< BPWM_T::CAPINEN: CAPINEN0 Position         */
#define BPWM_CAPINEN_CAPINEN0_Msk        (0x1ul << BPWM_CAPINEN_CAPINEN0_Pos)               /*!< BPWM_T::CAPINEN: CAPINEN0 Mask             */

#define BPWM_CAPINEN_CAPINEN1_Pos        (1)                                                /*!< BPWM_T::CAPINEN: CAPINEN1 Position         */
#define BPWM_CAPINEN_CAPINEN1_Msk        (0x1ul << BPWM_CAPINEN_CAPINEN1_Pos)               /*!< BPWM_T::CAPINEN: CAPINEN1 Mask             */

#define BPWM_CAPINEN_CAPINEN2_Pos        (2)                                                /*!< BPWM_T::CAPINEN: CAPINEN2 Position         */
#define BPWM_CAPINEN_CAPINEN2_Msk        (0x1ul << BPWM_CAPINEN_CAPINEN2_Pos)               /*!< BPWM_T::CAPINEN: CAPINEN2 Mask             */

#define BPWM_CAPINEN_CAPINEN3_Pos        (3)                                                /*!< BPWM_T::CAPINEN: CAPINEN3 Position         */
#define BPWM_CAPINEN_CAPINEN3_Msk        (0x1ul << BPWM_CAPINEN_CAPINEN3_Pos)               /*!< BPWM_T::CAPINEN: CAPINEN3 Mask             */

#define BPWM_CAPINEN_CAPINEN4_Pos        (4)                                                /*!< BPWM_T::CAPINEN: CAPINEN4 Position         */
#define BPWM_CAPINEN_CAPINEN4_Msk        (0x1ul << BPWM_CAPINEN_CAPINEN4_Pos)               /*!< BPWM_T::CAPINEN: CAPINEN4 Mask             */

#define BPWM_CAPINEN_CAPINEN5_Pos        (5)                                                /*!< BPWM_T::CAPINEN: CAPINEN5 Position         */
#define BPWM_CAPINEN_CAPINEN5_Msk        (0x1ul << BPWM_CAPINEN_CAPINEN5_Pos)               /*!< BPWM_T::CAPINEN: CAPINEN5 Mask             */

#define BPWM_CAPCTL_CAPENn_Pos           (0)                                                /*!< BPWM_T::CAPCTL: CAPENn Position            */
#define BPWM_CAPCTL_CAPENn_Msk           (0x3ful << BPWM_CAPCTL_CAPENn_Pos)                 /*!< BPWM_T::CAPCTL: CAPENn Mask                */

#define BPWM_CAPCTL_CAPEN0_Pos           (0)                                                /*!< BPWM_T::CAPCTL: CAPEN0 Position            */
#define BPWM_CAPCTL_CAPEN0_Msk           (0x1ul << BPWM_CAPCTL_CAPEN0_Pos)                  /*!< BPWM_T::CAPCTL: CAPEN0 Mask                */

#define BPWM_CAPCTL_CAPEN1_Pos           (1)                                                /*!< BPWM_T::CAPCTL: CAPEN1 Position            */
#define BPWM_CAPCTL_CAPEN1_Msk           (0x1ul << BPWM_CAPCTL_CAPEN1_Pos)                  /*!< BPWM_T::CAPCTL: CAPEN1 Mask                */

#define BPWM_CAPCTL_CAPEN2_Pos           (2)                                                /*!< BPWM_T::CAPCTL: CAPEN2 Position            */
#define BPWM_CAPCTL_CAPEN2_Msk           (0x1ul << BPWM_CAPCTL_CAPEN2_Pos)                  /*!< BPWM_T::CAPCTL: CAPEN2 Mask                */

#define BPWM_CAPCTL_CAPEN3_Pos           (3)                                                /*!< BPWM_T::CAPCTL: CAPEN3 Position            */
#define BPWM_CAPCTL_CAPEN3_Msk           (0x1ul << BPWM_CAPCTL_CAPEN3_Pos)                  /*!< BPWM_T::CAPCTL: CAPEN3 Mask                */

#define BPWM_CAPCTL_CAPEN4_Pos           (4)                                                /*!< BPWM_T::CAPCTL: CAPEN4 Position            */
#define BPWM_CAPCTL_CAPEN4_Msk           (0x1ul << BPWM_CAPCTL_CAPEN4_Pos)                  /*!< BPWM_T::CAPCTL: CAPEN4 Mask                */

#define BPWM_CAPCTL_CAPEN5_Pos           (5)                                                /*!< BPWM_T::CAPCTL: CAPEN5 Position            */
#define BPWM_CAPCTL_CAPEN5_Msk           (0x1ul << BPWM_CAPCTL_CAPEN5_Pos)                  /*!< BPWM_T::CAPCTL: CAPEN5 Mask                */

#define BPWM_CAPCTL_CAPINVn_Pos          (8)                                                /*!< BPWM_T::CAPCTL: CAPINVn Position           */
#define BPWM_CAPCTL_CAPINVn_Msk          (0x3ful << BPWM_CAPCTL_CAPINVn_Pos)                /*!< BPWM_T::CAPCTL: CAPINVn Mask               */

#define BPWM_CAPCTL_CAPINV0_Pos          (8)                                                /*!< BPWM_T::CAPCTL: CAPINV0 Position           */
#define BPWM_CAPCTL_CAPINV0_Msk          (0x1ul << BPWM_CAPCTL_CAPINV0_Pos)                 /*!< BPWM_T::CAPCTL: CAPINV0 Mask               */

#define BPWM_CAPCTL_CAPINV1_Pos          (9)                                                /*!< BPWM_T::CAPCTL: CAPINV1 Position           */
#define BPWM_CAPCTL_CAPINV1_Msk          (0x1ul << BPWM_CAPCTL_CAPINV1_Pos)                 /*!< BPWM_T::CAPCTL: CAPINV1 Mask               */

#define BPWM_CAPCTL_CAPINV2_Pos          (10)                                               /*!< BPWM_T::CAPCTL: CAPINV2 Position           */
#define BPWM_CAPCTL_CAPINV2_Msk          (0x1ul << BPWM_CAPCTL_CAPINV2_Pos)                 /*!< BPWM_T::CAPCTL: CAPINV2 Mask               */

#define BPWM_CAPCTL_CAPINV3_Pos          (11)                                               /*!< BPWM_T::CAPCTL: CAPINV3 Position           */
#define BPWM_CAPCTL_CAPINV3_Msk          (0x1ul << BPWM_CAPCTL_CAPINV3_Pos)                 /*!< BPWM_T::CAPCTL: CAPINV3 Mask               */

#define BPWM_CAPCTL_CAPINV4_Pos          (12)                                               /*!< BPWM_T::CAPCTL: CAPINV4 Position           */
#define BPWM_CAPCTL_CAPINV4_Msk          (0x1ul << BPWM_CAPCTL_CAPINV4_Pos)                 /*!< BPWM_T::CAPCTL: CAPINV4 Mask               */

#define BPWM_CAPCTL_CAPINV5_Pos          (13)                                               /*!< BPWM_T::CAPCTL: CAPINV5 Position           */
#define BPWM_CAPCTL_CAPINV5_Msk          (0x1ul << BPWM_CAPCTL_CAPINV5_Pos)                 /*!< BPWM_T::CAPCTL: CAPINV5 Mask               */

#define BPWM_CAPCTL_RCRLDENn_Pos         (16)                                               /*!< BPWM_T::CAPCTL: RCRLDENn Position          */
#define BPWM_CAPCTL_RCRLDENn_Msk         (0x3ful << BPWM_CAPCTL_RCRLDENn_Pos)               /*!< BPWM_T::CAPCTL: RCRLDENn Mask              */

#define BPWM_CAPCTL_RCRLDEN0_Pos         (16)                                               /*!< BPWM_T::CAPCTL: RCRLDEN0 Position          */
#define BPWM_CAPCTL_RCRLDEN0_Msk         (0x1ul << BPWM_CAPCTL_RCRLDEN0_Pos)                /*!< BPWM_T::CAPCTL: RCRLDEN0 Mask              */

#define BPWM_CAPCTL_RCRLDEN1_Pos         (17)                                               /*!< BPWM_T::CAPCTL: RCRLDEN1 Position          */
#define BPWM_CAPCTL_RCRLDEN1_Msk         (0x1ul << BPWM_CAPCTL_RCRLDEN1_Pos)                /*!< BPWM_T::CAPCTL: RCRLDEN1 Mask              */

#define BPWM_CAPCTL_RCRLDEN2_Pos         (18)                                               /*!< BPWM_T::CAPCTL: RCRLDEN2 Position          */
#define BPWM_CAPCTL_RCRLDEN2_Msk         (0x1ul << BPWM_CAPCTL_RCRLDEN2_Pos)                /*!< BPWM_T::CAPCTL: RCRLDEN2 Mask              */

#define BPWM_CAPCTL_RCRLDEN3_Pos         (19)                                               /*!< BPWM_T::CAPCTL: RCRLDEN3 Position          */
#define BPWM_CAPCTL_RCRLDEN3_Msk         (0x1ul << BPWM_CAPCTL_RCRLDEN3_Pos)                /*!< BPWM_T::CAPCTL: RCRLDEN3 Mask              */

#define BPWM_CAPCTL_RCRLDEN4_Pos         (20)                                               /*!< BPWM_T::CAPCTL: RCRLDEN4 Position          */
#define BPWM_CAPCTL_RCRLDEN4_Msk         (0x1ul << BPWM_CAPCTL_RCRLDEN4_Pos)                /*!< BPWM_T::CAPCTL: RCRLDEN4 Mask              */

#define BPWM_CAPCTL_RCRLDEN5_Pos         (21)                                               /*!< BPWM_T::CAPCTL: RCRLDEN5 Position          */
#define BPWM_CAPCTL_RCRLDEN5_Msk         (0x1ul << BPWM_CAPCTL_RCRLDEN5_Pos)                /*!< BPWM_T::CAPCTL: RCRLDEN5 Mask              */

#define BPWM_CAPCTL_FCRLDENn_Pos         (24)                                               /*!< BPWM_T::CAPCTL: FCRLDENn Position          */
#define BPWM_CAPCTL_FCRLDENn_Msk         (0x3ful << BPWM_CAPCTL_FCRLDENn_Pos)               /*!< BPWM_T::CAPCTL: FCRLDENn Mask              */

#define BPWM_CAPCTL_FCRLDEN0_Pos         (24)                                               /*!< BPWM_T::CAPCTL: FCRLDEN0 Position          */
#define BPWM_CAPCTL_FCRLDEN0_Msk         (0x1ul << BPWM_CAPCTL_FCRLDEN0_Pos)                /*!< BPWM_T::CAPCTL: FCRLDEN0 Mask              */

#define BPWM_CAPCTL_FCRLDEN1_Pos         (25)                                               /*!< BPWM_T::CAPCTL: FCRLDEN1 Position          */
#define BPWM_CAPCTL_FCRLDEN1_Msk         (0x1ul << BPWM_CAPCTL_FCRLDEN1_Pos)                /*!< BPWM_T::CAPCTL: FCRLDEN1 Mask              */

#define BPWM_CAPCTL_FCRLDEN2_Pos         (26)                                               /*!< BPWM_T::CAPCTL: FCRLDEN2 Position          */
#define BPWM_CAPCTL_FCRLDEN2_Msk         (0x1ul << BPWM_CAPCTL_FCRLDEN2_Pos)                /*!< BPWM_T::CAPCTL: FCRLDEN2 Mask              */

#define BPWM_CAPCTL_FCRLDEN3_Pos         (27)                                               /*!< BPWM_T::CAPCTL: FCRLDEN3 Position          */
#define BPWM_CAPCTL_FCRLDEN3_Msk         (0x1ul << BPWM_CAPCTL_FCRLDEN3_Pos)                /*!< BPWM_T::CAPCTL: FCRLDEN3 Mask              */

#define BPWM_CAPCTL_FCRLDEN4_Pos         (28)                                               /*!< BPWM_T::CAPCTL: FCRLDEN4 Position          */
#define BPWM_CAPCTL_FCRLDEN4_Msk         (0x1ul << BPWM_CAPCTL_FCRLDEN4_Pos)                /*!< BPWM_T::CAPCTL: FCRLDEN4 Mask              */

#define BPWM_CAPCTL_FCRLDEN5_Pos         (29)                                               /*!< BPWM_T::CAPCTL: FCRLDEN5 Position          */
#define BPWM_CAPCTL_FCRLDEN5_Msk         (0x1ul << BPWM_CAPCTL_FCRLDEN5_Pos)                /*!< BPWM_T::CAPCTL: FCRLDEN5 Mask              */

#define BPWM_CAPSTS_CRLIFOVn_Pos         (0)                                                /*!< BPWM_T::CAPSTS: CRLIFOVn Position          */
#define BPWM_CAPSTS_CRLIFOVn_Msk         (0x3ful << BPWM_CAPSTS_CRLIFOVn_Pos)               /*!< BPWM_T::CAPSTS: CRLIFOVn Mask              */

#define BPWM_CAPSTS_CRLIFOV0_Pos         (0)                                                /*!< BPWM_T::CAPSTS: CRLIFOV0 Position          */
#define BPWM_CAPSTS_CRLIFOV0_Msk         (0x1ul << BPWM_CAPSTS_CRLIFOV0_Pos)                /*!< BPWM_T::CAPSTS: CRLIFOV0 Mask              */

#define BPWM_CAPSTS_CRLIFOV1_Pos         (1)                                                /*!< BPWM_T::CAPSTS: CRLIFOV1 Position          */
#define BPWM_CAPSTS_CRLIFOV1_Msk         (0x1ul << BPWM_CAPSTS_CRLIFOV1_Pos)                /*!< BPWM_T::CAPSTS: CRLIFOV1 Mask              */

#define BPWM_CAPSTS_CRLIFOV2_Pos         (2)                                                /*!< BPWM_T::CAPSTS: CRLIFOV2 Position          */
#define BPWM_CAPSTS_CRLIFOV2_Msk         (0x1ul << BPWM_CAPSTS_CRLIFOV2_Pos)                /*!< BPWM_T::CAPSTS: CRLIFOV2 Mask              */

#define BPWM_CAPSTS_CRLIFOV3_Pos         (3)                                                /*!< BPWM_T::CAPSTS: CRLIFOV3 Position          */
#define BPWM_CAPSTS_CRLIFOV3_Msk         (0x1ul << BPWM_CAPSTS_CRLIFOV3_Pos)                /*!< BPWM_T::CAPSTS: CRLIFOV3 Mask              */

#define BPWM_CAPSTS_CRLIFOV4_Pos         (4)                                                /*!< BPWM_T::CAPSTS: CRLIFOV4 Position          */
#define BPWM_CAPSTS_CRLIFOV4_Msk         (0x1ul << BPWM_CAPSTS_CRLIFOV4_Pos)                /*!< BPWM_T::CAPSTS: CRLIFOV4 Mask              */

#define BPWM_CAPSTS_CRLIFOV5_Pos         (5)                                                /*!< BPWM_T::CAPSTS: CRLIFOV5 Position          */
#define BPWM_CAPSTS_CRLIFOV5_Msk         (0x1ul << BPWM_CAPSTS_CRLIFOV5_Pos)                /*!< BPWM_T::CAPSTS: CRLIFOV5 Mask              */

#define BPWM_CAPSTS_CFLIFOVn_Pos         (8)                                                /*!< BPWM_T::CAPSTS: CFLIFOVn Position          */
#define BPWM_CAPSTS_CFLIFOVn_Msk         (0x3ful << BPWM_CAPSTS_CFLIFOVn_Pos)               /*!< BPWM_T::CAPSTS: CFLIFOVn Mask              */

#define BPWM_CAPSTS_CFLIFOV0_Pos         (8)                                                /*!< BPWM_T::CAPSTS: CFLIFOV0 Position          */
#define BPWM_CAPSTS_CFLIFOV0_Msk         (0x1ul << BPWM_CAPSTS_CFLIFOV0_Pos)                /*!< BPWM_T::CAPSTS: CFLIFOV0 Mask              */

#define BPWM_CAPSTS_CFLIFOV1_Pos         (9)                                                /*!< BPWM_T::CAPSTS: CFLIFOV1 Position          */
#define BPWM_CAPSTS_CFLIFOV1_Msk         (0x1ul << BPWM_CAPSTS_CFLIFOV1_Pos)                /*!< BPWM_T::CAPSTS: CFLIFOV1 Mask              */

#define BPWM_CAPSTS_CFLIFOV2_Pos         (10)                                               /*!< BPWM_T::CAPSTS: CFLIFOV2 Position          */
#define BPWM_CAPSTS_CFLIFOV2_Msk         (0x1ul << BPWM_CAPSTS_CFLIFOV2_Pos)                /*!< BPWM_T::CAPSTS: CFLIFOV2 Mask              */

#define BPWM_CAPSTS_CFLIFOV3_Pos         (11)                                               /*!< BPWM_T::CAPSTS: CFLIFOV3 Position          */
#define BPWM_CAPSTS_CFLIFOV3_Msk         (0x1ul << BPWM_CAPSTS_CFLIFOV3_Pos)                /*!< BPWM_T::CAPSTS: CFLIFOV3 Mask              */

#define BPWM_CAPSTS_CFLIFOV4_Pos         (12)                                               /*!< BPWM_T::CAPSTS: CFLIFOV4 Position          */
#define BPWM_CAPSTS_CFLIFOV4_Msk         (0x1ul << BPWM_CAPSTS_CFLIFOV4_Pos)                /*!< BPWM_T::CAPSTS: CFLIFOV4 Mask              */

#define BPWM_CAPSTS_CFLIFOV5_Pos         (13)                                               /*!< BPWM_T::CAPSTS: CFLIFOV5 Position          */
#define BPWM_CAPSTS_CFLIFOV5_Msk         (0x1ul << BPWM_CAPSTS_CFLIFOV5_Pos)                /*!< BPWM_T::CAPSTS: CFLIFOV5 Mask              */

#define BPWM_RCAPDAT0_RCAPDAT_Pos        (0)                                                /*!< BPWM_T::RCAPDAT0: RCAPDAT Position         */
#define BPWM_RCAPDAT0_RCAPDAT_Msk        (0xfffful << BPWM_RCAPDAT0_RCAPDAT_Pos)            /*!< BPWM_T::RCAPDAT0: RCAPDAT Mask             */

#define BPWM_FCAPDAT0_FCAPDAT_Pos        (0)                                                /*!< BPWM_T::FCAPDAT0: FCAPDAT Position         */
#define BPWM_FCAPDAT0_FCAPDAT_Msk        (0xfffful << BPWM_FCAPDAT0_FCAPDAT_Pos)            /*!< BPWM_T::FCAPDAT0: FCAPDAT Mask             */

#define BPWM_RCAPDAT1_RCAPDAT_Pos        (0)                                                /*!< BPWM_T::RCAPDAT1: RCAPDAT Position         */
#define BPWM_RCAPDAT1_RCAPDAT_Msk        (0xfffful << BPWM_RCAPDAT1_RCAPDAT_Pos)            /*!< BPWM_T::RCAPDAT1: RCAPDAT Mask             */

#define BPWM_FCAPDAT1_FCAPDAT_Pos        (0)                                                /*!< BPWM_T::FCAPDAT1: FCAPDAT Position         */
#define BPWM_FCAPDAT1_FCAPDAT_Msk        (0xfffful << BPWM_FCAPDAT1_FCAPDAT_Pos)            /*!< BPWM_T::FCAPDAT1: FCAPDAT Mask             */

#define BPWM_RCAPDAT2_RCAPDAT_Pos        (0)                                                /*!< BPWM_T::RCAPDAT2: RCAPDAT Position         */
#define BPWM_RCAPDAT2_RCAPDAT_Msk        (0xfffful << BPWM_RCAPDAT2_RCAPDAT_Pos)            /*!< BPWM_T::RCAPDAT2: RCAPDAT Mask             */

#define BPWM_FCAPDAT2_FCAPDAT_Pos        (0)                                                /*!< BPWM_T::FCAPDAT2: FCAPDAT Position         */
#define BPWM_FCAPDAT2_FCAPDAT_Msk        (0xfffful << BPWM_FCAPDAT2_FCAPDAT_Pos)            /*!< BPWM_T::FCAPDAT2: FCAPDAT Mask             */

#define BPWM_RCAPDAT3_RCAPDAT_Pos        (0)                                                /*!< BPWM_T::RCAPDAT3: RCAPDAT Position         */
#define BPWM_RCAPDAT3_RCAPDAT_Msk        (0xfffful << BPWM_RCAPDAT3_RCAPDAT_Pos)            /*!< BPWM_T::RCAPDAT3: RCAPDAT Mask             */

#define BPWM_FCAPDAT3_FCAPDAT_Pos        (0)                                                /*!< BPWM_T::FCAPDAT3: FCAPDAT Position         */
#define BPWM_FCAPDAT3_FCAPDAT_Msk        (0xfffful << BPWM_FCAPDAT3_FCAPDAT_Pos)            /*!< BPWM_T::FCAPDAT3: FCAPDAT Mask             */

#define BPWM_RCAPDAT4_RCAPDAT_Pos        (0)                                                /*!< BPWM_T::RCAPDAT4: RCAPDAT Position         */
#define BPWM_RCAPDAT4_RCAPDAT_Msk        (0xfffful << BPWM_RCAPDAT4_RCAPDAT_Pos)            /*!< BPWM_T::RCAPDAT4: RCAPDAT Mask             */

#define BPWM_FCAPDAT4_FCAPDAT_Pos        (0)                                                /*!< BPWM_T::FCAPDAT4: FCAPDAT Position         */
#define BPWM_FCAPDAT4_FCAPDAT_Msk        (0xfffful << BPWM_FCAPDAT4_FCAPDAT_Pos)            /*!< BPWM_T::FCAPDAT4: FCAPDAT Mask             */

#define BPWM_RCAPDAT5_RCAPDAT_Pos        (0)                                                /*!< BPWM_T::RCAPDAT5: RCAPDAT Position         */
#define BPWM_RCAPDAT5_RCAPDAT_Msk        (0xfffful << BPWM_RCAPDAT5_RCAPDAT_Pos)            /*!< BPWM_T::RCAPDAT5: RCAPDAT Mask             */

#define BPWM_FCAPDAT5_FCAPDAT_Pos        (0)                                                /*!< BPWM_T::FCAPDAT5: FCAPDAT Position         */
#define BPWM_FCAPDAT5_FCAPDAT_Msk        (0xfffful << BPWM_FCAPDAT5_FCAPDAT_Pos)            /*!< BPWM_T::FCAPDAT5: FCAPDAT Mask             */

#define BPWM_CAPIEN_CAPRIENn_Pos         (0)                                                /*!< BPWM_T::CAPIEN: CAPRIENn Position          */
#define BPWM_CAPIEN_CAPRIENn_Msk         (0x3ful << BPWM_CAPIEN_CAPRIENn_Pos)               /*!< BPWM_T::CAPIEN: CAPRIENn Mask              */

#define BPWM_CAPIEN_CAPRIEN0_Pos         (0)                                                /*!< BPWM_T::CAPIEN: CAPRIEN0 Position          */
#define BPWM_CAPIEN_CAPRIEN0_Msk         (0x1ul << BPWM_CAPIEN_CAPRIEN0_Pos)                /*!< BPWM_T::CAPIEN: CAPRIEN0 Mask              */

#define BPWM_CAPIEN_CAPRIEN1_Pos         (1)                                                /*!< BPWM_T::CAPIEN: CAPRIEN1 Position          */
#define BPWM_CAPIEN_CAPRIEN1_Msk         (0x1ul << BPWM_CAPIEN_CAPRIEN1_Pos)                /*!< BPWM_T::CAPIEN: CAPRIEN1 Mask              */

#define BPWM_CAPIEN_CAPRIEN2_Pos         (2)                                                /*!< BPWM_T::CAPIEN: CAPRIEN2 Position          */
#define BPWM_CAPIEN_CAPRIEN2_Msk         (0x1ul << BPWM_CAPIEN_CAPRIEN2_Pos)                /*!< BPWM_T::CAPIEN: CAPRIEN2 Mask              */

#define BPWM_CAPIEN_CAPRIEN3_Pos         (3)                                                /*!< BPWM_T::CAPIEN: CAPRIEN3 Position          */
#define BPWM_CAPIEN_CAPRIEN3_Msk         (0x1ul << BPWM_CAPIEN_CAPRIEN3_Pos)                /*!< BPWM_T::CAPIEN: CAPRIEN3 Mask              */

#define BPWM_CAPIEN_CAPRIEN4_Pos         (4)                                                /*!< BPWM_T::CAPIEN: CAPRIEN4 Position          */
#define BPWM_CAPIEN_CAPRIEN4_Msk         (0x1ul << BPWM_CAPIEN_CAPRIEN4_Pos)                /*!< BPWM_T::CAPIEN: CAPRIEN4 Mask              */

#define BPWM_CAPIEN_CAPRIEN5_Pos         (5)                                                /*!< BPWM_T::CAPIEN: CAPRIEN5 Position          */
#define BPWM_CAPIEN_CAPRIEN5_Msk         (0x1ul << BPWM_CAPIEN_CAPRIEN5_Pos)                /*!< BPWM_T::CAPIEN: CAPRIEN5 Mask              */

#define BPWM_CAPIEN_CAPFIENn_Pos         (8)                                                /*!< BPWM_T::CAPIEN: CAPFIENn Position          */
#define BPWM_CAPIEN_CAPFIENn_Msk         (0x3ful << BPWM_CAPIEN_CAPFIENn_Pos)               /*!< BPWM_T::CAPIEN: CAPFIENn Mask              */

#define BPWM_CAPIEN_CAPFIEN0_Pos         (8)                                                /*!< BPWM_T::CAPIEN: CAPFIEN0 Position          */
#define BPWM_CAPIEN_CAPFIEN0_Msk         (0x1ul << BPWM_CAPIEN_CAPFIEN0_Pos)                /*!< BPWM_T::CAPIEN: CAPFIEN0 Mask              */

#define BPWM_CAPIEN_CAPFIEN1_Pos         (9)                                                /*!< BPWM_T::CAPIEN: CAPFIEN1 Position          */
#define BPWM_CAPIEN_CAPFIEN1_Msk         (0x1ul << BPWM_CAPIEN_CAPFIEN1_Pos)                /*!< BPWM_T::CAPIEN: CAPFIEN1 Mask              */

#define BPWM_CAPIEN_CAPFIEN2_Pos         (10)                                               /*!< BPWM_T::CAPIEN: CAPFIEN2 Position          */
#define BPWM_CAPIEN_CAPFIEN2_Msk         (0x1ul << BPWM_CAPIEN_CAPFIEN2_Pos)                /*!< BPWM_T::CAPIEN: CAPFIEN2 Mask              */

#define BPWM_CAPIEN_CAPFIEN3_Pos         (11)                                               /*!< BPWM_T::CAPIEN: CAPFIEN3 Position          */
#define BPWM_CAPIEN_CAPFIEN3_Msk         (0x1ul << BPWM_CAPIEN_CAPFIEN3_Pos)                /*!< BPWM_T::CAPIEN: CAPFIEN3 Mask              */

#define BPWM_CAPIEN_CAPFIEN4_Pos         (12)                                               /*!< BPWM_T::CAPIEN: CAPFIEN4 Position          */
#define BPWM_CAPIEN_CAPFIEN4_Msk         (0x1ul << BPWM_CAPIEN_CAPFIEN4_Pos)                /*!< BPWM_T::CAPIEN: CAPFIEN4 Mask              */

#define BPWM_CAPIEN_CAPFIEN5_Pos         (13)                                               /*!< BPWM_T::CAPIEN: CAPFIEN5 Position          */
#define BPWM_CAPIEN_CAPFIEN5_Msk         (0x1ul << BPWM_CAPIEN_CAPFIEN5_Pos)                /*!< BPWM_T::CAPIEN: CAPFIEN5 Mask              */

#define BPWM_CAPIF_CRLIFn_Pos            (0)                                                /*!< BPWM_T::CAPIF: CRLIFn Position             */
#define BPWM_CAPIF_CRLIFn_Msk            (0x3ful << BPWM_CAPIF_CRLIFn_Pos)                  /*!< BPWM_T::CAPIF: CRLIFn Mask                 */

#define BPWM_CAPIF_CRLIF0_Pos            (0)                                                /*!< BPWM_T::CAPIF: CRLIF0 Position             */
#define BPWM_CAPIF_CRLIF0_Msk            (0x1ul << BPWM_CAPIF_CRLIF0_Pos)                   /*!< BPWM_T::CAPIF: CRLIF0 Mask                 */

#define BPWM_CAPIF_CRLIF1_Pos            (1)                                                /*!< BPWM_T::CAPIF: CRLIF1 Position             */
#define BPWM_CAPIF_CRLIF1_Msk            (0x1ul << BPWM_CAPIF_CRLIF1_Pos)                   /*!< BPWM_T::CAPIF: CRLIF1 Mask                 */

#define BPWM_CAPIF_CRLIF2_Pos            (2)                                                /*!< BPWM_T::CAPIF: CRLIF2 Position             */
#define BPWM_CAPIF_CRLIF2_Msk            (0x1ul << BPWM_CAPIF_CRLIF2_Pos)                   /*!< BPWM_T::CAPIF: CRLIF2 Mask                 */

#define BPWM_CAPIF_CRLIF3_Pos            (3)                                                /*!< BPWM_T::CAPIF: CRLIF3 Position             */
#define BPWM_CAPIF_CRLIF3_Msk            (0x1ul << BPWM_CAPIF_CRLIF3_Pos)                   /*!< BPWM_T::CAPIF: CRLIF3 Mask                 */

#define BPWM_CAPIF_CRLIF4_Pos            (4)                                                /*!< BPWM_T::CAPIF: CRLIF4 Position             */
#define BPWM_CAPIF_CRLIF4_Msk            (0x1ul << BPWM_CAPIF_CRLIF4_Pos)                   /*!< BPWM_T::CAPIF: CRLIF4 Mask                 */

#define BPWM_CAPIF_CRLIF5_Pos            (5)                                                /*!< BPWM_T::CAPIF: CRLIF5 Position             */
#define BPWM_CAPIF_CRLIF5_Msk            (0x1ul << BPWM_CAPIF_CRLIF5_Pos)                   /*!< BPWM_T::CAPIF: CRLIF5 Mask                 */

#define BPWM_CAPIF_CFLIFn_Pos            (8)                                                /*!< BPWM_T::CAPIF: CFLIFn Position             */
#define BPWM_CAPIF_CFLIFn_Msk            (0x3ful << BPWM_CAPIF_CFLIFn_Pos)                  /*!< BPWM_T::CAPIF: CFLIFn Mask                 */

#define BPWM_CAPIF_CFLIF0_Pos            (8)                                                /*!< BPWM_T::CAPIF: CFLIF0 Position             */
#define BPWM_CAPIF_CFLIF0_Msk            (0x1ul << BPWM_CAPIF_CFLIF0_Pos)                   /*!< BPWM_T::CAPIF: CFLIF0 Mask                 */

#define BPWM_CAPIF_CFLIF1_Pos            (9)                                                /*!< BPWM_T::CAPIF: CFLIF1 Position             */
#define BPWM_CAPIF_CFLIF1_Msk            (0x1ul << BPWM_CAPIF_CFLIF1_Pos)                   /*!< BPWM_T::CAPIF: CFLIF1 Mask                 */

#define BPWM_CAPIF_CFLIF2_Pos            (10)                                               /*!< BPWM_T::CAPIF: CFLIF2 Position             */
#define BPWM_CAPIF_CFLIF2_Msk            (0x1ul << BPWM_CAPIF_CFLIF2_Pos)                   /*!< BPWM_T::CAPIF: CFLIF2 Mask                 */

#define BPWM_CAPIF_CFLIF3_Pos            (11)                                               /*!< BPWM_T::CAPIF: CFLIF3 Position             */
#define BPWM_CAPIF_CFLIF3_Msk            (0x1ul << BPWM_CAPIF_CFLIF3_Pos)                   /*!< BPWM_T::CAPIF: CFLIF3 Mask                 */

#define BPWM_CAPIF_CFLIF4_Pos            (12)                                               /*!< BPWM_T::CAPIF: CFLIF4 Position             */
#define BPWM_CAPIF_CFLIF4_Msk            (0x1ul << BPWM_CAPIF_CFLIF4_Pos)                   /*!< BPWM_T::CAPIF: CFLIF4 Mask                 */

#define BPWM_CAPIF_CFLIF5_Pos            (13)                                               /*!< BPWM_T::CAPIF: CFLIF5 Position             */
#define BPWM_CAPIF_CFLIF5_Msk            (0x1ul << BPWM_CAPIF_CFLIF5_Pos)                   /*!< BPWM_T::CAPIF: CFLIF5 Mask                 */

#define BPWM_PBUF_PBUF_Pos               (0)                                                /*!< BPWM_T::PBUF: PBUF Position                */
#define BPWM_PBUF_PBUF_Msk               (0xfffful << BPWM_PBUF_PBUF_Pos)                   /*!< BPWM_T::PBUF: PBUF Mask                    */

#define BPWM_CMPBUF_CMPBUF_Pos           (0)                                                /*!< BPWM_T::CMPBUF: CMPBUF Position            */
#define BPWM_CMPBUF_CMPBUF_Msk           (0xfffful << BPWM_CMPBUF_CMPBUF_Pos)               /*!< BPWM_T::CMPBUF: CMPBUF Mask                */

/*@}*/ /* end of group BPWM_CONST */
/*@}*/ /* end of group BPWM */


/*----------------------------- CAN Controller ------------------------------*/
/** @addtogroup NUC131_CAN Controller Area Network (CAN)
  Register Structure for CAN Device Controller
  @{
 */

typedef struct
{

    /**
     * CAN_IFn_CREQ
     * ===================================================================================================
     * Offset: 0x20, 0x80  IFn (Register Map Note 2) Command Request Registers
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |MessageNumber|Message Number
     * |        |          |0x01-0x20: Valid Message Number, the Message Object in the Message
     * |        |          |RAM is selected for data transfer.
     * |        |          |0x00: Not a valid Message Number, interpreted as 0x20.
     * |        |          |0x21-0x3F: Not a valid Message Number, interpreted as 0x01-0x1F.
     * |[15]    |Busy      |Busy Flag
     * |        |          |0 = Read/write action has finished.
     * |        |          |1 = Writing to the IFn Command Request Register is in progress.
     * |        |          |This bit can only be read by the software.
     */
    __IO uint32_t CREQ;

    /**
     * CAN_IFn_CMASK
     * ===================================================================================================
     * Offset: 0x24, 0x84  IFn Command Mask Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |DAT_B     |Access Data Bytes [7:4]
     * |        |          |Write Operation:
     * |        |          |0 = Data Bytes [7:4] unchanged.
     * |        |          |1 = Transfer Data Bytes [7:4] to Message Object.
     * |        |          |Read Operation:
     * |        |          |0 = Data Bytes [7:4] unchanged.
     * |        |          |1 = Transfer Data Bytes [7:4] to IFn Message Buffer Register.
     * |[1]     |DAT_A     |Access Data Bytes [3:0]
     * |        |          |Write Operation:
     * |        |          |0 = Data Bytes [3:0] unchanged.
     * |        |          |1 = Transfer Data Bytes [3:0] to Message Object.
     * |        |          |Read Operation:
     * |        |          |0 = Data Bytes [3:0] unchanged.
     * |        |          |1 = Transfer Data Bytes [3:0] to IFn Message Buffer Register.
     * |[2]     |TxRqst_NewDat|Access Transmission Request Bit When Write Operation
     * |        |          |0 = TxRqst bit unchanged.
     * |        |          |1 = Set TxRqst bit.
     * |        |          |Note: If a transmission is requested by programming bit TxRqst/NewDat in the IFn Command Mask Register, bit TxRqst in the IFn Message Control Register will be ignored.
     * |        |          |Access New Data Bit when Read Operation.
     * |        |          |0 = NewDat bit remains unchanged.
     * |        |          |1 = Clear NewDat bit in the Message Object.
     * |        |          |Note: A read access to a Message Object can be combined with the reset of the control bits IntPnd and NewDat.
     * |        |          |The values of these bits transferred to the IFn Message Control Register always reflect the status before resetting these bits.
     * |[3]     |ClrIntPnd |Clear Interrupt Pending Bit
     * |        |          |Write Operation:
     * |        |          |When writing to a Message Object, this bit is ignored.
     * |        |          |Read Operation:
     * |        |          |0 = IntPnd bit (CAN_IFn_MCON[13]) remains unchanged.
     * |        |          |1 = Clear IntPnd bit in the Message Object.
     * |[4]     |Control   |Control Access Control Bits
     * |        |          |Write Operation:
     * |        |          |0 = Control Bits unchanged.
     * |        |          |1 = Transfer Control Bits to Message Object.
     * |        |          |Read Operation:
     * |        |          |0 = Control Bits unchanged.
     * |        |          |1 = Transfer Control Bits to IFn Message Buffer Register.
     * |[5]     |Arb       |Access Arbitration Bits
     * |        |          |Write Operation:
     * |        |          |0 = Arbitration bits unchanged.
     * |        |          |1 = Transfer Identifier + Dir (CAN_IFn_ARB2[13]) + Xtd (CAN_IFn_ARB2[14]) + MsgVal (CAN_IFn_APB2[15]) to Message Object.
     * |        |          |Read Operation:
     * |        |          |0 = Arbitration bits unchanged.
     * |        |          |1 = Transfer Identifier + Dir + Xtd + MsgVal to IFn Message Buffer Register.
     * |[6]     |Mask      |Access Mask Bits
     * |        |          |Write Operation:
     * |        |          |0 = Mask bits unchanged.
     * |        |          |1 = Transfer Identifier Mask + MDir + MXtd to Message Object.
     * |        |          |Read Operation:
     * |        |          |0 = Mask bits unchanged.
     * |        |          |1 = Transfer Identifier Mask + MDir + MXtd to IFn Message Buffer Register.
     * |[7]     |WR_RD     |Write / Read Mode
     * |        |          |0 = Read: Transfer data from the Message Object addressed by the Command Request Register into the selected Message Buffer Registers.
     * |        |          |1 = Write: Transfer data from the selected Message Buffer Registers to the Message Object addressed by the Command Request Register.
     */
    __IO uint32_t CMASK;

    /**
     * CAN_IFn_MASK1
     * ===================================================================================================
     * Offset: 0x28, 0x88  IFn Mask 1 Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |Msk[15:0] |Identifier Mask 15-0
     * |        |          |0 = The corresponding bit in the identifier of the message object cannot inhibit the match in the acceptance filtering.
     * |        |          |1 = The corresponding identifier bit is used for acceptance filtering.
     */
    __IO uint32_t MASK1;

    /**
     * CAN_IFn_MASK2
     * ===================================================================================================
     * Offset: 0x2C, 0x8C  IFn Mask 2 Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[12:0]  |Msk[28:16]|Identifier Mask 28-16
     * |        |          |0 = The corresponding bit in the identifier of the message object cannot inhibit the match in the acceptance filtering.
     * |        |          |1 = The corresponding identifier bit is used for acceptance filtering.
     * |[14]    |MDir      |Mask Message Direction
     * |        |          |0 = The message direction bit (Dir (CAN_IFn_ARB2[13])) has no effect on the acceptance filtering.
     * |        |          |1 = The message direction bit (Dir) is used for acceptance filtering.
     * |[15]    |MXtd      |Mask Extended Identifier
     * |        |          |0 = The extended identifier bit (IDE) has no effect on the acceptance filtering.
     * |        |          |1 = The extended identifier bit (IDE) is used for acceptance filtering.
     * |        |          |Note: When 11-bit ("standard") Identifiers are used for a Message Object, the identifiers of received Data Frames are written into bits ID28 to ID18 (CAN_IFn_ARB2[12:2]).
     * |        |          |For acceptance filtering, only these bits together with mask bits Msk28 to Msk18 (CAN_IFn_MASK2[12:2]) are considered.
     */
    __IO uint32_t MASK2;

    /**
     * CAN_IFn_ARB1
     * ===================================================================================================
     * Offset: 0x30, 0x90  IFn Arbitration 1 Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |ID[15:0]  |Message Identifier 15-0
     * |        |          |ID28 - ID0, 29-bit Identifier ("Extended Frame").
     * |        |          |ID28 - ID18, 11-bit Identifier ("Standard Frame")
     */
    __IO uint32_t ARB1;

    /**
     * CAN_IFn_ARB2
     * ===================================================================================================
     * Offset: 0x34, 0x94  IFn Arbitration 2 Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[12:0]  |ID[28:16] |Message Identifier 28-16
     * |        |          |ID28 - ID0, 29-bit Identifier ("Extended Frame").
     * |        |          |ID28 - ID18, 11-bit Identifier ("Standard Frame")
     * |[13]    |Dir       |Message Direction
     * |        |          |0 = Direction is receive.
     * |        |          |On TxRqst, a Remote Frame with the identifier of this Message Object is transmitted.
     * |        |          |On reception of a Data Frame with matching identifier, that message is stored in this Message Object.
     * |        |          |1 = Direction is transmit.
     * |        |          |On TxRqst, the respective Message Object is transmitted as a Data Frame.
     * |        |          |On reception of a Remote Frame with matching identifier, the TxRqst bit (CAN_IFn_CMASK[2]) of this Message Object is set (if RmtEn (CAN_IFn_MCON[9]) = one).
     * |[14]    |Xtd       |Extended Identifier
     * |        |          |0 = The 11-bit ("standard") Identifier will be used for this Message Object.
     * |        |          |1 = The 29-bit ("extended") Identifier will be used for this Message Object.
     * |[15]    |MsgVal    |Message Valid
     * |        |          |0 = The Message Object is ignored by the Message Handler.
     * |        |          |1 = The Message Object is configured and should be considered by the Message Handler.
     * |        |          |Note: The application software must reset the MsgVal bit of all unused Messages Objects during the initialization before it resets bit Init (CAN_CON[0]).
     * |        |          |This bit must also be reset before the identifier Id28-0 (CAN_IFn_ARB1/2), the control bits Xtd (CAN_IFn_ARB2[14]), Dir (CAN_IFn_APB2[13]), or the Data Length Code DLC3-0 (CAN_IFn_MCON[3:0]) are modified, or if the Messages Object is no longer required.
     */
    __IO uint32_t ARB2;

    /**
     * CAN_IFn_MCON
     * ===================================================================================================
     * Offset: 0x38, 0x98  IFn Message Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |DLC       |Data Length Code
     * |        |          |0-8: Data Frame has 0-8 data bytes.
     * |        |          |9-15: Data Frame has 8 data bytes
     * |        |          |Note: The Data Length Code of a Message Object must be defined the same as in all the corresponding objects with the same identifier at other nodes.
     * |        |          |When the Message Handler stores a data frame, it will write the DLC to the value given by the received message.
     * |        |          |Data 0: 1st data byte of a CAN Data Frame
     * |        |          |Data 1: 2nd data byte of a CAN Data Frame
     * |        |          |Data 2: 3rd data byte of a CAN Data Frame
     * |        |          |Data 3: 4th data byte of a CAN Data Frame
     * |        |          |Data 4: 5th data byte of a CAN Data Frame
     * |        |          |Data 5: 6th data byte of a CAN Data Frame
     * |        |          |Data 6: 7th data byte of a CAN Data Frame
     * |        |          |Data 7 : 8th data byte of a CAN Data Frame
     * |        |          |Note: The Data 0 Byte is the first data byte shifted into the shift register of the CAN Core during a reception while the Data 7 byte is the last.
     * |        |          |When the Message Handler stores a Data Frame, it will write all the eight data bytes into a Message Object.
     * |        |          |If the Data Length Code is less than 8, the remaining bytes of the Message Object will be overwritten by unspecified values.
     * |[7]     |EoB       |End Of Buffer
     * |        |          |0 = Message Object belongs to a FIFO Buffer and is not the last Message Object of that FIFO Buffer.
     * |        |          |1 = Single Message Object or last Message Object of a FIFO Buffer.
     * |        |          |Note: This bit is used to concatenate two or more Message Objects (up to 32) to build a FIFO Buffer.
     * |        |          |For single Message Objects (not belonging to a FIFO Buffer), this bit must always be set to one.
     * |[8]     |TxRqst    |Transmit Request
     * |        |          |0 = This Message Object is not waiting for transmission.
     * |        |          |1 = The transmission of this Message Object is requested and is not yet done.
     * |[9]     |RmtEn     |Remote Enable Control
     * |        |          |0 = At the reception of a Remote Frame, TxRqst (CAN_IFn_MCON[8]) is left unchanged.
     * |        |          |1 = At the reception of a Remote Frame, TxRqst is set.
     * |[10]    |RxIE      |Receive Interrupt Enable Control
     * |        |          |0 = IntPnd (CAN_IFn_MCON[13]) will be left unchanged after a successful reception of a frame.
     * |        |          |1 = IntPnd will be set after a successful reception of a frame.
     * |[11]    |TxIE      |Transmit Interrupt Enable Control
     * |        |          |0 = IntPnd (CAN_IFn_MCON[13]) will be left unchanged after the successful transmission of a frame.
     * |        |          |1 = IntPnd will be set after a successful transmission of a frame.
     * |[12]    |UMask     |Use Acceptance Mask
     * |        |          |0 = Mask ignored.
     * |        |          |1 = Use Mask (Msk28-0, MXtd, and MDir) for acceptance filtering.
     * |        |          |Note: If the UMask bit is set to one, the Message Object's mask bits have to be programmed during initialization of the Message Object before MsgVal bit (CAN_IFn_APB2[15]) is set to one.
     * |[13]    |IntPnd    |Interrupt Pending
     * |        |          |0 = This message object is not the source of an interrupt.
     * |        |          |1 = This message object is the source of an interrupt.
     * |        |          |The Interrupt Identifier in the Interrupt Register will point to this message object if there is no other interrupt source with higher priority.
     * |[14]    |MsgLst    |Message Lost (only valid for Message Objects with direction = receive).
     * |        |          |0 = No message lost since last time this bit was reset by the CPU.
     * |        |          |1 = The Message Handler stored a new message into this object when NewDat was still set, the CPU has lost a message.
     * |[15]    |NewDat    |New Data
     * |        |          |0 = No new data has been written into the data portion of this Message Object by the Message Handler since last time this flag was cleared by the application software.
     * |        |          |1 = The Message Handler or the application software has written new data into the data portion of this Message Object.
     */
    __IO uint32_t MCON;

    /**
     * CAN_IFn_DAT_A1
     * ===================================================================================================
     * Offset: 0x3C, 0x9C  IFn Data A1 Register (Register Map Note 3)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |Data0     |Data Byte 0
     * |        |          |1st data byte of a CAN Data Frame
     * |[15:8]  |Data1     |Data Byte 1
     * |        |          |2nd data byte of a CAN Data Frame
     */
    __IO uint32_t DAT_A1;

    /**
     * CAN_IFn_DAT_A2
     * ===================================================================================================
     * Offset: 0x40, 0xA0  IFn Data A2 Register (Register Map Note 3)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |Data2     |Data Byte 2
     * |        |          |3rd data byte of CAN Data Frame
     * |[15:8]  |Data3     |Data Byte 3
     * |        |          |4th data byte of CAN Data Frame
     */
    __IO uint32_t DAT_A2;

    /**
     * CAN_IFn_DAT_B1
     * ===================================================================================================
     * Offset: 0x44, 0xA4  IFn Data B1 Register (Register Map Note 3)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |Data4     |Data Byte 4
     * |        |          |5th data byte of CAN Data Frame
     * |[15:8]  |Data5     |Data Byte 5
     * |        |          |6th data byte of CAN Data Frame
     */
    __IO uint32_t DAT_B1;

    /**
     * CAN_IFn_DAT_B2
     * ===================================================================================================
     * Offset: 0x48, 0xA8  IFn Data B2 Register (Register Map Note 3)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |Data6     |Data Byte 6
     * |        |          |7th data byte of CAN Data Frame.
     * |[15:8]  |Data7     |Data Byte 7
     * |        |          |8th data byte of CAN Data Frame.
     */
    __IO uint32_t DAT_B2;

    /**
     * @cond HIDDEN_SYMBOLS     
     */    
    __I uint32_t RESERVE0[13];
    /**
     * @endcond
     */

} CAN_IF_T;

typedef struct
{

    /**
     * CAN_CON
     * ===================================================================================================
     * Offset: 0x00  Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |Init      |Init Initialization
     * |        |          |0 = Normal Operation.
     * |        |          |1 = Initialization is started.
     * |[1]     |IE        |Module Interrupt Enable Control
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |[2]     |SIE       |Status Change Interrupt Enable Control
     * |        |          |0 = Disabled - No Status Change Interrupt will be generated.
     * |        |          |1 = Enabled - An interrupt will be generated when a message transfer is successfully completed or a CAN bus error is detected.
     * |[3]     |EIE       |Error Interrupt Enable Control
     * |        |          |0 = Disabled - No Error Status Interrupt will be generated.
     * |        |          |1 = Enabled - A change in the bits BOff (CAN_STATUS[7]) or EWarn (CAN_STATUS[6]) in the Status Register will generate an interrupt.
     * |[5]     |DAR       |Automatic Re-Transmission Disable Control
     * |        |          |0 = Automatic Retransmission of disturbed messages enabled.
     * |        |          |1 = Automatic Retransmission disabled.
     * |[6]     |CCE       |Configuration Change Enable Control
     * |        |          |0 = No write access to the Bit Timing Register.
     * |        |          |1 = Write access to the Bit Timing Register (CAN_BTIME) allowed. (while Init bit (CAN_CON[0]) = 1).
     * |[7]     |Test      |Test Mode Enable Control
     * |        |          |0 = Normal Operation.
     * |        |          |1 = Test Mode.
     */
    __IO uint32_t   CON;

    /**
     * CAN_STATUS
     * ===================================================================================================
     * Offset: 0x04  Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2:0]   |LEC       |Last Error Code (Type Of The Last Error To Occur On The CAN Bus)
     * |        |          |The LEC field holds a code, which indicates the type of the last error to occur on the CAN bus.
     * |        |          |This field will be cleared to '0' when a message has been transferred (reception or transmission) without error.
     * |        |          |The unused code '7' may be written by the CPU to check for updates.
     * |        |          |The following table describes the error code.
     * |[3]     |TxOK      |Transmitted A Message Successfully
     * |        |          |0 = Since this bit was reset by the CPU, no message has been successfully transmitted.
     * |        |          |This bit is never reset by the CAN Core.
     * |        |          |1 = Since this bit was last reset by the CPU, a message has been successfully (error free and acknowledged by at least one other node) transmitted.
     * |[4]     |RxOK      |Received A Message Successfully
     * |        |          |0 = No message has been successfully received since this bit was last reset by the CPU.
     * |        |          |This bit is never reset by the CAN Core.
     * |        |          |1 = A message has been successfully received since this bit was last reset by the CPU (independent of the result of acceptance filtering).
     * |[5]     |EPass     |Error Passive (Read Only)
     * |        |          |0 = The CAN Core is error active.
     * |        |          |1 = The CAN Core is in the error passive state as defined in the CAN Specification.
     * |[6]     |EWarn     |Error Warning Status (Read Only)
     * |        |          |0 = Both error counters are below the error warning limit of 96.
     * |        |          |1 = At least one of the error counters in the EML has reached the error warning limit of 96.
     * |[7]     |BOff      |Bus-Off Status (Read Only)
     * |        |          |0 = The CAN module is not in bus-off state.
     * |        |          |1 = The CAN module is in bus-off state.
     */
    __IO uint32_t   STATUS;

    /**
     * CAN_ERR
     * ===================================================================================================
     * Offset: 0x08  Error Counter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |TEC       |Transmit Error Counter
     * |        |          |Actual state of the Transmit Error Counter. Values between 0 and 255.
     * |[14:8]  |REC       |Receive Error Counter
     * |        |          |Actual state of the Receive Error Counter. Values between 0 and 127.
     * |[15]    |RP        |Receive Error Passive
     * |        |          |0 = The Receive Error Counter is below the error passive level.
     * |        |          |1 = The Receive Error Counter has reached the error passive level as defined in the CAN Specification.
     */
    __IO uint32_t   ERR;

    /**
     * CAN_BTIME
     * ===================================================================================================
     * Offset: 0x0C  Bit Timing Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |BRP       |Baud Rate Prescaler
     * |        |          |0x01-0x3F: The value by which the oscillator frequency is divided for generating the bit time quanta.
     * |        |          |The bit time is built up from a multiple of this quanta.
     * |        |          |Valid values for the Baud Rate Prescaler are [ 0 ... 63 ].
     * |        |          |The actual interpretation by the hardware of this value is such that one more than the value programmed here is used.
     * |[7:6]   |SJW       |(Re)Synchronization Jump Width
     * |        |          |0x0-0x3: Valid programmed values are [0 ... 3].
     * |        |          |The actual interpretation by the hardware of this value is such that one more than the value programmed here is used.
     * |[11:8]  |TSeg1     |Time Segment Before The Sample Point Minus Sync_Seg
     * |        |          |0x01-0x0F: valid values for TSeg1 are [1 ... 15].
     * |        |          |The actual interpretation by the hardware of this value is such that one more than the value programmed is used.
     * |[14:12] |TSeg2     |Time Segment After Sample Point
     * |        |          |0x0-0x7: Valid values for TSeg2 are [0 ... 7].
     * |        |          |The actual interpretation by the hardware of this value is such that one more than the value programmed here is used.
     */
    __IO uint32_t   BTIME;

    /**
     * CAN_IIDR
     * ===================================================================================================
     * Offset: 0x10  Interrupt Identifier Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |IntId     |Interrupt Identifier (Indicates The Source Of The Interrupt)
     * |        |          |If several interrupts are pending, the CAN Interrupt Register will point to the pending interrupt with the highest priority, disregarding their chronological order.
     * |        |          |An interrupt remains pending until the application software has cleared it.
     * |        |          |If IntId is different from 0x0000 and IE (CAN_IFn_MCON[1]) is set, the IRQ interrupt signal to the EIC is active.
     * |        |          |The interrupt remains active until IntId is back to value 0x0000 (the cause of the interrupt is reset) or until IE is reset.
     * |        |          |The Status Interrupt has the highest priority.
     * |        |          |Among the message interrupts, the Message Object' s interrupt priority decreases with increasing message number.
     * |        |          |A message interrupt is cleared by clearing the Message Object's IntPnd bit (CAN_IFn_MCON[13]).
     * |        |          |The Status Interrupt is cleared by reading the Status Register.
     */
    __IO uint32_t   IIDR;

    /**
     * CAN_TEST
     * ===================================================================================================
     * Offset: 0x14  Test Register (Register Map Note 1)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |Res       |Reserved
     * |        |          |There are reserved bits.
     * |        |          |These bits are always read as '0' and must always be written with '0'.
     * |[2]     |Basic     |Basic Mode
     * |        |          |0 = Basic Mode disabled.
     * |        |          |1= IF1 Registers used as Tx Buffer, IF2 Registers used as Rx Buffer.
     * |[3]     |Silent    |Silent Mode
     * |        |          |0 = Normal operation.
     * |        |          |1 = The module is in Silent Mode.
     * |[4]     |LBack     |Loop Back Mode Enable Control
     * |        |          |0 = Loop Back Mode is disabled.
     * |        |          |1 = Loop Back Mode is enabled.
     * |[6:5]   |Tx10      |Tx[1:0]: Control Of CAN_TX Pin
     * |        |          |00 = Reset value, CAN_TX pin is controlled by the CAN Core.
     * |        |          |01 = Sample Point can be monitored at CAN_TX pin.
     * |        |          |10 = CAN_TX pin drives a dominant ('0') value.
     * |        |          |11 = CAN_TX pin drives a recessive ('1') value.
     * |[7]     |Rx        |Monitors The Actual Value Of CAN_RX Pin (Read Only)
     * |        |          |0 = The CAN bus is dominant (CAN_RX = '0').
     * |        |          |1 = The CAN bus is recessive (CAN_RX = '1').
     */
    __IO uint32_t   TEST;

    /**
     * CAN_BRPE
     * ===================================================================================================
     * Offset: 0x18  Baud Rate Prescaler Extension Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |BRPE      |BRPE: Baud Rate Prescaler Extension
     * |        |          |0x00-0x0F: By programming BRPE, the Baud Rate Prescaler can be extended to values up to 1023.
     * |        |          |The actual interpretation by the hardware is that one more than the value programmed by BRPE (MSBs) and BTIME (LSBs) is used.
     */
    __IO uint32_t   BRPE;

    /**
     * @cond HIDDEN_SYMBOLS     
     */
    __I uint32_t    RESERVE0[1];
    /**
     * @endcond
     */

    __IO CAN_IF_T   IF[2];

    /**
     * @cond HIDDEN_SYMBOLS     
     */
    __I uint32_t    RESERVE1[8];
    /**
     * @endcond
     */
    

    /**
     * CAN_TXREQ1
     * ===================================================================================================
     * Offset: 0x100  Transmission Request Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |TxRqst161 |Transmission Request Bits 16-1 (Of All Message Objects)
     * |        |          |0 = This Message Object is not waiting for transmission.
     * |        |          |1 = The transmission of this Message Object is requested and is not yet done.
     * |        |          |These bits are read only.
     */
    __IO uint32_t   TXREQ1;

    /**
     * CAN_TXREQ2
     * ===================================================================================================
     * Offset: 0x104  Transmission Request Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |TxRqst3217|Transmission Request Bits 32-17 (Of All Message Objects)
     * |        |          |0 = This Message Object is not waiting for transmission.
     * |        |          |1 = The transmission of this Message Object is requested and is not yet done.
     * |        |          |These bits are read only.
     */
    __IO uint32_t   TXREQ2;

    /**
     * @cond HIDDEN_SYMBOLS     
     */
    __I uint32_t    RESERVE2[6];
    /**
     * @endcond
     */

    /**
     * CAN_NDAT1
     * ===================================================================================================
     * Offset: 0x120  New Data Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |NewData161|New Data Bits 16-1 (Of All Message Objects)
     * |        |          |0 = No new data has been written into the data portion of this Message Object by the Message Handler since the last time this flag was cleared by the application software.
     * |        |          |1 = The Message Handler or the application software has written new data into the data portion of this Message Object.
     */
    __IO uint32_t   NDAT1;

    /**
     * CAN_NDAT2
     * ===================================================================================================
     * Offset: 0x124  New Data Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |NewData3217|New Data Bits 32-17 (Of All Message Objects)
     * |        |          |0 = No new data has been written into the data portion of this Message Object by the Message Handler since the last time this flag was cleared by the application software.
     * |        |          |1 = The Message Handler or the application software has written new data into the data portion of this Message Object.
     */
    __IO uint32_t   NDAT2;

    /**
     * @cond HIDDEN_SYMBOLS     
     */
    __I uint32_t    RESERVE3[6];
    /**
     * @endcond
     */
    

    /**
     * CAN_IPND1
     * ===================================================================================================
     * Offset: 0x140  Interrupt Pending Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |IntPnd161 |Interrupt Pending Bits 16-1 (Of All Message Objects)
     * |        |          |0 = This message object is not the source of an interrupt.
     * |        |          |1 = This message object is the source of an interrupt.
     */
    __IO uint32_t   IPND1;

    /**
     * CAN_IPND2
     * ===================================================================================================
     * Offset: 0x144  Interrupt Pending Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |IntPnd3217|Interrupt Pending Bits 32-17(Of All Message Objects)
     * |        |          |0 = This message object is not the source of an interrupt.
     * |        |          |1 = This message object is the source of an interrupt.
     */
    __IO uint32_t   IPND2;

    /**
     * @cond HIDDEN_SYMBOLS     
     */
    __I uint32_t    RESERVE4[6];
    /**
     * @endcond
     */

    /**
     * CAN_MVLD1
     * ===================================================================================================
     * Offset: 0x160  Message Valid Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |MsgVal161 |Message Valid Bits 16-1 (Of All Message Objects) (Read Only)
     * |        |          |0 = This Message Object is ignored by the Message Handler.
     * |        |          |1 = This Message Object is configured and should be considered by the Message Handler.
     * |        |          |Ex.
     * |        |          |CAN_MVLD1[0] means Message object No.1 is valid or not.
     * |        |          |If CAN_MVLD1[0] is set, message object No.1 is configured.
     */
    __IO uint32_t   MVLD1;

    /**
     * CAN_MVLD2
     * ===================================================================================================
     * Offset: 0x164  Message Valid Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |MsgVal3217|Message Valid Bits 32-17 (Of All Message Objects) (Read Only)
     * |        |          |0 = This Message Object is ignored by the Message Handler.
     * |        |          |1 = This Message Object is configured and should be considered by the Message Handler.
     * |        |          |Ex.CAN_MVLD2[15] means Message object No.32 is valid or not.
     * |        |          |If CAN_MVLD2[15] is set, message object No.32 is configured.
     */
    __IO uint32_t   MVLD2;

    /**
     * CAN_WU_EN
     * ===================================================================================================
     * Offset: 0x168  Wake-up Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WAKUP_EN  |Wake-Up Enable Control
     * |        |          |0 = The wake-up function Disabled.
     * |        |          |1 = The wake-up function Enabled.
     * |        |          |Note: User can wake-up system when there is a falling edge in the CAN_Rx pin.
     */
    __IO uint32_t   WU_EN;

    /**
     * CAN_WU_STATUS
     * ===================================================================================================
     * Offset: 0x16C  Wake-up Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WAKUP_STS |Wake-Up Status
     * |        |          |0 = No wake-up event occurred.
     * |        |          |1 = Wake-up event occurred.
     * |        |          |Note: This bit can be cleared by writing '0'.
     */
    __IO uint32_t   WU_STATUS;
} CAN_T;

/** @addtogroup CAN_CONST CAN Bit Field Definition
    Constant Definitions for CAN Controller
  @{
 */




/* CAN CON Bit Field Definitions */
#define CAN_CON_TEST_Pos           7                                    /*!< CAN_T::CON: TEST Position */
#define CAN_CON_TEST_Msk           (1ul << CAN_CON_TEST_Pos)            /*!< CAN_T::CON: TEST Mask     */

#define CAN_CON_CCE_Pos            6                                    /*!< CAN_T::CON: CCE Position  */
#define CAN_CON_CCE_Msk            (1ul << CAN_CON_CCE_Pos)             /*!< CAN_T::CON: CCE Mask      */

#define CAN_CON_DAR_Pos            5                                    /*!< CAN_T::CON: DAR Position  */
#define CAN_CON_DAR_Msk            (1ul << CAN_CON_DAR_Pos)             /*!< CAN_T::CON: DAR Mask      */

#define CAN_CON_EIE_Pos            3                                    /*!< CAN_T::CON: EIE Position  */
#define CAN_CON_EIE_Msk            (1ul << CAN_CON_EIE_Pos)             /*!< CAN_T::CON: EIE Mask      */

#define CAN_CON_SIE_Pos            2                                    /*!< CAN_T::CON: SIE Position  */
#define CAN_CON_SIE_Msk            (1ul << CAN_CON_SIE_Pos)             /*!< CAN_T::CON: SIE Mask      */

#define CAN_CON_IE_Pos             1                                    /*!< CAN_T::CON: IE Position   */
#define CAN_CON_IE_Msk             (1ul << CAN_CON_IE_Pos)              /*!< CAN_T::CON: IE Mask       */

#define CAN_CON_INIT_Pos           0                                    /*!< CAN_T::CON: INIT Position */
#define CAN_CON_INIT_Msk           (1ul << CAN_CON_INIT_Pos)            /*!< CAN_T::CON: INIT Mask     */

/* CAN STATUS Bit Field Definitions */
#define CAN_STATUS_BOFF_Pos        7                                    /*!< CAN_T::STATUS: BOFF Position  */
#define CAN_STATUS_BOFF_Msk        (1ul << CAN_STATUS_BOFF_Pos)         /*!< CAN_T::STATUS: BOFF Mask      */

#define CAN_STATUS_EWARN_Pos       6                                    /*!< CAN_T::STATUS: EWARN Position */
#define CAN_STATUS_EWARN_Msk       (1ul << CAN_STATUS_EWARN_Pos)        /*!< CAN_T::STATUS: EWARN Mask     */

#define CAN_STATUS_EPASS_Pos       5                                    /*!< CAN_T::STATUS: EPASS Position */
#define CAN_STATUS_EPASS_Msk       (1ul << CAN_STATUS_EPASS_Pos)        /*!< CAN_T::STATUS: EPASS Mask     */

#define CAN_STATUS_RXOK_Pos        4                                    /*!< CAN_T::STATUS: RXOK Position  */
#define CAN_STATUS_RXOK_Msk        (1ul << CAN_STATUS_RXOK_Pos)         /*!< CAN_T::STATUS: RXOK Mask      */

#define CAN_STATUS_TXOK_Pos        3                                    /*!< CAN_T::STATUS: TXOK Position  */
#define CAN_STATUS_TXOK_Msk        (1ul << CAN_STATUS_TXOK_Pos)         /*!< CAN_T::STATUS: TXOK Mask      */

#define CAN_STATUS_LEC_Pos         0                                    /*!< CAN_T::STATUS: LEC Position   */
#define CAN_STATUS_LEC_Msk         (0x3ul << CAN_STATUS_LEC_Pos)        /*!< CAN_T::STATUS: LEC Mask       */

/* CAN ERR Bit Field Definitions */                                             
#define CAN_ERR_RP_Pos             15                                   /*!< CAN_T::ERR: RP Position       */
#define CAN_ERR_RP_Msk             (1ul << CAN_ERR_RP_Pos)              /*!< CAN_T::ERR: RP Mask           */

#define CAN_ERR_REC_Pos            8                                    /*!< CAN_T::ERR: REC Position      */
#define CAN_ERR_REC_Msk            (0x7Ful << CAN_ERR_REC_Pos)          /*!< CAN_T::ERR: REC Mask          */

#define CAN_ERR_TEC_Pos            0                                    /*!< CAN_T::ERR: TEC Position      */
#define CAN_ERR_TEC_Msk            (0xFFul << CAN_ERR_TEC_Pos)          /*!< CAN_T::ERR: TEC Mask          */

/* CAN BTIME Bit Field Definitions */                                           
#define CAN_BTIME_TSEG2_Pos        12                                   /*!< CAN_T::BTIME: TSEG2 Position  */
#define CAN_BTIME_TSEG2_Msk        (0x7ul << CAN_BTIME_TSEG2_Pos)       /*!< CAN_T::BTIME: TSEG2 Mask      */

#define CAN_BTIME_TSEG1_Pos        8                                    /*!< CAN_T::BTIME: TSEG1 Position  */
#define CAN_BTIME_TSEG1_Msk        (0xFul << CAN_BTIME_TSEG1_Pos)       /*!< CAN_T::BTIME: TSEG1 Mask      */

#define CAN_BTIME_SJW_Pos          6                                    /*!< CAN_T::BTIME: SJW Position    */
#define CAN_BTIME_SJW_Msk          (0x3ul << CAN_BTIME_SJW_Pos)         /*!< CAN_T::BTIME: SJW Mask        */

#define CAN_BTIME_BRP_Pos          0                                    /*!< CAN_T::BTIME: BRP Position    */
#define CAN_BTIME_BRP_Msk          (0x3Ful << CAN_BTIME_BRP_Pos)        /*!< CAN_T::BTIME: BRP Mask        */

/* CAN IIDR Bit Field Definitions */                                            
#define CAN_IIDR_INTID_Pos         0                                    /*!< CAN_T::IIDR: INTID Position   */
#define CAN_IIDR_INTID_Msk         (0xFFFFul << CAN_IIDR_INTID_Pos)     /*!< CAN_T::IIDR: INTID Mask       */

/* CAN TEST Bit Field Definitions */                                            
#define CAN_TEST_RX_Pos            7                                    /*!< CAN_T::TEST: RX Position      */
#define CAN_TEST_RX_Msk            (1ul << CAN_TEST_RX_Pos)             /*!< CAN_T::TEST: RX Mask          */

#define CAN_TEST_TX_Pos            5                                    /*!< CAN_T::TEST: TX Position      */
#define CAN_TEST_TX_Msk            (0x3ul << CAN_TEST_TX_Pos)           /*!< CAN_T::TEST: TX Mask          */

#define CAN_TEST_LBACK_Pos         4                                    /*!< CAN_T::TEST: LBACK Position   */
#define CAN_TEST_LBACK_Msk         (1ul << CAN_TEST_LBACK_Pos)          /*!< CAN_T::TEST: LBACK Mask       */

#define CAN_TEST_SILENT_Pos        3                                    /*!< CAN_T::TEST: Silent Position  */
#define CAN_TEST_SILENT_Msk        (1ul << CAN_TEST_SILENT_Pos)         /*!< CAN_T::TEST: Silent Mask      */

#define CAN_TEST_BASIC_Pos         2                                    /*!< CAN_T::TEST: Basic Position   */
#define CAN_TEST_BASIC_Msk         (1ul << CAN_TEST_BASIC_Pos)          /*!< CAN_T::TEST: Basic Mask       */

/* CAN BPRE Bit Field Definitions */                                            
#define CAN_BRPE_BRPE_Pos          0                                    /*!< CAN_T::BRPE: BRPE Position    */
#define CAN_BRPE_BRPE_Msk          (0xFul << CAN_BRPE_BRPE_Pos)         /*!< CAN_T::BRPE: BRPE Mask        */

/* CAN IFn_CREQ Bit Field Definitions */                                        
#define CAN_IF_CREQ_BUSY_Pos       15                                   /*!< CAN_T::IFnCREQ: BUSY Position */
#define CAN_IF_CREQ_BUSY_Msk       (1ul << CAN_IF_CREQ_BUSY_Pos)        /*!< CAN_T::IFnCREQ: BUSY Mask     */

#define CAN_IF_CREQ_MSGNUM_Pos     0                                    /*!< CAN_T::IFnCREQ: MSGNUM Position */
#define CAN_IF_CREQ_MSGNUM_Msk     (0x3Ful << CAN_IF_CREQ_MSGNUM_Pos)   /*!< CAN_T::IFnCREQ: MSGNUM Mask     */

/* CAN IFn_CMASK Bit Field Definitions */                                      
#define CAN_IF_CMASK_WRRD_Pos      7                                    /*!< CAN_T::IFnCMASK: WRRD Position */
#define CAN_IF_CMASK_WRRD_Msk      (1ul << CAN_IF_CMASK_WRRD_Pos)       /*!< CAN_T::IFnCMASK: WRRD Mask     */

#define CAN_IF_CMASK_MASK_Pos      6                                    /*!< CAN_T::IFnCMASK: MASK Position */
#define CAN_IF_CMASK_MASK_Msk      (1ul << CAN_IF_CMASK_MASK_Pos)       /*!< CAN_T::IFnCMASK: MASK Mask     */

#define CAN_IF_CMASK_ARB_Pos       5                                    /*!< CAN_T::IFnCMASK: ARB Position  */
#define CAN_IF_CMASK_ARB_Msk       (1ul << CAN_IF_CMASK_ARB_Pos)        /*!< CAN_T::IFnCMASK: ARB Mask      */

#define CAN_IF_CMASK_CONTROL_Pos   4                                    /*!< CAN_T::IFnCMASK: CONTROL Position */
#define CAN_IF_CMASK_CONTROL_Msk   (1ul << CAN_IF_CMASK_CONTROL_Pos)    /*!< CAN_T::IFnCMASK: CONTROL Mask */

#define CAN_IF_CMASK_CLRINTPND_Pos 3                                    /*!< CAN_T::IFnCMASK: CLRINTPND Position */
#define CAN_IF_CMASK_CLRINTPND_Msk (1ul << CAN_IF_CMASK_CLRINTPND_Pos)  /*!< CAN_T::IFnCMASK: CLRINTPND Mask */

#define CAN_IF_CMASK_TXRQSTNEWDAT_Pos 2                                         /*!< CAN_T::IFnCMASK: TXRQSTNEWDAT Position */
#define CAN_IF_CMASK_TXRQSTNEWDAT_Msk (1ul << CAN_IF_CMASK_TXRQSTNEWDAT_Pos)    /*!< CAN_T::IFnCMASK: TXRQSTNEWDAT Mask     */

#define CAN_IF_CMASK_DATAA_Pos     1                                    /*!< CAN_T::IFnCMASK: DATAA Position */
#define CAN_IF_CMASK_DATAA_Msk     (1ul << CAN_IF_CMASK_DATAA_Pos)      /*!< CAN_T::IFnCMASK: DATAA Mask     */

#define CAN_IF_CMASK_DATAB_Pos     0                                    /*!< CAN_T::IFnCMASK: DATAB Position */
#define CAN_IF_CMASK_DATAB_Msk     (1ul << CAN_IF_CMASK_DATAB_Pos)      /*!< CAN_T::IFnCMASK: DATAB Mask     */

/* CAN IFn_MASK1 Bit Field Definitions */                                       
#define CAN_IF_MASK1_MSK_Pos       0                                    /*!< CAN_T::IFnMASK1: MSK Position   */
#define CAN_IF_MASK1_MSK_Msk       (0xFFul << CAN_IF_MASK1_MSK_Pos)     /*!< CAN_T::IFnMASK1: MSK Mask       */

/* CAN IFn_MASK2 Bit Field Definitions */                                       
#define CAN_IF_MASK2_MXTD_Pos      15                                   /*!< CAN_T::IFnMASK2: MXTD Position */
#define CAN_IF_MASK2_MXTD_Msk      (1ul << CAN_IF_MASK2_MXTD_Pos)       /*!< CAN_T::IFnMASK2: MXTD Mask     */

#define CAN_IF_MASK2_MDIR_Pos      14                                   /*!< CAN_T::IFnMASK2: MDIR Position */
#define CAN_IF_MASK2_MDIR_Msk      (1ul << CAN_IF_MASK2_MDIR_Pos)       /*!< CAN_T::IFnMASK2: MDIR Mask     */

#define CAN_IF_MASK2_MSK_Pos       0                                    /*!< CAN_T::IFnMASK2: MSK Position */
#define CAN_IF_MASK2_MSK_Msk       (0x1FFul << CAN_IF_MASK2_MSK_Pos)    /*!< CAN_T::IFnMASK2: MSK Mask     */

/* CAN IFn_ARB1 Bit Field Definitions */                                        
#define CAN_IF_ARB1_ID_Pos         0                                    /*!< CAN_T::IFnARB1: ID Position   */
#define CAN_IF_ARB1_ID_Msk         (0xFFFFul << CAN_IF_ARB1_ID_Pos)     /*!< CAN_T::IFnARB1: ID Mask       */

/* CAN IFn_ARB2 Bit Field Definitions */                                        
#define CAN_IF_ARB2_MSGVAL_Pos     15                                   /*!< CAN_T::IFnARB2: MSGVAL Position */
#define CAN_IF_ARB2_MSGVAL_Msk     (1ul << CAN_IF_ARB2_MSGVAL_Pos)      /*!< CAN_T::IFnARB2: MSGVAL Mask     */

#define CAN_IF_ARB2_XTD_Pos        14                                   /*!< CAN_T::IFnARB2: XTD Position    */
#define CAN_IF_ARB2_XTD_Msk        (1ul << CAN_IF_ARB2_XTD_Pos)         /*!< CAN_T::IFnARB2: XTD Mask        */

#define CAN_IF_ARB2_DIR_Pos        13                                   /*!< CAN_T::IFnARB2: DIR Position    */
#define CAN_IF_ARB2_DIR_Msk        (1ul << CAN_IF_ARB2_DIR_Pos)         /*!< CAN_T::IFnARB2: DIR Mask        */

#define CAN_IF_ARB2_ID_Pos         0                                    /*!< CAN_T::IFnARB2: ID Position     */
#define CAN_IF_ARB2_ID_Msk         (0x1FFFul << CAN_IF_ARB2_ID_Pos)     /*!< CAN_T::IFnARB2: ID Mask         */

/* CAN IFn_MCON Bit Field Definitions */                                        
#define CAN_IF_MCON_NEWDAT_Pos     15                                   /*!< CAN_T::IFnMCON: NEWDAT Position */
#define CAN_IF_MCON_NEWDAT_Msk     (1ul << CAN_IF_MCON_NEWDAT_Pos)      /*!< CAN_T::IFnMCON: NEWDAT Mask     */

#define CAN_IF_MCON_MSGLST_Pos     14                                   /*!< CAN_T::IFnMCON: MSGLST Position */
#define CAN_IF_MCON_MSGLST_Msk     (1ul << CAN_IF_MCON_MSGLST_Pos)      /*!< CAN_T::IFnMCON: MSGLST Mask     */

#define CAN_IF_MCON_INTPND_Pos     13                                   /*!< CAN_T::IFnMCON: INTPND Position */
#define CAN_IF_MCON_INTPND_Msk     (1ul << CAN_IF_MCON_INTPND_Pos)      /*!< CAN_T::IFnMCON: INTPND Mask     */

#define CAN_IF_MCON_UMASK_Pos      12                                   /*!< CAN_T::IFnMCON: UMASK Position  */
#define CAN_IF_MCON_UMASK_Msk      (1ul << CAN_IF_MCON_UMASK_Pos)       /*!< CAN_T::IFnMCON: UMASK Mask      */

#define CAN_IF_MCON_TXIE_Pos       11                                   /*!< CAN_T::IFnMCON: TXIE Position   */
#define CAN_IF_MCON_TXIE_Msk       (1ul << CAN_IF_MCON_TXIE_Pos)        /*!< CAN_T::IFnMCON: TXIE Mask       */

#define CAN_IF_MCON_RXIE_Pos       10                                   /*!< CAN_T::IFnMCON: RXIE Position   */
#define CAN_IF_MCON_RXIE_Msk       (1ul << CAN_IF_MCON_RXIE_Pos)        /*!< CAN_T::IFnMCON: RXIE Mask       */

#define CAN_IF_MCON_RMTEN_Pos      9                                    /*!< CAN_T::IFnMCON: RMTEN Position  */
#define CAN_IF_MCON_RMTEN_Msk      (1ul << CAN_IF_MCON_RMTEN_Pos)       /*!< CAN_T::IFnMCON: RMTEN Mask      */

#define CAN_IF_MCON_TXRQST_Pos     8                                    /*!< CAN_T::IFnMCON: TXRQST Position */
#define CAN_IF_MCON_TXRQST_Msk     (1ul << CAN_IF_MCON_TXRQST_Pos)      /*!< CAN_T::IFnMCON: TXRQST Mask     */

#define CAN_IF_MCON_EOB_Pos        7                                    /*!< CAN_T::IFnMCON: EOB Position    */
#define CAN_IF_MCON_EOB_Msk        (1ul << CAN_IF_MCON_EOB_Pos)         /*!< CAN_T::IFnMCON: EOB Mask        */

#define CAN_IF_MCON_DLC_Pos        0                                    /*!< CAN_T::IFnMCON: DLC Position    */
#define CAN_IF_MCON_DLC_Msk        (0xFul << CAN_IF_MCON_DLC_Pos)       /*!< CAN_T::IFnMCON: DLC Mask        */

/* CAN IFn_DATA_A1 Bit Field Definitions */                                     
#define CAN_IF_DAT_A1_DATA1_Pos    8                                    /*!< CAN_T::IFnDATAA1: DATA1 Position */
#define CAN_IF_DAT_A1_DATA1_Msk    (0xFFul << CAN_IF_DAT_A1_DATA1_Pos)  /*!< CAN_T::IFnDATAA1: DATA1 Mask     */

#define CAN_IF_DAT_A1_DATA0_Pos    0                                    /*!< CAN_T::IFnDATAA1: DATA0 Position */
#define CAN_IF_DAT_A1_DATA0_Msk    (0xFFul << CAN_IF_DAT_A1_DATA0_Pos)  /*!< CAN_T::IFnDATAA1: DATA0 Mask     */

/* CAN IFn_DATA_A2 Bit Field Definitions */                                     
#define CAN_IF_DAT_A2_DATA3_Pos    8                                    /*!< CAN_T::IFnDATAA1: DATA3 Position */
#define CAN_IF_DAT_A2_DATA3_Msk    (0xFFul << CAN_IF_DAT_A2_DATA3_Pos)  /*!< CAN_T::IFnDATAA1: DATA3 Mask     */

#define CAN_IF_DAT_A2_DATA2_Pos    0                                    /*!< CAN_T::IFnDATAA1: DATA2 Position */
#define CAN_IF_DAT_A2_DATA2_Msk    (0xFFul << CAN_IF_DAT_A2_DATA2_Pos)  /*!< CAN_T::IFnDATAA1: DATA2 Mask     */

/* CAN IFn_DATA_B1 Bit Field Definitions */                                     
#define CAN_IF_DAT_B1_DATA5_Pos    8                                    /*!< CAN_T::IFnDATAB1: DATA5 Position */
#define CAN_IF_DAT_B1_DATA5_Msk    (0xFFul << CAN_IF_DAT_B1_DATA5_Pos)  /*!< CAN_T::IFnDATAB1: DATA5 Mask */

#define CAN_IF_DAT_B1_DATA4_Pos    0                                    /*!< CAN_T::IFnDATAB1: DATA4 Position */
#define CAN_IF_DAT_B1_DATA4_Msk    (0xFFul << CAN_IF_DAT_B1_DATA4_Pos)  /*!< CAN_T::IFnDATAB1: DATA4 Mask */

/* CAN IFn_DATA_B2 Bit Field Definitions */                                     
#define CAN_IF_DAT_B2_DATA7_Pos    8                                    /*!< CAN_T::IFnDATAB2: DATA7 Position */
#define CAN_IF_DAT_B2_DATA7_Msk    (0xFFul << CAN_IF_DAT_B2_DATA7_Pos)  /*!< CAN_T::IFnDATAB2: DATA7 Mask     */

#define CAN_IF_DAT_B2_DATA6_Pos    0                                    /*!< CAN_T::IFnDATAB2: DATA6 Position */
#define CAN_IF_DAT_B2_DATA6_Msk    (0xFFul << CAN_IF_DAT_B2_DATA6_Pos)  /*!< CAN_T::IFnDATAB2: DATA6 Mask     */

/* CAN IFn_TXRQST1 Bit Field Definitions */
#define CAN_IF_TXRQST1_TXRQST_Pos  0                                        /*!< CAN_T::IFnTXRQST1: TXRQST Position */
#define CAN_IF_TXRQST1_TXRQST_Msk  (0xFFFFul << CAN_IF_TXRQST1_TXRQST_Pos)  /*!< CAN_T::IFnTXRQST1: TXRQST Mask     */

/* CAN IFn_TXRQST2 Bit Field Definitions */                                         
#define CAN_IF_TXRQST2_TXRQST_Pos  0                                        /*!< CAN_T::IFnTXRQST2: TXRQST Position  */
#define CAN_IF_TXRQST2_TXRQST_Msk  (0xFFFFul << CAN_IF_TXRQST2_TXRQST_Pos)  /*!< CAN_T::IFnTXRQST2: TXRQST Mask      */

/* CAN IFn_NDAT1 Bit Field Definitions */                                           
#define CAN_IF_NDAT1_NEWDATA_Pos   0                                        /*!< CAN_T::IFnNDAT1: NEWDATA Position */
#define CAN_IF_NDAT1_NEWDATA_Msk   (0xFFFFul << CAN_IF_NDAT1_NEWDATA_Pos)   /*!< CAN_T::IFnNDAT1: NEWDATA Mask     */

/* CAN IFn_NDAT2 Bit Field Definitions */                                           
#define CAN_IF_NDAT2_NEWDATA_Pos   0                                        /*!< CAN_T::IFnNDAT2: NEWDATA Position */
#define CAN_IF_NDAT2_NEWDATA_Msk   (0xFFFFul << CAN_IF_NDAT2_NEWDATA_Pos)   /*!< CAN_T::IFnNDAT2: NEWDATA Mask     */

/* CAN IFn_IPND1 Bit Field Definitions */                                           
#define CAN_IF_IPND1_INTPND_Pos   0                                         /*!< CAN_T::IFnIPND1: INTPND Position */
#define CAN_IF_IPND1_INTPND_Msk   (0xFFFFul << CAN_IF_IPND1_INTPND_Pos)     /*!< CAN_T::IFnIPND1: INTPND Mask     */

/* CAN IFn_IPND2 Bit Field Definitions */                                           
#define CAN_IF_IPND2_INTPND_Pos   0                                         /*!< CAN_T::IFnIPND2: INTPND Position */
#define CAN_IF_IPND2_INTPND_Msk   (0xFFFFul << CAN_IF_IPND2_INTPND_Pos)     /*!< CAN_T::IFnIPND2: INTPND Mask     */

/* CAN IFn_MVLD1 Bit Field Definitions */                                           
#define CAN_IF_MVLD1_MSGVAL_Pos   0                                         /*!< CAN_T::IFnMVLD1: MSGVAL Position */
#define CAN_IF_MVLD1_MSGVAL_Msk   (0xFFFFul << CAN_IF_MVLD1_MSGVAL_Pos)     /*!< CAN_T::IFnMVLD1: MSGVAL Mask     */

/* CAN IFn_MVLD2 Bit Field Definitions */                                           
#define CAN_IF_MVLD2_MSGVAL_Pos   0                                         /*!< CAN_T::IFnMVLD2: MSGVAL Position */
#define CAN_IF_MVLD2_MSGVAL_Msk   (0xFFFFul << CAN_IF_MVLD2_MSGVAL_Pos)     /*!< CAN_T::IFnMVLD2: MSGVAL Mask     */

/* CAN WUEN Bit Field Definitions */                                                
#define CAN_WUEN_WAKUP_EN_Pos     0                                         /*!< CAN_T::WUEN: WAKUP_EN Position */
#define CAN_WUEN_WAKUP_EN_Msk    (1ul << CAN_WUEN_WAKUP_EN_Pos)             /*!< CAN_T::WUEN: WAKUP_EN Mask     */

/* CAN WUSTATUS Bit Field Definitions */                                           
#define CAN_WUSTATUS_WAKUP_STS_Pos     0                                    /*!< CAN_T::WUSTATUS: WAKUP_STS Position */
#define CAN_WUSTATUS_WAKUP_STS_Msk    (1ul << CAN_WUSTATUS_WAKUP_STS_Pos)   /*!< CAN_T::WUSTATUS: WAKUP_STS Mask     */
/*@}*/ /* end of group CAN_CONST */
/*@}*/ /* end of group CAN */




/*---------------------- System Clock Controller -------------------------*/
/**
    @addtogroup CLK System Clock Controller (CLK)
    Memory Mapped Structure for CLK Controller
@{ */
typedef struct
{

    /**
     * PWRCON
     * ===================================================================================================
     * Offset: 0x00  System Power-down Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |XTL12M_EN |External 4~24 MHz High Speed Crystal Enable (HXT) Control (Write Protect)
     * |        |          |The bit default value is set by flash controller user configuration register CONFIG0 [26:24].
     * |        |          |When the default clock source is from external 4~24 MHz high speed crystal, this bit is set to 1 automatically.
     * |        |          |0 = External 4~24 MHz high speed crystal oscillator (HXT) Disabled.
     * |        |          |1 = External 4~24 MHz high speed crystal oscillator (HXT) Enabled.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[2]     |OSC22M_EN |Internal 22.1184 MHz High Speed Oscillator (HIRC) Enable Control (Write Protect)
     * |        |          |0 = Internal 22.1184 MHz high speed oscillator (HIRC) Disabled.
     * |        |          |1 = Internal 22.1184 MHz high speed oscillator (HIRC) Enabled.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[3]     |OSC10K_EN |Internal 10 KHz Low Speed Oscillator (LIRC) Enable Control (Write Protect)
     * |        |          |0 = Internal 10 kHz low speed oscillator (LIRC) Disabled.
     * |        |          |1 = Internal 10 kHz low speed oscillator (LIRC) Enabled.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[4]     |PD_WU_DLY |Wake-up Delay Counter Enable Control (Write Protect)
     * |        |          |When the chip wakes up from Power-down mode, the clock control will delay certain clock cycles to wait system clock stable.
     * |        |          |The delayed clock cycle is 4096 clock cycles when chip work at external 4~24 MHz high speed crystal, and 256 clock cycles when chip work at internal 22.1184 MHz high speed oscillator.
     * |        |          |0 = Clock cycles delay Disabled.
     * |        |          |1 = Clock cycles delay Enabled.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[5]     |PD_WU_INT_EN|Power-Down Mode Wake-Up Interrupt Enable Control (Write Protect)
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |        |          |Note1: The interrupt will occur when both PD_WU_STS and PD_WU_INT_EN are high.
     * |        |          |Note2: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[6]     |PD_WU_STS |Power-Down Mode Wake-Up Interrupt Status
     * |        |          |Set by "Power-down wake-up event", it indicates that resume from Power-down mode.
     * |        |          |The flag is set if the GPIO, UART, WDT, I2C, TIMER or BOD wake-up occurred. Write 1 to clear the bit to 0.
     * |        |          |Note: This bit is working only if PD_WU_INT_EN (PWRCON[5]) set to 1.
     * |[7]     |PWR_DOWN_EN|System Power-Down Enable Bit (Write Protect)
     * |        |          |When this bit is set to 1, Power-down mode is enabled and chip Power-down behavior will depends on the PD_WAIT_CPU bit
     * |        |          |(a) If the PD_WAIT_CPU is 0, then the chip enters Power-down mode immediately after the PWR_DOWN_EN bit set.
     * |        |          |(b) if the PD_WAIT_CPU is 1, then the chip keeps active till the CPU sleep mode is also active and then the chip enters Power-down mode (recommend)
     * |        |          |When chip wakes up from Power-down mode, this bit is cleared by hardware.
     * |        |          |User needs to set this bit again for next Power-down.
     * |        |          |In Power-down mode, external 4~24 MHz high speed crystal oscillator and the internal 22.1184 MHz high speed oscillator will be disabled in this mode, but the internal 10 kHz low speed oscillator are not controlled by Power-down mode.
     * |        |          |In Power- down mode, the PLL and system clock are disabled, and ignored the clock source selection.
     * |        |          |The clocks of peripheral are not controlled by Power-down mode, if the peripheral clock source is from the internal 10 kHz low speed oscillator.
     * |        |          |0 = Chip operating normally or chip in Idle mode because of WFI command.
     * |        |          |1 = Chip enters Power-down mode instantly or waits CPU sleep command WFI.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[8]     |PD_WAIT_CPU|This Bit Control The Power-Down Entry Condition (Write Protect)
     * |        |          |0 = Chip enters Power-down mode when the PWR_DOWN_EN bit is set to 1.
     * |        |          |1 = Chip enters Power- down mode when the both PD_WAIT_CPU and PWR_DOWN_EN bits are set to 1 and CPU run WFI instruction.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     */
    __IO uint32_t PWRCON;

    /**
    * AHBCLK
    * ===================================================================================================
    * Offset: 0x04  AHB Devices Clock Enable Control Register
    * ---------------------------------------------------------------------------------------------------
    * |Bits    |Field     |Descriptions
    * | :----: | :----:   | :---- |
    * |[2]     |ISP_EN    |Flash ISP Controller Clock Enable Control
    * |        |          |0 = Flash ISP peripheral clock Disabled.
    * |        |          |1 = Flash ISP peripheral clock Enabled.
    */
    __IO uint32_t AHBCLK;

    /**
     * APBCLK
     * ===================================================================================================
     * Offset: 0x08  APB Devices Clock Enable Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WDT_EN    |Watchdog Timer Clock Enable Control (Write Protect)
     * |        |          |0 = Watchdog Timer clock Disabled.
     * |        |          |1 = Watchdog Timer clock Enabled.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[2]     |TMR0_EN   |Timer0 Clock Enable Control
     * |        |          |0 = Timer0 clock Disabled.
     * |        |          |1 = Timer0 clock Enabled.
     * |[3]     |TMR1_EN   |Timer1 Clock Enable Control
     * |        |          |0 = Timer1 clock Disabled.
     * |        |          |1 = Timer1 clock Enabled.
     * |[4]     |TMR2_EN   |Timer2 Clock Enable Control
     * |        |          |0 = Timer2 clock Disabled.
     * |        |          |1 = Timer2 clock Enabled.
     * |[5]     |TMR3_EN   |Timer3 Clock Enable Control
     * |        |          |0 = Timer3 clock Disabled.
     * |        |          |1 = Timer3 clock Enabled.
     * |[6]     |FDIV_EN   |Frequency Divider Output Clock Enable Control
     * |        |          |0 = FDIV clock Disabled.
     * |        |          |1 = FDIV clock Enabled.
     * |[8]     |I2C0_EN   |I2C0 Clock Enable Control
     * |        |          |0 = I2C0 clock Disabled.
     * |        |          |1 = I2C0 clock Enabled.
     * |[9]     |I2C1_EN   |I2C1 Clock Enable Control
     * |        |          |0 = I2C1 clock Disabled.
     * |        |          |1 = I2C1 clock Enabled.
     * |[12]    |SPI0_EN   |SPI0 Clock Enable Control
     * |        |          |0 = SPI0 clock Disabled.
     * |        |          |1 = SPI0 clock Enabled.
     * |[16]    |UART0_EN  |UART0 Clock Enable Control
     * |        |          |0 = UART0 clock Disabled.
     * |        |          |1 = UART0 clock Enabled.
     * |[17]    |UART1_EN  |UART1 Clock Enable Control
     * |        |          |0 = UART1 clock Disabled.
     * |        |          |1 = UART1 clock Enabled.
     * |[18]    |UART2_EN  |UART2 Clock Enable Control
     * |        |          |0 = UART2 clock Disabled.
     * |        |          |1 = UART2 clock Enabled.
     * |[24]    |CAN0_EN   |CAN Bus Controller-0 Clock Enable Control
     * |        |          |0 = CAN0 clock Disabled.
     * |        |          |1 = CAN0 clock Enable.     
     * |[28]    |ADC_EN    |Analog-Digital-Converter (ADC) Clock Enable Control
     * |        |          |0 = ADC clock Disabled.
     * |        |          |1 = ADC clock Enabled.
     */
    __IO uint32_t APBCLK;

    /**
     * CLKSTATUS
     * ===================================================================================================
     * Offset: 0x0C  Clock status monitor Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |XTL12M_STB|External 4~24 MHz High Speed Crystal (HXT) Clock Source Stable Flag (Read Only)
     * |        |          |0 = External 4~24 MHz high speed crystal clock (HXT) is not stable or disabled.
     * |        |          |1 = External 4~24 MHz high speed crystal clock (HXT) is stable.
     * |[2]     |PLL_STB   |Internal PLL Clock Source Stable Flag (Read Only)
     * |        |          |0 = Internal PLL clock is not stable or disabled.
     * |        |          |1 = Internal PLL clock is stable.
     * |[3]     |OSC10K_STB|Internal 10 KHz Low Speed Oscillator (LIRC) Clock Source Stable Flag (Read Only)
     * |        |          |0 = Internal 10 kHz low speed oscillator clock (LIRC) is not stable or disabled.
     * |        |          |1 = Internal 10 kHz low speed oscillator clock (LIRC) is stable.
     * |[4]     |OSC22M_STB|Internal 22.1184 MHz High Speed Oscillator (HIRC) Clock Source Stable Flag (Read Only)
     * |        |          |0 = Internal 22.1184 MHz high speed oscillator (HIRC) clock is not stable or disabled.
     * |        |          |1 = Internal 22.1184 MHz high speed oscillator (HIRC) clock is stable.
     * |[7]     |CLK_SW_FAIL|Clock Switching Fail Flag (Read Only)
     * |        |          |0 = Clock switching success.
     * |        |          |1 = Clock switching failure.
     * |        |          |This bit is an index that if current system clock source is match as user defined at HCLK_S (CLKSEL[2:0]).
     * |        |          |When user switch system clock, the system clock source will keep old clock until the new clock is stable.
     * |        |          |During the period that waiting new clock stable, this bit will be an index shows system clock source is not match as user wanted.
     */
    __IO uint32_t CLKSTATUS;

    /**
     * CLKSEL0
     * ===================================================================================================
     * Offset: 0x10  Clock Source Select Control Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2:0]   |HCLK_S    |HCLK Clock Source Select (Write Protect)
     * |        |          |1. Before clock switching, the related clock sources (both pre-select and new-select) must be turn on.
     * |        |          |2. The 3-bit default value is reloaded from the value of CFOSC (CONFIG0[26:24]) in user configuration register of Flash controller by any reset. Therefore the default value is either 000b or 111b.
     * |        |          |000 = Clock source from external 4~24 MHz high speed crystal oscillator clock.
     * |        |          |010 = Clock source from PLL clock.
     * |        |          |011 = Clock source from internal 10 kHz low speed oscillator clock.
     * |        |          |111 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection. Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[5:3]   |STCLK_S   |Cortex-M0 SysTick Clock Source Select (Write Protect)
     * |        |          |If SYST_CSR[2] = 1, SysTick clock source is from HCLK.
     * |        |          |If SYST_CSR[2] = 0, SysTick clock source is defined by STCLK_S(CLKSEL0[5:3]).
     * |        |          |000 = Clock source from external 4~24 MHz high speed crystal clock.
     * |        |          |010 = Clock source from external 4~24 MHz high speed crystal clock/2.
     * |        |          |011 = Clock source from HCLK/2.
     * |        |          |111 = Clock source from internal 22.1184 MHz high speed oscillator clock/2.
     * |        |          |Note1: These bits are protected bit. It means programming this bit needs to write "59h", "16h", "88h" to address 0x5000_0100 to disable register protection. Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |        |          |Note2: if SysTick clock source is not from HCLK (i.e. SYST_CSR[2] = 0), SysTick clock source must less than or equal to HCLK/2.
     */
    __IO uint32_t CLKSEL0;

    /**
     * CLKSEL1
     * ===================================================================================================
     * Offset: 0x14  Clock Source Select Control Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |WDT_S     |Watchdog Timer Clock Source Select (Write Protect)
     * |        |          |10 = Clock source from HCLK/2048 clock.
     * |        |          |11 = Clock source from internal 10 kHz low speed oscillator clock.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[3:2]   |ADC_S     |ADC Clock Source Select
     * |        |          |00 = Clock source from external 4~24 MHz high speed crystal oscillator clock.
     * |        |          |01 = Clock source from PLL clock.
     * |        |          |10 = Clock source from HCLK.
     * |        |          |11 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |[4]     |SPI0_S    |SPI0 Clock Source Selection
     * |        |          |0 = Clock source from PLL clock.
     * |        |          |1 = Clock source from HCLK.
     * |[10:8]  |TMR0_S    |TIMER0 Clock Source Selection
     * |        |          |000 = Clock source from external 4~24 MHz high speed crystal clock.
     * |        |          |010 = Clock source from HCLK.
     * |        |          |011 = Clock source from external trigger.
     * |        |          |101 = Clock source from internal 10 kHz low speed oscillator clock.
     * |        |          |111 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |        |          |Others = reserved.
     * |[14:12] |TMR1_S    |TIMER1 Clock Source Selection
     * |        |          |000 = Clock source from external 4~24 MHz high speed crystal clock.
     * |        |          |010 = Clock source from HCLK.
     * |        |          |011 = Clock source from external trigger.
     * |        |          |101 = Clock source from internal 10 kHz low speed oscillator clock.
     * |        |          |111 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |        |          |Others = reserved.
     * |[18:16] |TMR2_S    |TIMER2 Clock Source Selection
     * |        |          |000 = Clock source from external 4~24 MHz high speed crystal clock.
     * |        |          |010 = Clock source from HCLK.
     * |        |          |011 = Clock source from external trigger.
     * |        |          |101 = Clock source from internal 10 kHz low speed oscillator clock.
     * |        |          |111 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |        |          |Others = reserved.
     * |[22:20] |TMR3_S    |TIMER3 Clock Source Selection
     * |        |          |000 = Clock source from external 4~24 MHz high speed crystal clock.
     * |        |          |010 = Clock source from HCLK.
     * |        |          |011 = Clock source from external trigger.
     * |        |          |101 = Clock source from internal 10 kHz low speed oscillator clock.
     * |        |          |111 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |        |          |Others = reserved.
     * |[25:24] |UART_S    |UART Clock Source Selection
     * |        |          |00 = Clock source from external 4~24 MHz high speed crystal oscillator clock.
     * |        |          |01 = Clock source from PLL clock.
     * |        |          |11 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     */
    __IO uint32_t CLKSEL1;

    /**
     * CLKDIV
     * ===================================================================================================
     * Offset: 0x18  Clock Divider Number Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |HCLK_N    |HCLK Clock Divide Number From HCLK Clock Source
     * |        |          |HCLK clock frequency = (HCLK clock source frequency) / (HCLK_N + 1).
     * |[11:8]  |UART_N    |UART Clock Divide Number From UART Clock Source
     * |        |          |UART clock frequency = (UART clock source frequency) / (UART_N + 1).
     * |[23:16] |ADC_N     |ADC Clock Divide Number From ADC Clock Source
     * |        |          |ADC clock frequency = (ADC clock source frequency) / (ADC_N + 1).
     */
    __IO uint32_t CLKDIV;

    /**
     * CLKSEL2
     * ===================================================================================================
     * Offset: 0x1C  Clock Source Select Control Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:2]   |FRQDIV_S  |Clock Divider Clock Source Selection
     * |        |          |00 = Clock source from external 4~24 MHz high speed crystal oscillator clock.
     * |        |          |10 = Clock source from HCLK.
     * |        |          |11 = Clock source from internal 22.1184 MHz high speed oscillator clock.
     * |[17:16] |WWDT_S    |Window Watchdog Timer Clock Source Selection
     * |        |          |10 = Clock source from HCLK/2048 clock.
     * |        |          |11 = Clock source from internal 10 kHz low speed oscillator clock.
     */
    __IO uint32_t CLKSEL2;

    /**
     * PLLCON
     * ===================================================================================================
     * Offset: 0x20  PLL Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[8:0]   |FB_DV     |PLL Feedback Divider Control Bits
     * |        |          |Refer to the formulas below the table.
     * |[13:9]  |IN_DV     |PLL Input Divider Control Bits
     * |        |          |Refer to the formulas below the table.
     * |[15:14] |OUT_DV    |PLL Output Divider Control Bits
     * |        |          |Refer to the formulas below the table.
     * |[16]    |PD        |Power-Down Mode
     * |        |          |If the PWR_DOWN_EN bit is set to 1 in PWRCON register, the PLL will enter Power-down mode too.
     * |        |          |0 = PLL is in Normal mode.
     * |        |          |1 = PLL is in Power-down mode (default).
     * |[17]    |BP        |PLL Bypass Control
     * |        |          |0 = PLL is in Normal mode (default).
     * |        |          |1 = PLL clock output is same as PLL source clock input.
     * |[18]    |OE        |PLL OE (FOUT Enable) Pin Control
     * |        |          |0 = PLL FOUT Enabled.
     * |        |          |1 = PLL FOUT is fixed low.
     * |[19]    |PLL_SRC   |PLL Source Clock Selection
     * |        |          |0 = PLL source clock from external 4~24 MHz high speed crystal.
     * |        |          |1 = PLL source clock from internal 22.1184 MHz high speed oscillator.
     */
    __IO uint32_t PLLCON;

    /**
     * FRQDIV
     * ===================================================================================================
     * Offset: 0x24  Frequency Divider Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |FSEL      |Divider Output Frequency Selection Bits
     * |        |          |The formula of output frequency is Fout = Fin/2(N+1).
     * |        |          |Fin is the input clock frequency.
     * |        |          |Fout is the frequency of divider output clock.
     * |        |          |N is the 4-bit value of FSEL[3:0].
     * |[4]     |DIVIDER_EN|Frequency Divider Enable Bit
     * |        |          |0 = Frequency Divider function Disabled.
     * |        |          |1 = Frequency Divider function Enabled.
     * |[5]     |DIVIDER1  |Frequency Divider One Enable Bit
     * |        |          |0 = Frequency divider will output clock with source frequency divided by FSEL.
     * |        |          |1 = Frequency divider will output clock with source frequency.
     */
    __IO uint32_t FRQDIV;

    __IO uint32_t RESERVE0[2];

    /**
     * APBCLK1
     * ===================================================================================================
     * Offset: 0x30  APB Devices Clock Enable Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[8]     |UART3_EN  |UART3 Clock Enable Bit
     * |        |          |0 = UART3 clock Disabled.
     * |        |          |1 = UART3 clock Enabled.
     * |[9]     |UART4_EN  |UART4 Clock Enable Bit
     * |        |          |0 = UART4 clock Disabled.
     * |        |          |1 = UART4 clock Enabled.
     * |[10]    |UART5_EN  |UART5 Clock Enable Bit
     * |        |          |0 = UART5 clock Disabled.
     * |        |          |1 = UART5 clock Enabled.
     * |[16]    |PWM0_EN   |PWM0 Clock Enable Bit
     * |        |          |0 = PWM0 clock Disabled.
     * |        |          |1 = PWM0 clock Enabled.
     * |[17]    |PWM1_EN   |PWM1 Clock Enable Bit
     * |        |          |0 = PWM1 clock Disabled.
     * |        |          |1 = PWM1 clock Enabled.
     * |[18]    |BPWM0_EN  |BPWM0 Clock Enable Bit
     * |        |          |0 = BPWM0 clock Disabled.
     * |        |          |1 = BPWM0 clock Enabled.
     * |[19]    |BPWM1_EN  |BPWM1 Clock Enable Bit
     * |        |          |0 = BPWM1 clock Disabled.
     * |        |          |1 = BPWM1 clock Enabled.
     */
    __IO uint32_t APBCLK1;

    /**
     * CLKSEL3
     * ===================================================================================================
     * Offset: 0x34  Clock Source Select Control Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[16]    |PWM0_S    |PWM0 Clock Source Selection
     * |        |          |The peripheral clock source of PWM0 is defined by PWM0_S.
     * |        |          |0 = Clock source from PLL.
     * |        |          |1 = Clock source from PCLK.
     * |[17]    |PWM1_S    |PWM1 Clock Source Selection
     * |        |          |The peripheral clock source of PWM1 is defined by PWM1_S.
     * |        |          |0 = Clock source from PLL.
     * |        |          |1 = Clock source from PCLK.
     * |[18]    |BPWM0_S   |BPWM0 Clock Source Selection
     * |        |          |The peripheral clock source of BPWM0 is defined by BPWM0_S.
     * |        |          |0 = Clock source from PLL.
     * |        |          |1 = Clock source from PCLK.
     * |[19]    |BPWM1_S   |BPWM1 Clock Source Selection
     * |        |          |The peripheral clock source of BPWM1 is defined by BPWM1_S.
     * |        |          |0 = Clock source from PLL.
     * |        |          |1 = Clock source from PCLK.
     */
    __IO uint32_t CLKSEL3;

    __IO uint32_t RESERVE1[14];

    /**
     * CLKDCTL
     * ===================================================================================================
     * Offset: 0x70  Clock Fail Detector Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |HXTFDEN   |HXT Clock Fail Detector Enable Bit
     * |        |          |0 = HXT clock Fail detector Disabled.
     * |        |          |1 = HXT clock Fail detector Enabled.
     * |[5]     |HXTFIEN   |HXT Clock Fail Interrupt Enable Bit
     * |        |          |0 = HXT clock Fail interrupt Disabled.
     * |        |          |1 = HXT clock Fail interrupt Enabled.
     * |[16]    |HXTFQDEN  |HXT Clock Frequency Monitor Enable Bit
     * |        |          |0 = HXT clock frequency monitor Disabled.
     * |        |          |1 = HXT clock frequency monitor Enabled.
     * |[17]    |HXTFQIEN  |HXT Clock Frequency Monitor Interrupt Enable Bit
     * |        |          |0 = HXT clock frequency monitor fail interrupt Disabled.
     * |        |          |1 = HXT clock frequency monitor fail interrupt Enabled.
     */
    __IO uint32_t CLKDCTL;

    /**
     * CLKDSTS
     * ===================================================================================================
     * Offset: 0x74  Clock Fail Detector Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |HXTFIF    |HXT Clock Fail Interrupt Flag
     * |        |          |0 = HXT clock normal.
     * |        |          |1 = HXT clock stop (write "1" to clear).
     * |[8]     |HXTFQIF   |HXT Clock Frequency Monitor Interrupt Flag
     * |        |          |0 = HXT clock normal.
     * |        |          |1 = HXT clock frequency abnormal (write "1" to clear).
     */
    __IO uint32_t CLKDSTS;

    /**
     * CDUPB
     * ===================================================================================================
     * Offset: 0x78  Clock Frequency Detector Upper Boundary Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[9:0]   |UPERBD    |HXT Clock Frequency Detector Upper Boundary
     * |        |          |The bits define the high value of frequency monitor window.
     * |        |          |When HXT frequency monitor value higher than this register,
     * |        |          |the HXT frequency detect fail interrupt flag will set to 1.
     */
    __IO uint32_t CDUPB;

    /**
     * CDLOWB
     * ===================================================================================================
     * Offset: 0x7C  Clock Frequency Detector Lower Boundary Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[9:0]   |LOWERBD   |HXT Clock Frequency Detector Lower Boundary
     * |        |          |The bits define the low value of frequency monitor window.
     * |        |          |When HXT frequency monitor values lower than this register,
     * |        |          |the HXT frequency detect fail interrupt flag will set to 1.
     */
    __IO uint32_t CDLOWB;


} CLK_T;

/**
    @addtogroup CLK_CONST CLK Bit Field Definition
    Constant Definitions for CLK Controller
@{ */

/* CLK PWRCON Bit Field Definitions */
#define CLK_PWRCON_PD_WAIT_CPU_Pos           8                                    /*!< CLK_T::PWRCON: PD_WAIT_CPU Position */
#define CLK_PWRCON_PD_WAIT_CPU_Msk           (1ul << CLK_PWRCON_PD_WAIT_CPU_Pos)  /*!< CLK_T::PWRCON: PD_WAIT_CPU Mask */

#define CLK_PWRCON_PWR_DOWN_EN_Pos           7                                    /*!< CLK_T::PWRCON: PWR_DOWN_EN Position */
#define CLK_PWRCON_PWR_DOWN_EN_Msk           (1ul << CLK_PWRCON_PWR_DOWN_EN_Pos)  /*!< CLK_T::PWRCON: PWR_DOWN_EN Mask */

#define CLK_PWRCON_PD_WU_STS_Pos             6                                    /*!< CLK_T::PWRCON: PD_WU_STS Position */
#define CLK_PWRCON_PD_WU_STS_Msk             (1ul << CLK_PWRCON_PD_WU_STS_Pos)    /*!< CLK_T::PWRCON: PD_WU_STS Mask */

#define CLK_PWRCON_PD_WU_INT_EN_Pos          5                                    /*!< CLK_T::PWRCON: PD_WU_INT_EN Position */
#define CLK_PWRCON_PD_WU_INT_EN_Msk          (1ul << CLK_PWRCON_PD_WU_INT_EN_Pos) /*!< CLK_T::PWRCON: PD_WU_INT_EN Mask */

#define CLK_PWRCON_PD_WU_DLY_Pos             4                                    /*!< CLK_T::PWRCON: PD_WU_DLY Position */
#define CLK_PWRCON_PD_WU_DLY_Msk             (1ul << CLK_PWRCON_PD_WU_DLY_Pos)    /*!< CLK_T::PWRCON: PD_WU_DLY Mask */

#define CLK_PWRCON_OSC10K_EN_Pos             3                                    /*!< CLK_T::PWRCON: OSC10K_EN Position */
#define CLK_PWRCON_OSC10K_EN_Msk             (1ul << CLK_PWRCON_OSC10K_EN_Pos)    /*!< CLK_T::PWRCON: OSC10K_EN Mask */
#define CLK_PWRCON_IRC10K_EN_Pos             3                                    /*!< CLK_T::PWRCON: IRC10K_EN Position */
#define CLK_PWRCON_IRC10K_EN_Msk             (1ul << CLK_PWRCON_IRC10K_EN_Pos)    /*!< CLK_T::PWRCON: IRC10K_EN Mask */

#define CLK_PWRCON_OSC22M_EN_Pos             2                                    /*!< CLK_T::PWRCON: OSC22M_EN Position */
#define CLK_PWRCON_OSC22M_EN_Msk             (1ul << CLK_PWRCON_OSC22M_EN_Pos)    /*!< CLK_T::PWRCON: OSC22M_EN Mask */
#define CLK_PWRCON_IRC22M_EN_Pos             2                                    /*!< CLK_T::PWRCON: IRC22M_EN Position */
#define CLK_PWRCON_IRC22M_EN_Msk             (1ul << CLK_PWRCON_IRC22M_EN_Pos)    /*!< CLK_T::PWRCON: IRC22M_EN Mask */

#define CLK_PWRCON_XTL12M_EN_Pos             0                                    /*!< CLK_T::PWRCON: XTL12M_EN Position */
#define CLK_PWRCON_XTL12M_EN_Msk             (1ul << CLK_PWRCON_XTL12M_EN_Pos)    /*!< CLK_T::PWRCON: XTL12M_EN Mask */

/* CLK AHBCLK Bit Field Definitions */
#define CLK_AHBCLK_ISP_EN_Pos                2                                    /*!< CLK_T::AHBCLK: ISP_EN Position */
#define CLK_AHBCLK_ISP_EN_Msk                (1ul << CLK_AHBCLK_ISP_EN_Pos)       /*!< CLK_T::AHBCLK: ISP_EN Mask */

/* CLK APBCLK Bit Field Definitions */
#define CLK_APBCLK_ADC_EN_Pos                28                                   /*!< CLK_T::APBCLK: ADC_EN Position */
#define CLK_APBCLK_ADC_EN_Msk                (1ul << CLK_APBCLK_ADC_EN_Pos)       /*!< CLK_T::APBCLK: ADC_EN Mask */

#define CLK_APBCLK_CAN0_EN_Pos               24                                   /*!< CLK APBCLK: CAN0_EN Position */
#define CLK_APBCLK_CAN0_EN_Msk               (1ul << CLK_APBCLK_CAN0_EN_Pos)      /*!< CLK APBCLK: CAN0_EN Mask */

#define CLK_APBCLK_UART2_EN_Pos              18                                   /*!< CLK_T::APBCLK: UART2_EN Position */
#define CLK_APBCLK_UART2_EN_Msk              (1ul << CLK_APBCLK_UART2_EN_Pos)     /*!< CLK_T::APBCLK: UART2_EN Mask */

#define CLK_APBCLK_UART1_EN_Pos              17                                   /*!< CLK_T::APBCLK: UART1_EN Position */
#define CLK_APBCLK_UART1_EN_Msk              (1ul << CLK_APBCLK_UART1_EN_Pos)     /*!< CLK_T::APBCLK: UART1_EN Mask */

#define CLK_APBCLK_UART0_EN_Pos              16                                   /*!< CLK_T::APBCLK: UART0_EN Position */
#define CLK_APBCLK_UART0_EN_Msk              (1ul << CLK_APBCLK_UART0_EN_Pos)     /*!< CLK_T::APBCLK: UART0_EN Mask */

#define CLK_APBCLK_SPI0_EN_Pos               12                                   /*!< CLK_T::APBCLK: SPI0_EN Position */
#define CLK_APBCLK_SPI0_EN_Msk               (1ul << CLK_APBCLK_SPI0_EN_Pos)      /*!< CLK_T::APBCLK: SPI0_EN Mask */

#define CLK_APBCLK_I2C1_EN_Pos               9                                    /*!< CLK_T::APBCLK: I2C1_EN Position */
#define CLK_APBCLK_I2C1_EN_Msk               (1ul << CLK_APBCLK_I2C1_EN_Pos)      /*!< CLK_T::APBCLK: I2C1_EN Mask */

#define CLK_APBCLK_I2C0_EN_Pos               8                                    /*!< CLK_T::APBCLK: I2C0_EN_ Position */
#define CLK_APBCLK_I2C0_EN_Msk               (1ul << CLK_APBCLK_I2C0_EN_Pos)      /*!< CLK_T::APBCLK: I2C0_EN_ Mask */

#define CLK_APBCLK_FDIV_EN_Pos               6                                    /*!< CLK_T::APBCLK: FDIV_EN Position */
#define CLK_APBCLK_FDIV_EN_Msk               (1ul << CLK_APBCLK_FDIV_EN_Pos)      /*!< CLK_T::APBCLK: FDIV_EN Mask */

#define CLK_APBCLK_TMR3_EN_Pos               5                                    /*!< CLK_T::APBCLK: TMR3_EN Position */
#define CLK_APBCLK_TMR3_EN_Msk               (1ul << CLK_APBCLK_TMR3_EN_Pos)      /*!< CLK_T::APBCLK: TMR3_EN Mask */

#define CLK_APBCLK_TMR2_EN_Pos               4                                    /*!< CLK_T::APBCLK: TMR2_EN Position */
#define CLK_APBCLK_TMR2_EN_Msk               (1ul << CLK_APBCLK_TMR2_EN_Pos)      /*!< CLK_T::APBCLK: TMR2_EN Mask */

#define CLK_APBCLK_TMR1_EN_Pos               3                                    /*!< CLK_T::APBCLK: TMR1_EN Position */
#define CLK_APBCLK_TMR1_EN_Msk               (1ul << CLK_APBCLK_TMR1_EN_Pos)      /*!< CLK_T::APBCLK: TMR1_EN Mask */

#define CLK_APBCLK_TMR0_EN_Pos               2                                    /*!< CLK_T::APBCLK: TMR0_EN Position */
#define CLK_APBCLK_TMR0_EN_Msk               (1ul << CLK_APBCLK_TMR0_EN_Pos)      /*!< CLK_T::APBCLK: TMR0_EN Mask */

#define CLK_APBCLK_WDT_EN_Pos                0                                    /*!< CLK_T::APBCLK: WDT_EN Position */
#define CLK_APBCLK_WDT_EN_Msk                (1ul << CLK_APBCLK_WDT_EN_Pos)       /*!< CLK_T::APBCLK: WDT_EN Mask */

/* CLK APBCLK1 Bit Field Definitions */
#define CLK_APBCLK1_BPWM1_EN_Pos             19                                   /*!< CLK_T::APBCLK1: BPWM1_EN Position */
#define CLK_APBCLK1_BPWM1_EN_Msk             (1ul << CLK_APBCLK1_BPWM1_EN_Pos)    /*!< CLK_T::APBCLK1: BPWM1_EN Mask */

#define CLK_APBCLK1_BPWM0_EN_Pos             18                                   /*!< CLK_T::APBCLK1: BPWM0_EN Position */
#define CLK_APBCLK1_BPWM0_EN_Msk             (1ul << CLK_APBCLK1_BPWM0_EN_Pos)    /*!< CLK_T::APBCLK1: BPWM0_EN Mask */

#define CLK_APBCLK1_PWM1_EN_Pos              17                                   /*!< CLK_T::APBCLK1: PWM1_EN Position */
#define CLK_APBCLK1_PWM1_EN_Msk              (1ul << CLK_APBCLK1_PWM1_EN_Pos)     /*!< CLK_T::APBCLK1: PWM1_EN Mask */

#define CLK_APBCLK1_PWM0_EN_Pos              16                                   /*!< CLK_T::APBCLK1: PWM0_EN Position */
#define CLK_APBCLK1_PWM0_EN_Msk              (1ul << CLK_APBCLK1_PWM0_EN_Pos)     /*!< CLK_T::APBCLK1: PWM0_EN Mask */

#define CLK_APBCLK1_UART5_EN_Pos             10                                   /*!< CLK_T::APBCLK1: UART5_EN Position */
#define CLK_APBCLK1_UART5_EN_Msk             (1ul << CLK_APBCLK1_UART5_EN_Pos)    /*!< CLK_T::APBCLK1: UART5_EN Mask */

#define CLK_APBCLK1_UART4_EN_Pos             9                                    /*!< CLK_T::APBCLK1: UART4_EN Position */
#define CLK_APBCLK1_UART4_EN_Msk             (1ul << CLK_APBCLK1_UART4_EN_Pos)    /*!< CLK_T::APBCLK1: UART4_EN Mask */

#define CLK_APBCLK1_UART3_EN_Pos             8                                    /*!< CLK_T::APBCLK1: UART3_EN Position */
#define CLK_APBCLK1_UART3_EN_Msk             (1ul << CLK_APBCLK1_UART3_EN_Pos)    /*!< CLK_T::APBCLK1: UART3_EN Mask */

/* CLK CLKSTATUS Bit Field Definitions */
#define CLK_CLKSTATUS_CLK_SW_FAIL_Pos        7                                        /*!< CLK_T::CLKSTATUS: CLK_SW_FAIL Position */
#define CLK_CLKSTATUS_CLK_SW_FAIL_Msk        (1ul << CLK_CLKSTATUS_CLK_SW_FAIL_Pos)   /*!< CLK_T::CLKSTATUS: CLK_SW_FAIL Mask */

#define CLK_CLKSTATUS_OSC22M_STB_Pos         4                                        /*!< CLK_T::CLKSTATUS: OSC22M_STB Position */
#define CLK_CLKSTATUS_OSC22M_STB_Msk         (1ul << CLK_CLKSTATUS_OSC22M_STB_Pos)    /*!< CLK_T::CLKSTATUS: OSC22M_STB Mask */
#define CLK_CLKSTATUS_IRC22M_STB_Pos         4                                        /*!< CLK_T::CLKSTATUS: IRC22M_STB Position */
#define CLK_CLKSTATUS_IRC22M_STB_Msk         (1ul << CLK_CLKSTATUS_IRC22M_STB_Pos)    /*!< CLK_T::CLKSTATUS: IRC22M_STB Mask */

#define CLK_CLKSTATUS_OSC10K_STB_Pos         3                                        /*!< CLK_T::CLKSTATUS: OSC10K_STB Position */
#define CLK_CLKSTATUS_OSC10K_STB_Msk         (1ul << CLK_CLKSTATUS_OSC10K_STB_Pos)    /*!< CLK_T::CLKSTATUS: OSC10K_STB Mask */
#define CLK_CLKSTATUS_IRC10K_STB_Pos         3                                        /*!< CLK_T::CLKSTATUS: IRC10K_STB Position */
#define CLK_CLKSTATUS_IRC10K_STB_Msk         (1ul << CLK_CLKSTATUS_IRC10K_STB_Pos)    /*!< CLK_T::CLKSTATUS: IRC10K_STB Mask */

#define CLK_CLKSTATUS_PLL_STB_Pos            2                                        /*!< CLK_T::CLKSTATUS: PLL_STB Position */
#define CLK_CLKSTATUS_PLL_STB_Msk            (1ul << CLK_CLKSTATUS_PLL_STB_Pos)       /*!< CLK_T::CLKSTATUS: PLL_STB Mask */

#define CLK_CLKSTATUS_XTL12M_STB_Pos         0                                        /*!< CLK_T::CLKSTATUS: XTL12M_STB Position */
#define CLK_CLKSTATUS_XTL12M_STB_Msk         (1ul << CLK_CLKSTATUS_XTL12M_STB_Pos)    /*!< CLK_T::CLKSTATUS: XTL12M_STB Mask */

/* CLK CLKSEL0 Bit Field Definitions */
#define CLK_CLKSEL0_STCLK_S_Pos              3                                        /*!< CLK_T::CLKSEL0: STCLK_S Position */
#define CLK_CLKSEL0_STCLK_S_Msk              (7ul << CLK_CLKSEL0_STCLK_S_Pos)         /*!< CLK_T::CLKSEL0: STCLK_S Mask */

#define CLK_CLKSEL0_HCLK_S_Pos               0                                        /*!< CLK_T::CLKSEL0: HCLK_S Position */
#define CLK_CLKSEL0_HCLK_S_Msk               (7ul << CLK_CLKSEL0_HCLK_S_Pos)          /*!< CLK_T::CLKSEL0: HCLK_S Mask */

/* CLK CLKSEL1 Bit Field Definitions */
#define CLK_CLKSEL1_UART_S_Pos               24                                       /*!< CLK_T::CLKSEL1: UART_S Position */
#define CLK_CLKSEL1_UART_S_Msk               (3ul << CLK_CLKSEL1_UART_S_Pos)          /*!< CLK_T::CLKSEL1: UART_S Mask */

#define CLK_CLKSEL1_TMR3_S_Pos               20                                       /*!< CLK_T::CLKSEL1: TMR3_S Position */
#define CLK_CLKSEL1_TMR3_S_Msk               (7ul << CLK_CLKSEL1_TMR3_S_Pos)          /*!< CLK_T::CLKSEL1: TMR3_S Mask */

#define CLK_CLKSEL1_TMR2_S_Pos               16                                       /*!< CLK_T::CLKSEL1: TMR2_S Position */
#define CLK_CLKSEL1_TMR2_S_Msk               (7ul << CLK_CLKSEL1_TMR2_S_Pos)          /*!< CLK_T::CLKSEL1: TMR2_S Mask */

#define CLK_CLKSEL1_TMR1_S_Pos               12                                       /*!< CLK_T::CLKSEL1: TMR1_S Position */
#define CLK_CLKSEL1_TMR1_S_Msk               (7ul << CLK_CLKSEL1_TMR1_S_Pos)          /*!< CLK_T::CLKSEL1: TMR1_S Mask */

#define CLK_CLKSEL1_TMR0_S_Pos               8                                        /*!< CLK_T::CLKSEL1: TMR0_S Position */
#define CLK_CLKSEL1_TMR0_S_Msk               (7ul << CLK_CLKSEL1_TMR0_S_Pos)          /*!< CLK_T::CLKSEL1: TMR0_S Mask */

#define CLK_CLKSEL1_SPI0_S_Pos               4                                        /*!< CLK_T::CLKSEL1: SPI0_S Position */
#define CLK_CLKSEL1_SPI0_S_Msk               (1ul << CLK_CLKSEL1_SPI0_S_Pos)          /*!< CLK_T::CLKSEL1: SPI0_S Mask */

#define CLK_CLKSEL1_ADC_S_Pos                2                                        /*!< CLK_T::CLKSEL1: ADC_S Position */
#define CLK_CLKSEL1_ADC_S_Msk                (3ul << CLK_CLKSEL1_ADC_S_Pos)           /*!< CLK_T::CLKSEL1: ADC_S Mask */

#define CLK_CLKSEL1_WDT_S_Pos                0                                        /*!< CLK_T::CLKSEL1: WDT_S Position */
#define CLK_CLKSEL1_WDT_S_Msk                (3ul << CLK_CLKSEL1_WDT_S_Pos)           /*!< CLK_T::CLKSEL1: WDT_S Mask */

/* CLK CLKSEL2 Bit Field Definitions */
#define CLK_CLKSEL2_WWDT_S_Pos               16                                       /*!< CLK_T::CLKSEL2: WWDT_S Position */
#define CLK_CLKSEL2_WWDT_S_Msk               (3ul << CLK_CLKSEL2_WWDT_S_Pos)          /*!< CLK_T::CLKSEL2: WWDT_S Mask */

#define CLK_CLKSEL2_FRQDIV_S_Pos             2                                        /*!< CLK_T::CLKSEL2: FRQDIV_S Position */
#define CLK_CLKSEL2_FRQDIV_S_Msk             (3ul << CLK_CLKSEL2_FRQDIV_S_Pos)        /*!< CLK_T::CLKSEL2: FRQDIV_S Mask */

/* CLK CLKSEL3 Bit Field Definitions */
#define CLK_CLKSEL3_BPWM1_S_Pos              19                                       /*!< CLK_T::CLKSEL3: BPWM1_S Position */
#define CLK_CLKSEL3_BPWM1_S_Msk              (1ul << CLK_CLKSEL3_BPWM1_S_Pos)         /*!< CLK_T::CLKSEL3: BPWM1_S Mask */

#define CLK_CLKSEL3_BPWM0_S_Pos              18                                       /*!< CLK_T::CLKSEL3: BPWM0_S Position */
#define CLK_CLKSEL3_BPWM0_S_Msk              (1ul << CLK_CLKSEL3_BPWM0_S_Pos)         /*!< CLK_T::CLKSEL3: BPWM0_S Mask */

#define CLK_CLKSEL3_PWM1_S_Pos               17                                       /*!< CLK_T::CLKSEL3: PWM1_S Position */
#define CLK_CLKSEL3_PWM1_S_Msk               (1ul << CLK_CLKSEL3_PWM1_S_Pos)          /*!< CLK_T::CLKSEL3: PWM1_S Mask */

#define CLK_CLKSEL3_PWM0_S_Pos               16                                       /*!< CLK_T::CLKSEL3: PWM0_S Position */
#define CLK_CLKSEL3_PWM0_S_Msk               (1ul << CLK_CLKSEL3_PWM0_S_Pos)          /*!< CLK_T::CLKSEL3: PWM0_S Mask */

/* CLK CLKDIV Bit Field Definitions */
#define CLK_CLKDIV_ADC_N_Pos                 16                                       /*!< CLK_T::CLKDIV: ADC_N Position */
#define CLK_CLKDIV_ADC_N_Msk                 (0xFFul << CLK_CLKDIV_ADC_N_Pos)         /*!< CLK_T::CLKDIV: ADC_N Mask */

#define CLK_CLKDIV_UART_N_Pos                8                                        /*!< CLK_T::CLKDIV: UART_N Position */
#define CLK_CLKDIV_UART_N_Msk                (0xFul << CLK_CLKDIV_UART_N_Pos)         /*!< CLK_T::CLKDIV: UART_N Mask */

#define CLK_CLKDIV_HCLK_N_Pos                0                                        /*!< CLK_T::CLKDIV: HCLK_N Position */
#define CLK_CLKDIV_HCLK_N_Msk                (0xFul << CLK_CLKDIV_HCLK_N_Pos)         /*!< CLK_T::CLKDIV: HCLK_N Mask */

/* CLK PLLCON Bit Field Definitions */
#define CLK_PLLCON_PLL_SRC_Pos               19                                       /*!< CLK_T::PLLCON: PLL_SRC Position */
#define CLK_PLLCON_PLL_SRC_Msk               (1ul << CLK_PLLCON_PLL_SRC_Pos)          /*!< CLK_T::PLLCON: PLL_SRC Mask */

#define CLK_PLLCON_OE_Pos                    18                                       /*!< CLK_T::PLLCON: PLL_SRC Position */
#define CLK_PLLCON_OE_Msk                    (1ul << CLK_PLLCON_OE_Pos)               /*!< CLK_T::PLLCON: PLL_SRC Mask */

#define CLK_PLLCON_BP_Pos                    17                                       /*!< CLK_T::PLLCON: OE Position */
#define CLK_PLLCON_BP_Msk                    (1ul << CLK_PLLCON_BP_Pos)               /*!< CLK_T::PLLCON: OE Mask */

#define CLK_PLLCON_PD_Pos                    16                                       /*!< CLK_T::PLLCON: PD Position */
#define CLK_PLLCON_PD_Msk                    (1ul << CLK_PLLCON_PD_Pos)               /*!< CLK_T::PLLCON: PD Mask */

#define CLK_PLLCON_OUT_DV_Pos                14                                       /*!< CLK_T::PLLCON: OUT_DV Position */
#define CLK_PLLCON_OUT_DV_Msk                (3ul << CLK_PLLCON_OUT_DV_Pos)           /*!< CLK_T::PLLCON: OUT_DV Mask */

#define CLK_PLLCON_IN_DV_Pos                 9                                        /*!< CLK_T::PLLCON: IN_DV Position */
#define CLK_PLLCON_IN_DV_Msk                 (0x1Ful << CLK_PLLCON_IN_DV_Pos)         /*!< CLK_T::PLLCON: IN_DV Mask */

#define CLK_PLLCON_FB_DV_Pos                 0                                        /*!< CLK_T::PLLCON: FB_DV Position */
#define CLK_PLLCON_FB_DV_Msk                 (0x1FFul << CLK_PLLCON_FB_DV_Pos)        /*!< CLK_T::PLLCON: FB_DV Mask */

/* CLK FRQDIV Bit Field Definitions */
#define CLK_FRQDIV_DIVIDER1_Pos              5                                        /*!< CLK_T::FRQDIV: DIVIDER1 Position */
#define CLK_FRQDIV_DIVIDER1_Msk              (1ul << CLK_FRQDIV_DIVIDER1_Pos)         /*!< CLK_T::FRQDIV: DIVIDER1 Mask */

#define CLK_FRQDIV_DIVIDER_EN_Pos            4                                        /*!< CLK_T::FRQDIV: DIVIDER_EN Position */
#define CLK_FRQDIV_DIVIDER_EN_Msk            (1ul << CLK_FRQDIV_DIVIDER_EN_Pos)       /*!< CLK_T::FRQDIV: DIVIDER_EN Mask */

#define CLK_FRQDIV_FSEL_Pos                  0                                        /*!< CLK_T::FRQDIV: FRQDIV_FSEL Position */
#define CLK_FRQDIV_FSEL_Msk                  (0xFul << CLK_FRQDIV_FSEL_Pos)           /*!< CLK_T::FRQDIV: FRQDIV_FSEL Mask */

/* CLK CLKDCTL Bit Field Definitions */
#define CLK_CLKDCTL_HXTFQIEN_Pos           17                                         /*!< CLK_T::CLKDCTL: HXTFQIEN Position */
#define CLK_CLKDCTL_HXTFQIEN_Msk           (1ul << CLK_CLKDCTL_HXTFQIEN_Pos)          /*!< CLK_T::CLKDCTL: HXTFQIEN Mask */

#define CLK_CLKDCTL_HXTFQDEN_Pos           16                                         /*!< CLK_T::CLKDCTL: HXTFQDEN Position */
#define CLK_CLKDCTL_HXTFQDEN_Msk           (1ul << CLK_CLKDCTL_HXTFQDEN_Pos)          /*!< CLK_T::CLKDCTL: HXTFQDEN Mask */

#define CLK_CLKDCTL_HXTFIEN_Pos           5                                           /*!< CLK_T::CLKDCTL: HXTFIEN Position */
#define CLK_CLKDCTL_HXTFIEN_Msk           (1ul << CLK_CLKDCTL_HXTFIEN_Pos)            /*!< CLK_T::CLKDCTL: HXTFIEN Mask */
                                                                                    
#define CLK_CLKDCTL_HXTFDEN_Pos           4                                           /*!< CLK_T::CLKDCTL: HXTFDEN Position */
#define CLK_CLKDCTL_HXTFDEN_Msk           (1ul << CLK_CLKDCTL_HXTFDEN_Pos)            /*!< CLK_T::CLKDCTL: HXTFDEN Mask */

/* CLK CLKDSTS Bit Field Definitions */
#define CLK_CLKDSTS_HXTFQIF_Pos           8                                           /*!< CLK_T::CLKDSTS: HXTFQIF Position */
#define CLK_CLKDSTS_HXTFQIF_Msk           (1ul << CLK_CLKDSTS_HXTFQIF_Pos)            /*!< CLK_T::CLKDSTS: HXTFQIF Mask */

#define CLK_CLKDSTS_HXTFIF_Pos           0                                            /*!< CLK_T::CLKDSTS: HXTFIF Position */
#define CLK_CLKDSTS_HXTFIF_Msk           (1ul << CLK_CLKDSTS_HXTFIF_Pos)              /*!< CLK_T::CLKDSTS: HXTFIF Mask */

/* CLK CDUPB Bit Field Definitions */
#define CLK_CDUPB_UPERBD_Pos           0                                             /*!< CLK_T::CDUPB: UPERBD Position */
#define CLK_CDUPB_UPERBD_Msk           (0x3Ful << CLK_CDUPB_UPERBD_Pos)              /*!< CLK_T::CDUPB: UPERBD Mask */

/* CLK CDLOWB Bit Field Definitions */
#define CLK_CDLOWB_UPERBD_Pos           0                                            /*!< CLK_T::CDLOWB: LOWRBD Position */
#define CLK_CDLOWB_UPERBD_Msk           (0x3Ful << CLK_CDLOWB_LOWERBD_Pos)           /*!< CLK_T::CDLOWB: LOWERBD Mask */

/*@}*/ /* end of group CLK_CONST */
/*@}*/ /* end of group CLK */




/*---------------------- Flash Memory Controller -------------------------*/
/**
    @addtogroup FMC Flash Memory Controller (FMC)
    Memory Mapped Structure for FMC Controller
@{ */
typedef struct
{
    /**
     * ISPCON
     * ===================================================================================================
     * Offset: 0x00  ISP Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ISPEN     |ISP Enable (Write Protect)
     * |        |          |ISP function enable bit. Set this bit to enable ISP function.
     * |        |          |0 = ISP function Disabled.     
     * |        |          |1 = ISP function Enabled.
     * |[1]     |BS        |Boot Select (Write Protect)
     * |        |          |Set/clear this bit to select next booting from LDROM/APROM,
     * |        |          |respectively. This bit also functions as MCU booting status flag, which can be used to check where
     * |        |          |MCU booted from. This bit is initiated with the inverted value of CBS in Config0 after power-
     * |        |          |on reset; It keeps the same value at other reset.
     * |        |          |0 = boot from APROM     
     * |        |          |1 = boot from LDROM
     * |[3]     |APUEN     |APROM Update Enable Bit (Write Protect)
     * |        |          |0 = APROM cannot be updated when the chip runs in APROM.
     * |        |          |1 = APROM can be updated when the chip runs in APROM.
     * |[4]     |CFGUEN    |Config Update Enable (Write Protect)
     * |        |          |Writing this bit to 1 enables s/w to update Config value by ISP procedure regardless of program
     * |        |          |code is running in APROM or LDROM.
     * |        |          |0 = Config update disable     
     * |        |          |1 = Config update enable
     * |[5]     |LDUEN     |LDROM Update Enable (Write Protect)
     * |        |          |LDROM update enable bit.
     * |        |          |0 = LDROM cannot be updated
     * |        |          |1 = LDROM can be updated when chip runs in APROM.     
     * |[6]     |ISPFF     |ISP Fail Flag (Write Protect)
     * |        |          |This bit is set by hardware when a triggered ISP meets any of the following conditions:
     * |        |          |(1) APROM writes to itself if APUEN is set to 0.
     * |        |          |(2) LDROM writes to itself if LDUEN is set to 0.
     * |        |          |(3) CONFIG is erased/programmed if CFGUEN is set to 0.
     * |        |          |(4) Destination address is illegal, such as over an available range.
     * |        |          |Note: Write 1 to clear.
     */
    __IO uint32_t ISPCON;

    /**
     * ISPADR
     * ===================================================================================================
     * Offset: 0x04  ISP Address Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |ISPADR    |ISP Address
     * |        |          |NuMicro NUC131 series has a maximum of 17Kx32(68KB) embedded Flash, which supports word program only.
     * |        |          |ISPADR[1:0] must be kept 00b for ISP operation.
     */
    __IO uint32_t ISPADR;

    /**
     * ISPDAT
     * ===================================================================================================
     * Offset: 0x08  ISP Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |ISPDAT    |ISP Data
     * |        |          |Write data to this register before ISP program operation
     * |        |          |Read data from this register after ISP read operation
     */
    __IO uint32_t ISPDAT;

    /**
     * ISPCMD
     * ===================================================================================================
     * Offset: 0x0C  ISP Command Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |ISPCMD    |ISP Command
     * |        |          |ISP command table is shown below:
     * |        |          |0x00 = Read.
     * |        |          |0x04 = Read Unique ID.
     * |        |          |0x0B = Read Company ID (0xDA).
     * |        |          |0x21 = Program.
     * |        |          |0x22 = Page Erase.
     * |        |          |0x2E = Set Vector Page Re-Map.
     */
    __IO uint32_t ISPCMD;

    /**
     * ISPTRG
     * ===================================================================================================
     * Offset: 0x10  IISP Trigger Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ISPGO     |ISP Start Trigger (Write Protect)
     * |        |          |Write 1 to start ISP operation and this bit will be cleared to 0 by hardware automatically when ISP
     * |        |          |operation is finish.
     * |        |          |0 = ISP operation is finished.
     * |        |          |1 = ISP is in progress.     
     */
    __IO uint32_t ISPTRG;

    /**
     * DFBADR
     * ===================================================================================================
     * Offset: 0x14  Data Flash Base Address Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |DFBA      |Data Flash Base Address
     * |        |          |This register indicates data flash start address.
     * |        |          |It is a read only register.
     * |        |          |When DFVSEN is set to 0, the data flash is shared with APROM. The data flash size is defined by user configuration and the content of this register is loaded from Config1.
     * |        |          |When DFVSEN is set to 1, the data flash size is fixed as 4K and the start address can be read from this register is fixed at 0x0001_F000.
     */
    __I  uint32_t DFBADR;

    /**
     * FATCON
     * ===================================================================================================
     * Offset: 0x18  Flash Access Time Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |FOMSEL0   |Chip Frequency Optimization Mode Select 0 (Write Protect)
     * |        |          |When CPU frequency is lower than 25MHz, user can modify flash access delay cycle by FOMSEL1 and FOMSEL0 to improve system performance.
     * |        |          |00 = CPU runs up to 50MHz with zero wait cycle for continuous address read access.
     * |        |          |01 = CPU runs up to 25MHz with zero wait cycle for random address read access.
     * |        |          |10 = Reserved.
     * |        |          |11 = Reserved.
     * |        |          |Note: Where 00 means FOMSEL1=0, FOMSEL0=0; 01 means FOMSEL1 = 0, FOMSEL0=1 and etc.
     * |[6]     |FOMSEL1   |Chip Frequency Optimization Mode Select 1 (Write Protect)
     * |        |          |When CPU frequency is lower than 25MHz, user can modify flash access delay cycle by FOMSEL1 and FOMSEL0 to improve system performance.
     * |        |          |00 = CPU runs up to 50MHz with zero wait cycle for continuous address read access.
     * |        |          |01 = CPU runs up to 25MHz with zero wait cycle for random address read access.
     * |        |          |10 = Reserved.
     * |        |          |11 = Reserved.
     * |        |          |Note: Where 00 means FOMSEL1=0, FOMSEL0=0; 01 means FOMSEL1 = 0, FOMSEL0=1 and etc.
     */
    __IO uint32_t FATCON;

    __I  uint32_t  RESERVED[9];

    /**
     * ISPSTA
     * ===================================================================================================
     * Offset: 0x40  ISP Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ISPGO     |ISP Start Trigger (Read Only)
     * |        |          |Write 1 to start ISP operation and this bit will be cleared to 0 by hardware automatically when ISP operation is finished.
     * |        |          |This bit is the mirror of ISPGO(FMC_ISPTRG[0]).
     * |        |          |0 = ISP operation is finished.
     * |        |          |1 = ISP operation is progressed.
     * |[2:1]   |CBS       |Boot Selection Of CONFIG (Read Only)
     * |        |          |This bit is initiated with the CBS (CONFIG0[7:6]) after any reset is happened except CPU reset (CPU is 1) or system reset (SYS) is happened.
     * |        |          |00 = LDROM with IAP mode.
     * |        |          |01 = LDROM without IAP mode.
     * |        |          |10 = APROM with IAP mode.
     * |        |          |11 = APROM without IAP mode.
     * |[6]     |ISPFF     |ISP Fail Flag (Write Protect)
     * |        |          |This bit is the mirror of ISPFF (FMC_ISPCON[6]), it needs to be cleared by writing 1 to FMC_ISPCON[6] or FMC_ISPSTA[6].
     * |        |          |This bit is set by hardware when a triggered ISP meets any of the following conditions:
     * |        |          |(1) APROM writes to itself if APUEN is set to 0.
     * |        |          |(2) LDROM writes to itself if LDUEN is set to 0.
     * |        |          |(3) CONFIG is erased/programmed if CFGUEN is set to 0.
     * |        |          |(4) Destination address is illegal, such as over an available range.
     * |[20:9]  |VECMAP    |Vector Page Mapping Address (Read Only)
     * |        |          |All access to 0x0000_0000~0x0000_01FF is remapped to the flash memory address {VECMAP[11:0], 9'h000} ~ {VECMAP[11:0], 9'h1FF}
    */
    __IO uint32_t ISPSTA;

} FMC_T;

/**
    @addtogroup FMC_CONST FMC Bit Field Definition
    Constant Definitions for FMC Controller
@{ */


/* FMC ISPCON Bit Field Definitions */
#define FMC_ISPCON_ISPFF_Pos                    6                                       /*!< FMC_T::ISPCON: ISPFF Position */
#define FMC_ISPCON_ISPFF_Msk                    (1ul << FMC_ISPCON_ISPFF_Pos)           /*!< FMC_T::ISPCON: ISPFF Mask */

#define FMC_ISPCON_LDUEN_Pos                    5                                       /*!< FMC_T::ISPCON: LDUEN Position */
#define FMC_ISPCON_LDUEN_Msk                    (1ul << FMC_ISPCON_LDUEN_Pos)           /*!< FMC_T::ISPCON: LDUEN Mask */

#define FMC_ISPCON_CFGUEN_Pos                   4                                       /*!< FMC_T::ISPCON: CFGUEN Position */
#define FMC_ISPCON_CFGUEN_Msk                   (1ul << FMC_ISPCON_CFGUEN_Pos)          /*!< FMC_T::ISPCON: CFGUEN Mask */

#define FMC_ISPCON_APUEN_Pos                    3                                       /*!< FMC_T::ISPCON: APUEN Position */
#define FMC_ISPCON_APUEN_Msk                    (1ul << FMC_ISPCON_APUEN_Pos)           /*!< FMC_T::ISPCON: APUEN Mask */

#define FMC_ISPCON_BS_Pos                       1                                       /*!< FMC_T::ISPCON: BS Position */
#define FMC_ISPCON_BS_Msk                       (0x1ul << FMC_ISPCON_BS_Pos)            /*!< FMC_T::ISPCON: BS Mask */

#define FMC_ISPCON_ISPEN_Pos                    0                                       /*!< FMC_T::ISPCON: ISPEN Position */
#define FMC_ISPCON_ISPEN_Msk                    (1ul << FMC_ISPCON_ISPEN_Pos)           /*!< FMC_T::ISPCON: ISPEN Mask */

/* FMC ISPADR Bit Field Definitions */
#define FMC_ISPADR_ISPADR_Pos                   0                                       /*!< FMC_T::ISPADR: ISPADR Position */
#define FMC_ISPADR_ISPADR_Msk                   (0xFFFFFFFFul << FMC_ISPADR_ISPADR_Pos) /*!< FMC_T::ISPADR: ISPADR Mask */

/* FMC ISPDAT Bit Field Definitions */
#define FMC_ISPDAT_ISPDAT_Pos                   0                                       /*!< FMC_T::ISPDAT: ISPDAT Position */
#define FMC_ISPDAT_ISPDAT_Msk                   (0xFFFFFFFFul << FMC_ISPDAT_ISPDAT_Pos) /*!< FMC_T::ISPDAT: ISPDAT Mask */

/* FMC ISPCMD Bit Field Definitions */
#define FMC_ISPCMD_ISPCMD_Pos                   0                                       /*!< FMC_T::ISPCMD: ISPCMD Position */
#define FMC_ISPCMD_ISPCMD_Msk                   (0xFul << FMC_ISPCMD_ISPCMD_Pos)        /*!< FMC_T::ISPCMD: ISPCMD Mask */

/* FMC ISPTRG Bit Field Definitions */
#define FMC_ISPTRG_ISPGO_Pos                    0                                       /*!< FMC_T::ISPTRG: ISPGO Position */
#define FMC_ISPTRG_ISPGO_Msk                    (1ul << FMC_ISPTRG_ISPGO_Pos)           /*!< FMC_T::ISPTRG: ISPGO Mask */

/* FMC DFBADR Bit Field Definitions */
#define FMC_DFBADR_DFBA_Pos                     0                                       /*!< FMC_T::DFBADR: DFBA Position */
#define FMC_DFBADR_DFBA_Msk                     (0xFFFFFFFFul << FMC_DFBADR_DFBA_Pos)   /*!< FMC_T::DFBADR: DFBA Mask */

/* FMC FATCON Bit Field Definitions */
#define FMC_FATCON_FOMSEL1_Pos                  6                                       /*!< FMC_T::FATCON: FOMSEL1 Position */
#define FMC_FATCON_FOMSEL1_Msk                  (1ul << FMC_FATCON_FOMSEL1_Pos)         /*!< FMC_T::FATCON: FOMSEL1 Mask */

#define FMC_FATCON_FOMSEL0_Pos                  4                                       /*!< FMC_T::FATCON: FOMSEL0 Position */
#define FMC_FATCON_FOMSEL0_Msk                  (1ul << FMC_FATCON_FOMSEL0_Pos)         /*!< FMC_T::FATCON: FOMSEL0 Mask */

/* FMC ISPSTA Bit Field Definitions */
#define FMC_ISPSTA_VECMAP_Pos                   9                                       /*!< FMC_T::ISPSTA: VECMAP Position */
#define FMC_ISPSTA_VECMAP_Msk                   (0xFFFul << FMC_ISPSTA_VECMAP_Pos)      /*!< FMC_T::ISPSTA: VECMAP Mask */

#define FMC_ISPSTA_ISPFF_Pos                    6                                       /*!< FMC_T::ISPSTA: ISPFF Position */
#define FMC_ISPSTA_ISPFF_Msk                    (0x3ul << FMC_ISPSTA_ISPFF_Pos)         /*!< FMC_T::ISPSTA: ISPFF Mask */

#define FMC_ISPSTA_CBS_Pos                      1                                       /*!< FMC_T::ISPSTA: CBS Position */
#define FMC_ISPSTA_CBS_Msk                      (0x3ul << FMC_ISPSTA_CBS_Pos)           /*!< FMC_T::ISPSTA: CBS Mask */

#define FMC_ISPSTA_ISPGO_Pos                    0                                       /*!< FMC_T::ISPSTA: ISPGO Position */
#define FMC_ISPSTA_ISPGO_Msk                    (1ul << FMC_ISPSTA_ISPGO_Pos)           /*!< FMC_T::ISPSTA: ISPGO Mask */
/*@}*/ /* end of group FMC_CONST */
/*@}*/ /* end of group FMC */





/*--------------------- General Purpose I/O (GPIO) ---------------------*/
/** @addtogroup GPIO General purpose I/O (GPIO)
  Memory Mapped Structure for NUC131 Series General Purpose I/O
  @{
 */
typedef struct
{
    /**
     * GPIOx_PMD
     * ===================================================================================================
     * Offset: 0x00  GPIO Port [A/B/C/D/E/F] Pin I/O Mode Control
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2n+1:2n]|PMDn     |GPIOx I/O Pin[n] Mode Control
     * |        |          |Determine each I/O mode of GPIOx pins.
     * |        |          |00 = GPIO port [n] pin is in Input mode.
     * |        |          |01 = GPIO port [n] pin is in Push-pull Output mode.
     * |        |          |10 = GPIO port [n] pin is in Open-drain Output mode.
     * |        |          |11 = GPIO port [n] pin is in Quasi-bidirectional mode.
     * |        |          |Note1:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     * |        |          |Note2:
     * |        |          |The initial value of this field is defined by CIOINI (CONFIG0[10]).
     * |        |          |If CIOINI is set to 1, the default value is 0xFFFF_FFFF and all pins will be Quasi-bidirectional mode after chip is powered on.
     * |        |          |If CIOINI is cleared to 0, the default value is 0x0000_0000 and all pins will be input only mode after chip is powered on.
     */
    __IO uint32_t  PMD;

    /**
     * GPIOx_OFFD
     * ===================================================================================================
     * Offset: 0x04  GPIO Port [A/B/C/D/E/F] Pin Digital Input Path Disable Control
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:16] |OFFD      |GPIOx Pin[n] Digital Input Path Disable Control
     * |        |          |Each of these bits is used to control if the digital input path of corresponding GPIO pin is disabled.
     * |        |          |If input is analog signal, users can disable GPIO digital input path to avoid current leakage.
     * |        |          |0 = I/O digital input path Enabled.
     * |        |          |1 = I/O digital input path Disabled (digital input tied to low).
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  OFFD;

    /**
     * GPIOx_DOUT
     * ===================================================================================================
     * Offset: 0x08  GPIO Port [A/B/C/D/E/F] Data Output Value
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |DOUTn     |GPIOx Pin[n] Output Value
     * |        |          |Each of these bits controls the status of a GPIO pin when the GPIO pin is configured as Push-pull output, open-drain output or quasi-bidirectional mode.
     * |        |          |0 = GPIO port [A/B/C/D/E/F] Pin[n] will drive Low if the GPIO pin is configured as Push-pull output, Open-drain output or Quasi-bidirectional mode.
     * |        |          |1 = GPIO port [A/B/C/D/E/F] Pin[n] will drive High if the GPIO pin is configured as Push-pull output or Quasi-bidirectional mode.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  DOUT;

    /**
     * GPIOx_DMASK
     * ===================================================================================================
     * Offset: 0x0C  GPIO Port [A/B/C/D/E/F] Data Output Write Mask
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |DMASKn    |Port [A/B/C/D/E/F] Data Output Write Mask
     * |        |          |These bits are used to protect the corresponding register of GPIOx_DOUT bit[n].
     * |        |          |When the DMASK bit[n] is set to 1, the corresponding GPIOx_DOUT[n] bit is protected.
     * |        |          |If the write signal is masked, write data to the protect bit is ignored.
     * |        |          |0 = Corresponding GPIOx_DOUT[n] bit can be updated.
     * |        |          |1 = Corresponding GPIOx_DOUT[n] bit protected.
     * |        |          |Note1: This function only protects the corresponding GPIOx_DOUT[n] bit, and will not protect the corresponding bit control register (GPIOAx_DOUT, GPIOBx_DOUT, GPIOCx_DOUT, GPIODx_DOUT, GPIOEx_DOUT and GPIOFx_DOUT).
     * |        |          |Note2:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  DMASK;

    /**
     * GPIOx_PIN
     * ===================================================================================================
     * Offset: 0x10  GPIO Port [A/B/C/D/E/F] Pin Value
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |PINn      |Port [A/B/C/D/E/F] Pin Values
     * |        |          |Each bit of the register reflects the actual status of the respective GPIO pin.
     * |        |          |If the bit is 1, it indicates the corresponding pin status is high, else the pin status is low.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __I  uint32_t  PIN;

    /**
     * GPIOx_DBEN
     * ===================================================================================================
     * Offset: 0x14  GPIO Port [A/B/C/D/E/F] De-bounce Enable
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |DBENn     |Port [A/B/C/D/E/F] Input Signal De-Bounce Enable
     * |        |          |DBEN[n] is used to enable the de-bounce function for each corresponding bit.
     * |        |          |If the input signal pulse width cannot be sampled by continuous two de-bounce sample cycle, the input signal transition is seen as the signal bounce and will not trigger the interrupt.
     * |        |          |The de-bounce clock source is controlled by DBNCECON[4], one de-bounce sample cycle period is controlled by DBNCECON[3:0].
     * |        |          |0 = Bit[n] de-bounce function Disabled.
     * |        |          |1 = Bit[n] de-bounce function Enabled.
     * |        |          |The de-bounce function is valid only for edge triggered interrupt.
     * |        |          |If the interrupt mode is level triggered, the de-bounce enable bit is ignored.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  DBEN;

    /**
     * GPIOA_IMD
     * ===================================================================================================
     * Offset: 0x18  GPIO Port [A/B/C/D/E/F] Interrupt Mode Control
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |IMDn      |Port [A/B/C/D/E/F] Edge Or Level Detection Interrupt Control
     * |        |          |IMD[n] is used to control the interrupt is by level trigger or by edge trigger.
     * |        |          |If the interrupt is by edge trigger, the trigger source can be controlled by de-bounce.
     * |        |          |If the interrupt is by level trigger, the input source is sampled by one HCLK.
     * |        |          |clock and generates the interrupt.
     * |        |          |0 = Edge trigger interrupt.
     * |        |          |1 = Level trigger interrupt.
     * |        |          |If the pin is set as the level trigger interrupt, only one level can be set on the registers GPIOx_IEN.
     * |        |          |If both levels to trigger interrupt are set, the setting is ignored and no interrupt will occur.
     * |        |          |The de-bounce function is valid only for edge triggered interrupt.
     * |        |          |If the interrupt mode is level triggered, the de-bounce enable bit is ignored.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  IMD;

    /**
     * GPIOx_IEN
     * ===================================================================================================
     * Offset: 0x1C  GPIO Port [A/B/C/D/E/F] Interrupt Enable
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |IF_ENn    |Port [A/B/C/D/E/F] Interrupt Enable By Input Falling Edge Or Input Level Low
     * |        |          |IF_EN[n] is used to enable the interrupt for each of the corresponding input GPIO_PIN[n].
     * |        |          |Set bit to 1 also enable the pin wake-up function.
     * |        |          |When setting the IF_EN[n] bit to 1:
     * |        |          |If the interrupt is level trigger, the input PIN[n] state at level "low" will generate the interrupt.
     * |        |          |If the interrupt is edge trigger, the input PIN[n] state change from "high-to-low" will generate the interrupt.
     * |        |          |0 = PIN[n] state low-level or high-to-low change interrupt Disabled.
     * |        |          |1 = PIN[n] state low-level or high-to-low change interrupt Enabled.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     * |[n+16]  |IR_ENn    |Port [A/B/C/D/E/F] Interrupt Enable By Input Rising Edge Or Input Level High
     * |        |          |IR_EN[n] used to enable the interrupt for each of the corresponding input GPIO_PIN[n].
     * |        |          |Set bit to 1 also enable the pin wake-up function.
     * |        |          |When setting the IR_EN[n] bit to 1:
     * |        |          |If the interrupt is level trigger, the input PIN[n] state at level "high" will generate the interrupt.
     * |        |          |If the interrupt is edge trigger, the input PIN[n] state change from "low-to-high" will generate the interrupt.
     * |        |          |0 = PIN[n] level-high or low-to-high interrupt Disabled.
     * |        |          |1 = PIN[n] level-high or low-to-high interrupt Enabled.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  IEN;

    /**
     * GPIOx_ISRC
     * ===================================================================================================
     * Offset: 0x20  GPIO Port [A/B/C/D/E/F] Interrupt Source Flag
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[n]     |ISRCn     |Port [A/B/C/D/E/F] Interrupt Source Flag
     * |        |          |Read :
     * |        |          |0 = No interrupt at GPIOx[n].
     * |        |          |1 = GPIOx[n] generates an interrupt.
     * |        |          |Write :
     * |        |          |0= No action.
     * |        |          |1= Clear the corresponding pending interrupt.
     * |        |          |Note:
     * |        |          |n = 0~15 for GPIOA/GPIOB;
     * |        |          |n = 0~3, 6~11, 14, 15 for GPIOC;
     * |        |          |n = 6, 7, 14, 15 for GPIOD;
     * |        |          |n = 5 for GPIOE;
     * |        |          |n = 0, 1, 4~8 for GPIOF.
     */
    __IO uint32_t  ISRC;
} GPIO_T;

typedef struct
{
    /**
     * DBNCECON
     * ===================================================================================================
     * Offset: 0x180  External Interrupt De-bounce Control
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |DBCLKSEL  |De-Bounce Sampling Cycle Selection
     * |        |          |0000 = Sample interrupt input once per 1 clocks
     * |        |          |0001 = Sample interrupt input once per 2 clocks
     * |        |          |0010 = Sample interrupt input once per 4 clocks
     * |        |          |0011 = Sample interrupt input once per 8 clocks
     * |        |          |0100 = Sample interrupt input once per 16 clocks
     * |        |          |0101 = Sample interrupt input once per 32 clocks
     * |        |          |0110 = Sample interrupt input once per 64 clocks
     * |        |          |0111 = Sample interrupt input once per 128 clocks
     * |        |          |1000 = Sample interrupt input once per 256 clocks
     * |        |          |1001 = Sample interrupt input once per 2*256 clocks
     * |        |          |1010 = Sample interrupt input once per 4*256clocks
     * |        |          |1011 = Sample interrupt input once per 8*256 clocks
     * |        |          |1100 = Sample interrupt input once per 16*256 clocks
     * |        |          |1101 = Sample interrupt input once per 32*256 clocks
     * |        |          |1110 = Sample interrupt input once per 64*256 clocks
     * |        |          |1111 = Sample interrupt input once per 128*256 clocks
     * |[4]     |DBCLKSRC  |De-Bounce Counter Clock Source Selection
     * |        |          |0 = De-bounce counter clock source is the HCLK.
     * |        |          |1 = De-bounce counter clock source is the internal 10 kHz low speed oscillator.
     * |[5]     |ICLK_ON   |Interrupt Clock On Mode
     * |        |          |0 = Edge detection circuit is active only if I/O pin corresponding GPIOx_IEN bit is set to 1.
     * |        |          |1 = All I/O pins edge detection circuit is always active after reset.
     * |        |          |It is recommended to turn off this bit to save system power if no special application concern.
     */
    __IO uint32_t  DBNCECON;
} GPIO_DBNCECON_T;

/** @addtogroup GPIO_CONST GPIO Bit Field Definition
    Constant Definitions for GPIO Controller
  @{
 */

/* GPIO PMD Bit Field Definitions */
#define GPIO_PMD_PMD15_Pos          30                                          /*!< GPIO_T::PMD: PMD15 Position */
#define GPIO_PMD_PMD15_Msk          (0x3ul << GPIO_PMD_PMD15_Pos)               /*!< GPIO_T::PMD: PMD15 Mask */

#define GPIO_PMD_PMD14_Pos          28                                          /*!< GPIO_T::PMD: PMD14 Position */
#define GPIO_PMD_PMD14_Msk          (0x3ul << GPIO_PMD_PMD14_Pos)               /*!< GPIO_T::PMD: PMD14 Mask */

#define GPIO_PMD_PMD13_Pos          26                                          /*!< GPIO_T::PMD: PMD13 Position */
#define GPIO_PMD_PMD13_Msk          (0x3ul << GPIO_PMD_PMD13_Pos)               /*!< GPIO_T::PMD: PMD13 Mask */

#define GPIO_PMD_PMD12_Pos          24                                          /*!< GPIO_T::PMD: PMD12 Position */
#define GPIO_PMD_PMD12_Msk          (0x3ul << GPIO_PMD_PMD12_Pos)               /*!< GPIO_T::PMD: PMD12 Mask */

#define GPIO_PMD_PMD11_Pos          22                                          /*!< GPIO_T::PMD: PMD11 Position */
#define GPIO_PMD_PMD11_Msk          (0x3ul << GPIO_PMD_PMD11_Pos)               /*!< GPIO_T::PMD: PMD11 Mask */

#define GPIO_PMD_PMD10_Pos          20                                          /*!< GPIO_T::PMD: PMD10 Position */
#define GPIO_PMD_PMD10_Msk          (0x3ul << GPIO_PMD_PMD10_Pos)               /*!< GPIO_T::PMD: PMD10 Mask */

#define GPIO_PMD_PMD9_Pos           18                                          /*!< GPIO_T::PMD: PMD9 Position */
#define GPIO_PMD_PMD9_Msk           (0x3ul << GPIO_PMD_PMD9_Pos)                /*!< GPIO_T::PMD: PMD9 Mask */

#define GPIO_PMD_PMD8_Pos           16                                          /*!< GPIO_T::PMD: PMD8 Position */
#define GPIO_PMD_PMD8_Msk           (0x3ul << GPIO_PMD_PMD8_Pos)                /*!< GPIO_T::PMD: PMD8 Mask */

#define GPIO_PMD_PMD7_Pos           14                                          /*!< GPIO_T::PMD: PMD7 Position */
#define GPIO_PMD_PMD7_Msk           (0x3ul << GPIO_PMD_PMD7_Pos)                /*!< GPIO_T::PMD: PMD7 Mask */

#define GPIO_PMD_PMD6_Pos           12                                          /*!< GPIO_T::PMD: PMD6 Position */
#define GPIO_PMD_PMD6_Msk           (0x3ul << GPIO_PMD_PMD6_Pos)                /*!< GPIO_T::PMD: PMD6 Mask */

#define GPIO_PMD_PMD5_Pos           10                                          /*!< GPIO_T::PMD: PMD5 Position */
#define GPIO_PMD_PMD5_Msk           (0x3ul << GPIO_PMD_PMD5_Pos)                /*!< GPIO_T::PMD: PMD5 Mask */

#define GPIO_PMD_PMD4_Pos           8                                           /*!< GPIO_T::PMD: PMD4 Position */
#define GPIO_PMD_PMD4_Msk           (0x3ul << GPIO_PMD_PMD4_Pos)                /*!< GPIO_T::PMD: PMD4 Mask */

#define GPIO_PMD_PMD3_Pos           6                                           /*!< GPIO_T::PMD: PMD3 Position */
#define GPIO_PMD_PMD3_Msk           (0x3ul << GPIO_PMD_PMD3_Pos)                /*!< GPIO_T::PMD: PMD3 Mask */

#define GPIO_PMD_PMD2_Pos           4                                           /*!< GPIO_T::PMD: PMD2 Position */
#define GPIO_PMD_PMD2_Msk           (0x3ul << GPIO_PMD_PMD2_Pos)                /*!< GPIO_T::PMD: PMD2 Mask */

#define GPIO_PMD_PMD1_Pos           2                                           /*!< GPIO_T::PMD: PMD1 Position */
#define GPIO_PMD_PMD1_Msk           (0x3ul << GPIO_PMD_PMD1_Pos)                /*!< GPIO_T::PMD: PMD1 Mask */

#define GPIO_PMD_PMD0_Pos           0                                           /*!< GPIO_T::PMD: PMD0 Position */
#define GPIO_PMD_PMD0_Msk           (0x3ul << GPIO_PMD_PMD0_Pos)                /*!< GPIO_T::PMD: PMD0 Mask */

/* GPIO OFFD Bit Field Definitions */
#define GPIO_OFFD_OFFD_Pos          16                                          /*!< GPIO_T::OFFD: OFFD Position */
#define GPIO_OFFD_OFFD_Msk          (0xFFFFul << GPIO_OFFD_OFFD_Pos)            /*!< GPIO_T::OFFD: OFFD Mask */

/* GPIO DOUT Bit Field Definitions */
#define GPIO_DOUT_DOUT_Pos          0                                           /*!< GPIO_T::DOUT: DOUT Position */
#define GPIO_DOUT_DOUT_Msk          (0xFFFFul << GPIO_DOUT_DOUT_Pos)            /*!< GPIO_T::DOUT: DOUT Mask */

/* GPIO DMASK Bit Field Definitions */
#define GPIO_DMASK_DMASK_Pos        0                                           /*!< GPIO_T::DMASK: DMASK Position */
#define GPIO_DMASK_DMASK_Msk        (0xFFFFul << GPIO_DMASK_DMASK_Pos)          /*!< GPIO_T::DMASK: DMASK Mask */

/* GPIO PIN Bit Field Definitions */
#define GPIO_PIN_PIN_Pos            0                                           /*!< GPIO_T::PIN: PIN Position */
#define GPIO_PIN_PIN_Msk            (0xFFFFul << GPIO_PIN_PIN_Pos)              /*!< GPIO_T::PIN: PIN Mask */

/* GPIO DBEN Bit Field Definitions */
#define GPIO_DBEN_DBEN_Pos          0                                           /*!< GPIO_T::DBEN: DBEN Position */
#define GPIO_DBEN_DBEN_Msk          (0xFFFFul << GPIO_DBEN_DBEN_Pos)            /*!< GPIO_T::DBEN: DBEN Mask */

/* GPIO IMD Bit Field Definitions */
#define GPIO_IMD_IMD_Pos            0                                           /*!< GPIO_T::IMD: IMD Position */
#define GPIO_IMD_IMD_Msk            (0xFFFFul << GPIO_IMD_IMD_Pos)              /*!< GPIO_T::IMD: IMD Mask */

/* GPIO IEN Bit Field Definitions */
#define GPIO_IEN_IR_EN_Pos          16                                          /*!< GPIO_T::IEN: IR_EN Position */
#define GPIO_IEN_IR_EN_Msk          (0xFFFFul << GPIO_IEN_IR_EN_Pos)            /*!< GPIO_T::IEN: IR_EN Mask */

#define GPIO_IEN_IF_EN_Pos          0                                           /*!< GPIO_T::IEN: IF_EN Position */
#define GPIO_IEN_IF_EN_Msk          (0xFFFFul << GPIO_IEN_IF_EN_Pos)            /*!< GPIO_T::IEN: IF_EN Mask */

/* GPIO ISRC Bit Field Definitions */
#define GPIO_ISRC_ISRC_Pos          0                                           /*!< GPIO_T::ISRC: ISRC Position */
#define GPIO_ISRC_ISRC_Msk          (0xFFFFul << GPIO_ISRC_ISRC_Pos)            /*!< GPIO_T::ISRC: ISRC Mask */

/* GPIO DBNCECON Bit Field Definitions */
#define GPIO_DBNCECON_ICLK_ON_Pos   5                                           /*!< GPIO_T::DBNCECON: ICLK_ON  Position */
#define GPIO_DBNCECON_ICLK_ON_Msk   (1ul << GPIO_DBNCECON_ICLK_ON_Pos)          /*!< GPIO_T::DBNCECON: ICLK_ON  Mask */

#define GPIO_DBNCECON_DBCLKSRC_Pos  4                                           /*!< GPIO_T::DBNCECON: DBCLKSRC Position */
#define GPIO_DBNCECON_DBCLKSRC_Msk  (1ul << GPIO_DBNCECON_DBCLKSRC_Pos)         /*!< GPIO_T::DBNCECON: DBCLKSRC Mask */

#define GPIO_DBNCECON_DBCLKSEL_Pos  0                                           /*!< GPIO_T::DBNCECON: DBCLKSEL Position */
#define GPIO_DBNCECON_DBCLKSEL_Msk  (0xFul << GPIO_DBNCECON_DBCLKSEL_Pos)       /*!< GPIO_T::DBNCECON: DBCLKSEL Mask */
/*@}*/ /* end of group GPIO_CONST */
/*@}*/ /* end of group NUC131_GPIO */





/*---------------------- Inter-IC Bus Controller -------------------------*/
/**
    @addtogroup I2C Inter-IC Bus Controller (I2C)
    Memory Mapped Structure for I2C Controller
@{ */
typedef struct
{
    /**
     * I2CON
     * ===================================================================================================
     * Offset: 0x00  I2C Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2]     |AA        |Assert Acknowledge Control
     * |        |          |When AA = 0 prior to address or data received, a Not acknowledged (high level to I2Cn_SDA) will be returned during the acknowledge clock pulse on the I2Cn_SCL line.     
     * |        |          |When AA = 1 prior to address or data received, an acknowledged (low level to I2Cn_SDA) will be returned during the acknowledge clock pulse on the I2Cn_SCL line when 1.) A slave is acknowledging the address sent from master, 2.) The receiver devices are acknowledging the data sent by transmitter.
     * |[3]     |SI        |I2C Interrupt Flag
     * |        |          |When a new I2C state is present in the I2CSTATUS register, the SI flag is set by hardware, and if bit EI (I2CON [7]) is set, the I2C interrupt is requested.
     * |        |          |SI must be cleared by software.
     * |        |          |Clear SI by writing 1 to this bit.
     * |[4]     |STO       |I2C STOP Control
     * |        |          |In Master mode, setting STO to transmit a STOP condition to bus then I2C hardware will check the bus condition if a STOP condition is detected this bit will be cleared by hardware automatically.
     * |        |          |In a slave mode, setting STO resets I2C hardware to the defined "not addressed" slave mode.
     * |        |          |This means it is NO LONGER in the slave receiver mode to receive data from the master transmit device.
     * |[5]     |STA       |I2C START Control
     * |        |          |Setting STA to logic 1 to enter Master mode, the I2C hardware sends a START or repeat START condition to bus when the bus is free.
     * |[6]     |ENS1      |I2C Controller Enable
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |        |          |Set to enable I2C serial function controller.
     * |        |          |When ENS1=1 the I2C serial function enables.
     * |        |          |The multi-function pin function of I2Cn_SDA and I2Cn_SCL must set to I2C function first.
     * |[7]     |EI        |Enable Interrupt
     * |        |          |0 = I2C interrupt Disabled.
     * |        |          |1 = I2C interrupt Enabled.
     */
    __IO uint32_t I2CON;

    /**
     * I2CADDR0
     * ===================================================================================================
     * Offset: 0x04  I2C Slave Address Register0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GC        |General Call Function
     * |        |          |0 = General Call Function Disabled.
     * |        |          |1 = General Call Function Enabled.
     * |[7:1]   |I2CADDR   |I2C Address Register
     * |        |          |The content of this register is irrelevant when I2C is in Master mode.
     * |        |          |In the slave mode, the seven most significant bits must be loaded with the chip's own address.
     * |        |          |The I2C hardware will react if either of the address is matched.
     */
    __IO uint32_t I2CADDR0;

    /**
     * I2CDAT
     * ===================================================================================================
     * Offset: 0x08  I2C Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |I2CDAT    |I2C Data Register
     * |        |          |Bit [7:0] is located with the 8-bit transferred data of I2C serial port.
     */
    __IO uint32_t I2CDAT;

    /**
     * I2CSTATUS
     * ===================================================================================================
     * Offset: 0x0C  I2C Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |I2CSTATUS |I2C Status Register
     * |        |          |The status register of I2C:
     * |        |          |There are 26 possible status codes.
     * |        |          |When I2CSTATUS contains F8H, no serial interrupt is requested.
     * |        |          |All other I2CSTATUS values correspond to defined I2C states.
     * |        |          |When each of these states is entered, a status interrupt is requested (SI = 1).
     * |        |          |A valid status code is present in I2CSTATUS one cycle after SI is set by hardware and is still present one cycle after SI has been reset by software.
     * |        |          |In addition, states 00H stands for a Bus Error.
     * |        |          |A Bus Error occurs when a START or STOP condition is present at an illegal position in the formation frame.
     * |        |          |Example of illegal position are during the serial transfer of an address byte, a data byte or an acknowledge bit.
     */
    __I  uint32_t I2CSTATUS;

    /**
     * I2CLK
     * ===================================================================================================
     * Offset: 0x10  I2C Clock Divided Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |I2CLK     |I2C Clock Divided Register
     * |        |          |The I2C clock rate bits: Data Baud Rate of I2C = (system clock) / (4x (I2CLK+1)).
     * |        |          |Note: The minimum value of I2CLK is 4.
     */
    __IO uint32_t I2CLK;

    /**
     * I2CTOC
     * ===================================================================================================
     * Offset: 0x14  I2C Time-out Counter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |TIF       |Time-out Flag
     * |        |          |This bit is set by H/W when I2C time-out happened and it can interrupt CPU if I2C interrupt enable bit (EI) is set to 1.
     * |        |          |Note: Write 1 to clear this bit.
     * |[1]     |DIV4      |Time-out Counter Input Clock Divided by 4
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |        |          |When Enabled, The time-out period is extend 4 times.
     * |[2]     |ENTI      |Time-out Counter Enable/Disable
     * |        |          |0 = Disabled.
     * |        |          |1 = Enabled.
     * |        |          |When Enabled, the 14-bit time-out counter will start counting when SI is clear.
     * |        |          |Setting flag SI to high will reset counter and re-start up counting after SI is cleared.
     */
    __IO uint32_t I2CTOC;

    /**
     * I2CADDR1
     * ===================================================================================================
     * Offset: 0x18  I2C Slave Address Register1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GC        |General Call Function
     * |        |          |0 = General Call Function Disabled.
     * |        |          |1 = General Call Function Enabled.
     * |[7:1]   |I2CADDR   |I2C Address Register
     * |        |          |The content of this register is irrelevant when I2C is in Master mode.
     * |        |          |In the slave mode, the seven most significant bits must be loaded with the chip's own address.
     * |        |          |The I2C hardware will react if either of the address is matched.
     */
    __IO uint32_t I2CADDR1;

    /**
     * I2CADDR2
     * ===================================================================================================
     * Offset: 0x1C  I2C Slave Address Register2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GC        |General Call Function
     * |        |          |0 = General Call Function Disabled.
     * |        |          |1 = General Call Function Enabled.
     * |[7:1]   |I2CADDR   |I2C Address Register
     * |        |          |The content of this register is irrelevant when I2C is in Master mode.
     * |        |          |In the slave mode, the seven most significant bits must be loaded with the chip's own address.
     * |        |          |The I2C hardware will react if either of the address is matched.
     */
    __IO uint32_t I2CADDR2;

    /**
     * I2CADDR3
     * ===================================================================================================
     * Offset: 0x20  I2C Slave Address Register3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GC        |General Call Function
     * |        |          |0 = General Call Function Disabled.
     * |        |          |1 = General Call Function Enabled.
     * |[7:1]   |I2CADDR   |I2C Address Register
     * |        |          |The content of this register is irrelevant when I2C is in Master mode.
     * |        |          |In the slave mode, the seven most significant bits must be loaded with the chip's own address.
     * |        |          |The I2C hardware will react if either of the address is matched.
     */
    __IO uint32_t I2CADDR3;

    /**
     * I2CADM0
     * ===================================================================================================
     * Offset: 0x24  I2C Slave Address Mask Register0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:1]   |I2CADM    |I2C Address Mask Register
     * |        |          |0 = Mask Disabled (the received corresponding register bit should be exact the same as address register.).
     * |        |          |1 = Mask Enabled (the received corresponding address bit is don't care.).
     * |        |          |I2C bus controllers support multiple address recognition with four address mask register.
     * |        |          |When the bit in the address mask register is set to one, it means the received corresponding address bit is don't-care.
     * |        |          |If the bit is set to zero, that means the received corresponding register bit should be exact the same as address register.
     */
    __IO uint32_t I2CADM0;

    /**
     * I2CADM1
     * ===================================================================================================
     * Offset: 0x28  I2C Slave Address Mask Register1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:1]   |I2CADM    |I2C Address Mask Register
     * |        |          |0 = Mask Disabled (the received corresponding register bit should be exact the same as address register.).
     * |        |          |1 = Mask Enabled (the received corresponding address bit is don't care.).
     * |        |          |I2C bus controllers support multiple address recognition with four address mask register.
     * |        |          |When the bit in the address mask register is set to one, it means the received corresponding address bit is don't-care.
     * |        |          |If the bit is set to zero, that means the received corresponding register bit should be exact the same as address register.
     */
    __IO uint32_t I2CADM1;

    /**
     * I2CADM2
     * ===================================================================================================
     * Offset: 0x2C  I2C Slave Address Mask Register2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:1]   |I2CADM    |I2C Address Mask Register
     * |        |          |0 = Mask Disabled (the received corresponding register bit should be exact the same as address register.).
     * |        |          |1 = Mask Enabled (the received corresponding address bit is don't care.).
     * |        |          |I2C bus controllers support multiple address recognition with four address mask register.
     * |        |          |When the bit in the address mask register is set to one, it means the received corresponding address bit is don't-care.
     * |        |          |If the bit is set to zero, that means the received corresponding register bit should be exact the same as address register.
     */
    __IO uint32_t I2CADM2;

    /**
     * I2CADM3
     * ===================================================================================================
     * Offset: 0x30  I2C Slave Address Mask Register3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:1]   |I2CADM    |I2C Address Mask Register
     * |        |          |0 = Mask Disabled (the received corresponding register bit should be exact the same as address register.).
     * |        |          |1 = Mask Enabled (the received corresponding address bit is don't care.).
     * |        |          |I2C bus controllers support multiple address recognition with four address mask register.
     * |        |          |When the bit in the address mask register is set to one, it means the received corresponding address bit is don't-care.
     * |        |          |If the bit is set to zero, that means the received corresponding register bit should be exact the same as address register.
     */
    __IO uint32_t I2CADM3;

    /**
     * RESERVED0
     * ===================================================================================================
     *
     * ---------------------------------------------------------------------------------------------------
     */
    uint32_t RESERVED0[2];

    /**
     * I2CWKUPCON
     * ===================================================================================================
     * Offset: 0x3C  I2C Wake-up Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WKUPEN    |I2C Wake-up Enable
     * |        |          |0 = I2C wake-up function Disabled.
     * |        |          |1 = I2C wake-up function Enabled.
     */
    __IO uint32_t I2CWKUPCON;

    /**
     * I2CWKUPSTS
     * ===================================================================================================
     * Offset: 0x40  I2C Wake-up Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WKUPIF    |I2C Wake-up Flag
     * |        |          |0 = Chip is not woken-up from Power-down mode by I2C.
     * |        |          |1 = Chip is woken-up from Power-down mode by I2C.
     * |        |          |Note: Software can write 1 to clear this bit.
     */
    __IO uint32_t I2CWKUPSTS;
} I2C_T;


/**
    @addtogroup I2C_CONST I2C Bit Field Definition
    Constant Definitions for I2C Controller
@{ */


/* I2C I2CON Bit Field Definitions */
#define I2C_I2CON_EI_Pos                        7                                       /*!< I2C_T::I2CON: EI Position */
#define I2C_I2CON_EI_Msk                        (1ul << I2C_I2CON_EI_Pos)               /*!< I2C_T::I2CON: EI Mask */

#define I2C_I2CON_ENS1_Pos                      6                                       /*!< I2C_T::I2CON: ENS1 Position */
#define I2C_I2CON_ENS1_Msk                      (1ul << I2C_I2CON_ENS1_Pos)             /*!< I2C_T::I2CON: ENS1 Mask */

#define I2C_I2CON_STA_Pos                       5                                       /*!< I2C_T::I2CON: STA Position */
#define I2C_I2CON_STA_Msk                       (1ul << I2C_I2CON_STA_Pos)              /*!< I2C_T::I2CON: STA Mask */

#define I2C_I2CON_STO_Pos                       4                                       /*!< I2C_T::I2CON: STO Position */
#define I2C_I2CON_STO_Msk                       (1ul << I2C_I2CON_STO_Pos)              /*!< I2C_T::I2CON: STO Mask */

#define I2C_I2CON_SI_Pos                        3                                       /*!< I2C_T::I2CON: SI Position */
#define I2C_I2CON_SI_Msk                        (1ul << I2C_I2CON_SI_Pos)               /*!< I2C_T::I2CON: SI Mask */

#define I2C_I2CON_AA_Pos                        2                                       /*!< I2C_T::I2CON: AA Position */
#define I2C_I2CON_AA_Msk                        (1ul << I2C_I2CON_AA_Pos)               /*!< I2C_T::I2CON: AA Mask */

/* I2C I2CADDR Bit Field Definitions */
#define I2C_I2CADDR_I2CADDR_Pos                 1                                       /*!< I2C_T::I2CADDR0: I2CADDR Position */
#define I2C_I2CADDR_I2CADDR_Msk                 (0x7Ful << I2C_I2CADDR_I2CADDR_Pos)     /*!< I2C_T::I2CADDR0: I2CADDR Mask */

#define I2C_I2CADDR_GC_Pos                      0                                       /*!< I2C_T::I2CADDR0: GC Position */
#define I2C_I2CADDR_GC_Msk                      (1ul << I2C_I2CADDR_GC_Pos)             /*!< I2C_T::I2CADDR0: GC Mask */

/* I2C I2CDAT Bit Field Definitions */
#define I2C_I2CDAT_I2CDAT_Pos                   0                                       /*!< I2C_T::I2CDAT: I2CDAT Position */
#define I2C_I2CDAT_I2CDAT_Msk                   (0xFFul << I2C_I2CDAT_I2CDAT_Pos)       /*!< I2C_T::I2CDAT: I2CDAT Mask */

/* I2C I2CSTATUS Bit Field Definitions */
#define I2C_I2CSTATUS_I2CSTATUS_Pos             0                                       /*!< I2C_T::I2CSTATUS: I2CSTATUS Position */
#define I2C_I2CSTATUS_I2CSTATUS_Msk             (0xFFul << I2C_I2CSTATUS_I2CSTATUS_Pos) /*!< I2C_T::I2CSTATUS: I2CSTATUS Mask */

/* I2C I2CLK Bit Field Definitions */
#define I2C_I2CLK_I2CLK_Pos                     0                                       /*!< I2C_T::I2CLK: I2CLK Position */
#define I2C_I2CLK_I2CLK_Msk                     (0xFFul << I2C_I2CLK_I2CLK_Pos)         /*!< I2C_T::I2CLK: I2CLK Mask */

/* I2C I2CTOC Bit Field Definitions */
#define I2C_I2CTOC_ENTI_Pos                     2                                       /*!< I2C_T::I2CTOC: ENTI Position */
#define I2C_I2CTOC_ENTI_Msk                     (1ul << I2C_I2CTOC_ENTI_Pos)            /*!< I2C_T::I2CTOC: ENTI Mask */

#define I2C_I2CTOC_DIV4_Pos                     1                                       /*!< I2C_T::I2CTOC: DIV4 Position */
#define I2C_I2CTOC_DIV4_Msk                     (1ul << I2C_I2CTOC_DIV4_Pos)            /*!< I2C_T::I2CTOC: DIV4 Mask */

#define I2C_I2CTOC_TIF_Pos                      0                                       /*!< I2C_T::I2CTOC: TIF Position */
#define I2C_I2CTOC_TIF_Msk                      (1ul << I2C_I2CTOC_TIF_Pos)             /*!< I2C_T::I2CTOC: TIF Mask */

/* I2C I2CADM Bit Field Definitions */
#define I2C_I2CADM_I2CADM_Pos                   1                                       /*!< I2C_T::I2CADM0: I2CADM Position */
#define I2C_I2CADM_I2CADM_Msk                   (0x7Ful << I2C_I2CADM_I2CADM_Pos)       /*!< I2C_T::I2CADM0: I2CADM Mask */

/* I2C I2CWKUPCON Bit Field Definitions */
#define I2C_I2CWKUPCON_WKUPEN_Pos               0                                       /*!< I2C_T::I2CWKUPCON: WKUPEN Position */
#define I2C_I2CWKUPCON_WKUPEN_Msk               (1ul << I2C_I2CWKUPCON_WKUPEN_Pos)      /*!< I2C_T::I2CWKUPCON: WKUPEN Mask */

/* I2C I2CWKUPSTS Bit Field Definitions */
#define I2C_I2CWKUPSTS_WKUPIF_Pos               0                                       /*!< I2C_T::I2CWKUPSTS: WKUPIF Position */
#define I2C_I2CWKUPSTS_WKUPIF_Msk               (1ul << I2C_I2CWKUPSTS_WKUPIF_Pos)      /*!< I2C_T::I2CWKUPSTS: WKUPIF Mask */
/*@}*/ /* end of group I2C_CONST */
/*@}*/ /* end of group I2C */


/*---------------------- Pulse Width Modulation Controller -------------------------*/
/**
    @addtogroup PWM Pulse Width Modulation Controller (PWM)
    Memory Mapped Structure for PWM Controller
@{ */
typedef struct
{
    /**
     * CTL0
     * ===================================================================================================
     * Offset: 0x00  PWM Control Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CTRLDn    |Center Re-Load
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |In up-down counter type, PERIOD will load to PBUF at the end point of each period.
     * |        |          |CMPDAT will load to CMPBUF at the center point of a period.
     * |[21:16] |IMMLDENn  |Immediately Load Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = PERIOD will load to PBUF at the end point of each period.
     * |        |          |CMPDAT will load to CMPBUF at the end point or center point of each period by setting CTRLD bit.
     * |        |          |1 = PERIOD/CMPDAT will load to PBUF and CMPBUF immediately when software update PERIOD/CMPDAT.
     * |        |          |Note: If IMMLDENn is enabled, WINLDENn and CTRLDn will be invalid.
     * |[30]    |DBGHALT   |ICE Debug Mode Counter Halt (Write Protect)
     * |        |          |If counter halt is enabled, PWM all counters will keep current value until exit ICE debug mode.
     * |        |          |0 = ICE debug mode counter halt disable.
     * |        |          |1 = ICE debug mode counter halt enable.
     * |        |          |Note: This register is write protected. Refer toSYS_REGLCTL register.
     * |[31]    |DBGTRIOFF |ICE Debug Mode Acknowledge Disable (Write Protect)
     * |        |          |0 = ICE debug mode acknowledgment effects PWM output.
     * |        |          |PWM pin will be forced as tri-state while ICE debug mode acknowledged.
     * |        |          |1 = ICE debug mode acknowledgment disabled.
     * |        |          |PWM pin will keep output no matter ICE debug mode acknowledged or not.
     * |        |          |Note: This register is write protected. Refer toSYS_REGLCTL register.
     */
    __IO uint32_t CTL0;

    /**
     * CTL1
     * ===================================================================================================
     * Offset: 0x04  PWM Control Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |CNTTYPE0  |PWM Counter Behavior Type
     * |        |          |Each bit n controls corresponding PWM channel n.
     * |        |          |00 = Up counter type (supports in capture mode).
     * |        |          |01 = Down count type (supports in capture mode).
     * |        |          |10 = Up-down counter type.
     * |        |          |11 = Reserved.
     * |[5:4]   |CNTTYPE2  |PWM Counter Behavior Type
     * |        |          |Each bit n controls corresponding PWM channel n.
     * |        |          |00 = Up counter type (supports in capture mode).
     * |        |          |01 = Down count type (supports in capture mode).
     * |        |          |10 = Up-down counter type.
     * |        |          |11 = Reserved.
     * |[9:8]   |CNTTYPE4  |PWM Counter Behavior Type
     * |        |          |Each bit n controls corresponding PWM channel n.
     * |        |          |00 = Up counter type (supports in capture mode).
     * |        |          |01 = Down count type (supports in capture mode).
     * |        |          |10 = Up-down counter type.
     * |        |          |11 = Reserved.
     * |[26:24] |OUTMODEn  |PWM Output Mode
     * |        |          |Each bit n controls the output mode of corresponding PWM channel n.
     * |        |          |0 = PWM independent mode.
     * |        |          |1 = PWM complementary mode.
     * |        |          |Note: When operating in group function, these bits must all set to the same mode.
     */
    __IO uint32_t CTL1;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED0
     * ===================================================================================================
     * Offset: 0x0008 ~ 0x000C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED0[2];
    /**
      @endcond
     */


    /**
     * CLKSRC
     * ===================================================================================================
     * Offset: 0x10  PWM Clock Source Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2:0]   |ECLKSRC0  |PWM_CH01 External Clock Source Select
     * |        |          |000 = PWMx_CLK, x denotes 0 or 1.
     * |        |          |001 = TIMER0 overflow.
     * |        |          |010 = TIMER1 overflow.
     * |        |          |011 = TIMER2 overflow.
     * |        |          |100 = TIMER3 overflow.
     * |        |          |Others = Reserved.
     * |[10:8]  |ECLKSRC2  |PWM_CH23 External Clock Source Select
     * |        |          |000 = PWMx_CLK, x denotes 0 or 1.
     * |        |          |001 = TIMER0 overflow.
     * |        |          |010 = TIMER1 overflow.
     * |        |          |011 = TIMER2 overflow.
     * |        |          |100 = TIMER3 overflow.
     * |        |          |Others = Reserved.
     * |[18:16] |ECLKSRC4  |PWM_CH45 External Clock Source Select
     * |        |          |000 = PWMx_CLK, x denotes 0 or 1.
     * |        |          |001 = TIMER0 overflow.
     * |        |          |010 = TIMER1 overflow.
     * |        |          |011 = TIMER2 overflow.
     * |        |          |100 = TIMER3 overflow.
     * |        |          |Others = Reserved.
     */
    __IO uint32_t CLKSRC;

    /**
     * CLKPSC0_1
     * ===================================================================================================
     * Offset: 0x14  PWM Clock Pre-scale Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |CLKPSC    |PWM Counter Clock Pre-Scale
     * |        |          |The clock of PWM counter is decided by clock prescaler.
     * |        |          |Each PWM pair share one PWM counter clock prescaler.
     * |        |          |The clock of PWM counter is divided by (CLKPSC+ 1).
     */
    __IO uint32_t CLKPSC0_1;

    /**
     * CLKPSC2_3
     * ===================================================================================================
     * Offset: 0x18  PWM Clock Pre-scale Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |CLKPSC    |PWM Counter Clock Pre-Scale
     * |        |          |The clock of PWM counter is decided by clock prescaler.
     * |        |          |Each PWM pair share one PWM counter clock prescaler.
     * |        |          |The clock of PWM counter is divided by (CLKPSC+ 1).
     */
    __IO uint32_t CLKPSC2_3;

    /**
     * CLKPSC4_5
     * ===================================================================================================
     * Offset: 0x1C  PWM Clock Pre-scale Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |CLKPSC    |PWM Counter Clock Pre-Scale
     * |        |          |The clock of PWM counter is decided by clock prescaler.
     * |        |          |Each PWM pair share one PWM counter clock prescaler.
     * |        |          |The clock of PWM counter is divided by (CLKPSC+ 1).
     */
    __IO uint32_t CLKPSC4_5;

    /**
     * CNTEN
     * ===================================================================================================
     * Offset: 0x20  PWM Counter Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTEN0    |PWM Counter Enable 0
     * |        |          |0 = PWM Counter and clock prescaler Stop Running.
     * |        |          |1 = PWM Counter and clock prescaler Start Running.
     * |[2]     |CNTEN2    |PWM Counter Enable 2
     * |        |          |0 = PWM Counter and clock prescaler Stop Running.
     * |        |          |1 = PWM Counter and clock prescaler Start Running.
     * |[4]     |CNTEN0    |PWM Counter Enable 4
     * |        |          |0 = PWM Counter and clock prescaler Stop Running.
     * |        |          |1 = PWM Counter and clock prescaler Start Running.
     */
    __IO uint32_t CNTEN;

    /**
     * CNTCLR
     * ===================================================================================================
     * Offset: 0x24  PWM Clear Counter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTCLR0   |Clear PWM Counter Control Bit 0
     * |        |          |It is automatically cleared by hardware.
     * |        |          |0 = No effect.
     * |        |          |1 = Clear 16-bit PWM counter to 0000H.
     * |[2]     |CNTCLR2   |Clear PWM Counter Control Bit 2
     * |        |          |It is automatically cleared by hardware.
     * |        |          |0 = No effect.
     * |        |          |1 = Clear 16-bit PWM counter to 0000H.
     * |[4]     |CNTCLR4   |Clear PWM Counter Control Bit 4
     * |        |          |It is automatically cleared by hardware.
     * |        |          |0 = No effect.
     * |        |          |1 = Clear 16-bit PWM counter to 0000H.
     */
    __IO uint32_t CNTCLR;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED1
     * ===================================================================================================
     * Offset: 0x0028 ~ 0x002C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED1[2];
    /**
     * @endcond
     */

    /**
     * PERIOD0, 2, 4
     * ===================================================================================================
     * Offset: 0x30, 0x38, 0x40  PWM Period Register 0, 2, 4 (offset: 0x34, 0x3C, 0x44 are reserved)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |PERIOD    |PWM Period Register
     * |        |          |Up-Count mode: In this mode, PWM counter counts from 0 to PERIOD, and restarts from 0.
     * |        |          |Down-Count mode: In this mode, PWM counter counts from PERIOD to 0, and restarts from PERIOD.
     * |        |          |PWM period time = (PERIOD+1) * PWM_CLK period.
     * |        |          |Up-Down-Count mode: In this mode, PWM counter counts from 0 to PERIOD, then decrements to 0 and repeats again.
     * |        |          |PWM period time = 2 * PERIOD * PWM_CLK period.
     */
    __IO uint32_t PERIOD[6];


    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED2
     * ===================================================================================================
     * Offset: 0x0048 ~ 0x004C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED2[2];
    /**
     * @endcond
     */

    /**
     * CMPDAT0~5
     * ===================================================================================================
     * Offset: 0x50~0x64  PWM Comparator Register 0~5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |CMP       |PWM Comparator Register
     * |        |          |CMP use to compare with CNTR to generate PWM waveform, interrupt and trigger ADC.
     * |        |          |In independent mode, CMPDAT0~5 denote as 6 independent PWM_CH0~5 compared point.
     * |        |          |In complementary mode, CMPDAT0, 2, 4 denote as first compared point, and CMPDAT1, 3, 5
     * |        |          |denote as second compared point for the corresponding 3 complementary pairs PWM_CH0 and PWM_CH1, PWM_CH2 and PWM_CH3, PWM_CH4 and PWM_CH5.
     */
    __IO uint32_t CMPDAT[6];

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED3
     * ===================================================================================================
     * Offset: 0x0068 ~ 0x006C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED3[2];
    /**
     * @endcond
     */

    /**
     * DTCTL0_1
     * ===================================================================================================
     * Offset: 0x70  PWM Dead-Time Control Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |DTCNT     |Dead-Time Counter (Write Protect)
     * |        |          |The dead-time can be calculated from the following formula:
     * |        |          |Dead-time = (DTCNT[11:0]+1) * PWM_CLK period.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[16]    |DTEN      |Enable Dead-Time Insertion For PWM Pair (PWM_CH0, PWM_CH1) (PWM_CH2, PWM_CH3)
     * |        |          |(PWM_CH4, PWM_CH5) (Write Protect)
     * |        |          |Dead-time insertion is only active when this pair of complementary PWM is enabled. If
     * |        |          |dead-time insertion is inactive, the outputs of pin pair are complementary
     * |        |          |without any delay.
     * |        |          |0 = Dead-time insertion Disabled on the pin pair.
     * |        |          |1 = Dead-time insertion Enabled on the pin pair.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[24]    |DTCKSEL   |Dead-Time Clock Select (Write Protect)
     * |        |          |0 = Dead-time clock source from PWM_CLK.
     * |        |          |1 = Dead-time clock source from prescaler output.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t DTCTL0_1;

    /**
     * DTCTL2_3
     * ===================================================================================================
     * Offset: 0x74  PWM Dead-Time Control Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |DTCNT     |Dead-Time Counter (Write Protect)
     * |        |          |The dead-time can be calculated from the following formula:
     * |        |          |Dead-time = (DTCNT[11:0]+1) * PWM_CLK period.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[16]    |DTEN      |Enable Dead-Time Insertion For PWM Pair (PWM_CH0, PWM_CH1) (PWM_CH2, PWM_CH3)
     * |        |          |(PWM_CH4, PWM_CH5) (Write Protect)
     * |        |          |Dead-time insertion is only active when this pair of complementary PWM is enabled. If
     * |        |          |dead-time insertion is inactive, the outputs of pin pair are complementary
     * |        |          |without any delay.
     * |        |          |0 = Dead-time insertion Disabled on the pin pair.
     * |        |          |1 = Dead-time insertion Enabled on the pin pair.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[24]    |DTCKSEL   |Dead-Time Clock Select (Write Protect)
     * |        |          |0 = Dead-time clock source from PWM_CLK.
     * |        |          |1 = Dead-time clock source from prescaler output.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t DTCTL2_3;

    /**
     * DTCTL4_5
     * ===================================================================================================
     * Offset: 0x78  PWM Dead-Time Control Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |DTCNT     |Dead-Time Counter (Write Protect)
     * |        |          |The dead-time can be calculated from the following formula:
     * |        |          |Dead-time = (DTCNT[11:0]+1) * PWM_CLK period.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[16]    |DTEN      |Enable Dead-Time Insertion For PWM Pair (PWM_CH0, PWM_CH1) (PWM_CH2, PWM_CH3)
     * |        |          |(PWM_CH4, PWM_CH5) (Write Protect)
     * |        |          |Dead-time insertion is only active when this pair of complementary PWM is enabled. If
     * |        |          |dead- time insertion is inactive, the outputs of pin pair are complementary
     * |        |          |without any delay.
     * |        |          |0 = Dead-time insertion Disabled on the pin pair.
     * |        |          |1 = Dead-time insertion Enabled on the pin pair.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[24]    |DTCKSEL   |Dead-Time Clock Select (Write Protect)
     * |        |          |0 = Dead-time clock source from PWM_CLK.
     * |        |          |1 = Dead-time clock source from prescaler output.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t DTCTL4_5;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED4
     * ===================================================================================================
     * Offset: 0x007C ~ 0x008C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED4[5];
    /**
     * @endcond
     */

    /**
     * CNT0, 2, 4
     * ===================================================================================================
     * Offset: 0x90, 0x98, 0xA0  PWM Counter Register 0, 2, 4 (offset: 0x94, 0x9C, 0xA4 are reserved)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |CNT       |PWM Data Register (Read Only)
     * |        |          |User can monitor CNTR to know the current value in 16-bit period counter.
     * |[16]    |DIRF      |PWM Direction Indicator Flag (Read Only)
     * |        |          |0 = Counter is Down count.
     * |        |          |1 = Counter is UP count.
     */
    __IO uint32_t CNT[6];

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED5
     * ===================================================================================================
     * Offset: 0x00A8 ~ 0x00AC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED5[2];
    /**
     * @endcond
     */

    /**
     * WGCTL0
     * ===================================================================================================
     * Offset: 0xB0  PWM Generation Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |ZPCTLn    |PWM Zero Point Control
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = PWM zero point output Low.
     * |        |          |10 = PWM zero point output High.
     * |        |          |11 = PWM zero point output Toggle.
     * |        |          |PWM can control output level when PWM counter count to zero.
     * |[27:16] |PRDPCTLn  |PWM Period (Center) Point Control
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = PWM period (center) point output Low.
     * |        |          |10 = PWM period (center) point output High.
     * |        |          |11 = PWM period (center) point output Toggle.
     * |        |          |PWM can control output level when PWM counter count to (PERIODn+1).
     * |        |          |Note: This bit is center point control when PWM counter operating in up-down counter type.
     */
    __IO uint32_t WGCTL0;

    /**
     * WGCTL1
     * ===================================================================================================
     * Offset: 0xB4  PWM Generation Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[11:0]  |CMPUCTLn  |PWM Compare Up Point Control
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = PWM compare up point output Low.
     * |        |          |10 = PWM compare up point output High.
     * |        |          |11 = PWM compare up point output Toggle.
     * |        |          |PWM can control output level when PWM counter up count to CMPDAT.
     * |        |          |Note: In complementary mode, CMPUCTL1, 3, 5 use as another CMPUCTL for channel 0, 2, 4.
     * |[27:16] |CMPDCTLn  |PWM Compare Down Point Control
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |00 = Do nothing.
     * |        |          |01 = PWM compare down point output Low.
     * |        |          |10 = PWM compare down point output High.
     * |        |          |11 = PWM compare down point output Toggle.
     * |        |          |PWM can control output level when PWM counter down count to CMPDAT.
     * |        |          |Note: In complementary mode, CMPDCTL1, 3, 5 use as another CMPDCTL for channel 0, 2, 4.
     */
    __IO uint32_t WGCTL1;

    /**
     * MSKEN
     * ===================================================================================================
     * Offset: 0xB8  PWM Mask Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |MSKENn    |PWM Mask Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |The PWM output signal will be masked when this bit is enabled.
     * |        |          |The corresponding PWM channel n will output MSKDATn (PWM_MSK[5:0]) data.
     * |        |          |0 = PWM output signal is non-masked.
     * |        |          |1 = PWM output signal is masked and output MSKDATn data.
     */
    __IO uint32_t MSKEN;

    /**
     * MSK
     * ===================================================================================================
     * Offset: 0xBC  PWM Mask Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |MSKDATn   |PWM Mask Data Bit
     * |        |          |This data bit control the state of PWMn output pin, if corresponding mask function is enabled.
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Output logic low to PWMn.
     * |        |          |1 = Output logic high to PWMn.
     */
    __IO uint32_t MSK;

    /**
     * BNF
     * ===================================================================================================
     * Offset: 0xC0  PWM Brake Noise Filter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |BRK0FEN   |PWM Brake 0 Noise Filter Enable
     * |        |          |0 = Noise filter of PWM Brake 0 Disabled.
     * |        |          |1 = Noise filter of PWM Brake 0 Enabled.
     * |[3:1]   |BRK0FCS   |Brake 0 Edge Detector Filter Clock Selection
     * |        |          |000 = Filter clock = HCLK.
     * |        |          |001 = Filter clock = HCLK/2.
     * |        |          |010 = Filter clock = HCLK/4.
     * |        |          |011 = Filter clock = HCLK/8.
     * |        |          |100 = Filter clock = HCLK/16.
     * |        |          |101 = Filter clock = HCLK/32.
     * |        |          |110 = Filter clock = HCLK/64.
     * |        |          |111 = Filter clock = HCLK/128.
     * |[6:4]   |BRK0FCNT  |Brake 0 Edge Detector Filter Count
     * |        |          |The register bits control the Brake0 filter counter to count from 0 to BRK1FCNT.
     * |[7]     |BRK0PINV  |Brake 0 Pin Inverse
     * |        |          |0 = The state of pin PWMx_BRAKE0 is passed to the negative edge detector.
     * |        |          |1 = The inverted state of pin PWMx_BRAKE10 is passed to the negative edge detector.
     * |[8]     |BRK1FEN   |PWM Brake 1 Noise Filter Enable
     * |        |          |0 = Noise filter of PWM Brake 1 Disabled.
     * |        |          |1 = Noise filter of PWM Brake 1 Enabled.
     * |[11:9]  |BRK1FCS   |Brake 1 Edge Detector Filter Clock Selection
     * |        |          |000 = Filter clock = HCLK.
     * |        |          |001 = Filter clock = HCLK/2.
     * |        |          |010 = Filter clock = HCLK/4.
     * |        |          |011 = Filter clock = HCLK/8.
     * |        |          |100 = Filter clock = HCLK/16.
     * |        |          |101 = Filter clock = HCLK/32.
     * |        |          |110 = Filter clock = HCLK/64.
     * |        |          |111 = Filter clock = HCLK/128.
     * |[14:12] |BRK1FCNT  |Brake 1 Edge Detector Filter Count
     * |        |          |The register bits control the Brake1 filter counter to count from 0 to BRK1FCNT.
     * |[15]    |BRK1PINV  |Brake 1 Pin Inverse
     * |        |          |0 = The state of pin PWMx_BRAKE1 is passed to the negative edge detector.
     * |        |          |1 = The inverted state of pin PWMx_BRAKE1 is passed to the negative edge detector.
     * |[16]    |BK0SRC    |Brake 0 Pin Source Select
     * |        |          |For PWM0 setting:
     * |        |          |0 = Brake 0 pin source come from PWM0_BRAKE0.
     * |        |          |1 = Brake 0 pin source come from PWM1_BRAKE0.
     * |        |          |For PWM1 setting:
     * |        |          |0 = Brake 0 pin source come from PWM1_BRAKE0.
     * |        |          |1 = Brake 0 pin source come from PWM0_BRAKE0.
     * |[24]    |BK1SRC    |Brake 1 Pin Source Select
     * |        |          |For PWM0 setting:
     * |        |          |0 = Brake 1 pin source come from PWM0_BRAKE1.
     * |        |          |1 = Brake 1 pin source come from PWM1_BRAKE1.
     * |        |          |For PWM1 setting:
     * |        |          |0 = Brake 1 pin source come from PWM1_BRAKE1.
     * |        |          |1 = Brake 1 pin source come from PWM0_BRAKE1.
     */
    __IO uint32_t BNF;

    /**
     * FAILBRK
     * ===================================================================================================
     * Offset: 0xC4  PWM System Fail Brake Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CSSBRKEN  |Clock Security System Detection Trigger PWM Brake Function 0 Enable
     * |        |          |0 = Brake Function triggered by CSS detection Disabled.
     * |        |          |1 = Brake Function triggered by CSS detection Enabled.
     * |[1]     |BODBRKEN  |Brown-Out Detection Trigger PWM Brake Function 0 Enable
     * |        |          |0 = Brake Function triggered by BOD Disabled.
     * |        |          |1 = Brake Function triggered by BOD Enabled.
     * |[3]     |CORBRKEN  |Core Lockup Detection Trigger PWM Brake Function 0 Enable
     * |        |          |0 = Brake Function triggered by Core lockup detection Disabled.
     * |        |          |1 = Brake Function triggered by Core lockup detection Enabled.
     */
    __IO uint32_t FAILBRK;

    /**
     * BRKCTL0_1
     * ===================================================================================================
     * Offset: 0xC8  PWM Brake Edge Detect Control Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |BRKP0EEN  |Enable PWMx_BRAKE0 Pin As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = BKP0 pin as edge-detect brake source Disabled.
     * |        |          |1 = BKP0 pin as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[5]     |BRKP1EEN  |Enable PWMx_BRAKE1 Pin As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = BKP1 pin as edge-detect brake source Disabled.
     * |        |          |1 = BKP1 pin as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[7]     |SYSEEN    |Enable System Fail As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = System Fail condition as edge-detect brake source Disabled.
     * |        |          |1 = System Fail condition as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[12]    |BRKP0LEN  |Enable BKP0 Pin As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = PWMx_BRAKE0 pin as level-detect brake source Disabled.
     * |        |          |1 = PWMx_BRAKE0 pin as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[13]    |BRKP1LEN  |Enable BKP1 Pin As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = PWMx_BRAKE1 pin as level-detect brake source Disabled.
     * |        |          |1 = PWMx_BRAKE1 pin as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[15]    |SYSLEN    |Enable System Fail As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = System Fail condition as level-detect brake source Disabled.
     * |        |          |1 = System Fail condition as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[17:16] |BRKAEVEN  |PWM Brake Action Select For Even Channel (Write Protect)
     * |        |          |00 = PWM even channel level-detect brake function not affect channel output.
     * |        |          |01 = PWM even channel output tri-state when level-detect brake happened.
     * |        |          |10 = PWM even channel output low level when level-detect brake happened.
     * |        |          |11 = PWM even channel output high level when level-detect brake happened.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[19:18] |BRKAODD   |PWM Brake Action Select For Odd Channel (Write Protect)
     * |        |          |00 = PWM odd channel level-detect brake function not affect channel output.
     * |        |          |01 = PWM odd channel output tri-state when level-detect brake happened.
     * |        |          |10 = PWM odd channel output low level when level-detect brake happened.
     * |        |          |11 = PWM odd channel output high level when level-detect brake happened.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t BRKCTL0_1;

    /**
     * PWM_BRKCTL2_3
     * ===================================================================================================
     * Offset: 0xCC  PWM Brake Edge Detect Control Register 2_3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |BRKP0EEN  |Enable PWMx_BRAKE0 Pin As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = BKP0 pin as edge-detect brake source Disabled.
     * |        |          |1 = BKP0 pin as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[5]     |BRKP1EEN  |Enable PWMx_BRAKE1 Pin As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = BKP1 pin as edge-detect brake source Disabled.
     * |        |          |1 = BKP1 pin as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[7]     |SYSEEN    |Enable System Fail As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = System Fail condition as edge-detect brake source Disabled.
     * |        |          |1 = System Fail condition as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[12]    |BRKP0LEN  |Enable BKP0 Pin As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = PWMx_BRAKE0 pin as level-detect brake source Disabled.
     * |        |          |1 = PWMx_BRAKE0 pin as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[13]    |BRKP1LEN  |Enable BKP1 Pin As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = PWMx_BRAKE1 pin as level-detect brake source Disabled.
     * |        |          |1 = PWMx_BRAKE1 pin as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[15]    |SYSLEN    |Enable System Fail As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = System Fail condition as level-detect brake source Disabled.
     * |        |          |1 = System Fail condition as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[17:16] |BRKAEVEN  |PWM Brake Action Select For Even Channel (Write Protect)
     * |        |          |00 = PWM even channel level-detect brake function not affect channel output.
     * |        |          |01 = PWM even channel output tri-state when level-detect brake happened.
     * |        |          |10 = PWM even channel output low level when level-detect brake happened.
     * |        |          |11 = PWM even channel output high level when level-detect brake happened.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[19:18] |BRKAODD   |PWM Brake Action Select For Odd Channel (Write Protect)
     * |        |          |00 = PWM odd channel level-detect brake function not affect channel output.
     * |        |          |01 = PWM odd channel output tri-state when level-detect brake happened.
     * |        |          |10 = PWM odd channel output low level when level-detect brake happened.
     * |        |          |11 = PWM odd channel output high level when level-detect brake happened.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t BRKCTL2_3;

    /**
     * PWM_BRKCTL4_5
     * ===================================================================================================
     * Offset: 0xD0  PWM Brake Edge Detect Control Register 4_5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |BRKP0EEN  |Enable PWMx_BRAKE0 Pin As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = BKP0 pin as edge-detect brake source Disabled.
     * |        |          |1 = BKP0 pin as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[5]     |BRKP1EEN  |Enable PWMx_BRAKE1 Pin As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = BKP1 pin as edge-detect brake source Disabled.
     * |        |          |1 = BKP1 pin as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[7]     |SYSEEN    |Enable System Fail As Edge-Detect Brake Source (Write Protect)
     * |        |          |0 = System Fail condition as edge-detect brake source Disabled.
     * |        |          |1 = System Fail condition as edge-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[12]    |BRKP0LEN  |Enable BKP0 Pin As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = PWMx_BRAKE0 pin as level-detect brake source Disabled.
     * |        |          |1 = PWMx_BRAKE0 pin as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[13]    |BRKP1LEN  |Enable BKP1 Pin As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = PWMx_BRAKE1 pin as level-detect brake source Disabled.
     * |        |          |1 = PWMx_BRAKE1 pin as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[15]    |SYSLEN    |Enable System Fail As Level-Detect Brake Source (Write Protect)
     * |        |          |0 = System Fail condition as level-detect brake source Disabled.
     * |        |          |1 = System Fail condition as level-detect brake source Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[17:16] |BRKAEVEN  |PWM Brake Action Select For Even Channel (Write Protect)
     * |        |          |00 = PWM even channel level-detect brake function not affect channel output.
     * |        |          |01 = PWM even channel output tri-state when level-detect brake happened.
     * |        |          |10 = PWM even channel output low level when level-detect brake happened.
     * |        |          |11 = PWM even channel output high level when level-detect brake happened.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[19:18] |BRKAODD   |PWM Brake Action Select For Odd Channel (Write Protect)
     * |        |          |00 = PWM odd channel level-detect brake function not affect channel output.
     * |        |          |01 = PWM odd channel output tri-state when level-detect brake happened.
     * |        |          |10 = PWM odd channel output low level when level-detect brake happened.
     * |        |          |11 = PWM odd channel output high level when level-detect brake happened.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t BRKCTL4_5;

    /**
     * POLCTL
     * ===================================================================================================
     * Offset: 0xD4  PWM Pin Polar Inverse Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |PINVn     |PWM PIN Polar Inverse Control
     * |        |          |The register controls polarity state of PWM output.
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = PWM output polar inverse Disabled.
     * |        |          |1 = PWM output polar inverse Enabled.
     */
    __IO uint32_t POLCTL;

    /**
     * POEN
     * ===================================================================================================
     * Offset: 0xD8  PWM Output Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |POENn     |PWM Pin Output Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = PWM pin at tri-state.
     * |        |          |1 = PWM pin in output mode.
     */
    __IO uint32_t POEN;

    /**
     * SWBRK
     * ===================================================================================================
     * Offset: 0xDC  PWM Software Brake Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2:0]   |BRKETRGn  |PWM Edge Brake Software Trigger (Write Only) (Write Protect)
     * |        |          |Each bit n controls the corresponding PWM pair n.
     * |        |          |Write 1 to this bit will trigger Edge brake, and set BRKEIFn to 1 in PWM_INTSTS1 register.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[10:8]  |BRKLTRGn  |PWM Level Brake Software Trigger (Write Only) (Write Protect)
     * |        |          |Each bit n controls the corresponding PWM pair n.
     * |        |          |Write 1 to this bit will trigger level brake, and set BRKLIFn to 1 in PWM_INTSTS1 register.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t SWBRK;

    /**
     * PWM_INTEN0
     * ===================================================================================================
     * Offset: 0xE0  PWM Interrupt Enable Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ZIEN0     |PWM Zero Point Interrupt Enable 0
     * |        |          |0 = Zero point interrupt Disabled.
     * |        |          |1 = Zero point interrupt Enabled.
     * |        |          |Note: Odd channels will read always 0 at complementary mode.
     * |[2]     |ZIEN2     |PWM Zero Point Interrupt Enable 2
     * |        |          |0 = Zero point interrupt Disabled.
     * |        |          |1 = Zero point interrupt Enabled.
     * |        |          |Note: Odd channels will read always 0 at complementary mode.
     * |[4]     |ZIEN4     |PWM Zero Point Interrupt Enable 4
     * |        |          |0 = Zero point interrupt Disabled.
     * |        |          |1 = Zero point interrupt Enabled.
     * |        |          |Note: Odd channels will read always 0 at complementary mode.
     * |[8]     |PIEN0     |PWM Period Point Interrupt Enable 0
     * |        |          |0 = Period point interrupt Disabled.
     * |        |          |1 = Period point interrupt Enabled.
     * |        |          |Note: When up-down counter type period point means center point.
     * |[10]    |PIEN2     |PWM Period Point Interrupt Enable 2
     * |        |          |0 = Period point interrupt Disabled.
     * |        |          |1 = Period point interrupt Enabled.
     * |        |          |Note: When up-down counter type period point means center point.
     * |[12]    |PIEN4     |PWM Period Point Interrupt Enable 4
     * |        |          |0 = Period point interrupt Disabled.
     * |        |          |1 = Period point interrupt Enabled.
     * |        |          |Note: When up-down counter type period point means center point.
     * |[21:16] |CMPUIENn  |PWM Compare Up Count Interrupt Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Compare up count interrupt Disabled.
     * |        |          |1 = Compare up count interrupt Enabled.
     * |        |          |Note: In complementary mode, CMPUIEN1, 3, 5 use as another CMPUIEN for channel 0, 2, 4.
     * |[29:24] |CMPDIENn  |PWM Compare Down Count Interrupt Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Compare down count interrupt Disabled.
     * |        |          |1 = Compare down count interrupt Enabled.
     * |        |          |Note: In complementary mode, CMPDIEN1, 3, 5 use as another CMPDIEN for channel 0, 2, 4.
     */
    __IO uint32_t INTEN0;

    /**
     * PWM_INTEN1
     * ===================================================================================================
     * Offset: 0xE4  PWM Interrupt Enable Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |BRKEIEN0_1|PWM Edge-Detect Brake Interrupt Enable For Channel0/1 (Write Protect)
     * |        |          |0 = Edge-detect Brake interrupt for channel0/1 Disabled.
     * |        |          |1 = Edge-detect Brake interrupt for channel0/1 Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[1]     |BRKEIEN2_3|PWM Edge-Detect Brake Interrupt Enable For Channel2/3 (Write Protect)
     * |        |          |0 = Edge-detect Brake interrupt for channel2/3 Disabled.
     * |        |          |1 = Edge-detect Brake interrupt for channel2/3 Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[2]     |BRKEIEN4_5|PWM Edge-Detect Brake Interrupt Enable For Channel4/5 (Write Protect)
     * |        |          |0 = Edge-detect Brake interrupt for channel4/5 Disabled.
     * |        |          |1 = Edge-detect Brake interrupt for channel4/5 Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[8]     |BRKLIEN0_1|PWM Level-Detect Brake Interrupt Enable For Channel0/1 (Write Protect)
     * |        |          |0 = Level-detect Brake interrupt for channel0/1 Disabled.
     * |        |          |1 = Level-detect Brake interrupt for channel0/1 Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[9]     |BRKLIEN2_3|PWM Level-Detect Brake Interrupt Enable For Channel2/3 (Write Protect)
     * |        |          |0 = Level-detect Brake interrupt for channel2/3 Disabled.
     * |        |          |1 = Level-detect Brake interrupt for channel2/3 Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[10]    |BRKLIEN4_5|PWM Level-Detect Brake Interrupt Enable For Channel4/5 (Write Protect)
     * |        |          |0 = Level-detect Brake interrupt for channel4/5 Disabled.
     * |        |          |1 = Level-detect Brake interrupt for channel4/5 Enabled.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     */
    __IO uint32_t INTEN1;

    /**
     * PWM_INTSTS0
     * ===================================================================================================
     * Offset: 0xE8  PWM Interrupt Flag Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ZIF0      |PWM Zero Point Interrupt Flag 0
     * |        |          |This bit is set by hardware when PWM_CH0 counter reaches zero, software can write 1
     * |        |          |to clear this bit to zero.
     * |[2]     |ZIF2      |PWM Zero Point Interrupt Flag 2
     * |        |          |This bit is set by hardware when PWM_CH2 counter reaches zero, software can write 1
     * |        |          |to clear this bit to zero.
     * |[4]     |ZIF4      |PWM Zero Point Interrupt Flag 4
     * |        |          |This bit is set by hardware when PWM_CH4 counter reaches zero, software can write 1
     * |        |          |to clear this bit to zero.
     * |[8]     |PIF0      |PWM Period Point Interrupt Flag 0
     * |        |          |This bit is set by hardware when PWM_CH0 counter reaches PWM_PERIOD0, software can
     * |        |          |write 1 to clear this bit to zero.
     * |[10]    |PIF2      |PWM Period Point Interrupt Flag 2
     * |        |          |This bit is set by hardware when PWM_CH2 counter reaches PWM_PERIOD2, software can
     * |        |          |write 1 to clear this bit to zero.
     * |[12]    |PIF4      |PWM Period Point Interrupt Flag 4
     * |        |          |This bit is set by hardware when PWM_CH4 counter reaches PWM_PERIOD4, software can write
     * |        |          |1 to clear this bit to zero.
     * |[21:16] |CMPUIFn   |PWM Compare Up Count Interrupt Flag
     * |        |          |Flag is set by hardware when PWM counter up count and reaches PWM_CMPDATn, software
     * |        |          |can clear this bit by writing 1 to it. Each bit n controls the corresponding
     * |        |          |PWM channel n.
     * |        |          |Note1: If CMPDAT equal to PERIOD, this flag is not working in up counter type selection.
     * |        |          |Note2: In complementary mode, CMPUIF1, 3, 5 use as another CMPUIF for channel 0, 2, 4.
     * |[29:24] |CMPDIFn   |PWM Compare Down Count Interrupt Flag
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |Flag is set by hardware when PWM counter down count and reaches PWM_CMPDATn,
     * |        |          |software can clear this bit by writing 1 to it.
     * |        |          |Note1: If CMPDAT equal to PERIOD, this flag is not working in down counter type selection.
     * |        |          |Note2: In complementary mode, CMPDIF1, 3, 5 use as another CMPDIF for channel 0, 2, 4.
     */
    __IO uint32_t INTSTS0;

    /**
     * PWM_INTSTS1
     * ===================================================================================================
     * Offset: 0xEC  PWM Interrupt Flag Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |BRKEIF0   |PWM Channel0 Edge-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel0 edge-detect brake event do not happened.
     * |        |          |1 = When PWM channel0 edge-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[1]     |BRKEIF1   |PWM Channel1 Edge-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel1 edge-detect brake event do not happened.
     * |        |          |1 = When PWM channel1 edge-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[2]     |BRKEIF2   |PWM Channel2 Edge-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel2 edge-detect brake event do not happened.
     * |        |          |1 = When PWM channel2 edge-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[3]     |BRKEIF3   |PWM Channel3 Edge-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel3 edge-detect brake event do not happened.
     * |        |          |1 = When PWM channel3 edge-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[4]     |BRKEIF4   |PWM Channel4 Edge-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel4 edge-detect brake event do not happened.
     * |        |          |1 = When PWM channel4 edge-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[5]     |BRKEIF5   |PWM Channel5 Edge-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel5 edge-detect brake event do not happened.
     * |        |          |1 = When PWM channel5 edge-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[8]     |BRKLIF0   |PWM Channel0 Level-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel0 level-detect brake event do not happened.
     * |        |          |1 = When PWM channel0 level-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[9]     |BRKLIF1   |PWM Channel1 Level-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel1 level-detect brake event do not happened.
     * |        |          |1 = When PWM channel1 level-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[10]    |BRKLIF2   |PWM Channel2 Level-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel2 level-detect brake event do not happened.
     * |        |          |1 = When PWM channel2 level-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[11]    |BRKLIF3   |PWM Channel3 Level-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel3 level-detect brake event do not happened.
     * |        |          |1 = When PWM channel3 level-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[12]    |BRKLIF4   |PWM Channel4 Level-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel4 level-detect brake event do not happened.
     * |        |          |1 = When PWM channel4 level-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer to REGWRPROT register.
     * |[13]    |BRKLIF5   |PWM Channel5 Level-Detect Brake Interrupt Flag (Write Protect)
     * |        |          |0 = PWM channel5 level-detect brake event do not happened.
     * |        |          |1 = When PWM channel5 level-detect brake event happened, this bit is set to 1,
     * |        |          |writing 1 to clear.
     * |        |          |Note: This register is write protected. Refer toREGWRPROT register.
     * |[16]    |BRKESTS0  |PWM Channel0 Edge-Detect Brake Status
     * |        |          |0 = PWM channel0 edge-detect brake state is released.
     * |        |          |1 = When PWM channel0 edge-detect brake detects a falling edge of any enabled brake
     * |        |          |source; this flag will be set to indicate the PWM channel0 at brake state,
     * |        |          |writing 1 to clear.
     * |[17]    |BRKESTS1  |PWM Channel1 Edge-Detect Brake Status
     * |        |          |0 = PWM channel1 edge-detect brake state is released.
     * |        |          |1 = When PWM channel1 edge-detect brake detects a falling edge of any enabled brake
     * |        |          |source; this flag will be set to indicate the PWM channel1 at brake state,
     * |        |          |writing 1 to clear.
     * |[18]    |BRKESTS2  |PWM Channel2 Edge-Detect Brake Status
     * |        |          |0 = PWM channel2 edge-detect brake state is released.
     * |        |          |1 = When PWM channel2 edge-detect brake detects a falling edge of any enabled brake
     * |        |          |source; this flag will be set to indicate the PWM channel2 at brake state,
     * |        |          |writing 1 to clear.
     * |[19]    |BRKESTS3  |PWM Channel3 Edge-Detect Brake Status
     * |        |          |0 = PWM channel3 edge-detect brake state is released.
     * |        |          |1 = When PWM channel3 edge-detect brake detects a falling edge of any enabled brake
     * |        |          |source; this flag will be set to indicate the PWM channel3 at brake state,
     * |        |          |writing 1 to clear.
     * |[20]    |BRKESTS4  |PWM Channel4 Edge-Detect Brake Status
     * |        |          |0 = PWM channel4 edge-detect brake state is released.
     * |        |          |1 = When PWM channel4 edge-detect brake detects a falling edge of any enabled brake
     * |        |          |source; this flag will be set to indicate the PWM channel4 at brake state,
     * |        |          |writing 1 to clear.
     * |[21]    |BRKESTS5  |PWM Channel5 Edge-Detect Brake Status
     * |        |          |0 = PWM channel5 edge-detect brake state is released.
     * |        |          |1 = When PWM channel5 edge-detect brake detects a falling edge of any enabled brake
     * |        |          |source; this flag will be set to indicate the PWM channel5 at brake state,
     * |        |          |writing 1 to clear.
     * |[24]    |BRKLSTS0  |PWM Channel0 Level-Detect Brake Status (Read Only)
     * |        |          |0 = PWM channel0 level-detect brake state is released.
     * |        |          |1 = When PWM channel0 level-detect brake detects a falling edge of any enabled
     * |        |          |brake source; this flag will be set to indicate the PWM channel0 at brake state.
     * |        |          |Note: This bit is read only and auto
     * |        |          |cleared by hardware. When enabled brake source return to high level, PWM will
     * |        |          |release brake state until current PWM period finished. The PWM waveform will
     * |        |          |start output from next full PWM period.
     * |[25]    |BRKLSTS1  |PWM Channel1 Level-Detect Brake Status (Read Only)
     * |        |          |0 = PWM channel1 level-detect brake state is released.
     * |        |          |1 = When PWM channel1 level-detect brake detects a falling edge of any enabled
     * |        |          |brake source; this flag will be set to indicate the PWM channel1 at brake state.
     * |        |          |Note: This bit is read only and auto
     * |        |          |cleared by hardware. When enabled brake source return to high level, PWM will
     * |        |          |release brake state until current PWM period finished. The PWM waveform will
     * |        |          |start output from next full PWM period.
     * |[26]    |BRKLSTS2  |PWM Channel2 Level-Detect Brake Status (Read Only)
     * |        |          |0 = PWM channel2 level-detect brake state is released.
     * |        |          |1 = When PWM channel2 level-detect brake detects a falling edge of any enabled
     * |        |          |brake source; this flag will be set to indicate the PWM channel2 at brake
     * |        |          |state.
     * |        |          |Note: This bit is read only and auto
     * |        |          |cleared by hardware. When enabled brake source return to high level, PWM will
     * |        |          |release brake state until current PWM period finished. The PWM waveform will
     * |        |          |start output from next full PWM period.
     * |[27]    |BRKLSTS3  |PWM Channel3 Level-Detect Brake Status (Read Only)
     * |        |          |0 = PWM channel3 level-detect brake state is released.
     * |        |          |1 = When PWM channel3 level-detect brake detects a falling edge of any enabled
     * |        |          |brake source; this flag will be set to indicate the PWM channel3 at brake
     * |        |          |state.
     * |        |          |Note: This bit is read only and auto
     * |        |          |cleared by hardware. When enabled brake source return to high level, PWM will
     * |        |          |release brake state until current PWM period finished. The PWM waveform will
     * |        |          |start output from next full PWM period.
     * |[28]    |BRKLSTS4  |PWM Channel4 Level-Detect Brake Status (Read Only)
     * |        |          |0 = PWM channel4 level-detect brake state is released.
     * |        |          |1 = When PWM channel4 level-detect brake detects a falling edge of any enabled
     * |        |          |brake source; this flag will be set to indicate the PWM channel4 at brake
     * |        |          |state.
     * |        |          |Note: This bit is read only and auto
     * |        |          |cleared by hardware. When enabled brake source return to high level, PWM will
     * |        |          |release brake state until current PWM period finished. The PWM waveform will
     * |        |          |start output from next full PWM period.
     * |[29]    |BRKLSTS5  |PWM Channel5 Level-Detect Brake Status (Read Only)
     * |        |          |0 = PWM channel5 level-detect brake state is released.
     * |        |          |1 = When PWM channel5 level-detect brake detects a falling edge of any enabled
     * |        |          |brake source; this flag will be set to indicate the PWM channel5 at brake
     * |        |          |state.
     * |        |          |Note: This bit is read only and auto
     * |        |          |cleared by hardware. When enabled brake source return to high level, PWM will
     * |        |          |release brake state until current PWM period finished. The PWM waveform will
     * |        |          |start output from next full PWM period.
     */
    __IO uint32_t INTSTS1;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED6
     * ===================================================================================================
     * Offset: 0x00F0 ~ 0x00F4
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED6[2];
    /**
     * @endcond
     */

    /**
     * PWM_ADCTS0
     * ===================================================================================================
     * Offset: 0xF8  PWM Trigger ADC Source Select Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |TRGSEL0   |PWM_CH0 Trigger ADC Source Select
     * |        |          |0000 = PWM_CH0 zero point.
     * |        |          |0001 = PWM_CH0 period point.
     * |        |          |0010 = PWM_CH0 zero or period point.
     * |        |          |0011 = PWM_CH0 up-count CMPDAT point.
     * |        |          |0100 = PWM_CH0 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = PWM_CH1 up-count CMPDAT point.
     * |        |          |1001 = PWM_CH1 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[7]     |TRGEN0    |PWM_CH0 Trigger ADC Enable Bit
     * |[11:8]  |TRGSEL1   |PWM_CH1 Trigger ADC Source Select
     * |        |          |0000 = PWM_CH0 zero point.
     * |        |          |0001 = PWM_CH0 period point.
     * |        |          |0010 = PWM_CH0 zero or period point.
     * |        |          |0011 = PWM_CH0 up-count CMPDAT point.
     * |        |          |0100 = PWM_CH0 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = PWM_CH1 up-count CMPDAT point.
     * |        |          |1001 = PWM_CH1 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[15]    |TRGEN1    |PWM_CH1 Trigger ADC Enable Bit
     * |[19:16] |TRGSEL2   |PWM_CH2 Trigger ADC Source Select
     * |        |          |0000 = PWM_CH2 zero point.
     * |        |          |0001 = PWM_CH2 period point.
     * |        |          |0010 = PWM_CH2 zero or period point.
     * |        |          |0011 = PWM_CH2 up-count CMPDAT point.
     * |        |          |0100 = PWM_CH2 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = PWM_CH3 up-count CMPDAT point.
     * |        |          |1001 = PWM_CH3 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[23]    |TRGEN2    |PWM_CH2 Trigger ADC Enable Bit
     * |[27:24] |TRGSEL3   |PWM_CH3 Trigger ADC Source Select
     * |        |          |0000 = PWM_CH2 zero point.
     * |        |          |0001 = PWM_CH2 period point.
     * |        |          |0010 = PWM_CH2 zero or period point.
     * |        |          |0011 = PWM_CH2 up-count CMPDAT point.
     * |        |          |0100 = PWM_CH2 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = PWM_CH3 up-count CMPDAT point.
     * |        |          |1001 = PWM_CH3 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[31]    |TRGEN3    |PWM_CH3 Trigger ADC Enable Bit
     */
    __IO uint32_t ADCTS0;

    /**
     * PWM_ADCTS1
     * ===================================================================================================
     * Offset: 0xFC  PWM Trigger ADC Source Select Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |TRGSEL4   |PWM_CH4 Trigger ADC Source Select
     * |        |          |0000 = PWM_CH4 zero point.
     * |        |          |0001 = PWM_CH4 period point.
     * |        |          |0010 = PWM_CH4 zero or period point.
     * |        |          |0011 = PWM_CH4 up-count CMPDAT point.
     * |        |          |0100 = PWM_CH4 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = PWM_CH5 up-count CMPDAT point.
     * |        |          |1001 = PWM_CH5 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[7]     |TRGEN4    |PWM_CH4 Trigger ADC Enable Bit
     * |[11:8]  |TRGSEL5   |PWM_CH5 Trigger ADC Source Select
     * |        |          |0000 = PWM_CH4 zero point.
     * |        |          |0001 = PWM_CH4 period point.
     * |        |          |0010 = PWM_CH4 zero or period point.
     * |        |          |0011 = PWM_CH4 up-count CMPDAT point.
     * |        |          |0100 = PWM_CH4 down-count CMPDAT point.
     * |        |          |0101 = Reserved.
     * |        |          |0110 = Reserved.
     * |        |          |0111 = Reserved.
     * |        |          |1000 = PWM_CH5 up-count CMPDAT point.
     * |        |          |1001 = PWM_CH5 down-count CMPDAT point.
     * |        |          |Others = reserved.
     * |[15]    |TRGEN5    |PWM_CH5 Trigger ADC Enable Bit
     */
    __IO uint32_t ADCTS1;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED7
     * ===================================================================================================
     * Offset: 0x0100 ~ 0x010C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED7[4];
    /**
     * @endcond
     */

    /**
     * PWM_SSCTL
     * ===================================================================================================
     * Offset: 0x110  PWM Synchronous Start Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |SSEN0     |PWM Synchronous Start Function Enable 0
     * |        |          |When synchronous start function is enabled, the PWM_CH0 counter enable bit (CNTEN0)
     * |        |          |can be enabled by writing PWM synchronous start trigger bit (CNTSEN).
     * |        |          |0 = PWM synchronous start function Disabled.
     * |        |          |1 = PWM synchronous start function Enabled.
     * |[2]     |SSEN2     |PWM Synchronous Start Function Enable 2
     * |        |          |When synchronous start function is enabled, the PWM_CH2 counter enable bit (CNTEN2)
     * |        |          |can be enabled by writing PWM synchronous start trigger bit (CNTSEN).
     * |        |          |0 = PWM synchronous start function Disabled.
     * |        |          |1 = PWM synchronous start function Enabled.
     * |[4]     |SSEN4     |PWM Synchronous Start Function Enable 4
     * |        |          |When synchronous start function is enabled, the PWM_CH4 counter enable bit (CNTEN4)
     * |        |          |can be enabled by writing PWM synchronous start trigger bit (CNTSEN).
     * |        |          |0 = PWM synchronous start function Disabled.
     * |        |          |1 = PWM synchronous start function Enabled.
     * |[9:8]   |SSRC      |PWM Synchronous Start Source Select
     * |        |          |00 = Synchronous start source come from PWM0.
     * |        |          |01 = Synchronous start source come from PWM1.
     * |        |          |10 = Synchronous start source come from BPWM0.
     * |        |          |11 = Synchronous start source come from BPWM1.
     */
    __IO uint32_t SSCTL;

    /**
     * PWM_SSTRG
     * ===================================================================================================
     * Offset: 0x114  PWM Synchronous Start Trigger Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTSEN    |PWM Counter Synchronous Start Enable (Write Only)
     * |        |          |PMW counter synchronous enable function is used to make selected PWM channels
     * |        |          |(include PWM0_CHx and PWM1_CHx) start counting at the same time.
     * |        |          |Writing this bit to 1 will also set the counter enable bit (CNTENn, n denotes channel 0
     * |        |          |to 5) if correlated PWM channel counter synchronous start function is enabled.
     * |        |          |Note: This bit only present in PWM0_BA.
     */
    __IO uint32_t SSTRG;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED8
     * ===================================================================================================
     * Offset: 0x0118 ~ 0x011C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED8[2];
    /**
     * @endcond
     */


    /**
     * PWM_STATUS
     * ===================================================================================================
     * Offset: 0x120  PWM Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTMAX0   |Time-Base Counter 0 Equal To 0xFFFF Latched Status
     * |        |          |0 = indicates the time-base counter never reached its maximum value 0xFFFF.
     * |        |          |1 = indicates the time-base counter reached its maximum value, software can write 1
     * |        |          |to clear this bit.
     * |[2]     |CNTMAX2   |Time-Base Counter 2 Equal To 0xFFFF Latched Status
     * |        |          |0 = indicates the time-base counter never reached its maximum value 0xFFFF.
     * |        |          |1 = indicates the time-base counter reached its maximum value, software can write 1
     * |        |          |to clear this bit.
     * |[4]     |CNTMAX4   |Time-Base Counter 4 Equal To 0xFFFF Latched Status
     * |        |          |0 = indicates the time-base counter never reached its maximum value 0xFFFF.
     * |        |          |1 = indicates the time-base counter reached its maximum value, software can write 1
     * |        |          |to clear this bit.
     * |[21:16] |ADCTRGn   |ADC Start Of Conversion Status
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Indicates no ADC start of conversion trigger event has occurred.
     * |        |          |1 = Indicates an ADC start of conversion trigger event has occurred, software can
     * |        |          |write 1 to clear this bit.
     */
    __IO uint32_t STATUS;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED9
     * ===================================================================================================
     * Offset: 0x0124 ~ 0x01FC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED9[55];
    /**
     * @endcond
     */

    /**
     * PWM_CAPINEN
     * ===================================================================================================
     * Offset: 0x200  PWM Capture Input Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPINENn  |Capture Input Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = PWM Channel capture input path Disabled. The input of PWM channel capture
     * |        |          |function is always regarded as 0.
     * |        |          |1 = PWM Channel capture input path Enabled. The input of PWM channel capture
     * |        |          |function comes from correlative multifunction pin.
     */
    __IO uint32_t CAPINEN;

    /**
     * PWM_CAPCTL
     * ===================================================================================================
     * Offset: 0x204  PWM Capture Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPENn    |Capture Function Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Capture function Disabled. RCAPDAT/FCAPDAT register will not be updated.
     * |        |          |1 = Capture function Enabled. Capture latched the PWM counter value when detected
     * |        |          |rising or falling edge of input signal and saved to RCAPDAT (Rising latch) and FCAPDAT
     * |        |          |(Falling latch).
     * |[13:8]  |CAPINVn   |Capture Inverter Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Capture source inverter Disabled.
     * |        |          |1 = Capture source inverter Enabled. Reverse the input signal from GPIO.
     * |[21:16] |RCRLDENn  |Rising Capture Reload Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Rising capture reload counter Disabled.
     * |        |          |1 = Rising capture reload counter Enabled.
     * |[29:24] |FCRLDENn  |Falling Capture Reload Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Falling capture reload counter Disabled.
     * |        |          |1 = Falling capture reload counter Enabled.
     */
    __IO uint32_t CAPCTL;

    /**
     * PWM_CAPSTS
     * ===================================================================================================
     * Offset: 0x208  PWM Capture Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CRIFOVn   |Capture Rising Interrupt Flag Overrun Status (Read Only)
     * |        |          |This flag indicates if rising latch happened when the corresponding CAPRIF is 1.
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |Note: This bit will be cleared automatically when user clear corresponding CAPRIF.
     * |[13:8]  |CFIFOVn   |Capture Falling Interrupt Flag Overrun Status (Read Only)
     * |        |          |This flag indicates if falling latch happened when the corresponding CAPFIF is 1.
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |Note: This bit will be cleared automatically when user clear corresponding CAPFIF.
     */
    __IO uint32_t CAPSTS;

    /**
     * PWM_RCAPDAT0
     * ===================================================================================================
     * Offset: 0x20C  PWM Rising Capture Data Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |PWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT0;

    /**
     * PWM_FCAPDAT0
     * ===================================================================================================
     * Offset: 0x210  PWM Falling Capture Data Register 0
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |PWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT0;

    /**
     * PWM_RCAPDAT1
     * ===================================================================================================
     * Offset: 0x214  PWM Rising Capture Data Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |PWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT1;

    /**
     * FCAPDAT1
     * ===================================================================================================
     * Offset: 0x218  PWM Falling Capture Data Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |PWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT1;

    /**
     * RCAPDAT2
     * ===================================================================================================
     * Offset: 0x21C  PWM Rising Capture Data Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |PWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT2;

    /**
     * FCAPDAT2
     * ===================================================================================================
     * Offset: 0x220  PWM Falling Capture Data Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |PWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT2;

    /**
     * RCAPDAT3
     * ===================================================================================================
     * Offset: 0x224  PWM Rising Capture Data Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |PWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT3;

    /**
     * FCAPDAT3
     * ===================================================================================================
     * Offset: 0x228  PWM Falling Capture Data Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |PWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT3;

    /**
     * RCAPDAT4
     * ===================================================================================================
     * Offset: 0x22C  PWM Rising Capture Data Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |PWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT4;

    /**
     * FCAPDAT4
     * ===================================================================================================
     * Offset: 0x230  PWM Falling Capture Data Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |PWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT4;

    /**
     * RCAPDAT5
     * ===================================================================================================
     * Offset: 0x234  PWM Rising Capture Data Register 5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |RCAPDAT   |PWM Rising Capture Data Register (Read Only)
     * |        |          |When rising capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t RCAPDAT5;

    /**
     * FCAPDAT5
     * ===================================================================================================
     * Offset: 0x238  PWM Falling Capture Data Register 5
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |FCAPDAT   |PWM Falling Capture Data Register (Read Only)
     * |        |          |When falling capture condition happened, the PWM counter value will be saved in this register.
     */
    __IO uint32_t FCAPDAT5;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED10
     * ===================================================================================================
     * Offset: 0x023C ~ 0x024C
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED10[5];
    /**
     * @endcond
     */

    /**
     * CAPIEN
     * ===================================================================================================
     * Offset: 0x250  PWM Capture Interrupt Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CAPRIENn  |PWM Capture Rising Latch Interrupt Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Capture rising edge latch interrupt Disabled.
     * |        |          |1 = Capture rising edge latch interrupt Enabled.
     * |[13:8]  |CAPFIENn  |PWM Capture Falling Latch Interrupt Enable
     * |        |          |Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = Capture falling edge latch interrupt Disabled.
     * |        |          |1 = Capture falling edge latch interrupt Enabled.
     */
    __IO uint32_t CAPIEN;

    /**
     * CAPIF
     * ===================================================================================================
     * Offset: 0x254  PWM Capture Interrupt Flag Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |CRLIFn    |PWM Capture Rising Latch Interrupt Flag
     * |        |          |This bit is writing 1 to clear. Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = No capture rising latch condition happened.
     * |        |          |1 = Capture rising latch condition happened, this flag will be set to high.
     * |[13:8]  |CFLIFn    |PWM Capture Falling Latch Interrupt Flag
     * |        |          |This bit is writing 1 to clear. Each bit n controls the corresponding PWM channel n.
     * |        |          |0 = No capture falling latch condition happened.
     * |        |          |1 = Capture falling latch condition happened, this flag will be set to high.
     */
    __IO uint32_t CAPIF;

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED11
     * ===================================================================================================
     * Offset: 0x0258 ~ 0x0300
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED11[43];
    /**
     * @endcond
     */

    /**
     * PBUF0, 2, 4
     * ===================================================================================================
     * Offset: 0x304, 0x30C, 0x314  PWM PERIOD0, 2, 4 Buffer (offset: 0x308, 0x310, 0x318 are reserved)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |PBUF      |PWM Period Register Buffer (Read Only)
     * |        |          |Used as PERIOD active register.
     */
    __I uint32_t PBUF[6];

    /**
     * CMPBUF0~5
     * ===================================================================================================
     * Offset: 0x31C~0x330  PWM CMPDAT0~5 Buffer
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |CMPBUF    |PWM Comparator Register Buffer (Read Only)
     * |        |          |Used as CMP active register.
     */
    __I uint32_t CMPBUF[6];

    /**
     * @cond HIDDEN_SYMBOLS
     * RESERVED12
     * ===================================================================================================
     * Offset: 0x0334 ~ 0x0FFC
     * ---------------------------------------------------------------------------------------------------
     */
    __I  uint32_t RESERVED12[819];          /*!< Offset: 0x0334 ~ 0x0FFC    Reserved*/
    /**
     * @endcond
     */

} PWM_T;

/**
    @addtogroup PWM_CONST PWM Bit Field Definition
    Constant Definitions for PWM Controller
@{ */


#define PWM_CTL0_CTRLDn_Pos              (0)                                                /*!< PWM_T::CTL0: CTRLDn Position              */
#define PWM_CTL0_CTRLDn_Msk              (0x3ful << PWM_CTL0_CTRLDn_Pos)                    /*!< PWM_T::CTL0: CTRLDn Mask                  */

#define PWM_CTL0_CTRLD0_Pos              (0)                                                /*!< PWM_T::CTL0: CTRLD0 Position              */
#define PWM_CTL0_CTRLD0_Msk              (0x1ul << PWM_CTL0_CTRLD0_Pos)                     /*!< PWM_T::CTL0: CTRLD0 Mask                  */

#define PWM_CTL0_CTRLD1_Pos              (1)                                                /*!< PWM_T::CTL0: CTRLD1 Position              */
#define PWM_CTL0_CTRLD1_Msk              (0x1ul << PWM_CTL0_CTRLD1_Pos)                     /*!< PWM_T::CTL0: CTRLD1 Mask                  */

#define PWM_CTL0_CTRLD2_Pos              (2)                                                /*!< PWM_T::CTL0: CTRLD2 Position              */
#define PWM_CTL0_CTRLD2_Msk              (0x1ul << PWM_CTL0_CTRLD2_Pos)                     /*!< PWM_T::CTL0: CTRLD2 Mask                  */

#define PWM_CTL0_CTRLD3_Pos              (3)                                                /*!< PWM_T::CTL0: CTRLD3 Position              */
#define PWM_CTL0_CTRLD3_Msk              (0x1ul << PWM_CTL0_CTRLD3_Pos)                     /*!< PWM_T::CTL0: CTRLD3 Mask                  */

#define PWM_CTL0_CTRLD4_Pos              (4)                                                /*!< PWM_T::CTL0: CTRLD4 Position              */
#define PWM_CTL0_CTRLD4_Msk              (0x1ul << PWM_CTL0_CTRLD4_Pos)                     /*!< PWM_T::CTL0: CTRLD4 Mask                  */

#define PWM_CTL0_CTRLD5_Pos              (5)                                                /*!< PWM_T::CTL0: CTRLD5 Position              */
#define PWM_CTL0_CTRLD5_Msk              (0x1ul << PWM_CTL0_CTRLD5_Pos)                     /*!< PWM_T::CTL0: CTRLD5 Mask                  */

#define PWM_CTL0_IMMLDENn_Pos            (16)                                               /*!< PWM_T::CTL0: IMMLDENn Position            */
#define PWM_CTL0_IMMLDENn_Msk            (0x3ful << PWM_CTL0_IMMLDENn_Pos)                  /*!< PWM_T::CTL0: IMMLDENn Mask                */

#define PWM_CTL0_IMMLDEN0_Pos            (16)                                               /*!< PWM_T::CTL0: IMMLDEN0 Position            */
#define PWM_CTL0_IMMLDEN0_Msk            (0x1ul << PWM_CTL0_IMMLDEN0_Pos)                   /*!< PWM_T::CTL0: IMMLDEN0 Mask                */

#define PWM_CTL0_IMMLDEN1_Pos            (17)                                               /*!< PWM_T::CTL0: IMMLDEN1 Position            */
#define PWM_CTL0_IMMLDEN1_Msk            (0x1ul << PWM_CTL0_IMMLDEN1_Pos)                   /*!< PWM_T::CTL0: IMMLDEN1 Mask                */

#define PWM_CTL0_IMMLDEN2_Pos            (18)                                               /*!< PWM_T::CTL0: IMMLDEN2 Position            */
#define PWM_CTL0_IMMLDEN2_Msk            (0x1ul << PWM_CTL0_IMMLDEN2_Pos)                   /*!< PWM_T::CTL0: IMMLDEN2 Mask                */

#define PWM_CTL0_IMMLDEN3_Pos            (19)                                               /*!< PWM_T::CTL0: IMMLDEN3 Position            */
#define PWM_CTL0_IMMLDEN3_Msk            (0x1ul << PWM_CTL0_IMMLDEN3_Pos)                   /*!< PWM_T::CTL0: IMMLDEN3 Mask                */

#define PWM_CTL0_IMMLDEN4_Pos            (20)                                               /*!< PWM_T::CTL0: IMMLDEN4 Position            */
#define PWM_CTL0_IMMLDEN4_Msk            (0x1ul << PWM_CTL0_IMMLDEN4_Pos)                   /*!< PWM_T::CTL0: IMMLDEN4 Mask                */

#define PWM_CTL0_IMMLDEN5_Pos            (21)                                               /*!< PWM_T::CTL0: IMMLDEN5 Position            */
#define PWM_CTL0_IMMLDEN5_Msk            (0x1ul << PWM_CTL0_IMMLDEN5_Pos)                   /*!< PWM_T::CTL0: IMMLDEN5 Mask                */

#define PWM_CTL0_DBGHALT_Pos             (30)                                               /*!< PWM_T::CTL0: DBGHALT Position             */
#define PWM_CTL0_DBGHALT_Msk             (0x1ul << PWM_CTL0_DBGHALT_Pos)                    /*!< PWM_T::CTL0: DBGHALT Mask                 */

#define PWM_CTL0_DBGTRIOFF_Pos           (31)                                               /*!< PWM_T::CTL0: DBGTRIOFF Position           */
#define PWM_CTL0_DBGTRIOFF_Msk           (0x1ul << PWM_CTL0_DBGTRIOFF_Pos)                  /*!< PWM_T::CTL0: DBGTRIOFF Mask               */

#define PWM_CTL1_CNTTYPEn_Pos            (0)                                                /*!< PWM_T::CTL1: CNTTYPEn Position            */
#define PWM_CTL1_CNTTYPEn_Msk            (0x333ul << PWM_CTL1_CNTTYPEn_Pos)                 /*!< PWM_T::CTL1: CNTTYPEn Mask                */

#define PWM_CTL1_CNTTYPE0_Pos            (0)                                                /*!< PWM_T::CTL1: CNTTYPE0 Position            */
#define PWM_CTL1_CNTTYPE0_Msk            (0x3ul << PWM_CTL1_CNTTYPE0_Pos)                   /*!< PWM_T::CTL1: CNTTYPE0 Mask                */

#define PWM_CTL1_CNTTYPE2_Pos            (4)                                                /*!< PWM_T::CTL1: CNTTYPE2 Position            */
#define PWM_CTL1_CNTTYPE2_Msk            (0x3ul << PWM_CTL1_CNTTYPE2_Pos)                   /*!< PWM_T::CTL1: CNTTYPE2 Mask                */

#define PWM_CTL1_CNTTYPE4_Pos            (8)                                                /*!< PWM_T::CTL1: CNTTYPE4 Position            */
#define PWM_CTL1_CNTTYPE4_Msk            (0x3ul << PWM_CTL1_CNTTYPE4_Pos)                   /*!< PWM_T::CTL1: CNTTYPE4 Mask                */

#define PWM_CTL1_OUTMODEn_Pos            (24)                                               /*!< PWM_T::CTL1: OUTMODEn Position            */
#define PWM_CTL1_OUTMODEn_Msk            (0x7ul << PWM_CTL1_OUTMODEn_Pos)                   /*!< PWM_T::CTL1: OUTMODEn Mask                */

#define PWM_CTL1_OUTMODE0_Pos            (24)                                               /*!< PWM_T::CTL1: OUTMODE0 Position            */
#define PWM_CTL1_OUTMODE0_Msk            (0x1ul << PWM_CTL1_OUTMODE0_Pos)                   /*!< PWM_T::CTL1: OUTMODE0 Mask                */

#define PWM_CTL1_OUTMODE2_Pos            (25)                                               /*!< PWM_T::CTL1: OUTMODE2 Position            */
#define PWM_CTL1_OUTMODE2_Msk            (0x1ul << PWM_CTL1_OUTMODE2_Pos)                   /*!< PWM_T::CTL1: OUTMODE2 Mask                */

#define PWM_CTL1_OUTMODE4_Pos            (26)                                               /*!< PWM_T::CTL1: OUTMODE4 Position            */
#define PWM_CTL1_OUTMODE4_Msk            (0x1ul << PWM_CTL1_OUTMODE4_Pos)                   /*!< PWM_T::CTL1: OUTMODE4 Mask                */

#define PWM_CLKSRC_ECLKSRC0_Pos          (0)                                                /*!< PWM_T::CLKSRC: ECLKSRC0 Position          */
#define PWM_CLKSRC_ECLKSRC0_Msk          (0x7ul << PWM_CLKSRC_ECLKSRC0_Pos)                 /*!< PWM_T::CLKSRC: ECLKSRC0 Mask              */

#define PWM_CLKSRC_ECLKSRC2_Pos          (8)                                                /*!< PWM_T::CLKSRC: ECLKSRC2 Position          */
#define PWM_CLKSRC_ECLKSRC2_Msk          (0x7ul << PWM_CLKSRC_ECLKSRC2_Pos)                 /*!< PWM_T::CLKSRC: ECLKSRC2 Mask              */

#define PWM_CLKSRC_ECLKSRC4_Pos          (16)                                               /*!< PWM_T::CLKSRC: ECLKSRC4 Position          */
#define PWM_CLKSRC_ECLKSRC4_Msk          (0x7ul << PWM_CLKSRC_ECLKSRC4_Pos)                 /*!< PWM_T::CLKSRC: ECLKSRC4 Mask              */

#define PWM_CLKPSC0_1_CLKPSC_Pos         (0)                                                /*!< PWM_T::CLKPSC0_1: CLKPSC Position         */
#define PWM_CLKPSC0_1_CLKPSC_Msk         (0xffful << PWM_CLKPSC0_1_CLKPSC_Pos)              /*!< PWM_T::CLKPSC0_1: CLKPSC Mask             */

#define PWM_CLKPSC2_3_CLKPSC_Pos         (0)                                                /*!< PWM_T::CLKPSC2_3: CLKPSC Position         */
#define PWM_CLKPSC2_3_CLKPSC_Msk         (0xffful << PWM_CLKPSC2_3_CLKPSC_Pos)              /*!< PWM_T::CLKPSC2_3: CLKPSC Mask             */

#define PWM_CLKPSC4_5_CLKPSC_Pos         (0)                                                /*!< PWM_T::CLKPSC4_5: CLKPSC Position         */
#define PWM_CLKPSC4_5_CLKPSC_Msk         (0xffful << PWM_CLKPSC4_5_CLKPSC_Pos)              /*!< PWM_T::CLKPSC4_5: CLKPSC Mask             */

#define PWM_CNTEN_CNTENn_Pos             (0)                                                /*!< PWM_T::CNTEN: CNTENn Position             */
#define PWM_CNTEN_CNTENn_Msk             (0x15ul << PWM_CNTEN_CNTENn_Pos)                   /*!< PWM_T::CNTEN: CNTENn Mask                 */

#define PWM_CNTEN_CNTEN0_Pos             (0)                                                /*!< PWM_T::CNTEN: CNTEN0 Position             */
#define PWM_CNTEN_CNTEN0_Msk             (0x1ul << PWM_CNTEN_CNTEN0_Pos)                    /*!< PWM_T::CNTEN: CNTEN0 Mask                 */

#define PWM_CNTEN_CNTEN2_Pos             (2)                                                /*!< PWM_T::CNTEN: CNTEN2 Position             */
#define PWM_CNTEN_CNTEN2_Msk             (0x1ul << PWM_CNTEN_CNTEN2_Pos)                    /*!< PWM_T::CNTEN: CNTEN2 Mask                 */

#define PWM_CNTEN_CNTEN4_Pos             (4)                                                /*!< PWM_T::CNTEN: CNTEN4 Position             */
#define PWM_CNTEN_CNTEN4_Msk             (0x1ul << PWM_CNTEN_CNTEN4_Pos)                    /*!< PWM_T::CNTEN: CNTEN4 Mask                 */

#define PWM_CNTCLR_CNTCLRn_Pos           (0)                                                /*!< PWM_T::CNTCLR: CNTCLRn Position           */
#define PWM_CNTCLR_CNTCLRn_Msk           (0x15ul << PWM_CNTCLR_CNTCLRn_Pos)                 /*!< PWM_T::CNTCLR: CNTCLRn Mask               */

#define PWM_CNTCLR_CNTCLR0_Pos           (0)                                                /*!< PWM_T::CNTCLR: CNTCLR0 Position           */
#define PWM_CNTCLR_CNTCLR0_Msk           (0x1ul << PWM_CNTCLR_CNTCLR0_Pos)                  /*!< PWM_T::CNTCLR: CNTCLR0 Mask               */

#define PWM_CNTCLR_CNTCLR2_Pos           (2)                                                /*!< PWM_T::CNTCLR: CNTCLR2 Position           */
#define PWM_CNTCLR_CNTCLR2_Msk           (0x1ul << PWM_CNTCLR_CNTCLR2_Pos)                  /*!< PWM_T::CNTCLR: CNTCLR2 Mask               */

#define PWM_CNTCLR_CNTCLR4_Pos           (4)                                                /*!< PWM_T::CNTCLR: CNTCLR4 Position           */
#define PWM_CNTCLR_CNTCLR4_Msk           (0x1ul << PWM_CNTCLR_CNTCLR4_Pos)                  /*!< PWM_T::CNTCLR: CNTCLR4 Mask               */

#define PWM_PERIOD_PERIOD_Pos            (0)                                                /*!< PWM_T::PERIOD: PERIOD Position            */
#define PWM_PERIOD_PERIOD_Msk            (0xfffful << PWM_PERIOD_PERIOD_Pos)                /*!< PWM_T::PERIOD: PERIOD Mask                */

#define PWM_CMPDAT_CMP_Pos               (0)                                                /*!< PWM_T::CMPDAT: CMP Position               */
#define PWM_CMPDAT_CMP_Msk               (0xfffful << PWM_CMPDAT_CMP_Pos)                   /*!< PWM_T::CMPDAT: CMP Mask                   */

#define PWM_DTCTL0_1_DTCNT_Pos           (0)                                                /*!< PWM_T::DTCTL0_1: DTCNT Position           */
#define PWM_DTCTL0_1_DTCNT_Msk           (0xffful << PWM_DTCTL0_1_DTCNT_Pos)                /*!< PWM_T::DTCTL0_1: DTCNT Mask               */

#define PWM_DTCTL0_1_DTEN_Pos            (16)                                               /*!< PWM_T::DTCTL0_1: DTEN Position            */
#define PWM_DTCTL0_1_DTEN_Msk            (0x1ul << PWM_DTCTL0_1_DTEN_Pos)                   /*!< PWM_T::DTCTL0_1: DTEN Mask                */

#define PWM_DTCTL0_1_DTCKSEL_Pos         (24)                                               /*!< PWM_T::DTCTL0_1: DTCKSEL Position         */
#define PWM_DTCTL0_1_DTCKSEL_Msk         (0x1ul << PWM_DTCTL0_1_DTCKSEL_Pos)                /*!< PWM_T::DTCTL0_1: DTCKSEL Mask             */

#define PWM_DTCTL2_3_DTCNT_Pos           (0)                                                /*!< PWM_T::DTCTL2_3: DTCNT Position           */
#define PWM_DTCTL2_3_DTCNT_Msk           (0xffful << PWM_DTCTL2_3_DTCNT_Pos)                /*!< PWM_T::DTCTL2_3: DTCNT Mask               */

#define PWM_DTCTL2_3_DTEN_Pos            (16)                                               /*!< PWM_T::DTCTL2_3: DTEN Position            */
#define PWM_DTCTL2_3_DTEN_Msk            (0x1ul << PWM_DTCTL2_3_DTEN_Pos)                   /*!< PWM_T::DTCTL2_3: DTEN Mask                */

#define PWM_DTCTL2_3_DTCKSEL_Pos         (24)                                               /*!< PWM_T::DTCTL2_3: DTCKSEL Position         */
#define PWM_DTCTL2_3_DTCKSEL_Msk         (0x1ul << PWM_DTCTL2_3_DTCKSEL_Pos)                /*!< PWM_T::DTCTL2_3: DTCKSEL Mask             */

#define PWM_DTCTL4_5_DTCNT_Pos           (0)                                                /*!< PWM_T::DTCTL4_5: DTCNT Position           */
#define PWM_DTCTL4_5_DTCNT_Msk           (0xffful << PWM_DTCTL4_5_DTCNT_Pos)                /*!< PWM_T::DTCTL4_5: DTCNT Mask               */

#define PWM_DTCTL4_5_DTEN_Pos            (16)                                               /*!< PWM_T::DTCTL4_5: DTEN Position            */
#define PWM_DTCTL4_5_DTEN_Msk            (0x1ul << PWM_DTCTL4_5_DTEN_Pos)                   /*!< PWM_T::DTCTL4_5: DTEN Mask                */

#define PWM_DTCTL4_5_DTCKSEL_Pos         (24)                                               /*!< PWM_T::DTCTL4_5: DTCKSEL Position         */
#define PWM_DTCTL4_5_DTCKSEL_Msk         (0x1ul << PWM_DTCTL4_5_DTCKSEL_Pos)                /*!< PWM_T::DTCTL4_5: DTCKSEL Mask             */

#define PWM_CNT_CNT_Pos                  (0)                                                /*!< PWM_T::CNT: CNT Position                  */
#define PWM_CNT_CNT_Msk                  (0xfffful << PWM_CNT_CNT_Pos)                      /*!< PWM_T::CNT: CNT Mask                      */

#define PWM_CNT_DIRF_Pos                 (16)                                               /*!< PWM_T::CNT: DIRF Position                 */
#define PWM_CNT_DIRF_Msk                 (0x1ul << PWM_CNT_DIRF_Pos)                        /*!< PWM_T::CNT: DIRF Mask                     */

#define PWM_WGCTL0_ZPCTLn_Pos            (0)                                                /*!< PWM_T::WGCTL0: ZPCTLn Position            */
#define PWM_WGCTL0_ZPCTLn_Msk            (0xffful << PWM_WGCTL0_ZPCTLn_Pos)                 /*!< PWM_T::WGCTL0: ZPCTLn Mask                */

#define PWM_WGCTL0_ZPCTL0_Pos            (0)                                                /*!< PWM_T::WGCTL0: ZPCTL0 Position            */
#define PWM_WGCTL0_ZPCTL0_Msk            (0x3ul << PWM_WGCTL0_ZPCTL0_Pos)                   /*!< PWM_T::WGCTL0: ZPCTL0 Mask                */

#define PWM_WGCTL0_ZPCTL1_Pos            (2)                                                /*!< PWM_T::WGCTL0: ZPCTL1 Position            */
#define PWM_WGCTL0_ZPCTL1_Msk            (0x3ul << PWM_WGCTL0_ZPCTL1_Pos)                   /*!< PWM_T::WGCTL0: ZPCTL1 Mask                */

#define PWM_WGCTL0_ZPCTL2_Pos            (4)                                                /*!< PWM_T::WGCTL0: ZPCTL2 Position            */
#define PWM_WGCTL0_ZPCTL2_Msk            (0x3ul << PWM_WGCTL0_ZPCTL2_Pos)                   /*!< PWM_T::WGCTL0: ZPCTL2 Mask                */

#define PWM_WGCTL0_ZPCTL3_Pos            (6)                                                /*!< PWM_T::WGCTL0: ZPCTL3 Position            */
#define PWM_WGCTL0_ZPCTL3_Msk            (0x3ul << PWM_WGCTL0_ZPCTL3_Pos)                   /*!< PWM_T::WGCTL0: ZPCTL3 Mask                */

#define PWM_WGCTL0_ZPCTL4_Pos            (8)                                                /*!< PWM_T::WGCTL0: ZPCTL4 Position            */
#define PWM_WGCTL0_ZPCTL4_Msk            (0x3ul << PWM_WGCTL0_ZPCTL4_Pos)                   /*!< PWM_T::WGCTL0: ZPCTL4 Mask                */

#define PWM_WGCTL0_ZPCTL5_Pos            (10)                                               /*!< PWM_T::WGCTL0: ZPCTL5 Position            */
#define PWM_WGCTL0_ZPCTL5_Msk            (0x3ul << PWM_WGCTL0_ZPCTL5_Pos)                   /*!< PWM_T::WGCTL0: ZPCTL5 Mask                */

#define PWM_WGCTL0_PRDPCTLn_Pos          (16)                                               /*!< PWM_T::WGCTL0: PRDPCTLn Position          */
#define PWM_WGCTL0_PRDPCTLn_Msk          (0xffful << PWM_WGCTL0_PRDPCTLn_Pos)               /*!< PWM_T::WGCTL0: PRDPCTLn Mask              */

#define PWM_WGCTL0_PRDPCTL0_Pos          (16)                                               /*!< PWM_T::WGCTL0: PRDPCTL0 Position          */
#define PWM_WGCTL0_PRDPCTL0_Msk          (0x3ul << PWM_WGCTL0_PRDPCTL0_Pos)                 /*!< PWM_T::WGCTL0: PRDPCTL0 Mask              */

#define PWM_WGCTL0_PRDPCTL1_Pos          (18)                                               /*!< PWM_T::WGCTL0: PRDPCTL1 Position          */
#define PWM_WGCTL0_PRDPCTL1_Msk          (0x3ul << PWM_WGCTL0_PRDPCTL1_Pos)                 /*!< PWM_T::WGCTL0: PRDPCTL1 Mask              */

#define PWM_WGCTL0_PRDPCTL2_Pos          (20)                                               /*!< PWM_T::WGCTL0: PRDPCTL2 Position          */
#define PWM_WGCTL0_PRDPCTL2_Msk          (0x3ul << PWM_WGCTL0_PRDPCTL2_Pos)                 /*!< PWM_T::WGCTL0: PRDPCTL2 Mask              */

#define PWM_WGCTL0_PRDPCTL3_Pos          (22)                                               /*!< PWM_T::WGCTL0: PRDPCTL3 Position          */
#define PWM_WGCTL0_PRDPCTL3_Msk          (0x3ul << PWM_WGCTL0_PRDPCTL3_Pos)                 /*!< PWM_T::WGCTL0: PRDPCTL3 Mask              */

#define PWM_WGCTL0_PRDPCTL4_Pos          (24)                                               /*!< PWM_T::WGCTL0: PRDPCTL4 Position          */
#define PWM_WGCTL0_PRDPCTL4_Msk          (0x3ul << PWM_WGCTL0_PRDPCTL4_Pos)                 /*!< PWM_T::WGCTL0: PRDPCTL4 Mask              */

#define PWM_WGCTL0_PRDPCTL5_Pos          (26)                                               /*!< PWM_T::WGCTL0: PRDPCTL5 Position          */
#define PWM_WGCTL0_PRDPCTL5_Msk          (0x3ul << PWM_WGCTL0_PRDPCTL5_Pos)                 /*!< PWM_T::WGCTL0: PRDPCTL5 Mask              */

#define PWM_WGCTL1_CMPUCTLn_Pos          (0)                                                /*!< PWM_T::WGCTL1: CMPUCTLn Position          */
#define PWM_WGCTL1_CMPUCTLn_Msk          (0xffful << PWM_WGCTL1_CMPUCTLn_Pos)               /*!< PWM_T::WGCTL1: CMPUCTLn Mask              */

#define PWM_WGCTL1_CMPUCTL0_Pos          (0)                                                /*!< PWM_T::WGCTL1: CMPUCTL0 Position          */
#define PWM_WGCTL1_CMPUCTL0_Msk          (0x3ul << PWM_WGCTL1_CMPUCTL0_Pos)                 /*!< PWM_T::WGCTL1: CMPUCTL0 Mask              */

#define PWM_WGCTL1_CMPUCTL1_Pos          (2)                                                /*!< PWM_T::WGCTL1: CMPUCTL1 Position          */
#define PWM_WGCTL1_CMPUCTL1_Msk          (0x3ul << PWM_WGCTL1_CMPUCTL1_Pos)                 /*!< PWM_T::WGCTL1: CMPUCTL1 Mask              */

#define PWM_WGCTL1_CMPUCTL2_Pos          (4)                                                /*!< PWM_T::WGCTL1: CMPUCTL2 Position          */
#define PWM_WGCTL1_CMPUCTL2_Msk          (0x3ul << PWM_WGCTL1_CMPUCTL2_Pos)                 /*!< PWM_T::WGCTL1: CMPUCTL2 Mask              */

#define PWM_WGCTL1_CMPUCTL3_Pos          (6)                                                /*!< PWM_T::WGCTL1: CMPUCTL3 Position          */
#define PWM_WGCTL1_CMPUCTL3_Msk          (0x3ul << PWM_WGCTL1_CMPUCTL3_Pos)                 /*!< PWM_T::WGCTL1: CMPUCTL3 Mask              */

#define PWM_WGCTL1_CMPUCTL4_Pos          (8)                                                /*!< PWM_T::WGCTL1: CMPUCTL4 Position          */
#define PWM_WGCTL1_CMPUCTL4_Msk          (0x3ul << PWM_WGCTL1_CMPUCTL4_Pos)                 /*!< PWM_T::WGCTL1: CMPUCTL4 Mask              */

#define PWM_WGCTL1_CMPUCTL5_Pos          (10)                                               /*!< PWM_T::WGCTL1: CMPUCTL5 Position          */
#define PWM_WGCTL1_CMPUCTL5_Msk          (0x3ul << PWM_WGCTL1_CMPUCTL5_Pos)                 /*!< PWM_T::WGCTL1: CMPUCTL5 Mask              */

#define PWM_WGCTL1_CMPDCTLn_Pos          (16)                                               /*!< PWM_T::WGCTL1: CMPDCTLn Position          */
#define PWM_WGCTL1_CMPDCTLn_Msk          (0xffful << PWM_WGCTL1_CMPDCTLn_Pos)               /*!< PWM_T::WGCTL1: CMPDCTLn Mask              */

#define PWM_WGCTL1_CMPDCTL0_Pos          (16)                                               /*!< PWM_T::WGCTL1: CMPDCTL0 Position          */
#define PWM_WGCTL1_CMPDCTL0_Msk          (0x3ul << PWM_WGCTL1_CMPDCTL0_Pos)                 /*!< PWM_T::WGCTL1: CMPDCTL0 Mask              */

#define PWM_WGCTL1_CMPDCTL1_Pos          (18)                                               /*!< PWM_T::WGCTL1: CMPDCTL1 Position          */
#define PWM_WGCTL1_CMPDCTL1_Msk          (0x3ul << PWM_WGCTL1_CMPDCTL1_Pos)                 /*!< PWM_T::WGCTL1: CMPDCTL1 Mask              */

#define PWM_WGCTL1_CMPDCTL2_Pos          (20)                                               /*!< PWM_T::WGCTL1: CMPDCTL2 Position          */
#define PWM_WGCTL1_CMPDCTL2_Msk          (0x3ul << PWM_WGCTL1_CMPDCTL2_Pos)                 /*!< PWM_T::WGCTL1: CMPDCTL2 Mask              */

#define PWM_WGCTL1_CMPDCTL3_Pos          (22)                                               /*!< PWM_T::WGCTL1: CMPDCTL3 Position          */
#define PWM_WGCTL1_CMPDCTL3_Msk          (0x3ul << PWM_WGCTL1_CMPDCTL3_Pos)                 /*!< PWM_T::WGCTL1: CMPDCTL3 Mask              */

#define PWM_WGCTL1_CMPDCTL4_Pos          (24)                                               /*!< PWM_T::WGCTL1: CMPDCTL4 Position          */
#define PWM_WGCTL1_CMPDCTL4_Msk          (0x3ul << PWM_WGCTL1_CMPDCTL4_Pos)                 /*!< PWM_T::WGCTL1: CMPDCTL4 Mask              */

#define PWM_WGCTL1_CMPDCTL5_Pos          (26)                                               /*!< PWM_T::WGCTL1: CMPDCTL5 Position          */
#define PWM_WGCTL1_CMPDCTL5_Msk          (0x3ul << PWM_WGCTL1_CMPDCTL5_Pos)                 /*!< PWM_T::WGCTL1: CMPDCTL5 Mask              */

#define PWM_MSKEN_MSKENn_Pos             (0)                                                /*!< PWM_T::MSKEN: MSKENn Position             */
#define PWM_MSKEN_MSKENn_Msk             (0x3ful << PWM_MSKEN_MSKENn_Pos)                   /*!< PWM_T::MSKEN: MSKENn Mask                 */

#define PWM_MSKEN_MSKEN0_Pos             (0)                                                /*!< PWM_T::MSKEN: MSKEN0 Position             */
#define PWM_MSKEN_MSKEN0_Msk             (0x1ul << PWM_MSKEN_MSKEN0_Pos)                    /*!< PWM_T::MSKEN: MSKEN0 Mask                 */

#define PWM_MSKEN_MSKEN1_Pos             (1)                                                /*!< PWM_T::MSKEN: MSKEN1 Position             */
#define PWM_MSKEN_MSKEN1_Msk             (0x1ul << PWM_MSKEN_MSKEN1_Pos)                    /*!< PWM_T::MSKEN: MSKEN1 Mask                 */

#define PWM_MSKEN_MSKEN2_Pos             (2)                                                /*!< PWM_T::MSKEN: MSKEN2 Position             */
#define PWM_MSKEN_MSKEN2_Msk             (0x1ul << PWM_MSKEN_MSKEN2_Pos)                    /*!< PWM_T::MSKEN: MSKEN2 Mask                 */

#define PWM_MSKEN_MSKEN3_Pos             (3)                                                /*!< PWM_T::MSKEN: MSKEN3 Position             */
#define PWM_MSKEN_MSKEN3_Msk             (0x1ul << PWM_MSKEN_MSKEN3_Pos)                    /*!< PWM_T::MSKEN: MSKEN3 Mask                 */

#define PWM_MSKEN_MSKEN4_Pos             (4)                                                /*!< PWM_T::MSKEN: MSKEN4 Position             */
#define PWM_MSKEN_MSKEN4_Msk             (0x1ul << PWM_MSKEN_MSKEN4_Pos)                    /*!< PWM_T::MSKEN: MSKEN4 Mask                 */

#define PWM_MSKEN_MSKEN5_Pos             (5)                                                /*!< PWM_T::MSKEN: MSKEN5 Position             */
#define PWM_MSKEN_MSKEN5_Msk             (0x1ul << PWM_MSKEN_MSKEN5_Pos)                    /*!< PWM_T::MSKEN: MSKEN5 Mask                 */

#define PWM_MSK_MSKDATn_Pos              (0)                                                /*!< PWM_T::MSK: MSKDATn Position              */
#define PWM_MSK_MSKDATn_Msk              (0x3ful << PWM_MSK_MSKDATn_Pos)                    /*!< PWM_T::MSK: MSKDATn Mask                  */

#define PWM_MSK_MSKDAT0_Pos              (0)                                                /*!< PWM_T::MSK: MSKDAT0 Position              */
#define PWM_MSK_MSKDAT0_Msk              (0x1ul << PWM_MSK_MSKDAT0_Pos)                     /*!< PWM_T::MSK: MSKDAT0 Mask                  */

#define PWM_MSK_MSKDAT1_Pos              (1)                                                /*!< PWM_T::MSK: MSKDAT1 Position              */
#define PWM_MSK_MSKDAT1_Msk              (0x1ul << PWM_MSK_MSKDAT1_Pos)                     /*!< PWM_T::MSK: MSKDAT1 Mask                  */

#define PWM_MSK_MSKDAT2_Pos              (2)                                                /*!< PWM_T::MSK: MSKDAT2 Position              */
#define PWM_MSK_MSKDAT2_Msk              (0x1ul << PWM_MSK_MSKDAT2_Pos)                     /*!< PWM_T::MSK: MSKDAT2 Mask                  */

#define PWM_MSK_MSKDAT3_Pos              (3)                                                /*!< PWM_T::MSK: MSKDAT3 Position              */
#define PWM_MSK_MSKDAT3_Msk              (0x1ul << PWM_MSK_MSKDAT3_Pos)                     /*!< PWM_T::MSK: MSKDAT3 Mask                  */

#define PWM_MSK_MSKDAT4_Pos              (4)                                                /*!< PWM_T::MSK: MSKDAT4 Position              */
#define PWM_MSK_MSKDAT4_Msk              (0x1ul << PWM_MSK_MSKDAT4_Pos)                     /*!< PWM_T::MSK: MSKDAT4 Mask                  */

#define PWM_MSK_MSKDAT5_Pos              (5)                                                /*!< PWM_T::MSK: MSKDAT5 Position              */
#define PWM_MSK_MSKDAT5_Msk              (0x1ul << PWM_MSK_MSKDAT5_Pos)                     /*!< PWM_T::MSK: MSKDAT5 Mask                  */

#define PWM_BNF_BRK0NFEN_Pos             (0)                                                /*!< PWM_T::BNF: BRK0NFEN Position             */
#define PWM_BNF_BRK0NFEN_Msk             (0x1ul << PWM_BNF_BRK0NFEN_Pos)                    /*!< PWM_T::BNF: BRK0NFEN Mask                 */

#define PWM_BNF_BRK0NFSEL_Pos            (1)                                                /*!< PWM_T::BNF: BRK0NFSEL Position            */
#define PWM_BNF_BRK0NFSEL_Msk            (0x7ul << PWM_BNF_BRK0NFSEL_Pos)                   /*!< PWM_T::BNF: BRK0NFSEL Mask                */

#define PWM_BNF_BRK0FCNT_Pos             (4)                                                /*!< PWM_T::BNF: BRK0FCNT Position             */
#define PWM_BNF_BRK0FCNT_Msk             (0x7ul << PWM_BNF_BRK0FCNT_Pos)                    /*!< PWM_T::BNF: BRK0FCNT Mask                 */

#define PWM_BNF_BRK0PINV_Pos             (7)                                                /*!< PWM_T::BNF: BRK0PINV Position             */
#define PWM_BNF_BRK0PINV_Msk             (0x1ul << PWM_BNF_BRK0PINV_Pos)                    /*!< PWM_T::BNF: BRK0PINV Mask                 */

#define PWM_BNF_BRK1NFEN_Pos             (8)                                                /*!< PWM_T::BNF: BRK1NFEN Position             */
#define PWM_BNF_BRK1NFEN_Msk             (0x1ul << PWM_BNF_BRK1NFEN_Pos)                    /*!< PWM_T::BNF: BRK1NFEN Mask                 */

#define PWM_BNF_BRK1NFSEL_Pos            (9)                                                /*!< PWM_T::BNF: BRK1NFSEL Position            */
#define PWM_BNF_BRK1NFSEL_Msk            (0x7ul << PWM_BNF_BRK1NFSEL_Pos)                   /*!< PWM_T::BNF: BRK1NFSEL Mask                */

#define PWM_BNF_BRK1FCNT_Pos             (12)                                               /*!< PWM_T::BNF: BRK1FCNT Position             */
#define PWM_BNF_BRK1FCNT_Msk             (0x7ul << PWM_BNF_BRK1FCNT_Pos)                    /*!< PWM_T::BNF: BRK1FCNT Mask                 */

#define PWM_BNF_BRK1PINV_Pos             (15)                                               /*!< PWM_T::BNF: BRK1PINV Position             */
#define PWM_BNF_BRK1PINV_Msk             (0x1ul << PWM_BNF_BRK1PINV_Pos)                    /*!< PWM_T::BNF: BRK1PINV Mask                 */

#define PWM_BNF_BK0SRC_Pos               (16)                                               /*!< PWM_T::BNF: BK0SRC Position               */
#define PWM_BNF_BK0SRC_Msk               (0x1ul << PWM_BNF_BK0SRC_Pos)                      /*!< PWM_T::BNF: BK0SRC Mask                   */

#define PWM_BNF_BK1SRC_Pos               (24)                                               /*!< PWM_T::BNF: BK1SRC Position               */
#define PWM_BNF_BK1SRC_Msk               (0x1ul << PWM_BNF_BK1SRC_Pos)                      /*!< PWM_T::BNF: BK1SRC Mask                   */

#define PWM_FAILBRK_CSSBRKEN_Pos         (0)                                                /*!< PWM_T::FAILBRK: CSSBRKEN Position         */
#define PWM_FAILBRK_CSSBRKEN_Msk         (0x1ul << PWM_FAILBRK_CSSBRKEN_Pos)                /*!< PWM_T::FAILBRK: CSSBRKEN Mask             */

#define PWM_FAILBRK_BODBRKEN_Pos         (1)                                                /*!< PWM_T::FAILBRK: BODBRKEN Position         */
#define PWM_FAILBRK_BODBRKEN_Msk         (0x1ul << PWM_FAILBRK_BODBRKEN_Pos)                /*!< PWM_T::FAILBRK: BODBRKEN Mask             */

#define PWM_FAILBRK_RAMBRKEN_Pos         (2)                                                /*!< PWM_T::FAILBRK: RAMBRKEN Position         */
#define PWM_FAILBRK_RAMBRKEN_Msk         (0x1ul << PWM_FAILBRK_RAMBRKEN_Pos)                /*!< PWM_T::FAILBRK: RAMBRKEN Mask             */

#define PWM_FAILBRK_CORBRKEN_Pos         (3)                                                /*!< PWM_T::FAILBRK: CORBRKEN Position         */
#define PWM_FAILBRK_CORBRKEN_Msk         (0x1ul << PWM_FAILBRK_CORBRKEN_Pos)                /*!< PWM_T::FAILBRK: CORBRKEN Mask             */

#define PWM_BRKCTL0_1_BRKP0EEN_Pos       (4)                                                /*!< PWM_T::BRKCTL0_1: BRKP0EEN Position       */
#define PWM_BRKCTL0_1_BRKP0EEN_Msk       (0x1ul << PWM_BRKCTL0_1_BRKP0EEN_Pos)              /*!< PWM_T::BRKCTL0_1: BRKP0EEN Mask           */

#define PWM_BRKCTL0_1_BRKP1EEN_Pos       (5)                                                /*!< PWM_T::BRKCTL0_1: BRKP1EEN Position       */
#define PWM_BRKCTL0_1_BRKP1EEN_Msk       (0x1ul << PWM_BRKCTL0_1_BRKP1EEN_Pos)              /*!< PWM_T::BRKCTL0_1: BRKP1EEN Mask           */

#define PWM_BRKCTL0_1_SYSEBEN_Pos        (7)                                                /*!< PWM_T::BRKCTL0_1: SYSEBEN Position        */
#define PWM_BRKCTL0_1_SYSEBEN_Msk        (0x1ul << PWM_BRKCTL0_1_SYSEBEN_Pos)               /*!< PWM_T::BRKCTL0_1: SYSEBEN Mask            */

#define PWM_BRKCTL0_1_BRKP0LEN_Pos       (12)                                               /*!< PWM_T::BRKCTL0_1: BRKP0LEN Position       */
#define PWM_BRKCTL0_1_BRKP0LEN_Msk       (0x1ul << PWM_BRKCTL0_1_BRKP0LEN_Pos)              /*!< PWM_T::BRKCTL0_1: BRKP0LEN Mask           */

#define PWM_BRKCTL0_1_BRKP1LEN_Pos       (13)                                               /*!< PWM_T::BRKCTL0_1: BRKP1LEN Position       */
#define PWM_BRKCTL0_1_BRKP1LEN_Msk       (0x1ul << PWM_BRKCTL0_1_BRKP1LEN_Pos)              /*!< PWM_T::BRKCTL0_1: BRKP1LEN Mask           */

#define PWM_BRKCTL0_1_SYSLBEN_Pos        (15)                                               /*!< PWM_T::BRKCTL0_1: SYSLBEN Position        */
#define PWM_BRKCTL0_1_SYSLBEN_Msk        (0x1ul << PWM_BRKCTL0_1_SYSLBEN_Pos)               /*!< PWM_T::BRKCTL0_1: SYSLBEN Mask            */

#define PWM_BRKCTL0_1_BRKAEVEN_Pos       (16)                                               /*!< PWM_T::BRKCTL0_1: BRKAEVEN Position       */
#define PWM_BRKCTL0_1_BRKAEVEN_Msk       (0x3ul << PWM_BRKCTL0_1_BRKAEVEN_Pos)              /*!< PWM_T::BRKCTL0_1: BRKAEVEN Mask           */

#define PWM_BRKCTL0_1_BRKAODD_Pos        (18)                                               /*!< PWM_T::BRKCTL0_1: BRKAODD Position        */
#define PWM_BRKCTL0_1_BRKAODD_Msk        (0x3ul << PWM_BRKCTL0_1_BRKAODD_Pos)               /*!< PWM_T::BRKCTL0_1: BRKAODD Mask            */

#define PWM_BRKCTL2_3_BRKP0EEN_Pos       (4)                                                /*!< PWM_T::BRKCTL2_3: BRKP0EEN Position       */
#define PWM_BRKCTL2_3_BRKP0EEN_Msk       (0x1ul << PWM_BRKCTL2_3_BRKP0EEN_Pos)              /*!< PWM_T::BRKCTL2_3: BRKP0EEN Mask           */

#define PWM_BRKCTL2_3_BRKP1EEN_Pos       (5)                                                /*!< PWM_T::BRKCTL2_3: BRKP1EEN Position       */
#define PWM_BRKCTL2_3_BRKP1EEN_Msk       (0x1ul << PWM_BRKCTL2_3_BRKP1EEN_Pos)              /*!< PWM_T::BRKCTL2_3: BRKP1EEN Mask           */

#define PWM_BRKCTL2_3_SYSEBEN_Pos        (7)                                                /*!< PWM_T::BRKCTL2_3: SYSEBEN Position        */
#define PWM_BRKCTL2_3_SYSEBEN_Msk        (0x1ul << PWM_BRKCTL2_3_SYSEBEN_Pos)               /*!< PWM_T::BRKCTL2_3: SYSEBEN Mask            */

#define PWM_BRKCTL2_3_BRKP0LEN_Pos       (12)                                               /*!< PWM_T::BRKCTL2_3: BRKP0LEN Position       */
#define PWM_BRKCTL2_3_BRKP0LEN_Msk       (0x1ul << PWM_BRKCTL2_3_BRKP0LEN_Pos)              /*!< PWM_T::BRKCTL2_3: BRKP0LEN Mask           */

#define PWM_BRKCTL2_3_BRKP1LEN_Pos       (13)                                               /*!< PWM_T::BRKCTL2_3: BRKP1LEN Position       */
#define PWM_BRKCTL2_3_BRKP1LEN_Msk       (0x1ul << PWM_BRKCTL2_3_BRKP1LEN_Pos)              /*!< PWM_T::BRKCTL2_3: BRKP1LEN Mask           */

#define PWM_BRKCTL2_3_SYSLBEN_Pos        (15)                                               /*!< PWM_T::BRKCTL2_3: SYSLBEN Position        */
#define PWM_BRKCTL2_3_SYSLBEN_Msk        (0x1ul << PWM_BRKCTL2_3_SYSLBEN_Pos)               /*!< PWM_T::BRKCTL2_3: SYSLBEN Mask            */

#define PWM_BRKCTL2_3_BRKAEVEN_Pos       (16)                                               /*!< PWM_T::BRKCTL2_3: BRKAEVEN Position       */
#define PWM_BRKCTL2_3_BRKAEVEN_Msk       (0x3ul << PWM_BRKCTL2_3_BRKAEVEN_Pos)              /*!< PWM_T::BRKCTL2_3: BRKAEVEN Mask           */

#define PWM_BRKCTL2_3_BRKAODD_Pos        (18)                                               /*!< PWM_T::BRKCTL2_3: BRKAODD Position        */
#define PWM_BRKCTL2_3_BRKAODD_Msk        (0x3ul << PWM_BRKCTL2_3_BRKAODD_Pos)               /*!< PWM_T::BRKCTL2_3: BRKAODD Mask            */

#define PWM_BRKCTL4_5_BRKP0EEN_Pos       (4)                                                /*!< PWM_T::BRKCTL4_5: BRKP0EEN Position       */
#define PWM_BRKCTL4_5_BRKP0EEN_Msk       (0x1ul << PWM_BRKCTL4_5_BRKP0EEN_Pos)              /*!< PWM_T::BRKCTL4_5: BRKP0EEN Mask           */

#define PWM_BRKCTL4_5_BRKP1EEN_Pos       (5)                                                /*!< PWM_T::BRKCTL4_5: BRKP1EEN Position       */
#define PWM_BRKCTL4_5_BRKP1EEN_Msk       (0x1ul << PWM_BRKCTL4_5_BRKP1EEN_Pos)              /*!< PWM_T::BRKCTL4_5: BRKP1EEN Mask           */

#define PWM_BRKCTL4_5_SYSEBEN_Pos        (7)                                                /*!< PWM_T::BRKCTL4_5: SYSEBEN Position        */
#define PWM_BRKCTL4_5_SYSEBEN_Msk        (0x1ul << PWM_BRKCTL4_5_SYSEBEN_Pos)               /*!< PWM_T::BRKCTL4_5: SYSEBEN Mask            */

#define PWM_BRKCTL4_5_BRKP0LEN_Pos       (12)                                               /*!< PWM_T::BRKCTL4_5: BRKP0LEN Position       */
#define PWM_BRKCTL4_5_BRKP0LEN_Msk       (0x1ul << PWM_BRKCTL4_5_BRKP0LEN_Pos)              /*!< PWM_T::BRKCTL4_5: BRKP0LEN Mask           */

#define PWM_BRKCTL4_5_BRKP1LEN_Pos       (13)                                               /*!< PWM_T::BRKCTL4_5: BRKP1LEN Position       */
#define PWM_BRKCTL4_5_BRKP1LEN_Msk       (0x1ul << PWM_BRKCTL4_5_BRKP1LEN_Pos)              /*!< PWM_T::BRKCTL4_5: BRKP1LEN Mask           */

#define PWM_BRKCTL4_5_SYSLBEN_Pos        (15)                                               /*!< PWM_T::BRKCTL4_5: SYSLBEN Position        */
#define PWM_BRKCTL4_5_SYSLBEN_Msk        (0x1ul << PWM_BRKCTL4_5_SYSLBEN_Pos)               /*!< PWM_T::BRKCTL4_5: SYSLBEN Mask            */

#define PWM_BRKCTL4_5_BRKAEVEN_Pos       (16)                                               /*!< PWM_T::BRKCTL4_5: BRKAEVEN Position       */
#define PWM_BRKCTL4_5_BRKAEVEN_Msk       (0x3ul << PWM_BRKCTL4_5_BRKAEVEN_Pos)              /*!< PWM_T::BRKCTL4_5: BRKAEVEN Mask           */

#define PWM_BRKCTL4_5_BRKAODD_Pos        (18)                                               /*!< PWM_T::BRKCTL4_5: BRKAODD Position        */
#define PWM_BRKCTL4_5_BRKAODD_Msk        (0x3ul << PWM_BRKCTL4_5_BRKAODD_Pos)               /*!< PWM_T::BRKCTL4_5: BRKAODD Mask            */

#define PWM_POLCTL_PINVn_Pos             (0)                                                /*!< PWM_T::POLCTL: PINVn Position             */
#define PWM_POLCTL_PINVn_Msk             (0x3ful << PWM_POLCTL_PINVn_Pos)                   /*!< PWM_T::POLCTL: PINVn Mask                 */

#define PWM_POLCTL_PINV0_Pos             (0)                                                /*!< PWM_T::POLCTL: PINV0 Position             */
#define PWM_POLCTL_PINV0_Msk             (0x1ul << PWM_POLCTL_PINV0_Pos)                    /*!< PWM_T::POLCTL: PINV0 Mask                 */

#define PWM_POLCTL_PINV1_Pos             (1)                                                /*!< PWM_T::POLCTL: PINV1 Position             */
#define PWM_POLCTL_PINV1_Msk             (0x1ul << PWM_POLCTL_PINV1_Pos)                    /*!< PWM_T::POLCTL: PINV1 Mask                 */

#define PWM_POLCTL_PINV2_Pos             (2)                                                /*!< PWM_T::POLCTL: PINV2 Position             */
#define PWM_POLCTL_PINV2_Msk             (0x1ul << PWM_POLCTL_PINV2_Pos)                    /*!< PWM_T::POLCTL: PINV2 Mask                 */

#define PWM_POLCTL_PINV3_Pos             (3)                                                /*!< PWM_T::POLCTL: PINV3 Position             */
#define PWM_POLCTL_PINV3_Msk             (0x1ul << PWM_POLCTL_PINV3_Pos)                    /*!< PWM_T::POLCTL: PINV3 Mask                 */

#define PWM_POLCTL_PINV4_Pos             (4)                                                /*!< PWM_T::POLCTL: PINV4 Position             */
#define PWM_POLCTL_PINV4_Msk             (0x1ul << PWM_POLCTL_PINV4_Pos)                    /*!< PWM_T::POLCTL: PINV4 Mask                 */

#define PWM_POLCTL_PINV5_Pos             (5)                                                /*!< PWM_T::POLCTL: PINV5 Position             */
#define PWM_POLCTL_PINV5_Msk             (0x1ul << PWM_POLCTL_PINV5_Pos)                    /*!< PWM_T::POLCTL: PINV5 Mask                 */

#define PWM_POEN_POENn_Pos               (0)                                                /*!< PWM_T::POEN: POENn Position               */
#define PWM_POEN_POENn_Msk               (0x3ful << PWM_POEN_POENn_Pos)                     /*!< PWM_T::POEN: POENn Mask                   */

#define PWM_POEN_POEN0_Pos               (0)                                                /*!< PWM_T::POEN: POEN0 Position               */
#define PWM_POEN_POEN0_Msk               (0x1ul << PWM_POEN_POEN0_Pos)                      /*!< PWM_T::POEN: POEN0 Mask                   */

#define PWM_POEN_POEN1_Pos               (1)                                                /*!< PWM_T::POEN: POEN1 Position               */
#define PWM_POEN_POEN1_Msk               (0x1ul << PWM_POEN_POEN1_Pos)                      /*!< PWM_T::POEN: POEN1 Mask                   */

#define PWM_POEN_POEN2_Pos               (2)                                                /*!< PWM_T::POEN: POEN2 Position               */
#define PWM_POEN_POEN2_Msk               (0x1ul << PWM_POEN_POEN2_Pos)                      /*!< PWM_T::POEN: POEN2 Mask                   */

#define PWM_POEN_POEN3_Pos               (3)                                                /*!< PWM_T::POEN: POEN3 Position               */
#define PWM_POEN_POEN3_Msk               (0x1ul << PWM_POEN_POEN3_Pos)                      /*!< PWM_T::POEN: POEN3 Mask                   */

#define PWM_POEN_POEN4_Pos               (4)                                                /*!< PWM_T::POEN: POEN4 Position               */
#define PWM_POEN_POEN4_Msk               (0x1ul << PWM_POEN_POEN4_Pos)                      /*!< PWM_T::POEN: POEN4 Mask                   */

#define PWM_POEN_POEN5_Pos               (5)                                                /*!< PWM_T::POEN: POEN5 Position               */
#define PWM_POEN_POEN5_Msk               (0x1ul << PWM_POEN_POEN5_Pos)                      /*!< PWM_T::POEN: POEN5 Mask                   */

#define PWM_SWBRK_BRKETRGn_Pos           (0)                                                /*!< PWM_T::SWBRK: BRKETRGn Position           */
#define PWM_SWBRK_BRKETRGn_Msk           (0x7ul << PWM_SWBRK_BRKETRGn_Pos)                  /*!< PWM_T::SWBRK: BRKETRGn Mask               */

#define PWM_SWBRK_BRKETRG0_Pos           (0)                                                /*!< PWM_T::SWBRK: BRKETRG0 Position           */
#define PWM_SWBRK_BRKETRG0_Msk           (0x1ul << PWM_SWBRK_BRKETRG0_Pos)                  /*!< PWM_T::SWBRK: BRKETRG0 Mask               */

#define PWM_SWBRK_BRKETRG2_Pos           (1)                                                /*!< PWM_T::SWBRK: BRKETRG2 Position           */
#define PWM_SWBRK_BRKETRG2_Msk           (0x1ul << PWM_SWBRK_BRKETRG2_Pos)                  /*!< PWM_T::SWBRK: BRKETRG2 Mask               */

#define PWM_SWBRK_BRKETRG4_Pos           (2)                                                /*!< PWM_T::SWBRK: BRKETRG4 Position           */
#define PWM_SWBRK_BRKETRG4_Msk           (0x1ul << PWM_SWBRK_BRKETRG4_Pos)                  /*!< PWM_T::SWBRK: BRKETRG4 Mask               */

#define PWM_SWBRK_BRKLTRGn_Pos           (8)                                                /*!< PWM_T::SWBRK: BRKLTRGn Position           */
#define PWM_SWBRK_BRKLTRGn_Msk           (0x7ul << PWM_SWBRK_BRKLTRGn_Pos)                  /*!< PWM_T::SWBRK: BRKLTRGn Mask               */

#define PWM_SWBRK_BRKLTRG0_Pos           (8)                                                /*!< PWM_T::SWBRK: BRKLTRG0 Position           */
#define PWM_SWBRK_BRKLTRG0_Msk           (0x1ul << PWM_SWBRK_BRKLTRG0_Pos)                  /*!< PWM_T::SWBRK: BRKLTRG0 Mask               */

#define PWM_SWBRK_BRKLTRG2_Pos           (9)                                                /*!< PWM_T::SWBRK: BRKLTRG2 Position           */
#define PWM_SWBRK_BRKLTRG2_Msk           (0x1ul << PWM_SWBRK_BRKLTRG2_Pos)                  /*!< PWM_T::SWBRK: BRKLTRG2 Mask               */

#define PWM_SWBRK_BRKLTRG4_Pos           (10)                                               /*!< PWM_T::SWBRK: BRKLTRG4 Position           */
#define PWM_SWBRK_BRKLTRG4_Msk           (0x1ul << PWM_SWBRK_BRKLTRG4_Pos)                  /*!< PWM_T::SWBRK: BRKLTRG4 Mask               */

#define PWM_INTEN0_ZIENn_Pos             (0)                                                /*!< PWM_T::INTEN0: ZIENn Position             */
#define PWM_INTEN0_ZIENn_Msk             (0x15ul << PWM_INTEN0_ZIENn_Pos)                   /*!< PWM_T::INTEN0: ZIENn Mask                 */

#define PWM_INTEN0_ZIEN0_Pos             (0)                                                /*!< PWM_T::INTEN0: ZIEN0 Position             */
#define PWM_INTEN0_ZIEN0_Msk             (0x1ul << PWM_INTEN0_ZIEN0_Pos)                    /*!< PWM_T::INTEN0: ZIEN0 Mask                 */

#define PWM_INTEN0_ZIEN2_Pos             (2)                                                /*!< PWM_T::INTEN0: ZIEN2 Position             */
#define PWM_INTEN0_ZIEN2_Msk             (0x1ul << PWM_INTEN0_ZIEN2_Pos)                    /*!< PWM_T::INTEN0: ZIEN2 Mask                 */

#define PWM_INTEN0_ZIEN4_Pos             (4)                                                /*!< PWM_T::INTEN0: ZIEN4 Position             */
#define PWM_INTEN0_ZIEN4_Msk             (0x1ul << PWM_INTEN0_ZIEN4_Pos)                    /*!< PWM_T::INTEN0: ZIEN4 Mask                 */

#define PWM_INTEN0_PIENn_Pos             (8)                                                /*!< PWM_T::INTEN0: PIENn Position             */
#define PWM_INTEN0_PIENn_Msk             (0x15ul << PWM_INTEN0_PIENn_Pos)                   /*!< PWM_T::INTEN0: PIENn Mask                 */

#define PWM_INTEN0_PIEN0_Pos             (8)                                                /*!< PWM_T::INTEN0: PIEN0 Position             */
#define PWM_INTEN0_PIEN0_Msk             (0x1ul << PWM_INTEN0_PIEN0_Pos)                    /*!< PWM_T::INTEN0: PIEN0 Mask                 */

#define PWM_INTEN0_PIEN2_Pos             (10)                                               /*!< PWM_T::INTEN0: PIEN2 Position             */
#define PWM_INTEN0_PIEN2_Msk             (0x1ul << PWM_INTEN0_PIEN2_Pos)                    /*!< PWM_T::INTEN0: PIEN2 Mask                 */

#define PWM_INTEN0_PIEN4_Pos             (12)                                               /*!< PWM_T::INTEN0: PIEN4 Position             */
#define PWM_INTEN0_PIEN4_Msk             (0x1ul << PWM_INTEN0_PIEN4_Pos)                    /*!< PWM_T::INTEN0: PIEN4 Mask                 */

#define PWM_INTEN0_CMPUIENn_Pos          (16)                                               /*!< PWM_T::INTEN0: CMPUIENn Position          */
#define PWM_INTEN0_CMPUIENn_Msk          (0x3ful << PWM_INTEN0_CMPUIENn_Pos)                /*!< PWM_T::INTEN0: CMPUIENn Mask              */

#define PWM_INTEN0_CMPUIEN0_Pos          (16)                                               /*!< PWM_T::INTEN0: CMPUIEN0 Position          */
#define PWM_INTEN0_CMPUIEN0_Msk          (0x1ul << PWM_INTEN0_CMPUIEN0_Pos)                 /*!< PWM_T::INTEN0: CMPUIEN0 Mask              */

#define PWM_INTEN0_CMPUIEN1_Pos          (17)                                               /*!< PWM_T::INTEN0: CMPUIEN1 Position          */
#define PWM_INTEN0_CMPUIEN1_Msk          (0x1ul << PWM_INTEN0_CMPUIEN1_Pos)                 /*!< PWM_T::INTEN0: CMPUIEN1 Mask              */

#define PWM_INTEN0_CMPUIEN2_Pos          (18)                                               /*!< PWM_T::INTEN0: CMPUIEN2 Position          */
#define PWM_INTEN0_CMPUIEN2_Msk          (0x1ul << PWM_INTEN0_CMPUIEN2_Pos)                 /*!< PWM_T::INTEN0: CMPUIEN2 Mask              */

#define PWM_INTEN0_CMPUIEN3_Pos          (19)                                               /*!< PWM_T::INTEN0: CMPUIEN3 Position          */
#define PWM_INTEN0_CMPUIEN3_Msk          (0x1ul << PWM_INTEN0_CMPUIEN3_Pos)                 /*!< PWM_T::INTEN0: CMPUIEN3 Mask              */

#define PWM_INTEN0_CMPUIEN4_Pos          (20)                                               /*!< PWM_T::INTEN0: CMPUIEN4 Position          */
#define PWM_INTEN0_CMPUIEN4_Msk          (0x1ul << PWM_INTEN0_CMPUIEN4_Pos)                 /*!< PWM_T::INTEN0: CMPUIEN4 Mask              */

#define PWM_INTEN0_CMPUIEN5_Pos          (21)                                               /*!< PWM_T::INTEN0: CMPUIEN5 Position          */
#define PWM_INTEN0_CMPUIEN5_Msk          (0x1ul << PWM_INTEN0_CMPUIEN5_Pos)                 /*!< PWM_T::INTEN0: CMPUIEN5 Mask              */

#define PWM_INTEN0_IFAIEN4_5_Pos         (23)                                               /*!< PWM_T::INTEN0: IFAIEN4_5 Position         */
#define PWM_INTEN0_IFAIEN4_5_Msk         (0x1ul << PWM_INTEN0_IFAIEN4_5_Pos)                /*!< PWM_T::INTEN0: IFAIEN4_5 Mask             */

#define PWM_INTEN0_CMPDIENn_Pos          (24)                                               /*!< PWM_T::INTEN0: CMPDIENn Position          */
#define PWM_INTEN0_CMPDIENn_Msk          (0x3ful << PWM_INTEN0_CMPDIENn_Pos)                /*!< PWM_T::INTEN0: CMPDIENn Mask              */

#define PWM_INTEN0_CMPDIEN0_Pos          (24)                                               /*!< PWM_T::INTEN0: CMPDIEN0 Position          */
#define PWM_INTEN0_CMPDIEN0_Msk          (0x1ul << PWM_INTEN0_CMPDIEN0_Pos)                 /*!< PWM_T::INTEN0: CMPDIEN0 Mask              */

#define PWM_INTEN0_CMPDIEN1_Pos          (25)                                               /*!< PWM_T::INTEN0: CMPDIEN1 Position          */
#define PWM_INTEN0_CMPDIEN1_Msk          (0x1ul << PWM_INTEN0_CMPDIEN1_Pos)                 /*!< PWM_T::INTEN0: CMPDIEN1 Mask              */

#define PWM_INTEN0_CMPDIEN2_Pos          (26)                                               /*!< PWM_T::INTEN0: CMPDIEN2 Position          */
#define PWM_INTEN0_CMPDIEN2_Msk          (0x1ul << PWM_INTEN0_CMPDIEN2_Pos)                 /*!< PWM_T::INTEN0: CMPDIEN2 Mask              */

#define PWM_INTEN0_CMPDIEN3_Pos          (27)                                               /*!< PWM_T::INTEN0: CMPDIEN3 Position          */
#define PWM_INTEN0_CMPDIEN3_Msk          (0x1ul << PWM_INTEN0_CMPDIEN3_Pos)                 /*!< PWM_T::INTEN0: CMPDIEN3 Mask              */

#define PWM_INTEN0_CMPDIEN4_Pos          (28)                                               /*!< PWM_T::INTEN0: CMPDIEN4 Position          */
#define PWM_INTEN0_CMPDIEN4_Msk          (0x1ul << PWM_INTEN0_CMPDIEN4_Pos)                 /*!< PWM_T::INTEN0: CMPDIEN4 Mask              */

#define PWM_INTEN0_CMPDIEN5_Pos          (29)                                               /*!< PWM_T::INTEN0: CMPDIEN5 Position          */
#define PWM_INTEN0_CMPDIEN5_Msk          (0x1ul << PWM_INTEN0_CMPDIEN5_Pos)                 /*!< PWM_T::INTEN0: CMPDIEN5 Mask              */

#define PWM_INTEN1_BRKEIEN0_1_Pos        (0)                                                /*!< PWM_T::INTEN1: BRKEIEN0_1 Position        */
#define PWM_INTEN1_BRKEIEN0_1_Msk        (0x1ul << PWM_INTEN1_BRKEIEN0_1_Pos)               /*!< PWM_T::INTEN1: BRKEIEN0_1 Mask            */

#define PWM_INTEN1_BRKEIEN2_3_Pos        (1)                                                /*!< PWM_T::INTEN1: BRKEIEN2_3 Position        */
#define PWM_INTEN1_BRKEIEN2_3_Msk        (0x1ul << PWM_INTEN1_BRKEIEN2_3_Pos)               /*!< PWM_T::INTEN1: BRKEIEN2_3 Mask            */

#define PWM_INTEN1_BRKEIEN4_5_Pos        (2)                                                /*!< PWM_T::INTEN1: BRKEIEN4_5 Position        */
#define PWM_INTEN1_BRKEIEN4_5_Msk        (0x1ul << PWM_INTEN1_BRKEIEN4_5_Pos)               /*!< PWM_T::INTEN1: BRKEIEN4_5 Mask            */

#define PWM_INTEN1_BRKLIEN0_1_Pos        (8)                                                /*!< PWM_T::INTEN1: BRKLIEN0_1 Position        */
#define PWM_INTEN1_BRKLIEN0_1_Msk        (0x1ul << PWM_INTEN1_BRKLIEN0_1_Pos)               /*!< PWM_T::INTEN1: BRKLIEN0_1 Mask            */

#define PWM_INTEN1_BRKLIEN2_3_Pos        (9)                                                /*!< PWM_T::INTEN1: BRKLIEN2_3 Position        */
#define PWM_INTEN1_BRKLIEN2_3_Msk        (0x1ul << PWM_INTEN1_BRKLIEN2_3_Pos)               /*!< PWM_T::INTEN1: BRKLIEN2_3 Mask            */

#define PWM_INTEN1_BRKLIEN4_5_Pos        (10)                                               /*!< PWM_T::INTEN1: BRKLIEN4_5 Position        */
#define PWM_INTEN1_BRKLIEN4_5_Msk        (0x1ul << PWM_INTEN1_BRKLIEN4_5_Pos)               /*!< PWM_T::INTEN1: BRKLIEN4_5 Mask            */

#define PWM_INTSTS0_ZIFn_Pos             (0)                                                /*!< PWM_T::INTSTS0: ZIFn Position             */
#define PWM_INTSTS0_ZIFn_Msk             (0x15ul << PWM_INTSTS0_ZIFn_Pos)                   /*!< PWM_T::INTSTS0: ZIFn Mask                 */

#define PWM_INTSTS0_ZIF0_Pos             (0)                                                /*!< PWM_T::INTSTS0: ZIF0 Position             */
#define PWM_INTSTS0_ZIF0_Msk             (0x1ul << PWM_INTSTS0_ZIF0_Pos)                    /*!< PWM_T::INTSTS0: ZIF0 Mask                 */

#define PWM_INTSTS0_ZIF2_Pos             (2)                                                /*!< PWM_T::INTSTS0: ZIF2 Position             */
#define PWM_INTSTS0_ZIF2_Msk             (0x1ul << PWM_INTSTS0_ZIF2_Pos)                    /*!< PWM_T::INTSTS0: ZIF2 Mask                 */

#define PWM_INTSTS0_ZIF4_Pos             (4)                                                /*!< PWM_T::INTSTS0: ZIF4 Position             */
#define PWM_INTSTS0_ZIF4_Msk             (0x1ul << PWM_INTSTS0_ZIF4_Pos)                    /*!< PWM_T::INTSTS0: ZIF4 Mask                 */

#define PWM_INTSTS0_PIFn_Pos             (8)                                                /*!< PWM_T::INTSTS0: PIFn Position             */
#define PWM_INTSTS0_PIFn_Msk             (0x15ul << PWM_INTSTS0_PIFn_Pos)                   /*!< PWM_T::INTSTS0: PIFn Mask                 */

#define PWM_INTSTS0_PIF0_Pos             (8)                                                /*!< PWM_T::INTSTS0: PIF0 Position             */
#define PWM_INTSTS0_PIF0_Msk             (0x1ul << PWM_INTSTS0_PIF0_Pos)                    /*!< PWM_T::INTSTS0: PIF0 Mask                 */

#define PWM_INTSTS0_PIF2_Pos             (10)                                               /*!< PWM_T::INTSTS0: PIF2 Position             */
#define PWM_INTSTS0_PIF2_Msk             (0x1ul << PWM_INTSTS0_PIF2_Pos)                    /*!< PWM_T::INTSTS0: PIF2 Mask                 */

#define PWM_INTSTS0_PIF4_Pos             (12)                                               /*!< PWM_T::INTSTS0: PIF4 Position             */
#define PWM_INTSTS0_PIF4_Msk             (0x1ul << PWM_INTSTS0_PIF4_Pos)                    /*!< PWM_T::INTSTS0: ZIF4 Mask                 */

#define PWM_INTSTS0_CMPUIFn_Pos          (16)                                               /*!< PWM_T::INTSTS0: CMPUIFn Position          */
#define PWM_INTSTS0_CMPUIFn_Msk          (0x3ful << PWM_INTSTS0_CMPUIFn_Pos)                /*!< PWM_T::INTSTS0: CMPUIFn Mask              */

#define PWM_INTSTS0_CMPUIF0_Pos          (16)                                               /*!< PWM_T::INTSTS0: CMPUIF0 Position          */
#define PWM_INTSTS0_CMPUIF0_Msk          (0x1ul << PWM_INTSTS0_CMPUIF0_Pos)                 /*!< PWM_T::INTSTS0: CMPUIF0 Mask              */

#define PWM_INTSTS0_CMPUIF1_Pos          (17)                                               /*!< PWM_T::INTSTS0: CMPUIF1 Position          */
#define PWM_INTSTS0_CMPUIF1_Msk          (0x1ul << PWM_INTSTS0_CMPUIF1_Pos)                 /*!< PWM_T::INTSTS0: CMPUIF1 Mask              */

#define PWM_INTSTS0_CMPUIF2_Pos          (18)                                               /*!< PWM_T::INTSTS0: CMPUIF2 Position          */
#define PWM_INTSTS0_CMPUIF2_Msk          (0x1ul << PWM_INTSTS0_CMPUIF2_Pos)                 /*!< PWM_T::INTSTS0: CMPUIF2 Mask              */

#define PWM_INTSTS0_CMPUIF3_Pos          (19)                                               /*!< PWM_T::INTSTS0: CMPUIF3 Position          */
#define PWM_INTSTS0_CMPUIF3_Msk          (0x1ul << PWM_INTSTS0_CMPUIF3_Pos)                 /*!< PWM_T::INTSTS0: CMPUIF3 Mask              */

#define PWM_INTSTS0_CMPUIF4_Pos          (20)                                               /*!< PWM_T::INTSTS0: CMPUIF4 Position          */
#define PWM_INTSTS0_CMPUIF4_Msk          (0x1ul << PWM_INTSTS0_CMPUIF4_Pos)                 /*!< PWM_T::INTSTS0: CMPUIF4 Mask              */

#define PWM_INTSTS0_CMPUIF5_Pos          (21)                                               /*!< PWM_T::INTSTS0: CMPUIF5 Position          */
#define PWM_INTSTS0_CMPUIF5_Msk          (0x1ul << PWM_INTSTS0_CMPUIF5_Pos)                 /*!< PWM_T::INTSTS0: CMPUIF5 Mask              */

#define PWM_INTSTS0_IFAIF4_5_Pos         (23)                                               /*!< PWM_T::INTSTS0: IFAIF4_5 Position         */
#define PWM_INTSTS0_IFAIF4_5_Msk         (0x1ul << PWM_INTSTS0_IFAIF4_5_Pos)                /*!< PWM_T::INTSTS0: IFAIF4_5 Mask             */

#define PWM_INTSTS0_CMPDIFn_Pos          (24)                                               /*!< PWM_T::INTSTS0: CMPDIFn Position          */
#define PWM_INTSTS0_CMPDIFn_Msk          (0x3ful << PWM_INTSTS0_CMPDIFn_Pos)                /*!< PWM_T::INTSTS0: CMPDIFn Mask              */

#define PWM_INTSTS0_CMPDIF0_Pos          (24)                                               /*!< PWM_T::INTSTS0: CMPDIF0 Position          */
#define PWM_INTSTS0_CMPDIF0_Msk          (0x1ul << PWM_INTSTS0_CMPDIF0_Pos)                 /*!< PWM_T::INTSTS0: CMPDIF0 Mask              */

#define PWM_INTSTS0_CMPDIF1_Pos          (25)                                               /*!< PWM_T::INTSTS0: CMPDIF1 Position          */
#define PWM_INTSTS0_CMPDIF1_Msk          (0x1ul << PWM_INTSTS0_CMPDIF1_Pos)                 /*!< PWM_T::INTSTS0: CMPDIF1 Mask              */

#define PWM_INTSTS0_CMPDIF2_Pos          (26)                                               /*!< PWM_T::INTSTS0: CMPDIF2 Position          */
#define PWM_INTSTS0_CMPDIF2_Msk          (0x1ul << PWM_INTSTS0_CMPDIF2_Pos)                 /*!< PWM_T::INTSTS0: CMPDIF2 Mask              */

#define PWM_INTSTS0_CMPDIF3_Pos          (27)                                               /*!< PWM_T::INTSTS0: CMPDIF3 Position          */
#define PWM_INTSTS0_CMPDIF3_Msk          (0x1ul << PWM_INTSTS0_CMPDIF3_Pos)                 /*!< PWM_T::INTSTS0: CMPDIF3 Mask              */

#define PWM_INTSTS0_CMPDIF4_Pos          (28)                                               /*!< PWM_T::INTSTS0: CMPDIF4 Position          */
#define PWM_INTSTS0_CMPDIF4_Msk          (0x1ul << PWM_INTSTS0_CMPDIF4_Pos)                 /*!< PWM_T::INTSTS0: CMPDIF4 Mask              */

#define PWM_INTSTS0_CMPDIF5_Pos          (29)                                               /*!< PWM_T::INTSTS0: CMPDIF5 Position          */
#define PWM_INTSTS0_CMPDIF5_Msk          (0x1ul << PWM_INTSTS0_CMPDIF5_Pos)                 /*!< PWM_T::INTSTS0: CMPDIF5 Mask              */

#define PWM_INTSTS1_BRKEIFn_Pos          (0)                                                /*!< PWM_T::INTSTS1: BRKEIFn Position          */
#define PWM_INTSTS1_BRKEIFn_Msk          (0x3ful << PWM_INTSTS1_BRKEIFn_Pos)                /*!< PWM_T::INTSTS1: BRKEIFn Mask              */

#define PWM_INTSTS1_BRKEIF0_Pos          (0)                                                /*!< PWM_T::INTSTS1: BRKEIF0 Position          */
#define PWM_INTSTS1_BRKEIF0_Msk          (0x1ul << PWM_INTSTS1_BRKEIF0_Pos)                 /*!< PWM_T::INTSTS1: BRKEIF0 Mask              */

#define PWM_INTSTS1_BRKEIF1_Pos          (1)                                                /*!< PWM_T::INTSTS1: BRKEIF1 Position          */
#define PWM_INTSTS1_BRKEIF1_Msk          (0x1ul << PWM_INTSTS1_BRKEIF1_Pos)                 /*!< PWM_T::INTSTS1: BRKEIF1 Mask              */

#define PWM_INTSTS1_BRKEIF2_Pos          (2)                                                /*!< PWM_T::INTSTS1: BRKEIF2 Position          */
#define PWM_INTSTS1_BRKEIF2_Msk          (0x1ul << PWM_INTSTS1_BRKEIF2_Pos)                 /*!< PWM_T::INTSTS1: BRKEIF2 Mask              */

#define PWM_INTSTS1_BRKEIF3_Pos          (3)                                                /*!< PWM_T::INTSTS1: BRKEIF3 Position          */
#define PWM_INTSTS1_BRKEIF3_Msk          (0x1ul << PWM_INTSTS1_BRKEIF3_Pos)                 /*!< PWM_T::INTSTS1: BRKEIF3 Mask              */

#define PWM_INTSTS1_BRKEIF4_Pos          (4)                                                /*!< PWM_T::INTSTS1: BRKEIF4 Position          */
#define PWM_INTSTS1_BRKEIF4_Msk          (0x1ul << PWM_INTSTS1_BRKEIF4_Pos)                 /*!< PWM_T::INTSTS1: BRKEIF4 Mask              */

#define PWM_INTSTS1_BRKEIF5_Pos          (5)                                                /*!< PWM_T::INTSTS1: BRKEIF5 Position          */
#define PWM_INTSTS1_BRKEIF5_Msk          (0x1ul << PWM_INTSTS1_BRKEIF5_Pos)                 /*!< PWM_T::INTSTS1: BRKEIF5 Mask              */

#define PWM_INTSTS1_BRKLIFn_Pos          (8)                                                /*!< PWM_T::INTSTS1: BRKLIFn Position          */
#define PWM_INTSTS1_BRKLIFn_Msk          (0x3ful << PWM_INTSTS1_BRKLIFn_Pos)                /*!< PWM_T::INTSTS1: BRKLIFn Mask              */

#define PWM_INTSTS1_BRKLIF0_Pos          (8)                                                /*!< PWM_T::INTSTS1: BRKLIF0 Position          */
#define PWM_INTSTS1_BRKLIF0_Msk          (0x1ul << PWM_INTSTS1_BRKLIF0_Pos)                 /*!< PWM_T::INTSTS1: BRKLIF0 Mask              */

#define PWM_INTSTS1_BRKLIF1_Pos          (9)                                                /*!< PWM_T::INTSTS1: BRKLIF1 Position          */
#define PWM_INTSTS1_BRKLIF1_Msk          (0x1ul << PWM_INTSTS1_BRKLIF1_Pos)                 /*!< PWM_T::INTSTS1: BRKLIF1 Mask              */

#define PWM_INTSTS1_BRKLIF2_Pos          (10)                                               /*!< PWM_T::INTSTS1: BRKLIF2 Position          */
#define PWM_INTSTS1_BRKLIF2_Msk          (0x1ul << PWM_INTSTS1_BRKLIF2_Pos)                 /*!< PWM_T::INTSTS1: BRKLIF2 Mask              */

#define PWM_INTSTS1_BRKLIF3_Pos          (11)                                               /*!< PWM_T::INTSTS1: BRKLIF3 Position          */
#define PWM_INTSTS1_BRKLIF3_Msk          (0x1ul << PWM_INTSTS1_BRKLIF3_Pos)                 /*!< PWM_T::INTSTS1: BRKLIF3 Mask              */

#define PWM_INTSTS1_BRKLIF4_Pos          (12)                                               /*!< PWM_T::INTSTS1: BRKLIF4 Position          */
#define PWM_INTSTS1_BRKLIF4_Msk          (0x1ul << PWM_INTSTS1_BRKLIF4_Pos)                 /*!< PWM_T::INTSTS1: BRKLIF4 Mask              */

#define PWM_INTSTS1_BRKLIF5_Pos          (13)                                               /*!< PWM_T::INTSTS1: BRKLIF5 Position          */
#define PWM_INTSTS1_BRKLIF5_Msk          (0x1ul << PWM_INTSTS1_BRKLIF5_Pos)                 /*!< PWM_T::INTSTS1: BRKLIF5 Mask              */

#define PWM_INTSTS1_BRKESTS0_Pos         (16)                                               /*!< PWM_T::INTSTS1: BRKESTS0 Position         */
#define PWM_INTSTS1_BRKESTS0_Msk         (0x1ul << PWM_INTSTS1_BRKESTS0_Pos)                /*!< PWM_T::INTSTS1: BRKESTS0 Mask             */

#define PWM_INTSTS1_BRKESTS1_Pos         (17)                                               /*!< PWM_T::INTSTS1: BRKESTS1 Position         */
#define PWM_INTSTS1_BRKESTS1_Msk         (0x1ul << PWM_INTSTS1_BRKESTS1_Pos)                /*!< PWM_T::INTSTS1: BRKESTS1 Mask             */

#define PWM_INTSTS1_BRKESTS2_Pos         (18)                                               /*!< PWM_T::INTSTS1: BRKESTS2 Position         */
#define PWM_INTSTS1_BRKESTS2_Msk         (0x1ul << PWM_INTSTS1_BRKESTS2_Pos)                /*!< PWM_T::INTSTS1: BRKESTS2 Mask             */

#define PWM_INTSTS1_BRKESTS3_Pos         (19)                                               /*!< PWM_T::INTSTS1: BRKESTS3 Position         */
#define PWM_INTSTS1_BRKESTS3_Msk         (0x1ul << PWM_INTSTS1_BRKESTS3_Pos)                /*!< PWM_T::INTSTS1: BRKESTS3 Mask             */

#define PWM_INTSTS1_BRKESTS4_Pos         (20)                                               /*!< PWM_T::INTSTS1: BRKESTS4 Position         */
#define PWM_INTSTS1_BRKESTS4_Msk         (0x1ul << PWM_INTSTS1_BRKESTS4_Pos)                /*!< PWM_T::INTSTS1: BRKESTS4 Mask             */

#define PWM_INTSTS1_BRKESTS5_Pos         (21)                                               /*!< PWM_T::INTSTS1: BRKESTS5 Position         */
#define PWM_INTSTS1_BRKESTS5_Msk         (0x1ul << PWM_INTSTS1_BRKESTS5_Pos)                /*!< PWM_T::INTSTS1: BRKESTS5 Mask             */

#define PWM_INTSTS1_BRKLSTS0_Pos         (24)                                               /*!< PWM_T::INTSTS1: BRKLSTS0 Position         */
#define PWM_INTSTS1_BRKLSTS0_Msk         (0x1ul << PWM_INTSTS1_BRKLSTS0_Pos)                /*!< PWM_T::INTSTS1: BRKLSTS0 Mask             */

#define PWM_INTSTS1_BRKLSTS1_Pos         (25)                                               /*!< PWM_T::INTSTS1: BRKLSTS1 Position         */
#define PWM_INTSTS1_BRKLSTS1_Msk         (0x1ul << PWM_INTSTS1_BRKLSTS1_Pos)                /*!< PWM_T::INTSTS1: BRKLSTS1 Mask             */

#define PWM_INTSTS1_BRKLSTS2_Pos         (26)                                               /*!< PWM_T::INTSTS1: BRKLSTS2 Position         */
#define PWM_INTSTS1_BRKLSTS2_Msk         (0x1ul << PWM_INTSTS1_BRKLSTS2_Pos)                /*!< PWM_T::INTSTS1: BRKLSTS2 Mask             */

#define PWM_INTSTS1_BRKLSTS3_Pos         (27)                                               /*!< PWM_T::INTSTS1: BRKLSTS3 Position         */
#define PWM_INTSTS1_BRKLSTS3_Msk         (0x1ul << PWM_INTSTS1_BRKLSTS3_Pos)                /*!< PWM_T::INTSTS1: BRKLSTS3 Mask             */

#define PWM_INTSTS1_BRKLSTS4_Pos         (28)                                               /*!< PWM_T::INTSTS1: BRKLSTS4 Position         */
#define PWM_INTSTS1_BRKLSTS4_Msk         (0x1ul << PWM_INTSTS1_BRKLSTS4_Pos)                /*!< PWM_T::INTSTS1: BRKLSTS4 Mask             */

#define PWM_INTSTS1_BRKLSTS5_Pos         (29)                                               /*!< PWM_T::INTSTS1: BRKLSTS5 Position         */
#define PWM_INTSTS1_BRKLSTS5_Msk         (0x1ul << PWM_INTSTS1_BRKLSTS5_Pos)                /*!< PWM_T::INTSTS1: BRKLSTS5 Mask             */

#define PWM_ADCTS0_TRGSEL0_Pos           (0)                                                /*!< PWM_T::ADCTS0: TRGSEL0 Position           */
#define PWM_ADCTS0_TRGSEL0_Msk           (0xful << PWM_ADCTS0_TRGSEL0_Pos)                  /*!< PWM_T::ADCTS0: TRGSEL0 Mask               */

#define PWM_ADCTS0_TRGEN0_Pos            (7)                                                /*!< PWM_T::ADCTS0: TRGEN0 Position            */
#define PWM_ADCTS0_TRGEN0_Msk            (0x1ul << PWM_ADCTS0_TRGEN0_Pos)                   /*!< PWM_T::ADCTS0: TRGEN0 Mask                */

#define PWM_ADCTS0_TRGSEL1_Pos           (8)                                                /*!< PWM_T::ADCTS0: TRGSEL1 Position           */
#define PWM_ADCTS0_TRGSEL1_Msk           (0xful << PWM_ADCTS0_TRGSEL1_Pos)                  /*!< PWM_T::ADCTS0: TRGSEL1 Mask               */

#define PWM_ADCTS0_TRGEN1_Pos            (15)                                               /*!< PWM_T::ADCTS0: TRGEN1 Position            */
#define PWM_ADCTS0_TRGEN1_Msk            (0x1ul << PWM_ADCTS0_TRGEN1_Pos)                   /*!< PWM_T::ADCTS0: TRGEN1 Mask                */

#define PWM_ADCTS0_TRGSEL2_Pos           (16)                                               /*!< PWM_T::ADCTS0: TRGSEL2 Position           */
#define PWM_ADCTS0_TRGSEL2_Msk           (0xful << PWM_ADCTS0_TRGSEL2_Pos)                  /*!< PWM_T::ADCTS0: TRGSEL2 Mask               */

#define PWM_ADCTS0_TRGEN2_Pos            (23)                                               /*!< PWM_T::ADCTS0: TRGEN2 Position            */
#define PWM_ADCTS0_TRGEN2_Msk            (0x1ul << PWM_ADCTS0_TRGEN2_Pos)                   /*!< PWM_T::ADCTS0: TRGEN2 Mask                */

#define PWM_ADCTS0_TRGSEL3_Pos           (24)                                               /*!< PWM_T::ADCTS0: TRGSEL3 Position           */
#define PWM_ADCTS0_TRGSEL3_Msk           (0xful << PWM_ADCTS0_TRGSEL3_Pos)                  /*!< PWM_T::ADCTS0: TRGSEL3 Mask               */

#define PWM_ADCTS0_TRGEN3_Pos            (31)                                               /*!< PWM_T::ADCTS0: TRGEN3 Position            */
#define PWM_ADCTS0_TRGEN3_Msk            (0x1ul << PWM_ADCTS0_TRGEN3_Pos)                   /*!< PWM_T::ADCTS0: TRGEN3 Mask                */

#define PWM_ADCTS1_TRGSEL4_Pos           (0)                                                /*!< PWM_T::ADCTS1: TRGSEL4 Position           */
#define PWM_ADCTS1_TRGSEL4_Msk           (0xful << PWM_ADCTS1_TRGSEL4_Pos)                  /*!< PWM_T::ADCTS1: TRGSEL4 Mask               */

#define PWM_ADCTS1_TRGEN4_Pos            (7)                                                /*!< PWM_T::ADCTS1: TRGEN4 Position            */
#define PWM_ADCTS1_TRGEN4_Msk            (0x1ul << PWM_ADCTS1_TRGEN4_Pos)                   /*!< PWM_T::ADCTS1: TRGEN4 Mask                */

#define PWM_ADCTS1_TRGSEL5_Pos           (8)                                                /*!< PWM_T::ADCTS1: TRGSEL5 Position           */
#define PWM_ADCTS1_TRGSEL5_Msk           (0xful << PWM_ADCTS1_TRGSEL5_Pos)                  /*!< PWM_T::ADCTS1: TRGSEL5 Mask               */

#define PWM_ADCTS1_TRGEN5_Pos            (15)                                               /*!< PWM_T::ADCTS1: TRGEN5 Position            */
#define PWM_ADCTS1_TRGEN5_Msk            (0x1ul << PWM_ADCTS1_TRGEN5_Pos)                   /*!< PWM_T::ADCTS1: TRGEN5 Mask                */

#define PWM_SSCTL_SSENn_Pos              (0)                                                /*!< PWM_T::SSCTL: SSENn Position              */
#define PWM_SSCTL_SSENn_Msk              (0x15ul << PWM_SSCTL_SSENn_Pos)                    /*!< PWM_T::SSCTL: SSENn Mask                  */

#define PWM_SSCTL_SSEN0_Pos              (0)                                                /*!< PWM_T::SSCTL: SSEN0 Position              */
#define PWM_SSCTL_SSEN0_Msk              (0x1ul << PWM_SSCTL_SSEN0_Pos)                     /*!< PWM_T::SSCTL: SSEN0 Mask                  */

#define PWM_SSCTL_SSEN2_Pos              (2)                                                /*!< PWM_T::SSCTL: SSEN2 Position              */
#define PWM_SSCTL_SSEN2_Msk              (0x1ul << PWM_SSCTL_SSEN2_Pos)                     /*!< PWM_T::SSCTL: SSEN2 Mask                  */

#define PWM_SSCTL_SSEN4_Pos              (4)                                                /*!< PWM_T::SSCTL: SSEN4 Position              */
#define PWM_SSCTL_SSEN4_Msk              (0x1ul << PWM_SSCTL_SSEN4_Pos)                     /*!< PWM_T::SSCTL: SSEN4 Mask                  */

#define PWM_SSCTL_SSRC_Pos               (8)                                                /*!< PWM_T::SSCTL: SSRC Position               */
#define PWM_SSCTL_SSRC_Msk               (0x3ul << PWM_SSCTL_SSRC_Pos)                      /*!< PWM_T::SSCTL: SSRC Mask                   */

#define PWM_SSTRG_CNTSEN_Pos             (0)                                                /*!< PWM_T::SSTRG: CNTSEN Position             */
#define PWM_SSTRG_CNTSEN_Msk             (0x1ul << PWM_SSTRG_CNTSEN_Pos)                    /*!< PWM_T::SSTRG: CNTSEN Mask                 */

#define PWM_STATUS_CNTMAXFn_Pos          (0)                                                /*!< PWM_T::STATUS: CNTMAXFn Position          */
#define PWM_STATUS_CNTMAXFn_Msk          (0x15ul << PWM_STATUS_CNTMAXFn_Pos)                /*!< PWM_T::STATUS: CNTMAXFn Mask              */

#define PWM_STATUS_CNTMAXF0_Pos          (0)                                                /*!< PWM_T::STATUS: CNTMAXF0 Position          */
#define PWM_STATUS_CNTMAXF0_Msk          (0x1ul << PWM_STATUS_CNTMAXF0_Pos)                 /*!< PWM_T::STATUS: CNTMAXF0 Mask              */

#define PWM_STATUS_CNTMAXF2_Pos          (2)                                                /*!< PWM_T::STATUS: CNTMAXF2 Position          */
#define PWM_STATUS_CNTMAXF2_Msk          (0x1ul << PWM_STATUS_CNTMAXF2_Pos)                 /*!< PWM_T::STATUS: CNTMAXF2 Mask              */

#define PWM_STATUS_CNTMAXF4_Pos          (4)                                                /*!< PWM_T::STATUS: CNTMAXF4 Position          */
#define PWM_STATUS_CNTMAXF4_Msk          (0x1ul << PWM_STATUS_CNTMAXF4_Pos)                 /*!< PWM_T::STATUS: CNTMAXF4 Mask              */

#define PWM_STATUS_ADCTRGFn_Pos          (16)                                               /*!< PWM_T::STATUS: ADCTRGFn Position          */
#define PWM_STATUS_ADCTRGFn_Msk          (0x3ful << PWM_STATUS_ADCTRGFn_Pos)                /*!< PWM_T::STATUS: ADCTRGFn Mask              */

#define PWM_STATUS_ADCTRGF0_Pos          (16)                                               /*!< PWM_T::STATUS: ADCTRGF0 Position          */
#define PWM_STATUS_ADCTRGF0_Msk          (0x1ul << PWM_STATUS_ADCTRGF0_Pos)                 /*!< PWM_T::STATUS: ADCTRGF0 Mask              */

#define PWM_STATUS_ADCTRGF1_Pos          (17)                                               /*!< PWM_T::STATUS: ADCTRGF1 Position          */
#define PWM_STATUS_ADCTRGF1_Msk          (0x1ul << PWM_STATUS_ADCTRGF1_Pos)                 /*!< PWM_T::STATUS: ADCTRGF1 Mask              */

#define PWM_STATUS_ADCTRGF2_Pos          (18)                                               /*!< PWM_T::STATUS: ADCTRGF2 Position          */
#define PWM_STATUS_ADCTRGF2_Msk          (0x1ul << PWM_STATUS_ADCTRGF2_Pos)                 /*!< PWM_T::STATUS: ADCTRGF2 Mask              */

#define PWM_STATUS_ADCTRGF3_Pos          (19)                                               /*!< PWM_T::STATUS: ADCTRGF3 Position          */
#define PWM_STATUS_ADCTRGF3_Msk          (0x1ul << PWM_STATUS_ADCTRGF3_Pos)                 /*!< PWM_T::STATUS: ADCTRGF3 Mask              */

#define PWM_STATUS_ADCTRGF4_Pos          (20)                                               /*!< PWM_T::STATUS: ADCTRGF4 Position          */
#define PWM_STATUS_ADCTRGF4_Msk          (0x1ul << PWM_STATUS_ADCTRGF4_Pos)                 /*!< PWM_T::STATUS: ADCTRGF4 Mask              */

#define PWM_STATUS_ADCTRGF5_Pos          (21)                                               /*!< PWM_T::STATUS: ADCTRGF5 Position          */
#define PWM_STATUS_ADCTRGF5_Msk          (0x1ul << PWM_STATUS_ADCTRGF5_Pos)                 /*!< PWM_T::STATUS: ADCTRGF5 Mask              */

#define PWM_CAPINEN_CAPINENn_Pos         (0)                                                /*!< PWM_T::CAPINEN: CAPINENn Position         */
#define PWM_CAPINEN_CAPINENn_Msk         (0x3ful << PWM_CAPINEN_CAPINENn_Pos)               /*!< PWM_T::CAPINEN: CAPINENn Mask             */

#define PWM_CAPINEN_CAPINEN0_Pos         (0)                                                /*!< PWM_T::CAPINEN: CAPINEN0 Position         */
#define PWM_CAPINEN_CAPINEN0_Msk         (0x1ul << PWM_CAPINEN_CAPINEN0_Pos)                /*!< PWM_T::CAPINEN: CAPINEN0 Mask             */

#define PWM_CAPINEN_CAPINEN1_Pos         (1)                                                /*!< PWM_T::CAPINEN: CAPINEN1 Position         */
#define PWM_CAPINEN_CAPINEN1_Msk         (0x1ul << PWM_CAPINEN_CAPINEN1_Pos)                /*!< PWM_T::CAPINEN: CAPINEN1 Mask             */

#define PWM_CAPINEN_CAPINEN2_Pos         (2)                                                /*!< PWM_T::CAPINEN: CAPINEN2 Position         */
#define PWM_CAPINEN_CAPINEN2_Msk         (0x1ul << PWM_CAPINEN_CAPINEN2_Pos)                /*!< PWM_T::CAPINEN: CAPINEN2 Mask             */

#define PWM_CAPINEN_CAPINEN3_Pos         (3)                                                /*!< PWM_T::CAPINEN: CAPINEN3 Position         */
#define PWM_CAPINEN_CAPINEN3_Msk         (0x1ul << PWM_CAPINEN_CAPINEN3_Pos)                /*!< PWM_T::CAPINEN: CAPINEN3 Mask             */

#define PWM_CAPINEN_CAPINEN4_Pos         (4)                                                /*!< PWM_T::CAPINEN: CAPINEN4 Position         */
#define PWM_CAPINEN_CAPINEN4_Msk         (0x1ul << PWM_CAPINEN_CAPINEN4_Pos)                /*!< PWM_T::CAPINEN: CAPINEN4 Mask             */

#define PWM_CAPINEN_CAPINEN5_Pos         (5)                                                /*!< PWM_T::CAPINEN: CAPINEN5 Position         */
#define PWM_CAPINEN_CAPINEN5_Msk         (0x1ul << PWM_CAPINEN_CAPINEN5_Pos)                /*!< PWM_T::CAPINEN: CAPINEN5 Mask             */

#define PWM_CAPCTL_CAPENn_Pos            (0)                                                /*!< PWM_T::CAPCTL: CAPENn Position            */
#define PWM_CAPCTL_CAPENn_Msk            (0x3ful << PWM_CAPCTL_CAPENn_Pos)                  /*!< PWM_T::CAPCTL: CAPENn Mask                */

#define PWM_CAPCTL_CAPEN0_Pos            (0)                                                /*!< PWM_T::CAPCTL: CAPEN0 Position            */
#define PWM_CAPCTL_CAPEN0_Msk            (0x1ul << PWM_CAPCTL_CAPEN0_Pos)                   /*!< PWM_T::CAPCTL: CAPEN0 Mask                */

#define PWM_CAPCTL_CAPEN1_Pos            (1)                                                /*!< PWM_T::CAPCTL: CAPEN1 Position            */
#define PWM_CAPCTL_CAPEN1_Msk            (0x1ul << PWM_CAPCTL_CAPEN1_Pos)                   /*!< PWM_T::CAPCTL: CAPEN1 Mask                */

#define PWM_CAPCTL_CAPEN2_Pos            (2)                                                /*!< PWM_T::CAPCTL: CAPEN2 Position            */
#define PWM_CAPCTL_CAPEN2_Msk            (0x1ul << PWM_CAPCTL_CAPEN2_Pos)                   /*!< PWM_T::CAPCTL: CAPEN2 Mask                */

#define PWM_CAPCTL_CAPEN3_Pos            (3)                                                /*!< PWM_T::CAPCTL: CAPEN3 Position            */
#define PWM_CAPCTL_CAPEN3_Msk            (0x1ul << PWM_CAPCTL_CAPEN3_Pos)                   /*!< PWM_T::CAPCTL: CAPEN3 Mask                */

#define PWM_CAPCTL_CAPEN4_Pos            (4)                                                /*!< PWM_T::CAPCTL: CAPEN4 Position            */
#define PWM_CAPCTL_CAPEN4_Msk            (0x1ul << PWM_CAPCTL_CAPEN4_Pos)                   /*!< PWM_T::CAPCTL: CAPEN4 Mask                */

#define PWM_CAPCTL_CAPEN5_Pos            (5)                                                /*!< PWM_T::CAPCTL: CAPEN5 Position            */
#define PWM_CAPCTL_CAPEN5_Msk            (0x1ul << PWM_CAPCTL_CAPEN5_Pos)                   /*!< PWM_T::CAPCTL: CAPEN5 Mask                */

#define PWM_CAPCTL_CAPINVn_Pos           (8)                                                /*!< PWM_T::CAPCTL: CAPINVn Position           */
#define PWM_CAPCTL_CAPINVn_Msk           (0x3ful << PWM_CAPCTL_CAPINVn_Pos)                 /*!< PWM_T::CAPCTL: CAPINVn Mask               */

#define PWM_CAPCTL_CAPINV0_Pos           (8)                                                /*!< PWM_T::CAPCTL: CAPINV0 Position           */
#define PWM_CAPCTL_CAPINV0_Msk           (0x1ul << PWM_CAPCTL_CAPINV0_Pos)                  /*!< PWM_T::CAPCTL: CAPINV0 Mask               */

#define PWM_CAPCTL_CAPINV1_Pos           (9)                                                /*!< PWM_T::CAPCTL: CAPINV1 Position           */
#define PWM_CAPCTL_CAPINV1_Msk           (0x1ul << PWM_CAPCTL_CAPINV1_Pos)                  /*!< PWM_T::CAPCTL: CAPINV1 Mask               */

#define PWM_CAPCTL_CAPINV2_Pos           (10)                                               /*!< PWM_T::CAPCTL: CAPINV2 Position           */
#define PWM_CAPCTL_CAPINV2_Msk           (0x1ul << PWM_CAPCTL_CAPINV2_Pos)                  /*!< PWM_T::CAPCTL: CAPINV2 Mask               */

#define PWM_CAPCTL_CAPINV3_Pos           (11)                                               /*!< PWM_T::CAPCTL: CAPINV3 Position           */
#define PWM_CAPCTL_CAPINV3_Msk           (0x1ul << PWM_CAPCTL_CAPINV3_Pos)                  /*!< PWM_T::CAPCTL: CAPINV3 Mask               */

#define PWM_CAPCTL_CAPINV4_Pos           (12)                                               /*!< PWM_T::CAPCTL: CAPINV4 Position           */
#define PWM_CAPCTL_CAPINV4_Msk           (0x1ul << PWM_CAPCTL_CAPINV4_Pos)                  /*!< PWM_T::CAPCTL: CAPINV4 Mask               */

#define PWM_CAPCTL_CAPINV5_Pos           (13)                                               /*!< PWM_T::CAPCTL: CAPINV5 Position           */
#define PWM_CAPCTL_CAPINV5_Msk           (0x1ul << PWM_CAPCTL_CAPINV5_Pos)                  /*!< PWM_T::CAPCTL: CAPINV5 Mask               */

#define PWM_CAPCTL_RCRLDENn_Pos          (16)                                               /*!< PWM_T::CAPCTL: RCRLDENn Position          */
#define PWM_CAPCTL_RCRLDENn_Msk          (0x3ful << PWM_CAPCTL_RCRLDENn_Pos)                /*!< PWM_T::CAPCTL: RCRLDENn Mask              */

#define PWM_CAPCTL_RCRLDEN0_Pos          (16)                                               /*!< PWM_T::CAPCTL: RCRLDEN0 Position          */
#define PWM_CAPCTL_RCRLDEN0_Msk          (0x1ul << PWM_CAPCTL_RCRLDEN0_Pos)                 /*!< PWM_T::CAPCTL: RCRLDEN0 Mask              */

#define PWM_CAPCTL_RCRLDEN1_Pos          (17)                                               /*!< PWM_T::CAPCTL: RCRLDEN1 Position          */
#define PWM_CAPCTL_RCRLDEN1_Msk          (0x1ul << PWM_CAPCTL_RCRLDEN1_Pos)                 /*!< PWM_T::CAPCTL: RCRLDEN1 Mask              */

#define PWM_CAPCTL_RCRLDEN2_Pos          (18)                                               /*!< PWM_T::CAPCTL: RCRLDEN2 Position          */
#define PWM_CAPCTL_RCRLDEN2_Msk          (0x1ul << PWM_CAPCTL_RCRLDEN2_Pos)                 /*!< PWM_T::CAPCTL: RCRLDEN2 Mask              */

#define PWM_CAPCTL_RCRLDEN3_Pos          (19)                                               /*!< PWM_T::CAPCTL: RCRLDEN3 Position          */
#define PWM_CAPCTL_RCRLDEN3_Msk          (0x1ul << PWM_CAPCTL_RCRLDEN3_Pos)                 /*!< PWM_T::CAPCTL: RCRLDEN3 Mask              */

#define PWM_CAPCTL_RCRLDEN4_Pos          (20)                                               /*!< PWM_T::CAPCTL: RCRLDEN4 Position          */
#define PWM_CAPCTL_RCRLDEN4_Msk          (0x1ul << PWM_CAPCTL_RCRLDEN4_Pos)                 /*!< PWM_T::CAPCTL: RCRLDEN4 Mask              */

#define PWM_CAPCTL_RCRLDEN5_Pos          (21)                                               /*!< PWM_T::CAPCTL: RCRLDEN5 Position          */
#define PWM_CAPCTL_RCRLDEN5_Msk          (0x1ul << PWM_CAPCTL_RCRLDEN5_Pos)                 /*!< PWM_T::CAPCTL: RCRLDEN5 Mask              */

#define PWM_CAPCTL_FCRLDENn_Pos          (24)                                               /*!< PWM_T::CAPCTL: FCRLDENn Position          */
#define PWM_CAPCTL_FCRLDENn_Msk          (0x3ful << PWM_CAPCTL_FCRLDENn_Pos)                /*!< PWM_T::CAPCTL: FCRLDENn Mask              */

#define PWM_CAPCTL_FCRLDEN0_Pos          (24)                                               /*!< PWM_T::CAPCTL: FCRLDEN0 Position          */
#define PWM_CAPCTL_FCRLDEN0_Msk          (0x1ul << PWM_CAPCTL_FCRLDEN0_Pos)                 /*!< PWM_T::CAPCTL: FCRLDEN0 Mask              */

#define PWM_CAPCTL_FCRLDEN1_Pos          (25)                                               /*!< PWM_T::CAPCTL: FCRLDEN1 Position          */
#define PWM_CAPCTL_FCRLDEN1_Msk          (0x1ul << PWM_CAPCTL_FCRLDEN1_Pos)                 /*!< PWM_T::CAPCTL: FCRLDEN1 Mask              */

#define PWM_CAPCTL_FCRLDEN2_Pos          (26)                                               /*!< PWM_T::CAPCTL: FCRLDEN2 Position          */
#define PWM_CAPCTL_FCRLDEN2_Msk          (0x1ul << PWM_CAPCTL_FCRLDEN2_Pos)                 /*!< PWM_T::CAPCTL: FCRLDEN2 Mask              */

#define PWM_CAPCTL_FCRLDEN3_Pos          (27)                                               /*!< PWM_T::CAPCTL: FCRLDEN3 Position          */
#define PWM_CAPCTL_FCRLDEN3_Msk          (0x1ul << PWM_CAPCTL_FCRLDEN3_Pos)                 /*!< PWM_T::CAPCTL: FCRLDEN3 Mask              */

#define PWM_CAPCTL_FCRLDEN4_Pos          (28)                                               /*!< PWM_T::CAPCTL: FCRLDEN4 Position          */
#define PWM_CAPCTL_FCRLDEN4_Msk          (0x1ul << PWM_CAPCTL_FCRLDEN4_Pos)                 /*!< PWM_T::CAPCTL: FCRLDEN4 Mask              */

#define PWM_CAPCTL_FCRLDEN5_Pos          (29)                                               /*!< PWM_T::CAPCTL: FCRLDEN5 Position          */
#define PWM_CAPCTL_FCRLDEN5_Msk          (0x1ul << PWM_CAPCTL_FCRLDEN5_Pos)                 /*!< PWM_T::CAPCTL: FCRLDEN5 Mask              */

#define PWM_CAPSTS_CRLIFOVn_Pos          (0)                                                /*!< PWM_T::CAPSTS: CRLIFOVn Position          */
#define PWM_CAPSTS_CRLIFOVn_Msk          (0x3ful << PWM_CAPSTS_CRLIFOVn_Pos)                /*!< PWM_T::CAPSTS: CRLIFOVn Mask              */

#define PWM_CAPSTS_CRLIFOV0_Pos          (0)                                                /*!< PWM_T::CAPSTS: CRLIFOV0 Position          */
#define PWM_CAPSTS_CRLIFOV0_Msk          (0x1ul << PWM_CAPSTS_CRLIFOV0_Pos)                 /*!< PWM_T::CAPSTS: CRLIFOV0 Mask              */

#define PWM_CAPSTS_CRLIFOV1_Pos          (1)                                                /*!< PWM_T::CAPSTS: CRLIFOV1 Position          */
#define PWM_CAPSTS_CRLIFOV1_Msk          (0x1ul << PWM_CAPSTS_CRLIFOV1_Pos)                 /*!< PWM_T::CAPSTS: CRLIFOV1 Mask              */

#define PWM_CAPSTS_CRLIFOV2_Pos          (2)                                                /*!< PWM_T::CAPSTS: CRLIFOV2 Position          */
#define PWM_CAPSTS_CRLIFOV2_Msk          (0x1ul << PWM_CAPSTS_CRLIFOV2_Pos)                 /*!< PWM_T::CAPSTS: CRLIFOV2 Mask              */

#define PWM_CAPSTS_CRLIFOV3_Pos          (3)                                                /*!< PWM_T::CAPSTS: CRLIFOV3 Position          */
#define PWM_CAPSTS_CRLIFOV3_Msk          (0x1ul << PWM_CAPSTS_CRLIFOV3_Pos)                 /*!< PWM_T::CAPSTS: CRLIFOV3 Mask              */

#define PWM_CAPSTS_CRLIFOV4_Pos          (4)                                                /*!< PWM_T::CAPSTS: CRLIFOV4 Position          */
#define PWM_CAPSTS_CRLIFOV4_Msk          (0x1ul << PWM_CAPSTS_CRLIFOV4_Pos)                 /*!< PWM_T::CAPSTS: CRLIFOV4 Mask              */

#define PWM_CAPSTS_CRLIFOV5_Pos          (5)                                                /*!< PWM_T::CAPSTS: CRLIFOV5 Position          */
#define PWM_CAPSTS_CRLIFOV5_Msk          (0x1ul << PWM_CAPSTS_CRLIFOV5_Pos)                 /*!< PWM_T::CAPSTS: CRLIFOV5 Mask              */

#define PWM_CAPSTS_CFLIFOVn_Pos          (8)                                                /*!< PWM_T::CAPSTS: CFLIFOVn Position          */
#define PWM_CAPSTS_CFLIFOVn_Msk          (0x3ful << PWM_CAPSTS_CFLIFOVn_Pos)                /*!< PWM_T::CAPSTS: CFLIFOVn Mask              */

#define PWM_CAPSTS_CFLIFOV0_Pos          (8)                                                /*!< PWM_T::CAPSTS: CFLIFOV0 Position          */
#define PWM_CAPSTS_CFLIFOV0_Msk          (0x1ul << PWM_CAPSTS_CFLIFOV0_Pos)                 /*!< PWM_T::CAPSTS: CFLIFOV0 Mask              */

#define PWM_CAPSTS_CFLIFOV1_Pos          (9)                                                /*!< PWM_T::CAPSTS: CFLIFOV1 Position          */
#define PWM_CAPSTS_CFLIFOV1_Msk          (0x1ul << PWM_CAPSTS_CFLIFOV1_Pos)                 /*!< PWM_T::CAPSTS: CFLIFOV1 Mask              */

#define PWM_CAPSTS_CFLIFOV2_Pos          (10)                                               /*!< PWM_T::CAPSTS: CFLIFOV2 Position          */
#define PWM_CAPSTS_CFLIFOV2_Msk          (0x1ul << PWM_CAPSTS_CFLIFOV2_Pos)                 /*!< PWM_T::CAPSTS: CFLIFOV2 Mask              */

#define PWM_CAPSTS_CFLIFOV3_Pos          (11)                                               /*!< PWM_T::CAPSTS: CFLIFOV3 Position          */
#define PWM_CAPSTS_CFLIFOV3_Msk          (0x1ul << PWM_CAPSTS_CFLIFOV3_Pos)                 /*!< PWM_T::CAPSTS: CFLIFOV3 Mask              */

#define PWM_CAPSTS_CFLIFOV4_Pos          (12)                                               /*!< PWM_T::CAPSTS: CFLIFOV4 Position          */
#define PWM_CAPSTS_CFLIFOV4_Msk          (0x1ul << PWM_CAPSTS_CFLIFOV4_Pos)                 /*!< PWM_T::CAPSTS: CFLIFOV4 Mask              */

#define PWM_CAPSTS_CFLIFOV5_Pos          (13)                                               /*!< PWM_T::CAPSTS: CFLIFOV5 Position          */
#define PWM_CAPSTS_CFLIFOV5_Msk          (0x1ul << PWM_CAPSTS_CFLIFOV5_Pos)                 /*!< PWM_T::CAPSTS: CFLIFOV5 Mask              */

#define PWM_RCAPDAT0_RCAPDAT_Pos         (0)                                                /*!< PWM_T::RCAPDAT0: RCAPDAT Position         */
#define PWM_RCAPDAT0_RCAPDAT_Msk         (0xfffful << PWM_RCAPDAT0_RCAPDAT_Pos)             /*!< PWM_T::RCAPDAT0: RCAPDAT Mask             */

#define PWM_FCAPDAT0_FCAPDAT_Pos         (0)                                                /*!< PWM_T::FCAPDAT0: FCAPDAT Position         */
#define PWM_FCAPDAT0_FCAPDAT_Msk         (0xfffful << PWM_FCAPDAT0_FCAPDAT_Pos)             /*!< PWM_T::FCAPDAT0: FCAPDAT Mask             */

#define PWM_RCAPDAT1_RCAPDAT_Pos         (0)                                                /*!< PWM_T::RCAPDAT1: RCAPDAT Position         */
#define PWM_RCAPDAT1_RCAPDAT_Msk         (0xfffful << PWM_RCAPDAT1_RCAPDAT_Pos)             /*!< PWM_T::RCAPDAT1: RCAPDAT Mask             */

#define PWM_FCAPDAT1_FCAPDAT_Pos         (0)                                                /*!< PWM_T::FCAPDAT1: FCAPDAT Position         */
#define PWM_FCAPDAT1_FCAPDAT_Msk         (0xfffful << PWM_FCAPDAT1_FCAPDAT_Pos)             /*!< PWM_T::FCAPDAT1: FCAPDAT Mask             */

#define PWM_RCAPDAT2_RCAPDAT_Pos         (0)                                                /*!< PWM_T::RCAPDAT2: RCAPDAT Position         */
#define PWM_RCAPDAT2_RCAPDAT_Msk         (0xfffful << PWM_RCAPDAT2_RCAPDAT_Pos)             /*!< PWM_T::RCAPDAT2: RCAPDAT Mask             */

#define PWM_FCAPDAT2_FCAPDAT_Pos         (0)                                                /*!< PWM_T::FCAPDAT2: FCAPDAT Position         */
#define PWM_FCAPDAT2_FCAPDAT_Msk         (0xfffful << PWM_FCAPDAT2_FCAPDAT_Pos)             /*!< PWM_T::FCAPDAT2: FCAPDAT Mask             */

#define PWM_RCAPDAT3_RCAPDAT_Pos         (0)                                                /*!< PWM_T::RCAPDAT3: RCAPDAT Position         */
#define PWM_RCAPDAT3_RCAPDAT_Msk         (0xfffful << PWM_RCAPDAT3_RCAPDAT_Pos)             /*!< PWM_T::RCAPDAT3: RCAPDAT Mask             */

#define PWM_FCAPDAT3_FCAPDAT_Pos         (0)                                                /*!< PWM_T::FCAPDAT3: FCAPDAT Position         */
#define PWM_FCAPDAT3_FCAPDAT_Msk         (0xfffful << PWM_FCAPDAT3_FCAPDAT_Pos)             /*!< PWM_T::FCAPDAT3: FCAPDAT Mask             */

#define PWM_RCAPDAT4_RCAPDAT_Pos         (0)                                                /*!< PWM_T::RCAPDAT4: RCAPDAT Position         */
#define PWM_RCAPDAT4_RCAPDAT_Msk         (0xfffful << PWM_RCAPDAT4_RCAPDAT_Pos)             /*!< PWM_T::RCAPDAT4: RCAPDAT Mask             */

#define PWM_FCAPDAT4_FCAPDAT_Pos         (0)                                                /*!< PWM_T::FCAPDAT4: FCAPDAT Position         */
#define PWM_FCAPDAT4_FCAPDAT_Msk         (0xfffful << PWM_FCAPDAT4_FCAPDAT_Pos)             /*!< PWM_T::FCAPDAT4: FCAPDAT Mask             */

#define PWM_RCAPDAT5_RCAPDAT_Pos         (0)                                                /*!< PWM_T::RCAPDAT5: RCAPDAT Position         */
#define PWM_RCAPDAT5_RCAPDAT_Msk         (0xfffful << PWM_RCAPDAT5_RCAPDAT_Pos)             /*!< PWM_T::RCAPDAT5: RCAPDAT Mask             */

#define PWM_FCAPDAT5_FCAPDAT_Pos         (0)                                                /*!< PWM_T::FCAPDAT5: FCAPDAT Position         */
#define PWM_FCAPDAT5_FCAPDAT_Msk         (0xfffful << PWM_FCAPDAT5_FCAPDAT_Pos)             /*!< PWM_T::FCAPDAT5: FCAPDAT Mask             */

#define PWM_CAPIEN_CAPRIENn_Pos          (0)                                                /*!< PWM_T::CAPIEN: CAPRIENn Position          */
#define PWM_CAPIEN_CAPRIENn_Msk          (0x3ful << PWM_CAPIEN_CAPRIENn_Pos)                /*!< PWM_T::CAPIEN: CAPRIENn Mask              */

#define PWM_CAPIEN_CAPRIEN0_Pos          (0)                                                /*!< PWM_T::CAPIEN: CAPRIEN0 Position          */
#define PWM_CAPIEN_CAPRIEN0_Msk          (0x1ul << PWM_CAPIEN_CAPRIEN0_Pos)                 /*!< PWM_T::CAPIEN: CAPRIEN0 Mask              */

#define PWM_CAPIEN_CAPRIEN1_Pos          (1)                                                /*!< PWM_T::CAPIEN: CAPRIEN1 Position          */
#define PWM_CAPIEN_CAPRIEN1_Msk          (0x1ul << PWM_CAPIEN_CAPRIEN1_Pos)                 /*!< PWM_T::CAPIEN: CAPRIEN1 Mask              */

#define PWM_CAPIEN_CAPRIEN2_Pos          (2)                                                /*!< PWM_T::CAPIEN: CAPRIEN2 Position          */
#define PWM_CAPIEN_CAPRIEN2_Msk          (0x1ul << PWM_CAPIEN_CAPRIEN2_Pos)                 /*!< PWM_T::CAPIEN: CAPRIEN2 Mask              */

#define PWM_CAPIEN_CAPRIEN3_Pos          (3)                                                /*!< PWM_T::CAPIEN: CAPRIEN3 Position          */
#define PWM_CAPIEN_CAPRIEN3_Msk          (0x1ul << PWM_CAPIEN_CAPRIEN3_Pos)                 /*!< PWM_T::CAPIEN: CAPRIEN3 Mask              */

#define PWM_CAPIEN_CAPRIEN4_Pos          (4)                                                /*!< PWM_T::CAPIEN: CAPRIEN4 Position          */
#define PWM_CAPIEN_CAPRIEN4_Msk          (0x1ul << PWM_CAPIEN_CAPRIEN4_Pos)                 /*!< PWM_T::CAPIEN: CAPRIEN4 Mask              */

#define PWM_CAPIEN_CAPRIEN5_Pos          (5)                                                /*!< PWM_T::CAPIEN: CAPRIEN5 Position          */
#define PWM_CAPIEN_CAPRIEN5_Msk          (0x1ul << PWM_CAPIEN_CAPRIEN5_Pos)                 /*!< PWM_T::CAPIEN: CAPRIEN5 Mask              */

#define PWM_CAPIEN_CAPFIENn_Pos          (8)                                                /*!< PWM_T::CAPIEN: CAPFIENn Position          */
#define PWM_CAPIEN_CAPFIENn_Msk          (0x3ful << PWM_CAPIEN_CAPFIENn_Pos)                /*!< PWM_T::CAPIEN: CAPFIENn Mask              */

#define PWM_CAPIEN_CAPFIEN0_Pos          (8)                                                /*!< PWM_T::CAPIEN: CAPFIEN0 Position          */
#define PWM_CAPIEN_CAPFIEN0_Msk          (0x1ul << PWM_CAPIEN_CAPFIEN0_Pos)                 /*!< PWM_T::CAPIEN: CAPFIEN0 Mask              */

#define PWM_CAPIEN_CAPFIEN1_Pos          (9)                                                /*!< PWM_T::CAPIEN: CAPFIEN1 Position          */
#define PWM_CAPIEN_CAPFIEN1_Msk          (0x1ul << PWM_CAPIEN_CAPFIEN1_Pos)                 /*!< PWM_T::CAPIEN: CAPFIEN1 Mask              */

#define PWM_CAPIEN_CAPFIEN2_Pos          (10)                                               /*!< PWM_T::CAPIEN: CAPFIEN2 Position          */
#define PWM_CAPIEN_CAPFIEN2_Msk          (0x1ul << PWM_CAPIEN_CAPFIEN2_Pos)                 /*!< PWM_T::CAPIEN: CAPFIEN2 Mask              */

#define PWM_CAPIEN_CAPFIEN3_Pos          (11)                                               /*!< PWM_T::CAPIEN: CAPFIEN3 Position          */
#define PWM_CAPIEN_CAPFIEN3_Msk          (0x1ul << PWM_CAPIEN_CAPFIEN3_Pos)                 /*!< PWM_T::CAPIEN: CAPFIEN3 Mask              */

#define PWM_CAPIEN_CAPFIEN4_Pos          (12)                                               /*!< PWM_T::CAPIEN: CAPFIEN4 Position          */
#define PWM_CAPIEN_CAPFIEN4_Msk          (0x1ul << PWM_CAPIEN_CAPFIEN4_Pos)                 /*!< PWM_T::CAPIEN: CAPFIEN4 Mask              */

#define PWM_CAPIEN_CAPFIEN5_Pos          (13)                                               /*!< PWM_T::CAPIEN: CAPFIEN5 Position          */
#define PWM_CAPIEN_CAPFIEN5_Msk          (0x1ul << PWM_CAPIEN_CAPFIEN5_Pos)                 /*!< PWM_T::CAPIEN: CAPFIEN5 Mask              */

#define PWM_CAPIF_CRLIFn_Pos             (0)                                                /*!< PWM_T::CAPIF: CRLIFn Position             */
#define PWM_CAPIF_CRLIFn_Msk             (0x3ful << PWM_CAPIF_CRLIFn_Pos)                   /*!< PWM_T::CAPIF: CRLIFn Mask                 */

#define PWM_CAPIF_CRLIF0_Pos             (0)                                                /*!< PWM_T::CAPIF: CRLIF0 Position             */
#define PWM_CAPIF_CRLIF0_Msk             (0x1ul << PWM_CAPIF_CRLIF0_Pos)                    /*!< PWM_T::CAPIF: CRLIF0 Mask                 */

#define PWM_CAPIF_CRLIF1_Pos             (1)                                                /*!< PWM_T::CAPIF: CRLIF1 Position             */
#define PWM_CAPIF_CRLIF1_Msk             (0x1ul << PWM_CAPIF_CRLIF1_Pos)                    /*!< PWM_T::CAPIF: CRLIF1 Mask                 */

#define PWM_CAPIF_CRLIF2_Pos             (2)                                                /*!< PWM_T::CAPIF: CRLIF2 Position             */
#define PWM_CAPIF_CRLIF2_Msk             (0x1ul << PWM_CAPIF_CRLIF2_Pos)                    /*!< PWM_T::CAPIF: CRLIF2 Mask                 */

#define PWM_CAPIF_CRLIF3_Pos             (3)                                                /*!< PWM_T::CAPIF: CRLIF3 Position             */
#define PWM_CAPIF_CRLIF3_Msk             (0x1ul << PWM_CAPIF_CRLIF3_Pos)                    /*!< PWM_T::CAPIF: CRLIF3 Mask                 */

#define PWM_CAPIF_CRLIF4_Pos             (4)                                                /*!< PWM_T::CAPIF: CRLIF4 Position             */
#define PWM_CAPIF_CRLIF4_Msk             (0x1ul << PWM_CAPIF_CRLIF4_Pos)                    /*!< PWM_T::CAPIF: CRLIF4 Mask                 */

#define PWM_CAPIF_CRLIF5_Pos             (5)                                                /*!< PWM_T::CAPIF: CRLIF5 Position             */
#define PWM_CAPIF_CRLIF5_Msk             (0x1ul << PWM_CAPIF_CRLIF5_Pos)                    /*!< PWM_T::CAPIF: CRLIF5 Mask                 */

#define PWM_CAPIF_CFLIFn_Pos             (8)                                                /*!< PWM_T::CAPIF: CFLIFn Position             */
#define PWM_CAPIF_CFLIFn_Msk             (0x3ful << PWM_CAPIF_CFLIFn_Pos)                   /*!< PWM_T::CAPIF: CFLIFn Mask                 */

#define PWM_CAPIF_CFLIF0_Pos             (8)                                                /*!< PWM_T::CAPIF: CFLIF0 Position             */
#define PWM_CAPIF_CFLIF0_Msk             (0x1ul << PWM_CAPIF_CFLIF0_Pos)                    /*!< PWM_T::CAPIF: CFLIF0 Mask                 */

#define PWM_CAPIF_CFLIF1_Pos             (9)                                                /*!< PWM_T::CAPIF: CFLIF1 Position             */
#define PWM_CAPIF_CFLIF1_Msk             (0x1ul << PWM_CAPIF_CFLIF1_Pos)                    /*!< PWM_T::CAPIF: CFLIF1 Mask                 */

#define PWM_CAPIF_CFLIF2_Pos             (10)                                               /*!< PWM_T::CAPIF: CFLIF2 Position             */
#define PWM_CAPIF_CFLIF2_Msk             (0x1ul << PWM_CAPIF_CFLIF2_Pos)                    /*!< PWM_T::CAPIF: CFLIF2 Mask                 */

#define PWM_CAPIF_CFLIF3_Pos             (11)                                               /*!< PWM_T::CAPIF: CFLIF3 Position             */
#define PWM_CAPIF_CFLIF3_Msk             (0x1ul << PWM_CAPIF_CFLIF3_Pos)                    /*!< PWM_T::CAPIF: CFLIF3 Mask                 */

#define PWM_CAPIF_CFLIF4_Pos             (12)                                               /*!< PWM_T::CAPIF: CFLIF4 Position             */
#define PWM_CAPIF_CFLIF4_Msk             (0x1ul << PWM_CAPIF_CFLIF4_Pos)                    /*!< PWM_T::CAPIF: CFLIF4 Mask                 */

#define PWM_CAPIF_CFLIF5_Pos             (13)                                               /*!< PWM_T::CAPIF: CFLIF5 Position             */
#define PWM_CAPIF_CFLIF5_Msk             (0x1ul << PWM_CAPIF_CFLIF5_Pos)                    /*!< PWM_T::CAPIF: CFLIF5 Mask                 */

#define PWM_PBUF_PBUF_Pos                (0)                                                /*!< PWM_T::PBUF: PBUF Position                */
#define PWM_PBUF_PBUF_Msk                (0xfffful << PWM_PBUF_PBUF_Pos)                    /*!< PWM_T::PBUF: PBUF Mask                    */

#define PWM_CMPBUF_CMPBUF_Pos            (0)                                                /*!< PWM_T::CMPBUF: CMPBUF Position            */
#define PWM_CMPBUF_CMPBUF_Msk            (0xfffful << PWM_CMPBUF_CMPBUF_Pos)                /*!< PWM_T::CMPBUF: CMPBUF Mask                */

/*@}*/ /* end of group PWM_CONST */
/*@}*/ /* end of group PWM */



/*---------------------- Serial Peripheral Interface Controller -------------------------*/
/**
    @addtogroup SPI Serial Peripheral Interface Controller (SPI)
    Memory Mapped Structure for SPI Controller
@{ */
typedef struct
{
    /**
     * SPI_CNTRL
     * ===================================================================================================
     * Offset: 0x00  Control and Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GO_BUSY   |SPI Transfer Control Bit And Busy Status
     * |        |          |0 = Data transfer stopped.
     * |        |          |1 = In Master mode, writing 1 to this bit to start the SPI data transfer; in Slave mode,
     * |        |          |    writing 1 to this bit indicates that the slave is ready to communicate with a master.
     * |        |          |If FIFO mode is disabled, during the data transfer, this bit keeps the value of 1.
     * |        |          |As the transfer is finished, this bit will be cleared automatically.
     * |        |          |Software can read this bit to check if the SPI is in busy status.
     * |        |          |In FIFO mode, this bit will be controlled by hardware.
     * |        |          |Software should not modify this bit.
     * |        |          |In Slave mode, this bit always returns 1 when this register is read by software.
     * |        |          |In Master mode, this bit reflects the busy or idle status of SPI.
     * |        |          |Note:
     * |        |          |1. When FIFO mode is disabled, all configurations should be set before writing 1 to this GO_BUSY bit.
     * |        |          |2. When FIFO mode is disabled and the software uses TX or RX PDMA function to transfer data, this bit
     * |        |          |   will be cleared after the PDMA finishes the data transfer.
     * |[1]     |RX_NEG    |Receive On Negative Edge
     * |        |          |0 = Received data input signal is latched on the rising edge of SPI bus clock.
     * |        |          |1 = Received data input signal is latched on the falling edge of SPI bus clock.
     * |[2]     |TX_NEG    |Transmit On Negative Edge
     * |        |          |0 = Transmitted data output signal is changed on the rising edge of SPI bus clock.
     * |        |          |1 = Transmitted data output signal is changed on the falling edge of SPI bus clock.
     * |[7:3]   |TX_BIT_LEN|Transmit Bit Length
     * |        |          |This field specifies how many bits can be transmitted / received in one transaction.
     * |        |          |The minimum bit length is 8 bits and can up to 32 bits.
     * |        |          |TX_BIT_LEN = 0x08 ... 8 bits.
     * |        |          |TX_BIT_LEN = 0x09 ... 9 bits.
     * |        |          |......
     * |        |          |TX_BIT_LEN = 0x1F ... 31 bits.
     * |        |          |TX_BIT_LEN = 0x00 ... 32 bits.
     * |[10]    |LSB       |Send LSB First
     * |        |          |0 = The MSB, which bit of transmit/receive register depends on the setting of TX_BIT_LEN, is transmitted/received first.
     * |        |          |1 = The LSB, bit 0 of the SPI TX0/1 register, is sent first to the SPI data output pin, and the first bit received from
     * |        |          |    the SPI data input pin will be put in the LSB position of the RX register (bit 0 of SPI_RX0/1).
     * |[11]    |CLKP      |Clock Polarity
     * |        |          |0 = SPI bus clock is idle low.
     * |        |          |1 = SPI bus clock is idle high.
     * |[15:12] |SP_CYCLE  |Suspend Interval (Master Only)
     * |        |          |The four bits provide configurable suspend interval between two successive transmit/receive transaction in a transfer.
     * |        |          |The definition of the suspend interval is the interval between the last clock edge of the preceding transaction word
     * |        |          |and the first clock edge of the following transaction word.
     * |        |          |The default value is 0x3.
     * |        |          |The period of the suspend interval is obtained according to the following equation.
     * |        |          |(SP_CYCLE[3:0] + 0.5) * period of SPI bus clock cycle
     * |        |          |Example:
     * |        |          |SP_CYCLE = 0x0 ... 0.5 SPI bus clock cycle.
     * |        |          |SP_CYCLE = 0x1 ... 1.5 SPI bus clock cycle.
     * |        |          |......
     * |        |          |SP_CYCLE = 0xE ... 14.5 SPI bus clock cycle.
     * |        |          |SP_CYCLE = 0xF ... 15.5 SPI bus clock cycle.
     * |        |          |If the variable clock function is enabled and the transmit FIFO buffer is not empty, the minimum period of suspend
     * |        |          |interval between the successive transactions is (6.5 + SP_CYCLE) * SPI bus clock cycle.
     * |[16]    |IF        |Unit Transfer Interrupt Flag
     * |        |          |0 = No transaction has been finished since this bit was cleared to 0.
     * |        |          |1 = SPI controller has finished one unit transfer.
     * |        |          |Note: This bit will be cleared by writing 1 to itself.
     * |[17]    |IE        |Unit Transfer Interrupt Enable
     * |        |          |0 = SPI unit transfer interrupt Disabled.
     * |        |          |1 = SPI unit transfer interrupt Enabled.
     * |[18]    |SLAVE     |Slave Mode Enable
     * |        |          |0 = Master mode.
     * |        |          |1 = Slave mode.
     * |[19]    |REORDER   |Byte Reorder Function Enable
     * |        |          |0 = Byte Reorder function Disabled.
     * |        |          |1 = Byte Reorder function Enabled.
     * |        |          |A byte suspend interval will be inserted among each byte.
     * |        |          |The period of the byte suspend interval depends on the setting of SP_CYCLE.
     * |        |          |Note:
     * |        |          |1. Byte Reorder function is only available if TX_BIT_LEN is defined as 16, 24, and 32 bits.
     * |        |          |2. In Slave mode with level-trigger configuration, the slave select pin must be kept at active state during the
     * |        |          |   byte suspend interval.
     * |        |          |3. The Byte Reorder function is not supported when the variable bus clock function or Dual I/O mode is enabled.
     * |[21]    |FIFO      |FIFO Mode Enable
     * |        |          |0 = FIFO mode Disabled.
     * |        |          |1 = FIFO mode Enabled.
     * |        |          |Note:
     * |        |          |1. Before enabling FIFO mode, the other related settings should be set in advance.
     * |        |          |2. In Master mode, if the FIFO mode is enabled, the GO_BUSY bit will be set to 1 automatically after writing data
     * |        |          |   to the transmit FIFO buffer; the GO_BUSY bit will be cleared to 0 automatically when the SPI controller is in idle.
     * |        |          |   If all data stored at transmit FIFO buffer are sent out, the TX_EMPTY bit will be set to 1 and the GO_BUSY bit will be cleared to 0.
     * |        |          |3. After clearing this bit to 0, user must wait for at least 2 peripheral clock periods before setting this bit to 1 again.
     * |[23]    |VARCLK_EN |Variable Clock Enable (Master Only)
     * |        |          |0 = SPI clock output frequency is fixed and decided only by the value of DIVIDER.
     * |        |          |1 = SPI clock output frequency is variable.
     * |        |          |The output frequency is decided by the value of VARCLK, DIVIDER, and DIVIDER2.
     * |        |          |Note: When this VARCLK_EN bit is set to 1, the setting of TX_BIT_LEN must be programmed as 0x10 (16-bit mode).
     * |[24]    |RX_EMPTY  |Receive FIFO Buffer Empty Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_STATUS[24].
     * |        |          |0 = Receive FIFO buffer is not empty.
     * |        |          |1 = Receive FIFO buffer is empty.
     * |[25]    |RX_FULL   |Receive FIFO Buffer Full Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_STATUS[25].
     * |        |          |0 = Receive FIFO buffer is not full.
     * |        |          |1 = Receive FIFO buffer is full.
     * |[26]    |TX_EMPTY  |Transmit FIFO Buffer Empty Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_STATUS[26].
     * |        |          |0 = Transmit FIFO buffer is not empty.
     * |        |          |1 = Transmit FIFO buffer is empty.
     * |[27]    |TX_FULL   |Transmit FIFO Buffer Full Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_STATUS[27].
     * |        |          |0 = Transmit FIFO buffer is not full.
     * |        |          |1 = Transmit FIFO buffer is full.
     */
    __IO uint32_t CNTRL;

    /**
     * SPI_DIVIDER
     * ===================================================================================================
     * Offset: 0x04  Clock Divider Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |DIVIDER   |Clock Divider 1 Register
     * |        |          |The value in this field is the frequency divider for generating the SPI peripheral clock and the SPI bus clock of SPI master.
     * |        |          |The frequency is obtained according to the following equation.
     * |        |          |If the bit of BCn, SPI_CNTRL2[31], is set to 0,
     * |        |          |   SPI peripheral clock frequency = SPI peripheral clock source frequency / (DIVIDER + 1) / 2
     * |        |          |else if BCn is set to 1,
     * |        |          |   SPI peripheral clock frequency = SPI peripheral clock source frequency / (DIVIDER + 1)
     * |        |          |The SPI peripheral clock source is defined in the CLKSEL1 register.
     * |[23:16] |DIVIDER2  |Clock Divider 2 Register (Master Only)
     * |        |          |The value in this field is the 2nd frequency divider for generating the second clock of the variable clock function.
     * |        |          |The frequency is obtained according to the following equation:
     * |        |          |   f_clk2 = SPI peripheral clock frequency / (DIVIDER2 + 1) / 2
     * |        |          |If the VARCLK_EN bit is cleared to 0, this setting is unmeaning.
     */
    __IO uint32_t DIVIDER;

    /**
     * SPI_SSR
     * ===================================================================================================
     * Offset: 0x08  Slave Select Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |SSR       |Slave Select Control Bits (Master Only)
     * |        |          |If AUTOSS bit is cleared, writing 1 to any bit of this field sets the SPI0_SS
     * |        |          |line to an active state and writing 0 sets the line back to inactive state.
     * |        |          |If the AUTOSS bit is set, writing 0 to any bit location of this field will keep the
     * |        |          |SPI0_SS line at inactive state; writing 1 to this field will select
     * |        |          |SPI0_SS line to be automatically driven to active state for the duration of the
     * |        |          |transmit/receive, and will be driven to inactive state for the rest of the time.
     * |        |          |The active state of SPI0_SS is specified in SS_LVL.
     * |[2]     |SS_LVL    |Slave Select Active Level
     * |        |          |This bit defines the active status of slave select signal (SPI0_SS).
     * |        |          |0 = The slave select signal SPI0_SS is active on low-level/falling-edge.
     * |        |          |1 = The slave select signal SPI0_SS is active on high-level/rising-edge.
     * |[3]     |AUTOSS    |Automatic Slave Select Function Enable (Master Only)
     * |        |          |0 = If this bit is cleared, slave select signals will be asserted/de-asserted by setting /clearing
     * |        |          |    SPI_SSR[0].
     * |        |          |1 = If this bit is set, SPI0_SS signals will be generated automatically.
     * |        |          |    It means that slave select signal, which is set in SPI_SSR[0], will be asserted by the
     * |        |          |    SPI controller when transmit/receive is started, and will be de-asserted after each transmit/receive is finished.
     * |[4]     |SS_LTRIG  |Slave Select Level Trigger Enable (Slave Only)
     * |        |          |0 = Slave select signal is edge-trigger.
     * |        |          |    This is the default value.
     * |        |          |    The SS_LVL bit decides the signal is active after a falling-edge or rising-edge.
     * |        |          |1 = Slave select signal is level-trigger.
     * |        |          |    The SS_LVL bit decides the signal is active low or active high.
     * |[5]     |LTRIG_FLAG|Level Trigger Accomplish Flag
     * |        |          |In Slave mode, this bit indicates whether the received bit number meets the requirement or not after the current transaction done.
     * |        |          |0 = Transferred bit length of one transaction does not meet the specified requirement.
     * |        |          |1 = Transferred bit length meets the specified requirement which defined in TX_BIT_LEN.
     * |        |          |Note: This bit is READ only.
     * |        |          |As the GO_BUSY bit is set to 1 by software, the LTRIG_FLAG will be cleared to 0 after 4 SPI peripheral clock periods plus 1 system clock period.
     * |        |          |In FIFO mode, this bit has no meaning.
     */
    __IO uint32_t SSR;

    __I  uint32_t RESERVE0;

    /**
     * SPI_RX
     * ===================================================================================================
     * Offset: 0x10  Data Receive Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |RX        |Data Receive Register
     * |        |          |The data receive register holds the datum received from SPI data input pin.
     * |        |          |If FIFO mode is disabled, the last received data can be accessed through software by reading this register.
     * |        |          |If the FIFO bit is set as 1 and the RX_EMPTY bit, SPI_CNTRL[24] or SPI_STATUS[24], is not set to 1, the receive
     * |        |          |FIFO buffer can be accessed through software by reading this register. This is a read-only register.
     */
    __I  uint32_t RX;

    __I  uint32_t RESERVE1;
    __I  uint32_t RESERVE2;
    __I  uint32_t RESERVE3;

    /**
     * SPI_TX
     * ===================================================================================================
     * Offset: 0x20  Data Transmit Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |TX        |Data Transmit Register
     * |        |          |The data transmit registers hold the data to be transmitted in the next transfer.
     * |        |          |The number of valid bits depends on the setting of transmit bit length field of the SPI_CNTRL register.
     * |        |          |For example, if TX_BIT_LEN is set to 0x08, the bits TX[7:0] will be transmitted in next transfer.
     * |        |          |If TX_BIT_LEN is set to 0x00, the SPI controller will perform a 32-bit transfer.
     * |        |          |Note 1: When the SPI controller is configured as a slave device and FIFO mode is disabled, if the SPI
     * |        |          |        controller attempts to transmit data to a master, the transmit data register should be updated
     * |        |          |        by software before setting the GO_BUSY bit to 1.
     * |        |          |Note 2: In Master mode, SPI controller will start to transfer after 5 peripheral clock cycles after user writes to this register.
     */
    __O  uint32_t TX;

    __I  uint32_t RESERVE4;
    __I  uint32_t RESERVE5;
    __I  uint32_t RESERVE6;
    __I  uint32_t RESERVE7;

    /**
     * SPI_VARCLK
     * ===================================================================================================
     * Offset: 0x34  Variable Clock Pattern Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |VARCLK    |Variable Clock Pattern
     * |        |          |This register defines the clock pattern of the SPI transfer.
     * |        |          |If the variable clock function is disabled, this setting is unmeaning.
     */
    __IO uint32_t VARCLK;

    __I  uint32_t RESERVE8;

    /**
     * SPI_CNTRL2
     * ===================================================================================================
     * Offset: 0x3C  Control and Status Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[8]     |NOSLVSEL  |Slave 3-Wire Mode Enable
     * |        |          |This is used to ignore the slave select signal in Slave mode.
     * |        |          |The SPI controller can work with 3-wire interface including SPIn_CLK, SPIn_MISO, and SPIn_MOSI.
     * |        |          |0 = 4-wire bi-direction interface.
     * |        |          |1 = 3-wire bi-direction interface.
     * |        |          |Note: In Slave 3-wire mode, the SS_LTRIG, SPI_SSR[4] will be set as 1 automatically.
     * |[9]     |SLV_ABORT |Slave 3-Wire Mode Abort Control
     * |        |          |In normal operation, there is an interrupt event when the received data meet the required bits which defined in TX_BIT_LEN.
     * |        |          |If the received bits are less than the requirement and there is no more SPI clock input over the one transfer time in
     * |        |          |Slave 3-wire mode, the user can set this bit to force the current transfer done and then the user can get a transfer done interrupt event.
     * |        |          |Note: This bit will be cleared to 0 automatically by hardware after it is set to 1 by software.
     * |[10]    |SSTA_INTEN|Slave 3-Wire Mode Start Interrupt Enable
     * |        |          |Used to enable interrupt when the transfer has started in Slave 3-wire mode.
     * |        |          |If there is no transfer done interrupt over the time period which is defined by user after the transfer start,
     * |        |          |the user can set the SLV_ABORT bit to force the transfer done.
     * |        |          |0 = Transaction start interrupt Disabled.
     * |        |          |1 = Transaction start interrupt Enabled.
     * |        |          |It will be cleared to 0 as the current transfer is done or the SLV_START_INTSTS bit is cleared.
     * |[11]    |SLV_START_INTSTS|Slave 3-Wire Mode Start Interrupt Status
     * |        |          |This bit indicates if a transaction has started in Slave 3-wire mode.
     * |        |          |It is a mutual mirror bit of SPI_STATUS[11].
     * |        |          |0 = Slave has not detected any SPI clock transition since the SSTA_INTEN bit was set to 1.
     * |        |          |1 = A transaction has started in Slave 3-wire mode.
     * |        |          |It will be cleared automatically when a transaction is done or by writing 1 to this bit.
     * |[12]    |DUAL_IO_DIR|Dual I/O Mode Direction Control
     * |        |          |0 = Dual Input mode.
     * |        |          |1 = Dual Output mode.
     * |[13]    |DUAL_IO_EN|Dual I/O Mode Enable
     * |        |          |0 = Dual I/O mode Disabled.
     * |        |          |1 = Dual I/O mode Enabled.
     * |[16]    |SS_INT_OPT|Slave Select Inactive Interrupt Option
     * |        |          |This setting is only available if the SPI controller is configured as level trigger slave device.
     * |        |          |0 = As the slave select signal goes to inactive level, the IF bit will NOT be set to 1.
     * |        |          |1 = As the slave select signal goes to inactive level, the IF bit will be set to 1.
     * |[31]    |BCn       |SPI Peripheral Clock Backward Compatible Option
     * |        |          |0 = Backward compatible clock configuration.
     * |        |          |1 = Clock configuration is not backward compatible.
     * |        |          |Refer to the description of SPI_DIVIDER register for details.
     */
    __IO uint32_t CNTRL2;

    /**
     * SPI_FIFO_CTL
     * ===================================================================================================
     * Offset: 0x40  SPI FIFO Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RX_CLR    |Clear Receive FIFO Buffer
     * |        |          |0 = No effect.
     * |        |          |1 = Clear receive FIFO buffer.
     * |        |          |The RX_FULL flag will be cleared to 0 and the RX_EMPTY flag will be set to 1.
     * |        |          |This bit will be cleared to 0 by hardware after it is set to 1 by software.
     * |[1]     |TX_CLR    |Clear Transmit FIFO Buffer
     * |        |          |0 = No effect.
     * |        |          |1 = Clear transmit FIFO buffer.
     * |        |          |The TX_FULL flag will be cleared to 0 and the TX_EMPTY flag will be set to 1.
     * |        |          |This bit will be cleared to 0 by hardware after it is set to 1 by software.
     * |[2]     |RX_INTEN  |Receive Threshold Interrupt Enable
     * |        |          |0 = RX threshold interrupt Disabled.
     * |        |          |1 = RX threshold interrupt Enabled.
     * |[3]     |TX_INTEN  |Transmit Threshold Interrupt Enable
     * |        |          |0 = TX threshold interrupt Disabled.
     * |        |          |1 = TX threshold interrupt Enabled.
     * |[6]     |RXOV_INTEN|Receive FIFO Overrun Interrupt Enable
     * |        |          |0 = Receive FIFO overrun interrupt Disabled.
     * |        |          |1 = Receive FIFO overrun interrupt Enabled.
     * |[21]    |TIMEOUT_INTEN|Receive FIFO Time-Out Interrupt Enable
     * |        |          |0 = Time-out interrupt Disabled.
     * |        |          |1 = Time-out interrupt Enabled.
     * |[26:24] |RX_THRESHOLD|Receive FIFO Threshold
     * |        |          |If the valid data count of the receive FIFO buffer is larger than the RX_THRESHOLD setting,
     * |        |          |the RX_INTSTS bit will be set to 1, else the RX_INTSTS bit will be cleared to 0.
     * |[30:28] |TX_THRESHOLD|Transmit FIFO Threshold
     * |        |          |If the valid data count of the transmit FIFO buffer is less than or equal to the TX_THRESHOLD
     * |        |          |setting, the TX_INTSTS bit will be set to 1, else the TX_INTSTS bit will be cleared to 0.
     */
    __IO uint32_t FIFO_CTL;

    /**
     * SPI_STATUS
     * ===================================================================================================
     * Offset: 0x44  SPI Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RX_INTSTS |Receive FIFO Threshold Interrupt Status (Read Only)
     * |        |          |0 = The valid data count within the Rx FIFO buffer is smaller than or equal to the setting value of RX_THRESHOLD.
     * |        |          |1 = The valid data count within the receive FIFO buffer is larger than the setting value of RX_THRESHOLD.
     * |        |          |Note: If RX_INTEN = 1 and RX_INTSTS = 1, the SPI controller will generate a SPI interrupt request.
     * |[2]     |RX_OVERRUN|Receive FIFO Overrun Status
     * |        |          |When the receive FIFO buffer is full, the follow-up data will be dropped and this bit will be set to 1.
     * |        |          |Note: This bit will be cleared by writing 1 to itself.
     * |[4]     |TX_INTSTS |Transmit FIFO Threshold Interrupt Status (Read Only)
     * |        |          |0 = The valid data count within the transmit FIFO buffer is larger than the setting value of TX_THRESHOLD.
     * |        |          |1 = The valid data count within the transmit FIFO buffer is less than or equal to the setting value of TX_THRESHOLD.
     * |        |          |Note: If TX_INTEN = 1 and TX_INTSTS = 1, the SPI controller will generate a SPI interrupt request.
     * |[11]    |SLV_START_INTSTS|Slave Start Interrupt Status
     * |        |          |It is used to dedicate if a transaction has started in Slave 3-wire mode.
     * |        |          |It is a mutual mirror bit of SPI_CNTRL2[11].
     * |        |          |0 = Slave has not detected any SPI clock transition since the SSTA_INTEN bit was set to 1.
     * |        |          |1 = A transaction has started in Slave 3-wire mode.
     * |        |          |It will be cleared as a transaction is done or by writing 1 to this bit.
     * |[15:12] |RX_FIFO_COUNT|Receive FIFO Data Count (Read Only)
     * |        |          |This bit field indicates the valid data count of receive FIFO buffer.
     * |[16]    |IF        |SPI Unit Transfer Interrupt Flag
     * |        |          |It is a mutual mirror bit of SPI_CNTRL[16].
     * |        |          |0 = No transaction has been finished since this bit was cleared to 0.
     * |        |          |1 = SPI controller has finished one unit transfer.
     * |        |          |Note: This bit will be cleared by writing 1 to itself.
     * |[20]    |TIMEOUT   |Time-Out Interrupt Flag
     * |        |          |0 = No receive FIFO time-out event.
     * |        |          |1 = Receive FIFO buffer is not empty and no read operation on receive FIFO buffer over 64 SPI clock
     * |        |          |period in Master mode or over 576 SPI peripheral clock period in Slave mode.
     * |        |          |When the received FIFO buffer is read by software, the time-out status will be cleared automatically.
     * |        |          |Note: This bit will be cleared by writing 1 to itself.
     * |[24]    |RX_EMPTY  |Receive FIFO Buffer Empty Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_CNTRL[24].
     * |        |          |0 = Receive FIFO buffer is not empty.
     * |        |          |1 = Receive FIFO buffer is empty.
     * |[25]    |RX_FULL   |Receive FIFO Buffer Empty Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_CNTRL[25].
     * |        |          |0 = Receive FIFO buffer is not empty.
     * |        |          |1 = Receive FIFO buffer is empty.
     * |[26]    |TX_EMPTY  |Transmit FIFO Buffer Empty Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_CNTRL[26].
     * |        |          |0 = Transmit FIFO buffer is not empty.
     * |        |          |1 = Transmit FIFO buffer is empty.
     * |[27]    |TX_FULL   |Transmit FIFO Buffer Full Indicator (Read Only)
     * |        |          |It is a mutual mirror bit of SPI_CNTRL[27].
     * |        |          |0 = Transmit FIFO buffer is not full.
     * |        |          |1 = Transmit FIFO buffer is full.
     * |[31:28] |TX_FIFO_COUNT|Transmit FIFO Data Count (Read Only)
     * |        |          |This bit field indicates the valid data count of transmit FIFO buffer.
     */
    __IO uint32_t STATUS;
} SPI_T;

/**
    @addtogroup SPI_CONST SPI Bit Field Definition
    Constant Definitions for SPI Controller
@{ */


/* SPI_CNTRL Bit Field Definitions */
#define SPI_CNTRL_TX_FULL_Pos      27                                     /*!< SPI_T::CNTRL: TX_FULL Position */
#define SPI_CNTRL_TX_FULL_Msk      (1ul << SPI_CNTRL_TX_FULL_Pos)         /*!< SPI_T::CNTRL: TX_FULL Mask     */

#define SPI_CNTRL_TX_EMPTY_Pos     26                                     /*!< SPI_T::CNTRL: TX_EMPTY Position */
#define SPI_CNTRL_TX_EMPTY_Msk     (1ul << SPI_CNTRL_TX_EMPTY_Pos)        /*!< SPI_T::CNTRL: TX_EMPTY Mask     */

#define SPI_CNTRL_RX_FULL_Pos      25                                     /*!< SPI_T::CNTRL: RX_FULL Position */
#define SPI_CNTRL_RX_FULL_Msk      (1ul << SPI_CNTRL_RX_FULL_Pos)         /*!< SPI_T::CNTRL: RX_FULL Mask     */

#define SPI_CNTRL_RX_EMPTY_Pos     24                                     /*!< SPI_T::CNTRL: RX_EMPTY Position */
#define SPI_CNTRL_RX_EMPTY_Msk     (1ul << SPI_CNTRL_RX_EMPTY_Pos)        /*!< SPI_T::CNTRL: RX_EMPTY Mask     */

#define SPI_CNTRL_VARCLK_EN_Pos    23                                     /*!< SPI_T::CNTRL: VARCLK_EN Position */
#define SPI_CNTRL_VARCLK_EN_Msk    (1ul << SPI_CNTRL_VARCLK_EN_Pos)       /*!< SPI_T::CNTRL: VARCLK_EN Mask     */

#define SPI_CNTRL_FIFO_Pos         21                                     /*!< SPI_T::CNTRL: FIFO Position */
#define SPI_CNTRL_FIFO_Msk         (1ul << SPI_CNTRL_FIFO_Pos)            /*!< SPI_T::CNTRL: FIFO Mask     */

#define SPI_CNTRL_REORDER_Pos      19                                     /*!< SPI_T::CNTRL: REORDER Position */
#define SPI_CNTRL_REORDER_Msk      (1ul << SPI_CNTRL_REORDER_Pos)         /*!< SPI_T::CNTRL: REORDER Mask     */

#define SPI_CNTRL_SLAVE_Pos        18                                     /*!< SPI_T::CNTRL: SLAVE Position */
#define SPI_CNTRL_SLAVE_Msk        (1ul << SPI_CNTRL_SLAVE_Pos)           /*!< SPI_T::CNTRL: SLAVE Mask     */

#define SPI_CNTRL_IE_Pos           17                                     /*!< SPI_T::CNTRL: IE Position */
#define SPI_CNTRL_IE_Msk           (1ul << SPI_CNTRL_IE_Pos)              /*!< SPI_T::CNTRL: IE Mask     */

#define SPI_CNTRL_IF_Pos           16                                     /*!< SPI_T::CNTRL: IF Position */
#define SPI_CNTRL_IF_Msk           (1ul << SPI_CNTRL_IF_Pos)              /*!< SPI_T::CNTRL: IF Mask     */

#define SPI_CNTRL_SP_CYCLE_Pos     12                                     /*!< SPI_T::CNTRL: SP_CYCLE Position */
#define SPI_CNTRL_SP_CYCLE_Msk     (0xFul << SPI_CNTRL_SP_CYCLE_Pos)      /*!< SPI_T::CNTRL: SP_CYCLE Mask     */

#define SPI_CNTRL_CLKP_Pos         11                                     /*!< SPI_T::CNTRL: CLKP Position */
#define SPI_CNTRL_CLKP_Msk         (1ul << SPI_CNTRL_CLKP_Pos)            /*!< SPI_T::CNTRL: CLKP Mask     */

#define SPI_CNTRL_LSB_Pos          10                                     /*!< SPI_T::CNTRL: LSB Position */
#define SPI_CNTRL_LSB_Msk          (1ul << SPI_CNTRL_LSB_Pos)             /*!< SPI_T::CNTRL: LSB Mask     */

#define SPI_CNTRL_TX_BIT_LEN_Pos   3                                      /*!< SPI_T::CNTRL: TX_BIT_LEN Position */
#define SPI_CNTRL_TX_BIT_LEN_Msk   (0x1Ful << SPI_CNTRL_TX_BIT_LEN_Pos)   /*!< SPI_T::CNTRL: TX_BIT_LEN Mask     */

#define SPI_CNTRL_TX_NEG_Pos       2                                      /*!< SPI_T::CNTRL: TX_NEG Position */
#define SPI_CNTRL_TX_NEG_Msk       (1ul << SPI_CNTRL_TX_NEG_Pos)          /*!< SPI_T::CNTRL: TX_NEG Mask     */

#define SPI_CNTRL_RX_NEG_Pos       1                                      /*!< SPI_T::CNTRL: RX_NEG Position */
#define SPI_CNTRL_RX_NEG_Msk       (1ul << SPI_CNTRL_RX_NEG_Pos)          /*!< SPI_T::CNTRL: RX_NEG Mask     */

#define SPI_CNTRL_GO_BUSY_Pos      0                                      /*!< SPI_T::CNTRL: GO_BUSY Position */
#define SPI_CNTRL_GO_BUSY_Msk      (1ul << SPI_CNTRL_GO_BUSY_Pos)         /*!< SPI_T::CNTRL: GO_BUSY Mask     */

/* SPI_DIVIDER Bit Field Definitions */
#define SPI_DIVIDER_DIVIDER2_Pos   16                                     /*!< SPI_T::DIVIDER: DIVIDER2 Position */
#define SPI_DIVIDER_DIVIDER2_Msk   (0xFFul << SPI_DIVIDER_DIVIDER2_Pos)   /*!< SPI_T::DIVIDER: DIVIDER2 Mask */

#define SPI_DIVIDER_DIVIDER_Pos    0                                      /*!< SPI_T::DIVIDER: DIVIDER Position */
#define SPI_DIVIDER_DIVIDER_Msk    (0xFFul << SPI_DIVIDER_DIVIDER_Pos)    /*!< SPI_T::DIVIDER: DIVIDER Mask */

/* SPI_SSR Bit Field Definitions */
#define SPI_SSR_LTRIG_FLAG_Pos     5                                 /*!< SPI_T::SSR: LTRIG_FLAG Position */
#define SPI_SSR_LTRIG_FLAG_Msk     (1ul << SPI_SSR_LTRIG_FLAG_Pos)   /*!< SPI_T::SSR: LTRIG_FLAG Mask */

#define SPI_SSR_SS_LTRIG_Pos       4                                 /*!< SPI_T::SSR: SS_LTRIG Position */
#define SPI_SSR_SS_LTRIG_Msk       (1ul << SPI_SSR_SS_LTRIG_Pos)     /*!< SPI_T::SSR: SS_LTRIG Mask */

#define SPI_SSR_AUTOSS_Pos         3                                 /*!< SPI_T::SSR: AUTOSS Position */
#define SPI_SSR_AUTOSS_Msk         (1ul << SPI_SSR_AUTOSS_Pos)       /*!< SPI_T::SSR: AUTOSS Mask */

#define SPI_SSR_SS_LVL_Pos         2                                 /*!< SPI_T::SSR: SS_LVL Position */
#define SPI_SSR_SS_LVL_Msk         (1ul << SPI_SSR_SS_LVL_Pos)       /*!< SPI_T::SSR: SS_LVL Mask */

#define SPI_SSR_SSR_Pos            0                                 /*!< SPI_T::SSR: SSR Position */
#define SPI_SSR_SSR_Msk            (1ul << SPI_SSR_SSR_Pos)          /*!< SPI_T::SSR: SSR Mask */

/* SPI_CNTRL2 Bit Field Definitions */
#define SPI_CNTRL2_BCn_Pos   31                                                      /*!< SPI_T::CNTRL2: BCn Position */
#define SPI_CNTRL2_BCn_Msk   (1ul << SPI_CNTRL2_BCn_Pos)                             /*!< SPI_T::CNTRL2: BCn Mask */

#define SPI_CNTRL2_SS_INT_OPT_Pos   16                                               /*!< SPI_T::CNTRL2: SS_INT_OPT Position */
#define SPI_CNTRL2_SS_INT_OPT_Msk   (1ul << SPI_CNTRL2_SS_INT_OPT_Pos)               /*!< SPI_T::CNTRL2: SS_INT_OPT Mask */

#define SPI_CNTRL2_DUAL_IO_EN_Pos   13                                               /*!< SPI_T::CNTRL2: DUAL_IO_EN Position */
#define SPI_CNTRL2_DUAL_IO_EN_Msk   (1ul << SPI_CNTRL2_DUAL_IO_EN_Pos)               /*!< SPI_T::CNTRL2: DUAL_IO_EN Mask */

#define SPI_CNTRL2_DUAL_IO_DIR_Pos   12                                              /*!< SPI_T::CNTRL2: DUAL_IO_DIR Position */
#define SPI_CNTRL2_DUAL_IO_DIR_Msk   (1ul << SPI_CNTRL2_DUAL_IO_DIR_Pos)             /*!< SPI_T::CNTRL2: DUAL_IO_DIR Mask */

#define SPI_CNTRL2_SLV_START_INTSTS_Pos   11                                         /*!< SPI_T::CNTRL2: SLV_START_INTSTS Position */
#define SPI_CNTRL2_SLV_START_INTSTS_Msk   (1ul << SPI_CNTRL2_SLV_START_INTSTS_Pos)   /*!< SPI_T::CNTRL2: SLV_START_INTSTS Mask */

#define SPI_CNTRL2_SSTA_INTEN_Pos   10                                               /*!< SPI_T::CNTRL2: SSTA_INTEN Position */
#define SPI_CNTRL2_SSTA_INTEN_Msk   (1ul << SPI_CNTRL2_SSTA_INTEN_Pos)               /*!< SPI_T::CNTRL2: SSTA_INTEN Mask */

#define SPI_CNTRL2_SLV_ABORT_Pos    9                                                /*!< SPI_T::CNTRL2: SLV_ABORT Position */
#define SPI_CNTRL2_SLV_ABORT_Msk    (1ul << SPI_CNTRL2_SLV_ABORT_Pos)                /*!< SPI_T::CNTRL2: SLV_ABORT Mask */

#define SPI_CNTRL2_NOSLVSEL_Pos     8                                                /*!< SPI_T::CNTRL2: NOSLVSEL Position */
#define SPI_CNTRL2_NOSLVSEL_Msk     (1ul << SPI_CNTRL2_NOSLVSEL_Pos)                 /*!< SPI_T::CNTRL2: NOSLVSEL Mask */

/* SPI_FIFO_CTL Bit Field Definitions */
#define SPI_FIFO_CTL_TX_THRESHOLD_Pos   28                                         /*!< SPI_T::FIFO_CTL: TX_THRESHOLD Position */
#define SPI_FIFO_CTL_TX_THRESHOLD_Msk   (7ul << SPI_FIFO_CTL_TX_THRESHOLD_Pos)     /*!< SPI_T::FIFO_CTL: TX_THRESHOLD Mask */

#define SPI_FIFO_CTL_RX_THRESHOLD_Pos   24                                         /*!< SPI_T::FIFO_CTL: RX_THRESHOLD Position */
#define SPI_FIFO_CTL_RX_THRESHOLD_Msk   (7ul << SPI_FIFO_CTL_RX_THRESHOLD_Pos)     /*!< SPI_T::FIFO_CTL: RX_THRESHOLD Mask */

#define SPI_FIFO_CTL_TIMEOUT_INTEN_Pos   21                                        /*!< SPI_T::FIFO_CTL: TIMEOUT_INTEN Position */
#define SPI_FIFO_CTL_TIMEOUT_INTEN_Msk   (1ul << SPI_FIFO_CTL_TIMEOUT_INTEN_Pos)   /*!< SPI_T::FIFO_CTL: TIMEOUT_INTEN Mask */

#define SPI_FIFO_CTL_RXOV_INTEN_Pos    6                                           /*!< SPI_T::FIFO_CTL: RXOV_INTEN Position */
#define SPI_FIFO_CTL_RXOV_INTEN_Msk    (1ul << SPI_FIFO_CTL_RXOV_INTEN_Pos)        /*!< SPI_T::FIFO_CTL: RXOV_INTEN Mask */

#define SPI_FIFO_CTL_TX_INTEN_Pos    3                                             /*!< SPI_T::FIFO_CTL: TX_INTEN Position */
#define SPI_FIFO_CTL_TX_INTEN_Msk    (1ul << SPI_FIFO_CTL_TX_INTEN_Pos)            /*!< SPI_T::FIFO_CTL: TX_INTEN Mask */

#define SPI_FIFO_CTL_RX_INTEN_Pos    2                                             /*!< SPI_T::FIFO_CTL: RX_INTEN Position */
#define SPI_FIFO_CTL_RX_INTEN_Msk    (1ul << SPI_FIFO_CTL_RX_INTEN_Pos)            /*!< SPI_T::FIFO_CTL: RX_INTEN Mask */

#define SPI_FIFO_CTL_TX_CLR_Pos     1                                              /*!< SPI_T::FIFO_CTL: TX_CLR Position */
#define SPI_FIFO_CTL_TX_CLR_Msk     (1ul << SPI_FIFO_CTL_TX_CLR_Pos)               /*!< SPI_T::FIFO_CTL: TX_CLR Mask */

#define SPI_FIFO_CTL_RX_CLR_Pos      0                                             /*!< SPI_T::FIFO_CTL: RX_CLR Position */
#define SPI_FIFO_CTL_RX_CLR_Msk      (1ul << SPI_FIFO_CTL_RX_CLR_Pos)              /*!< SPI_T::FIFO_CTL: RX_CLR Mask */

/* SPI_STATUS Bit Field Definitions */
#define SPI_STATUS_TX_FIFO_COUNT_Pos   28                                            /*!< SPI_T::STATUS: TX_FIFO_COUNT Position */
#define SPI_STATUS_TX_FIFO_COUNT_Msk   (0xFul << SPI_STATUS_TX_FIFO_COUNT_Pos)       /*!< SPI_T::STATUS: TX_FIFO_COUNT Mask */

#define SPI_STATUS_TX_FULL_Pos   27                                                  /*!< SPI_T::STATUS: TX_FULL Position */
#define SPI_STATUS_TX_FULL_Msk   (1ul << SPI_STATUS_TX_FULL_Pos)                     /*!< SPI_T::STATUS: TX_FULL Mask */

#define SPI_STATUS_TX_EMPTY_Pos   26                                                 /*!< SPI_T::STATUS: TX_EMPTY Position */
#define SPI_STATUS_TX_EMPTY_Msk   (1ul << SPI_STATUS_TX_EMPTY_Pos)                   /*!< SPI_T::STATUS: TX_EMPTY Mask */

#define SPI_STATUS_RX_FULL_Pos   25                                                  /*!< SPI_T::STATUS: RX_FULL Position */
#define SPI_STATUS_RX_FULL_Msk   (1ul << SPI_STATUS_RX_FULL_Pos)                     /*!< SPI_T::STATUS: RX_FULL Mask */

#define SPI_STATUS_RX_EMPTY_Pos   24                                                 /*!< SPI_T::STATUS: RX_EMPTY Position */
#define SPI_STATUS_RX_EMPTY_Msk   (1ul << SPI_STATUS_RX_EMPTY_Pos)                   /*!< SPI_T::STATUS: RX_EMPTY Mask */

#define SPI_STATUS_TIMEOUT_Pos   20                                                  /*!< SPI_T::STATUS: TIMEOUT Position */
#define SPI_STATUS_TIMEOUT_Msk   (1ul << SPI_STATUS_TIMEOUT_Pos)                     /*!< SPI_T::STATUS: TIMEOUT Mask */

#define SPI_STATUS_IF_Pos   16                                                       /*!< SPI_T::STATUS: IF Position */
#define SPI_STATUS_IF_Msk   (1ul << SPI_STATUS_IF_Pos)                               /*!< SPI_T::STATUS: IF Mask     */

#define SPI_STATUS_RX_FIFO_COUNT_Pos   12                                            /*!< SPI_T::STATUS: RX_FIFO_COUNT Position */
#define SPI_STATUS_RX_FIFO_COUNT_Msk   (0xFul << SPI_STATUS_RX_FIFO_COUNT_Pos)       /*!< SPI_T::STATUS: RX_FIFO_COUNT Mask */

#define SPI_STATUS_SLV_START_INTSTS_Pos   11                                         /*!< SPI_T::STATUS: SLV_START_INTSTS Position */
#define SPI_STATUS_SLV_START_INTSTS_Msk   (1ul << SPI_STATUS_SLV_START_INTSTS_Pos)   /*!< SPI_T::STATUS: SLV_START_INTSTS Mask */

#define SPI_STATUS_TX_INTSTS_Pos   4                                                 /*!< SPI_T::STATUS: TX_INTSTS Position */
#define SPI_STATUS_TX_INTSTS_Msk   (1ul << SPI_STATUS_TX_INTSTS_Pos)                 /*!< SPI_T::STATUS: TX_INTSTS Mask */

#define SPI_STATUS_RX_OVERRUN_Pos   2                                                /*!< SPI_T::STATUS: RX_OVERRUN Position */
#define SPI_STATUS_RX_OVERRUN_Msk   (1ul << SPI_STATUS_RX_OVERRUN_Pos)               /*!< SPI_T::STATUS: RX_OVERRUN Mask */

#define SPI_STATUS_RX_INTSTS_Pos   0                                                 /*!< SPI_T::STATUS: RX_INTSTS Position */
#define SPI_STATUS_RX_INTSTS_Msk   (1ul << SPI_STATUS_RX_INTSTS_Pos)                 /*!< SPI_T::STATUS: RX_INTSTS Mask */
/*@}*/ /* end of group SPI_CONST */
/*@}*/ /* end of group SPI */




/*---------------------- System Manger Controller -------------------------*/
/**
    @addtogroup SYS System Manger Controller (SYS)
    Memory Mapped Structure for SYS Controller
@{ */
typedef struct
{

    /**
     * PDID
     * ===================================================================================================
     * Offset: 0x00  Part Device Identification Number Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |PDID      |Part Device Identification Number
     * |        |          |This register reflects device part number code.
     * |        |          |Software can read this register to identify which device is used.
     */
    __I uint32_t PDID;

    /**
     * RSTSRC
     * ===================================================================================================
     * Offset: 0x04  System Reset Source Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RSTS_POR  |Power-on Reset Flag
     * |        |          |The RSTS_POR flag is set by the "reset signal" from the Power-On Reset (POR) controller or bit CHIP_RST (IPRSTC1[0]) to indicate the previous reset source.
     * |        |          |0 = No reset from POR or CHIP_RST (IPRSTC1[0]).
     * |        |          |1 = Power-on Reset (POR) or CHIP_RST (IPRSTC1[0]) had issued the reset signal to reset the system.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[1]     |RSTS_RESET|Reset Pin Reset Flag
     * |        |          |The RSTS_RESET flag is set by the "Reset Signal" from the /RESET pin to indicate the previous reset source.
     * |        |          |0 = No reset from /RESET pin.
     * |        |          |1 = The Pin /RESET had issued the reset signal to reset the system.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[2]     |RSTS_WDT  |Watchdog Reset Flag
     * |        |          |The RSTS_WDT flag is set by The "Reset Signal" from the Watchdog Timer to indicate the previous reset source
     * |        |          |0 = No reset from watchdog timer.
     * |        |          |1 = The watchdog timer had issued the reset signal to reset the system.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[3]     |RSTS_LVR  |Low Voltage Reset Flag
     * |        |          |The RSTS_LVR flag Is Set By The "Reset Signal" From The Low-Voltage-Reset Controller To Indicate The Previous Reset Source
     * |        |          |0 = No reset from LVR.
     * |        |          |1 = The LVR controller had issued the reset signal to reset the system.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[4]     |RSTS_BOD  |Brown-out Detector Reset Flag
     * |        |          |The RSTS_BOD flag is set by the "Reset Signal" from the Brown-Out Detector to indicate the previous reset source.
     * |        |          |0 = No reset from BOD.
     * |        |          |1 = The BOD had issued the reset signal to reset the system.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[5]     |RSTS_SYS  |SYS Reset Flag
     * |        |          |The RSTS_SYS flag is set by the "Reset Signal" from the Cortex-M0 kernel to indicate the previous reset source.
     * |        |          |0 = No reset from Cortex-M0.
     * |        |          |1 = The Cortex-M0 had issued the reset signal to reset the system by writing 1 to bit SYSRESETREQ (AIRCR[2], Application Interrupt and Reset Control Register, address = 0xE000ED0C) in system control registers of Cortex-M0 kernel.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[7]     |RSTS_CPU  |CPU Reset Flag
     * |        |          |The RSTS_CPU flag is set by hardware if software writes CPU_RST (IPRSTC1[1]) 1 to reset Cortex-M0 CPU kernel and flash. Memory Controller (FMC)
     * |        |          |0 = No reset from CPU.
     * |        |          |1 = Cortex-M0 CPU kernel and FMC are reset by software setting CPU_RST(IPRSTC1[1]) to 1.
     * |        |          |Note: Write 1 to clear this bit to 0.
     */
    __IO uint32_t RSTSRC;

    /**
     * IPRSTC1
     * ===================================================================================================
     * Offset: 0x08  IP Reset Control Register 1
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CHIP_RST  |CHIP One-Shot Reset (Write Protect)
     * |        |          |Setting this bit will reset the whole chip, including CPU kernel and all peripherals, and this bit will automatically return to 0 after the 2 clock cycles.
     * |        |          |The CHIP_RST is the same as the POR reset, all the chip controllers are reset and the chip setting from flash are also reload.
     * |        |          |0 = CHIP normal operation.
     * |        |          |1 = CHIP one-shot reset.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[1]     |CPU_RST   |CPU Kernel One-Shot Reset (Write Protect)
     * |        |          |Setting this bit will only reset the CPU kernel and Flash Memory Controller(FMC), and this bit will automatically return 0 after the two clock cycles
     * |        |          |0 = CPU normal operation.
     * |        |          |1 = CPU one-shot reset.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     */
    __IO uint32_t IPRSTC1;

    /**
     * IPRSTC2
     * ===================================================================================================
     * Offset: 0x0C  IP Reset Control Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1]     |GPIO_RST  |GPIO Controller Reset
     * |        |          |0 = GPIO controller normal operation.
     * |        |          |1 = GPIO controller reset.
     * |[2]     |TMR0_RST  |Timer0 Controller Reset
     * |        |          |0 = Timer0 controller normal operation.
     * |        |          |1 = Timer0 controller reset.
     * |[3]     |TMR1_RST  |Timer1 Controller Reset
     * |        |          |0 = Timer1 controller normal operation.
     * |        |          |1 = Timer1 controller reset.
     * |[4]     |TMR2_RST  |Timer2 Controller Reset
     * |        |          |0 = Timer2 controller normal operation.
     * |        |          |1 = Timer2 controller reset.
     * |[5]     |TMR3_RST  |Timer3 Controller Reset
     * |        |          |0 = Timer3 controller normal operation.
     * |        |          |1 = Timer3 controller reset.
     * |[8]     |I2C0_RST  |I2C0 Controller Reset
     * |        |          |0 = I2C0 controller normal operation.
     * |        |          |1 = I2C0 controller reset.
     * |[9]     |I2C1_RST  |I2C1 Controller Reset
     * |        |          |0 = I2C1 controller normal operation.
     * |        |          |1 = I2C1 controller reset.
     * |[12]    |SPI0_RST  |SPI0 Controller Reset
     * |        |          |0 = SPI0 controller normal operation.
     * |        |          |1 = SPI0 controller reset.
     * |[16]    |UART0_RST |UART0 Controller Reset
     * |        |          |0 = UART0 controller normal operation.
     * |        |          |1 = UART0 controller reset.
     * |[17]    |UART1_RST |UART1 Controller Reset
     * |        |          |0 = UART1 controller normal operation.
     * |        |          |1 = UART1 controller reset.
     * |[18]    |UART2_RST |UART2 Controller Reset
     * |        |          |0 = UART2 controller normal operation.
     * |        |          |1 = UART2 controller reset.
     * |[24]    |CAN0_RST  |CAN0 Controller Reset
     * |        |          |0 = CAN0 controller normal operation.
     * |        |          |1 = CAN0 controller reset.     
     * |[28]    |ADC_RST   |ADC Controller Reset
     * |        |          |0 = ADC controller normal operation.
     * |        |          |1 = ADC controller reset.
     */
    __IO uint32_t IPRSTC2;

    /**
     * IPRSTC3
     * ===================================================================================================
     * Offset: 0x10  Peripheral Reset Control Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[8]     |UART3_RST |UART3 Controller Reset
     * |        |          |0 = UART3 controller normal operation.
     * |        |          |1 = UART3 controller reset.
     * |[9]     |UART4_RST |UART4 Controller Reset
     * |        |          |0 = UART4 controller normal operation.
     * |        |          |1 = UART4 controller reset.
     * |[10]    |UART5_RST |UART5 Controller Reset
     * |        |          |0 = UART5 controller normal operation.
     * |        |          |1 = UART5 controller reset.
     * |[16]    |PWM0_RST  |PWM0 Controller Reset
     * |        |          |0 = PWM0 controller normal operation.
     * |        |          |1 = PWM0 controller reset.
     * |[17]    |PWM1_RST  |PWM1 Controller Reset
     * |        |          |0 = PWM1 controller normal operation.
     * |        |          |1 = PWM1 controller reset.
     * |[18]    |BPWM0_RST |BPWM0 Controller Reset
     * |        |          |0 = BPWM0 controller normal operation.
     * |        |          |1 = BPWM0 controller reset.
     * |[19]    |BPWM1_RST |BPWM1 Controller Reset
     * |        |          |0 = BPWM1 controller normal operation.
     * |        |          |1 = BPWM1 controller reset.
     */
    __IO uint32_t IPRSTC3;

    uint32_t RESERVE0;

    /**
     * BODCR
     * ===================================================================================================
     * Offset: 0x18  Brown-out Detector Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |BOD_EN    |Brown-Out Detector Enable Bit (Write Protect)
     * |        |          |The default value is set by flash memory controller user configuration register CBODEN (CONFIG0[23]) bit.
     * |        |          |0 = Brown-out Detector function Disabled.
     * |        |          |1 = Brown-out Detector function Enabled.
     * |        |          |Note: This bit is the protected bit.
     * |        |          |It means programming this needs to write "59h", "16h", "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[2:1]   |BOD_VL    |Brown-Out Detector Threshold Voltage Selection (Write Protect)
     * |        |          |The default value is set by flash memory controller user configuration register CBOV (CONFIG0[22:21]) bit.
     * |        |          |00 = Brown-out voltage is 2.2V.
     * |        |          |01 = Brown-out voltage is 2.7V.
     * |        |          |10 = Brown-out voltage is 3.7V.
     * |        |          |11 = Brown-out voltage is 4.4V.
     * |        |          |Note: This bit is the protected bit.
     * |        |          |It means programming this needs to write "59h", "16h", "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[3]     |BOD_RSTEN |Brown-Out Reset Enable Bit (Write Protect)
     * |        |          |0 = Brown-out "INTERRUPT" function Enabled.
     * |        |          |1 = Brown-out "RESET" function Enabled.
     * |        |          |While the Brown-out Detector function is enabled (BOD_EN high) and BOD reset function is enabled (BOD_RSTEN high), BOD will assert a signal to reset chip when the detected voltage is lower than the threshold (BOD_OUT high).
     * |        |          |Note1: While the BOD function is enabled (BOD_EN high) and BOD interrupt function is enabled (BOD_RSTEN low), BOD will assert an interrupt if BOD_OUT is high.
     * |        |          |BOD interrupt will keep till to the BOD_EN set to 0.
     * |        |          |BOD interrupt can be blocked by disabling the NVIC BOD interrupt or disabling BOD function (set BOD_EN low).
     * |        |          |Note2: The default value is set by flash controller user configuration register CBORST (CONFIG0[20]) bit.
     * |        |          |Note3: This bit is the protected bit.
     * |        |          |It means programming this needs to write "59h", "16h", "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[4]     |BOD_INTF  |Brown-Out Detector Interrupt Flag
     * |        |          |0 = Brown-out Detector does not detect any voltage draft at VDD down through or up through the voltage of BOD_VL setting.
     * |        |          |1 = When Brown-out Detector detects the VDD is dropped down through the voltage of BOD_VL setting or the VDD is raised up through the voltage of BOD_VL setting, this bit is set to 1 and the Brown-out interrupt is requested if Brown-out interrupt is enabled.
     * |        |          |Note: Write 1 to clear this bit to 0.
     * |[5]     |BOD_LPM   |Brown-Out Detector Low Power Mode (Write Protect)
     * |        |          |0 = BOD operated in Normal mode (default).
     * |        |          |1 = BOD Low Power mode Enabled.
     * |        |          |Note1: The BOD consumes about 100 uA in Normal mode, and the low power mode can reduce the current to about 1/10 but slow the BOD response.
     * |        |          |Note2: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[6]     |BOD_OUT   |Brown-Out Detector Output Status
     * |        |          |0 = Brown-out Detector output status is 0.
     * |        |          |It means the detected voltage is higher than BOD_VL setting or BOD_EN is 0.
     * |        |          |1 = Brown-out Detector output status is 1.
     * |        |          |It means the detected voltage is lower than BOD_VL setting.
     * |        |          |If the BOD_EN is 0, BOD function disabled, this bit always responds to 0.
     * |[7]     |LVR_EN    |Low Voltage Reset Enable Bit (Write Protect)
     * |        |          |The LVR function reset the chip when the input power voltage is lower than LVR circuit setting.
     * |        |          |LVR function is enabled by default.
     * |        |          |0 = Low Voltage Reset function Disabled.
     * |        |          |1 = Low Voltage Reset function Enabled - After enabling the bit, the LVR function will be active with 100us delay for LVR output stable (default).
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[10:8]  |BODDGSEL  |Brown-Out Detector Output De-Glitch Time Select (Write Protect)
     * |        |          |000 = BOD output is sampled by RC10K clock.
     * |        |          |001 = 4 system clock (HCLK).
     * |        |          |010 = 8 system clock (HCLK).
     * |        |          |011 = 16 system clock (HCLK).
     * |        |          |100 = 32 system clock (HCLK).
     * |        |          |101 = 64 system clock (HCLK).
     * |        |          |110 = 128 system clock (HCLK).
     * |        |          |111 = 256 system clock (HCLK).
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     * |[14:12] |LVRDGSEL  |LVR Output De-Glitch Time Select (Write Protect)
     * |        |          |000 = Without de-glitch function.
     * |        |          |001 = 4 system clock (HCLK).
     * |        |          |010 = 8 system clock (HCLK).
     * |        |          |011 = 16 system clock (HCLK).
     * |        |          |100 = 32 system clock (HCLK).
     * |        |          |101 = 64 system clock (HCLK).
     * |        |          |110 = 128 system clock (HCLK).
     * |        |          |111 = 256 system clock (HCLK).
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     */
    __IO uint32_t BODCR;

    uint32_t RESERVE1[2];

    /**
     * PORCR
     * ===================================================================================================
     * Offset: 0x24  Power-on-Reset Controller Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |POR_DIS_CODE|Power-On-Reset Enable Control (Write Protect)
     * |        |          |When powered on, the POR circuit generates a reset signal to reset the whole chip function, but noise on the power may cause the POR active again.
     * |        |          |User can disable internal POR circuit to avoid unpredictable noise to cause chip reset by writing 0x5AA5 to this field.
     * |        |          |The POR function will be active again when this field is set to another value or chip is reset by other reset source, including:
     * |        |          |/RESET, Watchdog, LVR reset, BOD reset, ICE reset command and the software-chip reset function
     * |        |          |Note: This bit is the protected bit. It means programming this needs to write "59h", "16h", "88h" to address 0x5000_0100 to disable register protection. Refer to the register REGWRPROT at address GCR_BA+0x100.
     */
    __IO uint32_t PORCR;

    /**
     * VREFCR
     * ===================================================================================================
     * Offset: 0x28  VREF Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |ADC_VREFSEL|ADC VREF Path Control (Write Protect)
     * |        |          |0 = ADC VREF is from VREF pin.
     * |        |          |1 = ADC VREF is from AVDD.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection. Refer to the register REGWRPROT at address GCR_BA+0x100.
     */
    __IO uint32_t VREFCR;

    uint32_t RESERVE2[1];

    /**
     * GPA_MFP
     * ===================================================================================================
     * Offset: 0x30  GPIOA Multiple Function and Input Type Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GPA_MFP0  |PA.0 Pin Function Selection
     * |        |          |Bits PA0_PWM04 (ALT_MFP3[4]), PA0_UR5TXD (ALT_MFP4[7]), PA0_I2C1SCL (ALT_MFP4[12]) and GPA_MFP0 determine the PA.0 function.
     * |        |          |(PA0_PWM04, PA0_UR5TXD, PA0_I2C1SCL, GPA_MFP0) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC0 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SCL function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_TXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH4 function is selected.
     * |[1]     |GPA_MFP1  |PA.1 Pin Function Selection
     * |        |          |Bits PA1_PWM05 (ALT_MFP3[5]), PA1_UR5RXD (ALT_MFP4[6]), PA1_I2C1SDA (ALT_MFP4[13]) and GPA_MFP1 determine the PA.1 function.
     * |        |          |(PA1_PWM05, PA1_UR5RXD, PA1_I2C1SDA, GPA_MFP1) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC1 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SDA function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_RXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH5 function is selected.
     * |[2]     |GPA_MFP2  |PA.2 Pin Function Selection
     * |        |          |Bits PA2_PWM10 (ALT_MFP3[6]), PA2_UR3TXD (ALT_MFP4[3]) and GPA_MFP2 determine the PA.2 function.
     * |        |          |(PA2_PWM10, PA2_UR3TXD, GPA_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = ADC2 function is selected.
     * |        |          |(0, 1, 1) = UART3_TXD function is selected.
     * |        |          |(1, 0, 1) = PWM1_CH0 function is selected.
     * |[3]     |GPA_MFP3  |PA.3 Pin Function Selection
     * |        |          |Bits PA3_PWM11 (ALT_MFP3[7]), PA3_UR3RXD (ALT_MFP4[2]) and GPA_MFP3 determine the PA.3 function.
     * |        |          |(PA3_PWM11, PA3_UR3RXD, GPA_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = ADC3 function is selected.
     * |        |          |(0, 1, 1) = UART3_RXD function is selected.
     * |        |          |(1, 0, 1) = PWM1_CH1 function is selected.
     * |[4]     |GPA_MFP4  |PA.4 Pin Function Selection
     * |        |          |Bit GPA_MFP4 determines the PA.4 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = ADC4 function is selected.
     * |[5]     |GPA_MFP5  |PA.5 Pin Function Selection
     * |        |          |Bits PA5_UR3RXD (ALT_MFP4[4]) and GPA_MFP5 determine the PA.5 function.
     * |        |          |(PA5_UR3RXD, GPA_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = ADC5 function is selected.
     * |        |          |(1, 1) = UART3_RXD function is selected.
     * |[6]     |GPA_MFP6  |PA.6 Pin Function Selection
     * |        |          |Bits PA6_UR3TXD (ALT_MFP4[5]) and GPA_MFP6 determine the PA.6 function.
     * |        |          |(PA6_UR3TXD, GPA_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = ADC6 function is selected.
     * |        |          |(1, 1) = UART3_TXD function is selected.
     * |[7]     |GPA_MFP7  |PA.7 Pin Function Selection
     * |        |          |Bits PA7_VREF (ALT_MFP4[14]) and GPA_MFP7 determine the PA.7 function.
     * |        |          |(PA7_VREF, GPA_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = ADC7 function is selected.
     * |        |          |(1, 1) = Vref function is selected.
     * |[8]     |GPA_MFP8  |PA.8 Pin Function Selection
     * |        |          |Bits PA8_UR1RTS (ALT_MFP4[0]) and GPA_MFP8 determine the PA.8 function.
     * |        |          |(PA8_UR1RTS, GPA_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 1) = UART1_nRTS function is selected.
     * |[9]     |GPA_MFP9  |PA.9 Pin Function Selection
     * |        |          |Bits PA9_UR1CTS (ALT_MFP4[1]) and GPA_MFP9 determine the PA.9 function.
     * |        |          |(PA9_UR1CTS, GPA_MFP9) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 1) = UART1_nCTS function is selected.
     * |[10]    |GPA_MFP10 |PA.10 Pin Function Selection
     * |        |          |Bits PA10_PWM12 (ALT_MFP3[8]) and GPA_MFP10 determine the PA.10 function.
     * |        |          |(PA10_PWM12, GPA_MFP10) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C1_SDA function is selected.
     * |        |          |(1, 1) = PWM1_CH2 function is selected.
     * |[11]    |GPA_MFP11 |PA.11 Pin Function Selection
     * |        |          |Bits PA11_PWM13 (ALT_MFP3[9]) and GPA_MFP11 determine the PA.11 function.
     * |        |          |(PA11_PWM13, GPA_MFP11) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C1_SCL function is selected.
     * |        |          |(1, 1) = PWM1_CH3 function is selected.
     * |[12]    |GPA_MFP12 |PA.12 Pin Function Selection
     * |        |          |Bits PA12_UR5RXD (ALT_MFP4[8]) and GPA_MFP12 determine the PA.12 function.
     * |        |          |(PA12_UR5RXD, GPA_MFP12) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = PWM0_CH0 function is selected.
     * |        |          |(1, 1) = UART5_RXD function is selected.
     * |[13]    |GPA_MFP13 |PA.13 Pin Function Selection
     * |        |          |Bits PA13_UR5TXD (ALT_MFP4[9]) and GPA_MFP13 determine the PA.13 function.
     * |        |          |(PA13_UR5TXD, GPA_MFP13) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = PWM0_CH1 function is selected.
     * |        |          |(1, 1) = UART5_TXD function is selected.
     * |[14]    |GPA_MFP14 |PA.14 Pin Function Selection
     * |        |          |Bit GPA_MFP14 determines the PA.14 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = PWM0_CH2 function is selected.
     * |[15]    |GPA_MFP15 |PA.15 Pin Function Selection
     * |        |          |Bit GPA_MFP15 determines the PA.15 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = PWM0_CH3 function is selected.
     * |[31:16] |GPA_TYPEn |Trigger Function Selection
     * |        |          |0 = GPIOA[15:0] I/O input Schmitt Trigger function Disabled.
     * |        |          |1 = GPIOA[15:0] I/O input Schmitt Trigger function Enabled.
     */
    __IO uint32_t GPA_MFP;

    /**
     * GPB_MFP
     * ===================================================================================================
     * Offset: 0x34  GPIOB Multiple Function and Input Type Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GPB_MFP0  |PB.0 Pin Function Selection
     * |        |          |Bit GPB_MFP0 determines the PB.0 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = UART0_RXD function is selected.
     * |[1]     |GPB_MFP1  |PB.1 Pin Function Selection
     * |        |          |Bit GPB_MFP1 determines the PB.1 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = UART0_TXD function is selected.
     * |[2]     |GPB_MFP2  |PB.2 Pin Function Selection
     * |        |          |Bits PB2_TM2 (ALT_MFP2[4]), PB2_PWM1BK1 (ALT_MFP3[31]), PB2_T2EX (ALT_MFP[26]) and GPB_MFP2 determine the PB.2 function.
     * |        |          |(PB2_TM2, PB2_PWM1BK1, PB2_T2EX, GPB_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nRTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM2_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE1 function is selected.
     * |        |          |(1, 0, 0, 1) = TM2 function is selected.
     * |[3]     |GPB_MFP3  |PB.3 Pin Function Selection
     * |        |          |Bits PB3_TM3 (ALT_MFP2[5]), PB3_PWM1BK0 (ALT_MFP3[30]), PB3_T3EX (ALT_MFP[27]) and GPB_MFP3 determine the PB.3 function.
     * |        |          |(PB3_TM3, PB3_PWM1BK0, PB3_T3EX, GPB_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nCTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM3_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE0 function is selected.
     * |        |          |(1, 0, 0, 1) = TM3 function is selected.
     * |[4]     |GPB_MFP4  |PB.4 Pin Function Selection
     * |        |          |Bit GPB_MFP4 determines the PB.4 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = UART1_RXD function is selected.
     * |[5]     |GPB_MFP5  |PB 5 Pin Function Selection
     * |        |          |Bit GPB_MFP5 determines the PB.5 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = UART1_TXD function is selected.
     * |[6]     |GPB_MFP6  |PB.6 Pin Function Selection
     * |        |          |Bit GPB_MFP6 determines the PB.6 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = UART1_nRTS function is selected.
     * |[7]     |GPB_MFP7  |PB.7 Pin Function Selection
     * |        |          |Bit GPB_MFP7 determines the PB.7 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = UART1_nCTS function is selected.
     * |[8]     |GPB_MFP8  |PB.8 Pin Function Selection
     * |        |          |Bits PB8_BPWM12 (ALT_MFP3[20]), PB8_CLKO (ALT_MFP[29]) and GPB_MFP8 determine the PB.8 function.
     * |        |          |(PB8_BPWM12, PB8_CLKO, GPB_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = TM0 function is selected.
     * |        |          |(0, 1, 1) = CLKO function is selected.
     * |        |          |(1, 0, 1) = BPWM1_CH2 function is selected.
     * |[9]     |GPB_MFP9  |PB.9 Pin Function Selection
     * |        |          |Bit GPB_MFP9 determines the PB.9 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = TM1 function is selected.
     * |[10]    |GPB_MFP10 |PB.10 Pin Function Selection
     * |        |          |Bit GPB_MFP10 determines the PB.10 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = TM2 function is selected.
     * |[11]    |GPB_MFP11 |PB.11 Pin Function Selection
     * |        |          |Bits PB11_PWM04 (ALT_MFP3[24]) and GPB_MFP11 determine the PB.11 function.
     * |        |          |(PB11_PWM04, GPB_MFP11) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = TM3 function is selected.
     * |        |          |(1, 1) = PWM0_CH4 function is selected.
     * |[12]    |GPB_MFP12 |PB.12 Pin Function Selection
     * |        |          |Bits PB12_BPWM13 (ALT_MFP3[21]) and GPB_MFP12 determine the PB.12 function.
     * |        |          |(PB12_BPWM13, GPB_MFP12) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CLKO function is selected.
     * |        |          |(1, 1) = BPWM1_CH3 function is selected.
     * |[14]    |GPB_MFP14 |PB.14 Pin Function Selection
     * |        |          |Bit GPB_MFP14 determines the PB.14 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = INT0 function is selected.
     * |[15]    |GPB_MFP15 |PB.15 Pin Function Selection
     * |        |          |Bits PB15_BPWM15 (ALT_MFP3[23]), PB15_T0EX (ALT_MFP[24]), PB15_TM0 (ALT_MFP2[2]) and GPB_MFP15 determine the PB.15 function.
     * |        |          |(PB15_BPWM15, PB15_T0EX, PB15_TM0, GPB_MFP15) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = INT1 function is selected.
     * |        |          |(0, 0, 1, 1) = TM0 function is selected.
     * |        |          |(0, 1, 0, 1) = TM0_EXT function is selected.
     * |        |          |(1, 0, 1, 1) = BPWM1_CH5 function is selected.
     * |[31:16] |GPB_TYPEn |Trigger Function Selection
     * |        |          |0 = GPIOB[15:0] I/O input Schmitt Trigger function Disabled.
     * |        |          |1 = GPIOB[15:0] I/O input Schmitt Trigger function Enabled.
     */
    __IO uint32_t GPB_MFP;

    /**
     * GPC_MFP
     * ===================================================================================================
     * Offset: 0x38  GPIOC Multiple Function and Input Type Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GPC_MFP0  |PC.0 Pin Function Selection
     * |        |          |Bits PC0_BPWM00 (ALT_MFP3[12]) and GPC_MFP0 determine the PC.0 function.
     * |        |          |(PC0_BPWM00, GPC_MFP0) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_SS0 function is selected.
     * |        |          |(1, 1) = BPWM0_CH0 function is selected.
     * |[1]     |GPC_MFP1  |PC.1 Pin Function Selection
     * |        |          |Bits PC1_BPWM01 (ALT_MFP3[13]) and GPC_MFP1 determine the PC.1 function.
     * |        |          |(PC1_BPWM01, GPC_MFP1) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_CLK function is selected.
     * |        |          |(1, 1) = BPWM0_CH1 function is selected.
     * |[2]     |GPC_MFP2  |PC.2 Pin Function Selection
     * |        |          |Bits PC2_BPWM02 (ALT_MFP3[14]) and GPC_MFP2 determine the PC.2 function.
     * |        |          |(PC2_BPWM02, GPC_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_MISO0 function is selected.
     * |        |          |(1, 1) = BPWM0_CH2 function is selected.
     * |[3]     |GPC_MFP3  |PC.3 Pin Function Selection
     * |        |          |Bits PC3_BPWM03 (ALT_MFP3[15]) and GPC_MFP3 determine the PC.3 function.
     * |        |          |(PC3_BPWM03, GPC_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_MOSI0 function is selected.
     * |        |          |(1, 1) = BPWM0_CH3 function is selected.
     * |[6]     |GPC_MFP6  |PC.6 Pin Function Selection
     * |        |          |Bits PC6_PWM0BK0 (ALT_MFP3[28]), PC6_I2C0SDA (ALT_MFP4[10]) and GPC_MFP6 determine the PC.6 function.
     * |        |          |(PC6_PWM0BK0, PC6_I2C0SDA, GPC_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = UART4_TXD function is selected.
     * |        |          |(0, 1, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 0, 1) = PWM0_BRAKE0 function is selected.
     * |[7]     |GPC_MFP7  |PC.7 Pin Function Selection
     * |        |          |Bits PC7_PWM0BK1 (ALT_MFP3[29]), PC7_I2C0SCL (ALT_MFP4[11]) and GPC_MFP7 determine the PC.7 function.
     * |        |          |(PC7_PWM0BK1, PC7_I2C0SCL, GPC_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = UART4_RXD function is selected.
     * |        |          |(0, 1, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 0, 1) = PWM0_BRAKE1 function is selected.
     * |[8]     |GPC_MFP8  |PC.8 Pin Function Selection
     * |        |          |Bit GPC_MFP8 determines the PC.8 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = PWM0_BRAKE0 function is selected.
     * |[9]     |GPC_MFP9  |PC.9 Pin Function Selection
     * |        |          |Bit GPC_MFP9 determines the PC.9 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = PWM0_BRAKE1 function is selected.
     * |[10]    |GPC_MFP10 |PC.10 Pin Function Selection
     * |        |          |Bit GPC_MFP10 determines the PC.10 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = PWM1_BRAKE0 function is selected.
     * |[11]    |GPC_MFP11 |PC.11 Pin Function Selection
     * |        |          |Bit GPC_MFP11 determines the PC.11 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = PWM1_BRAKE1 function is selected.
     * |[31:16] |GPC_TYPEn |Trigger Function Selection
     * |        |          |0 = GPIOC[15:0] I/O input Schmitt Trigger function Disabled.
     * |        |          |1 = GPIOC[15:0] I/O input Schmitt Trigger function Enabled.
     */
    __IO uint32_t GPC_MFP;

    /**
     * GPD_MFP
     * ===================================================================================================
     * Offset: 0x3C  GPIOD Multiple Function and Input Type Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[6]     |GPD_MFP6  |PD.6 Pin Function Selection
     * |        |          |Bits PD6_BPWM11 (ALT_MFP3[19]) and GPD_MFP6 determine the PD.6 function.
     * |        |          |(PD6_BPWM11, GPD_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CAN0_RXD function is selected.
     * |        |          |(1, 1) = BPWM1_CH1 function is selected.
     * |[7]     |GPD_MFP7  |PD.7 Pin Function Selection
     * |        |          |Bits PD7_BPWM10 (ALT_MFP3[18]) and GPD_MFP7 determine the PD.7 function.
     * |        |          |(PD7_BPWM10, GPD_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CAN0_TXD function is selected.
     * |        |          |(1, 1) = BPWM1_CH0 function is selected.
     * |[14]    |GPD_MFP14 |PD.14 Pin Function Selection
     * |        |          |Bits PD14_BPWM05 (ALT_MFP3[17]) and GPD_MFP14 determine the PD.14 function.
     * |        |          |(PD14_BPWM05, GPD_MFP14) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = UART2_RXD function is selected.
     * |        |          |(1, 1) = BPWM0_CH5 function is selected.
     * |[15]    |GPD_MFP15 |PD.15 Pin Function Selection
     * |        |          |Bits PD15_BPWM04 (ALT_MFP3[16]) and GPD_MFP15 determine the PD.15 function.
     * |        |          |(PD15_BPWM04, GPD_MFP15) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = UART2_TXD function is selected.
     * |        |          |(1, 1) = BPWM0_CH4 function is selected.
     * |[31:16] |GPD_TYPEn |Trigger Function Selection
     * |        |          |0 = GPIOD[15:0] I/O input Schmitt Trigger function Disabled.
     * |        |          |1 = GPIOD[15:0] I/O input Schmitt Trigger function Enabled.
     */
    __IO uint32_t GPD_MFP;

    /**
     * GPE_MFP
     * ===================================================================================================
     * Offset: 0x40  GPIOE Multiple Function and Input Type Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5]     |GPE_MFP5  |PE.5 Pin Function Selection
     * |        |          |Bits PE5_T1EX (ALT_MFP[25]), PE5_TM1 (ALT_MFP2[3]) and GPE_MFP5 determine the PE.5 function.
     * |        |          |(PE5_T1EX, PE5_TM1, GPE_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = PWM0_CH5 function is selected.
     * |        |          |(0, 1, 1) = TM1 function is selected.
     * |        |          |(1, 0, 1) = TM1_EXT function is selected.
     * |[21]    |GPE_TYPE5 |Trigger Function Selection
     * |        |          |0 = GPIOE[5] I/O input Schmitt Trigger function Disabled.
     * |        |          |1 = GPIOE[5] I/O input Schmitt Trigger function Enabled.
     */
    __IO uint32_t GPE_MFP;

    /**
     * GPF_MFP
     * ===================================================================================================
     * Offset: 0x44  GPIOF Multiple Function and Input Type Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GPF_MFP0  |PF.0 Pin Function Selection
     * |        |          |Bit GPF_MFP0 determine the PF.0 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = XT1_OUT function is selected.
     * |        |          |Note: This bit is read only and is decided by user configuration CGPFMFP (CONFIG0[27]).
     * |[1]     |GPF_MFP1  |PF.1 Pin Function Selection
     * |        |          |Bit GPF_MFP1 determine the PF.1 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = XT1_IN function is selected.
     * |        |          |Note: This bit is read only and is decided by user configuration CGPFMFP (CONFIG0[27]).
     * |[4]     |GPF_MFP4  |PF.4 Pin Function Selection
     * |        |          |Bits PF4_PWM14 (ALT_MFP3[10]) and GPF_MFP4 determine the PF.4 function.
     * |        |          |(PF4_PWM14, GPF_MFP4) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 1) = PWM1_CH4 function is selected.
     * |[5]     |GPF_MFP5  |PF.5 Pin Function Selection
     * |        |          |Bits PF5_PWM15 (ALT_MFP3[11]) and GPF_MFP5 determine the PF.5 function.
     * |        |          |(PF5_PWM15, GPF_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 1) = PWM1_CH5 function is selected.
     * |[6]     |GPF_MFP6  |PF.6 Pin Function Selection
     * |        |          |Bit GPF_MFP6 determines the PF.6 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = ICE_CLK function is selected.
     * |[7]     |GPF_MFP7  |PF.7 Pin Function Selection
     * |        |          |Bit GPF_MFP7 determines the PF.7 function.
     * |        |          |0 = GPIO function is selected.
     * |        |          |1 = ICE_DAT function is selected.
     * |[8]     |GPF_MFP8  |PF.8 Pin Function Selection
     * |        |          |Bits PF8_BPWM14 (ALT_MFP3[22]) and GPF_MFP8 determine the PF.8 function.
     * |        |          |(PF8_BPWM14, GPF_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CLKO function is selected.
     * |        |          |(1, 0) = BPWM1_CH4 function is selected.
     * |[24:16] |GPF_TYPEn |Trigger Function Selection
     * |        |          |0 = GPIOF[8:0] I/O input Schmitt Trigger function Disabled.
     * |        |          |1 = GPIOF[8:0] I/O input Schmitt Trigger function Enabled.
     */
    __IO uint32_t GPF_MFP;

    uint32_t RESERVE3[2];

    /**
     * ALT_MFP
     * ===================================================================================================
     * Offset: 0x50  Alternative Multiple Function Pin Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[24]    |PB15_T0EX |PB.15 Pin Alternative Function Selection
     * |        |          |Bits PB15_BPWM15 (ALT_MFP3[23]), PB15_T0EX (ALT_MFP[24]), PB15_TM0 (ALT_MFP2[2]) and GPB_MFP15 determine the PB.15 function.
     * |        |          |(PB15_BPWM15, PB15_T0EX, PB15_TM0, GPB_MFP15) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = INT1 function is selected.
     * |        |          |(0, 0, 1, 1) = TM0 function is selected.
     * |        |          |(0, 1, 0, 1) = TM0_EXT function is selected.
     * |        |          |(1, 0, 1, 1) = BPWM1_CH5 function is selected.
     * |[25]    |PE5_T1EX  |PE.5 Pin Alternative Function Selection
     * |        |          |Bits PE5_T1EX (ALT_MFP[25]), PE5_TM1 (ALT_MFP2[3]) and GPE_MFP5 determine the PE.5 function.
     * |        |          |(PE5_T1EX, PE5_TM1, GPE_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = PWM0_CH5 function is selected.
     * |        |          |(0, 1, 1) = TM1 function is selected.
     * |        |          |(1, 0, 1) = TM1_EXT function is selected.
     * |[26]    |PB2_T2EX  |PB.2 Pin Alternative Function Selection
     * |        |          |Bits PB2_TM2 (ALT_MFP2[4]), PB2_PWM1BK1 (ALT_MFP3[31]), PB2_T2EX (ALT_MFP[26]) and GPB_MFP2 determine the PB.2 function.
     * |        |          |(PB2_TM2, PB2_PWM1BK1, PB2_T2EX, GPB_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nRTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM2_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE1 function is selected.
     * |        |          |(1, 0, 0, 1) = TM2 function is selected.
     * |[27]    |PB3_T3EX  |PB.3 Pin Alternative Function Selection
     * |        |          |Bits PB3_TM3 (ALT_MFP2[5]), PB3_PWM1BK0 (ALT_MFP3[30]), PB3_T3EX (ALT_MFP[27]) and GPB_MFP3 determine the PB.3 function.
     * |        |          |(PB3_TM3, PB3_PWM1BK0, PB3_T3EX, GPB_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nCTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM3_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE0 function is selected.
     * |        |          |(1, 0, 0, 1) = TM3 function is selected.
     * |[29]    |PB8_CLKO  |PB.8 Pin Alternative Function Selection
     * |        |          |Bits PB8_BPWM12 (ALT_MFP3[20]), PB8_CLKO (ALT_MFP[29]) and GPB_MFP8 determine the PB.8 function.
     * |        |          |(PB8_BPWM12, PB8_CLKO, GPB_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = TM0 function is selected.
     * |        |          |(0, 1, 1) = CLKO function is selected.
     * |        |          |(1, 0, 1) = BPWM1_CH2 function is selected.
     */
    __IO uint32_t ALT_MFP;

    uint32_t RESERVE4[2];

    /**
     * ALT_MFP2
     * ===================================================================================================
     * Offset: 0x5C  Alternative Multiple Function Pin Control Register 2
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[2]     |PB15_TM0  |PB.15 Pin Alternative Function Selection
     * |        |          |Bits PB15_BPWM15 (ALT_MFP3[23]), PB15_T0EX (ALT_MFP[24]), PB15_TM0 (ALT_MFP2[2]) and GPB_MFP15 determine the PB.15 function.
     * |        |          |(PB15_BPWM15, PB15_T0EX, PB15_TM0, GPB_MFP15) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = INT1 function is selected.
     * |        |          |(0, 0, 1, 1) = TM0 function is selected.
     * |        |          |(0, 1, 0, 1) = TM0_EXT function is selected.
     * |        |          |(1, 0, 1, 1) = BPWM1_CH5 function is selected.
     * |[3]     |PE5_TM1   |PE.5 Pin Alternative Function Selection
     * |        |          |Bits PE5_T1EX (ALT_MFP[25]), PE5_TM1 (ALT_MFP2[3]) and GPE_MFP5 determine the PE.5 function.
     * |        |          |(PE5_T1EX, PE5_TM1, GPE_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = PWM0_CH5 function is selected.
     * |        |          |(0, 1, 1) = TM1 function is selected.
     * |        |          |(1, 0, 1) = TM1_EXT function is selected.
     * |[4]     |PB2_TM2   |PB.2 Pin Alternative Function Selection
     * |        |          |Bits PB2_TM2 (ALT_MFP2[4]), PB2_PWM1BK1 (ALT_MFP3[31]), PB2_T2EX (ALT_MFP[26]) and GPB_MFP2 determine the PB.2 function.
     * |        |          |(PB2_TM2, PB2_PWM1BK1, PB2_T2EX, GPB_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nRTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM2_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE1 function is selected.
     * |        |          |(1, 0, 0, 1) = TM2 function is selected.
     * |[5]     |PB3_TM3   |PB.3 Pin Alternative Function Selection
     * |        |          |Bits PB3_TM3 (ALT_MFP2[5]), PB3_PWM1BK0 (ALT_MFP3[30]), PB3_T3EX (ALT_MFP[27]) and GPB_MFP3 determine the PB.3 function.
     * |        |          |(PB3_TM3, PB3_PWM1BK0, PB3_T3EX, GPB_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nCTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM3_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE0 function is selected.
     * |        |          |(1, 0, 0, 1) = TM3 function is selected.
     */
    __IO uint32_t ALT_MFP2;

    /**
     * ALT_MFP3
     * ===================================================================================================
     * Offset: 0x60  Alternative Multiple Function Pin Control Register 3
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4]     |PA0_PWM04 |PA.0 Pin Alternative Function Selection
     * |        |          |Bits PA0_PWM04 (ALT_MFP3[4]), PA0_UR5TXD (ALT_MFP4[7]), PA0_I2C1SCL (ALT_MFP4[12]) and GPA_MFP0 determine the PA.0 function.
     * |        |          |(PA0_PWM04, PA0_UR5TXD, PA0_I2C1SCL, GPA_MFP0) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC0 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SCL function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_TXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH4 function is selected.
     * |[5]     |PA1_PWM05 |PA.1 Pin Alternative Function Selection
     * |        |          |Bits PA1_PWM05 (ALT_MFP3[5]), PA1_UR5RXD (ALT_MFP4[6]), PA1_I2C1SDA (ALT_MFP4[13]) and GPA_MFP1 determine the PA.1 function.
     * |        |          |(PA1_PWM05, PA1_UR5RXD , PA1_I2C1SDA, GPA_MFP1) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC1 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SDA function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_RXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH5 function is selected.
     * |[6]     |PA2_PWM10 |PA.2 Pin Alternative Function Selection
     * |        |          |Bits PA2_PWM10 (ALT_MFP3[6]), PA2_UR3TXD (ALT_MFP4[3]) and GPA_MFP2 determine the PA.2 function.
     * |        |          |(PA2_PWM10, PA2_UR3TXD, GPA_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = ADC2 function is selected.
     * |        |          |(0, 1, 1) = UART3_TXD function is selected.
     * |        |          |(1, 0, 1) = PWM1_CH0 function is selected.
     * |[7]     |PA3_PWM11 |PA.3 Pin Alternative Function Selection
     * |        |          |Bits PA3_PWM11 (ALT_MFP3[7]), PA3_UR3RXD (ALT_MFP4[2]) and GPA_MFP3 determine the PA.3 function.
     * |        |          |(PA3_PWM11, PA3_UR3RXD, GPA_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = ADC3 function is selected.
     * |        |          |(0, 1, 1) = UART3_RXD function is selected.
     * |        |          |(1, 0, 1) = PWM1_CH1 function is selected.
     * |[8]     |PA10_PWM12|PA.10 Pin Alternative Function Selection
     * |        |          |Bits PA10_PWM12 (ALT_MFP3[8]) and GPA_MFP10 determine the PA.10 function.
     * |        |          |(PA10_PWM12, GPA_MFP10) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C1_SDA function is selected.
     * |        |          |(1, 1) = PWM1_CH2 function is selected.
     * |[9]     |PA11_PWM13|PA.11 Pin Alternative Function Selection
     * |        |          |Bits PA11_PWM13 (ALT_MFP3[9]) and GPA_MFP11 determine the PA.11 function.
     * |        |          |(PA11_PWM13, GPA_MFP11) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C1_SCL function is selected.
     * |        |          |(1, 1) = PWM1_CH3 function is selected.
     * |[10]    |PF4_PWM14 |PF.4 Pin Alternative Function Selection
     * |        |          |Bits PF4_PWM14 (ALT_MFP3[10]) and GPF_MFP4 determine the PF.4 function.
     * |        |          |(PF4_PWM14, GPF_MFP4) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 1) = PWM1_CH4 function is selected.
     * |[11]    |PF5_PWM15 |PF.5 Pin Alternative Function Selection
     * |        |          |Bits PF5_PWM15 (ALT_MFP3[11]) and GPF_MFP5 determine the PF.5 function.
     * |        |          |(PF5_PWM15, GPF_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 1) = PWM1_CH5 function is selected.
     * |[12]    |PC0_BPWM00|PC.0 Pin Alternative Function Selection
     * |        |          |Bits PC0_BPWM00 (ALT_MFP3[12]) and GPC_MFP0 determine the PC.0 function.
     * |        |          |(PC0_BPWM00, GPC_MFP0) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_SS0 function is selected.
     * |        |          |(1, 1) = BPWM0_CH0 function is selected.
     * |[13]    |PC1_BPWM01|PC.1 Pin Alternative Function Selection
     * |        |          |Bits PC1_BPWM01 (ALT_MFP3[13]) and GPC_MFP1 determine the PC.1 function.
     * |        |          |(PC1_BPWM01, GPC_MFP1) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_CLK function is selected.
     * |        |          |(1, 1) = BPWM0_CH1 function is selected.
     * |[14]    |PC2_BPWM02|PC.2 Pin Alternative Function Selection
     * |        |          |Bits PC2_BPWM02 (ALT_MFP3[14]) and GPC_MFP2 determine the PC.2 function.
     * |        |          |(PC2_BPWM02, GPC_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_MISO0 function is selected.
     * |        |          |(1, 1) = BPWM0_CH2 function is selected.
     * |[15]    |PC3_BPWM03|PC.3 Pin Alternative Function Selection
     * |        |          |Bits PC3_BPWM03 (ALT_MFP3[15]) and GPC_MFP3 determine the PC.3 function.
     * |        |          |(PC3_BPWM03, GPC_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = SPI0_MOSI0 function is selected.
     * |        |          |(1, 1) = BPWM0_CH3 function is selected.
     * |[16]    |PD15_BPWM04|PD.15 Pin Alternative Function Selection
     * |        |          |Bits PD15_BPWM04 (ALT_MFP3[16]) and GPD_MFP15 determine the PD.15 function.
     * |        |          |(PD15_BPWM04, GPD_MFP15) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = UART2_TXD function is selected.
     * |        |          |(1, 1) = BPWM0_CH4 function is selected.
     * |[17]    |PD14_BPWM05|PD.14 Pin Alternative Function Selection
     * |        |          |Bits PD14_BPWM05 (ALT_MFP3[17]) and GPD_MFP14 determine the PD.14 function.
     * |        |          |(PD14_BPWM05, GPD_MFP14) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = UART2_RXD function is selected.
     * |        |          |(1, 1) = BPWM0_CH5 function is selected.
     * |[18]    |PD7_BPWM10|PD.7 Pin Alternative Function Selection
     * |        |          |Bits PD7_BPWM10 (ALT_MFP3[18]) and GPD_MFP7 determine the PD.7 function.
     * |        |          |(PD7_BPWM10, GPD_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CAN0_TXD function is selected.
     * |        |          |(1, 1) = BPWM1_CH0 function is selected.
     * |[19]    |PD6_BPWM11|PD.6 Pin Alternative Function Selection
     * |        |          |Bits PD6_BPWM11 (ALT_MFP3[19]) and GPD_MFP6 determine the PD.6 function.
     * |        |          |(PD6_BPWM11, GPD_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CAN0_RXD function is selected.
     * |        |          |(1, 1) = BPWM1_CH1 function is selected.
     * |[20]    |PB8_BPWM12|PB.8 Pin Alternative Function Selection
     * |        |          |Bits PB8_BPWM12 (ALT_MFP3[20]), PB8_CLKO (ALT_MFP[29]) and GPB_MFP8 determine the PB.8 function.
     * |        |          |(PB8_BPWM12, PB8_CLKO, GPB_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = TM0 function is selected.
     * |        |          |(0, 1, 1) = CLKO function is selected.
     * |        |          |(1, 0, 1) = BPWM1_CH2 function is selected.
     * |[21]    |PB12_BPWM13|PB.12 Pin Alternative Function Selection
     * |        |          |Bits PB12_BPWM13 (ALT_MFP3[21]) and GPB_MFP12 determine the PB.12 function.
     * |        |          |(PB12_BPWM13, GPB_MFP12) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CLKO function is selected.
     * |        |          |(1, 1) = BPWM1_CH3 function is selected.
     * |[22]    |PF8_BPWM14|PF.8 Pin Alternative Function Selection
     * |        |          |Bits PF8_BPWM14 (ALT_MFP3[22]) and GPF_MFP8 determine the PF.8 function.
     * |        |          |(PF8_BPWM14, GPF_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = CLKO function is selected.
     * |        |          |(1, 0) = BPWM1_CH4 function is selected.
     * |[23]    |PB15_BPWM15|PB.15 Pin Alternative Function Selection
     * |        |          |Bits PB15_BPWM15 (ALT_MFP3[23]), PB15_T0EX (ALT_MFP[24]), PB15_TM0 (ALT_MFP2[2]) and GPB_MFP15 determine the PB.15 function.
     * |        |          |(PB15_BPWM15, PB15_T0EX, PB15_TM0, GPB_MFP15) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = INT1 function is selected.
     * |        |          |(0, 0, 1, 1) = TM0 function is selected.
     * |        |          |(0, 1, 0, 1) = TM0_EXT function is selected.
     * |        |          |(1, 0, 1, 1) = BPWM1_CH5 function is selected.
     * |[24]    |PB11_PWM04|PB.11 Pin Alternative Function Selection
     * |        |          |Bits PB11_PWM04 (ALT_MFP3[24]) and GPB_MFP11 determine the PB.11 function.
     * |        |          |(PB11_PWM04, GPB_MFP11) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = TM3 function is selected.
     * |        |          |(1, 1) = PWM0_CH4 function is selected.
     * |[28]    |PC6_PWM0BK0|PC.6 Pin Alternative Function Selection
     * |        |          |Bits PC6_PWM0BK0 (ALT_MFP3[28]), PC6_I2C0SDA (ALT_MFP4[10]) and GPC_MFP6 determine the PC.6 function.
     * |        |          |(PC6_PWM0BK0, PC6_I2C0SDA, GPB_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = UART4_TXD function is selected.
     * |        |          |(0, 1, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 0, 1) = PWM0_BRAKE0 function is selected.
     * |[29]    |PC7_PWM0BK1|PC.7 Pin Alternative Function Selection
     * |        |          |Bits PC7_PWM0BK1 (ALT_MFP3[29]), PC7_I2C0SCL (ALT_MFP4[11]) and GPC_MFP7 determine the PC.7 function.
     * |        |          |(PC7_PWM0BK1, PC7_I2C0SCL, GPC_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = UART4_RXD function is selected.
     * |        |          |(0, 1, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 0, 1) = PWM0_BRAKE1 function is selected.
     * |[30]    |PB3_PWM1BK0|PB.3 Pin Alternative Function Selection
     * |        |          |Bits PB3_TM3 (ALT_MFP2[5]), PB3_PWM1BK0 (ALT_MFP3[30]), PB3_T3EX (ALT_MFP[27]) and GPB_MFP3 determine the PB.3 function.
     * |        |          |(PB3_TM3, PB3_PWM1BK0, PB3_T3EX, GPB_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nCTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM3_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE0 function is selected.
     * |        |          |(1, 0, 0, 1) = TM3 function is selected.
     * |[31]    |PB2_PWM1BK1|PB.2 Pin Alternative Function Selection
     * |        |          |Bits PB2_TM2 (ALT_MFP2[4]), PB2_PWM1BK1 (ALT_MFP3[31]), PB2_T2EX (ALT_MFP[26]) and GPB_MFP2 determine the PB.2 function.
     * |        |          |(PB2_TM2, PB2_PWM1BK1, PB2_T2EX, GPB_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = UART0_nRTS function is selected.
     * |        |          |(0, 0, 1, 1) = TM2_EXT function is selected.
     * |        |          |(0, 1, 0, 1) = PWM1_BRAKE1 function is selected.
     * |        |          |(1, 0, 0, 1) = TM2 function is selected.
     */
    __IO uint32_t ALT_MFP3;

    /**
     * ALT_MFP4
     * ===================================================================================================
     * Offset: 0x64  Alternative Multiple Function Pin Control Register 4
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |PA8_UR1RTS|PA.8 Pin Alternative Function Selection
     * |        |          |Bits PA8_UR1RTS (ALT_MFP4[0]) and GPA_MFP8 determine the PA.8 function.
     * |        |          |(PA8_UR1RTS, GPA_MFP8) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 1) = UART1_nRTS function is selected.
     * |[1]     |PA9_UR1CTS|PA.9 Pin Alternative Function Selection
     * |        |          |Bits PA9_UR1CTS (ALT_MFP4[1]) and GPA_MFP9 determine the PA.9 function.
     * |        |          |(PA9_UR1CTS, GPA_MFP9) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 1) = UART1_nCTS function is selected.
     * |[2]     |PA3_UR3RXD|PA.3 Pin Alternative Function Selection
     * |        |          |Bits PA3_PWM11 (ALT_MFP3[7]), PA3_UR3RXD (ALT_MFP4[2]) and GPA_MFP3 determine the PA.3 function.
     * |        |          |(PA3_PWM11, PA3_UR3RXD, GPA_MFP3) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = ADC3 function is selected.
     * |        |          |(0, 1, 1) = UART3_RXD function is selected.
     * |        |          |(1, 0, 1) = PWM1_CH1 function is selected.
     * |[3]     |PA2_UR3TXD|PA.2 Pin Alternative Function Selection
     * |        |          |Bits PA2_PWM10 (ALT_MFP3[6]), PA2_UR3TXD (ALT_MFP4[3]) and GPA_MFP2 determine the PA.2 function.
     * |        |          |(PA2_PWM10, PA2_UR3TXD, GPA_MFP2) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = ADC2 function is selected.
     * |        |          |(0, 1, 1) = UART3_TXD function is selected.
     * |        |          |(1, 0, 1) = PWM1_CH0 function is selected.
     * |[4]     |PA5_UR3RXD|PA.5 Pin Alternative Function Selection
     * |        |          |Bits PA5_UR3RXD (ALT_MFP4[4]) and GPA_MFP5 determine the PA.5 function.
     * |        |          |(PA5_UR3RXD, GPA_MFP5) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = ADC5 function is selected.
     * |        |          |(1, 1) = UART3_RXD function is selected.
     * |[5]     |PA6_UR3TXD|PA.6 Pin Alternative Function Selection
     * |        |          |Bits PA6_UR3TXD (ALT_MFP4[5]) and GPA_MFP6 determine the PA.6 function.
     * |        |          |(PA6_UR3TXD, GPA_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = ADC6 function is selected.
     * |        |          |(1, 1) = UART3_TXD function is selected.
     * |[6]     |PA1_UR5RXD|PA.1 Pin Alternative Function Selection
     * |        |          |Bits PA1_PWM05 (ALT_MFP3[5]), PA1_UR5RXD (ALT_MFP4[6]), PA1_I2C1SDA (ALT_MFP4[13]) and GPA_MFP1 determine the PA.1 function.
     * |        |          |(PA1_PWM05, PA1_UR5RXD, PA1_I2C1SDA, GPA_MFP1) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC1 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SDA function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_RXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH5 function is selected.
     * |[7]     |PA0_UR5TXD|PA.0 Pin Alternative Function Selection
     * |        |          |Bits PA0_PWM04 (ALT_MFP3[4]), PA0_UR5TXD (ALT_MFP4[7]), PA0_I2C1SCL (ALT_MFP4[12]) and GPA_MFP0 determine the PA.0 function.
     * |        |          |(PA0_PWM04, PA0_UR5TXD, PA0_I2C1SCL, GPA_MFP0) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC0 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SCL function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_TXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH4 function is selected.
     * |[8]     |PA12_UR5RXD|PA.12 Pin Alternative Function Selection
     * |        |          |Bits PA12_UR5RXD (ALT_MFP4[8]) and GPA_MFP12 determine the PA.12 function.
     * |        |          |(PA12_UR5RXD, GPA_MFP12) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = PWM0_CH0 function is selected.
     * |        |          |(1, 1) = UART5_RXD function is selected.
     * |[9]     |PA13_UR5TXD|PA.13 Pin Alternative Function Selection
     * |        |          |Bits PA13_UR5TXD (ALT_MFP4[9]) and GPA_MFP13 determine the PA.13 function.
     * |        |          |(PA13_UR5TXD, GPA_MFP13) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = PWM0_CH1 function is selected.
     * |        |          |(1, 1) = UART5_TXD function is selected.
     * |[10]    |PC6_I2C0SDA|PC.6 Pin Alternative Function Selection
     * |        |          |Bits PC6_PWM0BK0 (ALT_MFP3[28]), PC6_I2C0SDA (ALT_MFP4[10]) and GPC_MFP6 determine the PC.6 function.
     * |        |          |(PC6_PWM0BK0, PC6_I2C0SDA, GPC_MFP6) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = UART4_TXD function is selected.
     * |        |          |(0, 1, 1) = I2C0_SDA function is selected.
     * |        |          |(1, 0, 1) = PWM0_BRAKE0 function is selected.
     * |[11]    |PC7_I2C0SCL|PC.7 Pin Alternative Function Selection
     * |        |          |Bits PC7_PWM0BK1 (ALT_MFP3[29]), PC7_I2C0SCL (ALT_MFP4[11]) and GPC_MFP7 determine the PC.7 function.
     * |        |          |(PC7_PWM0BK1, PC7_I2C0SCL, GPC_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 1) = UART4_RXD function is selected.
     * |        |          |(0, 1, 1) = I2C0_SCL function is selected.
     * |        |          |(1, 0, 1) = PWM0_BRAKE1 function is selected.
     * |[12]    |PA0_I2C1SCL|PA.0 Pin Alternative Function Selection
     * |        |          |Bits PA0_PWM04 (ALT_MFP3[4]), PA0_UR5TXD (ALT_MFP4[7]), PA0_I2C1SCL (ALT_MFP4[12]) and GPA_MFP0 determine the PA.0 function.
     * |        |          |(PA0_PWM04, PA0_UR5TXD, PA0_I2C1SCL, GPA_MFP0) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC0 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SCL function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_TXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH4 function is selected.
     * |[13]    |PA1_I2C1SDA|PA.1 Pin Alternative Function Selection
     * |        |          |Bits PA1_PWM05 (ALT_MFP3[5]), PA1_UR5RXD (ALT_MFP4[6]), PA1_I2C1SDA (ALT_MFP4[13]) and GPA_MFP1 determine the PA.1 function.
     * |        |          |(PA1_PWM05, PA1_UR5RXD, PA1_I2C1SDA, GPA_MFP1) value and function mapping is as following list.
     * |        |          |(0, 0, 0, 0) = GPIO function is selected.
     * |        |          |(0, 0, 0, 1) = ADC1 function is selected.
     * |        |          |(0, 0, 1, 1) = I2C1_SDA function is selected.
     * |        |          |(0, 1, 0, 1) = UART5_RXD function is selected.
     * |        |          |(1, 0, 0, 1) = PWM0_CH5 function is selected.
     * |[14]    |PA7_VREF  |PA.7 Pin Alternative Function Selection
     * |        |          |Bits PA7_VREF (ALT_MFP4[14]) and GPA_MFP7 determine the PA.7 function.
     * |        |          |(PA7_VREF, GPA_MFP7) value and function mapping is as following list.
     * |        |          |(0, 0) = GPIO function is selected.
     * |        |          |(0, 1) = ADC7 function is selected.
     * |        |          |(1, 1) = VREF function is selected.
     */
    __IO uint32_t ALT_MFP4;

    uint32_t RESERVE5[38];

    /**
     * REGWRPROT
     * ===================================================================================================
     * Offset: 0x100  Register Write Protection Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |REGPROTDIS|Register Write-Protection Disable Index (Read Only)
     * |        |          |0 = Write-protection is enabled for writing protected registers. Any write to the protected register is ignored.
     * |        |          |1 = Write-protection is disabled for writing protected registers.
     * |        |          |The Protected registers are:
     * |        |          |IPRSTC1: address 0x5000_0008
     * |        |          |BODCR: address 0x5000_0018
     * |        |          |PORCR: address 0x5000_0024
     * |        |          |VREFCR: address 0x5000_0028
     * |        |          |PWRCON: address 0x5000_0200 (bit[6] is not protected for power wake-up interrupt clear)
     * |        |          |APBCLK bit[0]: address 0x5000_0208 (bit[0] is Watchdog Timer clock enable)
     * |        |          |CLKSEL0: address 0x5000_0210 (for HCLK and CPU STCLK clock source selection)
     * |        |          |CLKSEL1 bit[1:0]: address 0x5000_0214 (for Watchdog Timer clock source selection)
     * |        |          |NMISEL bit[8]: address 0x5000_0380 (for NMI_EN NMI interrupt enable)
     * |        |          |ISPCON: address 0x5000_C000 (Flash ISP Control register)
     * |        |          |ISPTRG: address 0x5000_C010 (ISP Trigger Control register)
     * |        |          |FATCON: address 0x5000_C018
     * |        |          |WTCR: address 0x4000_4000
     * |        |          |WTCRALT: address 0x4000_4004
     * |        |          |PWM_CTL0: address 0x4004_0000, 0x4014_0000
     * |        |          |PWM_DTCTL0_1: address 0x4004_0070, 0x4014_0070
     * |        |          |PWM_DTCTL2_3: address 0x4004_0074, 0x4014_0074
     * |        |          |PWM_DTCTL4_5: address 0x4004_0078, 0x4014_0078
     * |        |          |PWM_BRKCTL0_1: address 0x4004_00C8, 0x4014_00C8
     * |        |          |PWM_BRKCTL2_3: address 0x4004_00CC, 0x4014_00CC
     * |        |          |PWM_BRKCTL4_5: address 0x4004_00D0, 0x4014_00D0
     * |        |          |PWM_SWBRK: address 0x4004_00DC, 0x4014_00DC
     * |        |          |PWM_INTEN1: address 0x4004_00E4, 0x4014_00E4
     * |        |          |PWM_INTSTS1: address 0x4004_00EC, 0x4014_00EC
     * |        |          |BPWM_CTL0: address 0x4004_4000, 0x4014_4000
     * |        |          |Note: The bits which are write-protected will be noted as" (Write Protect)" beside the description.
     * |[7:0]   |REGWRPROT |Register Write-Protection Code (Write Only)
     * |        |          |Some registers have write-protection function.
     * |        |          |Writing these registers have to disable the protected function by writing the sequence value "59h", "16h", "88h" to this field.
     * |        |          |After this sequence is completed, the REGPROTDIS bit will be set to 1 and write-protection registers can be normal write.
     */
    __IO uint32_t REGWRPROT;
} GCR_T;

/**
    @addtogroup SYS_CONST SYS Bit Field Definition
    Constant Definitions for SYS Controller
@{ */

/* GCR RSTSRC Bit Field Definitions */
#define SYS_RSTSRC_RSTS_CPU_Pos                 7                                   /*!< GCR_T::RSTSRC: RSTS_CPU Position */
#define SYS_RSTSRC_RSTS_CPU_Msk                 (1ul << SYS_RSTSRC_RSTS_CPU_Pos)    /*!< GCR_T::RSTSRC: RSTS_CPU Mask */

#define SYS_RSTSRC_RSTS_SYS_Pos                 5                                   /*!< GCR_T::RSTSRC: RSTS_SYS Position */
#define SYS_RSTSRC_RSTS_SYS_Msk                 (1ul << SYS_RSTSRC_RSTS_SYS_Pos)    /*!< GCR_T::RSTSRC: RSTS_SYS Mask */

#define SYS_RSTSRC_RSTS_BOD_Pos                 4                                   /*!< GCR_T::RSTSRC: RSTS_BOD Position */
#define SYS_RSTSRC_RSTS_BOD_Msk                 (1ul << SYS_RSTSRC_RSTS_BOD_Pos)    /*!< GCR_T::RSTSRC: RSTS_BOD Mask */

#define SYS_RSTSRC_RSTS_LVR_Pos                 3                                   /*!< GCR_T::RSTSRC: RSTS_LVR Position */
#define SYS_RSTSRC_RSTS_LVR_Msk                 (1ul << SYS_RSTSRC_RSTS_LVR_Pos)    /*!< GCR_T::RSTSRC: RSTS_LVR Mask */

#define SYS_RSTSRC_RSTS_WDT_Pos                 2                                   /*!< GCR_T::RSTSRC: RSTS_WDT Position */
#define SYS_RSTSRC_RSTS_WDT_Msk                 (1ul << SYS_RSTSRC_RSTS_WDT_Pos)    /*!< GCR_T::RSTSRC: RSTS_WDT Mask */

#define SYS_RSTSRC_RSTS_RESET_Pos               1                                   /*!< GCR_T::RSTSRC: RSTS_RESET Position */
#define SYS_RSTSRC_RSTS_RESET_Msk               (1ul << SYS_RSTSRC_RSTS_RESET_Pos)  /*!< GCR_T::RSTSRC: RSTS_RESET Mask */

#define SYS_RSTSRC_RSTS_POR_Pos                 0                                   /*!< GCR_T::RSTSRC: RSTS_POR Position */
#define SYS_RSTSRC_RSTS_POR_Msk                 (1ul << SYS_RSTSRC_RSTS_POR_Pos)    /*!< GCR_T::RSTSRC: RSTS_POR Mask */

/* GCR IPRSTC1 Bit Field Definitions */
#define SYS_IPRSTC1_CPU_RST_Pos                 1                                   /*!< GCR_T::IPRSTC1: CPU_RST Position */
#define SYS_IPRSTC1_CPU_RST_Msk                 (1ul << SYS_IPRSTC1_CPU_RST_Pos)    /*!< GCR_T::IPRSTC1: CPU_RST Mask */

#define SYS_IPRSTC1_CHIP_RST_Pos                0                                   /*!< GCR_T::IPRSTC1: CHIP_RST Position */
#define SYS_IPRSTC1_CHIP_RST_Msk                (1ul << SYS_IPRSTC1_CHIP_RST_Pos)   /*!< GCR_T::IPRSTC1: CHIP_RST Mask */

/* GCR IPRSTC2 Bit Field Definitions */
#define SYS_IPRSTC2_ADC_RST_Pos                 28                                  /*!< GCR_T::IPRSTC2: ADC_RST Position */
#define SYS_IPRSTC2_ADC_RST_Msk                 (1ul << SYS_IPRSTC2_ADC_RST_Pos)    /*!< GCR_T::IPRSTC2: ADC_RST Mask */

#define SYS_IPRSTC2_CAN0_RST_Pos                24                                  /*!< GCR_T IPRSTC2: CAN0_RST Position */
#define SYS_IPRSTC2_CAN0_RST_Msk                (1ul << SYS_IPRSTC2_CAN0_RST_Pos)   /*!< GCR_T IPRSTC2: CAN0_RST Mask */

#define SYS_IPRSTC2_UART2_RST_Pos               18                                  /*!< GCR_T::IPRSTC2: UART2_RST Position */
#define SYS_IPRSTC2_UART2_RST_Msk               (1ul << SYS_IPRSTC2_UART2_RST_Pos)  /*!< GCR_T::IPRSTC2: UART2_RST Mask */

#define SYS_IPRSTC2_UART1_RST_Pos               17                                  /*!< GCR_T::IPRSTC2: UART1_RST Position */
#define SYS_IPRSTC2_UART1_RST_Msk               (1ul << SYS_IPRSTC2_UART1_RST_Pos)  /*!< GCR_T::IPRSTC2: UART1_RST Mask */

#define SYS_IPRSTC2_UART0_RST_Pos               16                                  /*!< GCR_T::IPRSTC2: UART0_RST Position */
#define SYS_IPRSTC2_UART0_RST_Msk               (1ul << SYS_IPRSTC2_UART0_RST_Pos)  /*!< GCR_T::IPRSTC2: UART0_RST Mask */

#define SYS_IPRSTC2_SPI0_RST_Pos                12                                  /*!< GCR_T::IPRSTC2: SPI0_RST Position */
#define SYS_IPRSTC2_SPI0_RST_Msk                (1ul << SYS_IPRSTC2_SPI0_RST_Pos)   /*!< GCR_T::IPRSTC2: SPI0_RST Mask */

#define SYS_IPRSTC2_I2C1_RST_Pos                9                                   /*!< GCR_T::IPRSTC2: I2C1_RST Position */
#define SYS_IPRSTC2_I2C1_RST_Msk                (1ul << SYS_IPRSTC2_I2C1_RST_Pos)   /*!< GCR_T::IPRSTC2: I2C1_RST Mask */

#define SYS_IPRSTC2_I2C0_RST_Pos                8                                   /*!< GCR_T::IPRSTC2: I2C0_RST Position */
#define SYS_IPRSTC2_I2C0_RST_Msk                (1ul << SYS_IPRSTC2_I2C0_RST_Pos)   /*!< GCR_T::IPRSTC2: I2C0_RST Mask */

#define SYS_IPRSTC2_TMR3_RST_Pos                5                                   /*!< GCR_T::IPRSTC2: TMR3_RST Position */
#define SYS_IPRSTC2_TMR3_RST_Msk                (1ul << SYS_IPRSTC2_TMR3_RST_Pos)   /*!< GCR_T::IPRSTC2: TMR3_RST Mask */

#define SYS_IPRSTC2_TMR2_RST_Pos                4                                   /*!< GCR_T::IPRSTC2: TMR2_RST Position */
#define SYS_IPRSTC2_TMR2_RST_Msk                (1ul << SYS_IPRSTC2_TMR2_RST_Pos)   /*!< GCR_T::IPRSTC2: TMR2_RST Mask */

#define SYS_IPRSTC2_TMR1_RST_Pos                3                                   /*!< GCR_T::IPRSTC2: TMR1_RST Position */
#define SYS_IPRSTC2_TMR1_RST_Msk                (1ul << SYS_IPRSTC2_TMR1_RST_Pos)   /*!< GCR_T::IPRSTC2: TMR1_RST Mask */

#define SYS_IPRSTC2_TMR0_RST_Pos                2                                   /*!< GCR_T::IPRSTC2: TMR0_RST Position */
#define SYS_IPRSTC2_TMR0_RST_Msk                (1ul << SYS_IPRSTC2_TMR0_RST_Pos)   /*!< GCR_T::IPRSTC2: TMR0_RST Mask */

#define SYS_IPRSTC2_GPIO_RST_Pos                1                                   /*!< GCR_T::IPRSTC2: GPIO_RST Position */
#define SYS_IPRSTC2_GPIO_RST_Msk                (1ul << SYS_IPRSTC2_GPIO_RST_Pos)   /*!< GCR_T::IPRSTC2: GPIO_RST Mask */

/* GCR IPRSTC3 Bit Field Definitions */
#define SYS_IPRSTC3_BPWM1_RST_Pos               19                                  /*!< GCR_T::IPRSTC3: BPWM1_RST Position */
#define SYS_IPRSTC3_BPWM1_RST_Msk               (1ul << SYS_IPRSTC3_BPWM1_RST_Pos)  /*!< GCR_T::IPRSTC3: BPWM1_RST Mask */

#define SYS_IPRSTC3_BPWM0_RST_Pos               18                                  /*!< GCR_T::IPRSTC3: BPWM0_RST Position */
#define SYS_IPRSTC3_BPWM0_RST_Msk               (1ul << SYS_IPRSTC3_BPWM0_RST_Pos)  /*!< GCR_T::IPRSTC3: BPWM0_RST Mask */

#define SYS_IPRSTC3_PWM1_RST_Pos                17                                  /*!< GCR_T::IPRSTC3: PWM1_RST Position */
#define SYS_IPRSTC3_PWM1_RST_Msk                (1ul << SYS_IPRSTC3_PWM1_RST_Pos)   /*!< GCR_T::IPRSTC3: PWM1_RST Mask */

#define SYS_IPRSTC3_PWM0_RST_Pos                16                                  /*!< GCR_T::IPRSTC3: PWM0_RST Position */
#define SYS_IPRSTC3_PWM0_RST_Msk                (1ul << SYS_IPRSTC3_PWM0_RST_Pos)   /*!< GCR_T::IPRSTC3: PWM0_RST Mask */

#define SYS_IPRSTC3_UART5_RST_Pos               10                                  /*!< GCR_T::IPRSTC3: UART5_RST Position */
#define SYS_IPRSTC3_UART5_RST_Msk               (1ul << SYS_IPRSTC3_UART5_RST_Pos)  /*!< GCR_T::IPRSTC3: UART5_RST Mask */

#define SYS_IPRSTC3_UART4_RST_Pos               9                                   /*!< GCR_T::IPRSTC3: UART4_RST Position */
#define SYS_IPRSTC3_UART4_RST_Msk               (1ul << SYS_IPRSTC3_UART4_RST_Pos)  /*!< GCR_T::IPRSTC3: UART4_RST Mask */

#define SYS_IPRSTC3_UART3_RST_Pos               8                                   /*!< GCR_T::IPRSTC3: UART3_RST Position */
#define SYS_IPRSTC3_UART3_RST_Msk               (1ul << SYS_IPRSTC3_UART3_RST_Pos)  /*!< GCR_T::IPRSTC3: UART3_RST Mask */

/* GCR BODCR Bit Field Definitions */
#define SYS_BODCR_LVR_OUT_DG_SEL_Pos            12                                      /*!< GCR_T::BODCR: LVR_OUT_DG_SEL Position */
#define SYS_BODCR_LVR_OUT_DG_SEL_Msk            (0x7ul << SYS_BODCR_LVR_OUT_DG_SEL_Pos) /*!< GCR_T::BODCR: LVR_OUT_DG_SEL Mask */

#define SYS_BODCR_BOD_OUT_DG_SEL_Pos            8                                       /*!< GCR_T::BODCR: BOD_OUT_DG_SEL Position */
#define SYS_BODCR_BOD_OUT_DG_SEL_Msk            (0x7ul << SYS_BODCR_BOD_OUT_DG_SEL_Pos) /*!< GCR_T::BODCR: BOD_OUT_DG_SEL Mask */

#define SYS_BODCR_LVR_EN_Pos                    7                                   /*!< GCR_T::BODCR: LVR_EN Position */
#define SYS_BODCR_LVR_EN_Msk                    (1ul << SYS_BODCR_LVR_EN_Pos)       /*!< GCR_T::BODCR: LVR_EN Mask */

#define SYS_BODCR_BOD_OUT_Pos                   6                                   /*!< GCR_T::BODCR: BOD_OUT Position */
#define SYS_BODCR_BOD_OUT_Msk                   (1ul << SYS_BODCR_BOD_OUT_Pos)      /*!< GCR_T::BODCR: BOD_OUT Mask */

#define SYS_BODCR_BOD_LPM_Pos                   5                                   /*!< GCR_T::BODCR: BOD_LPM Position */
#define SYS_BODCR_BOD_LPM_Msk                   (1ul << SYS_BODCR_BOD_LPM_Pos)      /*!< GCR_T::BODCR: BOD_LPM Mask */

#define SYS_BODCR_BOD_INTF_Pos                  4                                   /*!< GCR_T::BODCR: BOD_INTF Position */
#define SYS_BODCR_BOD_INTF_Msk                  (1ul << SYS_BODCR_BOD_INTF_Pos)     /*!< GCR_T::BODCR: BOD_INTF Mask */

#define SYS_BODCR_BOD_RSTEN_Pos                 3                                   /*!< GCR_T::BODCR: BOD_RSTEN Position */
#define SYS_BODCR_BOD_RSTEN_Msk                 (1ul << SYS_BODCR_BOD_RSTEN_Pos)    /*!< GCR_T::BODCR: BOD_RSTEN Mask */

#define SYS_BODCR_BOD_VL_Pos                    1                                   /*!< GCR_T::BODCR: BOD_VL Position */
#define SYS_BODCR_BOD_VL_Msk                    (3ul << SYS_BODCR_BOD_VL_Pos)       /*!< GCR_T::BODCR: BOD_VL Mask */

#define SYS_BODCR_BOD_EN_Pos                    0                                   /*!< GCR_T::BODCR: BOD_EN Position */
#define SYS_BODCR_BOD_EN_Msk                    (1ul << SYS_BODCR_BOD_EN_Pos)       /*!< GCR_T::BODCR: BOD_EN Mask */

/* GCR VREFCR Bit Field Definitions */
#define SYS_VREFCR_ADC_VREFSEL_Pos              4                                       /*!< GCR_T::VREFCR: ADC_VREFSEL Position */
#define SYS_VREFCR_ADC_VREFSEL_Msk              (1ul << SYS_VREFCR_ADC_VREFSEL_Pos)     /*!< GCR_T::VREFCR: ADC_VREFSEL Mask */

/* GCR PORCR Bit Field Definitions */
#define SYS_PORCR_POR_DIS_CODE_Pos              0                                           /*!< GCR_T::PORCR: POR_DIS_CODE Position */
#define SYS_PORCR_POR_DIS_CODE_Msk              (0xFFFFul << SYS_PORCR_POR_DIS_CODE_Pos)    /*!< GCR_T::PORCR: POR_DIS_CODE Mask */

/* GCR GPAMFP Bit Field Definitions */
#define SYS_GPA_MFP_GPA_TYPE_Pos                 16                                         /*!< GCR_T::GPA_MFP: GPA_TYPE Position */
#define SYS_GPA_MFP_GPA_TYPE_Msk                 (0xFFFFul << SYS_GPA_MFP_GPA_TYPE_Pos)     /*!< GCR_T::GPA_MFP: GPA_TYPE Mask */

#define SYS_GPA_MFP_GPA_MFP_Pos                  0                                          /*!< GCR_T::GPA_MFP: GPA_MFP Position */
#define SYS_GPA_MFP_GPA_MFP_Msk                  (0xFFFFul << SYS_GPA_MFP_GPA_MFP_Pos)      /*!< GCR_T::GPA_MFP: GPA_MFP Mask */

/* GCR GPBMFP Bit Field Definitions */
#define SYS_GPB_MFP_GPB_TYPE_Pos                 16                                         /*!< GCR_T::GPB_MFP: GPB_TYPE Position */
#define SYS_GPB_MFP_GPB_TYPE_Msk                 (0xFFFFul << SYS_GPB_MFP_GPB_TYPE_Pos)     /*!< GCR_T::GPB_MFP: GPB_TYPE Mask */

#define SYS_GPB_MFP_GPB_MFP_Pos                  0                                          /*!< GCR_T::GPB_MFP: GPB_MFP Position */
#define SYS_GPB_MFP_GPB_MFP_Msk                  (0xFFFFul << SYS_GPB_MFP_GPB_MFP_Pos)      /*!< GCR_T::GPB_MFP: GPB_MFP Mask */

/* GCR GPCMFP Bit Field Definitions */
#define SYS_GPC_MFP_GPC_TYPE_Pos                 16                                         /*!< GCR_T::GPC_MFP: GPC_TYPE Position */
#define SYS_GPC_MFP_GPC_TYPE_Msk                 (0xFFFFul << SYS_GPC_MFP_GPC_TYPE_Pos)     /*!< GCR_T::GPC_MFP: GPC_TYPE Mask */

#define SYS_GPC_MFP_GPC_MFP_Pos                  0                                          /*!< GCR_T::GPC_MFP: GPC_MFP Position */
#define SYS_GPC_MFP_GPC_MFP_Msk                  (0xFFFFul << SYS_GPC_MFP_GPC_MFP_Pos)      /*!< GCR_T::GPC_MFP: GPC_MFP Mask */

/* GCR GPDMFP Bit Field Definitions */
#define SYS_GPD_MFP_GPD_TYPE_Pos                 16                                         /*!< GCR_T::GPD_MFP: GPD_TYPE Position */
#define SYS_GPD_MFP_GPD_TYPE_Msk                 (0xFFFFul << SYS_GPD_MFP_GPD_TYPE_Pos)     /*!< GCR_T::GPD_MFP: GPD_TYPE Mask */

#define SYS_GPD_MFP_GPD_MFP_Pos                  0                                          /*!< GCR_T::GPD_MFP: GPD_MFP Position */
#define SYS_GPD_MFP_GPD_MFP_Msk                  (0xFFFFul << SYS_GPD_MFP_GPD_MFP_Pos)      /*!< GCR_T::GPD_MFP: GPD_MFP Mask */

/* GCR GPEMFP Bit Field Definitions */
#define SYS_GPE_MFP_GPE_TYPE_Pos                 16                                         /*!< GCR_T::GPE_MFP: GPE_TYPE Position */
#define SYS_GPE_MFP_GPE_TYPE_Msk                 (0xFFFFul << SYS_GPE_MFP_GPE_TYPE_Pos)     /*!< GCR_T::GPE_MFP: GPE_TYPE Mask */

#define SYS_GPE_MFP_GPE_MFP5_Pos                 5                                          /*!< GCR_T::GPE_MFP: GPE_MFP5 Position */
#define SYS_GPE_MFP_GPE_MFP5_Msk                 (1ul << SYS_GPE_MFP_GPE_MFP5_Pos)          /*!< GCR_T::GPE_MFP: GPE_MFP5 Mask */

/* GCR GPFMFP Bit Field Definitions */
#define SYS_GPF_MFP_GPF_TYPE_Pos                 16                                         /*!< GCR_T::GPF_MFP: GPF_TYPE Position */
#define SYS_GPF_MFP_GPF_TYPE_Msk                 (0x1Ful << SYS_GPF_MFP_GPF_TYPE_Pos)       /*!< GCR_T::GPF_MFP: GPF_TYPE Mask */

#define SYS_GPF_MFP_GPF_MFP_Pos                  0                                          /*!< GCR_T::GPF_MFP: GPF_MFP Position */
#define SYS_GPF_MFP_GPF_MFP_Msk                  (0x1Ful << SYS_GPF_MFP_GPF_MFP_Pos)        /*!< GCR_T::GPF_MFP: GPF_MFP Mask */

/* GCR ALTMFP Bit Field Definitions */
#define SYS_ALT_MFP_PB8_CLKO_Pos                29                                          /*!< GCR_T::ALT_MFP: PB8_CLKO Position */
#define SYS_ALT_MFP_PB8_CLKO_Msk                (1ul << SYS_ALT_MFP_PB8_CLKO_Pos)           /*!< GCR_T::ALT_MFP: PB8_CLKO Mask */

#define SYS_ALT_MFP_PB3_T3EX_Pos                 27                                         /*!< GCR_T::ALT_MFP: PB3_T3EX Position */
#define SYS_ALT_MFP_PB3_T3EX_Msk                 (1ul << SYS_ALT_MFP_PB3_T3EX_Pos)          /*!< GCR_T::ALT_MFP: PB3_T3EX Mask */

#define SYS_ALT_MFP_PB2_T2EX_Pos                 26                                         /*!< GCR_T::ALT_MFP: PB2_T2EX Position */
#define SYS_ALT_MFP_PB2_T2EX_Msk                 (1ul << SYS_ALT_MFP_PB2_T2EX_Pos)          /*!< GCR_T::ALT_MFP: PB3_T3EX Mask */

#define SYS_ALT_MFP_PE5_T1EX_Pos                 25                                         /*!< GCR_T::ALT_MFP: PE5_T1EX Position */
#define SYS_ALT_MFP_PE5_T1EX_Msk                 (1ul << SYS_ALT_MFP_PE5_T1EX_Pos)          /*!< GCR_T::ALT_MFP: PE5_T1EX Mask */

#define SYS_ALT_MFP_PB15_T0EX_Pos                24                                         /*!< GCR_T::ALT_MFP: PB15_T0EX Position */
#define SYS_ALT_MFP_PB15_T0EX_Msk                (1ul << SYS_ALT_MFP_PB15_T0EX_Pos)         /*!< GCR_T::ALT_MFP: PB15_T0EX Mask */

/* GCR ALTMFP2 Bit Field Definitions */
#define SYS_ALT_MFP2_PB3_TM3_Pos                5                                           /*!< GCR_T::ALT_MFP2: PB3_TM3 Position */
#define SYS_ALT_MFP2_PB3_TM3_Msk                (1ul << SYS_ALT_MFP2_PB3_TM3_Pos)           /*!< GCR_T::ALT_MFP2: PB3_TM3 Mask */

#define SYS_ALT_MFP2_PB2_TM2_Pos                4                                           /*!< GCR_T::ALT_MFP2: PB2_TM2 Position */
#define SYS_ALT_MFP2_PB2_TM2_Msk                (1ul << SYS_ALT_MFP2_PB2_TM2_Pos)           /*!< GCR_T::ALT_MFP2: PB2_TM2 Mask */

#define SYS_ALT_MFP2_PE5_TM1_Pos                3                                           /*!< GCR_T::ALT_MFP2: PE5_TM1 Position */
#define SYS_ALT_MFP2_PE5_TM1_Msk                (1ul << SYS_ALT_MFP2_PE5_TM1_Pos)           /*!< GCR_T::ALT_MFP2: PE5_TM1 Mask */

#define SYS_ALT_MFP2_PB15_TM0_Pos               2                                           /*!< GCR_T::ALT_MFP2: PB15_TM0 Position */
#define SYS_ALT_MFP2_PB15_TM0_Msk               (1ul << SYS_ALT_MFP2_PB15_TM0_Pos)          /*!< GCR_T::ALT_MFP2: PB15_TM0 Mask */

/* GCR ALTMFP3 Bit Field Definitions */
#define SYS_ALT_MFP3_PB2_PWM1BK1_Pos            31                                          /*!< GCR_T::ALT_MFP3: PB2_PWM1BK1 Position */
#define SYS_ALT_MFP3_PB2_PWM1BK1_Msk            (1ul << SYS_ALT_MFP3_PB2_PWM1BK1_Pos)       /*!< GCR_T::ALT_MFP3: PB2_PWM1BK1 Mask */

#define SYS_ALT_MFP3_PB3_PWM1BK0_Pos            30                                          /*!< GCR_T::ALT_MFP3: PB3_PWM1BK0 Position */
#define SYS_ALT_MFP3_PB3_PWM1BK0_Msk            (1ul << SYS_ALT_MFP3_PB3_PWM1BK0_Pos)       /*!< GCR_T::ALT_MFP3: PB3_PWM1BK0 Mask */

#define SYS_ALT_MFP3_PC7_PWM0BK1_Pos            29                                          /*!< GCR_T::ALT_MFP3: PC7_PWM0BK1 Position */
#define SYS_ALT_MFP3_PC7_PWM0BK1_Msk            (1ul << SYS_ALT_MFP3_PC7_PWM0BK1_Pos)       /*!< GCR_T::ALT_MFP3: PC7_PWM0BK1 Mask */

#define SYS_ALT_MFP3_PC6_PWM0BK0_Pos            28                                          /*!< GCR_T::ALT_MFP3: PC6_PWM0BK0 Position */
#define SYS_ALT_MFP3_PC6_PWM0BK0_Msk            (1ul << SYS_ALT_MFP3_PC6_PWM0BK0_Pos)       /*!< GCR_T::ALT_MFP3: PC6_PWM0BK0 Mask */

#define SYS_ALT_MFP3_PB11_PWM04_Pos             24                                          /*!< GCR_T::ALT_MFP3: PB11_PWM04 Position */
#define SYS_ALT_MFP3_PB11_PWM04_Msk             (1ul << SYS_ALT_MFP3_PB11_PWM04_Pos)        /*!< GCR_T::ALT_MFP3: PB11_PWM04 Mask */

#define SYS_ALT_MFP3_PB15_BPWM15_Pos            23                                          /*!< GCR_T::ALT_MFP3: PFB15_BPWM15 Position */
#define SYS_ALT_MFP3_PB15_BPWM15_Msk            (1ul << SYS_ALT_MFP3_PB15_BPWM15_Pos)       /*!< GCR_T::ALT_MFP3: PB15_BPWM15 Mask */

#define SYS_ALT_MFP3_PF8_BPWM14_Pos             22                                          /*!< GCR_T::ALT_MFP3: PF8_BPWM14 Position */
#define SYS_ALT_MFP3_PF8_BPWM14_Msk             (1ul << SYS_ALT_MFP3_PF8_BPWM14_Pos)        /*!< GCR_T::ALT_MFP3: PF8_BPWM14 Mask */

#define SYS_ALT_MFP3_PB12_BPWM13_Pos            21                                          /*!< GCR_T::ALT_MFP3: PB12_BPWM13 Position */
#define SYS_ALT_MFP3_PB12_BPWM13_Msk            (1ul << SYS_ALT_MFP3_PB12_BPWM13_Pos)       /*!< GCR_T::ALT_MFP3: PB12_BPWM13 Mask */

#define SYS_ALT_MFP3_PB8_BPWM12_Pos             20                                          /*!< GCR_T::ALT_MFP3: PB8_BPWM12 Position */
#define SYS_ALT_MFP3_PB8_BPWM12_Msk             (1ul << SYS_ALT_MFP3_PB8_BPWM12_Pos)        /*!< GCR_T::ALT_MFP3: PB8_BPWM12 Mask */

#define SYS_ALT_MFP3_PD6_BPWM11_Pos             19                                          /*!< GCR_T::ALT_MFP3: PD6_BPWM11 Position */
#define SYS_ALT_MFP3_PD6_BPWM11_Msk             (1ul << SYS_ALT_MFP3_PD6_BPWM11_Pos)        /*!< GCR_T::ALT_MFP3: PD6_BPWM11 Mask */

#define SYS_ALT_MFP3_PD7_BPWM10_Pos             18                                          /*!< GCR_T::ALT_MFP3: PD7_BPWM10 Position */
#define SYS_ALT_MFP3_PD7_BPWM10_Msk             (1ul << SYS_ALT_MFP3_PD7_BPWM10_Pos)        /*!< GCR_T::ALT_MFP3: PD7_BPWM10 Mask */

#define SYS_ALT_MFP3_PD14_BPWM05_Pos            17                                          /*!< GCR_T::ALT_MFP3: PD14_BPWM05 Position */
#define SYS_ALT_MFP3_PD14_BPWM05_Msk            (1ul << SYS_ALT_MFP3_PD14_BPWM05_Pos)       /*!< GCR_T::ALT_MFP3: PD14_BPWM05 Mask */

#define SYS_ALT_MFP3_PD15_BPWM04_Pos            16                                          /*!< GCR_T::ALT_MFP3: PD15_BPWM04 Position */
#define SYS_ALT_MFP3_PD15_BPWM04_Msk            (1ul << SYS_ALT_MFP3_PD15_BPWM04_Pos)       /*!< GCR_T::ALT_MFP3: PD15_BPWM04 Mask */

#define SYS_ALT_MFP3_PC3_BPWM03_Pos             15                                          /*!< GCR_T::ALT_MFP3: PC3_BPWM03 Position */
#define SYS_ALT_MFP3_PC3_BPWM03_Msk             (1ul << SYS_ALT_MFP3_PC3_BPWM03_Pos)        /*!< GCR_T::ALT_MFP3: PC3_BPWM03 Mask */

#define SYS_ALT_MFP3_PC2_BPWM02_Pos             14                                          /*!< GCR_T::ALT_MFP3: PC2_BPWM02 Position */
#define SYS_ALT_MFP3_PC2_BPWM02_Msk             (1ul << SYS_ALT_MFP3_PC2_BPWM02_Pos)        /*!< GCR_T::ALT_MFP3: PC2_BPWM02 Mask */

#define SYS_ALT_MFP3_PC1_BPWM01_Pos             13                                          /*!< GCR_T::ALT_MFP3: PC1_BPWM01 Position */
#define SYS_ALT_MFP3_PC1_BPWM01_Msk             (1ul << SYS_ALT_MFP3_PC1_BPWM01_Pos)        /*!< GCR_T::ALT_MFP3: PC1_BPWM01 Mask */

#define SYS_ALT_MFP3_PC0_BPWM00_Pos             12                                          /*!< GCR_T::ALT_MFP3: PC0_BPWM00 Position */
#define SYS_ALT_MFP3_PC0_BPWM00_Msk             (1ul << SYS_ALT_MFP3_PC0_BPWM00_Pos)        /*!< GCR_T::ALT_MFP3: PC0_BPWM00 Mask */

#define SYS_ALT_MFP3_PF5_PWM15_Pos              11                                          /*!< GCR_T::ALT_MFP3: PF5_PWM15 Position */
#define SYS_ALT_MFP3_PF5_PWM15_Msk              (1ul << SYS_ALT_MFP3_PF5_PWM15_Pos)         /*!< GCR_T::ALT_MFP3: PF5_PWM15 Mask */

#define SYS_ALT_MFP3_PF4_PWM14_Pos              10                                          /*!< GCR_T::ALT_MFP3: PF5_PWM14 Position */
#define SYS_ALT_MFP3_PF4_PWM14_Msk              (1ul << SYS_ALT_MFP3_PF4_PWM14_Pos)         /*!< GCR_T::ALT_MFP3: PF5_PWM14 Mask */

#define SYS_ALT_MFP3_PA11_PWM13_Pos             9                                           /*!< GCR_T::ALT_MFP3: PA11_PWM13 Position */
#define SYS_ALT_MFP3_PA11_PWM13_Msk             (1ul << SYS_ALT_MFP3_PA11_PWM13_Pos)        /*!< GCR_T::ALT_MFP3: PA11_PWM13 Mask */

#define SYS_ALT_MFP3_PA10_PWM12_Pos             8                                           /*!< GCR_T::ALT_MFP3: PA10_PWM12 Position */
#define SYS_ALT_MFP3_PA10_PWM12_Msk             (1ul << SYS_ALT_MFP3_PA10_PWM12_Pos)        /*!< GCR_T::ALT_MFP3: PA10_PWM12 Mask */

#define SYS_ALT_MFP3_PA3_PWM11_Pos              7                                           /*!< GCR_T::ALT_MFP3: PA3_PWM11 Position */
#define SYS_ALT_MFP3_PA3_PWM11_Msk              (1ul << SYS_ALT_MFP3_PA3_PWM11_Pos)         /*!< GCR_T::ALT_MFP3: PA3_PWM11 Mask */

#define SYS_ALT_MFP3_PA2_PWM10_Pos              6                                           /*!< GCR_T::ALT_MFP3: PA2_PWM10 Position */
#define SYS_ALT_MFP3_PA2_PWM10_Msk              (1ul << SYS_ALT_MFP3_PA2_PWM10_Pos)         /*!< GCR_T::ALT_MFP3: PA2_PWM10 Mask */

#define SYS_ALT_MFP3_PA1_PWM05_Pos              5                                           /*!< GCR_T::ALT_MFP3: PA1_PWM05 Position */
#define SYS_ALT_MFP3_PA1_PWM05_Msk              (1ul << SYS_ALT_MFP3_PA1_PWM05_Pos)         /*!< GCR_T::ALT_MFP3: PA1_PWM05 Mask */

#define SYS_ALT_MFP3_PA0_PWM04_Pos              4                                           /*!< GCR_T::ALT_MFP3: PA0_PWM04 Position */
#define SYS_ALT_MFP3_PA0_PWM04_Msk              (1ul << SYS_ALT_MFP3_PA0_PWM04_Pos)         /*!< GCR_T::ALT_MFP3: PA0_PWM04 Mask */

/* GCR ALTMFP4 Bit Field Definitions */
#define SYS_ALT_MFP4_PA7_VREF_Pos               14                                          /*!< GCR_T::ALT_MFP4: PA7_VREF Position */
#define SYS_ALT_MFP4_PA7_VREF_Msk               (1ul << SYS_ALT_MFP4_PA7_VREF_Pos)          /*!< GCR_T::ALT_MFP4: PA7_VREF Mask */

#define SYS_ALT_MFP4_PA1_I2C1SDA_Pos            13                                          /*!< GCR_T::ALT_MFP4: PA1_I2C1SDA Position */
#define SYS_ALT_MFP4_PA1_I2C1SDA_Msk            (1ul << SYS_ALT_MFP4_PA1_I2C1SDA_Pos)       /*!< GCR_T::ALT_MFP4: PA1_I2C1SDA Mask */

#define SYS_ALT_MFP4_PA0_I2C1SCL_Pos            12                                          /*!< GCR_T::ALT_MFP4: PA0_I2C1SCL Position */
#define SYS_ALT_MFP4_PA0_I2C1SCL_Msk            (1ul << SYS_ALT_MFP4_PA0_I2C1SCL_Pos)       /*!< GCR_T::ALT_MFP4: PA0_I2C1SCL Mask */

#define SYS_ALT_MFP4_PC7_I2C0SCL_Pos            11                                          /*!< GCR_T::ALT_MFP4: PC7_I2C0SCL Position */
#define SYS_ALT_MFP4_PC7_I2C0SCL_Msk            (1ul << SYS_ALT_MFP4_PC7_I2C0SCL_Pos)       /*!< GCR_T::ALT_MFP4: PC7_I2C0SCL Mask */

#define SYS_ALT_MFP4_PC6_I2C0SDA_Pos            10                                          /*!< GCR_T::ALT_MFP4: PC6_I2C0SDA Position */
#define SYS_ALT_MFP4_PC6_I2C0SDA_Msk            (1ul << SYS_ALT_MFP4_PC6_I2C0SDA_Pos)       /*!< GCR_T::ALT_MFP4: PC6_I2C0SDA Mask */

#define SYS_ALT_MFP4_PA13_UR5TXD_Pos            9                                           /*!< GCR_T::ALT_MFP4: PA13_UR5TXD Position */
#define SYS_ALT_MFP4_PA13_UR5TXD_Msk            (1ul << SYS_ALT_MFP4_PA13_UR5TXD_Pos)       /*!< GCR_T::ALT_MFP4: PA13_UR5TXD Mask */

#define SYS_ALT_MFP4_PA12_UR5RXD_Pos            8                                           /*!< GCR_T::ALT_MFP4: PA12_UR5RXD Position */
#define SYS_ALT_MFP4_PA12_UR5RXD_Msk            (1ul << SYS_ALT_MFP4_PA12_UR5RXD_Pos)       /*!< GCR_T::ALT_MFP4: PA12_UR5RXD Mask */

#define SYS_ALT_MFP4_PA0_UR5TXD_Pos             7                                           /*!< GCR_T::ALT_MFP4: PA0_UR5TXD Position */
#define SYS_ALT_MFP4_PA0_UR5TXD_Msk             (1ul << SYS_ALT_MFP4_PA0_UR5TXD_Pos)        /*!< GCR_T::ALT_MFP4: PA0_UR5TXD Mask */

#define SYS_ALT_MFP4_PA1_UR5RXD_Pos             6                                           /*!< GCR_T::ALT_MFP4: PA1_UR5RXD Position */
#define SYS_ALT_MFP4_PA1_UR5RXD_Msk             (1ul << SYS_ALT_MFP4_PA1_UR5RXD_Pos)        /*!< GCR_T::ALT_MFP4: PA1_UR5RXD Mask */

#define SYS_ALT_MFP4_PA6_UR3TXD_Pos             5                                           /*!< GCR_T::ALT_MFP4: PA6_UR3TXD Position */
#define SYS_ALT_MFP4_PA6_UR3TXD_Msk             (1ul << SYS_ALT_MFP4_PA6_UR3TXD_Pos)        /*!< GCR_T::ALT_MFP4: PA6_UR3TXD Mask */

#define SYS_ALT_MFP4_PA5_UR3RXD_Pos             4                                           /*!< GCR_T::ALT_MFP4: PA5_UR3RXD Position */
#define SYS_ALT_MFP4_PA5_UR3RXD_Msk             (1ul << SYS_ALT_MFP4_PA5_UR3RXD_Pos)        /*!< GCR_T::ALT_MFP4: PA5_UR3RXD Mask */

#define SYS_ALT_MFP4_PA2_UR3TXD_Pos             3                                           /*!< GCR_T::ALT_MFP4: PA2_UR3TXD Position */
#define SYS_ALT_MFP4_PA2_UR3TXD_Msk             (1ul << SYS_ALT_MFP4_PA2_UR3TXD_Pos)        /*!< GCR_T::ALT_MFP4: PA2_UR3TXD Mask */

#define SYS_ALT_MFP4_PA3_UR3RXD_Pos             2                                           /*!< GCR_T::ALT_MFP4: PA3_UR3RXD Position */
#define SYS_ALT_MFP4_PA3_UR3RXD_Msk             (1ul << SYS_ALT_MFP4_PA3_UR3RXD_Pos)        /*!< GCR_T::ALT_MFP4: PA3_UR3RXD Mask */

#define SYS_ALT_MFP4_PA9_UR1CTS_Pos             1                                           /*!< GCR_T::ALT_MFP4: PA9_UR1CTS Position */
#define SYS_ALT_MFP4_PA9_UR1CTS_Msk             (1ul << SYS_ALT_MFP4_PA9_UR1CTS_Pos)        /*!< GCR_T::ALT_MFP4: PA9_UR1CTS Mask */

#define SYS_ALT_MFP4_PA8_UR1RTS_Pos             0                                           /*!< GCR_T::ALT_MFP4: PA8_UR1RTS Position */
#define SYS_ALT_MFP4_PA8_UR1RTS_Msk             (1ul << SYS_ALT_MFP4_PA8_UR1RTS_Pos)        /*!< GCR_T::ALT_MFP4: PA8_UR1RTS Mask */

/* GCR REGWRPROT Bit Field Definitions */
#define SYS_REGWRPROT_REGWRPROT_Pos             0                                           /*!< GCR_T::REGWRPROT: REGWRPROT Position */
#define SYS_REGWRPROT_REGWRPROT_Msk             (0xFFul << SYS_REGWRPROT_REGWRPROT_Pos)     /*!< GCR_T::REGWRPROT: REGWRPROT Mask */

#define SYS_REGWRPROT_REGPROTDIS_Pos            0                                           /*!< GCR_T::REGWRPROT: REGPROTDIS Position */
#define SYS_REGWRPROT_REGPROTDIS_Msk            (1ul << SYS_REGWRPROT_REGPROTDIS_Pos)       /*!< GCR_T::REGWRPROT: REGPROTDIS Mask */

/*@}*/ /* end of group SYS_CONST */

typedef struct
{
    /**
     * IRQSRC
     * ===================================================================================================
     * Offset: 0x00~0x7C  IRQ0~IRQ31 Interrupt Source Identity
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |INTSRC    |Interrupt Source
     * |        |          |Define the interrupt sources for interrupt event.
     */
    __I uint32_t IRQSRC[32];

    /**
     * NMISEL
     * ===================================================================================================
     * Offset: 0x80  NMI Source Interrupt Select Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[4:0]   |NMISEL    |NMI Interrupt Source Selection
     * |        |          |The NMI interrupt to Cortex-M0 can be selected from one of the peripheral interrupt by setting NMISEL.
     * |[8]     |NMIEN     |NMI Interrupt Enable Bit (Write Protect)
     * |        |          |0 = NMI interrupt Disabled.
     * |        |          |1 = NMI interrupt Enabled.
     * |        |          |Note: This bit is the protected bit, and programming it needs to write "59h", "16h", and "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Refer to the register REGWRPROT at address GCR_BA+0x100.
     */
    __IO uint32_t NMISEL;

    /**
     * MCUIRQ
     * ===================================================================================================
     * Offset: 0x84  MCU Interrupt Request Source Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |MCUIRQ    |MCU IRQ Source Register
     * |        |          |The MCUIRQ collects all the interrupts from the peripherals and generates the synchronous interrupt to Cortex-M0.
     * |        |          |There are two modes to generate interrupt to Cortex-M0, the normal mode and test mode.
     * |        |          |The MCUIRQ collects all interrupts from each peripheral and synchronizes them and interrupts the Cortex-M0.
     * |        |          |When the MCU_IRQ[n] is 0: Set MCU_IRQ[n] 1 will generate an interrupt to Cortex-M0 NVIC[n].
     * |        |          |When the MCU_IRQ[n] is 1 (mean an interrupt is assert), setting 1 to the MCU_IRQ[n] 1 will clear the interrupt and setting MCUIRQ[n] 0: has no effect.
     */
    __IO uint32_t MCUIRQ;

    /**
     * MCUIRQCR
     * ===================================================================================================
     * Offset: 0x88  MCU Interrupt Request Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |FASTIRQ   |Fast IRQ Latency Enable Bit
     * |        |          |0 = MCU IRQ latency is fixed at 13 clock cycles of HCLK, MCU will enter IRQ handler after this fixed latency when interrupt happened.
     * |        |          |1 = MCU IRQ latency will not fixed, MCU will enter IRQ handler as soon as possible when interrupt happened.
     */
    __IO uint32_t MCUIRQCR;

} GCR_INT_T;

/**
    @addtogroup INT_CONST System Interrupt Bit Field Definition
    Constant Definitions for SYS Interrupt Controller
@{ */

/* INT IRQSRC Bit Field Definitions */
#define INT_IRQSRC_INTSRC_Pos                   0                                   /*!< GCR_INT_T::IRQSRC: INTSRC Position */
#define INT_IRQSRC_INTSRC_Msk                   (0xFul << INT_IRQSRC_INTSRC_Pos)    /*!< GCR_INT_T::IRQSRC: INTSRC Mask */

/* INT NMISEL Bit Field Definitions */
#define INT_NMISEL_NMIEN_Pos                    8                                   /*!< GCR_INT_T::NMISEL: NMIEN Position */
#define INT_NMISEL_NMIEN_Msk                    (1ul << INT_NMISEL_NMIEN_Pos)       /*!< GCR_INT_T::NMISEL: NMIEN Mask */

#define INT_NMISEL_NMISEL_Pos                   0                                   /*!< GCR_INT_T::NMISEL: NMISEL Position */
#define INT_NMISEL_NMISEL_Msk                   (0x1Ful << INT_NMISEL_NMISEL_Pos)   /*!< GCR_INT_T::NMISEL: NMISEL Mask */

/* INT MCUIRQ Bit Field Definitions */
#define INT_MCUIRQ_MCUIRQ_Pos                   0                                       /*!< GCR_INT_T::MCUIRQ: MCUIRQ Position */
#define INT_MCUIRQ_MCUIRQ_Msk                   (0xFFFFFFFFul << INT_MCUIRQ_MCUIRQ_Pos) /*!< GCR_INT_T::MCUIRQ: MCUIRQ Mask */

/* INT MCUIRQCR Bit Field Definitions */
#define INT_MCUIRQCR_FASTIRQ_Pos                0                                   /*!< GCR_INT_T::MCUIRQCR: FASTIRQ Position */
#define INT_MCUIRQCR_FASTIRQ_Msk                (1ul << INT_MCUIRQCR_FASTIRQ_Pos)   /*!< GCR_INT_T::MCUIRQCR: FASTIRQ Mask */
/*@}*/ /* end of group INT_CONST */
/*@}*/ /* end of group SYS */




/*----------------------------- Timer Controller (TIMER) -----------------------------*/
/** @addtogroup TIMER Timer Controller (TIMER)
  Memory Mapped Structure for Timer Controller
  @{
 */
typedef struct
{
    /**
     * TCSR
     * ===================================================================================================
     * Offset: 0x00  Timer Control and Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |PRESCALE  |Prescale Counter
     * |        |          |Timer input clock source is divided by (PRESCALE+1) before it is fed to the Timer up counter.
     * |        |          |If this field is 0 (PRESCALE = 0), then there is no scaling.
     * |[16]    |TDR_EN    |Data Load Enable Control
     * |        |          |When TDR_EN is set, TDR (Timer Data Register) will be updated continuously with the 24-bit up-timer value as the timer is counting.
     * |        |          |0 = Timer Data Register update Disabled.
     * |        |          |1 = Timer Data Register update Enabled while Timer counter is active.
     * |[18]    |TRG_SRC_SEL    |Trigger Source Select Bit
     * |        |               |This bit is used to select trigger source is form timer time-out interrupt signal or capture interrupt signal.
     * |        |               |0 = Timer time-out interrupt signal is used to trigger PWM.
     * |        |               |1 = Capture interrupt signal is used to trigger PWM.
     * |[19]    |TRG_PWM_EN     |Trigger PWM Enable Bit
     * |        |               |If this bit is set to 1, timer time-out interrupt or capture interrupt can be triggered PWM.
     * |        |               |0 = Timer interrupt trigger PWM Disabled.
     * |        |               |1 = Timer interrupt trigger PWM Enabled.
     * |[23]    |WAKE_EN   |Wake Up Function Enable Control
     * |        |          |0 = Wake-up trigger event Disabled.
     * |        |          |1 = Wake-up trigger event Enabled.
     * |[24]    |CTB       |Counter Mode Enable Control
     * |        |          |This bit is for external counting pin function enabled.
     * |        |          |When timer is used as an event counter, this bit should be set to 1 and select HCLK as timer clock source.
     * |        |          |0 = External counter mode Disabled.
     * |        |          |1 = External counter mode Enabled.
     * |[25]    |CACT      |Timer Active Status (Read Only)
     * |        |          |This bit indicates the 24-bit up counter status.
     * |        |          |0 = 24-bit up counter is not active.
     * |        |          |1 = 24-bit up counter is active.
     * |[26]    |CRST      |Timer Reset
     * |        |          |0 = No effect.
     * |        |          |1 = Reset 8-bit prescale counter, 24-bit up counter value and CEN bit if CACT is 1.
     * |[28:27] |MODE      |Timer Operating Mode
     * |        |          |00 = The Timer controller is operated in One-shot mode.
     * |        |          |01 = The Timer controller is operated in Periodic mode.
     * |        |          |10 = The Timer controller is operated in Toggle-output mode.
     * |        |          |11 = The Timer controller is operated in Continuous Counting mode.
     * |[29]    |IE        |Interrupt Enable Control
     * |        |          |0 = Timer Interrupt function Disabled.
     * |        |          |1 = Timer Interrupt function Enabled.
     * |        |          |If this bit is enabled, when the timer interrupt flag (TISR[0] TIF) is set to 1, the timer interrupt signal is generated and inform to CPU.
     * |[30]    |CEN       |Timer Enable Control
     * |        |          |0 = Stops/Suspends counting.
     * |        |          |1 = Starts counting.
     * |        |          |Note1: In stop status, and then set CEN to 1 will enable the 24-bit up counter to keep counting from the last stop counting value.
     * |        |          |Note2: This bit is auto-cleared by hardware in one-shot mode (TCSR [28:27] = 00) when the timer interrupt flag (TISR[0] TIF) is generated.
     * |[31]    |DBGACK_TMR|ICE Debug Mode Acknowledge Disable (Write Protect)
     * |        |          |0 = ICE debug mode acknowledgment effects TIMER counting.
     * |        |          |TIMER counter will be held while CPU is held by ICE.
     * |        |          |1 = ICE debug mode acknowledgment Disabled.
     * |        |          |TIMER counter will keep going no matter CPU is held by ICE or not.
     */
    __IO uint32_t  TCSR;

    /**
     * TCMPR
     * ===================================================================================================
     * Offset: 0x04  Timer Compare Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[23:0]  |TCMP      |Timer Compared Value
     * |        |          |TCMP is a 24-bit compared value register.
     * |        |          |When the internal 24-bit up counter value is equal to TCMP value, the TIF flag will set to 1.
     * |        |          |Time-out period = (Period of Timer clock input) * (8-bit PRESCALE + 1) * (24-bit TCMP).
     * |        |          |Note1: Never write 0x0 or 0x1 in TCMP field, or the core will run into unknown state.
     * |        |          |Note2: When timer is operating at continuous counting mode, the 24-bit up counter will keep counting continuously even if user writes a new value into TCMP field.
     * |        |          |But if timer is operating at other modes, the 24-bit up counter will restart counting and using newest TCMP value to be the timer compared value if user writes a new value into TCMP field.
     */
    __IO uint32_t  TCMPR;

    /**
     * TISR
     * ===================================================================================================
     * Offset: 0x08  Timer Interrupt Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |TIF       |Timer Interrupt Flag
     * |        |          |This bit indicates the interrupt flag status of Timer while TDR value reaches to TCMP value.
     * |        |          |0 = No effect.
     * |        |          |1 = TDR value matches the TCMP value.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * |[1]     |TWF       |Timer Wake-Up Flag
     * |        |          |This bit indicates the interrupt wake-up flag status of Timer.
     * |        |          |0 = Timer does not cause CPU wake-up.
     * |        |          |1 = CPU wake-up from Idle or Power-down mode if Timer time-out interrupt signal generated.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     */
    __IO uint32_t  TISR;

    /**
     * TDR
     * ===================================================================================================
     * Offset: 0x0C  Timer Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[23:0]  |TDR       |Timer Data Register
     * |        |          |If TDR_EN (TCSR[16]) is set to 1, TDR register will be updated continuously to monitor 24-bit up counter value.
     */
    __I  uint32_t  TDR;

    /**
     * TCAP
     * ===================================================================================================
     * Offset: 0x10  Timer Capture Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[23:0]  |TCAP      |Timer Capture Data Register
     * |        |          |When TEXIF flag is set to 1, the current TDR value will be auto-loaded into this TCAP filed immediately.
     */
    __I  uint32_t  TCAP;

    /**
     * TEXCON
     * ===================================================================================================
     * Offset: 0x14  Timer External Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |TX_PHASE  |Timer External Count Pin Phase Detect Selection
     * |        |          |This bit indicates the detection phase of TMx pin.
     * |        |          |0 = A falling edge of TMx pin will be counted.
     * |        |          |1 = A rising edge of TMx pin will be counted.
     * |[2:1]   |TEX_EDGE  |Timer External Capture Pin Edge Detect Selection
     * |        |          |00 = A 1 to 0 transition on TMx_EXT pin will be detected.
     * |        |          |01 = A 0 to 1 transition on TMx_EXT pin will be detected.
     * |        |          |10 = Either 1 to 0 or 0 to 1 transition on TMx_EXT pin will be detected.
     * |        |          |11 = Reserved.
     * |[3]     |TEXEN     |Timer External Pin Function Enable
     * |        |          |This bit enables the RSTCAPSEL function on the TxEX pin.
     * |        |          |0 = RSTCAPSEL function of TxEX pin will be ignored.
     * |        |          |1 = RSTCAPSEL function of TxEX pin is active.
     * |[4]     |RSTCAPSEL |Timer External Reset Counter / Timer External Capture Mode Selection
     * |        |          |0 = Transition on TMx_EXT pin is using to save the TDR value into TCAP value if TEXIF flag is set to 1.
     * |        |          |1 = Transition on TMx_EXT pin is using to reset the 24-bit up counter.
     * |[5]     |TEXIEN    |Timer External Capture Interrupt Enable Control
     * |        |          |0 = TMx_EXT pin detection Interrupt Disabled.
     * |        |          |1 = TMx_EXT pin detection Interrupt Enabled.
     * |        |          |If TEXIEN enabled, Timer will raise an external capture interrupt signal and inform to CPU while TEXIF flag is set to 1.
     * |[6]     |TEXDB     |Timer External Capture Input Pin De-Bounce Enable Control
     * |        |          |0 = TMx_EXT pin de-bounce Disabled.
     * |        |          |1 = TMx_EXT pin de-bounce Enabled.
     * |        |          |If this bit is enabled, the edge detection of TMx_EXT pin is detected with de-bounce circuit.
     * |[7]     |TCDB      |Timer External Counter Input Pin De-Bounce Enable Control
     * |        |          |0 = TMx pin de-bounce Disabled.
     * |        |          |1 = TMx pin de-bounce Enabled.
     * |        |          |If this bit is enabled, the edge detection of TMx pin is detected with de-bounce circuit.
     */
    __IO uint32_t  TEXCON;

    /**
     * TEXISR
     * ===================================================================================================
     * Offset: 0x18  Timer External Interrupt Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |TEXIF     |Timer External Capture Interrupt Flag
     * |        |          |This bit indicates the external capture interrupt flag status.
     * |        |          |When TEXEN enabled, TMx_EXT pin selected as external capture function, and a transition on TMx_EXT pin matched the TEX_EDGE setting, this flag will set to 1 by hardware.
     * |        |          |0 = TMx_EXT pin interrupt did not occur.
     * |        |          |1 = TMx_EXT pin interrupt occurred.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     */
    __IO uint32_t  TEXISR;
} TIMER_T;


/** @addtogroup TIMER_CONST TIMER Bit Field Definition
    Constant Definitions for TIMER Controller
  @{
 */

/* TIMER TCSR Bit Field Definitions */
#define TIMER_TCSR_DBGACK_TMR_Pos   31                                          /*!< TIMER_T::TCSR: DBGACK_TMR Position */
#define TIMER_TCSR_DBGACK_TMR_Msk   (1ul << TIMER_TCSR_DBGACK_TMR_Pos)          /*!< TIMER_T::TCSR: DBGACK_TMR Mask */

#define TIMER_TCSR_CEN_Pos          30                                          /*!< TIMER_T::TCSR: CEN Position */
#define TIMER_TCSR_CEN_Msk          (1ul << TIMER_TCSR_CEN_Pos)                 /*!< TIMER_T::TCSR: CEN Mask */

#define TIMER_TCSR_IE_Pos           29                                          /*!< TIMER_T::TCSR: IE Position */
#define TIMER_TCSR_IE_Msk           (1ul << TIMER_TCSR_IE_Pos)                  /*!< TIMER_T::TCSR: IE Mask */

#define TIMER_TCSR_MODE_Pos         27                                          /*!< TIMER_T::TCSR: MODE Position */
#define TIMER_TCSR_MODE_Msk         (0x3ul << TIMER_TCSR_MODE_Pos)              /*!< TIMER_T::TCSR: MODE Mask */

#define TIMER_TCSR_CRST_Pos         26                                          /*!< TIMER_T::TCSR: CRST Position */
#define TIMER_TCSR_CRST_Msk         (1ul << TIMER_TCSR_CRST_Pos)                /*!< TIMER_T::TCSR: CRST Mask */

#define TIMER_TCSR_CACT_Pos         25                                          /*!< TIMER_T::TCSR: CACT Position */
#define TIMER_TCSR_CACT_Msk         (1ul << TIMER_TCSR_CACT_Pos)                /*!< TIMER_T::TCSR: CACT Mask */

#define TIMER_TCSR_CTB_Pos          24                                          /*!< TIMER_T::TCSR: CTB Position */
#define TIMER_TCSR_CTB_Msk          (1ul << TIMER_TCSR_CTB_Pos)                 /*!< TIMER_T::TCSR: CTB Mask */

#define TIMER_TCSR_WAKE_EN_Pos      23                                          /*!< TIMER_T::TCSR: WAKE_EN Position */
#define TIMER_TCSR_WAKE_EN_Msk      (1ul << TIMER_TCSR_WAKE_EN_Pos)             /*!< TIMER_T::TCSR: WAKE_EN Mask */

#define TIMER_TCSR_TRG_PWM_EN_Pos   19                                          /*!< TIMER_T::TCSR: TRG_PWM_EN Position */
#define TIMER_TCSR_TRG_PWM_EN_Msk   (1ul << TIMER_TCSR_TRG_PWM_EN_Pos)          /*!< TIMER_T::TCSR: TRG_PWM_EN Mask */

#define TIMER_TCSR_TRG_SRC_SEL_Pos  18                                          /*!< TIMER_T::TCSR: TRG_SRC_SEL Position */
#define TIMER_TCSR_TRG_SRC_SEL_Msk  (1ul << TIMER_TCSR_TRG_SRC_SEL_Pos)         /*!< TIMER_T::TCSR: TRG_SRC_SEL Mask */

#define TIMER_TCSR_TDR_EN_Pos       16                                          /*!< TIMER_T::TCSR: TDR_EN Position */
#define TIMER_TCSR_TDR_EN_Msk       (1ul << TIMER_TCSR_TDR_EN_Pos)              /*!< TIMER_T::TCSR: TDR_EN Mask */

#define TIMER_TCSR_PRESCALE_Pos     0                                           /*!< TIMER_T::TCSR: PRESCALE Position */
#define TIMER_TCSR_PRESCALE_Msk     (0xFFul << TIMER_TCSR_PRESCALE_Pos)         /*!< TIMER_T::TCSR: PRESCALE Mask */

/* TIMER TCMPR Bit Field Definitions */
#define TIMER_TCMP_TCMP_Pos         0                                           /*!< TIMER_T::TCMPR: TCMP Position */
#define TIMER_TCMP_TCMP_Msk         (0xFFFFFFul << TIMER_TCMP_TCMP_Pos)         /*!< TIMER_T::TCMPR: TCMP Mask */

/* TIMER TISR Bit Field Definitions */
#define TIMER_TISR_TWF_Pos          1                                           /*!< TIMER_T::TISR: TWF Position */
#define TIMER_TISR_TWF_Msk          (1ul << TIMER_TISR_TWF_Pos)                 /*!< TIMER_T::TISR: TWF Mask */

#define TIMER_TISR_TIF_Pos          0                                           /*!< TIMER_T::TISR: TIF Position */
#define TIMER_TISR_TIF_Msk          (1ul << TIMER_TISR_TIF_Pos)                 /*!< TIMER_T::TISR: TIF Mask */

/* TIMER TDR Bit Field Definitions */
#define TIMER_TDR_TDR_Pos           0                                           /*!< TIMER_T::TDR: TDR Position */
#define TIMER_TDR_TDR_Msk           (0xFFFFFFul << TIMER_TDR_TDR_Pos)           /*!< TIMER_T::TDR: TDR Mask */

/* TIMER TCAP Bit Field Definitions */
#define TIMER_TCAP_TCAP_Pos         0                                           /*!< TIMER_T::TCAP: TCAP Position */
#define TIMER_TCAP_TCAP_Msk         (0xFFFFFFul << TIMER_TCAP_TCAP_Pos)         /*!< TIMER_T::TCAP: TCAP Mask */

/* TIMER TEXCON Bit Field Definitions */
#define TIMER_TEXCON_TCDB_Pos       7                                           /*!< TIMER_T::TEXCON: TCDB Position */
#define TIMER_TEXCON_TCDB_Msk       (1ul << TIMER_TEXCON_TCDB_Pos)              /*!< TIMER_T::TEXCON: TCDB Mask */

#define TIMER_TEXCON_TEXDB_Pos      6                                           /*!< TIMER_T::TEXCON: TEXDB Position */
#define TIMER_TEXCON_TEXDB_Msk      (1ul << TIMER_TEXCON_TEXDB_Pos)             /*!< TIMER_T::TEXCON: TEXDB Mask */

#define TIMER_TEXCON_TEXIEN_Pos     5                                           /*!< TIMER_T::TEXCON: TEXIEN Position */
#define TIMER_TEXCON_TEXIEN_Msk     (1ul << TIMER_TEXCON_TEXIEN_Pos)            /*!< TIMER_T::TEXCON: TEXIEN Mask */

#define TIMER_TEXCON_RSTCAPSEL_Pos  4                                           /*!< TIMER_T::TEXCON: RSTCAPSEL Position */
#define TIMER_TEXCON_RSTCAPSEL_Msk  (1ul << TIMER_TEXCON_RSTCAPSEL_Pos)         /*!< TIMER_T::TEXCON: RSTCAPSEL Mask */

#define TIMER_TEXCON_TEXEN_Pos      3                                           /*!< TIMER_T::TEXCON: TEXEN Position */
#define TIMER_TEXCON_TEXEN_Msk      (1ul << TIMER_TEXCON_TEXEN_Pos)             /*!< TIMER_T::TEXCON: TEXEN Mask */

#define TIMER_TEXCON_TEX_EDGE_Pos   1                                           /*!< TIMER_T::TEXCON: TEX_EDGE Position */
#define TIMER_TEXCON_TEX_EDGE_Msk   (0x3ul << TIMER_TEXCON_TEX_EDGE_Pos)        /*!< TIMER_T::TEXCON: TEX_EDGE Mask */

#define TIMER_TEXCON_TX_PHASE_Pos   0                                           /*!< TIMER_T::TEXCON: TX_PHASE Position */
#define TIMER_TEXCON_TX_PHASE_Msk   (1ul << TIMER_TEXCON_TX_PHASE_Pos)          /*!< TIMER_T::TEXCON: TX_PHASE Mask */

/* TIMER TEXISR Bit Field Definitions */
#define TIMER_TEXISR_TEXIF_Pos      0                                           /*!< TIMER_T::TEXISR: TEXIF Position */
#define TIMER_TEXISR_TEXIF_Msk      (1ul << TIMER_TEXISR_TEXIF_Pos)             /*!< TIMER_T::TEXISR: TEXIF Mask */
/*@}*/ /* end of group TIMER_CONST */
/*@}*/ /* end of group TIMER */





/*------------------------- UART Interface Controller ------------------------*/
/**
    @addtogroup UART Universal Asynchronous Receiver/Transmitter Controller (UART)
    Memory Mapped Structure for UART Controller
@{ */
typedef struct
{

    union
    {
        __IO uint32_t DATA;

        /**
         * THR
         * ===================================================================================================
         * Offset: 0x00 Transmit Holding DATA
         * ---------------------------------------------------------------------------------------------------
         * |Bits    |Field     |Descriptions
         * | :----: | :----:   | :---- |
         * |[7:0]   |THR       |Transmit Holding Register
         * |        |          |By writing to this register, the UART will send out an 8-bit data through the Tx pin (LSB first).
         */
        __IO uint32_t THR;

        /**
         * UA_RBR
         * ===================================================================================================
         * Offset: 0x00  UART Receive Buffer Register
         * ---------------------------------------------------------------------------------------------------
         * |Bits    |Field     |Descriptions
         * | :----: | :----:   | :---- |
         * |[7:0]   |RBR       |Receive Buffer Register (Read Only)
         * |        |          |By reading this register, the UART will return the 8-bit data received from RX pin (LSB first).
         */
        __IO uint32_t RBR;
    };

    /**
     * UA_IER
     * ===================================================================================================
     * Offset: 0x04  UART Interrupt Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RDA_IEN   |Receive Data Available Interrupt Enable Control
     * |        |          |0 = RDA_INT Masked off.
     * |        |          |1 = RDA_INT Enabled.
     * |[1]     |THRE_IEN  |Transmit Holding Register Empty Interrupt Enable Control
     * |        |          |0 = THRE_INT Masked off.
     * |        |          |1 = THRE_INT Enabled.
     * |[2]     |RLS_IEN   |Receive Line Status Interrupt Enable Control
     * |        |          |0 = RLS_INT Masked off.
     * |        |          |1 = RLS_INT Enabled
     * |[3]     |MODEM_IEN |Modem Status Interrupt Enable Control (Available In UART0/UART1 Channel)
     * |        |          |0 = MODEM_INT Masked off.
     * |        |          |1 = MODEM_INT Enabled.
     * |[4]     |TOUT_IEN  |RX Time-Out Interrupt Enable Control
     * |        |          |0 = TOUT_INT Masked off.
     * |        |          |1 = TOUT_INT Enabled.
     * |[5]     |BUF_ERR_IEN|Buffer Error Interrupt Enable Control
     * |        |          |0 = BUF_ERR_INT Masked off.
     * |        |          |1 = BUF_ERR_INT Enabled.
     * |[6]     |WKCTSIEN  |nCTS Wake-Up Interrupt Enable Bit (Available In UART0/UART1 Channel)
     * |        |          |0 = nCTS wake-up system function Disabled.
     * |        |          |1 = Wake-up system function Enabled, when the system is in Power-down mode, an external nCTS change will wake-up system from Power-down mode.
     * |[8]     |LIN_IEN   |LIN Bus Interrupt Enable
     * |        |          |0 = Lin bus interrupt Disabled.
     * |        |          |1 = Lin bus interrupt Enabled.
     * |        |          |Note: This field is used for LIN function mode.
     * |[10]    |WKDATIEN  |Incoming Data Wake-Up Interrupt Enable Bit
     * |        |          |0 = Incoming data wake-up system function Disabled.
     * |        |          |1 = Incoming data wake-up system function Enabled, when the system is in Power-down mode, incoming data will wake-up system from Power-down mode.
     * |        |          |Note: Hardware will clear this bit when the incoming data wake-up operation finishes and "system clock" work stable.
     * |[11]    |TIME_OUT_EN|Time-Out Counter Enable
     * |        |          |0 = Time-out counter Disabled.
     * |        |          |1 = Time-out counter Enabled.
     * |[12]    |AUTO_RTS_EN|RTS Auto Flow Control Enable (Available In UART0/UART1 Channel)
     * |        |          |0 = RTS auto flow control Disabled.
     * |        |          |1 = RTS auto flow control Enabled.
     * |        |          |When RTS auto-flow is enabled, if the number of bytes in the RX FIFO equals the RTS_TRI_LEV (UA_FCR [19:16]), the UART will de-assert RTS signal.
     * |[13]    |AUTO_CTS_EN|CTS Auto Flow Control Enable (Available In UART0/UART1 Channel)
     * |        |          |0 = CTS auto flow control Disabled.
     * |        |          |1 = CTS auto flow control Enabled.
     * |        |          |When CTS auto-flow is enabled, the UART will send data to external device when CTS input assert (UART will not send data to device until CTS is asserted).
     * |[18]    |ABRIEN    |Auto-Baud Rate Interrupt Enable Bit
     * |        |          |0 = Auto-baud rate interrupt Disabled.
     * |        |          |1 = Auto-baud rate interrupt Enabled.
     */
    __IO uint32_t IER;

    /**
     * UA_FCR
     * ===================================================================================================
     * Offset: 0x08  UART FIFO Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1]     |RFR       |RX Field Software Reset
     * |        |          |When RFR is set, all the byte in the receiver FIFO and RX internal state machine are cleared.
     * |        |          |0 = No effect.
     * |        |          |1 = Reset the RX internal state machine and pointers.
     * |        |          |Note: This bit will automatically clear at least 3 UART peripheral clock cycles.
     * |[2]     |TFR       |TX Field Software Reset
     * |        |          |When TFR is set, all the byte in the transmit FIFO and TX internal state machine are cleared.
     * |        |          |0 = No effect.
     * |        |          |1 = Reset the TX internal state machine and pointers.
     * |        |          |Note: This bit will automatically clear at least 3 UART peripheral clock cycles.
     * |[7:4]   |RFITL     |RX FIFO Interrupt Trigger Level
     * |        |          |When the number of bytes in the receive FIFO equals the RFITL, the RDA_IF will be set (if RDA_IEN(UA_IER[0]) enabled, and an interrupt will be generated).
     * |        |          |0000 = RX FIFO Interrupt Trigger Level is 1 byte.
     * |        |          |0001 = RX FIFO Interrupt Trigger Level is 4 bytes.
     * |        |          |0010 = RX FIFO Interrupt Trigger Level is 8 bytes.
     * |        |          |0011 = RX FIFO Interrupt Trigger Level is 14 bytes.
     * |        |          |Other = Reserved.
     * |[8]     |RX_DIS    |Receiver Disable Register
     * |        |          |The receiver is disabled or not (set 1 to disable receiver).
     * |        |          |0 = Receiver Enabled.
     * |        |          |1 = Receiver Disabled.
     * |        |          |Note: This field is used for RS-485 Normal Multi-drop mode.
     * |        |          |It should be programmed before RS485_NMM(UA_ALT_CSR[8]) is programmed.
     * |[19:16] |RTS_TRI_LEV|RTS Trigger Level For Auto-Flow Control Use (Available In UART0/UART1 Channel)
     * |        |          |0000 = RTS Trigger Level is 1 byte.
     * |        |          |0001 = RTS Trigger Level is 4 bytes.
     * |        |          |0010 = RTS Trigger Level is 8 bytes.
     * |        |          |0011 = RTS Trigger Level is 14 bytes.
     * |        |          |Other = Reserved.
     * |        |          |Note: This field is used for RTS auto-flow control.
     */
    __IO uint32_t FCR;

    /**
     * UA_LCR
     * ===================================================================================================
     * Offset: 0x0C  UART Line Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |WLS       |Word Length Selection
     * |        |          |00 = Word length is 5-bit.
     * |        |          |01 = Word length is 6-bit.
     * |        |          |10 = Word length is 7-bit
     * |        |          |11 = Word length is 8-bit
     * |[2]     |NSB       |Number Of "STOP Bit"
     * |        |          |0 = One " STOP bit" is generated in the transmitted data.
     * |        |          |1 = When select 5-bit word length, 1.5 "STOP bit" is generated in the transmitted data.
     * |        |          |When select 6-,7- and 8-bit word length, 2 "STOP bit" is generated in the transmitted data.
     * |[3]     |PBE       |Parity Bit Enable
     * |        |          |0 = No parity bit.
     * |        |          |1 = Parity bit is generated on each outgoing character and is checked on each incoming data.
     * |[4]     |EPE       |Even Parity Enable
     * |        |          |0 = Odd number of logic 1's is transmitted and checked in each word.
     * |        |          |1 = Even number of logic 1's is transmitted and checked in each word.
     * |        |          |This bit has effect only when PBE (UA_LCR[3]) is set.
     * |[5]     |SPE       |Stick Parity Enable
     * |        |          |0 = Stick parity Disabled.
     * |        |          |1 = If PBE (UA_LCR[3]) and EBE (UA_LCR[4]) are logic 1, the parity bit is transmitted and checked as logic 0.
     * |        |          |If PBE (UA_LCR[3]) is 1 and EBE (UA_LCR[4]) is 0 then the parity bit is transmitted and checked as 1.
     * |[6]     |BCB       |Break Control Bit
     * |        |          |When this bit is set to logic 1, the serial data output (TX) is forced to the Spacing State (logic 0).
     * |        |          |This bit acts only on TX and has no effect on the transmitter logic.
     */
    __IO uint32_t LCR;

    /**
     * UA_MCR
     * ===================================================================================================
     * Offset: 0x10  UART Modem Control Register (Available In UART0/UART1 Channel)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1]     |RTS       |RTS (Request-To-Send) Signal Control
     * |        |          |This bit is direct control internal RTS signal active or not, and then drive the RTS pin output with LEV_RTS bit configuration.
     * |        |          |0 = RTS signal is active.
     * |        |          |1 = RTS signal is inactive.
     * |        |          |Note1: This RTS signal control bit is not effective when RTS auto-flow control is enabled in UART function mode.
     * |        |          |Note2: This RTS signal control bit is not effective when RS-485 auto direction mode (AUD) is enabled in RS-485 function mode.
     * |[9]     |LEV_RTS   |RTS Pin Active Level 
     * |        |          |This bit defines the active level state of RTS pin output.
     * |        |          |0 = RTS pin output is high level active.
     * |        |          |1 = RTS pin output is low level active.
     * |[13]    |RTS_ST    |RTS Pin State (Read Only)
     * |        |          |This bit mirror from RTS pin output of voltage logic status.
     * |        |          |0 = RTS pin output is low level voltage logic state.
     * |        |          |1 = RTS pin output is high level voltage logic state.
     */
    __IO uint32_t MCR;

    /**
     * UA_MSR
     * ===================================================================================================
     * Offset: 0x14  UART Modem Status Register (Available In UART0/UART1 Channel)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |DCTSF     |Detect CTS State Change Flag (Read Only)
     * |        |          |This bit is set whenever CTS input has change state, and it will generate Modem interrupt to CPU when MODEM_IEN (UA_IER [3]) is set to 1.
     * |        |          |0 = CTS input has not change state.
     * |        |          |1 = CTS input has change state.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[4]     |CTS_ST    |CTS Pin Status (Read Only) 
     * |        |          |This bit mirror from CTS pin input of voltage logic status.
     * |        |          |0 = CTS pin input is low level voltage logic state.
     * |        |          |1 = CTS pin input is high level voltage logic state.
     * |        |          |Note: This bit echoes when UART Controller peripheral clock is enabled, and CTS multi-function port is selected
     * |[8]     |LEV_CTS   |CTS Pin Active Level
     * |        |          |This bit defines the active level state of CTS pin input.
     * |        |          |0 = CTS pin input is high level active.
     * |        |          |1 = CTS pin input is low level active.
     */
    __IO uint32_t MSR;

    /**
     * UA_FSR
     * ===================================================================================================
     * Offset: 0x18  UART FIFO Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RX_OVER_IF|RX Overflow Error IF (Read Only)
     * |        |          |This bit is set when RX FIFO overflow.
     * |        |          |If the number of bytes of received data is greater than RX_FIFO (UA_RBR) size, 16 bytes of UART0/UART1/UART2, this bit will be set.
     * |        |          |0 = RX FIFO is not overflow.
     * |        |          |1 = RX FIFO is overflow.
     * |        |          |If the number of bytes of received data is greater than 1 byte, 1 byte of UART3/UART4/UART5, this bit will be set.
     * |        |          |0 = RX Buffer is not overflow.
     * |        |          |1 = RX Buffer is overflow.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[1]     |ABRDIF    |Auto-Baud Rate Detect Interrupt (Read Only)
     * |        |          |0 = Auto-baud rate detect function is not finished.
     * |        |          |1 = Auto-baud rate detect function is finished.
     * |        |          |This bit is set to logic "1" when auto-baud rate detect function is finished.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[2]     |ABRDTOIF  |Auto-Baud Rate Time-Out Interrupt (Read Only)
     * |        |          |0 = Auto-baud rate counter is underflow.
     * |        |          |1 = Auto-baud rate counter is overflow.
     * |        |          |Note1: This bit is set to logic "1" in Auto-baud Rate Detect mode and the baud rate counter is overflow.
     * |        |          |Note2: This bit is read only, but can be cleared by writing "1" to it.
     * |[3]     |RS485_ADD_DETF|RS-485 Address Byte Detection Flag (Read Only) (Available In UART0/UART1 Channel)
     * |        |          |0 = Receiver detects a data that is not an address bit (bit 9 ='1').
     * |        |          |1 = Receiver detects a data that is an address bit (bit 9 ='1').
     * |        |          |Note1: This field is used for RS-485 function mode and RS485_ADD_EN (UA_ALT_CSR[15]) is set to 1 to enable Address detection mode.
     * |        |          |Note2: This bit is read only, but can be cleared by writing '1' to it.
     * |[4]     |PEF       |Parity Error Flag (Read Only)
     * |        |          |This bit is set to logic 1 whenever the received character does not have a valid "parity bit", and is reset whenever the CPU writes 1 to this bit.
     * |        |          |0 = No parity error is generated.
     * |        |          |1 = Parity error is generated.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[5]     |FEF       |Framing Error Flag (Read Only)
     * |        |          |This bit is set to logic 1 whenever the received character does not have a valid "stop bit" (that is, the stop bit following the last data bit or parity bit is detected as logic 0), and is reset whenever the CPU writes 1 to this bit.
     * |        |          |0 = No framing error is generated.
     * |        |          |1 = Framing error is generated.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[6]     |BIF       |Break Interrupt Flag (Read Only)
     * |        |          |This bit is set to logic 1 whenever the received data input(RX) is held in the "spacing state" (logic 0) for longer than a full word transmission time (that is, the total time of "start bit" + data bits + parity + stop bits) and is reset whenever the CPU writes 1 to this bit.
     * |        |          |0 = No Break interrupt is generated.
     * |        |          |1 = Break interrupt is generated.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[13:8]  |RX_POINTER|RX FIFO Pointer (Read Only)
     * |        |          |This field indicates the RX FIFO Buffer Pointer.
     * |        |          |When UART receives one byte from external device, then RX_POINTER increases one. '
     * |        |          |When one byte of RX FIFO is read by CPU, then RX_POINTER decreases one.
     * |        |          |The Maximum value shown in RX_POINTER is 15 (UART0/UART1/UART2).
     * |        |          |When the using level of RX FIFO Buffer equal to 16, the RX_FULL bit is set to 1 and RX_POINTER will show 0.
     * |        |          |As one byte of RX FIFO is read by CPU, the RX_FULL bit is cleared to 0 and RX_POINTER will show 15 (UART0/UART1/UART2).
     * |        |          |When RX Buffer is equal to 1, if one byte data is received, the RX_FULL bit is set to 1 and RX_POINTER will show 1.
     * |        |          |Once the RX Buffer is read, the RX_POINTER is 0.
     * |[14]    |RX_EMPTY  |Receiver FIFO Empty (Read Only)
     * |        |          |This bit initiate RX FIFO empty or not. (UART0/UART1/UART2)
     * |        |          |0 = RX FIFO is not empty.
     * |        |          |1 = RX FIFO is empty.
     * |        |          |Note: When the last byte of RX FIFO has been read by CPU, hardware sets this bit high. It will be cleared when UART receives any new data.
     * |        |          |This bit initiate RX Buffer empty or not. (UART3/UART4/UART5)
     * |        |          |0 = RX Buffer is not empty.
     * |        |          |1 = RX Buffer is empty.
     * |        |          |Note: When the last byte of RX Buffer has been read by CPU, hardware sets this bit high.
     * |        |          |It will be cleared when UART receives any new data.
     * |[15]    |RX_FULL   |Receiver FIFO Full (Read Only)
     * |        |          |This bit initiates RX FIFO is full or not (UART0/UART1/UART2).
     * |        |          |0 = RX FIFO is not full.
     * |        |          |1 = RX FIFO is full.
     * |        |          |Note: This bit is set when the number of usage in RX FIFO Buffer is equal to 16 (UART0/UART1/UART2), otherwise is cleared by hardware.
     * |        |          |This bit initiates RX Buffer is full or not (UART3/UART4/UART5).
     * |        |          |0 = RX buffer is not full.
     * |        |          |1 = RX buffer is full.
     * |        |          |Note: This bit is set when the number of usage in RX Buffer is equal to 1 (UART3/UART4/UART5), otherwise is cleared by hardware.
     * |[21:16] |TX_POINTER|TX FIFO Pointer (Read Only)
     * |        |          |This field indicates the TX FIFO Buffer Pointer.
     * |        |          |When CPU writes one byte into UA_THR, then TX_POINTER increases one.
     * |        |          |When one byte of TX FIFO is transferred to Transmitter Shift Register, then TX_POINTER decreases one.
     * |        |          |The Maximum value shown in TX_POINTER is 15 (UART0/UART1/UART2).
     * |        |          |When the using level of TX FIFO Buffer is equal to 16, the TX_FULL bit is set to 1 and TX_POINTER will show 0.
     * |        |          |As one byte of TX FIFO is transferred to Transmitter Shift Register, the TX_FULL bit is cleared to 0 and TX_POINTER will show 15 (UART0/UART1/UART2).
     * |        |          |TX_POINTER is 0 (UART3/UART4/UART5).
     * |        |          |When TX Buffer is equal to 1, if one byte data is received, the TX_FULL bit is set to 1 and TX_POINTER will show 1.
     * |        |          |Once the TX Buffer is read, the TX_POINTER is 0.
     * |[22]    |TX_EMPTY  |Transmitter FIFO Empty (Read Only)
     * |        |          |This bit indicates TX FIFO empty or not. (UART0/UART1/UART2)
     * |        |          |0 = TX FIFO is not empty.
     * |        |          |1 = TX FIFO is empty.
     * |        |          |Note: When the last byte of TX FIFO has been transferred to Transmitter Shift Register, hardware sets this bit high.
     * |        |          |It will be cleared when writing data into THR (TX FIFO not empty).
     * |        |          |This bit indicates TX Buffer filled or not. (UART3/UART4/UART5)
     * |        |          |0 = TX Buffer is not empty.
     * |        |          |1 = TX Buffer is empty.
     * |        |          |Note: When the last byte of TX Buffer has been transferred to Transmitter Shift Register, hardware sets this bit high.
     * |        |          |It will be cleared when writing data into THR (TX FIFO not empty).
     * |[23]    |TX_FULL   |Transmitter FIFO Full (Read Only)
     * |        |          |This bit indicates TX FIFO is full or not. (UART0/UART1/UART2)
     * |        |          |0 = TX FIFO is not full.
     * |        |          |1 = TX FIFO is full.
     * |        |          |This bit is set when the number of usage in TX FIFO is equal to 16 (UART0/UART1/UART2), otherwise is cleared by hardware.
     * |        |          |This bit indicates TX Buffer is full or not.(UART3/UART4/UART5)
     * |        |          |0 = TX Buffer is not full.
     * |        |          |1 = TX Buffer is full.
     * |        |          |This bit is set when the number of usage in TX Buffer is equal to 1 (UART3/UART4/UART5), otherwise is cleared by hardware.
     * |[24]    |TX_OVER_IF|TX Overflow Error Interrupt Flag (Read Only)
     * |        |          |If TX FIFO (UA_THR) is full, an additional write to UA_THR will cause this bit to logic 1. (UART0/UART1/UART2)
     * |        |          |0 = TX FIFO is not overflow.
     * |        |          |1 = TX FIFO is overflow.
     * |        |          |If TX Buffer is filled, an additional write to UA_THR will cause this bit to logic 1. (UART3/UART4/UART5)
     * |        |          |0 = TX Buffer is not overflow.
     * |        |          |1 = TX Buffer is overflow.
     * |        |          |Note: This bit is read only, but can be cleared by writing "1" to it.
     * |[28]    |TE_FLAG   |Transmitter Empty Flag (Read Only)
     * |        |          |This bit is set by hardware when TX FIFO (UA_THR) is empty and the STOP bit of the last byte has been transmitted. (UART0/UART1/UART2)
     * |        |          |0 = TX FIFO is not empty.
     * |        |          |1 = TX FIFO is empty.
     * |        |          |This bit is set by hardware when TX Buffer (UA_THR) is empty and the STOP bit of the last byte has been transmitted. (UART3/UART4/UART5)
     * |        |          |0 = TX Buffer is not empty.
     * |        |          |1 = TX Buffer is empty.
     * |        |          |Note: This bit is cleared automatically when TX FIFO/TX Buffer is not empty or the last byte transmission has not completed.
      */
    __IO uint32_t FSR;

    /**
     * UA_ISR
     * ===================================================================================================
     * Offset: 0x1C  UART Interrupt Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RDA_IF    |Receive Data Available Interrupt Flag (Read Only)
     * |        |          |When the number of bytes in the RX FIFO equals the RFITL then the RDA_IF(UA_ISR[0]) will be set.
     * |        |          |If RDA_IEN (UA_IER [0]) is enabled, the RDA interrupt will be generated.
     * |        |          |0 = No RDA interrupt flag is generated.
     * |        |          |1 = RDA interrupt flag is generated.
     * |        |          |Note: This bit is read only and it will be cleared when the number of unread bytes of RX FIFO drops below the threshold level (RFITL(UA_FCR[7:4]).
     * |[1]     |THRE_IF   |Transmit Holding Register Empty Interrupt Flag (Read Only)
     * |        |          |This bit is set when the last data of TX FIFO is transferred to Transmitter Shift Register.
     * |        |          |If THRE_IEN (UA_IER[1]) is enabled, the THRE interrupt will be generated.
     * |        |          |0 = No THRE interrupt flag is generated.
     * |        |          |1 = THRE interrupt flag is generated.
     * |        |          |Note: This bit is read only and it will be cleared when writing data into THR (TX FIFO not empty).
     * |[2]     |RLS_IF    |Receive Line Interrupt Flag (Read Only)
     * |        |          |This bit is set when the RX receive data have parity error, frame error or break error (at least one of 3 bits, BIF(UA_FSR[6]), FEF(UA_FSR[5]) and PEF(UA_FSR[4]), is set).
     * |        |          |If RLS_IEN (UA_IER [2]) is enabled, the RLS interrupt will be generated.
     * |        |          |0 = No RLS interrupt flag is generated.
     * |        |          |1 = RLS interrupt flag is generated.
     * |        |          |Note1: In RS-485 function mode, this field is set include receiver detect and received address byte character (bit9 = '1') bit.
     * |        |          |At the same time, the bit of UA_FSR[RS485_ADD_DETF] is also set.
     * |        |          |Note2: This bit is read only and reset to 0 when all bits of BIF(UA_FSR[6]), FEF(UA_FSR[5]) and PEF(UA_FSR[4]) are cleared.
     * |        |          |Note3: In RS-485 function mode, this bit is read only and reset to 0 when all bits of BIF(UA_FSR[6]) , FEF(UA_FSR[5]) and PEF(UA_FSR[4]) and RS485_ADD_DETF (UA_FSR[3]) are cleared.
     * |[3]     |MODEM_IF  |MODEM Interrupt Flag (Read Only) (Available In UART0/UART1 Channel)
     * |        |          |This bit is set when the CTS pin has state change (DCTSF (UA_MSR[0]) = 1).
     * |        |          |If MODEM_IEN (UA_IER [3]) is enabled, the Modem interrupt will be generated.
     * |        |          |0 = No Modem interrupt flag is generated.
     * |        |          |1 = Modem interrupt flag is generated.
     * |        |          |Note: This bit is read only and reset to 0 when bit DCTSF is cleared by a write 1 on DCTSF(UA_MSR[0]).
     * |[4]     |TOUT_IF   |Time-Out Interrupt Flag (Read Only)
     * |        |          |This bit is set when the RX FIFO is not empty and no activities occurred in the RX FIFO and the time-out counter equal to TOIC.
     * |        |          |If TOUT_IEN (UA_IER [4]) is enabled, the Tout interrupt will be generated.
     * |        |          |0 = No Time-out interrupt flag is generated.
     * |        |          |1 = Time-out interrupt flag is generated.
     * |        |          |Note: This bit is read only and user can read UA_RBR (RX is in active) to clear it
     * |[5]     |BUF_ERR_IF|Buffer Error Interrupt Flag (Read Only)
     * |        |          |This bit is set when the TX FIFO or RX FIFO overflows (TX_OVER_IF (UA_FSR[24]) or RX_OVER_IF (UA_FSR[0]) is set).
     * |        |          |When BUF_ERR_IF (UA_ISR[5])is set, the transfer is not correct.
     * |        |          |If BUF_ERR_IEN (UA_IER [8]) is enabled, the buffer error interrupt will be generated.
     * |        |          |0 = No buffer error interrupt flag is generated.
     * |        |          |1 = Buffer error interrupt flag is generated.0 = No buffer error interrupt flag is generated.
     * |        |          |1 = Buffer error interrupt flag is generated.
     * |        |          |Note: This bit is read only and reset to 0 when all bits of TX_OVER_IF(UA_FSR[24]) and RX_OVER_IF(UA_FSR[0]) are cleared
     * |[6]     |WKIF      |UART Wake-up Interrupt Flag (Read Only)
     * |        |          |This bit is set when DATWKIF (UA_ISR[17]) or CTSWKIF(UA_ISR[16]) is set to 1.
     * |        |          |0 = No DATWKIF and CTSWKIF are generated.
     * |        |          |1 = DATWKIF or CTSWKIF.
     * |        |          |Note: This bit is read only.
     * |        |          |This bit is cleared if both of DATWKIF (UA_ISR[17]) and CTSWKIF(UA_ISR[16]) are cleared to 0 by writing 1 to DATWKIF (UA_ISR[17]) and CTSWKIF (UA_ISR[17]).
     * |[7]     |LIN_IF    |LIN Bus Flag (Read Only)
     * |        |          |This bit is set when LIN slave header detect (LINS_HDET_F (UA_LIN_SR[0] =1)), LIN break detect (LIN_BKDET_F(UA_LIN_SR[9]=1)), bit error detect (BIT_ERR_F(UA_LIN_SR[9]=1), LIN slave ID parity error (LINS_IDPERR_F(UA_LIN_SR[2] = 1) or LIN slave header error detect (LINS_HERR_F (UA_LIN_SR[1])).
     * |        |          |If LIN_ IEN (UA_IER [8]) is enabled the LIN interrupt will be generated.
     * |        |          |0 = None of LINS_HDET_F, LIN_BKDET_F, BIT_ERR_F, LINS_IDPERR_F and LINS_HERR_F is generated.
     * |        |          |1 = At least one of LINS_HDET_F, LIN_BKDET_F, BIT_ERR_F, LINS_IDPERR_F and LINS_HERR_F is generated.
     * |        |          |Note: This bit is read only.
     * |        |          |This bit is cleared when LINS_HDET_F(UA_LIN_SR[0]), LIN_BKDET_F(UA_LIN_SR[9]), BIT_ERR_F(UA_LIN_SR[9]), LINS_IDPENR_F (UA_LIN_SR[2]) and LINS_HERR_F(UA_LIN_SR[1]) all are cleared.
     * |[8]     |RDA_INT   |Receive Data Available Interrupt Indicator (Read Only)
     * |        |          |This bit is set if RDA_IEN (UA_IER[0]) and RDA_IF (UA_ISR[0]) are both set to 1.
     * |        |          |0 = No RDA interrupt is generated.
     * |        |          |1 = RDA interrupt is generated.
     * |[9]     |THRE_INT  |Transmit Holding Register Empty Interrupt Indicator (Read Only)
     * |        |          |This bit is set if THRE_IEN (UA_IER[1])and THRE_IF(UA_SR[1]) are both set to 1.
     * |        |          |0 = No THRE interrupt is generated.
     * |        |          |1 = THRE interrupt is generated.
     * |[10]    |RLS_INT   |Receive Line Status Interrupt Indicator (Read Only)
     * |        |          |This bit is set if RLS_IEN (UA_IER[2]) and RLS_IF(UA_ISR[2]) are both set to 1.
     * |        |          |0 = No RLS interrupt is generated.
     * |        |          |1 = RLS interrupt is generated
     * |[11]    |MODEM_INT |MODEM Status Interrupt Indicator (Read Only) (Not Available In UART2 Channel)
     * |        |          |This bit is set if MODEM_IEN(UA_IER[3] and MODEM_IF(UA_ISR[4]) are both set to 1
     * |        |          |0 = No Modem interrupt is generated.
     * |        |          |1 = Modem interrupt is generated.
     * |[12]    |TOUT_INT  |Time-Out Interrupt Indicator (Read Only)
     * |        |          |This bit is set if TOUT_IEN(UA_IER[4]) and TOUT_IF(UA_ISR[4]) are both set to 1.
     * |        |          |0 = No Tout interrupt is generated.
     * |        |          |1 = Tout interrupt is generated.
     * |[13]    |BUF_ERR_INT|Buffer Error Interrupt Indicator (Read Only)
     * |        |          |This bit is set if BUF_ERR_IEN(UA_IER[5] and BUF_ERR_IF(UA_ISR[5]) are both set to 1.
     * |        |          |0 = No buffer error interrupt is generated.
     * |        |          |1 = Buffer error interrupt is generated.
     * |[15]    |LIN_INT   |LIN Bus Interrupt Indicator (Read Only)
     * |        |          |This bit is set if LIN_IEN (UA_IER[8]) and LIN _IF(UA_ISR[7]) are both set to 1.
     * |        |          |0 = No LIN Bus interrupt is generated.
     * |        |          |1 = The LIN Bus interrupt is generated.
     * |[16]    |CTSWKIF   |nCTS Wake-Up Interrupt Flag (Read Only) (Available In UART0/UART1 Channel)
     * |        |          |0 = Chip stays in power-down state.
     * |        |          |1 = Chip wake-up from power-down state by nCTS wake-up.
     * |        |          |Note1: If WKCTSIEN (UA_IER[6])is enabled, the wake-up interrupt is generated.
     * |        |          |Note2: This bit is read only, but can be cleared by writing '1' to it.
     * |[17]    |DATWKIF   |Data Wake-Up Interrupt Flag (Read Only)
     * |        |          |This bit is set if chip wake-up from power-down state by data wake-up.
     * |        |          |0 = Chip stays in power-down state.
     * |        |          |1 = Chip wake-up from power-down state by data wake-up.
     * |        |          |Note1: If WKDATIEN (UA_IER[10]) is enabled, the wake-up interrupt is generated.
     * |        |          |Note2: This bit is read only, but can be cleared by writing '1' to it.
     */
    __IO uint32_t ISR;

    /**
     * UA_TOR
     * ===================================================================================================
     * Offset: 0x20  UART Time-out Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |TOIC      |Time-Out Interrupt Comparator
     * |        |          |The time-out counter resets and starts counting (the counting clock = baud rate) whenever the RX FIFO receives a new data word.
     * |        |          |Once the content of time-out counter is equal to that of time-out interrupt comparator (TOIC (UA_TOR[7:0])), a receiver time-out interrupt (INT_TOUT) is generated if TOUT_IEN (UA_IER [4]) enabled.
     * |        |          |A new incoming data word or RX FIFO empty will clear TOUT_INT(UA_IER[9]).
     * |        |          |In order to avoid receiver time-out interrupt generation immediately during one character is being received, TOIC (UA_TOR[7:0]) value should be set between 40 and 255.
     * |        |          |So, for example, if TOIC (UA_TOR[7:0]) is set with 40, the time-out interrupt is generated after four characters are not received when 1 stop bit and no parity check is set for UART transfer.
     * |[15:8]  |DLY       |TX Delay Time Value
     * |        |          |This field is used to programming the transfer delay time between the last stop bit and next start bit.
     */
    __IO uint32_t TOR;

    /**
     * UA_BAUD
     * ===================================================================================================
     * Offset: 0x24  UART Baud Rate Divisor Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[15:0]  |BRD       |Baud Rate Divider
     * |        |          |The field indicates the baud rate divider
     * |[27:24] |DIVIDER_X |Divider X
     * |        |          |The baud rate divider M = X+1.
     * |[28]    |DIV_X_ONE |Divider X Equal To 1
     * |        |          |0 = Divider M = X (the equation of M = X+1, but DIVIDER_X[27:24] must >= 8).
     * |        |          |1 = Divider M = 1 (the equation of M = 1, but BRD [15:0] must >= 3).  
     * |[29]    |DIV_X_EN  |Divider X Enable
     * |        |          |The BRD = Baud Rate Divider, and the baud rate equation is
     * |        |          |Baud Rate = Clock / [M * (BRD + 2)]; The default value of M is 16.
     * |        |          |0 = Divider X Disabled (the equation of M = 16).
     * |        |          |1 = Divider X Enabled (the equation of M = X+1, but DIVIDER_X [27:24] must >= 8).
     * |        |          |Note: In IrDA mode, this bit must disable.
     */
    __IO uint32_t BAUD;

    /**
     * UA_IRCR
     * ===================================================================================================
     * Offset: 0x28  UART IrDA Control Register 
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1]     |TX_SELECT |TX_SELECT
     * |        |          |0 = IrDA Transmitter Disabled and Receiver Enabled.
     * |        |          |1 = IrDA Transmitter Enabled and Receiver Disabled.
     * |[5]     |INV_TX    |IrDA inverse Transmitting Output Signal Control
     * |        |          |0 = None inverse transmitting signal.
     * |        |          |1 = Inverse transmitting output signal.
     * |[6]     |INV_RX    |IrDA inverse Receive Input Signal Control
     * |        |          |0 = None inverse receiving input signal.
     * |        |          |1 = Inverse receiving input signal.
     */
    __IO uint32_t IRCR;

    /**
     * UA_ALT_CSR
     * ===================================================================================================
     * Offset: 0x2C  UART Alternate Control/Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[3:0]   |LIN_BKFL  |UART LIN Break Field Length (Available In UART0/UART1/UART2)
     * |        |          |This field indicates a 4-bit LIN TX break field count.
     * |        |          |Note1: This break field length is UA_LIN_BKFL + 1
     * |        |          |Note2: According to LIN spec, the reset value is 0xC (break field length = 13).
     * |[6]     |LIN_RX_EN |LIN RX Enable (Available In UART0/UART1/UART2)
     * |        |          |0 = LIN RX mode Disabled.
     * |        |          |1 = LIN RX mode Enabled.
     * |[7]     |LIN_TX_EN |LIN TX Break Mode Enable (Available In UART0/UART1/UART2)
     * |        |          |0 = LIN TX Break mode Disabled.
     * |        |          |1 = LIN TX Break mode Enabled.
     * |        |          |Note: When TX break field transfer operation finished, this bit will be cleared automatically.
     * |[8]     |RS485_NMM |RS-485 Normal Multi-Drop Operation Mode (NMM) (Available In UART0/UART1)
     * |        |          |0 = RS-485 Normal Multi-drop Operation mode (NMM) Disabled.
     * |        |          |1 = RS-485 Normal Multi-drop Operation mode (NMM) Enabled.
     * |        |          |Note: It cannot be active with RS-485_AAD operation mode.
     * |[9]     |RS485_AAD |RS-485 Auto Address Detection Operation Mode (AAD) (Available In UART0/UART1)
     * |        |          |0 = RS-485 Auto Address Detection Operation mode (AAD) Disabled.
     * |        |          |1 = RS-485 Auto Address Detection Operation mode (AAD) Enabled.
     * |        |          |Note: It cannot be active with RS-485_NMM operation mode.
     * |[10]    |RS485_AUD |RS-485 Auto Direction Mode (AUD) (Available In UART0/UART1)
     * |        |          |0 = RS-485 Auto Direction Operation mode (AUO) Disabled.
     * |        |          |1 = RS-485 Auto Direction Operation mode (AUO) Enabled.
     * |        |          |Note: It can be active with RS-485_AAD or RS-485_NMM operation mode.
     * |[15]    |RS485_ADD_EN|RS-485 Address Detection Enable (Available In UART0/UART1)
     * |        |          |This bit is used to enable RS-485 Address Detection mode.
     * |        |          |0 = Address detection mode Disabled.
     * |        |          |1 = Address detection mode Enabled.
     * |        |          |Note: This bit is used for RS-485 any operation mode.
     * |[17]    |ABRIF     |Auto-Baud Rate Interrupt Flag (Read Only)
     * |        |          |This bit is set when auto-baud rate detection function finished or the auto-baud rate counter was overflow and if ABRIEN(UA_IEN [18]) is set then the auto-baud rate interrupt will be generated.
     * |        |          |Note: This bit is read only, but it can be cleared by writing "1" to ABRDTOIF (UA_FSR[2]) and ABRDIF(UA_FSR[1])
     * |[18]    |ABRDEN    |Auto-Baud Rate Detect Enable Bit
     * |        |          |0 = Auto-baud rate detect function Disabled.
     * |        |          |1 = Auto-baud rate detect function Enabled.
     * |        |          |This bit is cleared automatically after auto-baud detection is finished.
     * |[20:19] |ABRDBITS  |Auto-Baud Rate Detect Bit Length
     * |        |          |00 = 1-bit time from Start bit to the 1st rising edge. The input pattern shall be 0x01.
     * |        |          |01 = 2-bit time from Start bit to the 1st rising edge. The input pattern shall be 0x02.
     * |        |          |10 = 4-bit time from Start bit to the 1st rising edge. The input pattern shall be 0x08.
     * |        |          |11 = 8-bit time from Start bit to the 1st rising edge. The input pattern shall be 0x80.
     * |        |          |Note : The calculation of bit number includes the START bit.
     * |[31:24] |ADDR_MATCH|Address Match Value Register (Available In UART0/UART1)
     * |        |          |This field contains the RS-485 address match values.
     * |        |          |Note: This field is used for RS-485 auto address detection mode.
     */
    __IO uint32_t ALT_CSR;

    /**
     * UA_FUN_SEL
     * ===================================================================================================
     * Offset: 0x30  UART Function Select Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |FUN_SEL   |Function Select Enable
     * |        |          |00 = UART function Enabled.
     * |        |          |01 = LIN function Enabled. (Available In UART0/UART1/UART2)
     * |        |          |10 = IrDA function Enabled.
     * |        |          |11 = RS-485 function Enabled. (Available In UART0/UART1)
     */
    __IO uint32_t FUN_SEL;

    /**
     * UA_LIN_CTL
     * ===================================================================================================
     * Offset: 0x34  UART LIN Control Register (Available In UART0/UART1/UART2)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |LINS_EN   |LIN Slave Mode Enable Control
     * |        |          |0 = LIN slave mode Disabled.
     * |        |          |1 = LIN slave mode Enabled.
     * |[1]     |LINS_HDET_EN|LIN Slave Header Detection Enable Control
     * |        |          |0 = LIN slave header detection Disabled.
     * |        |          |1 = LIN slave header detection Enabled.
     * |        |          |Note1: This bit only valid when in LIN slave mode (LINS_EN (UA_LIN_CTL[0]) = 1).
     * |        |          |Note2: In LIN function mode, when detect header field (break + sync + frame ID), LINS_HDET_F (UA_LIN_SR [0]) flag will be asserted.
     * |        |          |If the LIN_IEN (UA_IER[8]) = 1, an interrupt will be generated.
     * |[2]     |LINS_ARS_EN|LIN Slave Automatic Resynchronization Mode Enable Control
     * |        |          |0 = LIN automatic resynchronization Disabled.
     * |        |          |1 = LIN automatic resynchronization Enabled.
     * |        |          |Note1: This bit only valid when in LIN slave mode (LINS_EN (UA_LIN_CTL[0]) = 1).
     * |        |          |Note2: When operation in Automatic Resynchronization mode, the baud rate setting must be mode2 (BAUD_M1 (UA_BAUD [29]) and BAUD_M0 (UA_BAUD [28]) must be 1).
     * |        |          |(Slave mode with automatic resynchronization).
     * |[3]     |LINS_DUM_EN|LIN Slave Divider Update Method Enable Control
     * |        |          |0 = UA_BAUD updated is written by software (if no automatic resynchronization update occurs at the same time).
     * |        |          |1 = UA_BAUD is updated at the next received character. User must set the bit before checksum reception.
     * |        |          |Note1: This bit only valid when in LIN slave mode (LINS_EN (UA_LIN_CTL[0]) = 1).
     * |        |          |Note2: This bit used for LIN Slave Automatic Resynchronization mode.
     * |        |          |(for Non-Automatic Resynchronization mode, this bit should be kept cleared).
     * |        |          |(Slave mode with automatic resynchronization).
     * |[4]     |LIN_MUTE_EN|LIN Mute Mode Enable Control
     * |        |          |0 = LIN mute mode Disabled.
     * |        |          |1 = LIN mute mode Enabled.
     * |        |          |Note: The exit from mute mode condition and each control and interactions of this field are explained in (LIN slave mode).
     * |[8]     |LIN_SHD   |LIN TX Send Header Enable Control
     * |        |          |The LIN TX header can be "break field" or "break and sync field" or "break, sync and frame ID field", it is depend on setting LIN_HEAD_SEL (UA_LIN_CTL[23:22]).
     * |        |          |0 = Send LIN TX header Disabled.
     * |        |          |1 = Send LIN TX header Enabled.
     * |        |          |Note1: These registers are shadow registers of LIN_SHD (UA_ALT_CSR [7]); user can read/write it by setting LIN_SHD (UA_ALT_CSR [7]) or LIN_SHD (UA_LIN_CTL [8]).
     * |        |          |Note2: When transmitter header field (it may be "break" or "break + sync" or "break + sync + frame ID" selected by LIN_HEAD_SEL (UA_LIN_CTL[23:22]) field) transfer operation finished, this bit will be cleared automatically.
     * |[9]     |LIN_IDPEN |LIN ID Parity Enable Control
     * |        |          |0 = LIN frame ID parity Disabled.
     * |        |          |1 = LIN frame ID parity Enabled.
     * |        |          |Note1: This bit can be used for LIN master to sending header field (LIN_SHD (UA_LIN_CTL[8])) = 1 and LIN_HEAD_SEL (UA_LIN_CTL[23:22]) = 10) or be used for enable LIN slave received frame ID parity checked.
     * |        |          |Note2: This bit is only use when the operation header transmitter is in LIN_HEAD_SEL (UA_LIN_CTL[23:22]) = 10
     * |[10]    |LIN_BKDET_EN|LIN Break Detection Enable Control
     * |        |          |When detect consecutive dominant greater than 11 bits, and are followed by a delimiter character, the LIN_BKDET_F (UA_LIN_SR[8]) flag is set in UA_LIN_SR register at the end of break field.
     * |        |          |If the LIN_IEN (UA_IER [8])=1, an interrupt will be generated.
     * |        |          |0 = LIN break detection Disabled.
     * |        |          |1 = LIN break detection Enabled.
     * |[11]    |LIN_RX_DIS|LIN Receiver Disable Control
     * |        |          |If the receiver is enabled (LIN_RX_DIS (UA_LIN_CTL[11] ) = 0), all received byte data will be accepted and stored in the RX-FIFO, and if the receiver is disabled (LIN_RX_DIS (UA_LIN_CTL[11] = 1), all received byte data will be ignore.
     * |        |          |0 = LIN receiver Enabled.
     * |        |          |1 = LIN receiver Disabled.
     * |        |          |Note: This bit is only valid when operating in LIN function mode (FUN_SEL (UA_FUN_SEL[1:0]) = 01)
     * |[12]    |BIT_ERR_EN|Bit Error Detect Enable Control
     * |        |          |0 = Bit error detection function Disabled.
     * |        |          |1 = Bit error detection Enabled.
     * |        |          |Note: In LIN function mode, when occur bit error, the BIT_ERR_F (UA_LIN_SR[9]) flag will be asserted.
     * |        |          |If the LIN_IEN (UA_IER[8]) = 1, an interrupt will be generated.
     * |[19:16] |LIN_BKFL  |LIN Break Field Length
     * |        |          |This field indicates a 4-bit LIN TX break field count.
     * |        |          |Note1: These registers are shadow registers of LIN_BKFL, User can read/write it by setting LIN_BKFL (UA_ALT_CSR[3:0]) or LIN_BKFL (UA_LIN_CTL[19:16].
     * |        |          |Note2: This break field length is LIN_BKFL + 1.
     * |        |          |Note3:
     * |        |          |According to LIN spec, the reset value is 12 (break field length = 13).
     * |[21:20] |LIN_BS_LEN|LIN Break/Sync Delimiter Length
     * |        |          |00 = The LIN break/sync delimiter length is 1 bit time.
     * |        |          |10 = The LIN break/sync delimiter length is 2 bit time.
     * |        |          |10 = The LIN break/sync delimiter length is 3 bit time.
     * |        |          |11 = The LIN break/sync delimiter length is 4 bit time.
     * |        |          |Note: This bit used for LIN master to sending header field.
     * |[23:22] |LIN_HEAD_SEL|LIN Header Select
     * |        |          |00 = The LIN header includes "break field".
     * |        |          |01 = The LIN header includes "break field" and "sync field".
     * |        |          |10 = The LIN header includes "break field", "sync field" and "frame ID field".
     * |        |          |11 = Reserved.
     * |        |          |Note: This bit is used to master mode for LIN to send header field (LIN_SHD (UA_LIN_CTL [8]) = 1) or used to slave to indicates exit from mute mode condition (LIN_MUTE_EN (UA_LIN_CTL[4] = 1).
     * |[31:24] |LIN_PID   |LIN PID Register
     * |        |          |This field contains the LIN frame ID value when in LIN function mode, the frame ID parity can be generated by software or hardware depends on LIN_IDPEN (UA_LIN_CTL[9]) = 1.
     * |        |          |If the parity generated by hardware, user fill ID0~ID5, (LIN_PID [29:24] )hardware will calculate P0 (LIN_PID[30]) and P1 (LIN_PID[31]), otherwise user must filled frame ID and parity in this field.
     * |        |          |Note1: User can fill any 8-bit value to this field and the bit 24 indicates ID0 (LSB first).
     * |        |          |Note2: This field can be used for LIN master mode or slave mode.
     */
    __IO uint32_t LIN_CTL;

    /**
     * UA_LIN_SR
     * ===================================================================================================
     * Offset: 0x38  UART LIN Status Register (Available In UART0/UART1/UART2)
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |LINS_HDET_F|LIN Slave Header Detection Flag (Read Only)
     * |        |          |This bit is set by hardware when a LIN header is detected in LIN slave mode and be cleared by writing 1 to it.
     * |        |          |0 = LIN header not detected.
     * |        |          |1 = LIN header detected (break + sync + frame ID).
     * |        |          |Note1: This bit is read only, but it can be cleared by writing 1 to it.
     * |        |          |Note2: This bit is only valid when in LIN slave mode (LINS_EN (UA_LIN_CTL [0]) = 1) and enable LIN slave header detection function (LINS_HDET_EN (UA_LIN_CTL [1])).
     * |        |          |Note3: When enable ID parity check LIN_IDPEN (UA_LIN_CTL [9]), if hardware detect complete header ("break + sync + frame ID"), the LINS_HEDT_F will be set whether the frame ID correct or not.
     * |[1]     |LINS_HERR_F|LIN Slave Header Error Flag (Read Only)
     * |        |          |This bit is set by hardware when a LIN header error is detected in LIN slave mode and be cleared by writing 1 to it.
     * |        |          |The header errors include "break delimiter is too short (less than 0.5 bit time)", "frame error in sync field or Identifier field", "sync field data is not 0x55 in Non-Automatic Resynchronization mode", "sync field deviation error with Automatic Resynchronization mode", "sync field measure time-out with Automatic Resynchronization mode" and "LIN header reception time-out".
     * |        |          |0 = LIN header error not detected.
     * |        |          |1 = LIN header error detected.
     * |        |          |Note1: This bit is read only, but it can be cleared by writing 1 to it.
     * |        |          |Note2: This bit is only valid when UART is operated in LIN slave mode (LINS_EN (UA_LIN_CTL [0]) = 1) and enables LIN slave header detection function (LINS_HDET_EN (UA_LIN_CTL [1])).
     * |[2]     |LINS_IDPERR_F|LIN Slave ID Parity Error Flag (Read Only)
     * |        |          |This bit is set by hardware when receipted frame ID parity is not correct.
     * |        |          |0 = No active.
     * |        |          |1 = Receipted frame ID parity is not correct.
     * |        |          |Note1: This bit is read only, but it can be cleared by writing "1" to it.
     * |        |          |Note2: This bit is only valid when in LIN slave mode (LINS_EN (UA_LIN_CTL [0])= 1) and enable LIN frame ID parity check function LIN_IDPEN (UA_LIN_CTL [9]).
     * |[3]     |LINS_SYNC_F|LIN Slave Sync Field
     * |        |          |This bit indicates that the LIN sync field is being analyzed in Automatic Resynchronization mode.
     * |        |          |When the receiver header have some error been detect, user must reset the internal circuit to re-search new frame header by writing 1 to this bit.
     * |        |          |0 = The current character is not at LIN sync state.
     * |        |          |1 = The current character is at LIN sync state.
     * |        |          |Note1: This bit is only valid when in LIN Slave mode (LINS_EN(UA_LIN_CTL[0]) = 1).
     * |        |          |Note2: This bit is read only, but it can be cleared by writing 1 to it.
     * |        |          |Note3: When writing 1 to it, hardware will reload the initial baud rate and re-search a new frame header.
     * |[8]     |LIN_BKDET_F|LIN Break Detection Flag (Read Only)
     * |        |          |This bit is set by hardware when a break is detected and be cleared by writing 1 to it through software.
     * |        |          |0 = LIN break not detected.
     * |        |          |1 = LIN break detected.
     * |        |          |Note1: This bit is read only, but it can be cleared by writing 1 to it.
     * |        |          |Note2: This bit is only valid when LIN break detection function is enabled (LIN_BKDET_EN (UA_LIN_CTL[10]) =1).
     * |[9]     |BIT_ERR_F |Bit Error Detect Status Flag (Read Only)
     * |        |          |At TX transfer state, hardware will monitoring the bus state, if the input pin (SIN) state not equals to the output pin (SOUT) state, BIT_ERR_F (UA_LIN_SR[9]) will be set.
     * |        |          |When occur bit error, if the LIN_IEN (UA_IER[8]) = 1, an interrupt will be generated.
     * |        |          |Note1: This bit is read only, but it can be cleared by writing 1 to it.
     * |        |          |Note2: This bit is only valid when enable bit error detection function (BIT_ERR_EN (UA_LIN_CTL [12]) = 1).
     */
    __IO uint32_t LIN_SR;
} UART_T;


/** @addtogroup UART_CONST UART Bit Field Definition
    Constant Definitions for UART Controller
  @{
 */


/* UART THR Bit Field Definitions */
#define UART_THR_THR_Pos            0                                       /*!< UART_T::THR: THR Position  */
#define UART_THR_THR_Msk            (0xFul << UART_THR_THR_Pos)             /*!< UART_T::THR: THR Mask      */

/* UART RBR Bit Field Definitions */
#define UART_RBR_RBR_Pos            0                                       /*!< UART_T::RBR: RBR Position */
#define UART_RBR_RBR_Msk            (0xFul << UART_RBR_RBR_Pos)             /*!< UART_T::RBR: RBR Mask      */

/* UART IER Bit Field Definitions */
#define UART_IER_AERIEN_Pos         18                                      /*!< UART_T::IER: AERIEN Position           */
#define UART_IER_AERIEN_Msk         (1ul << UART_IER_AERIEN_Pos)            /*!< UART_T::IER: AERIEN Mask                */

#define UART_IER_AUTO_CTS_EN_Pos    13                                      /*!< UART_T::IER: AUTO_CTS_EN Position      */
#define UART_IER_AUTO_CTS_EN_Msk    (1ul << UART_IER_AUTO_CTS_EN_Pos)       /*!< UART_T::IER: AUTO_CTS_EN Mask           */

#define UART_IER_AUTO_RTS_EN_Pos    12                                      /*!< UART_T::IER: AUTO_RTS_EN Position      */
#define UART_IER_AUTO_RTS_EN_Msk    (1ul << UART_IER_AUTO_RTS_EN_Pos)       /*!< UART_T::IER: AUTO_RTS_EN Mask           */

#define UART_IER_TIME_OUT_EN_Pos    11                                      /*!< UART_T::IER: TIME_OUT_EN Position      */
#define UART_IER_TIME_OUT_EN_Msk    (1ul << UART_IER_TIME_OUT_EN_Pos)       /*!< UART_T::IER: TIME_OUT_EN Mask           */

#define UART_IER_WKDATIEN_Pos       10                                      /*!< UART_T::IER: WKDATIEN Position         */
#define UART_IER_WKDATIEN_Msk       (1ul << UART_IER_WKDATIEN_Pos)          /*!< UART_T::IER: WKDATIEN Mask              */

#define UART_IER_LIN_IEN_Pos        8                                       /*!< UART_T::IER: LIN_IEN Position          */
#define UART_IER_LIN_IEN_Msk        (1ul << UART_IER_LIN_IEN_Pos)           /*!< UART_T::IER: LIN_IEN Mask               */

#define UART_IER_WKCTSIEN_Pos       6                                       /*!< UART_T::IER: WKCTSIEN Position         */
#define UART_IER_WKCTSIEN_Msk       (1ul << UART_IER_WKCTSIEN_Pos)          /*!< UART_T::IER: WKCTSIEN Mask              */

#define UART_IER_BUF_ERR_IEN_Pos    5                                       /*!< UART_T::IER: BUF_ERR_IEN Position      */
#define UART_IER_BUF_ERR_IEN_Msk    (1ul << UART_IER_BUF_ERR_IEN_Pos)       /*!< UART_T::IER: BUF_ERR_IEN Mask           */

#define UART_IER_TOUT_IEN_Pos        4                                      /*!< UART_T::IER: TOUT_IEN Position          */
#define UART_IER_TOUT_IEN_Msk        (1ul << UART_IER_TOUT_IEN_Pos)         /*!< UART_T::IER: TOUT_IEN Mask               */

#define UART_IER_MODEM_IEN_Pos      3                                       /*!< UART_T::IER: MODEM_IEN Position        */
#define UART_IER_MODEM_IEN_Msk      (1ul << UART_IER_MODEM_IEN_Pos)         /*!< UART_T::IER: MODEM_IEN Mask             */

#define UART_IER_RLS_IEN_Pos        2                                       /*!< UART_T::IER: RLS_IEN Position          */
#define UART_IER_RLS_IEN_Msk        (1ul << UART_IER_RLS_IEN_Pos)           /*!< UART_T::IER: RLS_IEN Mask               */

#define UART_IER_THRE_IEN_Pos       1                                       /*!< UART_T::IER: THRE_IEN Position         */
#define UART_IER_THRE_IEN_Msk       (1ul << UART_IER_THRE_IEN_Pos)          /*!< UART_T::IER: THRE_IEN Mask              */

#define UART_IER_RDA_IEN_Pos        0                                       /*!< UART_T::IER: RDA_IEN Position           */
#define UART_IER_RDA_IEN_Msk        (1ul << UART_IER_RDA_IEN_Pos)           /*!< UART_T::IER: RDA_IEN Mask               */

/* UART FCR Bit Field Definitions */
#define UART_FCR_RTS_TRI_LEV_Pos    16                                      /*!< UART_T::FCR: RTS_TRI_LEV Position       */
#define UART_FCR_RTS_TRI_LEV_Msk    (0xFul << UART_FCR_RTS_TRI_LEV_Pos)     /*!< UART_T::FCR: RTS_TRI_LEV Mask           */

#define UART_FCR_RX_DIS_Pos         8                                       /*!< UART_T::FCR: RX_DIS Position            */
#define UART_FCR_RX_DIS_Msk         (1ul << UART_FCR_RX_DIS_Pos)            /*!< UART_T::FCR: RX_DIS Mask                */

#define UART_FCR_RFITL_Pos          4                                       /*!< UART_T::FCR: RFITL Position             */
#define UART_FCR_RFITL_Msk          (0xFul << UART_FCR_RFITL_Pos)           /*!< UART_T::FCR: RFITL Mask                 */

#define UART_FCR_TFR_Pos            2                                       /*!< UART_T::FCR: TFR Position               */
#define UART_FCR_TFR_Msk            (1ul << UART_FCR_TFR_Pos)               /*!< UART_T::FCR: TFR Mask                   */

#define UART_FCR_RFR_Pos            1                                       /*!< UART_T::FCR: RFR Position               */
#define UART_FCR_RFR_Msk            (1ul << UART_FCR_RFR_Pos)               /*!< UART_T::FCR: RFR Mask                   */

/* UART LCR Bit Field Definitions */
#define UART_LCR_BCB_Pos            6                                       /*!< UART_T::LCR: BCB Position               */
#define UART_LCR_BCB_Msk            (1ul << UART_LCR_BCB_Pos)               /*!< UART_T::LCR: BCB Mask                   */

#define UART_LCR_SPE_Pos            5                                       /*!< UART_T::LCR: SPE Position               */
#define UART_LCR_SPE_Msk            (1ul << UART_LCR_SPE_Pos)               /*!< UART_T::LCR: SPE Mask                   */

#define UART_LCR_EPE_Pos            4                                       /*!< UART_T::LCR: EPE Position               */
#define UART_LCR_EPE_Msk            (1ul << UART_LCR_EPE_Pos)               /*!< UART_T::LCR: EPE Mask                   */

#define UART_LCR_PBE_Pos            3                                       /*!< UART_T::LCR: PBE Position               */
#define UART_LCR_PBE_Msk            (1ul << UART_LCR_PBE_Pos)               /*!< UART_T::LCR: PBE Mask                   */

#define UART_LCR_NSB_Pos            2                                       /*!< UART_T::LCR: NSB Position               */
#define UART_LCR_NSB_Msk            (1ul << UART_LCR_NSB_Pos)               /*!< UART_T::LCR: NSB Mask                   */

#define UART_LCR_WLS_Pos            0                                       /*!< UART_T::LCR: WLS Position               */
#define UART_LCR_WLS_Msk            (0x3ul << UART_LCR_WLS_Pos)             /*!< UART_T::LCR: WLS Mask                   */

/* UART MCR Bit Field Definitions */
#define UART_MCR_RTS_ST_Pos         13                                      /*!< UART_T::MCR: RTS_ST Position            */
#define UART_MCR_RTS_ST_Msk         (1ul << UART_MCR_RTS_ST_Pos)            /*!< UART_T::MCR: RTS_ST Mask                */

#define UART_MCR_LEV_RTS_Pos        9                                       /*!< UART_T::MCR: LEV_RTS Position           */
#define UART_MCR_LEV_RTS_Msk        (1ul << UART_MCR_LEV_RTS_Pos)           /*!< UART_T::MCR: LEV_RTS Mask               */

#define UART_MCR_RTS_Pos            1                                       /*!< UART_T::MCR: RTS Position               */
#define UART_MCR_RTS_Msk            (1ul << UART_MCR_RTS_Pos)               /*!< UART_T::MCR: RTS Mask                   */

/* UART MSR Bit Field Definitions */
#define UART_MSR_LEV_CTS_Pos        8                                       /*!< UART_T::MSR: LEV_CTS Position           */
#define UART_MSR_LEV_CTS_Msk        (1ul << UART_MSR_LEV_CTS_Pos)           /*!< UART_T::MSR: LEV_CTS Mask               */

#define UART_MSR_CTS_ST_Pos         4                                       /*!< UART_T::MSR: CTS_ST Position            */
#define UART_MSR_CTS_ST_Msk         (1ul << UART_MSR_CTS_ST_Pos)            /*!< UART_T::MSR: CTS_ST Mask                */

#define UART_MSR_DCTSF_Pos          0                                       /*!< UART_T::MSR: DCTST Position             */
#define UART_MSR_DCTSF_Msk          (1ul << UART_MSR_DCTSF_Pos)             /*!< UART_T::MSR: DCTST Mask                 */

/* UART FSR Bit Field Definitions */
#define UART_FSR_TE_FLAG_Pos        28                                      /*!< UART_T::FSR: TE_FLAG Position           */
#define UART_FSR_TE_FLAG_Msk        (1ul << UART_FSR_TE_FLAG_Pos)           /*!< UART_T::FSR: TE_FLAG Mask               */

#define UART_FSR_TX_OVER_IF_Pos     24                                      /*!< UART_T::FSR: TX_OVER_IF Position        */
#define UART_FSR_TX_OVER_IF_Msk     (1ul << UART_FSR_TX_OVER_IF_Pos)        /*!< UART_T::FSR: TX_OVER_IF Mask            */

#define UART_FSR_TX_FULL_Pos        23                                      /*!< UART_T::FSR: TX_FULL Position           */
#define UART_FSR_TX_FULL_Msk        (1ul << UART_FSR_TX_FULL_Pos)           /*!< UART_T::FSR: TX_FULL Mask               */

#define UART_FSR_TX_EMPTY_Pos       22                                      /*!< UART_T::FSR: TX_EMPTY Position          */
#define UART_FSR_TX_EMPTY_Msk       (1ul << UART_FSR_TX_EMPTY_Pos)          /*!< UART_T::FSR: TX_EMPTY Mask              */

#define UART_FSR_TX_POINTER_Pos     16                                      /*!< UART_T::FSR: TX_POINTER Position        */
#define UART_FSR_TX_POINTER_Msk     (0x3Ful << UART_FSR_TX_POINTER_Pos)     /*!< UART_T::FSR: TX_POINTER Mask            */

#define UART_FSR_RX_FULL_Pos        15                                      /*!< UART_T::FSR: RX_FULL Position           */
#define UART_FSR_RX_FULL_Msk        (1ul << UART_FSR_RX_FULL_Pos)           /*!< UART_T::FSR: RX_FULL Mask               */

#define UART_FSR_RX_EMPTY_Pos       14                                      /*!< UART_T::FSR: RX_EMPTY Position          */
#define UART_FSR_RX_EMPTY_Msk       (1ul << UART_FSR_RX_EMPTY_Pos)          /*!< UART_T::FSR: RX_EMPTY Mask              */

#define UART_FSR_RX_POINTER_Pos     8                                       /*!< UART_T::FSR: RX_POINTERS Position       */
#define UART_FSR_RX_POINTER_Msk     (0x3Ful << UART_FSR_RX_POINTER_Pos)     /*!< UART_T::FSR: RX_POINTER Mask            */

#define UART_FSR_BIF_Pos            6                                       /*!< UART_T::FSR: BIF Position               */
#define UART_FSR_BIF_Msk            (1ul << UART_FSR_BIF_Pos)               /*!< UART_T::FSR: BIF Mask                   */

#define UART_FSR_FEF_Pos            5                                       /*!< UART_T::FSR: FEF Position               */
#define UART_FSR_FEF_Msk            (1ul << UART_FSR_FEF_Pos)               /*!< UART_T::FSR: FEF Mask                   */

#define UART_FSR_PEF_Pos            4                                       /*!< UART_T::FSR: PEF Position               */
#define UART_FSR_PEF_Msk            (1ul << UART_FSR_PEF_Pos)               /*!< UART_T::FSR: PEF Mask                   */

#define UART_FSR_RS485_ADD_DETF_Pos 3                                       /*!< UART_T::FSR: RS485_ADD_DETF Position    */
#define UART_FSR_RS485_ADD_DETF_Msk (1ul << UART_FSR_RS485_ADD_DETF_Pos)    /*!< UART_T::FSR: RS485_ADD_DETF Mask        */

#define UART_FSR_ABRDTOIF_Pos       2                                       /*!< UART_T::FSR: ABRDTOIF Position          */
#define UART_FSR_ABRDTOIF_Msk       (1ul << UART_FSR_ABRDTOIF_Pos)          /*!< UART_T::FSR: ABRDTOIF Mask              */

#define UART_FSR_ABRDIF_Pos         1                                       /*!< UART_T::FSR: ABRDIF Position            */
#define UART_FSR_ABRDIF_Msk         (1ul << UART_FSR_ABRDIF_Pos)            /*!< UART_T::FSR: ABRDIF Mask                */

#define UART_FSR_RX_OVER_IF_Pos     0                                       /*!< UART_T::FSR: RX_OVER_IF Position        */
#define UART_FSR_RX_OVER_IF_Msk     (1ul << UART_FSR_RX_OVER_IF_Pos)        /*!< UART_T::FSR: RX_OVER_IF Mask            */

/* UART ISR Bit Field Definitions */
#define UART_ISR_DATWKIF_Pos        17                                      /*!< UART_T::ISR: DATWKIF Position           */
#define UART_ISR_DATWKIF_Msk        (1ul << UART_ISR_DATWKIF_Pos)           /*!< UART_T::ISR: DATWKIF Mask               */

#define UART_ISR_CTSWKIF_Pos        16                                      /*!< UART_T::ISR: CTSWKIF Position           */
#define UART_ISR_CTSWKIF_Msk        (1ul << UART_ISR_CTSWKIF_Pos)           /*!< UART_T::ISR: CTSWKIF Mask               */

#define UART_ISR_LIN_INT_Pos        15                                      /*!< UART_T::ISR: LIN_INT Position           */
#define UART_ISR_LIN_INT_Msk        (1ul << UART_ISR_LIN_INT_Pos)           /*!< UART_T::ISR: LIN_INT Mask               */

#define UART_ISR_BUF_ERR_INT_Pos    13                                      /*!< UART_T::ISR: BUF_ERR_INT Position       */
#define UART_ISR_BUF_ERR_INT_Msk    (1ul << UART_ISR_BUF_ERR_INT_Pos)       /*!< UART_T::ISR: BUF_ERR_INT Mask           */

#define UART_ISR_TOUT_INT_Pos       12                                      /*!< UART_T::ISR: TOUT_INT Position          */
#define UART_ISR_TOUT_INT_Msk       (1ul << UART_ISR_TOUT_INT_Pos)          /*!< UART_T::ISR: TOUT_INT Mask              */

#define UART_ISR_MODEM_INT_Pos      11                                      /*!< UART_T::ISR: MODEM_INT Position         */
#define UART_ISR_MODEM_INT_Msk      (1ul << UART_ISR_MODEM_INT_Pos)         /*!< UART_T::ISR: MODEM_INT Mask             */

#define UART_ISR_RLS_INT_Pos        10                                      /*!< UART_T::ISR: RLS_INT Position           */
#define UART_ISR_RLS_INT_Msk        (1ul << UART_ISR_RLS_INT_Pos)           /*!< UART_T::ISR: RLS_INT Mask               */

#define UART_ISR_THRE_INT_Pos       9                                       /*!< UART_T::ISR: THRE_INT Position          */
#define UART_ISR_THRE_INT_Msk       (1ul << UART_ISR_THRE_INT_Pos)          /*!< UART_T::ISR: THRE_INT Mask              */

#define UART_ISR_RDA_INT_Pos        8                                       /*!< UART_T::ISR: RDA_INT Position           */
#define UART_ISR_RDA_INT_Msk        (1ul << UART_ISR_RDA_INT_Pos)           /*!< UART_T::ISR: RDA_INT Mask               */

#define UART_ISR_LIN_IF_Pos         7                                       /*!< UART_T::ISR: LIN RX_IF Position         */
#define UART_ISR_LIN_IF_Msk         (1ul << UART_ISR_LIN_IF_Pos)            /*!< UART_T::ISR: LIN RX_IF Mask             */

#define UART_ISR_WKIF_Pos           6                                       /*!< UART_T::ISR: WKIF Position        */
#define UART_ISR_WKIF_Msk           (1ul << UART_ISR_WKIF_Pos)              /*!< UART_T::ISR: WKIF Mask            */

#define UART_ISR_BUF_ERR_IF_Pos     5                                       /*!< UART_T::ISR: BUF_ERR_IF Position        */
#define UART_ISR_BUF_ERR_IF_Msk     (1ul << UART_ISR_BUF_ERR_IF_Pos)        /*!< UART_T::ISR: BUF_ERR_IF Mask            */

#define UART_ISR_TOUT_IF_Pos        4                                       /*!< UART_T::ISR: TOUT_IF Position           */
#define UART_ISR_TOUT_IF_Msk        (1ul << UART_ISR_TOUT_IF_Pos)           /*!< UART_T::ISR: TOUT_IF Mask               */

#define UART_ISR_MODEM_IF_Pos       3                                       /*!< UART_T::ISR: MODEM_IF Position          */
#define UART_ISR_MODEM_IF_Msk       (1ul << UART_ISR_MODEM_IF_Pos)          /*!< UART_T::ISR: MODEM_IF Mask              */

#define UART_ISR_RLS_IF_Pos         2                                       /*!< UART_T::ISR: RLS_IF Position            */
#define UART_ISR_RLS_IF_Msk         (1ul << UART_ISR_RLS_IF_Pos)            /*!< UART_T::ISR: RLS_IF Mask                */

#define UART_ISR_THRE_IF_Pos        1                                       /*!< UART_T::ISR: THRE_IF Position           */
#define UART_ISR_THRE_IF_Msk        (1ul << UART_ISR_THRE_IF_Pos)           /*!< UART_T::ISR: THRE_IF Mask               */

#define UART_ISR_RDA_IF_Pos         0                                       /*!< UART_T::ISR: RDA_IF Position            */
#define UART_ISR_RDA_IF_Msk         (1ul << UART_ISR_RDA_IF_Pos)            /*!< UART_T::ISR: RDA_IF Mask                */


/* UART TOR Bit Field Definitions */
#define UART_TOR_DLY_Pos           8                                        /*!< UART_T::TOR: DLY Position               */
#define UART_TOR_DLY_Msk           (0xFFul << UART_TOR_DLY_Pos)             /*!< UART_T::TOR: DLY Mask                   */

#define UART_TOR_TOIC_Pos          0                                        /*!< UART_T::TOR: TOIC Position              */
#define UART_TOR_TOIC_Msk          (0xFFul << UART_TOR_TOIC_Pos)

/* UART BAUD Bit Field Definitions */
#define UART_BAUD_DIV_X_EN_Pos    29                                        /*!< UART BARD: DIV_X_EN Position         */
#define UART_BAUD_DIV_X_EN_Msk    (1ul << UART_BAUD_DIV_X_EN_Pos)           /*!< UART BARD: DIV_X_EN Mask             */

#define UART_BAUD_DIV_X_ONE_Pos   28                                        /*!< UART BARD: DIV_X_ONE Position        */
#define UART_BAUD_DIV_X_ONE_Msk   (1ul << UART_BAUD_DIV_X_ONE_Pos)          /*!< UART BARD: DIV_X_ONE Mask            */

#define UART_BAUD_DIVIDER_X_Pos   24                                        /*!< UART BARD: DIVIDER_X Position        */
#define UART_BAUD_DIVIDER_X_Msk   (0xFul << UART_BAUD_DIVIDER_X_Pos)        /*!< UART BARD: DIVIDER_X Mask            */

#define UART_BAUD_BRD_Pos         0                                         /*!< UART BARD: BRD Position              */
#define UART_BAUD_BRD_Msk         (0xFFFFul << UART_BAUD_BRD_Pos)           /*!< UART BARD: BRD Mask                  */

/* UART IRCR Bit Field Definitions */
#define UART_IRCR_INV_RX_Pos      6                                         /*!< UART_T::IRCR: INV_RX Position           */
#define UART_IRCR_INV_RX_Msk     (1ul << UART_IRCR_INV_RX_Pos)              /*!< UART_T::IRCR: INV_RX Mask               */

#define UART_IRCR_INV_TX_Pos      5                                         /*!< UART_T::IRCR: INV_TX Position           */
#define UART_IRCR_INV_TX_Msk     (1ul << UART_IRCR_INV_TX_Pos)              /*!< UART_T::IRCR: INV_TX Mask               */

#define UART_IRCR_TX_SELECT_Pos   1                                         /*!< UART_T::IRCR: TX_SELECT Position        */
#define UART_IRCR_TX_SELECT_Msk   (1ul << UART_IRCR_TX_SELECT_Pos)          /*!< UART_T::IRCR: TX_SELECT Mask            */

/* UART ALT_CSR Bit Field Definitions */
#define UART_ALT_CSR_ADDR_MATCH_Pos      24                                      /*!< UART_T::ALT_CSR: ADDR_MATCH Position    */
#define UART_ALT_CSR_ADDR_MATCH_Msk     (0xFFul << UART_ALT_CSR_ADDR_MATCH_Pos)  /*!< UART_T::ALT_CSR: ADDR_MATCH Mask        */

#define UART_ALT_CSR_ABRDBITS_Pos       19                                       /*!< UART_T::ALT_CSR: ABRDBITS Position      */
#define UART_ALT_CSR_ABRDBITS_Msk       (0x3ul << UART_ALT_CSR_ABRDBITS_Pos)     /*!< UART_T::ALT_CSR: ABRDBITS Mask          */

#define UART_ALT_CSR_ABRDEN_Pos         18                                       /*!< UART_T::ALT_CSR: ABRDEN Position        */
#define UART_ALT_CSR_ABRDEN_Msk         (1ul << UART_ALT_CSR_ABRDEN_Pos)         /*!< UART_T::ALT_CSR: ABRDEN Mask            */

#define UART_ALT_CSR_ABRIF_Pos          17                                       /*!< UART_T::ALT_CSR: ABRIF Position         */
#define UART_ALT_CSR_ABRIF_Msk          (1ul << UART_ALT_CSR_ABRIF_Pos)          /*!< UART_T::ALT_CSR: ABRIF Mask             */

#define UART_ALT_CSR_RS485_ADD_EN_Pos   15                                       /*!< UART_T::ALT_CSR: RS485_ADD_EN Position  */
#define UART_ALT_CSR_RS485_ADD_EN_Msk   (1ul << UART_ALT_CSR_RS485_ADD_EN_Pos)   /*!< UART_T::ALT_CSR: RS485_ADD_EN Mask      */

#define UART_ALT_CSR_RS485_AUD_Pos      10                                       /*!< UART_T::ALT_CSR: RS485_AUD Position     */
#define UART_ALT_CSR_RS485_AUD_Msk      (1ul << UART_ALT_CSR_RS485_AUD_Pos)      /*!< UART_T::ALT_CSR: RS485_AUD Mask         */

#define UART_ALT_CSR_RS485_AAD_Pos      9                                        /*!< UART_T::ALT_CSR: RS485_AAD Position     */
#define UART_ALT_CSR_RS485_AAD_Msk      (1ul << UART_ALT_CSR_RS485_AAD_Pos)      /*!< UART_T::ALT_CSR: RS485_AAD Mask         */

#define UART_ALT_CSR_RS485_NMM_Pos      8                                        /*!< UART_T::ALT_CSR: RS485_NMM Position     */
#define UART_ALT_CSR_RS485_NMM_Msk      (1ul << UART_ALT_CSR_RS485_NMM_Pos)      /*!< UART_T::ALT_CSR: RS485_NMM Mask         */

#define UART_ALT_CSR_LIN_TX_EN_Pos      7                                        /*!< UART_T::ALT_CSR: LIN TX Break Mode Enable Position     */
#define UART_ALT_CSR_LIN_TX_EN_Msk      (1ul << UART_ALT_CSR_LIN_TX_EN_Pos)      /*!< UART_T::ALT_CSR: LIN TX Break Mode Enable Mask         */

#define UART_ALT_CSR_LIN_RX_EN_Pos      6                                        /*!< UART_T::ALT_CSR: LIN RX Enable Position     */
#define UART_ALT_CSR_LIN_RX_EN_Msk      (1ul << UART_ALT_CSR_LIN_RX_EN_Pos)      /*!< UART_T::ALT_CSR: LIN RX Enable Mask         */

#define UART_ALT_CSR_UA_LIN_BKFL_Pos    0                                        /*!< UART_T::ALT_CSR: UART LIN Break Field Length Position     */
#define UART_ALT_CSR_UA_LIN_BKFL_Msk    (0xFul << UART_ALT_CSR_UA_LIN_BKFL_Pos)  /*!< UART_T::ALT_CSR: UART LIN Break Field Length Mask         */

/* UART FUN_SEL Bit Field Definitions */
#define UART_FUN_SEL_FUN_SEL_Pos        0                                        /*!< UART_T::FUN_SEL: FUN_SEL Position       */
#define UART_FUN_SEL_FUN_SEL_Msk       (0x3ul << UART_FUN_SEL_FUN_SEL_Pos)       /*!< UART_T::FUN_SEL: FUN_SEL Mask           */

/* UART LIN_CTL Bit Field Definitions */
#define UART_LIN_CTL_LIN_PID_Pos        24                                        /*!< UART_T::LIN_CTL: LIN_PID Position       */
#define UART_LIN_CTL_LIN_PID_Msk        (0xFFul << UART_LIN_CTL_LIN_PID_Pos)      /*!< UART_T::LIN_CTL: LIN_PID Mask           */

#define UART_LIN_CTL_LIN_HEAD_SEL_Pos   22                                        /*!< UART_T::LIN_CTL: LIN_HEAD_SEL Position       */
#define UART_LIN_CTL_LIN_HEAD_SEL_Msk   (0x3ul << UART_LIN_CTL_LIN_HEAD_SEL_Pos)  /*!< UART_T::LIN_CTL: LIN_HEAD_SEL Mask           */

#define UART_LIN_CTL_LIN_BS_LEN_Pos     20                                        /*!< UART_T::LIN_CTL: LIN_BS_LEN Position       */
#define UART_LIN_CTL_LIN_BS_LEN_Msk     (0x3ul << UART_LIN_CTL_LIN_BS_LEN_Pos)    /*!< UART_T::LIN_CTL: LIN_BS_LEN Mask           */

#define UART_LIN_CTL_LIN_BKFL_Pos       16                                        /*!< UART_T::LIN_CTL: LIN_BKFL Position       */
#define UART_LIN_CTL_LIN_BKFL_Msk       (0xFul << UART_LIN_CTL_LIN_BKFL_Pos)      /*!< UART_T::LIN_CTL: LIN_BKFL Mask           */

#define UART_LIN_CTL_BIT_ERR_EN_Pos     12                                        /*!< UART_T::LIN_CTL: BIT_ERR_EN Position       */
#define UART_LIN_CTL_BIT_ERR_EN_Msk     (1ul << UART_LIN_CTL_BIT_ERR_EN_Pos)      /*!< UART_T::LIN_CTL: BIT_ERR_EN Mask           */

#define UART_LIN_CTL_LIN_RX_DIS_Pos     11                                        /*!< UART_T::LIN_CTL: LIN_RX_DIS Position       */
#define UART_LIN_CTL_LIN_RX_DIS_Msk     (1ul << UART_LIN_CTL_LIN_RX_DIS_Pos)      /*!< UART_T::LIN_CTL: LIN_RX_DIS Mask           */

#define UART_LIN_CTL_LIN_BKDET_EN_Pos   10                                        /*!< UART_T::LIN_CTL: LIN_BKDET_EN Position       */
#define UART_LIN_CTL_LIN_BKDET_EN_Msk   (1ul << UART_LIN_CTL_LIN_BKDET_EN_Pos)    /*!< UART_T::LIN_CTL: LIN_BKDET_EN Mask           */

#define UART_LIN_CTL_LIN_IDPEN_Pos      9                                         /*!< UART_T::LIN_CTL: LIN_IDPEN Position       */
#define UART_LIN_CTL_LIN_IDPEN_Msk      (1ul << UART_LIN_CTL_LIN_IDPEN_Pos)       /*!< UART_T::LIN_CTL: LIN_IDPEN Mask           */

#define UART_LIN_CTL_LIN_SHD_Pos        8                                         /*!< UART_T::LIN_CTL: LIN_SHD Position       */
#define UART_LIN_CTL_LIN_SHD_Msk        (1ul << UART_LIN_CTL_LIN_SHD_Pos)         /*!< UART_T::LIN_CTL: LIN_SHD Mask           */

#define UART_LIN_CTL_LIN_MUTE_EN_Pos    4                                          /*!< UART_T::LIN_CTL: LIN_MUTE_EN Position       */
#define UART_LIN_CTL_LIN_MUTE_EN_Msk    (1ul << UART_LIN_CTL_LIN_MUTE_EN_Pos)      /*!< UART_T::LIN_CTL: LIN_MUTE_EN Mask           */

#define UART_LIN_CTL_LINS_DUM_EN_Pos    3                                          /*!< UART_T::LIN_CTL: LINS_DUM_EN Position       */
#define UART_LIN_CTL_LINS_DUM_EN_Msk    (1ul << UART_LIN_CTL_LINS_DUM_EN_Pos)      /*!< UART_T::LIN_CTL: LINS_DUM_EN Mask           */

#define UART_LIN_CTL_LINS_ARS_EN_Pos    2                                          /*!< UART_T::LIN_CTL: LINS_ARS_EN Position       */
#define UART_LIN_CTL_LINS_ARS_EN_Msk    (1ul << UART_LIN_CTL_LINS_ARS_EN_Pos)      /*!< UART_T::LIN_CTL: LINS_ARS_EN Mask           */

#define UART_LIN_CTL_LINS_HDET_EN_Pos   1                                          /*!< UART_T::LIN_CTL: LINS_HDET_EN Position       */
#define UART_LIN_CTL_LINS_HDET_EN_Msk   (1ul << UART_LIN_CTL_LINS_HDET_EN_Pos)     /*!< UART_T::LIN_CTL: LINS_HDET_EN Mask           */

#define UART_LIN_CTL_LINS_EN_Pos        0                                          /*!< UART_T::LIN_CTL: LINS_EN Position       */
#define UART_LIN_CTL_LINS_EN_Msk        (1ul << UART_LIN_CTL_LINS_EN_Pos)          /*!< UART_T::LIN_CTL: LINS_EN Mask           */

/* UART LIN_SR Bit Field Definitions */
#define UART_LIN_SR_BIT_ERR_F_Pos       9                                           /*!< UART_T::LIN_SR: BIT_ERR_F Position         */
#define UART_LIN_SR_BIT_ERR_F_Msk       (1ul << UART_LIN_SR_BIT_ERR_F_Pos)          /*!< UART_T::LIN_SR: BIT_ERR_F Mask             */

#define UART_LIN_SR_LINS_BKDET_F_Pos    8                                           /*!< UART_T::LIN_SR: LINS_BKDET_F Position      */
#define UART_LIN_SR_LINS_BKDET_F_Msk    (1ul << UART_LIN_SR_LINS_BKDET_F_Pos)       /*!< UART_T::LIN_SR: LINS_BKDET_F Mask          */

#define UART_LIN_SR_LINS_SYNC_F_Pos     3                                           /*!< UART_T::LIN_SR: LINS_SYNC_F Position       */
#define UART_LIN_SR_LINS_SYNC_F_Msk     (1ul << UART_LIN_SR_LINS_SYNC_F_Pos)        /*!< UART_T::LIN_SR: LINS_SYNC_F Mask           */

#define UART_LIN_SR_LINS_IDPERR_F_Pos   2                                           /*!< UART_T::LIN_SR: LINS_IDPERR_F Position     */
#define UART_LIN_SR_LINS_IDPERR_F_Msk   (1ul << UART_LIN_SR_LINS_IDPERR_F_Pos)      /*!< UART_T::LIN_SR: LINS_IDPERR_F Mask         */

#define UART_LIN_SR_LINS_HERR_F_Pos     1                                           /*!< UART_T::LIN_SR: LINS_HERR_F Position       */
#define UART_LIN_SR_LINS_HERR_F_Msk     (1ul << UART_LIN_SR_LINS_HERR_F_Pos)        /*!< UART_T::LIN_SR: LINS_HERR_F Mask           */

#define UART_LIN_SR_LINS_HDET_F_Pos     0                                           /*!< UART_T::LIN_SR: LINS_HDET_F Position       */
#define UART_LIN_SR_LINS_HDET_F_Msk     (1ul << UART_LIN_SR_LINS_HDET_F_Pos)        /*!< UART_T::LIN_SR: LINS_HDET_F Mask           */

/*@}*/ /* end of group UART_CONST */
/*@}*/ /* end of group UART */



/*----------------------------- Watchdog Timer (WDT) -----------------------------*/
/**
    @addtogroup WDT Watch Dog Timer Controller (WDT)
    Memory Mapped Structure for WDT Controller
@{ */
typedef struct
{
    /**
     * WTCR
     * ===================================================================================================
     * Offset: 0x00  Watchdog Timer Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WTR       |Reset Watchdog Timer Up Counter (Write Protect)
     * |        |          |0 = No effect.
     * |        |          |1 = Reset the internal 18-bit WDT up counter value.
     * |        |          |Note: This bit will be automatically cleared by hardware.
     * |[1]     |WTRE      |Watchdog Timer Reset Enable (Write Protect)
     * |        |          |Setting this bit will enable the WDT time-out reset function if the WDT up counter value has not been cleared after the specific WDT reset delay period expires.
     * |        |          |0 = WDT time-out reset function Disabled.
     * |        |          |1 = WDT time-out reset function Enabled.
     * |[2]     |WTRF      |Watchdog Timer Time-out Reset Flag
     * |        |          |This bit indicates the system has been reset by WDT time-out reset or not.
     * |        |          |0 = WDT time-out reset did not occur.
     * |        |          |1 = WDT time-out reset occurred.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * |[3]     |WTIF      |Watchdog Timer Time-out Interrupt Flag
     * |        |          |This bit will set to 1 while WDT up counter value reaches the selected WDT time-out interval.
     * |        |          |0 = WDT time-out interrupt did not occur.
     * |        |          |1 = WDT time-out interrupt occurred.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * |[4]     |WTWKE     |Watchdog Timer Time-out Wake-Up Function Control
     * |        |          |(Write Protect)
     * |        |          |If this bit is set to 1, while WTIF is generated to 1 and WTIE enabled, the WDT time-out interrupt signal will generate a wake-up trigger event to chip.
     * |        |          |0 = Wake-up trigger event Disabled if WDT time-out interrupt signal generated.
     * |        |          |1 = Wake-up trigger event Enabled if WDT time-out interrupt signal generated.
     * |        |          |Note: Chip can be woken-up by WDT time-out interrupt signal generated only if WDT clock source is selected to 10 kHz oscillator.
     * |[5]     |WTWKF     |Watchdog Timer Time-out Wake-Up Flag
     * |        |          |This bit indicates the interrupt wake-up flag status of WDT.
     * |        |          |0 = WDT does not cause chip wake-up.
     * |        |          |1 = Chip wake-up from Idle or Power-down mode if WDT time-out interrupt signal generated.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * |[6]     |WTIE      |Watchdog Timer Time-out Interrupt Enable Control (Write Protect)
     * |        |          |If this bit is enabled, the WDT time-out interrupt signal is generated and inform to CPU.
     * |        |          |0 = WDT time-out interrupt Disabled.
     * |        |          |1 = WDT time-out interrupt Enabled.
     * |[7]     |WTE       |Watchdog Timer Enable Control (Write Protect)
     * |        |          |0 = WDT Disabled. (This action will reset the internal up counter value.)
     * |        |          |1 = WDT Enabled.
     * |        |          |Note: If CWDTEN (CONFIG0[31] Watchdog Enable) bit is set to 0, this bit is forced as 1 and
     * |        |          | user cannot change this bit to 0.
     * |[10:8]  |WTIS      |Watchdog Timer Time-out Interval Selection (Write Protect)
     * |        |          |These three bits select the time-out interval period for the WDT.
     * |        |          |000 = 2^4 *WDT_CLK.
     * |        |          |001 = 2^6 * WDT_CLK.
     * |        |          |010 = 2^8 * WDT_CLK.
     * |        |          |011 = 2^10 * WDT_CLK.
     * |        |          |100 = 2^12 * WDT_CLK.
     * |        |          |101 = 2^14 * WDT_CLK.
     * |        |          |110 = 2^16 * WDT_CLK.
     * |        |          |111 = 2^18 * WDT_CLK.
     * |[31]    |DBGACK_WDT|ICE Debug Mode Acknowledge Disable Control (Write Protect)
     * |        |          |0 = ICE debug mode acknowledgment effects WDT counting.
     * |        |          |WDT up counter will be held while CPU is held by ICE.
     * |        |          |1 = ICE debug mode acknowledgment Disabled.
     * |        |          |WDT up counter will keep going no matter CPU is held by ICE or not.
     */
    __IO uint32_t  WTCR;

    /**
     * WTCRALT
     * ===================================================================================================
     * Offset: 0x04  Watchdog Timer Alternative Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[1:0]   |WTRDSEL   |Watchdog Timer Reset Delay Selection (Write Protect)
     * |        |          |When WDT time-out happened, user has a time named WDT Reset Delay Period to clear WDT counter to prevent WDT time-out reset happened.
     * |        |          |User can select a suitable value of WDT Reset Delay Period for different WDT time-out period.
     * |        |          |These bits are protected bit.
     * |        |          |It means programming this bit needs to write "59h", "16h", "88h" to address 0x5000_0100 to disable register protection.
     * |        |          |Reference the register REGWRPROT at address GCR_BASE+0x100.
     * |        |          |00 = Watchdog Timer Reset Delay Period is 1026 * WDT_CLK.
     * |        |          |01 = Watchdog Timer Reset Delay Period is 130 * WDT_CLK.
     * |        |          |10 = Watchdog Timer Reset Delay Period is 18 * WDT_CLK.
     * |        |          |11 = Watchdog Timer Reset Delay Period is 3 * WDT_CLK.
     * |        |          |Note: This register will be reset to 0 if WDT time-out reset happened.
     */
    __IO uint32_t  WTCRALT;
} WDT_T;

/**
    @addtogroup WDT_CONST WDT Bit Field Definition
    Constant Definitions for WDT Controller
@{ */


/* WDT WTCR Bit Field Definitions */
#define WDT_WTCR_DBGACK_WDT_Pos 31                                              /*!< WDT_T::WTCR: DBGACK_WDT Position */
#define WDT_WTCR_DBGACK_WDT_Msk (1ul << WDT_WTCR_DBGACK_WDT_Pos)                /*!< WDT_T::WTCR: DBGACK_WDT Mask */

#define WDT_WTCR_WTIS_Pos       8                                               /*!< WDT_T::WTCR: WTIS Position */
#define WDT_WTCR_WTIS_Msk       (0x7ul << WDT_WTCR_WTIS_Pos)                    /*!< WDT_T::WTCR: WTIS Mask */

#define WDT_WTCR_WTE_Pos        7                                               /*!< WDT_T::WTCR: WTE Position */
#define WDT_WTCR_WTE_Msk        (1ul << WDT_WTCR_WTE_Pos)                       /*!< WDT_T::WTCR: WTE Mask */

#define WDT_WTCR_WTIE_Pos       6                                               /*!< WDT_T::WTCR: WTIE Position */
#define WDT_WTCR_WTIE_Msk       (1ul << WDT_WTCR_WTIE_Pos)                      /*!< WDT_T::WTCR: WTIE Mask */

#define WDT_WTCR_WTWKF_Pos      5                                               /*!< WDT_T::WTCR: WTWKF Position */
#define WDT_WTCR_WTWKF_Msk      (1ul << WDT_WTCR_WTWKF_Pos)                     /*!< WDT_T::WTCR: WTWKF Mask */

#define WDT_WTCR_WTWKE_Pos      4                                               /*!< WDT_T::WTCR: WTWKE Position */
#define WDT_WTCR_WTWKE_Msk      (1ul << WDT_WTCR_WTWKE_Pos)                     /*!< WDT_T::WTCR: WTWKE Mask */

#define WDT_WTCR_WTIF_Pos       3                                               /*!< WDT_T::WTCR: WTIF Position */
#define WDT_WTCR_WTIF_Msk       (1ul << WDT_WTCR_WTIF_Pos)                      /*!< WDT_T::WTCR: WTIF Mask */

#define WDT_WTCR_WTRF_Pos       2                                               /*!< WDT_T::WTCR: WTRF Position */
#define WDT_WTCR_WTRF_Msk       (1ul << WDT_WTCR_WTRF_Pos)                      /*!< WDT_T::WTCR: WTRF Mask */

#define WDT_WTCR_WTRE_Pos       1                                               /*!< WDT_T::WTCR: WTRE Position */
#define WDT_WTCR_WTRE_Msk       (1ul << WDT_WTCR_WTRE_Pos)                      /*!< WDT_T::WTCR: WTRE Mask */

#define WDT_WTCR_WTR_Pos        0                                               /*!< WDT_T::WTCR: WTR Position */
#define WDT_WTCR_WTR_Msk        (1ul << WDT_WTCR_WTR_Pos)                       /*!< WDT_T::WTCR: WTR Mask */

/* WDT WTCRALT Bit Field Definitions */
#define WDT_WTCRALT_WTRDSEL_Pos 0                                               /*!< WDT_T::WTCRALT: WTRDSEL Position */
#define WDT_WTCRALT_WTRDSEL_Msk (0x3ul << WDT_WTCRALT_WTRDSEL_Pos)              /*!< WDT_T::WTCRALT: WTRDSEL Mask */
/*@}*/ /* end of group WDT_CONST */
/*@}*/ /* end of group WDT */


/*----------------------------- Window Watchdog Timer (WWDT) -----------------------------*/
/** @addtogroup WWDT Window Watchdog Timer (WWDT)
    Memory Mapped Structure for WWDT Controller
  @{
 */
typedef struct
{
    /**
     * WWDTRLD
     * ===================================================================================================
     * Offset: 0x00  Window Watchdog Timer Reload Counter Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |WWDTRLD   |WWDT Reload Counter Register
     * |        |          |Writing 0x00005AA5 to this register will reload the WWDT counter value to 0x3F.
     * |        |          |Note: User can only write WWDTRLD to reload WWDT counter value when current WWDT
     * |        |          | counter value between 0 and WINCMP. If user writes WWDTRLD when current WWDT
     * |        |          | counter value is larger than WINCMP, WWDT reset signal will generate immediately.
     */
    __IO uint32_t  WWDTRLD;

    /**
     * WWDTCR
     * ===================================================================================================
     * Offset: 0x04  Window Watchdog Timer Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WWDTEN    |WWDT Enable Control
     * |        |          |0 = WWDT counter is stopped.
     * |        |          |1 = WWDT counter is starting counting.
     * |[1]     |WWDTIE    |WWDT Interrupt Enable Control
     * |        |          |If this bit is enabled, the WWDT counter compare match interrupt signal is generated and inform to CPU.
     * |        |          |0 = WWDT counter compare match interrupt Disabled.
     * |        |          |1 = WWDT counter compare match interrupt Enabled.
     * |[11:8]  |PERIODSEL |WWDT Counter Prescale Period Selection
     * |        |          |0000 = Pre-scale is 1; Max time-out period is 1 * 64 * TWWDT.
     * |        |          |0001 = Pre-scale is 2; Max time-out period is 2 * 64 * TWWDT.
     * |        |          |0010 = Pre-scale is 4; Max time-out period is 4 * 64 * TWWDT.
     * |        |          |0011 = Pre-scale is 8; Max time-out period is 8 * 64 * TWWDT.
     * |        |          |0100 = Pre-scale is 16; Max time-out period is 16 * 64 * TWWDT.
     * |        |          |0101 = Pre-scale is 32; Max time-out period is 32 * 64 * TWWDT.
     * |        |          |0110 = Pre-scale is 64; Max time-out period is 64 * 64 * TWWDT.
     * |        |          |0111 = Pre-scale is 128; Max time-out period is 128 * 64 * TWWDT.
     * |        |          |1000 = Pre-scale is 192; Max time-out period is 192 * 64 * TWWDT.
     * |        |          |1001 = Pre-scale is 256; Max time-out period is 256 * 64 * TWWDT.
     * |        |          |1010 = Pre-scale is 384; Max time-out period is 384 * 64 * TWWDT.
     * |        |          |1011 = Pre-scale is 512; Max time-out period is 512 * 64 * TWWDT.
     * |        |          |1100 = Pre-scale is 768; Max time-out period is 768 * 64 * TWWDT.
     * |        |          |1101 = Pre-scale is 1024; Max time-out period is 1024 * 64 * TWWDT.
     * |        |          |1110 = Pre-scale is 1536; Max time-out period is 1536 * 64 * TWWDT.
     * |        |          |1111 = Pre-scale is 2048; Max time-out period is 2048 * 64 * TWWDT.
     * |[21:16] |WINCMP    |WWDT Window Compare Register
     * |        |          |Set this register to adjust the valid reload window.
     * |        |          |Note: User can only write WWDTRLD to reload WWDT counter value when current WWDT counter value between 0 and WINCMP.
     * |        |          |If user writes WWDTRLD when current WWDT counter value larger than WINCMP, WWDT reset signal will generate immediately.
     * |[31]    |DBGACK_WWDT|ICE Debug Mode Acknowledge Disable Control
     * |        |          |0 = ICE debug mode acknowledgment effects WWDT counting.
     * |        |          |WWDT down counter will be held while CPU is held by ICE.
     * |        |          |1 = ICE debug mode acknowledgment Disabled.
     * |        |          |WWDT down counter will keep going no matter CPU is held by ICE or not.
     */
    __IO uint32_t  WWDTCR;

    /**
     * WWDTSR
     * ===================================================================================================
     * Offset: 0x08  Window Watchdog Timer Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |WWDTIF    |WWDT Compare Match Interrupt Flag
     * |        |          |This bit indicates the interrupt flag status of WWDT while WWDT counter value matches WINCMP value.
     * |        |          |0 = No effect.
     * |        |          |1 = WWDT counter value matches WINCMP value.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * |[1]     |WWDTRF    |WWDT Time-out Reset Flag
     * |        |          |This bit indicates the system has been reset by WWDT time-out reset or not.
     * |        |          |0 = WWDT time-out reset did not occur.
     * |        |          |1 = WWDT time-out reset occurred.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     */
    __IO uint32_t  WWDTSR;

    /**
     * WWDTCVR
     * ===================================================================================================
     * Offset: 0x0C  Window Watchdog Timer Counter Value Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[5:0]   |WWDTCVAL  |WWDT Counter Value
     * |        |          |WWDTCVAL will be updated continuously to monitor 6-bit down counter value.
     */
    __I  uint32_t  WWDTCVR;
} WWDT_T;

/** @addtogroup WWDT_CONST Bit Field Definition
  @{
 */


/* WWDT WWDTRLD Bit Field Definitions */
#define WWDT_WWDTRLD_WWDTRLD_Pos    0                                           /*!< WWDT_T::WWDTRLD: WWDTRLD Position */
#define WWDT_WWDTRLD_WWDTRLD_Msk    (0xFFFFFFFFul << WWDT_WWDTRLD_WWDTRLD_Pos)  /*!< WWDT_T::WWDTRLD: WWDTRLD Mask */

/* WWDT WWDTCR Bit Field Definitions */
#define WWDT_WWDTCR_DBGACK_WWDT_Pos 31                                          /*!< WWDT_T::WWDTCR: DBGACK_WWDT Position */
#define WWDT_WWDTCR_DBGACK_WWDT_Msk (1ul << WWDT_WWDTCR_DBGACK_WWDT_Pos)        /*!< WWDT_T::WWDTCR: DBGACK_WWDT Mask */

#define WWDT_WWDTCR_WINCMP_Pos      16                                          /*!< WWDT_T::WWDTCR: WINCMP Position */
#define WWDT_WWDTCR_WINCMP_Msk      (0x3Ful << WWDT_WWDTCR_WINCMP_Pos)          /*!< WWDT_T::WWDTCR: WINCMP Mask */

#define WWDT_WWDTCR_PERIODSEL_Pos   8                                           /*!< WWDT_T::WWDTCR: PERIODSEL Position */
#define WWDT_WWDTCR_PERIODSEL_Msk   (0xFul << WWDT_WWDTCR_PERIODSEL_Pos)        /*!< WWDT_T::WWDTCR: PERIODSEL Mask */

#define WWDT_WWDTCR_WWDTIE_Pos      1                                           /*!< WWDT_T::WWDTCR: WWDTIE Position */
#define WWDT_WWDTCR_WWDTIE_Msk      (1ul << WWDT_WWDTCR_WWDTIE_Pos)             /*!< WWDT_T::WWDTCR: WWDTIE Mask */

#define WWDT_WWDTCR_WWDTEN_Pos      0                                           /*!< WWDT_T::WWDTCR: WWDTEN Position */
#define WWDT_WWDTCR_WWDTEN_Msk      (1ul << WWDT_WWDTCR_WWDTEN_Pos)             /*!< WWDT_T::WWDTCR: WWDTEN Mask */

/* WWDT WWDTSR Bit Field Definitions */
#define WWDT_WWDTSR_WWDTRF_Pos      1                                           /*!< WWDT_T::WWDTSR: WWDTRF Position */
#define WWDT_WWDTSR_WWDTRF_Msk      (1ul << WWDT_WWDTSR_WWDTRF_Pos)             /*!< WWDT_T::WWDTSR: WWDTRF Mask */

#define WWDT_WWDTSR_WWDTIF_Pos      0                                           /*!< WWDT_T::WWDTSR: WWDTIF Position */
#define WWDT_WWDTSR_WWDTIF_Msk      (1ul << WWDT_WWDTSR_WWDTIF_Pos)             /*!< WWDT_T::WWDTSR: WWDTIF Mask */

/* WWDT WWDTCVR Bit Field Definitions */
#define WWDT_WWDTCVR_WWDTCVAL_Pos   0                                           /*!< WWDT_T::WWDTCVR: WWDTCVAL Position */
#define WWDT_WWDTCVR_WWDTCVAL_Msk   (0x3Ful << WWDT_WWDTCVR_WWDTCVAL_Pos)       /*!< WWDT_T::WWDTCVR: WWDTCVAL Mask */
/*@}*/ /* end of group WWDT_CONST */
/*@}*/ /* end of group WWDT */





/*@}*/ /* end of group REGISTER */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/** @addtogroup PERIPHERAL_MEM_MAP Peripheral Memory Map
  Memory Mapped Structure for Peripheral
  @{
 */
/* Peripheral and SRAM base address */
#define FLASH_BASE          ((     uint32_t)0x00000000)
#define SRAM_BASE           ((     uint32_t)0x20000000)
#define AHB_BASE            ((     uint32_t)0x50000000)
#define APB1_BASE           ((     uint32_t)0x40000000)
#define APB2_BASE           ((     uint32_t)0x40100000)

/* Peripheral memory map */
#define GPIO_BASE           (AHB_BASE        + 0x4000)                  /*!< GPIO Base Address                                   */
#define PA_BASE             (GPIO_BASE               )                  /*!< GPIO PORTA Base Address                             */
#define PB_BASE             (GPIO_BASE       + 0x0040)                  /*!< GPIO PORTB Base Address                             */
#define PC_BASE             (GPIO_BASE       + 0x0080)                  /*!< GPIO PORTC Base Address                             */
#define PD_BASE             (GPIO_BASE       + 0x00C0)                  /*!< GPIO PORTD Base Address                             */
#define PE_BASE             (GPIO_BASE       + 0x0100)                  /*!< GPIO PORTE Base Address                             */
#define PF_BASE             (GPIO_BASE       + 0x0140)                  /*!< GPIO PORTF Base Address                             */
#define GPIO_DBNCECON_BASE  (GPIO_BASE       + 0x0180)                  /*!< GPIO De-bounce Cycle Control Base Address           */
#define GPIO_PIN_DATA_BASE  (GPIO_BASE       + 0x0200)                  /*!< GPIO Pin Data Input/Output Control Base Address     */


#define UART0_BASE           (APB1_BASE      + 0x50000)                 /*!< UART0 Base Address                              */
#define UART1_BASE           (APB2_BASE      + 0x50000)                 /*!< UART1 Base Address                              */
#define UART2_BASE           (APB2_BASE      + 0x54000)                 /*!< UART2 Base Address                              */
#define UART3_BASE           (APB1_BASE      + 0x54000)                 /*!< UART3 Base Address                              */
#define UART4_BASE           (APB1_BASE      + 0x58000)                 /*!< UART4 Base Address                              */
#define UART5_BASE           (APB2_BASE      + 0x58000)                 /*!< UART5 Base Address                              */


#define TIMER0_BASE          (APB1_BASE      + 0x10000)                 /*!< Timer0 Base Address                              */
#define TIMER1_BASE          (APB1_BASE      + 0x10020)                 /*!< Timer1 Base Address                              */
#define TIMER2_BASE          (APB2_BASE      + 0x10000)                 /*!< Timer2 Base Address                              */
#define TIMER3_BASE          (APB2_BASE      + 0x10020)                 /*!< Timer3 Base Address                              */

#define WDT_BASE             (APB1_BASE      + 0x4000)                  /*!< Watchdog Timer Base Address                      */

#define WWDT_BASE            (APB1_BASE      + 0x4100)                  /*!< Window Watchdog Timer Base Address               */

#define SPI0_BASE            (APB1_BASE      + 0x30000)                 /*!< SPI0 Base Address                                */

#define I2C0_BASE            (APB1_BASE      + 0x20000)                 /*!< I2C0 Base Address                                */
#define I2C1_BASE            (APB2_BASE      + 0x20000)                 /*!< I2C1 Base Address                                */

#define ADC_BASE             (APB1_BASE      + 0xE0000)                 /*!< ADC Base Address                                 */

#define CLK_BASE             (AHB_BASE       + 0x00200)                 /*!< System Clock Controller Base Address             */

#define GCR_BASE             (AHB_BASE       + 0x00000)                 /*!< System Global Controller Base Address            */

#define INT_BASE             (AHB_BASE       + 0x00300)                 /*!< Interrupt Source Controller Base Address         */

#define FMC_BASE             (AHB_BASE       + 0x0C000)

#define CAN0_BASE            (APB2_BASE      + 0x80000)                 /*!< CAN0 Base Address                                */
#define CAN1_BASE            (APB2_BASE      + 0x84000)                 /*!< CAN1 Base Address                                */

#define PWM0_BASE            (APB1_BASE      + 0x40000)                 /*!< PWM0 Base Address                                */
#define PWM1_BASE            (APB2_BASE      + 0x40000)                 /*!< PWM1 Base Address                                */

#define BPWM0_BASE           (APB1_BASE      + 0x44000)                 /*!< BPWM0 Base Address                                */
#define BPWM1_BASE           (APB2_BASE      + 0x44000)                 /*!< BPWM1 Base Address                                */

/*@}*/ /* end of group PERIPHERAL_MEM_MAP */

/******************************************************************************/
/*                         Peripheral Definitions                             */
/******************************************************************************/

/** @addtogroup PERIPHERAL Peripheral Definitions
  The Definitions of Peripheral
  @{
 */
#define PA                  ((GPIO_T *) PA_BASE)                        /*!< GPIO PORTA Configuration Struct                        */
#define PB                  ((GPIO_T *) PB_BASE)                        /*!< GPIO PORTB Configuration Struct                        */
#define PC                  ((GPIO_T *) PC_BASE)                        /*!< GPIO PORTC Configuration Struct                        */
#define PD                  ((GPIO_T *) PD_BASE)                        /*!< GPIO PORTD Configuration Struct                        */
#define PE                  ((GPIO_T *) PE_BASE)                        /*!< GPIO PORTE Configuration Struct                        */
#define PF                  ((GPIO_T *) PF_BASE)                        /*!< GPIO PORTF Configuration Struct                        */
#define GPIO                ((GPIO_DBNCECON_T *) GPIO_DBNCECON_BASE)    /*!< Interrupt De-bounce Cycle Control Configuration Struct */


#define UART0               ((UART_T *) UART0_BASE)                     /*!< UART0 Configuration Struct                       */
#define UART1               ((UART_T *) UART1_BASE)                     /*!< UART1 Configuration Struct                       */
#define UART2               ((UART_T *) UART2_BASE)                     /*!< UART2 Configuration Struct                       */
#define UART3               ((UART_T *) UART3_BASE)                     /*!< UART3 Configuration Struct                       */
#define UART4               ((UART_T *) UART4_BASE)                     /*!< UART4 Configuration Struct                       */
#define UART5               ((UART_T *) UART5_BASE)                     /*!< UART5 Configuration Struct                       */

#define TIMER0              ((TIMER_T *) TIMER0_BASE)                   /*!< Timer0 Configuration Struct                      */
#define TIMER1              ((TIMER_T *) TIMER1_BASE)                   /*!< Timer1 Configuration Struct                      */
#define TIMER2              ((TIMER_T *) TIMER2_BASE)                   /*!< Timer2 Configuration Struct                      */
#define TIMER3              ((TIMER_T *) TIMER3_BASE)                   /*!< Timer3 Configuration Struct                      */

#define WDT                 ((WDT_T *) WDT_BASE)                        /*!< Watchdog Timer Configuration Struct              */

#define WWDT                ((WWDT_T *) WWDT_BASE)                      /*!< Window Watchdog Timer Configuration Struct       */

#define SPI0                ((SPI_T *) SPI0_BASE)                       /*!< SPI0 Configuration Struct                        */

#define I2C0                ((I2C_T *) I2C0_BASE)                       /*!< I2C0 Configuration Struct                        */
#define I2C1                ((I2C_T *) I2C1_BASE)                       /*!< I2C1 Configuration Struct                        */

#define ADC                 ((ADC_T *) ADC_BASE)                        /*!< ADC Configuration Struct                         */

#define CLK                 ((CLK_T *) CLK_BASE)                        /*!< System Clock Controller Configuration Struct     */

#define SYS                 ((GCR_T *) GCR_BASE)                        /*!< System Global Controller Configuration Struct    */

#define SYSINT              ((GCR_INT_T *) INT_BASE)                    /*!< Interrupt Source Controller Configuration Struct */

#define FMC                 ((FMC_T *) FMC_BASE)

#define CAN                 ((CAN_T *) CAN0_BASE)
#define CAN0                ((CAN_T *) CAN0_BASE)                       /*!< CAN0 Configuration Struct                        */
#define CAN1                ((CAN_T *) CAN1_BASE)                       /*!< CAN1 Configuration Struct                        */

#define PWM0                ((PWM_T *) PWM0_BASE)                       /*!< PWM0 Configuration Struct                        */
#define PWM1                ((PWM_T *) PWM1_BASE)                       /*!< PWM1 Configuration Struct                        */

#define BPWM0               ((BPWM_T *) BPWM0_BASE)                     /*!< BPWM0 Configuration Struct                       */
#define BPWM1               ((BPWM_T *) BPWM1_BASE)                     /*!< BPWM1 Configuration Struct                       */


/*@}*/ /* end of group PERIPHERAL */

//=============================================================================
/** @addtogroup IO_ROUTINE I/O routines
  The Declaration of I/O routines
  @{
 */

#define UNLOCKREG(x)        do{*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x59;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x16;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x88;}while(*((__IO uint32_t *)(GCR_BASE + 0x100))==0)
#define LOCKREG(x)          *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x00

#define REGCOPY(dest, src)  *((uint32_t *)&(dest)) = *((uint32_t *)&(src))
#define CLEAR(dest)         *((uint32_t *)&(dest)) = 0


typedef volatile unsigned char  vu8;        ///< Define 8-bit unsigned volatile data type
typedef volatile unsigned short vu16;       ///< Define 16-bit unsigned volatile data type
typedef volatile unsigned long  vu32;       ///< Define 32-bit unsigned volatile data type

/**
  * @brief Get a 8-bit unsigned value from specified address
  * @param[in] addr Address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified address
  */
#define M8(addr)  (*((vu8  *) (addr)))

/**
  * @brief Get a 16-bit unsigned value from specified address
  * @param[in] addr Address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified address
  * @note The input address must be 16-bit aligned
  */
#define M16(addr) (*((vu16 *) (addr)))

/**
  * @brief Get a 32-bit unsigned value from specified address
  * @param[in] addr Address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified address
  * @note The input address must be 32-bit aligned
  */
#define M32(addr) (*((vu32 *) (addr)))

/**
  * @brief Set a 32-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 32-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 32-bit aligned
  */
#define outpw(port,value)     *((volatile unsigned int *)(port)) = (value)

/**
  * @brief Get a 32-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified I/O port
  * @note The input port must be 32-bit aligned
  */
#define inpw(port)            (*((volatile unsigned int *)(port)))

/**
  * @brief Set a 16-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 16-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 16-bit aligned
  */
#define outps(port,value)     *((volatile unsigned short *)(port)) = (value)

/**
  * @brief Get a 16-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified I/O port
  * @note The input port must be 16-bit aligned
  */
#define inps(port)            (*((volatile unsigned short *)(port)))

/**
  * @brief Set a 8-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 8-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  */
#define outpb(port,value)     *((volatile unsigned char *)(port)) = (value)

/**
  * @brief Get a 8-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified I/O port
  */
#define inpb(port)            (*((volatile unsigned char *)(port)))

/**
  * @brief Set a 32-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 32-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 32-bit aligned
  */
#define outp32(port,value)    *((volatile unsigned int *)(port)) = (value)

/**
  * @brief Get a 32-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified I/O port
  * @note The input port must be 32-bit aligned
  */
#define inp32(port)           (*((volatile unsigned int *)(port)))

/**
  * @brief Set a 16-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 16-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 16-bit aligned
  */
#define outp16(port,value)    *((volatile unsigned short *)(port)) = (value)

/**
  * @brief Get a 16-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified I/O port
  * @note The input port must be 16-bit aligned
  */
#define inp16(port)           (*((volatile unsigned short *)(port)))

/**
  * @brief Set a 8-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 8-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  */
#define outp8(port,value)     *((volatile unsigned char *)(port)) = (value)

/**
  * @brief Get a 8-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified I/O port
  */
#define inp8(port)            (*((volatile unsigned char *)(port)))

/*@}*/ /* end of group IO_ROUTINE */




/** @addtogroup legacy_Constants Legacy Constants
  Legacy Constants
  @{
*/


#define E_SUCCESS   0
#ifndef NULL
#define NULL        0
#endif

#define TRUE        1
#define FALSE       0

#define ENABLE     1
#define DISABLE    0

/* Define one bit mask */
#define BIT0    0x00000001
#define BIT1    0x00000002
#define BIT2    0x00000004
#define BIT3    0x00000008
#define BIT4    0x00000010
#define BIT5    0x00000020
#define BIT6    0x00000040
#define BIT7    0x00000080
#define BIT8    0x00000100
#define BIT9    0x00000200
#define BIT10   0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13   0x00002000
#define BIT14   0x00004000
#define BIT15   0x00008000
#define BIT16   0x00010000
#define BIT17   0x00020000
#define BIT18   0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21   0x00200000
#define BIT22   0x00400000
#define BIT23   0x00800000
#define BIT24   0x01000000
#define BIT25   0x02000000
#define BIT26   0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000

/* Byte Mask Definitions */
#define BYTE0_Msk               (0x000000FF)
#define BYTE1_Msk               (0x0000FF00)
#define BYTE2_Msk               (0x00FF0000)
#define BYTE3_Msk               (0xFF000000)

#define _GET_BYTE0(u32Param)    (((u32Param) & BYTE0_Msk)      )  /*!< Extract Byte 0 (Bit  0~ 7) from parameter u32Param */
#define _GET_BYTE1(u32Param)    (((u32Param) & BYTE1_Msk) >>  8)  /*!< Extract Byte 1 (Bit  8~15) from parameter u32Param */
#define _GET_BYTE2(u32Param)    (((u32Param) & BYTE2_Msk) >> 16)  /*!< Extract Byte 2 (Bit 16~23) from parameter u32Param */
#define _GET_BYTE3(u32Param)    (((u32Param) & BYTE3_Msk) >> 24)  /*!< Extract Byte 3 (Bit 24~31) from parameter u32Param */

/*@}*/ /* end of group legacy_Constants */


/******************************************************************************/
/*                         Peripheral header files                            */
/******************************************************************************/
#include "sys.h"
#include "adc.h"
#include "can.h"
#include "fmc.h"
#include "gpio.h"
#include "i2c.h"
#include "pwm.h"
#include "bpwm.h"
#include "spi.h"
#include "timer.h"
#include "wdt.h"
#include "wwdt.h"
#include "uart.h"
#include "clk.h"

#ifdef __cplusplus
}
#endif

#endif // End of __NUC131_H__


/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/

