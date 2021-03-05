/***************************************************************************//**
* \file cy_sw_timer_id.h
* \version 1.0
*
* Provides Soft Timer Identifier definitions.
*
********************************************************************************
* \copyright
* Copyright 2021, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _TIMER_ID_H_
#define _TIMER_ID_H_

#include <stdint.h>

#define CY_PDSTACK_GET_PD_TIMER_ID(context, id)                                 \
    (uint8_t)(((context)->port != 0u) ? ((uint8_t)(id) + (uint8_t)PD1_TIMERS_START_ID) : (uint8_t)(id))

#define CY_PDSTACK_GET_APP_TIMER_ID(context, id)                                \
    (uint8_t)(((context)->port != 0u) ? ((uint8_t)(id) + 64u) : (uint8_t)(id))

/**
* \addtogroup group_pdstack_enums
* \{
*/
/**
 * @typedef cy_sw_timer_id_t
 * @brief List of soft timer IDs defined for various applications. Values of the timer
 * IDs should not be changed. Timer IDs from USER_TIMERS_START_ID can be used by user
 * code.
 */
typedef enum ccg_timer_id {
    PD_TIMERS_START_ID = 0u,
    /**< 000: Start index for USB-PD stack timers. */

    PD_CABLE_TIMER,
    /**< 001: Timer used for cable capability check. */

    PD_NO_RESPONSE_TIMER,
    /**< 002: Response timer. */

    PD_CBL_DSC_ID_TIMER,
    /**< 003: Timer used for cable discovery state machine. */

    PD_CBL_DELAY_TIMER,
    /**< 004: Timer used to enforce cable delay. */

    PD_PHY_BUSY_TIMER,
    /**< 005: Timer used to handle PHY busy status. */

    PD_GOOD_CRC_TX_TIMER,
    /**< 006: GoodCRC timer. */

    PD_HARD_RESET_TX_TIMER,
    /**< 007: Hard reset transmit timer. */

    PD_VCONN_SWAP_INITIATOR_TIMER,
    /**< 008: VConn swap initiator timer. */

    PD_GENERIC_TIMER,
    /**< 009: Generic AMS timer. */

    PD_PPS_TIMER,
    /**< 010: PPS related timer. */

    PD_SINK_TX_TIMER,
    /**< 011: PD 3.0 sink Rp flow control timer. */

    PD_DATA_RESET_COMP_TIMER,
    /**< 012: Reserved for future use. */

    PD_TIMER_RESERVED_13,
    /**< 013: Reserved for future use. */

    PD_TIMERS_END_ID,
    /**< 014: End index (inclusive) for USB-PD stack timers. */

    TYPEC_TIMERS_START_ID = 15u,
    /**< 015: Start index for Type-C timers. */

    TYPEC_GENERIC_TIMER2 = 15u,
    /**< 015: Generic Type-C state machine timer #2. */

    TYPEC_GENERIC_TIMER1,
    /**< 016: Generic Type-C state machine timer #1. */

    TYPEC_CC1_DEBOUNCE_TIMER,
    /**< 017: Timer used for CC1 debounce. */

    TYPEC_CC2_DEBOUNCE_TIMER,
    /**< 018: Timer used for CC2 debounce. */

    TYPEC_RD_DEBOUNCE_TIMER,
    /**< 019: Timer used for Rd debounce. */

    TYPEC_VBUS_DISCHARGE_TIMER,
    /**< 020: VBus discharge timer id. */

    TYPEC_ACTIVITY_TIMER,
    /**< 021: Type-C activity timer id. */

    TYPEC_RP_CHANGE_TIMER,
    /**< 022: Timer used to trigger current limit update after Rp change. */

    TYPEC_TIMER_RESERVED_23,
    /**< 023: Reserved for future use. */

    TYPEC_TIMERS_END_ID,
    /**< 024: End index (inclusive) for Type-C timers. */

    PD_OCP_DEBOUNCE_TIMER,
    /**< 025: Timer used for FW debounce of VBus OCP. */

    HPD_RX_ACTIVITY_TIMER_ID,
    /**< 026: Timer used for HPD receive handling. */

    PD_VCONN_OCP_DEBOUNCE_TIMER,
    /**< 027: Timer used for FW debounce of VConn OCP. */

    PD_TIMER_RESERVED_28,
    /**< 028: Reserved for future use. */

    PD_TIMER_RESERVED_29,
    /**< 029: Reserved for future use. */

    PD_TIMER_RESERVED_30,
    /**< 030: Reserved for future use. */

    PD_TIMER_RESERVED_31,
    /**< 031: Reserved for future use. */

    PD1_TIMERS_START_ID = 32u,
    /**< 032: Start index for USB-PD stack timers. */

    PD1_CABLE_TIMER,
    /**< 033: Timer used for cable capability check. */

    PD1_NO_RESPONSE_TIMER,
    /**< 034: Response timer. */

    PD1_CBL_DSC_ID_TIMER,
    /**< 035: Timer used for cable discovery state machine. */

    PD1_CBL_DELAY_TIMER,
    /**< 036: Timer used to enforce cable delay. */

    PD1_PHY_BUSY_TIMER,
    /**< 037: Timer used to handle PHY busy status. */

    PD1_GOOD_CRC_TX_TIMER,
    /**< 038: GoodCRC timer. */

    PD1_HARD_RESET_TX_TIMER,
    /**< 039: Hard reset transmit timer. */

    PD1_VCONN_SWAP_INITIATOR_TIMER,
    /**< 040: VConn swap initiator timer. */

    PD1_GENERIC_TIMER,
    /**< 041: Generic AMS timer. */

    PD1_PPS_TIMER,
    /**< 042: PPS related timer. */

    PD1_SINK_TX_TIMER,
    /**< 043: PD 3.0 sink Rp flow control timer. */

    PD1_DATA_RESET_COMP_TIMER,
    /**< 044: Reserved for future use. */

    PD1_TIMER_RESERVED_43,
    /**< 045: Reserved for future use. */

    PD1_TIMERS_END_ID,
    /**< 046: End index (inclusive) for USB-PD stack timers. */

    TYPEC1_TIMERS_START_ID = 47u,
    /**< 047: Start index for Type-C timers. */

    TYPEC1_GENERIC_TIMER2 = 47u,
    /**< 047: Generic Type-C state machine timer #2. */

    TYPEC1_GENERIC_TIMER1,
    /**< 048: Generic Type-C state machine timer #1. */

    TYPEC1_CC1_DEBOUNCE_TIMER,
    /**< 049: Timer used for CC1 debounce. */

    TYPEC1_CC2_DEBOUNCE_TIMER,
    /**< 050: Timer used for CC2 debounce. */

    TYPEC1_RD_DEBOUNCE_TIMER,
    /**< 051: Timer used for Rd debounce. */

    TYPEC1_VBUS_DISCHARGE_TIMER,
    /**< 052: VBus discharge timer id. */

    TYPEC1_ACTIVITY_TIMER,
    /**< 053: Type-C activity timer id. */

    TYPEC1_RP_CHANGE_TIMER,
    /**< 054: Timer used to trigger current limit update after Rp change. */

    TYPEC1_TIMER_RESERVED_53,
    /**< 055: Reserved for future use. */

    TYPEC1_TIMERS_END_ID,
    /**< 056: End index (inclusive) for Type-C timers. */

    PD1_OCP_DEBOUNCE_TIMER,
    /**< 057: Timer used for FW debounce of VBus OCP. */

    HPD1_RX_ACTIVITY_TIMER_ID,
    /**< 058: Timer used for HPD receive handling. */

    PD1_VCONN_OCP_DEBOUNCE_TIMER,
    /**< 059: Timer used for FW debounce of VConn OCP. */

    PD1_TIMER_RESERVED_60,
    /**< 060: Reserved for future use. */

    PD1_TIMER_RESERVED_61,
    /**< 061: Reserved for future use. */

    PD1_TIMER_RESERVED_62,
    /**< 062: Reserved for future use. */

    PD1_TIMER_RESERVED_63,
    /**< 063: Reserved for future use. */

    APP_TIMERS_START_ID = 64u,
    /**< 064: Start index for Application level timers. */

    APP_PSOURCE_EN_TIMER = 64u,
    /**< 064: Timer used to ensure timely completion of power source enable operation. */

    APP_PSOURCE_EN_MONITOR_TIMER,
    /**< 065: Timer used to monitor voltage during power source enable operation. */

    APP_PSOURCE_EN_HYS_TIMER,
    /**< 066: Timer used to add hysteresis at the end of a power source enable operation. */

    APP_PSOURCE_DIS_TIMER,
    /**< 067: Timer used to ensure timely completion of power source disable operation. */

    APP_PSOURCE_DIS_MONITOR_TIMER,
    /**< 068: Timer used to monitor voltage during power source disable operation. */

    APP_PSOURCE_CF_TIMER,
    /**< 069: Power source Current foldback restart timer ID. */

    APP_PSOURCE_DIS_EXT_DIS_TIMER,
    /**< 070: Timer used to discharge VBus for some extra time at the end of a power source disable operation. */

    APP_DB_SNK_FET_DIS_DELAY_TIMER,
    /**< 071: Dead battery Sink Fet disable delay timer. */

    APP_PSINK_DIS_TIMER,
    /**< 072: Timer used to ensure timely completion of power sink disable operation. */

    APP_PSINK_DIS_MONITOR_TIMER,
    /**< 073: Timer used to monitor voltage during power sink disable operation. */

    APP_VDM_BUSY_TIMER,
    /**< 074: Timer used to delay retry of VDMs due to BUSY responses or errors. */

    APP_AME_TIMEOUT_TIMER,
    /**< 075: Timer used to implement AME timeout. */

    APP_VBUS_OCP_OFF_TIMER,
    /**< 076: Timer used to disable VBus supply after OC fault. */

    APP_VBUS_OVP_OFF_TIMER,
    /**< 077: Timer used to disable VBus supply after OV fault. */

    APP_VBUS_UVP_OFF_TIMER,
    /**< 078: Timer used to disable VBus supply after UV fault. */

    APP_VBUS_SCP_OFF_TIMER,
    /**< 079: Timer used to disable VBus supply after SC fault. */

    APP_FAULT_RECOVERY_TIMER,
    /**< 080: App timer used to delay port enable after detecting a fault. */

    APP_SBU_DELAYED_CONNECT_TIMER,
    /**< 081: Timer used to do delayed SBU connection in Thunderbolt mode. */

    APP_MUX_DELAY_TIMER,
    /**< 082: Timer used to delay VDM response. */

    APP_MUX_POLL_TIMER,
    /**< 083: Timer used to MUX status. */

    APP_CBL_DISC_TRIGGER_TIMER,
    /**< 084: Timer used to trigger cable discovery after a V5V supply change. */

    APP_V5V_CHANGE_DEBOUNCE_TIMER,
    /**< 085: Timer used to debounce V5V voltage changes. */

    APP_VCONN_RECOVERY_TIMER,
    /**< 086: Timer used to run Vconn swap after V5V was lost and recovered while UFP. */

    APP_OT_DETECTION_TIMER,
    /**< 087: Timer used to call OT measurment handler. */

    APP_CHUNKED_MSG_RESP_TIMER,
    /**< 088: Timer ID used to respond to chunked messages with NOT_SUPPORTED. */

    APP_RESET_VDM_LAYER_TIMER,
    /**< 089: Timer used to run reset of VDM layer. */

    APP_BB_ON_TIMER,
    /**< 090: Timer used to provide delay between disabling the Billboard device and re-enabling it. */

    APP_BB_OFF_TIMER,
    /**< 091: Timer used to display USB billboard interface to save power. */

    APP_INITIATE_SWAP_TIMER,
    /**< 092: Timer used to initiate SWAP operations in DRP applications with a power/data role preference. */

    APP_VDM_NOT_SUPPORT_RESP_TIMER_ID,
    /**< 093: VDM Not supported response timer. */

    APP_BC_TIMERS_START_ID,
    /**< 094: Start of Battery Charging State Machine timers. */

    APP_BC_GENERIC_TIMER1,
    /**< 095: Generic timer #1 used by the BC state machine. */

    APP_BC_GENERIC_TIMER2,
    /**< 096: Generic timer #2 used by the BC state machine. */

    APP_BC_DP_DM_DEBOUNCE_TIMER,
    /**< 097: Timer used to debounce voltage changes on DP and DM pins. */

    APP_BC_DETACH_DETECT_TIMER,
    /**< 098: Timer used to detect detach of a BC 1.2 sink while functioning as a CDP. */

    APP_CDP_DP_DM_POLL_TIMER,
    /**< 099: Timer used to initiate DP/DM voltage polling while connected as a CDP. */

    APP_TIMER_RESERVED_100,
    /**< 100: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_101,
    /**< 101: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_102,
    /**< 102: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_103,
    /**< 103: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_104,
    /**< 104: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_105,
    /**< 105: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_106,
    /**< 106: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_107,
    /**< 107: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_108,
    /**< 108: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_109,
    /**< 109: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_110,
    /**< 110: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_111,
    /**< 111: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_112,
    /**< 112: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_113,
    /**< 113: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_114,
    /**< 114: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_115,
    /**< 115: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_116,
    /**< 116: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_117,
    /**< 117: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_118,
    /**< 118: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_119,
    /**< 119: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_120,
    /**< 120: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_121,
    /**< 121: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_122,
    /**< 122: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_123,
    /**< 123: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_124,
    /**< 124: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_125,
    /**< 125: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_126,
    /**< 126: Timer ID reserved for future use. */

    APP_TIMER_RESERVED_127,
    /**< 127: Timer ID reserved for future use. */

    APP_PORT1_TIMER_START_ID = 128u,
    /**< 128: Start of timer IDs reserved for the application layer management of PD port #1. */

    USER_TIMERS_START_ID = 192u,
    /**< 192: Start of timer IDs left for generic solution level usage. */

} cy_sw_timer_id_t;

/** \} group_pdstack_enums */

#endif /* _TIMER_ID_H_ */

/* [] END OF FILE */
