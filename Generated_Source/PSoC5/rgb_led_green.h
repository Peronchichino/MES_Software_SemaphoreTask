/*******************************************************************************
* File Name: rgb_led_green.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_rgb_led_green_H) /* Pins rgb_led_green_H */
#define CY_PINS_rgb_led_green_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "rgb_led_green_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 rgb_led_green__PORT == 15 && ((rgb_led_green__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    rgb_led_green_Write(uint8 value);
void    rgb_led_green_SetDriveMode(uint8 mode);
uint8   rgb_led_green_ReadDataReg(void);
uint8   rgb_led_green_Read(void);
void    rgb_led_green_SetInterruptMode(uint16 position, uint16 mode);
uint8   rgb_led_green_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the rgb_led_green_SetDriveMode() function.
     *  @{
     */
        #define rgb_led_green_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define rgb_led_green_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define rgb_led_green_DM_RES_UP          PIN_DM_RES_UP
        #define rgb_led_green_DM_RES_DWN         PIN_DM_RES_DWN
        #define rgb_led_green_DM_OD_LO           PIN_DM_OD_LO
        #define rgb_led_green_DM_OD_HI           PIN_DM_OD_HI
        #define rgb_led_green_DM_STRONG          PIN_DM_STRONG
        #define rgb_led_green_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define rgb_led_green_MASK               rgb_led_green__MASK
#define rgb_led_green_SHIFT              rgb_led_green__SHIFT
#define rgb_led_green_WIDTH              1u

/* Interrupt constants */
#if defined(rgb_led_green__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in rgb_led_green_SetInterruptMode() function.
     *  @{
     */
        #define rgb_led_green_INTR_NONE      (uint16)(0x0000u)
        #define rgb_led_green_INTR_RISING    (uint16)(0x0001u)
        #define rgb_led_green_INTR_FALLING   (uint16)(0x0002u)
        #define rgb_led_green_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define rgb_led_green_INTR_MASK      (0x01u) 
#endif /* (rgb_led_green__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define rgb_led_green_PS                     (* (reg8 *) rgb_led_green__PS)
/* Data Register */
#define rgb_led_green_DR                     (* (reg8 *) rgb_led_green__DR)
/* Port Number */
#define rgb_led_green_PRT_NUM                (* (reg8 *) rgb_led_green__PRT) 
/* Connect to Analog Globals */                                                  
#define rgb_led_green_AG                     (* (reg8 *) rgb_led_green__AG)                       
/* Analog MUX bux enable */
#define rgb_led_green_AMUX                   (* (reg8 *) rgb_led_green__AMUX) 
/* Bidirectional Enable */                                                        
#define rgb_led_green_BIE                    (* (reg8 *) rgb_led_green__BIE)
/* Bit-mask for Aliased Register Access */
#define rgb_led_green_BIT_MASK               (* (reg8 *) rgb_led_green__BIT_MASK)
/* Bypass Enable */
#define rgb_led_green_BYP                    (* (reg8 *) rgb_led_green__BYP)
/* Port wide control signals */                                                   
#define rgb_led_green_CTL                    (* (reg8 *) rgb_led_green__CTL)
/* Drive Modes */
#define rgb_led_green_DM0                    (* (reg8 *) rgb_led_green__DM0) 
#define rgb_led_green_DM1                    (* (reg8 *) rgb_led_green__DM1)
#define rgb_led_green_DM2                    (* (reg8 *) rgb_led_green__DM2) 
/* Input Buffer Disable Override */
#define rgb_led_green_INP_DIS                (* (reg8 *) rgb_led_green__INP_DIS)
/* LCD Common or Segment Drive */
#define rgb_led_green_LCD_COM_SEG            (* (reg8 *) rgb_led_green__LCD_COM_SEG)
/* Enable Segment LCD */
#define rgb_led_green_LCD_EN                 (* (reg8 *) rgb_led_green__LCD_EN)
/* Slew Rate Control */
#define rgb_led_green_SLW                    (* (reg8 *) rgb_led_green__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define rgb_led_green_PRTDSI__CAPS_SEL       (* (reg8 *) rgb_led_green__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define rgb_led_green_PRTDSI__DBL_SYNC_IN    (* (reg8 *) rgb_led_green__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define rgb_led_green_PRTDSI__OE_SEL0        (* (reg8 *) rgb_led_green__PRTDSI__OE_SEL0) 
#define rgb_led_green_PRTDSI__OE_SEL1        (* (reg8 *) rgb_led_green__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define rgb_led_green_PRTDSI__OUT_SEL0       (* (reg8 *) rgb_led_green__PRTDSI__OUT_SEL0) 
#define rgb_led_green_PRTDSI__OUT_SEL1       (* (reg8 *) rgb_led_green__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define rgb_led_green_PRTDSI__SYNC_OUT       (* (reg8 *) rgb_led_green__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(rgb_led_green__SIO_CFG)
    #define rgb_led_green_SIO_HYST_EN        (* (reg8 *) rgb_led_green__SIO_HYST_EN)
    #define rgb_led_green_SIO_REG_HIFREQ     (* (reg8 *) rgb_led_green__SIO_REG_HIFREQ)
    #define rgb_led_green_SIO_CFG            (* (reg8 *) rgb_led_green__SIO_CFG)
    #define rgb_led_green_SIO_DIFF           (* (reg8 *) rgb_led_green__SIO_DIFF)
#endif /* (rgb_led_green__SIO_CFG) */

/* Interrupt Registers */
#if defined(rgb_led_green__INTSTAT)
    #define rgb_led_green_INTSTAT            (* (reg8 *) rgb_led_green__INTSTAT)
    #define rgb_led_green_SNAP               (* (reg8 *) rgb_led_green__SNAP)
    
	#define rgb_led_green_0_INTTYPE_REG 		(* (reg8 *) rgb_led_green__0__INTTYPE)
#endif /* (rgb_led_green__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_rgb_led_green_H */


/* [] END OF FILE */
