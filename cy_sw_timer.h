/***************************************************************************//**
* \file cy_sw_timer.h
* \version 2.0
*
* Provides API declarations of the Software Timer implementation.
*
********************************************************************************
* \copyright
* Copyright 2021-2022, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_SW_TIMER_H_
#define _CY_SW_TIMER_H_

#include "stdint.h"
#include "stdbool.h"
#include "cy_pdl.h"


/*****************************************************************************
 * MACRO Definition
 *****************************************************************************/

/**
* \addtogroup group_pdstack_macros
* \{
*/

/** Systick timer */
#define TIMER_TYPE_SYSTICK              (1u)

/** WDT Timer */
#define TIMER_TYPE_WDT                  (2u)

/** Timer Type */
#define TIMER_TYPE                      TIMER_TYPE_WDT

/** Number of timer instances. */
#define TIMER_NUM_INSTANCES             (1u)

/** The SYS_CLK frequency should match the actual hardware SYS_CLK frequency. */
#define SYSCLK_FREQUENCY                (SystemCoreClock)

/** The SYS_CLK counter threshold for 1ms timeout. Division is expected
 * to be optimized out by the compiler. */
#define SYSCLK_1MS_COUNT_VALUE          (SYSCLK_FREQUENCY / 1000u)

/** Mask for the WDT Counter value. */
#define WDT_COUNTER_COUNTER_MASK        (0x0000ffff)

#if (TIMER_TYPE == TIMER_TYPE_WDT)

/**
 * Maximum timeout value allowed. This value comprehends the maximum interrupt
 * latencies so as not to overrun. Providing for a worst case number, 64 ticks
 * is used.
 */
#define TIMER_HW_MAX_TIMEOUT            (0xFFC0u)

/**
 * This time covers for the worst case interrupt latency
 * for the timer interrupt handler. Considering the worst
 * case timing of 100us
 */
#define TIMER_OVERRUN_THRESHOLD         (5u)

/** Timer counter size in number of ticks. */
#define TIMER_NUM_TICK_COUNTS           (0x10000u)

#endif /* ((TIMER_TYPE == TIMER_TYPE_WDT)) */

/** Number of soft timers supported per instance of the timer module. The
   number of instances depends on the number of USB-PD ports supported by
   the device. */
#define TIMER_NUM_TIMERS                (63u)


/** Maximum timeout value in milliseconds supported by the module. */
#define TIMER_MAX_TIMEOUT               (0xFFFF)

/** Invalid timer ID value. */
#define TIMER_INVALID_ID                (0xFFFFu)

/** Invalid timer instance. */
#define TIMER_INVALID_INDEX             (0xFFu)

/** Macro to control whether to use Tickless timer */
#define TIMER_TICKLESS_ENABLE              (1)

/** \} group_pdstack_macros */

/*******************************************************************************
*                              Type Definitions
*******************************************************************************/

/**
* \addtogroup group_pdstack_data_structures
* \{
*/

/**
 * @typedef cy_timer_id_t
 * @brief Timer ID type definition.
 *
 * This type definition is used to identify software timer objects. The timer ID
 * needs to be unique for each soft timer instance and need to be maintained in
 * the application. To maintain uniqueness, the following timer ID allocation
 * rule is expected to be followed.
 *
 * - PD and Type-C stack (Port-0)         :   0 -  31 : 32 timers
 * - PD and Type-C stack (Port-1)         :  32 -  63 : 32 timers
 * - Base application stack (Port-0)      :  64 - 191 : 128 timers
 * - Base application stack (Port-1)      : 192 - 319 : 128 timers
 * - Solution (project directory)         : 324 - 387 : 63 timers
 */
typedef uint16_t cy_timer_id_t;

/**
 * @typedef cy_cb_timer_t
 * @brief Timer callback function.
 *
 * This callback function is invoked on timer expiry and
 * should be treated as interrupt.
 */
typedef void (*cy_cb_timer_t)(
        cy_timer_id_t id,           /**< Timer ID for which callback is being generated. */
        void *callbackContext       /**< Timer module instance number. */
        );

/**
 * @brief Structure encapsulating information relating to a software timer.
 *
 * This structure encapsulates all information related to a software timer
 * object. The timer module maintains a list of these objects corresponding to
 * the active software timers at any given point of time.
 */
typedef struct
{
    /** The actual count value. */
    uint32_t count;
    /** The callback function pointer for the timer object. */
    cy_cb_timer_t cb;
    /** Period of operation for the timer instance. */
    uint16_t period;
    /** The ID handle associated with the timer object. */
    cy_timer_id_t id;
    /** Pointer to the callback context */
    void *callbackContext;
} cy_timer_t;

/**
 * @brief Structure encapsulating information relating to a software timer
 * context.
 *
 */
typedef struct
{
    /** SysTick clock frequency in MHz. */
    uint32_t sys_clk_freq;

    /** Tick time at timer start. */
    uint16_t volatile start_tick;

    /** Tick time from the start to match. */
    uint16_t volatile tick_time;

    /** Match register content. */
    uint16_t volatile match;

    /** Multiplier for 1ms interval */
    uint16_t multiplier;

    /** Handle for software timers. */
    cy_timer_t volatile timer_handle[TIMER_NUM_TIMERS];

    /** Number of active timers. */
    uint8_t volatile num_active;

    /** Total number of active timers. */
    uint8_t volatile total_active;

    /** Flag indicating whether invoked from timer callback. */
    bool volatile is_callback;

} cy_stc_sw_timer_t;

/** \} group_pdstack_data_structures */


/*******************************************************************************
*                            Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_pdstack_functions
* \{
*/

/*******************************************************************************
* Function Name: cy_sw_timer_init
****************************************************************************//**
*
* This function initializes the software timer module. This function
* initializes the data structures for timer management and enables the hardware
* timer used for the soft timer implementation.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param sysClkFreq
* SysClk frequency in MHz. This value is used to calibrate the watchdog timer.
*
* \return
* None
*
*******************************************************************************/

void cy_sw_timer_init(cy_stc_sw_timer_t *context, uint32_t sysClkFreq);

/*******************************************************************************
* Function Name: cy_sw_timer_start
****************************************************************************//**
*
* Start a specific soft timer. All soft timers are one-shot timers which will
* run until the specified period has elapsed. The timer expiration callback
* will be called at the end of the period, if one is provided.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param callbackContext
* Pointer to the caller context.
*
* \param id
* Unique timer id
*
* \param period
* Timer period in milliseconds.
*
* \param cb
* Timer expiration callback. Can be NULL.
*
* \return bool
* True if the timer is started, false if timer start fails.
*
*******************************************************************************/
bool cy_sw_timer_start(cy_stc_sw_timer_t *context, void *callbackContext,
                       cy_timer_id_t id, uint16_t period, cy_cb_timer_t cb);

/*******************************************************************************
* Function Name: cy_sw_timer_start_wocb
****************************************************************************//**
*
* Start a specific soft timer without providing a callback for expiry notification.
* The status of the timer (running or not) can be checked using the
* \ref cy_sw_timer_is_running API.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param id
* Unique timer id
*
* \param period
* Timer period in milliseconds.
*
* \return bool
* True if the timer is started, false if timer start fails.
*
*******************************************************************************/

bool cy_sw_timer_start_wocb(cy_stc_sw_timer_t *context,
                            cy_timer_id_t id, uint16_t period);

/*******************************************************************************
* Function Name: cy_sw_timer_stop
****************************************************************************//**
*
* Stop a soft timer which is currently running.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param id
* Unique timer id
*
* \return None
*
*
*******************************************************************************/

void cy_sw_timer_stop(cy_stc_sw_timer_t *context, cy_timer_id_t id);

/*******************************************************************************
* Function Name: cy_sw_timer_is_running
****************************************************************************//**
*
* Check whether the specified soft timer is currently running.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param id
* Unique timer id
*
*
* \return bool
* True if the timer is running, false otherwise.
*
*******************************************************************************/

bool cy_sw_timer_is_running (cy_stc_sw_timer_t *context, cy_timer_id_t id);

/*******************************************************************************
* Function Name: cy_sw_timer_range_enabled
****************************************************************************//**
*
* Check whether the any of the soft timers in the specified range are currently running.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param low
* Lowest soft timer ID to be checked.
*
* \param high
* Highest soft timer ID to be checked.
*
* \return bool
* True if the timers are running, false otherwise.
*
*******************************************************************************/

bool cy_sw_timer_range_enabled(cy_stc_sw_timer_t *context,
                               cy_timer_id_t low, cy_timer_id_t high);

/*******************************************************************************
* Function Name: cy_sw_timer_get_count
****************************************************************************//**
*
* Returns the time (in ms) remaining for timer expiration.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param id
* Unique timer id
*
*
* \return
* Time remaining for expiration of the soft timer.
*
*******************************************************************************/

uint16_t cy_sw_timer_get_count(cy_stc_sw_timer_t *context, cy_timer_id_t id);

/*******************************************************************************
* Function Name: cy_sw_timer_stop_all
****************************************************************************//**
*
* Stops all active soft timers.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return None
*
*******************************************************************************/

void cy_sw_timer_stop_all(cy_stc_sw_timer_t *context);

/*******************************************************************************
* Function Name: cy_sw_timer_stop_range
****************************************************************************//**
*
* This function stops all soft timers with ids in the specified range.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \param start
* Starting timer ID. The value is inclusive.
*
* \param end
* Ending timer ID. The value is inclusive.
*
* \return None
*
*
*******************************************************************************/
void cy_sw_timer_stop_range(cy_stc_sw_timer_t *context,
                            cy_timer_id_t start, cy_timer_id_t end);

/*******************************************************************************
* Function Name: cy_sw_timer_num_active
****************************************************************************//**
*
* Returns number of active timers.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Number of Active Timers
*
*******************************************************************************/

uint8_t cy_sw_timer_num_active(cy_stc_sw_timer_t *context);

/*******************************************************************************
* Function Name: cy_sw_timer_enter_sleep
****************************************************************************//**
*
* This function prepares the timer module and the hardware timer for entering
* device deep sleep. This must be called prior to entering deep sleep mode.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* None
*
*******************************************************************************/

void cy_sw_timer_enter_sleep(cy_stc_sw_timer_t *context);

/*******************************************************************************
* Function Name: cy_sw_timer_interrupt_handler
****************************************************************************//**
*
* This function implements the interrupt handler callback used by the Soft Timer
* module, and is expected to be called by the application whenever the appropriate
* SysTick or WDT interrupt is received.
*
* This function scans all running timer instances and decrements their count
* as appropriate. If any of the timers has expired, it is stopped, de-allocated and
* the corresponding callback called (if not NULL). The timer can be restarted
* from the callback function if so desired.

* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* None
*
*******************************************************************************/

void cy_sw_timer_interrupt_handler(cy_stc_sw_timer_t *context);

/*******************************************************************************
* Function Name: cy_sw_timer_get_multiplier
****************************************************************************//**
*
* This function retrieves the number of low frequency (LF) clock ticks required per ms.
*
* \param context
* The pointer to the context structure \ref cy_stc_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Returns the number of LF clock ticks per ms.
*
*******************************************************************************/

uint16_t cy_sw_timer_get_multiplier(cy_stc_sw_timer_t *context);

#endif /* _CY_SW_TIMER_H_ */

/** \} group_pdstack_functions */

/* [] END OF FILE */
