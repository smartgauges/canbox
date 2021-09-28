;/*---------------------------------------------------------------------------------------------------------*/
;/*                                                                                                         */
;/* SPDX-License-Identifier: Apache-2.0                                                                     */
;/*                                                                                                         */
;/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
;/*                                                                                                         */
;/*---------------------------------------------------------------------------------------------------------*/

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    MODULE  ?cstartup

    ;; Forward declaration of sections.
    SECTION CSTACK:DATA:NOROOT(3) ;; 8 bytes alignment

    SECTION .intvec:CODE:NOROOT(2);; 4 bytes alignment

    EXTERN  SystemInit	
    EXTERN  __iar_program_start
    PUBLIC  __vector_table

    DATA
__vector_table
    DCD     sfe(CSTACK)
    DCD     __iar_program_start

    DCD     NMI_Handler
    DCD     HardFault_Handler
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     SVC_Handler
    DCD     0
    DCD     0
    DCD     PendSV_Handler
    DCD     SysTick_Handler

    ; External Interrupts
    DCD     BOD_IRQHandler              ; Brownout low voltage detected interrupt                 
    DCD     WDT_IRQHandler              ; Watch Dog Timer interrupt                              
    DCD     EINT0_IRQHandler            ; External signal interrupt from PB.14 pin                
    DCD     EINT1_IRQHandler            ; External signal interrupt from PB.15 pin                
    DCD     GPAB_IRQHandler             ; GPIO interrupt from PA[15:0]/PB[13:0]     
    DCD     GPCDEF_IRQHandler          	; GPIO interrupt from PC[15:0]/PD[15:0]/PE[15:0]//PF[8:0]     
    DCD     Default_Handler             ; Reserved                                 
    DCD     Default_Handler             ; Reserved                                 
    DCD     TMR0_IRQHandler             ; Timer 0 interrupt                                      
    DCD     TMR1_IRQHandler             ; Timer 1 interrupt                                      
    DCD     TMR2_IRQHandler             ; Timer 2 interrupt                                      
    DCD     TMR3_IRQHandler             ; Timer 3 interrupt                                      
    DCD     UART02_IRQHandler           ; UART0/2 interrupt                                        
    DCD     UART1_IRQHandler            ; UART1 interrupt                                        
    DCD     SPI0_IRQHandler             ; SPI0 interrupt                                         
    DCD     UART3_IRQHandler            ; UART3 interrupt                                         
    DCD     UART4_IRQHandler            ; UART4 interrupt                                         
    DCD     UART5_IRQHandler            ; UART5 interrupt                                         
    DCD     I2C0_IRQHandler             ; I2C0 interrupt                                         
    DCD     I2C1_IRQHandler             ; I2C1 interrupt                                         
    DCD     CAN0_IRQHandler             ; CAN0 interrupt                                         
    DCD     CAN1_IRQHandler             ; CAN1 interrupt                                         
    DCD     PWM0_IRQHandler             ; PWM0 interrupt 
    DCD     PWM1_IRQHandler             ; PWM1 interrupt                                
    DCD     BPWM0_IRQHandler            ; BPWM0 interrupt                                          
    DCD     BPWM1_IRQHandler            ; BPWM1 interrupt          
    DCD     BRAKE0_IRQHandler           ; BRAKE0 interrupt  
    DCD     BRAKE1_IRQHandler           ; BRAKE1 interrupt
    DCD     PWRWU_IRQHandler            ; Clock controller interrupt for chip wake up from power-down state
    DCD     ADC_IRQHandler              ; ADC interrupt                                          
    DCD     CKD_IRQHandler              ; Clock detection interrupt
    DCD     RTC_IRQHandler              ; Real time clock interrupt                              

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
    THUMB
    PUBWEAK Reset_Handler   
    SECTION .text:CODE:REORDER(2)       ; 4 bytes alignment
Reset_Handler
        LDR     R0, =0x50000100
        ; Unlock Register                
        LDR     R1, =0x59
        STR     R1, [R0]
        LDR     R1, =0x16
        STR     R1, [R0]
        LDR     R1, =0x88
        STR     R1, [R0]
        
        ; Init POR
        LDR     R2, =0x50000024
        LDR     R1, =0x00005AA5
        STR     R1, [R2]
        
        ; Lock register
        MOVS    R1, #0
        STR     R1, [R0]                

        LDR      R0, =SystemInit
        BLX      R0
        LDR      R0, =__iar_program_start
        BX       R0

    PUBWEAK HardFault_Handler 
    PUBWEAK NMI_Handler       
    PUBWEAK SVC_Handler       
    PUBWEAK PendSV_Handler    
    PUBWEAK SysTick_Handler   
    PUBWEAK BOD_IRQHandler   
    PUBWEAK WDT_IRQHandler   
    PUBWEAK EINT0_IRQHandler 
    PUBWEAK EINT1_IRQHandler 
    PUBWEAK GPAB_IRQHandler  
    PUBWEAK GPCDEF_IRQHandler
    PUBWEAK TMR0_IRQHandler 
    PUBWEAK TMR1_IRQHandler 
    PUBWEAK TMR2_IRQHandler 
    PUBWEAK TMR3_IRQHandler 
    PUBWEAK UART02_IRQHandler
    PUBWEAK UART1_IRQHandler
    PUBWEAK SPI0_IRQHandler 
    PUBWEAK UART3_IRQHandler 
    PUBWEAK UART4_IRQHandler 
    PUBWEAK UART5_IRQHandler 
    PUBWEAK I2C0_IRQHandler 
    PUBWEAK I2C1_IRQHandler 
    PUBWEAK CAN0_IRQHandler  
    PUBWEAK CAN1_IRQHandler  
    PUBWEAK PWM0_IRQHandler 	
    PUBWEAK PWM1_IRQHandler   
    PUBWEAK BPWM0_IRQHandler   
    PUBWEAK BPWM1_IRQHandler  
    PUBWEAK BRAKE0_IRQHandler 
    PUBWEAK BRAKE1_IRQHandler
    PUBWEAK PWRWU_IRQHandler  
    PUBWEAK ADC_IRQHandler    
    PUBWEAK CKD_IRQHandler      
    PUBWEAK RTC_IRQHandler  
    SECTION .text:CODE:REORDER(2)
HardFault_Handler 
NMI_Handler       
SVC_Handler       
PendSV_Handler    
SysTick_Handler   
BOD_IRQHandler   
WDT_IRQHandler   
EINT0_IRQHandler 
EINT1_IRQHandler 
GPAB_IRQHandler  
GPCDEF_IRQHandler 
TMR0_IRQHandler  
TMR1_IRQHandler  
TMR2_IRQHandler  
TMR3_IRQHandler  
UART02_IRQHandler 
UART1_IRQHandler 
SPI0_IRQHandler  
UART3_IRQHandler  
UART4_IRQHandler  
UART5_IRQHandler  
I2C0_IRQHandler  
I2C1_IRQHandler  
CAN0_IRQHandler 
CAN1_IRQHandler  
PWM0_IRQHandler 
PWM1_IRQHandler   
BPWM0_IRQHandler   
BPWM1_IRQHandler  
BRAKE0_IRQHandler  
BRAKE1_IRQHandler
PWRWU_IRQHandler
ADC_IRQHandler    
CKD_IRQHandler
RTC_IRQHandler    
Default_Handler          
    B Default_Handler         

    
    END

