/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 6 MCU with SensiML Software
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include <sensor.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "audio_rec.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cy_rgb_led.h"
#include "app_config.h"

#if (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)
#include "kb.h"
#endif // (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)


#define TASK_STACK_SIZE 8096
#define TASK_PRIORITY   2

/*******************************************************************************
* Global Variables
********************************************************************************/
/* This enables RTOS aware debugging */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();
#if APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE
    kb_model_init();
#endif //APPLICATION_RUNNING_MODE RECOGNITION_RUNNING_MODE

    /* Initialize retarget-io to use the debug UART port */
    if(CY_RSLT_SUCCESS != cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 1000000))
    {
        while(1);
    }
    #if(SENSOR_SELECT_MODE==SENSOR_MOTION)
        /* Create one task, sends motion sensor data via UART */
        xTaskCreate(sensor_task, "sensortask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    #elif(SENSOR_SELECT_MODE==SENSOR_AUDIO)
        /* Create one task, sends audio data via UART */
        xTaskCreate(audio_task, "audiotask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    #endif
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    for (;;)
    {
    }
}

/*******************************************************************************
* Function Name: vApplicationDaemonTaskStartupHook
********************************************************************************
* Summary:
*   Executes after the scheduler starts.
*
*******************************************************************************/
void vApplicationDaemonTaskStartupHook(void)
{
    /* Initializes the inference engine */
    cy_rslt_t result;


    result = cy_rgb_led_init(CYBSP_LED_RGB_RED, CYBSP_LED_RGB_GREEN, CYBSP_LED_RGB_BLUE, CY_RGB_LED_ACTIVE_LOW);
    if (result != CY_RSLT_SUCCESS) {
        NVIC_SystemReset();
    }

    #if(SENSOR_SELECT_MODE==SENSOR_MOTION)
    result = sensor_init();
    if(CY_RSLT_SUCCESS != result)
    {
        /* Reset the system on sensor fail */
        NVIC_SystemReset();
    }
    #elif(SENSOR_SELECT_MODE==SENSOR_AUDIO)
    result = audio_init();
    if(CY_RSLT_SUCCESS != result)
    {
        /* Reset the system on sensor fail */
        NVIC_SystemReset();
    }
    #endif
}


/* [] END OF FILE */
