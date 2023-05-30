/***************************************************************************//**
* \file cy_pdstack_port_config.h
* \version 3.10
*
* PD port configuration structure header file for the PDStack middleware.
*
********************************************************************************
* \copyright
* Copyright 2021-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_PDSTACK_PORT_CONFIG_H_
#define _CY_PDSTACK_PORT_CONFIG_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*******************************************************************************
*                              Type definitions
*******************************************************************************/

/**
* \addtogroup group_pdstack_data_structures
* \{
*/

#if !CY_USE_CONFIG_TABLE
/**
 * @brief Structure to hold PDStack middleware configuration information.
 */
typedef struct
{
    /** Structure signature - 'PDSC' */
    uint32_t signature;

    /** Configuration parameter version. Consists of 8-bit major (bits 15:8) and
     * 8-bit minor (bits 7:0) version. */
    uint16_t version;

    /** Length of manufacturer information.
     * 0 - For no information.
     * 5 to 26: For valid information. */
    uint8_t mfgLenInfo;

    /** Reserved byte for alignment. */
    uint8_t reserved0;

    /** Manufacturer vendor ID. Range 0x0000 - 0xFFFF. */
    uint16_t mfgVid;

    /** Manufacturer product ID. Range 0x0000 - 0xFFFF. */
    uint16_t mfgPid;

    /** Manufacturer name. Null terminated string. */
    const uint8_t *mfgName;

    /** Extended source capability message support:
     * 1 - Enable support
     * 0 - Disable support */
    uint8_t scedbEn;

    /** Extended sink capability message support:
     * 1 - Enable support
     * 0 - Disable support */
    uint8_t skedbEn;

    /** Pointer to the extended source capability message byte array. */
    const uint8_t *extSrcCap;

    /** Size of extended source capability message in bytes. */
    uint8_t extSrcCapSize;
    
    /** Reserved for alignment. */
    uint8_t extSrcCapResvd[3];
    
    /** Pointer to the extended sink capability message byte array. */
    const uint8_t *extSnkCap;

    /** Size of extended sink capability message in bytes. */
    uint8_t extSnkCapSize;
    
    /** Reserved for alignment. */
    uint8_t extSnkCapResvd[3];

    /** PD port role:
     *  0 - Sink
     *  1 - Source
     *  2 - Dual-role */
    uint8_t portRole;

    /** Default port role in case of dual role ports:
     *  0 - Sink
     *  1 - Source */
    uint8_t defPortRole;

    /** Type-C current level (Rp value) used as a source:
     *  0 - 900 mA
     *  1 - 1.5 A
     *  2 - 3 A */
    uint8_t curLevel;

    /** Number of cable discovery attempts to be made.
     *  Range: 0x00 - 0x14 */
    uint8_t cableDiscCount;

    /** B29:B20 of the first 5 V fixed source PDO. */
    uint8_t srcPdoFlags[2];

    /** B29:B20 of the first 5 V fixed sink PDO. */
    uint8_t snkPdoFlags[2];

    /** Whether the Rp-Rd toggle is enabled in the unattached state. */
    uint8_t drpToggleEn;

    /** Bitmask that specifies supported Rp values:
     *  Bit 0 - 900 mA Rp
     *  Bit 1 - 1.5 A Rp
     *  Bit 2 - 3 A Rp */
    uint8_t rpSupported;

    /** Whether USB PD operation is supported on the port:
     *  1 - Enable PD operation
     *  0 - Disable PD operation */
    uint8_t pdOpEn;

    /** Whether Try.Src or Try.Sink is enabled for the port:
     *  0 - No Try.Src or Try.Sink supported
     *  1 - Try.Src supported
     *  2 - Try.Sink supported */
    uint8_t prefPwrRole;

    /** Whether the PD port is to be disabled at start-up. */
    uint8_t portDis;

    /** Whether cable discovery is enabled as part of the source state machine.
     *  1 - Enable cable discovery
     *  0 - Disable cable discovery */
    uint8_t cableDiscEn;

    /** Whether dead battery operation is supported:
     *  1 - Dead battery operation is supported
     *  0 - Dead battery operation is not supported */
    uint8_t deadBatSupp;

    /** Whether Type-C error recovery is enabled:
     *  1 - Enable error recovery
     *  0 - Disable error recovery */
    uint8_t errorRecoveryEn;

    /** Whether to enable/disable accessory mode:
     *  1 - Enable accessory mode
     *  0 - Disable accessory mode */
    uint8_t accessoryEn;

    /** Whether to enable/disable the disconnect detect mechanism using Rp in the sink
     * role:
     * 1 - Enable disconnect detect using Rp in sink role
     * 0 - Disable disconnect detect using Rp in sink role */
    uint8_t rpDetachEn;

    /** Whether Vconn supply should be left enabled even if the EMCA's cable
     * VDO indicates that Vconn is not required. */
    uint8_t vconnRetain;

    /** Fast role swap feature enabled flags:
     *  Bit 0 - FRS receive enable
     *  Bit 1 - FRS transmit enable */
    uint8_t frsConfig;

    /** Number of source PDOs supported (max. value is 7) */
    uint8_t srcPdoCount;

    /** Bitmask that enables specified entries in the Src. PDO list. */
    uint8_t defSrcPdoMask;

    /** Number of sink PDOs supported (max. value is 7) */
    uint8_t snkPdoCount;

    /** Bitmask that enables specified entries in the sink PDO list. */
    uint8_t defSnkPdoMask;

    /** Source PDO list.  */
    uint32_t srcPdo[7];

    /** Sink PDO list.  */
    uint32_t snkPdo[7];

    /** Sink PDO Min/Max current list. */
    uint16_t snkPdoMinMaxCur[7];

    /** Reserved for future use. */
    uint16_t reserved2;

    /** Get_Revision message response. */
    uint32_t pdRevision;

    /** Get_Source_Info message response. */
    uint32_t srcInfo;

    /** Number of EPR source PDOs supported. Non-zero value enables EPR source
     * operation. Range: 0 - 6 */
    uint8_t eprSrcPdoCount;

    /** Bit-mask enabling each EPR source PDO. */
    uint8_t eprSrcPdoMask;

    /** Number of EPR sink PDOs supported. Non-zero value enables EPR sink
     * operation. Range: 0 - 6 */
    uint8_t eprSnkPdoCount;

    /** Bit-mask enabling each EPR sink PDO. */
    uint8_t eprSnkPdoMask;

    /** EPR source PDO list. */
    uint32_t eprSrcPdo[6];

    /** EPR sink PDO list. */
    uint32_t eprSnkPdo[6];

} cy_stc_pdstack_port_cfg_t;
#endif /* !CY_USE_CONFIG_TABLE */

/** \} group_pdstack_data_structures */

#endif  /* _CY_PDSTACK_PORT_CONFIG_H_ */

/* [] END OF FILE */
