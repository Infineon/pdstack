/***************************************************************************//**
* \file cy_pdstack_timer_id.h
* \version 1.0
*
* Provides soft timer identifier definitions for PDStack.
*
********************************************************************************
* \copyright
* Copyright 2022-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_PDSTACK_TIMER_ID_H_
#define _CY_PDSTACK_TIMER_ID_H_

#include <stdint.h>

#define CY_PDSTACK_GET_PD_TIMER_ID(context, id)                                 \
    (uint16_t)(((context)->port != 0U) ? ((((uint16_t)id) & 0x00FFU) + (uint16_t)CY_PDSTACK_PD1_TIMERS_START_ID) : (uint16_t)(id))

/**
* \addtogroup group_pdstack_enums
* \{
*/
/**
 * @typedef cy_en_pdstack_timer_id_t
 * @brief List of soft timer IDs defined for PDStack.
 */
typedef enum {
    
    CY_PDSTACK_PD_TIMERS_START_ID = CY_PDUTILS_TIMER_PD_PORT0_START_ID,
    /**< 0x100: Starts the index for USB PD stack timers. */

    CY_PDSTACK_PD_CABLE_TIMER,
    /**< 0x101: Timer used for cable capability check. */

    CY_PDSTACK_PD_NO_RESPONSE_TIMER,
    /**< 0x102: Response timer. */

    CY_PDSTACK_PD_CBL_DSC_ID_TIMER,
    /**< 0x103: Timer used for cable discovery state machine. */

    CY_PDSTACK_PD_CBL_DELAY_TIMER,
    /**< 0x104: Timer used to enforce cable delay. */

    CY_PDSTACK_PD_PHY_BUSY_TIMER,
    /**< 0x105: Timer used to handle PHY busy status. */

    CY_PDSTACK_PD_GOOD_CRC_TX_TIMER,
    /**< 0x106: GoodCRC timer. */

    CY_PDSTACK_PD_HARD_RESET_TX_TIMER,
    /**< 0x107: Hard reset transmit timer. */

    CY_PDSTACK_PD_VCONN_SWAP_INITIATOR_TIMER,
    /**< 0x108: VConn swap initiator timer. */

    CY_PDSTACK_PD_GENERIC_TIMER,
    /**< 0x109: Generic AMS timer. */

    CY_PDSTACK_PD_PPS_TIMER,
    /**< 0x10A: PPS related timer. */

    CY_PDSTACK_PD_SINK_TX_TIMER,
    /**< 0x10B: PD 3.0 sink Rp flow control timer. */

    CY_PDSTACK_PD_DATA_RESET_COMP_TIMER,
    /**< 0x10C: Reserved for future use. */

    CY_PDSTACK_PD_SNK_EPR_MODE_TIMER,
    /**< 0x10D: Timer used for PD sink EPR mode. */

    CY_PDSTACK_PD_SRC_EPR_MODE_TIMER,
    /**< 0x10E: Timer used for PD source EPR mode. */

    CY_PDSTACK_PD_EPR_KEEPALIVE_TIMER,
    /**< 0x10F: Timer used by the EPR state machine for sending Keepalive message. */

    CY_PDSTACK_PD_TIMER_RESERVED_16 = 0x110u,
    /**< 0x110: Reserved for future use. */

    CY_PDSTACK_PD_TIMERS_END_ID = 0x110u,
    /**< 0x110: End index (inclusive) for USB PD stack timers. */

    CY_PDSTACK_TYPEC_TIMERS_START_ID = 0x111u,
    /**< 0x111: Starts the index for Type-C timers. */

    CY_PDSTACK_TYPEC_GENERIC_TIMER2 = 0x111u,
    /**< 0x111: Generic Type-C state machine timer #2. */

    CY_PDSTACK_TYPEC_GENERIC_TIMER1,
    /**< 0x112: Generic Type-C state machine timer #1. */

    CY_PDSTACK_TYPEC_CC1_DEBOUNCE_TIMER,
    /**< 0x113: Timer used for CC1 debounce. */

    CY_PDSTACK_TYPEC_CC2_DEBOUNCE_TIMER,
    /**< 0x114: Timer used for CC2 debounce. */

    CY_PDSTACK_TYPEC_RD_DEBOUNCE_TIMER,
    /**< 0x115: Timer used for Rd debounce. */

    CY_PDSTACK_TYPEC_VBUS_DISCHARGE_TIMER,
    /**< 0x116: VBus discharge timer ID. */

    CY_PDSTACK_TYPEC_ACTIVITY_TIMER,
    /**< 0x117: Type-C activity timer ID. */

    CY_PDSTACK_TYPEC_RP_CHANGE_TIMER,
    /**< 0x118: Timer used to trigger current limit update after Rp change. */

    CY_PDSTACK_TYPEC_TIMER_RESERVED_25,
    /**< 0x119: Reserved for future use. */

    CY_PDSTACK_TYPEC_TIMERS_END_ID,
    /**< 0x11A: End index (inclusive) for Type-C timers. */

    CY_PDSTACK_PD_OCP_DEBOUNCE_TIMER,
    /**< 0x11B: Timer used for FW debounce of VBus OCP. */

    CY_PDSTACK_HPD_RX_ACTIVITY_TIMER_ID,
    /**< 0x11C: Timer used for HPD receive handling. */

    CY_PDSTACK_PD_VCONN_OCP_DEBOUNCE_TIMER,
    /**< 0x11D: Timer used for FW debounce of VConn OCP. */

    CY_PDSTACK_PD_TIMER_RESERVED_30,
    /**< 0x11E: Reserved for future use. */

    CY_PDSTACK_PD_TIMER_RESERVED_31,
    /**< 0x11F: Reserved for future use. */
    
    CY_PDSTACK_PD_ALTMODE_TIMERS_START_ID = 0x180u,
    /**< 0x180: Starts the index for USB PD alternate mode stack timers. */
    
    CY_PDSTACK_PD1_TIMERS_START_ID = CY_PDUTILS_TIMER_PD_PORT1_START_ID,
    /**< 0x200: Starts the index for USB PD stack timers. */

    CY_PDSTACK_PD1_CABLE_TIMER,
    /**< 0x201: Timer used for cable capability check. */

    CY_PDSTACK_PD1_NO_RESPONSE_TIMER,
    /**< 0x202: Response timer. */

    CY_PDSTACK_PD1_CBL_DSC_ID_TIMER,
    /**< 0x203: Timer used for cable discovery state machine. */

    CY_PDSTACK_PD1_CBL_DELAY_TIMER,
    /**< 0x204: Timer used to enforce cable delay. */

    CY_PDSTACK_PD1_PHY_BUSY_TIMER,
    /**< 0x205: Timer used to handle PHY busy status. */

    CY_PDSTACK_PD1_GOOD_CRC_TX_TIMER,
    /**< 0x206: GoodCRC timer. */

    CY_PDSTACK_PD1_HARD_RESET_TX_TIMER,
    /**< 0x207: Hard reset transmit timer. */

    CY_PDSTACK_PD1_VCONN_SWAP_INITIATOR_TIMER,
    /**< 0x208: VConn swap initiator timer. */

    CY_PDSTACK_PD1_GENERIC_TIMER,
    /**< 0x209: Generic AMS timer. */

    CY_PDSTACK_PD1_PPS_TIMER,
    /**< 0x20A: PPS-related timer. */

    CY_PDSTACK_PD1_SINK_TX_TIMER,
    /**< 0x20B: PD 3.0 sink Rp flow control timer. */

    CY_PDSTACK_PD1_DATA_RESET_COMP_TIMER,
    /**< 0x20C: Reserved for future use. */

    CY_PDSTACK_PD1_SNK_EPR_MODE_TIMER,
    /**< 0x20D: Timer used for PD sink EPR mode. */

    CY_PDSTACK_PD1_SRC_EPR_MODE_TIMER,
    /**< 0x20E: Timer used for PD source EPR mode. */

    CY_PDSTACK_PD1_EPR_KEEPALIVE_TIMER,
    /**< 0x20F: Timer used by the EPR state machine for sending Keepalive message. */

    CY_PDSTACK_PD1_TIMER_RESERVED_16 = 0x210u,
    /**< 0x210: Reserved for future use. */

    CY_PDSTACK_PD1_TIMERS_END_ID = 0x210u,
    /**< 0x210: End index (inclusive) for USB PD stack timers. */

    CY_PDSTACK_TYPEC1_TIMERS_START_ID = 0x211u,
    /**< 0x211: Starts the index for Type-C timers. */

    CY_PDSTACK_TYPEC1_GENERIC_TIMER2 = 0x211u,
    /**< 0x211: Generic Type-C state machine timer #2. */

    CY_PDSTACK_TYPEC1_GENERIC_TIMER1,
    /**< 0x212: Generic Type-C state machine timer #1. */

    CY_PDSTACK_TYPEC1_CC1_DEBOUNCE_TIMER,
    /**< 0x213: Timer used for CC1 debounce. */

    CY_PDSTACK_TYPEC1_CC2_DEBOUNCE_TIMER,
    /**< 0x214: Timer used for CC2 debounce. */

    CY_PDSTACK_TYPEC1_RD_DEBOUNCE_TIMER,
    /**< 0x215: Timer used for Rd debounce. */

    CY_PDSTACK_TYPEC1_VBUS_DISCHARGE_TIMER,
    /**< 0x216: VBus discharge timer ID. */

    CY_PDSTACK_TYPEC1_ACTIVITY_TIMER,
    /**< 0x217: Type-C activity timer ID. */

    CY_PDSTACK_TYPEC1_RP_CHANGE_TIMER,
    /**< 0x218: Timer used to trigger current limit update after Rp change. */

    CY_PDSTACK_TYPEC1_TIMER_RESERVED_25,
    /**< 0x219: Reserved for future use. */

    CY_PDSTACK_TYPEC1_TIMERS_END_ID,
    /**< 0x21A: Ends the index (inclusive) for Type-C timers. */

    CY_PDSTACK_PD1_OCP_DEBOUNCE_TIMER,
    /**< 0x21B: Timer used for FW debounce of VBus OCP. */

    CY_PDSTACK_HPD1_RX_ACTIVITY_TIMER_ID,
    /**< 0x21C: Timer used for HPD receive handling. */

    CY_PDSTACK_PD1_VCONN_OCP_DEBOUNCE_TIMER,
    /**< 0x21D: Timer used for FW debounce of VConn OCP. */

} cy_en_pdstack_timer_id_t;

/** \} group_pdstack_enums */

#endif /* _CY_PDSTACK_TIMER_ID_H_ */

/* [] END OF FILE */
