/*******************************************************************************
* File Name: PWM_rgb_green_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_rgb_green.h"

static PWM_rgb_green_backupStruct PWM_rgb_green_backup;


/*******************************************************************************
* Function Name: PWM_rgb_green_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_rgb_green_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_rgb_green_SaveConfig(void) 
{

    #if(!PWM_rgb_green_UsingFixedFunction)
        #if(!PWM_rgb_green_PWMModeIsCenterAligned)
            PWM_rgb_green_backup.PWMPeriod = PWM_rgb_green_ReadPeriod();
        #endif /* (!PWM_rgb_green_PWMModeIsCenterAligned) */
        PWM_rgb_green_backup.PWMUdb = PWM_rgb_green_ReadCounter();
        #if (PWM_rgb_green_UseStatus)
            PWM_rgb_green_backup.InterruptMaskValue = PWM_rgb_green_STATUS_MASK;
        #endif /* (PWM_rgb_green_UseStatus) */

        #if(PWM_rgb_green_DeadBandMode == PWM_rgb_green__B_PWM__DBM_256_CLOCKS || \
            PWM_rgb_green_DeadBandMode == PWM_rgb_green__B_PWM__DBM_2_4_CLOCKS)
            PWM_rgb_green_backup.PWMdeadBandValue = PWM_rgb_green_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_rgb_green_KillModeMinTime)
             PWM_rgb_green_backup.PWMKillCounterPeriod = PWM_rgb_green_ReadKillTime();
        #endif /* (PWM_rgb_green_KillModeMinTime) */

        #if(PWM_rgb_green_UseControl)
            PWM_rgb_green_backup.PWMControlRegister = PWM_rgb_green_ReadControlRegister();
        #endif /* (PWM_rgb_green_UseControl) */
    #endif  /* (!PWM_rgb_green_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_rgb_green_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_rgb_green_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_rgb_green_RestoreConfig(void) 
{
        #if(!PWM_rgb_green_UsingFixedFunction)
            #if(!PWM_rgb_green_PWMModeIsCenterAligned)
                PWM_rgb_green_WritePeriod(PWM_rgb_green_backup.PWMPeriod);
            #endif /* (!PWM_rgb_green_PWMModeIsCenterAligned) */

            PWM_rgb_green_WriteCounter(PWM_rgb_green_backup.PWMUdb);

            #if (PWM_rgb_green_UseStatus)
                PWM_rgb_green_STATUS_MASK = PWM_rgb_green_backup.InterruptMaskValue;
            #endif /* (PWM_rgb_green_UseStatus) */

            #if(PWM_rgb_green_DeadBandMode == PWM_rgb_green__B_PWM__DBM_256_CLOCKS || \
                PWM_rgb_green_DeadBandMode == PWM_rgb_green__B_PWM__DBM_2_4_CLOCKS)
                PWM_rgb_green_WriteDeadTime(PWM_rgb_green_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_rgb_green_KillModeMinTime)
                PWM_rgb_green_WriteKillTime(PWM_rgb_green_backup.PWMKillCounterPeriod);
            #endif /* (PWM_rgb_green_KillModeMinTime) */

            #if(PWM_rgb_green_UseControl)
                PWM_rgb_green_WriteControlRegister(PWM_rgb_green_backup.PWMControlRegister);
            #endif /* (PWM_rgb_green_UseControl) */
        #endif  /* (!PWM_rgb_green_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_rgb_green_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_rgb_green_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_rgb_green_Sleep(void) 
{
    #if(PWM_rgb_green_UseControl)
        if(PWM_rgb_green_CTRL_ENABLE == (PWM_rgb_green_CONTROL & PWM_rgb_green_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_rgb_green_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_rgb_green_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_rgb_green_UseControl) */

    /* Stop component */
    PWM_rgb_green_Stop();

    /* Save registers configuration */
    PWM_rgb_green_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_rgb_green_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_rgb_green_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_rgb_green_Wakeup(void) 
{
     /* Restore registers values */
    PWM_rgb_green_RestoreConfig();

    if(PWM_rgb_green_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_rgb_green_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
