/*******************************************************************************
* File Name: PWM_rgb_blue_PM.c
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

#include "PWM_rgb_blue.h"

static PWM_rgb_blue_backupStruct PWM_rgb_blue_backup;


/*******************************************************************************
* Function Name: PWM_rgb_blue_SaveConfig
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
*  PWM_rgb_blue_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_rgb_blue_SaveConfig(void) 
{

    #if(!PWM_rgb_blue_UsingFixedFunction)
        #if(!PWM_rgb_blue_PWMModeIsCenterAligned)
            PWM_rgb_blue_backup.PWMPeriod = PWM_rgb_blue_ReadPeriod();
        #endif /* (!PWM_rgb_blue_PWMModeIsCenterAligned) */
        PWM_rgb_blue_backup.PWMUdb = PWM_rgb_blue_ReadCounter();
        #if (PWM_rgb_blue_UseStatus)
            PWM_rgb_blue_backup.InterruptMaskValue = PWM_rgb_blue_STATUS_MASK;
        #endif /* (PWM_rgb_blue_UseStatus) */

        #if(PWM_rgb_blue_DeadBandMode == PWM_rgb_blue__B_PWM__DBM_256_CLOCKS || \
            PWM_rgb_blue_DeadBandMode == PWM_rgb_blue__B_PWM__DBM_2_4_CLOCKS)
            PWM_rgb_blue_backup.PWMdeadBandValue = PWM_rgb_blue_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_rgb_blue_KillModeMinTime)
             PWM_rgb_blue_backup.PWMKillCounterPeriod = PWM_rgb_blue_ReadKillTime();
        #endif /* (PWM_rgb_blue_KillModeMinTime) */

        #if(PWM_rgb_blue_UseControl)
            PWM_rgb_blue_backup.PWMControlRegister = PWM_rgb_blue_ReadControlRegister();
        #endif /* (PWM_rgb_blue_UseControl) */
    #endif  /* (!PWM_rgb_blue_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_rgb_blue_RestoreConfig
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
*  PWM_rgb_blue_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_rgb_blue_RestoreConfig(void) 
{
        #if(!PWM_rgb_blue_UsingFixedFunction)
            #if(!PWM_rgb_blue_PWMModeIsCenterAligned)
                PWM_rgb_blue_WritePeriod(PWM_rgb_blue_backup.PWMPeriod);
            #endif /* (!PWM_rgb_blue_PWMModeIsCenterAligned) */

            PWM_rgb_blue_WriteCounter(PWM_rgb_blue_backup.PWMUdb);

            #if (PWM_rgb_blue_UseStatus)
                PWM_rgb_blue_STATUS_MASK = PWM_rgb_blue_backup.InterruptMaskValue;
            #endif /* (PWM_rgb_blue_UseStatus) */

            #if(PWM_rgb_blue_DeadBandMode == PWM_rgb_blue__B_PWM__DBM_256_CLOCKS || \
                PWM_rgb_blue_DeadBandMode == PWM_rgb_blue__B_PWM__DBM_2_4_CLOCKS)
                PWM_rgb_blue_WriteDeadTime(PWM_rgb_blue_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_rgb_blue_KillModeMinTime)
                PWM_rgb_blue_WriteKillTime(PWM_rgb_blue_backup.PWMKillCounterPeriod);
            #endif /* (PWM_rgb_blue_KillModeMinTime) */

            #if(PWM_rgb_blue_UseControl)
                PWM_rgb_blue_WriteControlRegister(PWM_rgb_blue_backup.PWMControlRegister);
            #endif /* (PWM_rgb_blue_UseControl) */
        #endif  /* (!PWM_rgb_blue_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_rgb_blue_Sleep
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
*  PWM_rgb_blue_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_rgb_blue_Sleep(void) 
{
    #if(PWM_rgb_blue_UseControl)
        if(PWM_rgb_blue_CTRL_ENABLE == (PWM_rgb_blue_CONTROL & PWM_rgb_blue_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_rgb_blue_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_rgb_blue_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_rgb_blue_UseControl) */

    /* Stop component */
    PWM_rgb_blue_Stop();

    /* Save registers configuration */
    PWM_rgb_blue_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_rgb_blue_Wakeup
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
*  PWM_rgb_blue_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_rgb_blue_Wakeup(void) 
{
     /* Restore registers values */
    PWM_rgb_blue_RestoreConfig();

    if(PWM_rgb_blue_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_rgb_blue_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
