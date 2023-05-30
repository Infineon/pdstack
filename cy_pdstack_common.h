/***************************************************************************//**
* \file cy_pdstack_common.h
* \version 3.10
*
* Common header file of the PDStack middleware.
*
********************************************************************************
* \copyright
* Copyright 2021-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PDSTACK_COMMON_H)
#define CY_PDSTACK_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "cy_usbpd_defines.h"
#include "cy_usbpd_common.h"
#include "cy_pdutils_sw_timer.h"

#if CY_USE_CONFIG_TABLE
#include "cy_usbpd_config_table.h"
#else
#include "cy_pdstack_port_config.h"
#endif /* CY_USE_CONFIG_TABLE */

/**
********************************************************************************
* \mainpage PDStack middleware library
*
* The PDStack middleware implements state machines defined in the **Universal
* Serial Bus Type-C Cable and Connector Specification** and the **Universal
* Serial Bus Power Delivery Specification**. The middleware provides a set of
* Device Policy Manager (DPM) APIs through which the application can
* initialize, monitor, and configure the middleware operation.
*
* The PDStack middleware operates on top of the USB PD driver included in
* the MTB PDL CAT2 (mtb-pdl-cat2) peripheral driver library (PDL).
*
* <b>Features:</b>
* 1. Supports Type-C port configuration and connection detection in the Sink,
* source, and dual-role.
* 2. Supports Extended Power Range (EPR) in source and sink roles.
* 3. Support USB PD communication with the following features:
*    * USB PD revision 3.1 compliance
*    * USB PD revision 2.0 compliance (backward compatibility)
*    * Power contract negotiation in sink and source roles
*    * Ability to respond to SOP and SOP' messages to support designs with a
*    captive USB-C cable.
* 4. Supports configuration of the port roles and responses through the EZ-PD(TM)
* Configurator.
* 5. Support DPM APIs to initialize, configure, monitor, and
* control the port operation and send/receive USB PD messages
* associated with alternate mode discovery, authentication, firmware update,
* etc.
*
********************************************************************************
* \section section_pdstack_general_description General description
********************************************************************************
*
* Include cy_pdstack_common.h, cy_pdstack_dpm.h to
* get access to all functions and other declarations in this library. See the
* \ref section_pdstack_quick_start to start using the PDStack.
*
* See the \ref section_pdstack_toolchain section for compatibility
* information.
*
* See the \ref section_pdstack_changelog section for change history.
*
* PDStack operates on top of the USB PD driver. The USB PD driver has
* some prerequisites for proper operation.
* See the "USB PD (USB Power Delivery)" section of the MTB PDL CAT2 (mtb-pdl-cat2)
* peripheral driver library API reference manual.
* In addition, see the \ref section_pdstack_miscellaneous section for
* the different PDStack middleware restrictions and limitations.
*
* The PDStack middleware is released in the form of pre-compiled libraries. The
* following library variants with a different feature set to help limit the
* memory footprint of the stack in applications are released.
* 1. pmg1_pd3_snk_lite - Support USB Type-C sink operation and
* USB PD Revision 3.1 messaging.
* 2. pmg1_pd2_snk_lite - Supports USB Type-C sink operation and
* USB PD Revision 2.0 messaging. Using this library reduces the flash
* (code) memory usage by the application.
* 3. pmg1_pd3_drp - Supports USB Type-C dual-role operation and
* USB PD Revision 3.1 messaging. This library can be used in any Power Delivery
* application.
* 4. pmg1_pd3_snk_epr - Supports USB Type-C sink Extended Power
* Range (EPR). This library can be used in applications that require EPR.
* 5. pmg1_pd3_drp_epr - Supports USB Type-C dual role 
* EPR operation and USB PD Revision 3.1 messaging.
* 6. wlc1_pd3_snk - Library with support for USB Type-C sink operation and
* USB PD Revision 3.1 messaging. This library can be used in any Power Delivery
* WLC1 application.
********************************************************************************
* \section section_pdstack_quick_start Quick start guide
********************************************************************************
*
* PDStack middleware can be used in various development
* environments such as ModusToolbo(TM)), MBED, etc. See the
* \ref section_pdstack_toolchain section.
*
* These steps describe the simplest way of enabling the PDStack
* middleware in the application.
*
* 1. Open/create an application where to add the PDStack function.
*
* 2. Add the PDStack middleware to your project. This quick start guide
* assumes that the environment is configured to use the MTB CAT2 peripheral
* driver library (PDL) for development and the PDL is included in the project.
* If you are using the ModusToolbox(TM) development environment, select the
* application in the Project Explorer window and select the PDStack Middleware
* in the Library Manager.
*
* 3. Include cy_pdstack_common.h and cy_pdstack_dpm.h to get access to all
* functions and other declarations in this library.
*    \snippet pdstack/main.c snippet_configuration_include
*
* 4. Define the following data structures required by the PDStack middleware:
*    * USB-C port configuration
*    \snippet pdstack/main.c snippet_configuration_data
*    * Device Policy Manager parameters
*    \snippet pdstack/main.c snippet_configuration_dpm
*    * Register application callback functions
*    \snippet pdstack/main.c snippet_configuration_app_cbk
* The PDStack library uses this set of callbacks registered by the application
* to perform board-specific tasks such as turning the consumer power path
* ON/OFF, identifying the optimal source power profile to be used for charging,
* etc. The library also provides notification of various connections and PD
* policy state changes, therefore, the rest of the system can be configured as
* required.
*
* 5. Initialize the PDStack middleware after the start.
*    \snippet pdstack/main.c snippet_configuration_dpm_init
*
* 6. Start the DPM operation.
*    \snippet pdstack/main.c snippet_configuration_dpm_start
*    Initializes the USB PD block to detect Type-C connection state
*    changes and USB PD messages and notify the application through callback
*    functions.
*
* 7. Invoke the Cy_PdStack_Dpm_Task function from the main processing loop of the
* application to handle the device policy tasks for each PD Port.
*    \snippet pdstack/main.c snippet_configuration_dpm_task
*
********************************************************************************
* \section section_pdstack_configuration_considerations Configuration considerations
********************************************************************************
*
* This section consists of instructions on how to configure, and use
* the PDStack middleware in a design.
* 1. After selecting the PDStack middleware through the library manager, the
* user can select the library variant by editing the code example Makefile:
*    * Add 'PMG1_PD3_SNK_LITE' to the COMPONENTS for using the pmg1_pd3_snk_lite library
*    * Add 'PMG1_PD2_SNK_LITE' to the COMPONENTS for using the pmg1_pd2_snk_lite library
*    * Add 'PMG1_PD3_DRP' to the COMPONENTS for using the pmg1_pd3_drp library
*    * Add 'PMG1_PD3_SNK_EPR' to the COMPONENTS for using the pmg1_pd3_snk_epr library
*    * Add 'PMG1_PD3_DRP_EPR' to the COMPONENTS for using the pmg1_pd3_drp_epr library
*    * Add 'WLC1_PD3_SNK' to the COMPONENTS for using the wlc1_pd3_snk library
* 2. Configure the USB-C port properties
*    * The USB-C port properties are controlled with the
*    cy_stc_pdstack_port_cfg_t structure defined in config_table.h or cy_pdstack_port_config.h file. 
*    Please see its description to learn about the parameters and values.
*    * Use the EZ-PD(TM) Configurator to configure the properties of the USB-C port
*    including the port role and the default response to various USB PD messages.
*    See Step 4 of the \ref section_pdstack_quick_start guide for the
*    reference configuration data.
*    * For designs supporting EZ-PD(TM) Configuration Utility, configure the properties of the USB-C port
*    dynamically through the selection of appropriate values in the configuration utility tool.
* See the \ref section_pdstack_miscellaneous for the existing
* restrictions.
*
********************************************************************************
* \section section_pdstack_miscellaneous Limitations and restrictions
********************************************************************************
*
********************************************************************************
* \section section_pdstack_toolchain Supported software and tools
********************************************************************************
*
* This version of the PDStack middleware is validated for compatibility
* with the following software and tools:
*
* <table class="doxtable">
*   <tr>
*     <th>Software and tools</th>
*     <th>Version</th>
*   </tr>
*   <tr>
*     <td>ModusToolbox(TM) software</td>
*     <td>3.0</td>
*   </tr>
*   <tr>
*     <td>mtb-pdl-cat2</td>
*     <td>2.4.0</td>
*   </tr>
*   <tr>
*     <td>GCC compiler</td>
*     <td>10.3.1</td>
*   </tr>
*   <tr>
*     <td>IAR compiler</td>
*     <td>8.42.2</td>
*   </tr>
*   <tr>
*     <td>Arm(R) compiler 6</td>
*     <td>6.13</td>
*   </tr>
* </table>
*
********************************************************************************
* \section section_pdstack_changelog Changelog
********************************************************************************
*
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for change</th></tr>
*   <tr>
*     <td rowspan="3">3.10</td>
*     <td>Updated to USB PD Revision 3.1 Version 1.7.
*     </td>
*     <td>Feature addition.</td>
*   </tr>
*   <tr>
*     <td>Added EPR AVS feature support in the EPR sink library.
*     <td>Feature Addition.</td>
*   </tr>
*   <tr>
*     <td>Updated the handling of Get Source Info message for DRP.
*     <td>Defect fixes.</td>
*   </tr>
*   <tr>
*     <td rowspan="4">3.0</td>
*     <td>Added EZ-PD(TM) PMG1 DRP EPR library compliant to USB PD Revision 3.1.
*     </td>
*     <td>New Library</td>
*   </tr>
*   <tr>
*     <td>Moved software timer and utility functionality to separate PDUtils middleware library.
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td>Updated PDStack to use 27 ms and 29 ms for the tSenderResponse timer
*     while in PD 2.0 and PD3.x contracts respectively. Added API to update
*     the tSenderResponse value from the application space.
*     <td>PD specification updates.</td>
*   </tr>
*   <tr>
*     <td>EPR Keepalive process is incorrectly getting disabled during some pd transactions. This has been corrected.
*     <td>Defect fixes.</td>
*   </tr>
*   <tr>
*     <td>2.0</td>
*     <td>Added WLC1 sink library with support for USB Type-C sink operation and USB PD Revision 3.1 messaging.
* \note
*      This version of the PDStack middleware supports only the wlc1_pd3_snk
*      library and is intended to be used in any Power Delivery WLC1
*      application.
*     </td>
*     <td>WLC1 device support</td>
*   </tr>
*   <tr>
*   <td rowspan="3">1.20</td>
*     <td> Added libraries for USB Type-C dual role and sink Extended Power Range (EPR) operation </td>
*     <td> New feature addition </td>
*   </tr>
*   <tr>
*    <td> Updated the library to work with RTOS </td>
*    <td> New feature addition </td>
*   </tr>
*   <tr>
*     <td> Added APIs for changing source/sink pdo capabilities</td>
*     <td> Feature addition</td>.
*   </tr>
*   <tr>
*     <td rowspan="4">1.10</td>
*     <td>Updated Type-C error recovery period to 250 ms.</td>
*     <td>PD compliance specification updates.</td>
*   </tr>
*   <tr>
*     <td>Updated the size of the status extended message to 7 bytes.</td>
*     <td>PD compliance specification updates.</td>
*   </tr>
*   <tr>
*     <td>USB PD wakeup interrupt is incorrectly getting disabled as part of the software timer operation. This has been corrected.
*     <td>Defect fixes.</td>
*   </tr>
*   <tr>
*     <td>API description updated.</td>
*     <td>Documentation update and clarification.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
********************************************************************************
* \section section_pdstack_more_information More information
********************************************************************************
*
* For more information, see the following documents:
*
* * <a href=" https://www.infineon.com/modustoolbox">
*      <b>ModusToolbox(TM) software, quick start guide, documentation,
*         and videos</b>
*   </a>
*
* * <a href="https://infineon.github.io/mtb-pdl-cat2/pdl_api_reference_manual/html/index.html">
*   <b>PDL API reference</b></a>
*
* \note
* The links to the other software component's documentation (middleware and PDL)
* point to GitHub for the latest available version of the software.
* To get documentation of the specified version, download from GitHub, and unzip
* the component archive. The documentation is available in
* the <i>docs</i> folder.
*
********************************************************************************
*
* \defgroup group_pdstack_macros Macros
* \brief
* Describes the PDStack macros.
*
* \defgroup group_pdstack_functions Functions
* \brief
*  Describes the PDStack Function prototypes.
*
* \defgroup group_pdstack_data_structures Data structures
* \brief
* Describes the data structures defined by the PDStack.
*
* \defgroup group_pdstack_enums Enumerated types
* \brief
* Describes the enumeration types defined by the PDStack.
*
*/

/*******************************************************************************
*                              Type definitions
*******************************************************************************/

/* Provides default values for feature selection macros where not already defined. */
#ifndef DPM_DEBUG_SUPPORT
#define DPM_DEBUG_SUPPORT                (0u)
#endif /* DPM_DEBUG_SUPPORT */

#ifndef CY_PD_TRY_SRC_SNK_DISABLE
#define CY_PD_TRY_SRC_SNK_DISABLE        (1u)
#endif /* CY_PD_TRY_SRC_SNK_DISABLE */

#ifndef CY_PD_PPS_SRC_ENABLE
#define CY_PD_PPS_SRC_ENABLE             (0u)
#endif /* CY_PD_PPS_SRC_ENABLE */

#ifndef CY_PD_CBL_DISC_DISABLE
#define CY_PD_CBL_DISC_DISABLE           (1u)
#endif /* CY_PD_CBL_DISC_DISABLE */

#ifndef CY_PD_VCONN_DISABLE
#define CY_PD_VCONN_DISABLE              (1u)
#endif /* CY_PD_VCONN_DISABLE */

#ifndef CY_PD_BCR_EN
#define CY_PD_BCR_EN                     (0u)
#endif /* CY_PD_BCR_EN */

#ifndef CY_PD_BIST_STM_ENABLE
#define CY_PD_BIST_STM_ENABLE            (0u)
#endif /* CY_PD_BIST_STM_ENABLE */

#ifndef CY_PD_CRC_ERR_HANDLING_ENABLE
#define CY_PD_CRC_ERR_HANDLING_ENABLE    (1u)
#endif /* CY_PD_CRC_ERR_HANDLING_ENABLE */

#ifndef CY_PD_EPR_AVS_ENABLE
#define CY_PD_EPR_AVS_ENABLE             (0u)
#endif /* CY_PD_EPR_AVS_ENABLE */

/**
* \addtogroup group_pdstack_macros
* \{
*/

/** PDStack middleware major version */
#define CY_PDSTACK_MW_VERSION_MAJOR               (3)

/** PDStack middleware minor version */
#define CY_PDSTACK_MW_VERSION_MINOR               (10)

/** USB Type-C specification version 2.0 */
#define CY_PD_TYPE_C_2_0_REVISION                 (0x0200u)

/** Extended status present input offset */
#define CY_PD_EXTD_STATUS_PRESENT_INPUT_OFFSET    (1u)

/** Number of unacknowledged source CAP messages used to determine if the device connected is PD capable or not. */
#define CY_PD_MAX_SRC_CAP_TRY                     (6u)

/** Masks for the give-back supported bit in the snkPdoMinMaxCur field of cy_stc_pdstack_port_cfg_t. */
#define CY_PD_GIVE_BACK_MASK                      (0x8000u)

/** Masks to extract the actual min/max current value from the snkPdoMinMaxCur field of cy_stc_pdstack_port_cfg_t. */
#define CY_PD_SNK_MIN_MAX_MASK                    (0x3FFu)

/** Maximum retries of source capability messages. */
#define CY_PD_MAX_SRC_CAP_COUNT                   (50u)

/** Maximum hard reset retry count. */
#define CY_PD_MAX_HARD_RESET_COUNT                (3u)

/** Maximum number of cable discovery DISCOVER_IDENTITY messages that should be sent out. */
#define CY_PD_MAX_CBL_DSC_ID_COUNT                (20u)

/** Number of PR_SWAP messages for which DUT sends a WAIT response to allow VConn_SWAP completion. */
#define CY_PD_MAX_PR_SWAP_WAIT_COUNT              (2u)

/** Maximum number of SPR DOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_SPR_DO                    (7u)

/** Maximum number of DOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_DO                        (7u)

/** Maximum number of PDOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_PDO                       (CY_PD_MAX_NO_OF_DO)

/** Maximum number of SPR PDOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_SPR_PDO                   (CY_PD_MAX_NO_OF_SPR_DO)

/** Maximum number of EPR PDOs in a packet. Limited by PD message definition. */    
#define CY_PD_MAX_NO_OF_EPR_PDO                   (6u)

/** Maximum number of VDOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_VDO                       (CY_PD_MAX_NO_OF_DO)

/** Index of VDM header data object in a received message. */
#define CY_PD_VDM_HEADER_IDX                      (0u)

/** Index of BIST header data object in a received message. */
#define CY_PD_BDO_HDR_IDX                         (0u)

/** Index of ID_HEADER data object in a received VDM. */
#define CY_PD_ID_HEADER_IDX                       (1u)

/** Index of CERT_STAT data object in a received VDM. */
#define CY_PD_CERT_STAT_IDX                       (2u)

/** Index of PRODUCT VDO in a received VDM. */
#define CY_PD_PRODUCT_VDO_IDX                     (3u)

/** Index of the first product type VDO in a VDM. */
#define CY_PD_PRODUCT_TYPE_VDO_1_IDX              (4u)

/** Index of the second product type VDO in a VDM. */
#define CY_PD_PRODUCT_TYPE_VDO_2_IDX              (5u)

/** Index of the third product type VDO in a VDM. */
#define CY_PD_PRODUCT_TYPE_VDO_3_IDX              (6u)

/** Index of request data objects in a received message. */
#define CY_PD_RDO_IDX                             (0u)

/** Maximum extended message size in bytes. */
#define CY_PD_MAX_EXTD_PKT_SIZE                   (260u)

/** Maximum extended message 32-bit words. Each word is 32 bit. */
#define CY_PDSTACK_MAX_EXTD_PKT_WORDS             (65u)

/** Maximum legacy extended message size in bytes. */
#define CY_PD_MAX_EXTD_MSG_LEGACY_LEN             (26u)

/** Maximum message ID value in PD Header. */
#define CY_PD_MAX_MESSAGE_ID                      (7u)

/** Max SOP types excluding hard reset, cable reset, SOP_PDEBUG, and SOP_DPDEBUG. */
#define CY_PD_MAX_SOP_TYPES                       (3U)

/** Standard SVID defined by USB PD specification. */
#define CY_PD_STD_SVID                            (0xFF00UL)

/** DisplayPort SVID defined by VESA specification. */
#define CY_PD_DP_SVID                             (0xFF01UL)

/** Thunderbolt SVID defined by Intel specification. */
#define CY_PD_TBT_SVID                            (0x8087UL)

/** Apple SVID defined by Apple specification. */
#define CY_PD_APPLE_SVID                          (0x05ACUL)

/** UFP supports non-physical alternate mode UFP VDO1 mask. */
#define CY_PD_UFP_NON_PH_ALT_MODE_SUPP_MASK       (0x4u)

/** Infineon VID defined by Infineon for field upgrades. */
#define CY_PD_CY_VID                              (0x04B4UL)

/** Position of VDM version field in structured VDM header. */
#define CY_PD_STD_VDM_VERSION_IDX                 (13u)

/** Position of VDM version field in structured VDM header. */
#define CY_PD_STD_VDM_MINOR_VERSION_IDX           (11u)

/** VDM version 2.0. Used under USB PD Revision 3.0. */
#define CY_PD_STD_VDM_VERSION_REV3                (1u)

/** VDM version 1.0. Used under USB PD Revision 2.0. */
#define CY_PD_STD_VDM_VERSION_REV2                (0u)

/** Default VDM version used. Corresponds to VDM version 1.0. */
#define CY_PD_STD_VDM_VERSION                     (0u)

/** Maximum voltage allowed when PS_RDY is received during a SNK to SRC PR_SWAP. This is set to 3.0 V. */
#define CY_PD_VSAFE_0V_PR_SWAP_SNK_SRC            (3000u)

/** Maximum voltage allowed at the end of a hard reset when EZ-PD(TM) PMG1 is SNK. This is set to 3.0 V. */
#define CY_PD_VSAFE_0V_HARD_RESET                 (3000u)

/** Voltage unit used in PDOs. */
#define CY_PD_VOLT_PER_UNIT                    (50u)

/** Voltage unit (mV) used in PPS PDOs. */
#define CY_PD_VOLT_PER_UNIT_PPS                (100u)

/** Multiplier used to convert from the current unit used in other PDOs to that used in PPS PDO/RDO. */
#define CY_PD_CURRENT_PPS_MULTIPLIER              (5u)

/** Multiplier used to convert from the current unit used in other PDOs to that used in AVS PDO/RDO. */
#define CY_PD_CURRENT_AVS_MULTIPLIER              (4u)

/** VBus current usage = 0 A. */
#define CY_PD_ISAFE_0A                            (0u)

/** VBus current usage = 0.5 A. */
#define CY_PD_ISAFE_DEF                           (50u)

/** VBus current usage = 0.9 A. */
#define CY_PD_I_0P9A                              (90u)

/** VBus current usage = 1.0 A. */
#define CY_PD_I_1A                                (100u)

/** VBus current usage = 1.5 A. */
#define CY_PD_I_1P5A                              (150u)

/** VBus current usage = 2.0 A. */
#define CY_PD_I_2A                                (200u)

/** VBus current usage = 3.0 A. */
#define CY_PD_I_3A                                (300u)

/** VBus current usage = 4.0 A. */
#define CY_PD_I_4A                                (400u)

/** VBus current usage = 5.0 A. */
#define CY_PD_I_5A                                (500u)

/** Current unit used in PDOs. */
#define CY_PD_CUR_PER_UNIT                     (10u)

/** Number of VBus checks used to detect detach as the sink. */
#define CY_PD_SNK_DETACH_VBUS_POLL_COUNT          (5u)

/** Minimum DRP toggling period, in ms. See Table 4-16 of the Type-C spec Rev1. */
#define CY_PD_DRP_TOGGLE_PERIOD                   (75u)

/** Minimum percentage of DRP period for a source. See Table 4-16 of the Type-C spec Rev1. */
#define CY_PD_SRC_DRP_MIN_DC                      (30u)

/** Type-C state machine inactive mode. */
#define CY_PD_TYPEC_FSM_NONE                      (0x00000000u)

/** Type-C state machine active mode. */
#define CY_PD_TYPEC_FSM_GENERIC                   (0x00000001u)

/** Minimum HPD receiver timer period in ms. */
#define CY_PD_HPD_RX_ACTIVITY_TIMER_PERIOD_MIN    (5u)

/** Maximum HPD receiver timer period in ms. */
#define CY_PD_HPD_RX_ACTIVITY_TIMER_PERIOD_MAX    (105u)

/** PD No response timer period in ms. See Section 6.5.7 of USB PD spec Rev2 v1.2 */
#define CY_PD_NO_RESPONSE_TIMER_PERIOD         (5000u)

/** tVConnStable delay required by cable marker to power up. */
#define CY_PD_CBL_POWER_UP_TIMER_PERIOD        (55u)

/** Cable discovery timer period in ms. See Section 6.5.15 of USB PD spec Rev2 v1.2 */
#define CY_PD_CBL_DSC_ID_TIMER_PERIOD          (49u)

/** Cable discovery start period in ms. See Section 6.5.15 of USB PD spec Rev2 v1.2 */
#define CY_PD_CBL_DSC_ID_START_TIMER_PERIOD    (43u)

/** Cable delay timer period in ms. See Section 6.5.14 of USB PD spec Rev2 v1.2 */
#define CY_PD_CBL_DELAY_TIMER_PERIOD           (2u)

/** Period of the timer used internally by stack to prevent PHY lockup in TX state. */
#define CY_PD_PHY_BUSY_TIMER_PERIOD            (15u)

/** Hard reset transmit timer period in ms. See Section 6.3.13 of USB PD spec Rev2 v1.2 */
#define CY_PD_HARD_RESET_TX_TIMER_PERIOD       (20u)

/** This timer is used by stack to do auto retry VCONN swap before PR swap (if DUT is a sink).
 * Minimum gap between VCONN swap requests shall be a minimum of 10 ms, to be safe 110 ms
 * is used.
 */
#define CY_PD_VCONN_SWAP_INITIATOR_TIMER_PERIOD (110u)

/** Delay between VConn swap attempts when 5 V supply source is not present. */
#define CY_PD_VCONN_SWAP_INITIATOR_DELAY_PERIOD (500u)

/** VBus ON timer period in ms. See Table 7-22 of USB PD spec Rev2 v1.2. */
#define CY_PD_VBUS_TURN_ON_TIMER_PERIOD        (275u)

/** VBus ON timer period in ms for EPR mode. */
#define CY_PD_EPR_VBUS_TURN_ON_TIMER_PERIOD    (700u)

/** VBus OFF timer period in ms. See Table 7-22 of USB PD spec Rev2 v1.2. */
#define CY_PD_VBUS_TURN_OFF_TIMER_PERIOD       (625u)

/** Source transition timer period in ms. See Section 6.5.6.1 of USB PD spec Rev2 v1.2. */
#define CY_PD_PS_SRC_TRANS_TIMER_PERIOD        (400u)

/** Src.Trans timer period in ms for EPR fixed. */
#define CY_PD_PS_EPR_FIXED_SRC_TRANS_TIMER_PERIOD            (860u)

/** Src.Trans timer period in ms for EPR AVS Large */
#define CY_PD_PS_EPR_AVS_LARGE_SRC_TRANS_TIMER_PERIOD        (700u)

/** Src.Trans timer period in ms for EPR AVS small. */
#define CY_PD_PS_EPR_AVS_SMALL_SRC_TRANS_TIMER_PERIOD        (50u)

/** Source off-timer period in ms. See Section 6.5.6.2 of USB PD spec Rev2 v1.2. */
#define CY_PD_PS_SRC_OFF_TIMER_PERIOD          (900u)

/** Source on timer period in ms. See Section 6.5.6.3 of USB PD spec Rev2 v1.2. */
#define CY_PD_PS_SRC_ON_TIMER_PERIOD           (450u)

/** Sink transition period in ms. See Section 6.5.6.1 of USB PD spec Rev2 v1.2 */
#define CY_PD_PS_SNK_TRANSITION_TIMER_PERIOD   (500u)

/** Src.Recover timer period in ms for EPR mode. */
#define CY_PD_EPR_SRC_RECOVER_TIMER_PERIOD     (1250u)

/** EPR Snk. transition period in ms. */
#define CY_PD_PS_SNK_EPR_TRANSITION_TIMER_PERIOD   (925u)

/** Time (in ms) allowed for source voltage to become valid.
 * tAvsSrcTransSmall  */
#define CY_PD_PSOURCE_AVS_TRANS_SMALL_PERIOD      (50u)

/** Time (in ms) allowed for source voltage to become valid.
 * tAvsSrcTransSmall  */
#define CY_PD_PSOURCE_AVS_TRANS_LARGE_PERIOD      (700u)

/** Source Recover timer period in ms. See Section 7.6.1 of USB PD spec Rev2 v1.2. */
#define CY_PD_SRC_RECOVER_TIMER_PERIOD         (800u)

/** Src.Recover timer period in ms for EPR mode. */
#define CY_PD_EPR_SRC_RECOVER_TIMER_PERIOD     (1250u)

/** Sender response timeout period in ms for PD3. See Section 6.6.2 of USB PD spec Rev3 v1.6. */
#define CY_PD_3_SENDER_RESPONSE_TIMER_PERIOD     (30u)

/** Sender response timeout period in ms for PD2. See Section 6.6.2 of USB PD spec Rev2 v1.2. */
#define CY_PD_2_SENDER_RESPONSE_TIMER_PERIOD     (27u)

/** Receiver response timeout period in ms. See Section 6.5.2 of USB PD spec Rev2 v1.2. */
#define CY_PD_RECEIVER_RESPONSE_TIMER_PERIOD   (15u)

/** Sink wait cap period in ms. See Section 6.5.4.2 of USB PD spec Rev2 v1.2. */
#define CY_PD_SINK_WAIT_CAP_TIMER_PERIOD       (400u)

/** Source cap timer period in ms. See Section 6.5.4.1 of USB PD spec Rev2 v1.2. */
#define CY_PD_SRC_CAP_TIMER_PERIOD             (180u)

/** Source start (during PR_SWAP) period in ms. See Section 6.5.9.2 of USB PD spec Rev2 v1.2. */
#define CY_PD_SWAP_SRC_START_TIMER_PERIOD      (55u)

/** Source voltage transition timer period in ms. See Table 7-22 of USB PD spec Rev2 v1.2. */
#define CY_PD_SOURCE_TRANSITION_TIMER_PERIOD   (28u)

/** VConn off-timer period in ms. See Section 6.5.13 of USB PD spec Rev2 v1.2. */
#define CY_PD_VCONN_OFF_TIMER_PERIOD           (25u)

/** VConn on timer period in ms. */
#define CY_PD_VCONN_ON_TIMER_PERIOD            (100u)

/** Duration for which the UFP discharges VConn during Data_Reset sequence. */
#define CY_PD_UFP_VCONN_DISCHARGE_DURATION     (10u)

/** VConnSourceDischarge timer period. */
#define CY_PD_VCONN_SRC_DISC_TIMER_PERIOD      (200u)

/** tVConnReapplied period from USB PD specification. */
#define CY_PD_VCONN_REAPPLIED_TIMER_PERIOD     (18u)

/** Time between Data_Reset is accepted and Data_Reset_Complete message has to be sent. */
#define CY_PD_DATA_RESET_TIMER_PERIOD          (220u)

/** Data_Reset_Complete timeout period. */
#define CY_PD_DATA_RESET_TIMEOUT_PERIOD        (250u)

/** Delay applied before DFP sends Data_Reset_Complete message. */
#define CY_PD_DATA_RESET_COMPLETION_DELAY      (225u)

/** UFP Data_Reset complete timeout period. */
#define CY_PD_UFP_DATA_RESET_FAIL_TIMER_PERIOD (500u)

/** Period of VConn monitoring checks done internally. */
#define CY_PD_VCONN_TURN_ON_TIMER_PERIOD       (10u)

/** This timer is the delay between PD startup and sending a cable to discover the ID request
 * to ensure the cable is ready to respond. */
#define CY_PD_CBL_READY_TIMER_PERIOD           (50u)

/** VDM response timer period in ms. See Section 6.5.12.1 of USB PD spec Rev2 v1.2. */
#define CY_PD_VDM_RESPONSE_TIMER_PERIOD        (27u)

/** Enter mode response timeout period in ms. See Section 6.5.12.2 of USB PD Spec Rev2 v1.2. */
#define CY_PD_VDM_ENTER_MODE_RESPONSE_TIMER_PERIOD (45u)

/** Exit mode response timeout period in ms. See Section 6.5.12.3 of USB PD spec Rev2 v1.2. */
#define CY_PD_VDM_EXIT_MODE_RESPONSE_TIMER_PERIOD  (45u)

/** VDM receiver response timer period in ms. See Section 6.5.12.1 of USB PD spec Rev2 v1.2.
 * This timer is slightly relaxed from the spec value.
 */
#define CY_PD_DPM_RESP_REC_RESP_PERIOD         (20u)

/** BIST continuous mode period in ms. See Section 6.5.8.4 of USB PD spec Rev2 v1.2. */
#define CY_PD_BIST_CONT_MODE_TIMER_PERIOD      (55u)

/** Time in ms allowed for VBus to turn OFF during hard reset. */
#define CY_PD_SINK_VBUS_TURN_OFF_TIMER_PERIOD  (750u)

/** Time in ms allowed for VBus to turn on during hard reset. */
#define CY_PD_SINK_CY_PD_VBUS_TURN_ON_TIMER_PERIOD   (1300u)

/** Hard reset timer period in ms. See Section 6.5.11.2 of USB PD spec Rev2 v1.2.
 * */
#define CY_PD_PS_HARD_RESET_TIMER_PERIOD       (27u)

/** Time for which PD 3.0 source will keep Rp as SinkTxNG after returning to
 * Ready state. */
#define CY_PD_COLLISION_SRC_COOL_OFF_TIMER_PERIOD         (5u)

/** Delay between AMS initiation attempts by PD 3.0 sink while Rp = SinkTxNG. */
#define CY_PD_SINK_TX_TIMER_PERIOD                          (18u)

/** PPS timer period in ms. */
#define CY_PD_PPS_SRC_TIMER_PERIOD                          (14000u)

/** CC debounce period in ms from Type-C spec. */
#define CY_PD_TYPEC_CC_DEBOUNCE_TIMER_PERIOD                (140u)

/** PD debounce period in ms. */
#define CY_PD_TYPEC_PD_DEBOUNCE_TIMER_PERIOD                (11u)

/** Rd debounce period (detach detection) in ms. */
#define CY_PD_TYPEC_RD_DEBOUNCE_TIMER_PERIOD                (12u)

/** Delay between Attached.Wait state entry and start of checks for detached
 * status. */
#define CY_PD_TYPEC_ATTACH_WAIT_ENTRY_DELAY_PERIOD          (10u)

/** Debounce period used to detect detach when it is the source. */
#define CY_PD_TYPEC_SRC_DETACH_DEBOUNCE_PERIOD              (2u)

/** Period in ms used to detect Rp change in a PD 3.0 contract. */
#define CY_PD_TYPEC_PD3_RPCHANGE_DEBOUNCE_PERIOD            (2u)

/** Type-C error recovery timer period in ms. */
#define CY_PD_TYPEC_ERROR_RECOVERY_TIMER_PERIOD             (250u)

/** Type-C try DRP timer period in ms. */
#define CY_PD_TYPEC_DRP_TRY_TIMER_PERIOD                    (110u)

/** Type-C try timeout timer period in ms. */
#define CY_PD_TYPEC_TRY_TIMEOUT_PERIOD                      (800u)

/** Period in ms to check whether the solution state allows to move forward
 * with Type-C connection. */
#define CY_PD_SLN_STATUS_CHECK_PERIOD                       (10u)

/** EPR mode entry timeout in ms */
#define CY_PD_EPR_MODE_ENTER_TIMEOUT_PERIOD                 (500u)

/** EPR mode exit timeout in ms */
#define CY_PD_EPR_MODE_EXIT_TIMEOUT_PERIOD                  (500u)

/** EPR sink Keepalive timer period in ms */     
#define CY_PD_EPR_SNK_KEEPALIVE_TIMER_PERIOD                (375u)

/** EPR source Keepalive timer period in ms */ 
#define CY_PD_EPR_SRC_KEEPALIVE_TIMER_PERIOD                (900u)   

/** Chunk sender request timeout period in ms. */
#define CY_PD_CHUNK_SENDER_REQUEST_TIMER_PERIOD             (27u)

/** Chunk sender response timeout period in ms. */
#define CY_PD_CHUNK_SENDER_RESPONSE_TIMER_PERIOD            (27u)

/** Chunk received request timeout period in ms. */
#define CY_PD_CHUNK_RECEIVER_REQUEST_TIMER_PERIOD           (15u)

/** Chunk receiver response timeout period in ms. */
#define CY_PD_CHUNK_RECEIVER_RESPONSE_TIMER_PERIOD          (15u)

/** FRS transmit enable flag in the config table setting. */
#define CY_PD_FRS_TX_ENABLE_MASK                            (0x02u)

/** FRS receive enable flag in the config table setting. */
#define CY_PD_FRS_RX_ENABLE_MASK                            (0x01u)

/** Size of extended source capabilities message in bytes. */
#define CY_PD_EXT_SRCCAP_SIZE                               (25u)
/** Size of extended source capabilities message buffer. */
#define CY_PD_EXT_SRCCAP_BUF_SIZE                           (28u)

/** Index of source inputs field in extended source caps. */
#define CY_PD_EXT_SRCCAP_INP_INDEX                          (21u)

/** Mask for unconstrained source input in extended source caps. */
#define CY_PD_EXT_SRCCAP_INP_UNCONSTRAINED                  (0x02u)

/** Index of PDP field in extended source caps. */
#define CY_PD_EXT_SRCCAP_PDP_INDEX                          (23u)

/** Index of SPR PDP field in extended source caps. */
#define CY_PD_EXT_SPR_SRCCAP_PDP_INDEX                      (23u)

/** Index of EPR PDP field in extended source caps. */
#define CY_PD_EXT_EPR_SRCCAP_PDP_INDEX                      (24u)

/** Size of extended sink cap message in bytes. */
#define CY_PD_EXT_SNKCAP_SIZE                               (24u)

/** Size of the buffer allocated for extended sink cap data. */
#define CY_PD_EXT_SNKCAP_BUF_SIZE                           (28u)

/** Offset of SKEDB version field in Ext. Snk Cap. This field must be non-zero for a valid response. */
#define CY_PD_EXT_SNKCAP_VERS_INDEX                         (10u)

/** Size of status extended message in bytes. */
#define CY_PD_EXT_STATUS_SIZE                               (7u)

/** Size of PPS status extended message in bytes. */
#define CY_PD_EXT_PPS_STATUS_SIZE                           (4u)

/**  Size of EPR Avs small step*/
#define CY_PD_EPR_AVS_SMALL_STEP_VOLTAGE                    (1000u)

/** Index of EPR PDP field in extended sink caps. */    
#define CY_PD_EXT_SNKCAP_EPRPDP_INDEX                       (22u)

/** Externally powered bit position in source PDO mask. */
#define CY_PD_EXTERNALLY_POWERED_BIT_POS                    (7u)

/** Mask to be applied on fixed supply source PDO for PD Rev 2.0 */
#define CY_PD_FIX_SRC_PDO_MASK_REV2                (0xFE3FFFFFu)

/** Mask to be applied on fixed supply source PDO for PD Rev 3.0 */
#define CY_PD_FIX_SRC_PDO_MASK_REV3                (0xFF3FFFFFu)

/** Status field indicating that contract negotiation is in progress. */
#define CY_PD_FLAG_CONTRACT_NEG_ACTIVE             (1u)

/** Status field indicating that explicit contract is present. */
#define CY_PD_FLAG_EXPLICIT_CONTRACT               (2u)

/** Status field indicating that source is ready. */
#define CY_PD_FLAG_SRC_READY                       (4u)

/** Status field indicating that the port is currently a sink. */
#define CY_PD_FLAG_POWER_SINK                      (8u)

/** CC line status: ZOpen. */
#define CY_PD_CC_STAT_ZOPEN                           (0u)

/** CC line status: DRP toggle in progress. */
#define CY_PD_CC_STAT_DRP_TOGGLE                      (1u)

/** CC line status: Rd is applied. */
#define CY_PD_CC_STAT_RD_PRESENT                      (2u)

/** CC line status: Rp is applied. */
#define CY_PD_CC_STAT_RP_PRESENT                      (4u)

/** CC line status: VConn is applied. */
#define CY_PD_CC_STAT_VCONN_ACTIVE                    (8u)

/** No additional DPM error information is available. */
#define CY_PD_DPM_ERROR_NONE                          (0u)

/** DPM command failed because of the absence of VConn. */
#define CY_PD_DPM_ERROR_NO_VCONN                      (1u)

/** Enter USB DO USB4 mode mask. */
#define CY_PD_USB4_EUDO_USB4_EN_MASK                  (0x26000000u)

/** USB4 data mode as defined in entering USB DO. */
#define CY_PD_USB_MODE_USB4                           (2u)

/** USB 3.2 data mode as defined in entering USB DO. */
#define CY_PD_USB_MODE_USB3                           (1u)

/** USB 2.0 data mode as defined in entering USB DO. */
#define CY_PD_USB_MODE_USB2                           (0u)

/** TBT Gen 3 cable identifier in the TBT cable disc mode VDO response. */
#define CY_PD_TBT_GEN_3                               (3u)

/** TBT Gen 3 cable identifier in the TBT cable disc mode VDO response. */
#define CY_PD_UFP_VDO_1_RECFG_ALT_MODE_PARAM_MASK     (0x20u)

/* Macros used internally in the PD stack. Exclude from API guide. */

/** @cond DOXYGEN_HIDE */
#define CY_PD_LENGTH_CHECK_SKIP                (0xFFFFU)    /* Skips length check in pd_is_msg function. */
#define CY_PD_CBL_VDO_INDEX                    (4U)        /* Index of cable VDO in received VDM. */
#define CY_PD_CBL_VDO2_INDEX                   (5U)        /* Index of active cable VDO 2 in received VDM. */
#define CY_PD_CBL_VDO_COUNT                    (5U)        /* Number of expected data objects in D_ID response VDM. */

#define CY_PD_MAX_PD_PKT_WORDS                 (8U)        /* Maximum PD words in one packet. */
#define CY_PD_WORD_SIZE                        (4U)        /* Size of each PD word in bytes. */
#define CY_PD_MAX_PD_PKT_BYTES                 (CY_PD_MAX_PD_PKT_WORDS * CY_PD_WORD_SIZE)

/* Create PD 2.0 header given message type, ID, and count. */
#define CY_PD_HEADER(type, id, cnt)            \
    (((uint32_t)type) | (CY_PD_REV_V2_0 << 6) | (((uint32_t)id) << 9) | (((uint32_t)cnt) << 12))

/* Create PD 3.0 header given message type, ID, count, and extended message indication. */
#define CY_PD_HEADER_REV3(type, id, cnt, ext)  \
    (((uint32_t)type) | (((uint32_t)id) << 9) | (((uint32_t)cnt) << 12) | (((uint32_t)ext) << 15))

/* Return message count from the header. */
#define CY_PD_GET_PD_HDR_CNT(header)              ((((uint32_t)header) >> 12) & 0x7u)

/* Return the PR role bit from the header. */
#define CY_PD_GET_PD_HDR_PR_ROLE(header)          ((((uint32_t)header) >> 8) & 0x1u)

/* Return cable plug bit from the header. */
#define CY_PD_GET_PD_HDR_CBL_PLUG(header)         ((((uint32_t)header) >> 8) & 0x1u)

/* Return spec revision from the header. */
#define CY_PD_GET_PD_HDR_SPEC_REV(header)         ((((uint32_t)header) >> 6) & 0x3u)

/* Return the DR role bit from the header. */
#define CY_PD_GET_PD_HDR_DR_ROLE(header)          ((((uint32_t)header) >> 5) & 0x1u)

/* Return message ID from the header. */
#define CY_PD_GET_PD_HDR_ID(header)               ((((uint32_t)header) >> 9) & 0x7u)

/* Return message type from the header. */
#define CY_PD_GET_PD_HDR_TYPE(header)             (((uint32_t)header) & 0xFu)

/* PD header mask. */
#define CY_PD_HDR_MASK                            (0x0000FFFFu)

/* Message ID mask. */
#define CY_PD_MSG_ID_MASK                         ((0x7UL) << 9u)

/* Return complete PD header Rx buffer[0]. */
#define CY_PD_GET_PD_HDR(buf0)                    ((buf0) & CY_PD_HDR_MASK)

/* Update sender response timer period based on PD revision on the bus. */
#define CY_PDSTACK_UPDATE_SENDER_RESPONSE_TIMER(X,Y)       (((Y) == CY_PD_REV2) ? ((X)->senderRspTimeout = (X)->ptrPdTimerParams->pd2senderRspTimeout) : ((X)->senderRspTimeout = (X)->ptrPdTimerParams->pd3senderRspTimeout))


#define CY_PD_GET_RDO_OBJ_POS(rdo)                (((rdo) >> 28) & 0x7)       /* Gets object position from RDO. */
#define CY_PD_GET_RDO_GV_BACK(rdo)                (((rdo) >> 27) & 0x1)       /* Gets GiveBack flag from RDO. */
#define CY_PD_GET_RDO_CAP_MIS(rdo)                (((rdo) >> 26) & 0x1)       /* Gets Cap. Mismatch flag from RDO. */
#define CY_PD_GET_RDO_USB_COM(rdo)                (((rdo) >> 25) & 0x1)       /* Gets USB comm. Supports flag from RDO. */
#define CY_PD_GET_RDO_NO_SSPND(rdo)               (((rdo) >> 24) & 0x1)       /* Get USB suspend support flag from RDO. */
#define CY_PD_GET_RDO_OP_CUR(rdo)                 (((rdo) >> 10) & 0x3FF)     /* Gets Op. Current field from RDO. */
#define CY_PD_GET_RDO_OP_PWR(rdo)                 (((rdo) >> 10) & 0x3FF)     /* Gets Op. Power field from battery RDO. */
#define CY_PD_GET_RDO_MAX_OP_CUR(rdo)             (((rdo)) & 0x3FF)           /* Gets max. op. Current from RDO. */
#define CY_PD_GET_RDO_MIN_OP_CUR(rdo)             (((rdo)) & 0x3FF)           /* Gets min. op. Current from RDO. */
#define CY_PD_GET_RDO_MAX_OP_PWR(rdo)             (((rdo)) & 0x3FF)           /* Gets max. op. Power from battery RDO. */
#define CY_PD_GET_RDO_MIN_OP_PWR(rdo)             (((rdo)) & 0x3FF)           /* Gets min. op. Power from battery RDO. */

#define CY_PD_GET_VID(vdm_hdr)                    ((vdm_hdr) >> 16)           /* Gets VID from VDM header. */
#define CY_PD_GET_VDM_TYPE(vdm_hdr)               (((vdm_hdr) >> 15) & 0x1)   /* Gets VDM type from VDM header. */
#define CY_PD_GET_SVDM_VDM_VER(vdm_hdr)           (((vdm_hdr) >> 13) & 0x3)   /* Gets VDM version from VDM header. */
#define CY_PD_GET_SVDM_OBJ_POS(vdm_hdr)           (((vdm_hdr) >> 8) & 0x7)    /* Gets object position from VDM header. */
#define CY_PD_GET_SVDM_CMD_TYPE(vdm_hdr)          (((vdm_hdr) >> 6) & 0x3)    /* Gets command type from VDM header. */
#define CY_PD_GET_SVDM_CMD(vdm_hdr)               (((vdm_hdr)) & 0x1F)        /* Gets command code from VDM header. */

/* VDM header for D_ID command. */
#define CY_PD_STD_VDM_HEADER_IDENTITY_REQ                                             \
                                            (                                   \
                                                ((uint32_t)CY_PD_STD_SVID << 16) |              \
                                                ((uint32_t)CY_PDSTACK_VDM_TYPE_STRUCTURED << 15) |   \
                                                ((uint32_t)CY_PDSTACK_VDM_CMD_DSC_IDENTITY)          \
                                            )

/* VDM header for D_SVID command. */
#define CY_PD_STD_VDM_HEADER_SVID_REQ                                                 \
                                            (                                   \
                                                (CY_PD_STD_SVID << 16) |              \
                                                ((uint32_t)CY_PDSTACK_VDM_TYPE_STRUCTURED << 15) |   \
                                                ((uint32_t)CY_PDSTACK_VDM_CMD_DSC_SVIDS)             \
                                            )

#define CY_PD_GET_BIST_MODE(bist_hdr)             ((bist_hdr) >> 28)          /* Gets BIST mode from BIST header. */

/* Message masks for data messages. */
#define CY_PD_DATA_MSG_SRC_CAP_MASK               (0x1UL << CY_PDSTACK_DATA_MSG_SRC_CAP)
#define CY_PD_DATA_MSG_REQUEST_MASK               (0x1UL << CY_PDSTACK_DATA_MSG_REQUEST)
#define CY_PD_DATA_MSG_BIST_MASK                  (0x1UL << CY_PDSTACK_DATA_MSG_BIST)
#define CY_PD_DATA_MSG_SNK_CAP_MASK               (0x1UL << CY_PDSTACK_DATA_MSG_SNK_CAP)
#define CY_PD_DATA_MSG_BAT_STATUS_MASK            (0x1UL << CY_PDSTACK_DATA_MSG_BAT_STATUS)
#define CY_PD_DATA_MSG_SRC_ALERT_MASK             (0x1UL << CY_PDSTACK_DATA_MSG_ALERT)
#define CY_PD_DATA_MSG_VDM_MASK                   (0x1UL << CY_PDSTACK_DATA_MSG_VDM)
#define CY_PD_DATA_MSG_ENTER_USB_MASK             (0x1UL << CY_PDSTACK_DATA_MSG_ENTER_USB)

#define CY_PD_DATA_MSG_EPR_REQUEST_MASK           (0x1UL << CY_PDSTACK_DATA_MSG_EPR_REQUEST)
#define CY_PD_DATA_MSG_EPR_MODE_MASK              (0x1UL << CY_PDSTACK_DATA_MSG_EPR_MODE)     
#define CY_PD_DATA_MSG_SOURCE_INFO_MASK           (0x1UL << CY_PDSTACK_DATA_MSG_SOURCE_INFO)     
#define CY_PD_DATA_MSG_REVISION_MASK              (0x1UL << CY_PDSTACK_DATA_MSG_REVISION)    

/* Message masks for extended data messages. */
#define CY_PD_EXTD_MSG_SRC_CAP_EXTD_MASK          (0x1UL << CY_PDSTACK_EXTD_MSG_SRC_CAP_EXTD)
#define CY_PD_EXTD_MSG_STATUS_MASK                (0x1UL << CY_PDSTACK_EXTD_MSG_STATUS)
#define CY_PD_EXTD_MSG_GET_BAT_CAP_MASK           (0x1UL << CY_PDSTACK_EXTD_MSG_GET_BAT_CAP)
#define CY_PD_EXTD_MSG_GET_BAT_STATUS_MASK        (0x1UL << CY_PDSTACK_EXTD_MSG_GET_BAT_STATUS)
#define CY_PD_EXTD_MSG_BAT_CAP_MASK               (0x1UL << CY_PDSTACK_EXTD_MSG_BAT_CAP)
#define CY_PD_EXTD_MSG_GET_MANU_INFO_MASK         (0x1UL << CY_PDSTACK_EXTD_MSG_GET_MANU_INFO)
#define CY_PD_EXTD_MSG_MANU_INFO_MASK             (0x1UL << CY_PDSTACK_EXTD_MSG_MANU_INFO)
#define CY_PD_EXTD_MSG_SECURITY_REQ_MASK          (0x1UL << CY_PDSTACK_EXTD_MSG_SECURITY_REQ)
#define CY_PD_EXTD_MSG_SECURITY_RESP_MASK         (0x1UL << CY_PDSTACK_EXTD_MSG_SECURITY_RESP)
#define CY_PD_EXTD_MSG_FW_UPDATE_REQ_MASK         (0x1UL << CY_PDSTACK_EXTD_MSG_FW_UPDATE_REQ)
#define CY_PD_EXTD_MSG_FW_UPDATE_RESP_MASK        (0x1UL << CY_PDSTACK_EXTD_MSG_FW_UPDATE_RESP)
#define CY_PD_EXTD_MSG_SNK_CAP_EXTD_MASK          (0x1UL << CY_PDSTACK_EXTD_MSG_SNK_CAP_EXTD)

#define CY_PD_EXTD_MSG_EXTD_CTRL_MSG_MASK         (0x1 << CY_PDSTACK_EXTD_MSG_EXTD_CTRL_MSG)
#define CY_PD_EXTD_MSG_EPR_SRC_CAP_MASK           (0x1 << CY_PDSTACK_EXTD_MSG_EPR_SRC_CAP)
#define CY_PD_EXTD_MSG_EPR_SNK_CAP_MASK           (0x1 << CY_PDSTACK_EXTD_MSG_EPR_SNK_CAP)

#define CY_PD_CBL_CAP_0A                          (0u)        /* Cable current capability: 0 A. */
#define CY_PD_CBL_CAP_DFLT                        (90u)       /* Cable current capability: 900 mA (Type-C default). */
#define CY_PD_CBL_CAP_3A                          (300u)      /* Cable current capability: 3 A. */
#define CY_PD_CBL_CAP_5A                          (500u)      /* Cable current capability: 5 A. */

#define CY_PD_CBL_VDO_VERS_1_0                    (0u)        /* Active cable VDO version 1.0. */
#define CY_PD_CBL_VDO_VERS_1_2                    (2u)        /* Active cable VDO version 1.2. */
#define CY_PD_CBL_VDO_VERS_1_3                    (3u)        /* Active cable VDO version 1.3. */

#define CY_PD_UFP_VDO_VERSION                     (1u)        /* Version number of the VDO version 1.1. */

#define CY_PD_MAX_CBL_VBUS_50V                    (3u)        /* Max. cable VBUS voltage: 50 V. Checked in EPR mode. */

/*
 * Default cable current capability assumed by the stack. Use the
 * dpm_update_def_cable_cap() to change this value.
 */
#define CY_PD_DEFAULT_CBL_CAP                     (CY_PD_CBL_CAP_3A)

#define CY_PD_VSAFE_0V_SNK_MARGIN                 (0)         /* Allowed margin (%) for VSafe_0 measurement as sink. */
#define CY_PD_VSAFE_0V_PR_SWAP_SNK_SRC_MARGIN     (0)         /* Allowed margin (%) on VBus measurement during PR_SWAP. */
#define CY_PD_VSAFE_0V_HARD_RESET_MARGIN          (0)         /* Allowed margin (%) on VBus measurement during HARD RESET. */
#define CY_PD_VSAFE_0V_SRC_MARGIN                 (-50)
#define CY_PD_VSAFE_0V_SRC_TURN_ON_MARGIN         (0)
#define CY_PD_VSAFE_5V_SNK_TURN_ON_MARGIN         (-20)
#define CY_PD_VSAFE_5V_SNK_TURN_OFF_MARGIN        (-27)
#define CY_PD_VSAFE_SNK_TURN_OFF_MARGIN           (-20)
#define CY_PD_VSAFE_5V_SRC_TURN_ON_MARGIN         (-20)
#define CY_PD_VSAFE_5V_FRS_SWAP_RX_MARGIN         (10)
#define CY_PD_VSAFE_5V_FRS_SWAP_TX_MARGIN         (10)

/* Masks for policy engine-related events. */
#define CY_PD_PE_EVT_NONE                         (0x00000000u)       /* No policy engine events pending. */
#define CY_PD_PE_EVT_HARD_RESET_RCVD              (0x00000001u)       /* Hard reset received event pending. */
#define CY_PD_PE_EVT_SOFT_RESET_RCVD              (0x00000002u)       /* Soft reset received event pending. */
#define CY_PD_PE_EVT_ENTRY                        (0x00000004u)       /* Entry to new state machine. */
#define CY_PD_PE_EVT_TX_SUCCESS                   (0x00000008u)       /* PD message transmission completed. */
#define CY_PD_PE_EVT_TX_DISCARDED                 (0x00000010u)       /* PD message discarded due to collision. */
#define CY_PD_PE_EVT_TX_FAIL                      (0x00000020u)       /* PD message transmission failed. */
#define CY_PD_PE_EVT_PKT_RCVD                     (0x00000040u)       /* New PD message received. */
#define CY_PD_PE_EVT_PWR_RDY                      (0x00000080u)       /* Power ready (SRC/SNK transition complete) state. */
#define CY_PD_PE_EVT_TIMEOUT                      (0x00000100u)       /* Timeout event. */
#define CY_PD_PE_EVT_DPM_CMD_RCVD                 (0x00000200u)       /* DPM command received from APP layer. */
#define CY_PD_PE_EVT_APP_RESP_RCVD                (0x00000400u)       /* Response for PD message received from APP layer. */
#define CY_PD_PE_EVT_VDM_RESP_RCVD                (0x00000800u)       /* Response for VDM received from APP layer. */
#define CY_PD_PE_EVT_CABLE_TIMEOUT                (0x00001000u)       /* Cable access timed out. */
#define CY_PD_PE_EVT_NO_RESPONSE_TIMEOUT          (0x00002000u)       /* Response timeout event. */
#define CY_PD_PE_EVT_FR_SIGNAL_RCVD               (0x00004000u)       /* FR_Swap signalling received. */
#define CY_PD_PE_EVT_FR_SIGNAL_SENT               (0x00008000u)       /* FR_Swap signalling sent. */
#define CY_PD_PE_EVT_PPS_TIMEOUT                  (0x00010000u)       /* PPS timeout occurred. */
#define CY_PD_PE_EVT_CRC_ERROR                    (0x00020000u)       /* Message received with CRC error. */
#define CY_PD_PE_EVT_DRST_COMPLETE                (0x00040000u)       /* Data reset completion timer elapsed. */
#define CY_PD_PE_EVT_BIST_STM_ENTRY               (0x00080000u)       /* BIST STM Entry event. */
#define CY_PD_PE_EVT_BIST_STM_EXIT                (0x00100000u)       /* BIST STM Exit event. */
#define CY_PD_PE_EVT_ALL_MASK                     (0xFFFFFFFFu)       /* Mask to extract all pending events. */

/* Rp combinations macros. */
#define CY_PD_RP_CC1_OPEN_CC2_OPEN                ((((uint32_t)CY_PD_RP_OPEN) << 8) | (uint32_t)CY_PD_RP_OPEN) /* CCG/PMG1 Rp: No external termination on both CC. */
#define CY_PD_RP_CC1_OPEN_CC2_RD                  ((((uint32_t)CY_PD_RP_RD) << 8)   | (uint32_t)CY_PD_RP_OPEN) /* CCG/PMG1 Rp: external Rd on CC2. */
#define CY_PD_RP_CC1_OPEN_CC2_RA                  ((((uint32_t)CY_PD_RP_RA) << 8)   | (uint32_t)CY_PD_RP_OPEN) /* CCG/PMG1 Rp: external Ra on CC2. */
#define CY_PD_RP_CC1_RD_CC2_OPEN                  ((((uint32_t)CY_PD_RP_OPEN) << 8) | (uint32_t)CY_PD_RP_RD)   /* CCG/PMG1 Rp: external Rd on CC1. */
#define CY_PD_RP_CC1_RA_CC2_OPEN                  ((((uint32_t)CY_PD_RP_OPEN) << 8) | (uint32_t)CY_PD_RP_RA)   /* CCG/PMG1 Rp: external Ra on CC1. */
#define CY_PD_RP_CC1_RA_CC2_RD                    ((((uint32_t)CY_PD_RP_RD) << 8)   | (uint32_t)CY_PD_RP_RA)   /* CCG/PMG1 Rp: Rd on CC2, Ra on CC1. */
#define CY_PD_RP_CC1_RA_CC2_RA                    ((((uint32_t)CY_PD_RP_RA) << 8)   | (uint32_t)CY_PD_RP_RA)   /* CCG/PMG1 Rp: Ra on both CC. */
#define CY_PD_RP_CC1_RD_CC2_RA                    ((((uint32_t)CY_PD_RP_RA) << 8)   | (uint32_t)CY_PD_RP_RD)   /* CCG/PMG1 Rp: Rd on CC1, Ra on CC2. */
#define CY_PD_RP_CC1_RD_CC2_RD                    ((((uint32_t)CY_PD_RP_RD) << 8)   | (uint32_t)CY_PD_RP_RD)   /* CCG/PMG1 Rp: Rd on both CC. */

/* Rd combinations macros. */
#define CY_PD_RD_CC1_RA_CC2_USB                   ((((uint32_t)CY_PD_RD_USB) << 8)  | (uint32_t)CY_PD_RD_RA)   /* CCG/PMG1 Rd: External Default Rp on CC2. */
#define CY_PD_RD_CC1_RA_CC2_1_5A                  ((((uint32_t)CY_PD_RD_1_5A) << 8) | (uint32_t)CY_PD_RD_RA)   /* CCG/PMG1 Rd: 1.5 A Rp on CC2. */
#define CY_PD_RD_CC1_RA_CC2_3A                    ((((uint32_t)CY_PD_RD_3A) << 8)   | (uint32_t)CY_PD_RD_RA)   /* CCG/PMG1 Rd: 3A Rp on CC2. */
#define CY_PD_RD_CC1_USB_CC2_RA                   ((((uint32_t)CY_PD_RD_RA) << 8)   | (uint32_t)CY_PD_RD_USB)  /* CCG/PMG1 Rd: External Default Rp on CC1. */
#define CY_PD_RD_CC1_1_5A_CC2_RA                  ((((uint32_t)CY_PD_RD_RA) << 8)   | (uint32_t)CY_PD_RD_1_5A) /* CCG/PMG1 Rd: 1.5 A Rp on CC1. */
#define CY_PD_RD_CC1_3A_CC2_RA                    ((((uint32_t)CY_PD_RD_RA) << 8)   | (uint32_t)CY_PD_RD_3A)   /* CCG/PMG1 Rd: 3 A Rp on CC1. */
#define CY_PD_RD_CC1_RA_CC2_RA                    ((((uint32_t)CY_PD_RD_RA) << 8)   | (uint32_t)CY_PD_RD_RA)   /* CCG/PMG1 Rd: No Rp on either CC. */
#define CY_PD_RD_CC1_ERR_CC2_ERR                  ((((uint32_t)CY_PD_RD_ERR) << 8)  | (uint32_t)CY_PD_RD_ERR)  /* CCG/PMG1 Rd: Error state. */

#define CY_PD_RD_CC1_USB_CC2_USB                  ((((uint32_t)CY_PD_RD_USB) << 8)  | (uint32_t)CY_PD_RD_USB)  /* CCG/PMG1 Rd: External Default Rp on CC1 and CC2. */
#define CY_PD_RD_CC1_1_5A_CC2_1_5A                ((((uint32_t)CY_PD_RD_1_5A) << 8) | (uint32_t)CY_PD_RD_1_5A) /* CCG/PMG1 Rd: 1.5 A Rp on both CC1 and CC2. */
#define CY_PD_RD_CC1_3A_CC2_3A                    ((((uint32_t)CY_PD_RD_3A) << 8)   | (uint32_t)CY_PD_RD_3A)   /* CCG/PMG1 Rd: 3 A Rp on CC1 and CC2. */

/* Type-C events macros. */
#define CY_PD_TYPEC_EVT_NONE                      (0x00000000u)               /* No pending Type-C events. */
#define CY_PD_TYPEC_EVT_ERR_RECOVERY              (0x00000001u)               /* Type-C error recovery state. */
#define CY_PD_TYPEC_EVT_ENTRY                     (0x00000002u)               /* Entry to new Type-C state. */
#define CY_PD_TYPEC_EVT_DETACH                    (0x00000004u)               /* Detach event detected. */
#define CY_PD_TYPEC_EVT_ATTACH                    (0x00000008u)               /* Attach event detected. */
#define CY_PD_TYPEC_EVT_PWR_RDY                   (0x00000010u)               /* Power Ready (SRC/SNK transition complete) */
#define CY_PD_TYPEC_EVT_TIMEOUT1                  (0x00000020u)               /* Timeout event #1 */
#define CY_PD_TYPEC_EVT_TIMEOUT2                  (0x00000040u)               /* Timeout event #2 */
#define CY_PD_TYPEC_EVT_DPM_CMD_RCVD              (0x00000080u)               /* DPM command received from APP layer. */
#define CY_PD_TYPEC_EVT_ALL_MASK                  (0xFFFFFFFFu)               /* Mask to get all events. */

/* This is an internal timer used to prevent RX lock up */
#define CY_PD_GOOD_CRC_TX_TIMER_PERIOD         (3u)

#define CY_PD_FRS_SRC_SNK_MAX_WAIT_FOR_FR_SWAP (20u)
#define CY_PD_FRS_SNK_SRC_MAX_WAIT_FOR_RP      (14u)

/* CY_PD_TYPEC_SNK_TRY_TIMER_PERIOD must be more than CY_PD_TYPEC_PD_DEBOUNCE_TIMER_PERIOD */
#define CY_PD_TYPEC_SNK_TRY_TIMER_PERIOD          (18u)
#define CY_PD_TYPEC_DRP_TIMER_PERIOD              (37u)
#define CY_PD_TYPEC_VBUS_DISCHARGE_TIMER_PERIOD   (50u)
#define CY_PD_SRC_DISCONNECT_WAIT_PERIOD          (1u)
#define CY_PD_VCONN_TURN_ON_TIMER_PERIOD          (10u)
#define CY_PD_TYPEC_SINK_VBUS_DISCHARGE_PERIOD    (275u)
#define CY_PD_TYPEC_ACTIVITY_TIMER_PERIOD         (20u)
#define CY_PD_TYPEC_SYNC_TOGGLE_PERIOD            (30u)

#define CY_PD_PD_EXTD_STATUS_EVT_CF_POS           (4u)
#define CY_PD_PD_EXTD_PPS_STATUS_EVT_CF_POS       (3u)
#define CY_PD_PD_EXTD_STATUS_CBL_LIMIT_POS        (1u)

#define CY_PD_PD_EXTD_STATUS_PWR_SIZE             (8u)
#define CY_PD_PD_EXTD_STATUS_PWR_LED_POS          (3u)
#define CY_PD_PD_EXTD_STATUS_PWR_LED_MASK         (0x38u)

#define CY_PD_EXTD_STATUS_PWR_STATE_CHANGE_MASK   (0x000000FFu)
#define CY_PD_EXTD_STATUS_PWR_STATE_CHG_OFFSET    (6u)

/* Period to check whether the solution state allows to move forward with Type-C connection. */
#define CY_PD_TYPEC_SLN_STATUS_CHECK_PERIOD       (10u)

/** @endcond */

#define CY_PD_DP_HPD_TYPE_GPIO                    (0u)        /**< GPIO based HPD configuration. */
#define CY_PD_DP_HPD_TYPE_VIRTUAL                 (1u)        /**< Virtual (I2C based) HPD configuration. */

#define CY_PD_HOST_SBU_CFG_FULL                   (0u)        /**< Full (AUX/LSXX and polarity) selection for SBU MUX. */
#define CY_PD_HOST_SBU_CFG_FIXED_POL              (1u)        /**< SBU MUX (AUX/LSXX) connection without polarity switch. */
#define CY_PD_HOST_SBU_CFG_PASS_THROUGH           (2u)        /**< Pass through SBU MUX (AUX only) connection. */

/** \} group_pdstack_macros */

/**
* \addtogroup group_pdstack_enums
* \{
*/

/**
 * @typedef cy_en_pdstack_err_recov_reason_t
 * @brief Enumeration of reasons for error recovery entry.
 */
typedef enum {
    CY_PDSTACK_ERR_RECOV_REASON_NONE = 0,          /**< Error recovery is not active. */
    CY_PDSTACK_ERR_RECOV_HR_FAIL,                  /**< Error recovery because of hard reset failure. */
    CY_PDSTACK_ERR_RECOV_PROTECT_FAULT,            /**< Error recovery because of protection (OVP/OCP) fault. */
    CY_PDSTACK_ERR_RECOV_POWER_FAULT,              /**< Error recovery because of voltage fault. */
    CY_PDSTACK_ERR_RECOV_BAD_DATA_ROLE,            /**< Error recovery because of bad data role in incoming PD message. */
    CY_PDSTACK_ERR_RECOV_FRS_FAIL,                 /**< Error recovery because of Fast Role Swap error. */
    CY_PDSTACK_ERR_RECOV_DATA_RESET_FAIL           /**< Error recovery because of failed Data_Reset sequence. */
} cy_en_pdstack_err_recov_reason_t;

/**
 * @typedef cy_en_pdstack_emca_sr_reason_t
 * @brief Enumeration of possible reasons to issue an EMCA (SOP' or SOP'') soft reset.
 */
typedef enum {
    CY_PDSTACK_EMCA_SR_REASON_NONE = 0,            /**< No EMCA soft-reset in progress. */
    CY_PDSTACK_EMCA_SR_CABLE_DISC,                 /**< EMCA soft-reset due to cable discovery. */
    CY_PDSTACK_EMCA_SR_ALT_MODE_DISC               /**< EMCA soft-reset due to alt mode discovery. */
} cy_en_pdstack_emca_sr_reason_t;

/**
 * @typedef cy_en_pdstack_cable_reset_reason_t
 * @brief Enumeration of reasons for issuing a cable reset.
 */
typedef enum {
    CY_PDSTACK_EMCA_CABLE_RES_NONE = 0,            /**< No cable reset performed. */
    CY_PDSTACK_EMCA_CABLE_RES_SR_TIMEOUT           /**< SOP' or SOP'' soft reset timed out. */
} cy_en_pdstack_cable_reset_reason_t;

/**
 * @typedef cy_en_pdstack_hard_reset_reason_t
 * @brief Enumeration of reasons for issuing a hard reset.
 */
typedef enum {
    CY_PDSTACK_HARDRES_REASON_NONE = 0,         /**< Hard reset not issued. */
    CY_PDSTACK_HARDRES_REASON_NO_SRC_CAP,       /**< No source capability messages received. */
    CY_PDSTACK_HARDRES_REASON_HOSTCONN,         /**< TBT host connect state change. */
    CY_PDSTACK_HARDRES_REASON_SR_ERROR,         /**< Soft reset failed. */
    CY_PDSTACK_HARDRES_REASON_CONTRACT_ERROR,   /**< Power contract failed. */
    CY_PDSTACK_HARDRES_REASON_DRSWAP,           /**< DR swap received while in alternate mode. */
    CY_PDSTACK_HARDRES_REASON_VBUS_OVP,         /**< Over voltage condition detected. */
    CY_PDSTACK_HARDRES_REASON_VBUS_OCP,         /**< Over current condition detected. */
    CY_PDSTACK_HARDRES_REASON_AMS_ERROR,        /**< PD atomic message sequence error. */
} cy_en_pdstack_hard_reset_reason_t;

/**
 * @typedef cy_en_pdstack_soft_reset_reason_t
 * @brief Enumeration of reasons for issuing a soft reset.
 */
typedef enum {
    CY_PDSTACK_SOFTRES_REASON_NONE = 0,         /**< Soft reset not issued. */
    CY_PDSTACK_SOFTRES_REASON_SRCNEG_ERROR,     /**< Contract negotiation error when EZ-PD(TM) PMG1x is the source. */
    CY_PDSTACK_SOFTRES_REASON_SNKNEG_ERROR,     /**< Contract negotiation error when EZ-PD(TM) PMG1x is the sink. */
    CY_PDSTACK_SOFTRES_REASON_AMS_ERROR         /**< PD protocol error. */
} cy_en_pdstack_soft_reset_reason_t;

/**
 * @typedef app_swap_resp_t
 *
 * @brief Possible responses to various USB PD swap requests from the application layer.
 * The PD stack hands the requests up to the application for handling and gets directions
 * on handling the request in the form of these response codes.
 */
typedef enum {
    APP_RESP_ACCEPT         = 0u,       /**< Swap request should be accepted. */
    APP_RESP_REJECT,                    /**< Swap request should be rejected. */
    APP_RESP_WAIT,                      /**< Swap request handling should be delayed (send wait response). */
    APP_RESP_NOT_SUPPORTED              /**< Swap request is not supported. */
} app_swap_resp_t;

/**
 * @typedef cy_en_pdstack_pd_msg_class_t
 * @brief Enum of the PD message types.
 */
typedef enum
{
    CY_PDSTACK_PD_CTRL_MSG = 0,                    /**< Control message. */
    CY_PDSTACK_PD_DATA_MSG,                        /**< Data message. */
    CY_PDSTACK_PD_EXTD_MSG,                        /**< Extended data message. */
    CY_PDSTACK_PD_CABLE_RESET,                     /**< Cable reset message. */
    CY_PDSTACK_PD_MSG_RSVD                         /**< Undefined message type. */
} cy_en_pdstack_pd_msg_class_t;

/**
 * @typedef cy_en_pdstack_rdo_type_t
 * @brief Enum of the RDO types.
 */
typedef enum
{
    CY_PDSTACK_FIXED_VAR_RDO = 0,                  /**< Fixed or variable supply request data object. */
    CY_PDSTACK_BAT_RDO                             /**< Battery request data object. */
} cy_en_pdstack_rdo_type_t;

/**
 * @typedef cy_en_pdstack_data_msg_t
 * @brief Enum of the data message types.
 */
typedef enum
{
    CY_PDSTACK_DATA_MSG_SRC_CAP = 1,               /**< 0x01: Source_Capabilities message. */
    CY_PDSTACK_DATA_MSG_REQUEST,                   /**< 0x02: Request message. */
    CY_PDSTACK_DATA_MSG_BIST,                      /**< 0x03: BIST message. */
    CY_PDSTACK_DATA_MSG_SNK_CAP,                   /**< 0x04: Sink_Capabilities message. */
    CY_PDSTACK_DATA_MSG_BAT_STATUS,                /**< 0x05: Battery_Status message. */
    CY_PDSTACK_DATA_MSG_ALERT,                     /**< 0x06: Alert message. */
    CY_PDSTACK_DATA_MSG_GET_COUNTRY_INFO,          /**< 0x07: Get_Country_Info message. */
    CY_PDSTACK_DATA_MSG_ENTER_USB,                 /**< 0x08: Enter_USB message. */
    CY_PDSTACK_DATA_MSG_EPR_REQUEST,               /**< 0x09: EPR_Request message. */
    CY_PDSTACK_DATA_MSG_EPR_MODE,                  /**< 0x0A: EPR_Mode message. */
    CY_PDSTACK_DATA_MSG_SOURCE_INFO,               /**< 0x0B: Source_Info message. */
    CY_PDSTACK_DATA_MSG_REVISION,                  /**< 0x0C: Revision message. */
    CY_PDSTACK_DATA_MSG_VDM = 15                   /**< 0x0F: Vendor_Defined message. */
} cy_en_pdstack_data_msg_t;

/**
 * @typedef cy_en_pdstack_extd_msg_t
 * @brief Enum of the extended data message types.
 */
typedef enum
{
    CY_PDSTACK_EXTD_MSG_SRC_CAP_EXTD = 1,          /**< 0x01: Source_Capabilities_Extended message. */
    CY_PDSTACK_EXTD_MSG_STATUS,                    /**< 0x02: Status message. */
    CY_PDSTACK_EXTD_MSG_GET_BAT_CAP,               /**< 0x03: Get_Battery_Cap message. */
    CY_PDSTACK_EXTD_MSG_GET_BAT_STATUS,            /**< 0x04: Get_Battery_Status message. */
    CY_PDSTACK_EXTD_MSG_BAT_CAP,                   /**< 0x05: Battery_Capabilities message. */
    CY_PDSTACK_EXTD_MSG_GET_MANU_INFO,             /**< 0x06: Get_Manufacturer_Info message. */
    CY_PDSTACK_EXTD_MSG_MANU_INFO,                 /**< 0x07: Manufacturer_Info message. */
    CY_PDSTACK_EXTD_MSG_SECURITY_REQ,              /**< 0x08: Security_Request message. */
    CY_PDSTACK_EXTD_MSG_SECURITY_RESP,             /**< 0x09: Security_Response message. */
    CY_PDSTACK_EXTD_MSG_FW_UPDATE_REQ,             /**< 0x0A: Firmware_Update_Request message. */
    CY_PDSTACK_EXTD_MSG_FW_UPDATE_RESP,            /**< 0x0B: Firmware_Update_Response message. */
    CY_PDSTACK_EXTD_MSG_PPS_STATUS,                /**< 0x0C: PPS_Status message. */
    CY_PDSTACK_EXTD_MSG_COUNTRY_INFO,              /**< 0x0D: Country_Info message. */
    CY_PDSTACK_EXTD_MSG_COUNTRY_CODES,             /**< 0x0E: Country_Codes message. */
    CY_PDSTACK_EXTD_MSG_SNK_CAP_EXTD,              /**< 0x0F: Sink_Capabilities_Extended message. */
    CY_PDSTACK_EXTD_MSG_EXTD_CTRL_MSG,             /**< 0x10: Extended_Control_Msg message. */
    CY_PDSTACK_EXTD_MSG_EPR_SRC_CAP,               /**< 0x11: EPR_Source_Capabilities message. */
    CY_PDSTACK_EXTD_MSG_EPR_SNK_CAP                /**< 0x13: EPR_Sink_Capabilities message. */ 
} cy_en_pdstack_extd_msg_t;


/**
 * @typedef cy_en_pdstack_extd_ctrl_msg_t
 * @brief Enum of the extended control message types sent in Ext. Ctrl Message Data Block (offset 0).
 */
typedef enum
{
    CY_PDSTACK_EPR_GET_SRC_CAP = 1,                /**< EPR_Get_Source_Cap ext. control msg. */
    CY_PDSTACK_EPR_GET_SNK_CAP,                    /**< EPR_Get_Sink_Cap ext. control msg . */
    CY_PDSTACK_EPR_KEEP_ALIVE,                     /**< EPR_KeepAlive ext. control msg. */
    CY_PDSTACK_EPR_KEEP_ALIVE_ACK                  /**< EPR_KeepAlive_Ack ext. control msg. */
} cy_en_pdstack_extd_ctrl_msg_t;


/**
 * @typedef cy_en_pdstack_pdo_t
 * @brief Enum of the PDO types.
 */
typedef enum
{
    CY_PDSTACK_PDO_FIXED_SUPPLY = 0,               /**< Fixed (voltage) supply power data object. */
    CY_PDSTACK_PDO_BATTERY,                        /**< Battery-based power data object. */
    CY_PDSTACK_PDO_VARIABLE_SUPPLY,                /**< Variable (voltage) supply power data object. */
    CY_PDSTACK_PDO_AUGMENTED                       /**< Augmented power data object. */
} cy_en_pdstack_pdo_t;

/**
 * @typedef cy_en_pdstack_apdo_t
 * @brief Enum of the augmented PDO types.
 */
typedef enum
{
    CY_PDSTACK_APDO_PPS = 0,                       /**< Programmable power supply PDO. */
    CY_PDSTACK_APDO_AVS,                           /**< Reserved for future use. */
    CY_PDSTACK_APDO_RSVD2,                         /**< Reserved for future use. */
    CY_PDSTACK_APDO_RSVD3                          /**< Reserved for future use. */
} cy_en_pdstack_apdo_t;


/**
 * @typedef cy_en_pdstack_epr_avs_t
 * @brief Enum of the EPR AVS modes.
 */
typedef enum
{
    CY_PDSTACK_SPR_MODE = 0,                        /**< Standard power range mode. */
    CY_PDSTACK_EPR_FIXED_MODE,                      /**< Extended power range fixed mode.*/
    CY_PDSTACK_EPR_AVS_SMALL,                       /**< EPR adjustable voltage supply small step mode.*/
    CY_PDSTACK_EPR_AVS_LARGE,                       /**< EPR adjustable voltage supply large step mode. */
} cy_en_pdstack_epr_avs_t;

/**
 * @typedef cy_en_pdstack_peak_cur_cap_t
 * @brief Enum of peak current capability levels.
 */
typedef enum
{
    CY_PDSTACK_IMAX_EQ_IOC = 0,                    /**< Peak current equal to operating current. */
    CY_PDSTACK_IMAX_EQ_130_IOC,                    /**< Peak current is 1.3x operating current. */
    CY_PDSTACK_IMAX_EQ_150_IOC,                    /**< Peak current is 1.5x operating current. */
    CY_PDSTACK_IMAX_EQ_200_IOC                     /**< Peak current is 2x operating current. */
} cy_en_pdstack_peak_cur_cap_t;

/**
 * @typedef cy_en_pdstack_bist_mode_t
 * @brief Enum of the BIST modes.
 */
typedef enum
{
    CY_PDSTACK_BIST_RX_MODE = 0,                   /**< BIST receiver mode. */
    CY_PDSTACK_BIST_TX_MODE,                       /**< BIST transmit mode. */
    CY_PDSTACK_BIST_RETURN_COUNTERS_MODE,          /**< Send returned BIST counters response. */
    CY_PDSTACK_BIST_CARRIER_MODE_0,                /**< BIST carrier mode 0. */
    CY_PDSTACK_BIST_CARRIER_MODE_1,                /**< BIST carrier mode 1. */
    CY_PDSTACK_BIST_CARRIER_MODE_2,                /**< BIST carrier mode 2. */
    CY_PDSTACK_BIST_CARRIER_MODE_3,                /**< BIST carrier mode 3. */
    CY_PDSTACK_BIST_EYE_PATTERN_MODE,              /**< BIST eye pattern. */
    CY_PDSTACK_BIST_TEST_DATA_MODE,                /**< BIST test data mode. */
    CY_PDSTACK_BIST_STM_ENTRY,                     /**< BIST shared capacity test mode entry */
    CY_PDSTACK_BIST_STM_EXIT                       /**< BIST shared capacity test mode exit */    
} cy_en_pdstack_bist_mode_t;

/**
 * @typedef cy_en_pdstack_fr_swap_supp_t
 * @brief Enum to hold FR swap options in sink capabilities.
 */
typedef enum
{
    CY_PDSTACK_FR_SWAP_NOT_SUPPORTED = 0,          /**< FR_Swap is not supported. */
    CY_PDSTACK_FR_SWAP_DEF_USB,                    /**< Device will sink less than 900 mA of current after FR_Swap. */
    CY_PDSTACK_FR_SWAP_1_5A,                       /**< Device will sink less than 1.5 A of current after FR_Swap. */
    CY_PDSTACK_FR_SWAP_3A                          /**< Device will sink less than 3 A of current after FR_SWAP. */
} cy_en_pdstack_fr_swap_supp_t;

/**
 * @typedef cy_en_pdstack_app_req_status_t
 * @brief Enum of the PD request results. Enum fields map to the control
 * message field in the PD spec.
 */
typedef enum
{
    CY_PDSTACK_REQ_SEND_HARD_RESET = 1,            /**< Invalid message. Sends hard reset. */
    CY_PDSTACK_REQ_ACCEPT = 3,                     /**< Sends accept message. */
    CY_PDSTACK_REQ_REJECT = 4,                     /**< Sends reject message. */
    CY_PDSTACK_REQ_WAIT = 12,                      /**< Sends wait message. */
    CY_PDSTACK_REQ_NOT_SUPPORTED = 16              /**< Sends Not_Supported message. Translates to reject message under PD 2.0 */
} cy_en_pdstack_app_req_status_t;

/**
 * @typedef cy_en_pdstack_resp_status_t
 * @brief Enum of the response status to DPM commands.
 */
typedef enum
{
    CY_PDSTACK_SEQ_ABORTED = 0,                    /**< PD AMS aborted. */
    CY_PDSTACK_CMD_FAILED,                         /**< PD AMS failed. */
    CY_PDSTACK_RES_TIMEOUT,                        /**< No response received. */
    CY_PDSTACK_CMD_SENT,                           /**< PD command has been sent. Response wait may be in progress. */
    CY_PDSTACK_RES_RCVD                            /**< Response received. */
} cy_en_pdstack_resp_status_t;

/**
 * @typedef cy_en_pdstack_dpm_pd_cmd_t
 * @brief Enum of the Device Policy Manager (DPM) command types.
 */
typedef enum
{
    CY_PDSTACK_DPM_CMD_SRC_CAP_CHNG = 0,           /**< 00: Source caps changed notification. Used to trigger fresh contract. */
    CY_PDSTACK_DPM_CMD_SNK_CAP_CHNG,               /**< 01: Sink caps changed notification. Used to trigger fresh contract. */
    CY_PDSTACK_DPM_CMD_SEND_GO_TO_MIN,             /**< 02: Sends GotoMin message to the port partner. */
    CY_PDSTACK_DPM_CMD_GET_SNK_CAP,                /**< 03: Sends Get_Sink_Cap message to the port partner. */
    CY_PDSTACK_DPM_CMD_GET_SRC_CAP,                /**< 04: Sends Get_Source_Cap message to the port partner. */
    CY_PDSTACK_DPM_CMD_SEND_HARD_RESET,            /**< 05: Sends hard reset. */
    CY_PDSTACK_DPM_CMD_SEND_SOFT_RESET,            /**< 06: Sends a soft reset to the port partner. */
    CY_PDSTACK_DPM_CMD_SEND_CABLE_RESET,           /**< 07: Sends cable reset. */
    CY_PDSTACK_DPM_CMD_SEND_SOFT_RESET_EMCA,       /**< 08: Sends a soft reset to cable marker. */
    CY_PDSTACK_DPM_CMD_SEND_DR_SWAP,               /**< 09: Sends DR_Swap request. */
    CY_PDSTACK_DPM_CMD_SEND_PR_SWAP,               /**< 0A: Sends PR_Swap request. */
    CY_PDSTACK_DPM_CMD_SEND_VCONN_SWAP,            /**< 0B: Sends VCONN_Swap request. */
    CY_PDSTACK_DPM_CMD_SEND_VDM,                   /**< 0C: Sends VDM message. */
    CY_PDSTACK_DPM_CMD_SEND_EXTENDED,              /**< 0D: Sends extended data message. */
    CY_PDSTACK_DPM_CMD_GET_SRC_CAP_EXTENDED,       /**< 0E: Sends Get_Source_Cap_Extended message. */
    CY_PDSTACK_DPM_CMD_GET_STATUS,                 /**< 0F: Sends Get_Status message. */
    CY_PDSTACK_DPM_CMD_SEND_BATT_STATUS,           /**< 10: Sends Battery_Status data message. */
    CY_PDSTACK_DPM_CMD_SEND_ALERT,                 /**< 11: Sends an alert message. */
    CY_PDSTACK_DPM_CMD_SEND_NOT_SUPPORTED,         /**< 12: Sends Not_Supported message. */
    CY_PDSTACK_DPM_CMD_INITIATE_CBL_DISCOVERY,     /**< 13: Initiates cable discovery (preceded by VConn Swap if required). */
    CY_PDSTACK_DPM_CMD_SEND_DATA_RESET,            /**< 14: Sends a USB4 Data_Reset message. */
    CY_PDSTACK_DPM_CMD_SEND_ENTER_USB,             /**< 15: Sends a USB4 Enter_USB message to a port partner or cable marker. */
    CY_PDSTACK_DPM_CMD_GET_SNK_CAP_EXTENDED,       /**< 16: Sends Get_Sink_Cap_Extended message. */
    CY_PDSTACK_DPM_CMD_SEND_REQUEST,               /**< 17: Sends request data message. */
    CY_PDSTACK_DPM_CMD_GET_PPS_STATUS,             /**< 18: Sends Get_PPS_Status message. */
    CY_PDSTACK_DPM_CMD_GET_COUNTRY_CODES,          /**< 19: Sends Get_Country_Codes message. */
    CY_PDSTACK_DPM_CMD_SEND_EPR_MODE,              /**< 1A: Sends EPR_Mode message. */
    CY_PDSTACK_DPM_CMD_SNK_EPR_MODE_ENTRY,         /**< 1B: Sends EPR_Mode Entry message. */
    CY_PDSTACK_DPM_CMD_SNK_SEND_KEEP_ALIVE,        /**< 1C: Sends EPR Keepalive message. */
    CY_PDSTACK_DPM_CMD_SEND_EXTD_CTRL_MSG,         /**< 1D: Sends EXT_CTRL_MSG message. */
    CY_PDSTACK_DPM_CMD_SEND_GET_SOURCE_INFO,       /**< 1E: Sends Get_Source_Info message. */
    CY_PDSTACK_DPM_CMD_SEND_GET_REVISION,          /**< 1F: Sends Get_Revision message. */
    CY_PDSTACK_DPM_CMD_SEND_EPR_REQUEST,           /**< 20: Sends EPR request message. */
    CY_PDSTACK_DPM_CMD_SEND_INVALID = 0xFFu        /**< FF: Invalid command code. */
} cy_en_pdstack_dpm_pd_cmd_t;

/**
 * @typedef cy_en_pdstack_vdm_type_t
 * @brief Enum of the VDM types.
 */
typedef enum
{
    CY_PDSTACK_VDM_TYPE_UNSTRUCTURED = 0,        /**< Unstructured VDM. */
    CY_PDSTACK_VDM_TYPE_STRUCTURED,              /**< Structured VDM. */
} cy_en_pdstack_vdm_type_t;

/**
 * @typedef cy_en_pdstack_std_vdm_cmd_t
 * @brief Enum of the standard VDM commands.
 */
typedef enum
{
    CY_PDSTACK_VDM_CMD_DSC_IDENTITY = 1,           /**< Discover identity command. */
    CY_PDSTACK_VDM_CMD_DSC_SVIDS = 2,              /**< Discover SVIDs command. */
    CY_PDSTACK_VDM_CMD_DSC_MODES = 3,         /**< Discover modes command. */
    CY_PDSTACK_VDM_CMD_ENTER_MODE = 4,        /**< Enters mode command. */
    CY_PDSTACK_VDM_CMD_EXIT_MODE = 5,         /**< Exits mode command. */
    CY_PDSTACK_VDM_CMD_ATTENTION = 6,         /**< Attention message. */
    CY_PDSTACK_VDM_CMD_DP_STATUS_UPDT = 16,   /**< DisplayPort status update message. */
    CY_PDSTACK_VDM_CMD_DP_CONFIGURE = 17      /**< DisplayPort configure command. */
} cy_en_pdstack_std_vdm_cmd_t;

/**
 * @typedef cy_en_pdstack_std_vdm_cmd_type_t
 * @brief Enum of the standard VDM command types.
 */
typedef enum
{
    CY_PDSTACK_CMD_TYPE_INITIATOR = 0,             /**< VDM sent by command initiator. */
    CY_PDSTACK_CMD_TYPE_RESP_ACK,                  /**< ACK response. */
    CY_PDSTACK_CMD_TYPE_RESP_NAK,                  /**< NAK response. */
    CY_PDSTACK_CMD_TYPE_RESP_BUSY                  /**< BUSY response. */
} cy_en_pdstack_std_vdm_cmd_type_t;

/**
 * @typedef cy_en_pdstack_std_vdm_prod_t
 * @brief Enum of the standard VDM product types.
 */
typedef enum
{
    CY_PDSTACK_PROD_TYPE_UNDEF    = 0,             /**< Undefined device type. */
    CY_PDSTACK_PROD_TYPE_HUB      = 1,             /**< Hub device type. */
    CY_PDSTACK_PROD_TYPE_PERI     = 2,             /**< Peripheral device type. */
    CY_PDSTACK_PROD_TYPE_PSD      = 3,             /**< Power sink device. */
    CY_PDSTACK_PROD_TYPE_PAS_CBL  = 3,             /**< Passive cable. */
    CY_PDSTACK_PROD_TYPE_ACT_CBL  = 4,             /**< Active cable. */
    CY_PDSTACK_PROD_TYPE_AMA      = 5,             /**< Alternate mode accessory. */
    CY_PDSTACK_PROD_TYPE_VPD      = 6,             /**< VConn powered device. */
    CY_PDSTACK_PROD_TYPE_RSVD     = 7              /**< Reserved. Shall not be used. */
} cy_en_pdstack_std_vdm_prod_t;

/**
 * @typedef cy_en_pdstack_std_vdm_conn_t
 * @brief Enum of the standard VDM connector types.
 */
typedef enum
{
    CY_PDSTACK_CONN_TYPE_RSVD = 0,                 /**< Reserved for compatibility with legacy systems. */
    CY_PDSTACK_CONN_TYPE_RSVD1,                    /**< Reserved. Shall Not be used. */
    CY_PDSTACK_CONN_TYPE_RECEPTACLE,               /**< USB Type-C receptacle. */
    CY_PDSTACK_CONN_TYPE_PLUG                      /**< USB Type-C plug. */
} cy_en_pdstack_std_vdm_conn_t;

/**
 * @typedef cy_en_pdstack_std_vdm_ver_t
 * @brief Enum for the standard VDM version.
 */
typedef enum
{
    CY_PDSTACK_STD_VDM_VER1 = 0,                   /**< VDM version 1.0 */
    CY_PDSTACK_STD_VDM_VER2,                       /**< VDM version 2.0 */
    CY_PDSTACK_STD_VDM_VER3,                       /**< VDM version 3.0 */
    CY_PDSTACK_STD_VDM_VER4                        /**< VDM version 4.0 */
} cy_en_pdstack_std_vdm_ver_t;

/**
 * @typedef cy_en_pdstack_std_minor_vdm_ver_t
 * @brief Enum for the standard VDM minor version.
 */
typedef enum
{
    CY_PDSTACK_STD_VDM_MINOR_VER0 = 0,             /**< VDM minor version 0 */
    CY_PDSTACK_STD_VDM_MINOR_VER1,                 /**< VDM minor version 1 */
    CY_PDSTACK_STD_VDM_MINOR_VER2,                 /**< VDM minor version 2 */
    CY_PDSTACK_STD_VDM_MINOR_VER3                  /**< VDM minor version 3 */
} cy_en_pdstack_std_minor_vdm_ver_t;

/**
 * @typedef cy_en_pdstack_cbl_vbus_cur_t
 * @brief Enum of the cable current levels.
 */
typedef enum
{
    CY_PDSTACK_CBL_VBUS_CUR_DFLT = 0,              /**< Cable supports a maximum of 900 mA. */
    CY_PDSTACK_CBL_VBUS_CUR_3A,                    /**< Cable supports a maximum of 3 A. */
    CY_PDSTACK_CBL_VBUS_CUR_5A,                    /**< Cable supports a maximum of 5 A. */
    CY_PDSTACK_CBL_VBUS_CUR_0A                     /**< Cable does not conduct VBus power through. */
} cy_en_pdstack_cbl_vbus_cur_t;

/**
 * @typedef cy_en_pdstack_cbl_type_t
 * @brief Enum of the cable types.
 */
typedef enum
{
    CY_PDSTACK_CBL_TYPE_PASSIVE = 0,               /**< Passive cable. */
    CY_PDSTACK_CBL_TYPE_ACTIVE_RETIMER,            /**< Active re-timer cable. */
    CY_PDSTACK_CBL_TYPE_ACTIVE_REDRIVER,           /**< Active re-driver cable. */
    CY_PDSTACK_CBL_TYPE_OPTICAL                    /**< Optically isolated cable. */
} cy_en_pdstack_cbl_type_t;

/**
 * @typedef cy_en_pdstack_cbl_term_t
 * @brief Enum of the cable termination types.
 */
typedef enum
{
    CY_PDSTACK_CBL_TERM_BOTH_PAS_VCONN_NOT_REQ = 0,        /**< Passive cable; VConn not required. */
    CY_PDSTACK_CBL_TERM_BOTH_PAS_VCONN_REQ,                /**< Passive cable; VConn required. */
    CY_PDSTACK_CBL_TERM_ONE_ACT_ONE_PAS_VCONN_REQ,         /**< One end active; one end passive and VConn required. */
    CY_PDSTACK_CBL_TERM_BOTH_ACT_VCONN_REQ                 /**< Both ends of the cable are active; VConn required. */
} cy_en_pdstack_cbl_term_t;

/**
 * @typedef cy_en_pdstack_usb_sig_supp_t
 * @brief Enum of the USB signaling support.
 */
typedef enum
{
    CY_PDSTACK_USB_2_0 = 0,              /**< [USB 2.0] only. */
    CY_PDSTACK_USB_GEN_1,                /**< [USB 3.2] Gen1. */
    CY_PDSTACK_USB_GEN_2,                /**< [USB 3.2] Gen2 and [USB 4.0] Gen2. */
    CY_PDSTACK_USB_GEN_3,                /**< [USB 4.0] Gen 3. */
    CY_PDSTACK_USB_GEN_4                 /**< [USB 4.0] Gen 4. */
} cy_en_pdstack_usb_sig_supp_t;

/**
 * @typedef cy_en_pdstack_usb_dev_cap_t
 * @brief Enum of the USB device capabilities masks.
 */
typedef enum
{
    CY_PDSTACK_DEV_CAP_USB_2_0 = (1u<<0),     /**< [USB 2.0] device capable. */
    CY_PDSTACK_DEV_CAP_USB_BB_ONLY = (1u<<1), /**< Device capable (Billboard only). */
    CY_PDSTACK_DEV_CAP_USB_3_2 = (1u<<2),     /**< [USB 3.2] device capable. */
    CY_PDSTACK_DEV_CAP_USB_4_0 = (1u<<3)      /**< [USB4] device capable. */
} cy_en_pdstack_usb_dev_cap_t;

/**
 * @typedef cy_en_pdstack_usb_host_cap_t
 * @brief Enum of the USB host capabilities masks.
 */
typedef enum
{
    CY_PDSTACK_HOST_CAP_USB_2_0 = (1u<<0),  /**< [USB 2.0] host capable. */
    CY_PDSTACK_HOST_CAP_USB_3_2 = (1u<<1),  /**< [USB 3.2] host capable. */
    CY_PDSTACK_HOST_CAP_USB_4_0 = (1u<<2)   /**< [USB4] host capable. */
} cy_en_pdstack_usb_host_cap_t;

/**
 * @typedef cy_en_pdstack_usb_role_t
 * @brief Enum of the USB role types.
 */
typedef enum
{
    CY_PDSTACK_USB_ROLE_DEV = 0,                   /**< USB data role device. */
    CY_PDSTACK_USB_ROLE_HOST,                      /**< USB data role host. */
    CY_PDSTACK_USB_ROLE_DRD                        /**< USB data role device and host. */
} cy_en_pdstack_usb_role_t;

/**
 * @typedef cy_en_pdstack_pe_cbl_state_t
 * @brief Enum of the policy engine cable discovery states.
 */
typedef enum
{
    CY_PDSTACK_CBL_FSM_DISABLED = 0,                       /**< Cable state machine is inactive. */
    CY_PDSTACK_CBL_FSM_ENTRY,                              /**< Cable state machine starting up. */
    CY_PDSTACK_CBL_FSM_SEND_SOFT_RESET,                    /**< Cable state machine sending a soft reset to cable marker. */
    CY_PDSTACK_CBL_FSM_SEND_DSC_ID                         /**< Cable state machine waiting for cable response. */
} cy_en_pdstack_pe_cbl_state_t;

/**
 * @typedef cy_en_pdstack_try_src_snk_t
 * @brief Enum of the try source/sink options.
 */
typedef enum
{
    CY_PDSTACK_TRY_SRC_TRY_SNK_DISABLED = 0,       /**< Try.SRC and Try.SNK disabled. */
    CY_PDSTACK_TRY_SRC_ENABLED,                    /**< Try.SRC enabled. */
    CY_PDSTACK_TRY_SNK_ENABLED                     /**< Try.SNK enabled. */
} cy_en_pdstack_try_src_snk_t;

/**
 * @typedef cy_en_pdstack_dpm_typec_cmd_t
 * @brief Enum of the DPM command types that can be initiated through
 * the dpm_typec_command API.
 * @see dpm_typec_command
 */
typedef enum
{
    CY_PDSTACK_DPM_CMD_SET_RP_DFLT = 0,            /**< Command to select Default Rp. */
    CY_PDSTACK_DPM_CMD_SET_RP_1_5A,                /**< Command to select 1.5 A Rp. */
    CY_PDSTACK_DPM_CMD_SET_RP_3A                   /**< Command to select 3 A Rp. */,
    CY_PDSTACK_DPM_CMD_PORT_DISABLE,               /**< Command to disable the USB PD port. */
    CY_PDSTACK_DPM_CMD_TYPEC_ERR_RECOVERY,         /**< Command to initiate Type-C error recovery. */
    CY_PDSTACK_DPM_CMD_TYPEC_INVALID               /**< Invalid command type. */
} cy_en_pdstack_dpm_typec_cmd_t;

/**
 * @typedef cy_en_pdstack_dpm_typec_cmd_resp_t
 * @brief Enum of the DPM response types.
 */
typedef enum
{
    CY_PDSTACK_DPM_RESP_FAIL = 0,                  /**< Command failed. */
    CY_PDSTACK_DPM_RESP_SUCCESS                    /**< Command succeeded. */
} cy_en_pdstack_dpm_typec_cmd_resp_t;

/**
 * @typedef cy_en_pdstack_typec_fsm_state_t
 * @brief Enum of the Type-C FSM states. This is for internal stack usage.
 * @warning Ordering of elements must not be altered unless the state table
 * is also updated to match.
 */
typedef enum
{
    CY_PDSTACK_TYPEC_FSM_DISABLED = 0,             /**< Type-C state machine is disabled. */
    CY_PDSTACK_TYPEC_FSM_ERR_RECOV,                /**< Error recovery state. */
    CY_PDSTACK_TYPEC_FSM_ATTACH_WAIT,              /**< AttachWait.SRC or AttachWait.SNK state. */
#if (!(CY_PD_TRY_SRC_SNK_DISABLE))
    CY_PDSTACK_TYPEC_FSM_TRY_SRC,                  /**< Try.SRC state. */
    CY_PDSTACK_TYPEC_FSM_TRY_WAIT_SNK,             /**< TryWait.SNK state. */
    CY_PDSTACK_TYPEC_FSM_TRY_SNK,                  /**< Try.SNK state. */
    CY_PDSTACK_TYPEC_FSM_TRY_WAIT_SRC,             /**< TryWait.SRC state. */
#endif /* (!(CY_PD_TRY_SRC_SNK_DISABLE)) */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_TYPEC_FSM_UNATTACHED_SRC,           /**< Unattached.SRC state. */
#endif /* (!CY_PD_SINK_ONLY) */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_TYPEC_FSM_UNATTACHED_SNK,           /**< Unattached.SNK state. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_TYPEC_FSM_UNATTACHED_WAIT_SRC,      /**< UnattachedWait.SRC state. */
#endif /* (!CY_PD_SINK_ONLY) */
    CY_PDSTACK_TYPEC_FSM_AUD_ACC,                  /**< AudioAccessory state. */
    CY_PDSTACK_TYPEC_FSM_DBG_ACC,                  /**< DebugAccessory state. */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_TYPEC_FSM_ATTACHED_SRC,             /**< Attached.SRC state. */
#endif /* (!CY_PD_SINK_ONLY) */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_TYPEC_FSM_ATTACHED_SNK,             /**< Attached.SNK state. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    CY_PDSTACK_TYPEC_FSM_MAX_STATES                /**< Invalid Type-C state. */
} cy_en_pdstack_typec_fsm_state_t;

/**
 * @enum cy_en_pdstack_pe_fsm_state_t
 * @brief Enumeration of policy engine states for a USB PD port. This is for internal stack usage.
 * @warning  Ordering of elements must not be altered unless the state table in the stack
 * source is also updated.
 */
typedef enum
{
    CY_PDSTACK_PE_FSM_OFF = 0,                             /**< 00: Policy engine not started. */
    CY_PDSTACK_PE_FSM_HR_SEND,                             /**< 01: Sends hard reset */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_PE_FSM_HR_SRC_TRANS_DFLT,                   /**< 02: PE_SRC_Transition_to_default */
    CY_PDSTACK_PE_FSM_HR_SRC_RECOVER,                      /**< 03: Policy engine waiting for recovery before enabling VBus. */
    CY_PDSTACK_PE_FSM_HR_SRC_VBUS_ON,                      /**< 04: Policy engine enabling VBus after hard reset completion. */
#endif /* (!CY_PD_SINK_ONLY) */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_PE_FSM_HR_SNK_TRANS_DFLT,                   /**< 05: PE_SNK_Transition_to_default */
    CY_PDSTACK_PE_FSM_HR_SNK_WAIT_VBUS_OFF,                /**< 06: Policy engine waiting for VBus to turn OFF. */
    CY_PDSTACK_PE_FSM_HR_SNK_WAIT_VBUS_ON,                 /**< 07: Policy engine waiting for VBus to turn ON. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    CY_PDSTACK_PE_FSM_BIST_TEST_DATA,                      /**< 08: BIST test data state. */
    CY_PDSTACK_PE_FSM_BIST_CM2,                            /**< 09: PE_BIST_Carrier_Mode. */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_PE_FSM_SNK_STARTUP,                         /**< 10: PE_SNK_Startup. */
    CY_PDSTACK_PE_FSM_SNK_WAIT_FOR_CAP,                    /**< 11: PE_SNK_Wait_for_Capabilities. */
    CY_PDSTACK_PE_FSM_SNK_EVAL_CAP,                        /**< 12: PE_SNK_Evaluate_Capability. */
    CY_PDSTACK_PE_FSM_SNK_SEL_CAP,                         /**< 13: PE_SNK_Select_Capability. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_PE_FSM_SRC_STARTUP,                         /**< 14: PE_SRC_Startup. */
    CY_PDSTACK_PE_FSM_SRC_WAIT_NEW_CAP,                    /**< 15: PE_SRC_Wait_New_Capabilities. */
#if (!(CY_PD_CBL_DISC_DISABLE))
    CY_PDSTACK_PE_FSM_SRC_SEND_CBL_SR,                     /**< 16: PE_CBL_Soft_Reset. */
    CY_PDSTACK_PE_FSM_SRC_SEND_CBL_DSCID,                  /**< 17: PE_CBL_Get_Identity. */
#endif /* (!(CY_PD_CBL_DISC_DISABLE)) */
    CY_PDSTACK_PE_FSM_SRC_SEND_CAP,                        /**< 18: PE_SRC_Send_Capabilities. */
    CY_PDSTACK_PE_FSM_SRC_DISCOVERY,                       /**< 19: PE_SRC_Discovery. */
    CY_PDSTACK_PE_FSM_SRC_NEG_CAP,                         /**< 20: PE_SRC_Negotiate_Capability. */
    CY_PDSTACK_PE_FSM_SRC_TRANS_SUPPLY,                    /**< 21: PE_SRC_Transition_Supply. */
    CY_PDSTACK_PE_FSM_SRC_SEND_PS_RDY,                     /**< 22: Policy Engine waiting to send PS_RDY. */
#endif /* (!CY_PD_SINK_ONLY) */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_PE_FSM_SNK_TRANS,                           /**< 23: PE_SNK_Transition_Sink */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    CY_PDSTACK_PE_FSM_SR_SEND,                             /**< 24: Policy engine sending a soft reset. */
    CY_PDSTACK_PE_FSM_SR_RCVD,                             /**< 25: Policy engine received soft reset. */
    CY_PDSTACK_PE_FSM_VRS_VCONN_ON,                        /**< 26: Policy engine waiting for VConn to turn ON. */
    CY_PDSTACK_PE_FSM_VRS_VCONN_OFF,                       /**< 27: Policy engine waiting for VConn to turn OFF. */
    CY_PDSTACK_PE_FSM_SWAP_EVAL,                           /**< 28: Evaluate received swap command. */
    CY_PDSTACK_PE_FSM_SWAP_SEND,                           /**< 29: Waiting to send swap command. */
    CY_PDSTACK_PE_FSM_DRS_CHANGE_ROLE,                     /**< 30: Change data role. */
#if ((!(CY_PD_SOURCE_ONLY)) && (!CY_PD_SINK_ONLY))
    CY_PDSTACK_PE_FSM_PRS_SRC_SNK_TRANS,                   /**< 31: Source to sink PR_Swap transition start. */
    CY_PDSTACK_PE_FSM_PRS_SRC_SNK_VBUS_OFF,                /**< 32: Initial source waiting for VBus to turn OFF. */
    CY_PDSTACK_PE_FSM_PRS_SRC_SNK_WAIT_PS_RDY,             /**< 33: Initial source waiting for PS_RDY. */
    CY_PDSTACK_PE_FSM_PRS_SNK_SRC_WAIT_PS_RDY,             /**< 34: Initial sink waiting for PS_RDY. */
    CY_PDSTACK_PE_FSM_PRS_SNK_SRC_VBUS_ON,                 /**< 35: Initial sink turning VBus ON. */
    CY_PDSTACK_PE_FSM_FRS_CHECK_RP,                        /**< 36: Initial sink checking Rp to send FR_Swap message. */
    CY_PDSTACK_PE_FSM_FRS_SRC_SNK_CC_SIGNAL,               /**< 37: Initial source sending FR_Swap signal. */
#endif /* ((!(CY_PD_SOURCE_ONLY)) && (!CY_PD_SINK_ONLY)) */
    CY_PDSTACK_PE_FSM_READY,                               /**< 38: PE_Ready state. */
    CY_PDSTACK_PE_FSM_SEND_MSG,                            /**< 39: Policy engine sending new AMS. */
    CY_PDSTACK_PE_FSM_EVAL_DATA_RESET,                     /**< 40: Policy engine handling Data_Reset request. */
    CY_PDSTACK_PE_FSM_SEND_DATA_RESET,                     /**< 41: Policy engine initiating Data_Reset request. */
    CY_PDSTACK_PE_FSM_EVAL_ENTER_USB,                      /**< 42: Policy engine handling enter USB request. */
#if (!(CY_PD_SINK_ONLY)) 
    CY_PDSTACK_PE_FSM_SRC_EVAL_EPR_MODE_ENTRY,             /**< 43: Policy engine handling EPR mode request. */
    CY_PDSTACK_PE_FSM_SRC_SEND_EPR_MODE_RESULT,            /**< 44: Policy engine handling EPR mode request. */
    CY_PDSTACK_PE_FSM_SRC_SEND_EPR_SRC_CAP,                /**< 45: Policy engine sends EPR SRC CAP. */
#endif /* (!(CY_PD_SINK_ONLY)) */ 
#if (!(CY_PD_SOURCE_ONLY))    
    CY_PDSTACK_PE_FSM_SNK_SEND_EPR_MODE_ENTRY,             /**< 46: Policy engine requests EPR mode entry. */
    CY_PDSTACK_PE_FSM_SNK_EPR_ENTRY_WAIT_FOR_RESP,         /**< 47: Policy engine handling EPR mode request. */
    CY_PDSTACK_PE_FSM_SNK_EPR_KEEP_ALIVE,                  /**< 48: Policy engine sends EPR mode Keepalive. */
    CY_PDSTACK_PE_FSM_SNK_SEND_EPR_CAP,                    /**< 49: Policy engine sends EPR sink capabilities. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    CY_PDSTACK_PE_FSM_MAX_STATES                           /**< 50: Invalid policy engine state. */
}cy_en_pdstack_pe_fsm_state_t;

/**
 * @typedef cy_en_pdstack_contract_status_t
 * @brief Enum of possible PD contract negotiation scenarios that are used to
 * signal the application event handler. This status will be reported in byte 0
 * of the event data passed along with the APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE
 * event. Bytes 3:1 of the event data are not used, and bytes 7:4 reports
 * the RDO where applicable.
 */
typedef enum
{
    CY_PDSTACK_CONTRACT_NEGOTIATION_SUCCESSFUL      = 0x01,     /**< PD contract negotiation successful. */
    CY_PDSTACK_CONTRACT_CAP_MISMATCH_DETECTED       = 0x03,     /**< PD contract negotiated, but capability mismatch
                                                             is present. */
    CY_PDSTACK_CONTRACT_REJECT_CONTRACT_VALID       = 0x00,     /**< Contract rejected by EZ-PD(TM) PMG1 but the previous contract
                                                             is still valid. */
    CY_PDSTACK_CONTRACT_REJECT_CONTRACT_NOT_VALID   = 0x04,     /**< Contract rejected by EZ-PD(TM) PMG1 and the previous contract
                                                             became invalid. */
    CY_PDSTACK_CONTRACT_REJECT_NO_CONTRACT          = 0x08,     /**< Contract rejected by EZ-PD(TM) PMG1 and there was no previous
                                                             contract. */
    CY_PDSTACK_CONTRACT_REJECT_EXPLICIT_CONTRACT    = 0x0C,     /**< Request rejected by the port partner while in the previous
                                                             explicit contract. */
    CY_PDSTACK_CONTRACT_REJECT_NO_EXPLICIT_CONTRACT = 0x10,     /**< Request rejected by the port partner with no previous
                                                             explicit contract. */
    CY_PDSTACK_CONTRACT_PS_READY_NOT_RECEIVED       = 0x14,     /**< Failed to receive PS_RDY after accept. */
    CY_PDSTACK_CONTRACT_PS_READY_NOT_SENT           = 0x18      /**< Failed to send PS_RDY after accept. */
} cy_en_pdstack_contract_status_t;

/**
 * @typedef cy_en_pdstack_app_evt_t
 * @brief Enum of events that are signaled to the application.
 */
typedef enum
{
    APP_EVT_UNEXPECTED_VOLTAGE_ON_VBUS,         /**< 0x00: Unexpected high-voltage seen on VBus. */
    APP_EVT_TYPE_C_ERROR_RECOVERY,              /**< 0x01: Type-C error recovery initiated. */
    APP_EVT_CONNECT,                            /**< 0x02: Type-C connect detected. */
    APP_EVT_DISCONNECT,                         /**< 0x03: Type-C disconnect(detach) detected. */
    APP_EVT_EMCA_DETECTED,                      /**< 0x04: Cable (EMCA) discovery successful. */
    APP_EVT_EMCA_NOT_DETECTED,                  /**< 0x05: Cable (EMCA) discovery timed out. */
    APP_EVT_ALT_MODE,                           /**< 0x06: Alternate mode-related event. */
    APP_EVT_APP_HW,                             /**< 0x07: MUX control-related event. */
    APP_EVT_BB,                                 /**< 0x08: Billboard status change. */
    APP_EVT_RP_CHANGE,                          /**< 0x09: Rp termination change detected. */
    APP_EVT_HARD_RESET_RCVD,                    /**< 0x0A: Hard reset received. */
    APP_EVT_HARD_RESET_COMPLETE,                /**< 0x0B: Hard reset processing completed. */
    APP_EVT_PKT_RCVD,                           /**< 0x0C: New PD message received. */
    APP_EVT_PR_SWAP_COMPLETE,                   /**< 0x0D: PR_SWAP process completed. */
    APP_EVT_DR_SWAP_COMPLETE,                   /**< 0x0E: DR_SWAP process completed. */
    APP_EVT_VCONN_SWAP_COMPLETE,                /**< 0x0F: VConn_SWAP process completed. */
    APP_EVT_SENDER_RESPONSE_TIMEOUT,            /**< 0x10: Sender response timeout occurred. */
    APP_EVT_VENDOR_RESPONSE_TIMEOUT,            /**< 0x11: Vendor message response timeout occurred. */
    APP_EVT_HARD_RESET_SENT,                    /**< 0x12: Hard reset sent by EZ-PD(TM) PMG1. */
    APP_EVT_SOFT_RESET_SENT,                    /**< 0x13: Soft reset sent by EZ-PD(TM) PMG1. */
    APP_EVT_CBL_RESET_SENT,                     /**< 0x14: Cable reset sent by EZ-PD(TM) PMG1. */
    APP_EVT_PE_DISABLED,                        /**< 0x15: PE. Disabled state entered. */
    APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE,   /**< 0x16: Contract negotiation completed. */
    APP_EVT_VBUS_OVP_FAULT,                     /**< 0x17: VBus over voltage fault detected. */
    APP_EVT_VBUS_OCP_FAULT,                     /**< 0x18: VBus over current fault detected. */
    APP_EVT_VCONN_OCP_FAULT,                    /**< 0x19: VConn over current fault detected. */
    APP_EVT_VBUS_PORT_DISABLE,                  /**< 0x1A: PD port disable completed. */
    APP_EVT_TYPEC_STARTED,                      /**< 0x1B: PD port enable (start) completed. */
    APP_EVT_FR_SWAP_COMPLETE,                   /**< 0x1C: FR_SWAP process completed. */
    APP_EVT_TEMPERATURE_FAULT,                  /**< 0x1D: Over temperature fault detected. */
    APP_EVT_HANDLE_EXTENDED_MSG,                /**< 0x1E: Extended message received and needs to be handled. */
    APP_EVT_VBUS_UVP_FAULT,                     /**< 0x1F: VBus undervoltage fault detected. */
    APP_EVT_VBUS_SCP_FAULT,                     /**< 0x20: VBus short-circuit fault detected. */
    APP_EVT_TYPEC_ATTACH_WAIT,                  /**< 0x21: Type-C AttachWait state entered. */
    APP_EVT_TYPEC_ATTACH_WAIT_TO_UNATTACHED,    /**< 0x22: Type-C transition from AttachWait to unattached. */
    APP_EVT_TYPEC_ATTACH,                       /**< 0x23: Type-C attach event. */
    APP_EVT_CC_OVP,                             /**< 0x24: Over voltage on CC/VConn line detected. */
    APP_EVT_SBU_OVP,                            /**< 0x25: Over voltage on SBU1/SBU2 line detected. */
    APP_EVT_ALERT_RECEIVED,                     /**< 0x26: Alert message received. */
    APP_EVT_SRC_CAP_TRIED_WITH_NO_RESPONSE,     /**< 0x27: Src cap tried with no response. */
    APP_EVT_PD_SINK_DEVICE_CONNECTED,           /**< 0x28: Sink device connected. */
    APP_EVT_VBUS_RCP_FAULT,                     /**< 0x29: VBus reverse current fault detected. */
    APP_EVT_STANDBY_CURRENT,                    /**< 0x2A: Standby current. */
    APP_EVT_DATA_RESET_RCVD,                    /**< 0x2B: USB4 data reset message received. USB connection
                                                           should be disabled by DFP on receiving this event. */
    APP_EVT_DATA_RESET_SENT,                    /**< 0x2C: USB4 data reset message sent. USB connection should
                                                           be disabled by DFP on receiving this event. */
    APP_EVT_DATA_RESET_CPLT,                    /**< 0x2D: USB4 data reset process complete. No handling is required. */
    APP_EVT_USB_ENTRY_CPLT,                     /**< 0x2E: USB4 entry process complete. */
    APP_EVT_DATA_RESET_ACCEPTED,                /**< 0x2F: USB4 data reset accepted. USB connection can be
                                                           re-enabled by DFP on receiving this event. */
    APP_EVT_CONFIG_ERROR,                       /**< 0x30: Stack configuration error event. */
    APP_EVT_POWER_CYCLE,                        /**< 0x31: Power cycle/reset event. */
    APP_EVT_VBUS_IN_UVP_FAULT,                  /**< 0x32: Vbus_in undervoltage fault detected. */
    APP_EVT_VBUS_IN_OVP_FAULT,                  /**< 0x33: Vbus_in overvoltage fault detected. */
    APP_EVT_SYSTEM_OT_FAULT,                    /**< 0x34: System overtemperature fault detected. */
    APP_EVT_CRC_ERROR,                          /**< 0x35: PD CRC error detected. */
    APP_EVT_HR_PSRC_ENABLE,                     /**< 0x36: PSRC enable is about to be called after hard reset. */

    APP_EVT_TYPEC_RP_DETACH,                    /**< 0x37: Rp removal detected while in the Attached.SNK state. */
    APP_EVT_PR_SWAP_ACCEPTED,                   /**< 0x38: PR-SWAP accepted by EZ-PD(TM) PMG1 or port partner. */
    APP_EVT_HR_SENT_RCVD_DEFERRED,              /**< 0x39: Deferred hard reset sent/received event handling to
                                                           accommodate retimer communication delay timing. */
    APP_EVT_BAD_SINK_APDO_SEL,                  /**< 0x3A: APDO selection in PD 2.0 or less revision */

    APP_EVT_BC_DETECTION_COMPLETED,             /**< 0x3B: Legacy battery charging protocol detection completed. */

    APP_EVT_SNK_FET_ENABLE,                     /**< 0x3C: HPI enables  SNK FET cmd event. */
    APP_EVT_SNK_FET_DISABLE,                    /**< 0x3D: HPI disables SNK FET cmd event. */
    APP_EVT_SAFE_PWR_ENABLE,                    /**< 0x3E: HPI enables safe PWR path cmd event. */
    APP_EVT_SAFE_PWR_DISABLE,                   /**< 0x3F: HPI disables safe PWR path cmd event. */
    APP_EVT_FAULT_CLEANED,                      /**< 0x40: OVP/OCP/OTP fault state is cleaned. */
    APP_EVT_MISMATCH_CLEANED,                   /**< 0x41: MISMATCH fault state is cleaned. */

    APP_EVT_CUST_ALT_MODE_CHANGED,              /**< 0x42: Custom event that could be set for notification of alternate 
                                                           mode-specific conditions such as mode entry and exit. */
    APP_EVT_CUST_MODE_DISC_CMPL,                /**< 0x43: Apple sequencing is finished. */

    APP_EVT_VBAT_GND_SCP_FAULT,                 /**< 0x44: Battery to ground short circuit fault detected. */
    APP_EVT_VIN_UVP_FAULT,                      /**< 0x45: Regulator input undervoltage fault detected. */
    APP_EVT_VIN_OVP_FAULT,                      /**< 0x46: Regulator input overvoltage fault detected. */
    APP_EVT_BIST_STM_ENTRY,                     /**< 0x47: BIST STM entry event. */
    APP_EVT_BIST_STM_EXIT,                      /**< 0x48: BIST STM exit event. */
    APP_EVT_ILIM_FAULT,                         /**< 0x49: Inductor limit fault detected. */
    APP_EVT_VREG_INRUSH_FAULT,                  /**< 0x4A: Vreg inrush detect fault detected. */
    APP_EVT_VREG_BOD_FAULT,                     /**< 0x4B: Device brown-out detect fault detected. */
    APP_EVT_VCONN_SCP_FAULT,                    /**< 0x4C: VConn short-circuit fault detected. */
    APP_EVT_SOURCE_INFO_RECEIVED,               /**< 0x4D: Source info message received from the sink. */
    APP_EVT_REVISION_RECEIVED,                  /**< 0x4E: Revision data message received. */  

    APP_EVT_EPR_MODE_ENTER_RECEIVED,            /**< 0x4F: EPR mode enters command received from sink port. */
    APP_EVT_EPR_MODE_ENTER_SUCCESS,             /**< 0x50: EPR mode enter successfully, source response with enter succeeded. */
    APP_EVT_EPR_MODE_ENTER_FAILED,              /**< 0x51: EPR mode enter failed, source response with fail. */
    APP_EVT_EPR_MODE_EXIT,                      /**< 0x52: EPR mode exit received. */
    APP_EVT_VCONN_SWAP_FAILED,                  /**< 0x53: VConn_SWAP process failed in EPR enter msg. sequence. */
    APP_TOTAL_EVENTS                            /**< 0x54: Total number of application events. */
} cy_en_pdstack_app_evt_t;

/**
 * @typedef cy_en_pdstack_ams_type_t
 * @brief Type of USB PD Atomic Message Sequence (AMS).
 */
typedef enum
{
    CY_PDSTACK_AMS_NONE = 0,                            /**< No AMS active. */
    CY_PDSTACK_AMS_NON_INTR,                            /**< Non-interruptible AMS is active. */
    CY_PDSTACK_AMS_INTR                                 /**< Interruptible AMS is active. */
} cy_en_pdstack_ams_type_t;

/**
 * Enum to hold expected response result.
*/
typedef enum
{
    PD_AMS_INTERUPTIBLE = 0,
    PD_AMS_NON_INTR_SEND_PENDING,
    PD_AMS_NON_INTR_SEND_RCV_PENDING,

}pd_ams_type_t;

/**
 * @typedef cy_en_pdstack_vdm_ams_t
 * @brief Enumeration of application responses to policy manager.
 */
typedef enum
{
    CY_PDSTACK_VDM_AMS_RESP_READY = 0,                     /**< Response is ready. */
    CY_PDSTACK_VDM_AMS_RESP_NOT_REQ,                       /**< No response required. */
    CY_PDSTACK_VDM_AMS_RESP_FROM_EC,                       /**< Response will come from EC. */
    CY_PDSTACK_VDM_AMS_RESP_NOT_SUPP                       /**< Sends a NOT_SUPPORTED response. */
} cy_en_pdstack_vdm_ams_t;

/**
 * @typedef cy_en_pdstack_usb_data_sig_t
 * @brief Enumeration of USB signaling supported by a device or cable.
 */
typedef enum
{
    CY_PDSTACK_USB_2_0_SUPP = 0,                           /**< Only USB 2.0 support. */
    CY_PDSTACK_USB_GEN_1_SUPP,                             /**< USB 3.1 Gen1 (5 Gbps) support. */
    CY_PDSTACK_USB_GEN_2_SUPP,                             /**< USB 3.1 Gen2 (10 Gbps) support. */
    CY_PDSTACK_USB_GEN_3_SUPP,                             /**< USB 4 Gen3 (20 Gbps) support. */
    CY_PDSTACK_USB_BB_SUPP,                                /**< USB Billboard device support. */
    CY_PDSTACK_USB_SIG_UNKNOWN                             /**< USB data signaling support unknown. */
} cy_en_pdstack_usb_data_sig_t;

/**
 * @typedef cy_en_pdstack_data_reset_state_t
 * @brief Enumeration of sub-states associated with Data Reset AMS.
 */
typedef enum
{
    CY_PDSTACK_DATA_RESET_IDLE = 0,                        /**< No Data_Reset related operation pending. */
    CY_PDSTACK_DATA_RESET_WAIT_ACCEPT,                     /**< Sender waiting for Data_Reset acceptance. */
    CY_PDSTACK_DATA_RESET_ACCEPTED,                        /**< Waiting for the next step after sending accept response. */
    CY_PDSTACK_DATA_RESET_WAIT_PS_RDY,                     /**< Waiting for PS_RDY at the end of Data_Reset handshake. */
    CY_PDSTACK_DATA_RESET_WAIT_VCONN_OFF,                  /**< Waiting for VConn turn-off completion before sending PS_RDY. */
    CY_PDSTACK_DATA_RESET_SENDING_PS_RDY,                  /**< In the process of sending PS_RDY after Data_Reset handshake. */
    CY_PDSTACK_DATA_RESET_WAIT_VCONN_ON,                   /**< DFP waiting to turn VConn ON. */
    CY_PDSTACK_DATA_RESET_WAIT_COMPLETION,                 /**< UFP waiting for Data_Reset completion. */
    CY_PDSTACK_DATA_RESET_COMPLETE_DELAY                   /**< DFP waiting to send Data_Reset_Complete message. */
} cy_en_pdstack_data_reset_state_t;

/**
 * @typedef cy_en_pdstack_epr_mode_state_t
 * @brief Enumeration of sub-states associated with EPR_Mode AMS.
 */
typedef enum
{
    CY_PDSTACK_EPR_MODE_STATE_IDLE = 0,                     /**< No EPR mode-related operation pending. */
    CY_PDSTACK_EPR_MODE_STATE_WAIT_ACK,                     /**< Sender waiting for EPR mode acknowledgement. */
    CY_PDSTACK_EPR_MODE_STATE_ACKED,                        /**< Waiting for the next step after sending accept response. */    
    CY_PDSTACK_EPR_MODE_STATE_WAIT_CBL_DSC,                 /**< Waiting for VConn swap and cable discovery completion before sending EPR mode to enter/fail. */
    CY_PDSTACK_EPR_MODE_STATE_WAIT_COMPLETION,              /**< UFP waiting for EPR mode completion. */
    CY_PDSTACK_EPR_MODE_STATE_COMPLETE_DELAY                /**< DFP waiting to send EPR mode enter/fail message. */
} cy_en_pdstack_epr_mode_state_t;

/**
 * @typedef cy_en_pdstack_eprmdo_action_t
 * @brief Enumeration of action fields in EPR enter mode data object.
 */
typedef enum
{
    CY_PDSTACK_EPR_MODE_ENTER = 1,                          /**< Enter cmd. */
    CY_PDSTACK_EPR_MODE_ACK,                                /**< Enter acknowledged. */
    CY_PDSTACK_EPR_MODE_SUCCEEDED,                          /**< Enter succeeded. */    
    CY_PDSTACK_EPR_MODE_FAILED,                             /**< Enter failed. */
    CY_PDSTACK_EPR_MODE_EXIT                                /**< Exit cmd. */
} cy_en_pdstack_eprmdo_action_t;

/**
 * @typedef cy_en_pdstack_eprmdo_data_t
 * @brief Enumeration of data fields in EPR enter mode data object with EPR_MODE_ENTER_FAILED action field.
 */
typedef enum
{
    CY_PDSTACK_EPR_FAIL_UNKNOWN = 0,                         /**< Unknown cause. */
    CY_PDSTACK_EPR_FAIL_EPR_CABLE,                           /**< Cable not EPR capable. */
    CY_PDSTACK_EPR_FAIL_EPR_VCONN,                           /**< Source failed to become VCONN source. */    
    CY_PDSTACK_EPR_FAIL_RDO,                                 /**< EPR mode capable bit not set in RDO. */
    CY_PDSTACK_EPR_FAIL_UNABLE_NOW,                          /**< Unable at this time. */
    CY_PDSTACK_EPR_FAIL_PDO,                                 /**< EPR mode capable bit not set in PDO. */
    CY_PDSTACK_EPR_ENTER_SUCCESS                             /**< Enter succeeded. No failures. */
} cy_en_pdstack_eprmdo_data_t;


/**
 * @typedef cy_en_pdstack_intel_pf_type_t
 * @brief List of Intel TBT/USB platform types in which the EZ-PD(TM) CCG device is used.
 */
typedef enum
{
    CY_PDSTACK_PF_THUNDERBOLT = 0,                         /**< Thunderbolt platforms such as Alpine Ridge or Titan Ridge. */
    CY_PDSTACK_PF_ICE_LAKE,                                /**< Intel IceLake platform. */
    CY_PDSTACK_PF_TIGER_LAKE,                              /**< Intel TigerLake platform. */
    CY_PDSTACK_PF_MAPLE_RIDGE,                             /**< Intel RocketLake + Maple Ridge platform. */
    CY_PDSTACK_PF_METEOR_LAKE,                             /**< Intel MeteorLake platform. */
    CY_PDSTACK_PF_BARLOW_RIDGE                             /**< Intel Barlow Ridge platform. */
} cy_en_pdstack_intel_pf_type_t;

/**
 * @typedef cy_en_pdstack_amd_pf_type_t
 * @brief List of AMD platform types in which the EZ-PD(TM) CCG device is used.
 */
typedef enum
{
    CY_PDSTACK_AMD_PF_RENOIR = 0,                         /**< AMD Renoir-based platform. */
    CY_PDSTACK_AMD_PF_REMBRANDT_A0,                       /**< AMD Rembrandt A0 based platform. */
    CY_PDSTACK_AMD_PF_REMBRANDT_B0,                       /**< AMD Rembrandt B0 based platform. */
    CY_PDSTACK_AMD_PF_PHOENIX                             /**< AMD Phoenix-based platform. */
} cy_en_pdstack_amd_pf_type_t;

/**
 * @typedef cy_en_pdstack_amd_rtmr_type_t
 * @brief List of retimer types which could be used via AMD projects.
 */
typedef enum
{
    CY_PDSTACK_AMD_RETIMER_NONE = 0,                      /**< No retimer. */
    CY_PDSTACK_AMD_RETIMER_PI3DPX1205A,                   /**< Retimer PI3DPX1205A. */
    CY_PDSTACK_AMD_RETIMER_PS8828,                        /**< Retimer PS8828. */
    CY_PDSTACK_AMD_RETIMER_PS8830,                        /**< Retimer PS8830. */
    CY_PDSTACK_AMD_RETIMER_AUTO_PS8828A_OR_PS8830,        /**< Retimer type should be discovered. */
    CY_PDSTACK_AMD_RETIMER_KB800X_B0,                     /**< Retimer KB800X B0. */
    CY_PDSTACK_AMD_RETIMER_KB800X_B1,                     /**< Retimer KB800X B1. */
} cy_en_pdstack_amd_rtmr_type_t;

/**
 * @typedef cy_en_pdstack_pdo_sel_alg_t
 * @brief Algorithm selection for PDO evaluation.
 * Only fixed SRC_PDOs take part in current and voltage algorithms.
 */
typedef enum
{
    CY_PDSTACK_HIGHEST_POWER = 1,                           /**< Algorithm to select a contract with the highest power. */
    CY_PDSTACK_HIGHEST_CURRENT,                             /**< Algorithm to select a contract with the highest current. */
    CY_PDSTACK_HIGHEST_VOLTAGE                              /**< Algorithm to select a contract with the highest voltage. */
} cy_en_pdstack_pdo_sel_alg_t;

/**
 * @brief Interface status codes
 *
 * Enumeration to hold status codes for all EZ-PD(TM) PMG1 interfaces. These values
 * are pre-defined for each interface and should not be modified. To make
 * interface usage easier, the enumeration starts at -2. This allows the
 * success status to have a value of zero. The response code should be
 * incremented by two before sending out on the individual interfaces.
 */
typedef enum cy_en_pdstack_status
{
    CY_PDSTACK_STAT_NO_RESPONSE = -2,          /**< Special status code indicating
                                             no response. */
    CY_PDSTACK_STAT_SUCCESS = 0,               /**< Success status. */
    CY_PDSTACK_STAT_FLASH_DATA_AVAILABLE,      /**< Special status code indicating flash data
                                             availability. */
    CY_PDSTACK_STAT_BAD_PARAM,                 /**< Bad input parameter. */
    CY_PDSTACK_STAT_INVALID_COMMAND = 3,       /**< Operation failed because of invalid command. */
    CY_PDSTACK_STAT_FLASH_UPDATE_FAILED = 5,   /**< Flash write operation failed. */
    CY_PDSTACK_STAT_INVALID_FW,                /**< Special status code indicating invalid firmware */
    CY_PDSTACK_STAT_INVALID_ARGUMENT,          /**< Operation failed because of invalid arguments. */
    CY_PDSTACK_STAT_NOT_SUPPORTED,             /**< Feature not supported. */
    CY_PDSTACK_STAT_INVALID_SIGNATURE,         /**< Invalid signature parameter identified. */
    CY_PDSTACK_STAT_TRANS_FAILURE,             /**< Transaction failure status. */
    CY_PDSTACK_STAT_CMD_FAILURE,               /**< Command failure status */
    CY_PDSTACK_STAT_FAILURE,                   /**< Generic failure status. */
    CY_PDSTACK_STAT_READ_DATA,                 /**< Special status code indicating read data
                                             availability. */
    CY_PDSTACK_STAT_NOT_READY,                 /**< Operation failed because of device/stack is not ready. */
    CY_PDSTACK_STAT_BUSY,                      /**< Operation failed because of device/stack busy status. */
    CY_PDSTACK_STAT_TIMEOUT,                   /**< Operation timed out. */
    CY_PDSTACK_STAT_INVALID_PORT,              /**< Invalid port number. */

    CY_PDSTACK_STAT_INVALID_ID = 0x3E,         /**< Invalid FWCT identity received for signed firmware upgrade. */
    CY_PDSTACK_STAT_INVALID_GUID,              /**< Invalid GUID received for signed firmware upgrade. */
    CY_PDSTACK_STAT_INVALID_VER,               /**< Invalid/older primary firmware version received for signed firmware upgrade. */
    CY_PDSTACK_STAT_OUT_OF_SEQ_CMD,            /**< Command sent is not expected in the current state during the signed firmware upgrade. */
    CY_PDSTACK_STAT_INVALID_FWCT,              /**< Unauthentic FWCT received for the signed firmware upgrade. */
    CY_PDSTACK_STAT_HASH_CMP_FAILED            /**< Hash comparison of the firmware image and hash in FWCT is not matched during the signed firmware upgrade. */
} cy_en_pdstack_status_t;

/** \} group_pdstack_enums */

/**
* \addtogroup group_pdstack_data_structures
* \{
*/

/**
 * @brief Union to hold the PD extended header.
 */
typedef union
{
    uint16_t val;                               /**< Extended header expressed as 2-byte integer value. */

    /** @brief Extended header broken down into respective fields. */
    struct EXTD_HDR_T
    {
        uint16_t dataSize   : 9;                /**< Bits 08:00 - Extended message size in bytes. */
        uint16_t rsvd1      : 1;                /**< Bit     09 - Reserved. */
        uint16_t request    : 1;                /**< Bit     10 - Chunk request. */
        uint16_t chunkNum   : 4;                /**< Bits 14:11 - Chunk number. */
        uint16_t chunked    : 1;                /**< Bit     15 - Chunked message. */
    } extd;                                     /**< Extended header broken down into respective fields. */
} cy_pdstack_extd_hdr_t;

/**
 * @brief PD port status corresponding to the Status Data Block (SSDB)
 * See Table 6-39 of USB PD R3 specification.
 */
typedef struct
{
    uint8_t  intlTemperature;                  /**< Port's internal temperature. 0 if not supported. */
    uint8_t  presentInput;                     /**< Reports current input power status. */
    uint8_t  batteryInput;                     /**< Reports the current battery status. */
    uint8_t  eventFlags;                       /**< Event flags. */
    uint8_t  tempStatus;                       /**< Temperature status. */
    uint8_t  powerStatus;                      /**< Power status. */
    uint8_t  powerStateChange;                 /**< Power state change. */
    uint8_t  dummy;                            /**< Reserved field used for 4-byte alignment. */
} cy_stc_pdstack_pd_power_status_t;

/**
 * @brief PD port status as reported to the embedded controller.
 */
typedef union cy_stc_pdstack_pd_port_status_ec
{
    uint32_t    val;                                    /**< PD status register value. */

    /** @brief Structure containing status bits. */
    struct PD_PORT_STAT
    {
        uint32_t dfltDataRole           : 2;            /**< Bits 01:00 - Default data role. */
        uint32_t dfltDataPref           : 1;            /**< Bit     02 - Preferred data role in case of DRP. */
        uint32_t dfltPowerRole          : 2;            /**< Bits 04:03 - Default power role. */
        uint32_t dfltPowerPref          : 1;            /**< Bit     05 - Preferred power role in case of DRP. */
        uint32_t curDataRole            : 1;            /**< Bit     06 - Current data role. */
        uint32_t reserved0              : 1;            /**< Bit     07 - Reserved. */
        uint32_t curPowerRole           : 1;            /**< Bit     08 - Current power role. */
        uint32_t minState               : 1;            /**< Bit     09 - Whether in Min state (due to GotoMin). */
        uint32_t contractExist          : 1;            /**< Bit     10 - Whether explicit contract exists. */
        uint32_t emcaPresent            : 1;            /**< Bit     11 - EMCA detected or not. */
        uint32_t vconnSrc               : 1;            /**< Bit     12 - Whether CCG is VConn source. */
        uint32_t vconnOn                : 1;            /**< Bit     13 - Whether VConn is ON. */
        uint32_t rpStatus               : 1;            /**< Bit     14 - Current Rp status. */
        uint32_t peRdy                  : 1;            /**< Bit     15 - Whether policy engine is in Ready state. */
        uint32_t ccgSpecRev             : 2;            /**< Bits 17:16 - USB PD revision supported by the EZ-PD(TM) CCG firmware. */
        uint32_t peerPd3Supp            : 1;            /**< Bit     18 - Whether port partner supports PD 3.0. */
        uint32_t peerUnchunkSupp        : 1;            /**< Bit     19 - Whether port partner supports unchunked messages. */
        uint32_t emcaSpecRev            : 2;            /**< Bits 21:20 - USB PD revision supported by cable marker. */
        uint32_t emcaType               : 1;            /**< Bit     22 - EMCA type: Passive=0, Active=1. */
        uint32_t eprStatus              : 1;            /**< Bit     23 - Whether EZ-PD(TM) CCG in EPR mode or not */
        uint32_t eprSnkEnable           : 1;            /**< Bit     24 - EPR sink feature enable*/
        uint32_t eprSrcEnable           : 1;            /**< Bit     25 - EPR source feature enable*/
        uint32_t reserved2              : 6;            /**< Bit  31:26 - Reserved field. */
    } status;                                           /**< PD port status structure. */
} cy_stc_pdstack_pd_port_status_ec_t;

/**
 * @brief Struct to hold response to policy manager.
 * Note: The app_resp_t structure is only used for responses that have a single DO. This may need
 * to get extended if more DOs are to be supported.
 */
typedef struct
{
    cy_pd_pd_do_t             respDo;    /**< Response data object. */
    cy_en_pdstack_app_req_status_t reqStatus; /**< Request status. */
} app_resp_t;

/**
 * @brief Struct to hold response to policy manager.
 */
typedef struct
{
    cy_pd_pd_do_t     respBuf[CY_PD_MAX_NO_OF_DO]; /**< Data objects buffer. */
    uint8_t     doCount;               /**< Data objects count. */
    cy_en_pdstack_vdm_ams_t   noResp;                /**< Response type. */
} vdm_resp_t;

/**
 * @brief Struct to hold PD command buffer.
 * @warning When providing a pointer to the extended data, ensure the original buffer
 * is always 4-byte aligned, i.e, even if 1-byte data is required, 4 bytes should be used to
 * store that data.
 */
typedef struct
{
    cy_en_pd_sop_t       cmdSop;                /**< SOP typ. */
    cy_en_pdstack_extd_msg_t                extdType;              /**< Extended message type. */
    cy_pdstack_extd_hdr_t extdHdr;               /**< Extended header. */
    uint8_t                   noOfCmdDo;           /**< No of data objects including VDM header. */
    uint8_t*                  datPtr;                /**< Data pointer in case of the extended message only.*/
    uint8_t                   timeout;                /**< Timeout value in ms for a response.
                                                       *   If set to zero, the PD stack will not wait for a VDM response
                                                       *   and jumps to the ready state after this buffer has been sent.
                                                       */
    cy_pd_pd_do_t                   cmdDo[CY_PD_MAX_NO_OF_DO];   /**< Command data objects. */
} cy_stc_pdstack_dpm_pd_cmd_buf_t;

/**
 * @brief Structure to hold PD contract information.
 */
typedef struct
{
    uint16_t curPwr;           /**< PD contract current/power. */
    uint16_t maxVolt;          /**< PD contract max voltage in mV. */
    uint16_t minVolt;          /**< PD contract min voltage in mV. */
} cy_stc_pdstack_contract_t;

/**
 * @brief Structure to hold PD contract information passed with APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE
 * event to the application.
 */
typedef struct
{
    cy_pd_pd_do_t              rdo;           /**< RDO associated with the contract. */
    cy_en_pdstack_contract_status_t status;        /**< Status of the contract. */
} cy_stc_pdstack_pd_contract_info_t;

/**
 * @brief Struct to hold a PD packet.
 */
typedef struct
{
    cy_en_pd_sop_t     sop;                         /**< Packet type. */
    uint8_t     len;                                /**< Length in data objects. */
    uint8_t     msg;                                /**< Message code. */
    cy_en_pd_port_role_t     dataRole;              /**< Data role. */
    cy_pd_pd_hdr_t    hdr;                          /**< Message header. */
    cy_pd_pd_do_t     dat[CY_PD_MAX_NO_OF_DO + CY_PD_MAX_NO_OF_EPR_PDO];      /**< Data objects associated with the message. */
} cy_stc_pdstack_pd_packet_t;


/**
 * @brief Structure that encapsulates different timing parameters, therefore, they can be updated by a single API.
 */
typedef struct {
   
    uint8_t  pd2senderRspTimeout;     /**< Sender response timeout period in ms for PD2. */
    uint8_t  pd3senderRspTimeout;     /**< Sender response timeout period in ms for PD3. */
    uint8_t  ccDebouncePeriod;        /**< CC debounce period in ms. */
    uint8_t  errRecovDelay;           /**< Error recovery time period in ms. */
} cy_stc_pdstack_pd_timer_params_t;

/* Forward declarations of structures. */
struct cy_stc_pdstack_context;

/**
 * PD callback prototype.
 * This is a stack internal callback function used by the USB PD protocol
 * layer to send events to the policy engine. The events notified correspond
 * to policy engine events such as hard reset or soft reset received.
 *
 * \param ptrPdStackContext
 * PdStack library context pointer.
 *
 * @param Type of event being notified.
 */
typedef void (*cy_pdstack_pd_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, uint32_t event);

/**
 * @brief DPM PD command callback. This is the type of callback function used by the policy engine
 * to report the results of a command to the application layer.
 *
 * @param port PD port index.
 * @param resp Response code.
 * @param pkt_ptr Pointer to any PD packet associated with the response.
 */
typedef void (*cy_pdstack_dpm_pd_cmd_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, cy_en_pdstack_resp_status_t resp, const cy_stc_pdstack_pd_packet_t* pkt_ptr);

/**
 * @brief Application response callback. This is the type of callback used by the stack to
 * receive application-level responses associated with a PD message such as a SWAP request.
 *
 * @param port PD port index.
 * @param resp Pointer to the structure holding response information.
 */
typedef void (*cy_pdstack_app_resp_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, app_resp_t* resp);

/**
 * @brief VDM response callback. This is the type of callback used by the stack to receive
 * application-level responses to a VDM received from the port partner or cable marker.
 *
 * @param port PD port index.
 * @param resp Pointer to the structure holding response information.
 */
typedef void (*cy_pdstack_vdm_resp_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, vdm_resp_t* resp);

/**
 * @brief Power ready callback. Type of callback used by the stack to receive notification
 * from the power source/sink hardware manager that the requested power transition has
 * been completed.
 *
 * @param port PD port index.
 */
typedef void (*cy_pdstack_pwr_ready_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext);

/**
 * @brief Sink discharge off callback. Callback type used by the stack to receive
 * notification that the sink discharge circuit has been turned OFF.
 *
 * @param port PD port index.
 */
typedef void (*cy_pdstack_sink_discharge_off_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext);

/**
 * @brief Type C command response callback. Type of callback used by the stack to
 * report results of a dpm_typec_command API call to the application layer.
 *
 * @param port PD port index.
 * @param resp Response code.
 */
typedef void (*cy_pdstack_dpm_typec_cmd_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, cy_en_pdstack_dpm_typec_cmd_resp_t resp);

/**
 * @brief Struct to hold the application interface. The application is expected to
 * fill the structure with pointers to functions that use the onboard circuitry to
 * accomplish tasks such as source/sink power turn ON/OFF. All the functions in this
 * structure must be non-blocking and take minimum execution time.
 *
 * @warning The application must check the callback pointer passed by the
 * stack is not NULL.
 */
typedef struct
{
    void (*app_event_handler) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_en_pdstack_app_evt_t evt,              /**< Type of event. */
            const void* dat             /**< Event related data. */
            );                          /**< App event handler callback. */

#if (!CY_PD_SINK_ONLY)
    void (*psrc_set_voltage) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t volt_mV            /**< Target voltage in mV units. */
            );                          /**< Set power source voltage in mV units. */

    void (*psrc_set_current) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t cur_10mA           /**< Expected operating current in 10 mA units. */
            );                          /**< Sets power source current in 10 mA units. */

    void (*psrc_enable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_pwr_ready_cbk_t pwr_ready_handler   /**< Function to be called after power enable. */
            );                          /**< Enable the power supply. The pwr_ready_handler, if not NULL, must be
                                         *   called when the power supply is ready.
                                         */

    void (*psrc_disable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_pwr_ready_cbk_t pwr_ready_handler   /**< Function to be called after power disabled. */
            );                          /**< Disable the power supply. The pwr_ready_handler, if not NULL,
                                         *   must be called when the power supply has been discharged to Vsafe0V. */
#endif /* (!CY_PD_SINK_ONLY) */

    bool (*vconn_enable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint8_t channel             /**< CC channel on which to enable VConn. */
            );                          /**< Turn VCONN supply ON. Return true if VCONN was turned ON. */

    void (*vconn_disable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint8_t channel             /**< CC channel on which to disable VConn. */
            );                          /**< Turn VCONN supply OFF. */

    bool (*vconn_is_present) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Check whether the VConn supply is ON. */

    bool (*vbus_is_present) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t volt,              /**< Expected voltage in mV units. */
            int8_t per                  /**< Allowed voltage deviation in percentage units. */
            );                          /**< Check whether VBus voltage is within the expected range. */

    void (*vbus_discharge_on) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Turn ON the VBUS discharge circuit. */

    void (*vbus_discharge_off) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Turn OFF the VBUS discharge circuit. */

#if (!(CY_PD_SOURCE_ONLY))
    void (*psnk_set_voltage) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t volt_mV            /**< Target voltage in mV units. */
            );                          /**< Sets power sink voltage in mV units. */

    void (*psnk_set_current) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t cur_10mA           /**< Operating current in 10 mA units. */
            );                          /**< Sets power sink current in 10 mA units. */

    void (*psnk_enable) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Enables power sink-related circuitry. */

    void (*psnk_disable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_sink_discharge_off_cbk_t snk_discharge_off_handler  /**< Callback to be called after discharge is done. */
            );                          /**< Disables power sink-related circuitry. */

    void (*eval_src_cap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            const cy_stc_pdstack_pd_packet_t* srcCap, /**< Pointer to list of received source caps. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Evaluates received source caps and provides the RDO to be used
                                             to negotiate the contract. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */

#if (!CY_PD_SINK_ONLY)
    void (*eval_rdo)(
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pd_pd_do_t rdo,                /**< Received RDO object. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Evaluates sink request message. */
#endif /* (!CY_PD_SINK_ONLY) */

    void (*eval_dr_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handles DR swap request received by the port. */
    void (*eval_pr_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handles pr swap request received by the port. */

    void (*eval_vconn_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handles VCONN swap request received by the port. */

    void (*eval_vdm) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            const cy_stc_pdstack_pd_packet_t *vdm,     /**< Pointer to received VDM. */
            cy_pdstack_vdm_resp_cbk_t vdm_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handle VDMs (all structured/unstructured VDMs need to be handled) received
                                             by the port. */
#if ((!(CY_PD_SOURCE_ONLY)) && (!CY_PD_SINK_ONLY))
    void (*eval_fr_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handle FR swap request received by the specified port. */
#endif /* ((!(CY_PD_SOURCE_ONLY)) && (!CY_PD_SINK_ONLY))  */

    uint16_t (*vbus_get_value) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Gets the current VBUS value in mV from the application. */

#if (!CY_PD_SINK_ONLY)
    uint32_t (*psrc_get_voltage) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Gets the expected VBUS value in mV from the application. This is to include any
                                              additional compensation done for drops. */
#endif /* (!CY_PD_SINK_ONLY) */

#if (CY_PD_USB4_SUPPORT_ENABLE)
    void (*eval_enter_usb) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            const cy_stc_pdstack_pd_packet_t *eudo_p, 
            cy_pdstack_app_resp_cbk_t app_resp_handler
            );
#endif /* (!CY_PD_SINK_ONLY) */

#if ((CY_PD_EPR_ENABLE) && (!CY_PD_SINK_ONLY))
    bool (*eval_epr_mode) (
            struct cy_stc_pdstack_context *ptrPdStackContext,                /**< PdStack context. */
            cy_en_pdstack_eprmdo_action_t eprModeState,                     /**< EPR mode response that will be additionally checked. */
            cy_pdstack_app_resp_cbk_t appRespHandler                        /**< Callback to report response through. */
            );                                                              /**< Function to evaluate EPR mode enter request. */
    bool (*send_epr_cap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PdStack context. */
            cy_pdstack_app_resp_cbk_t appRespHandler                        /**< Callback to report response through. */
            );                                                              /**< Function to send EPR capabilities request. */
#endif /* ((CY_PD_EPR_ENABLE) && (!CY_PD_SINK_ONLY)) */
#if (!CY_PD_SINK_ONLY)
    bool (*send_src_info) (
                struct cy_stc_pdstack_context *ptrPdStackContext               /**< PdStack context. */
                );                                                              /**< Function to decide whether to send source info. */
#endif /* (!CY_PD_SINK_ONLY) */
} cy_stc_pdstack_app_cbk_t;


/**
 * @brief Extended Power Range structure. This structure holds a copy of the EPR configuration
 * associated with a port on the EZ-PD(TM) CCG device. Values used for EPR implementation.
 *
 * @warning Initial elements of this structure maps directly to configure table
 * fields and therefore, must not be moved around or changed.
 */
typedef struct
{
    /** Length of epr in the config table. */
    uint8_t  len;
    
    /** EPR SRC support enable/disable. */
    uint8_t  srcEnable;
    
    /** Default EPR SRC PDO enable mask. */
    uint8_t  srcPdoMask;
    
    /** Number of valid EPR SRC PDOs in the table. */
    uint8_t  srcPdoCount;
    
    /** EPR SRC PDO data array. */
    cy_pd_pd_do_t  srcPdo[CY_PD_MAX_NO_OF_EPR_PDO];
    
    /** Needed for bytes alignment with config table. */
    uint8_t  reserved;
    
    /** EPR SNK support enable/disable. */
    uint8_t  snkEnable;
    
    /** Default EPR SNK PDO enable mask. */
    uint8_t  snkPdoMask;
    
    /** Number of valid EPR SNK PDOs in the table. */
    uint8_t  snkPdoCount;
    
    /** EPR SNK PDO data array. */
    cy_pd_pd_do_t  snkPdo[CY_PD_MAX_NO_OF_EPR_PDO];
    
    /** Max min EPR current from the config table or updated at runtime by the EC.
     * */
    uint16_t snkMaxMin[CY_PD_MAX_NO_OF_EPR_PDO];    
}cy_stc_pdstack_epr_t;

/**
 * @brief Enum of the power LED States.
 */
typedef enum
{
    CY_PDSTACK_LED_OFF = 0UL,
    CY_PDSTACK_LED_ON,
    CY_PDSTACK_LED_BLINKING,
    CY_PDSTACK_LED_BREATHING
} cy_en_pdstack_pwr_led_t;

/**
 * @brief PD device policy status structure. This structure holds all of the
 * configuration and status information associated with a port on the EZ-PD(TM) PMG1
 * device. Members of this structure should not be directly modified by any of
 * the application code.
 */
typedef struct
{
    /** Port role: Sink, source, or dual. */
    cy_en_pd_port_role_t portRole;

    /** Default port role: Sink or source. */
    cy_en_pd_port_role_t dfltPortRole;

    /** Type C current level in the source role. */
    uint8_t srcCurLevel;

    /** Power source is connected to a battery. */
    uint8_t isSrcBat;

    /** Power sink is connected to a battery. */
    uint8_t isSnkBat;

    /** USB suspend supported indication. */
    uint8_t snkUsbSuspEn;

    /** USB communication supported indication. */
    uint8_t snkUsbCommEn;

    /** Source PDO count from the config table or updated at runtime by the EC. */
    uint8_t srcPdoCount;

    /** Source PDO mask from the config table or updated at runtime by the EC. */
    uint8_t srcPdoMask;

    /** Sink PDO count from the config table or update at runtime by the EC. */
    uint8_t snkPdoCount;

    /** Sink PDO mask from the config table or update at runtime by the EC. */
    uint8_t snkPdoMask;

    /** Supported Rp values bit mask.
      - Bit 0 => Default Rp supported.
      - Bit 1 => 1.5 A Rp supported.
      - Bit 2 => 3 A Rp supported.
      */
    uint8_t rpSupported;

    /** USB PD supported. */
    bool pdSupport;

    /** Try source/sink control knob. */
    uint8_t trySrcSnk;

    /** Dead battery support control knob. */
    uint8_t dbSupport;

    /** Error recovery control knob.*/
    bool errRecov;

    /** PD port disable flag. */
    uint8_t portDisable;

    /** FRS enable flag. */
    uint8_t frsEnable;

    /** Whether VConn should be retained in ON state. */
    uint8_t vconnRetain;

    /** Reserved words for padding to 4-byte aligned address. */
    uint16_t reserved3[5];

    /** Source PDO loaded from the config table or updated at runtime by the
     * EC. */
    cy_pd_pd_do_t srcPdo[CY_PD_MAX_NO_OF_PDO];

    /** Sink PDO loaded from the config table or updated at runtime by the EC.
     * */
    cy_pd_pd_do_t snkPdo[CY_PD_MAX_NO_OF_PDO];

    /** Max min current from the config table or updated at runtime by the EC.
     * */
    uint16_t snkMaxMin[CY_PD_MAX_NO_OF_PDO];

    /** B29:B20 of the first 5 V fixed source PDO */
    uint8_t srcPdoFlags[2];

    /** B29:B20 of the first 5 V fixed sink PDO */
    uint8_t snkPdoFlags[2];

    /** Flag to indicate chip bootup used to check dead battery. */
    bool volatile bootup;

    /** Flag to indicate dead battery operation. */
    bool volatile deadBat;

    /** Time period for DRP toggling. */
    uint8_t drpPeriod;

    /** Time period for which to stay as an SRC for a DRP device. */
    uint8_t srcPeriod;

    /** Time period for which to stay as an SNK for a DRP device. */
    uint8_t snkPeriod;

    /** Port role when the port moved to the attached state. */
    cy_en_pd_port_role_t roleAtConnect;

    /** Ports are PD-connected indications. */
    bool volatile pdConnected;

    /** PD disabled indication. */
    bool volatile pdDisabled;

    /** Ra present indication. */
    bool volatile raPresent;

    /** BIST carrier mode 2 running. */
    bool volatile bistCm2Enabled;

    /** BIST_Shared_Test_Mode enabled flag. */
    bool volatile bistStmEnabled;

    /** Stores the cable type. */
    cy_en_pdstack_std_vdm_prod_t cblType;

    /** Stores the cable VDM version. */
    cy_en_pdstack_std_vdm_ver_t cblVdmVersion;

    /** Source PDO count in the last sent source cap. */
    uint8_t curSrcPdocount;

    /** Sink PDO count in the last sent sink cap. */
    uint8_t curSnkPdocount;

    /** Flag to indicate cable discovery is waiting for some event. */
    bool cblWait;

    /** Cable discovery state machine state. */
    cy_en_pdstack_pe_cbl_state_t cblState;

    /** Stores number of cable soft reset tries. */
    uint8_t cblSoftResetTried;

    /** Type-C generic FSM state. */
    cy_en_pdstack_typec_fsm_state_t typecFsmState;

    /** Current DPM PD command. */
    cy_en_pdstack_dpm_pd_cmd_t dpmPdCmd;

    /** Indicates DPM PD command is registered. */
    bool dpmPdCmdActive;

    /** Indicates DPM Type C command is registered. */
    bool dpmTypecCmdActive;

    /** DPM Initialized flag. */
    bool dpmInit;

    /** DPM safe disable flag. Used to ensure that the port is disabled
     * correctly after a fault occurrence. */
    bool dpmSafeDisable;

    /** Current DPM Type C command. */
    cy_en_pdstack_dpm_typec_cmd_t dpmTypecCmd;

    /** Live CC status. */
    cy_pd_cc_state_t ccLive;

    /** Current debounced CC status. */
    cy_pd_cc_state_t ccStatus;

    /** Rd status. */
    cy_pd_cc_state_t ccRdStatus;

    /** Current spec revision for SOP Prime/DPrime messages. */
    cy_en_pd_pd_rev_t specRevSopPrimeLive;

    /** Spec revision of the currently connected cable. */
    cy_en_pd_pd_rev_t specRevCbl;

    /** Spec revision of the currently connected peer. */
    cy_en_pd_pd_rev_t specRevPeer;

    /** Mutual unchunk support with the currently connected peer. */
    bool unchunkSupLive;

    /** Unchunk support of the currently connected peer. */
    bool unchunkSupPeer;

    /** Flag to indicate sink will detach on Rp removal instead of VBUS
     * removal. */
    bool snkRpDetachEn;

    /** Flag to indicate current foldback is active */
    bool curFb;

    /** Flag to indicate stack is waiting for App response to a non
     * interruptible AMS */
    cy_en_pdstack_ams_type_t nonIntrResponse;

    /** FRS receive disabled by EC. */
    bool frRxDisabled;

    /** FRS transmit disabled by EC. */
    bool frTxDisabled;

    /** Flag to indicate the a fault condition exists. */
    volatile bool faultActive;

    /** Holds the current state of policy engine (PE). */
    cy_en_pdstack_pe_fsm_state_t peFsmState;

    /** Stores policy engine events. */
    uint32_t volatile peEvt;

    /** Current pd contract. */
    cy_stc_pdstack_contract_t contract;

    /** Alert status */
    cy_pd_pd_do_t alert;

    /** Stores the last received cable VDO. */
    cy_pd_pd_do_t cblVdo;

    /** Whether cable supports alternate modes. */
    bool cblModeEn;

    /** Place holder for src cap start delay in milliseconds */
    uint16_t srcCapStartDelay;

    /** Pointer to DPM PD callback function. */
    cy_pdstack_dpm_pd_cmd_cbk_t dpmPdCbk;

    /** Pointer to DPM Type C callback function. */
    cy_pdstack_dpm_typec_cmd_cbk_t dpmTypecCbk;

    /** Pointer to DPM command buffer. */
    cy_stc_pdstack_dpm_pd_cmd_buf_t* cmdP;

    /** Local DPM command buffer. */
    cy_stc_pdstack_dpm_pd_cmd_buf_t dpmCmdBuf;

    /** Max min current/power of current sink capabilities. */
    uint16_t curSnkMaxMin[CY_PD_MAX_NO_OF_PDO + CY_PD_MAX_NO_OF_EPR_PDO];

    /** Current source PDOs sent in source cap messages. */
    cy_pd_pd_do_t curSrcPdo[CY_PD_MAX_NO_OF_PDO + CY_PD_MAX_NO_OF_EPR_PDO];

    /** Current sink PDOs sent in sink cap messages. */
    cy_pd_pd_do_t curSnkPdo[CY_PD_MAX_NO_OF_PDO + CY_PD_MAX_NO_OF_EPR_PDO];

    /** Stores the current rdo received by source */
    cy_pd_pd_do_t srcCurRdo;

    /** Stores the last rdo received by source */
    cy_pd_pd_do_t srcLastRdo;

    /** Last RDO received (when operating as a source) that resulted in a
     * contract. */
    cy_pd_pd_do_t srcRdo;

    /** Last RDO sent (when operating as a sink) that resulted in a contract.
     * */
    cy_pd_pd_do_t snkRdo;

    /** Selected PDO which resulted in the contract (when sink). */
    cy_pd_pd_do_t snkSelPdo;

    /** Selected PDO which resulted in the contract (when source). */
    cy_pd_pd_do_t srcSelPdo;

    /** Pointer to the current/last source cap message received. May be NULL.
     * Data pointed to by this should not be changed. */
    cy_stc_pdstack_pd_packet_t *srcCapP;

    /** Fields below need to be properly aligned to a 4-byte boundary. */
    uint32_t padval;

    /** Port power status. */
    cy_stc_pdstack_pd_power_status_t portStatus;

    /** Reserved 24 bytes to match the old structure definition for ROM compatibility, the new extSrcCap 
      * moved to cy_stc_pdstack_dpm_ext_status_t. */
    uint8_t reserved_1[24];

    /** Buffer to hold PPS status. */
    uint8_t ppsStatus[CY_PD_EXT_PPS_STATUS_SIZE];

    /** Additional error status for DPM commands. */
    uint8_t dpmErrInfo;

    /** Whether SRC PDOs have been limited due to cable capability. */
    bool    pwrLimited;

    /** Stores the last received active cable VDO #2. */
    cy_pd_pd_do_t cblVdo2;

    /** Reserved 24 bytes to match the old structure definition for ROM compatibility, the new extSnkCap 
      * moved to cy_stc_pdstack_dpm_ext_status_t. */
    uint8_t reserved_2[24];

    /** Temporary variable used for random number generation. */
    uint32_t randBase;

    /** Checks whether to keep Rp at SinkTxNG as a PD 3.0 Source. */
    bool    pd3SrcCcBusy;

    /** Checks whether the solution state allows Type-C transitions */
    bool    isSolnBusy;

#if DPM_DEBUG_SUPPORT
    /** RDO that has been accepted last. */
    cy_pd_pd_do_t activeRdo;

    /** Stores the contract flags. */
    uint8_t contractFlags;

    /** Stores the source PDO flags. */
    uint8_t srcPdoFlags;

    /** Reason for port entering Type-C error recovery. */
    uint8_t errRecovReason;

    /** Reason for port issuing SOP'' soft reset. */
    uint8_t sopdpSoftResetReason;

    /** Reason for port issuing SOP' soft reset. */
    uint8_t soppSoftResetReason;

    /** Reason for port issuing cable reset. */
    uint8_t cableResetReason;

    /** Reason for port issuing a hard reset. */
    uint8_t hardResetReason;

    /** Reason for port issuing a soft reset. */
    uint8_t softResetReason;

    /** Checks whether SOP'' cable controller is present. */
    uint8_t sopdpPresent;

    /* Debug counters  */
    /** Number of connections since power-up. */
    uint8_t  connectionCount;

    /** Number of faults in the current connection. */
    uint8_t  faultCount;

    /** Number of contracts made during current connection. */
    uint8_t  contrNegotiationCount;

    /** Number of messages sent during current connection. */
    uint16_t pdMsgsSent;

    /** Number of messages received during current connection. */
    uint16_t pdMsgsRxd;

#endif /* DPM_DEBUG_SUPPORT */

    /** Flag indicating that PD Revision 3.0 support is enabled. */
    uint8_t rev3En;

    /** Flag indicating that hardware-based DRP toggling is enabled. */
    uint8_t hwDrpToggleEn;

    /** Flag indicating that Try.SRC Try.SNK is disabled. */
    uint8_t trySrcSnkDis;

    /** Flag indicating that FRS as an initial sink is supported. */
    uint8_t frsRxEn;

    /** Flag indicating that FRS as an initial source is supported. */
    uint8_t frsTxEn;

    /** Flag indicating that PPS source is supported. */
    uint8_t ppsSrcEn;

    /** Flag indicating that USB4 messages are supported. */
    uint8_t usb4En;

    /** Flag used Rp Detach detect. */
    bool typecRpDetachDisabled;

    /** Flag for accessory support. */
    bool typecAccessorySuppDisabled;

    /** Number of cable discovery attempts to be made.
     *  Range: 0x00 - 0x14. */
    uint8_t cableDiscCount;

    /** Length of manufacturer information.
     * 0 - For no information.
     * 5 to 26: For valid information. */
    uint8_t mfgLenInfo;

    /** Variable added for padding to a DWORD-aligned address. */
    uint32_t dWordPad0;

    /** Manufacturer name. Null terminated string. Should be 4-byte aligned. */
    uint8_t mfgName[26];

    /** Flag indicating that PPS SINK RDO from EC is supported. */
    uint8_t ppsSnkEn;

    /** PPS SINK RDO from EC */
    cy_pd_pd_do_t ppsSnkRdo;

    /** Number of dpm disable requests active. */
    uint16_t disReqCount;

    /** Response to be sent for each USB PD SWAP command.
     *  - Bits 1:0 => DR_SWAP response.
     *  - Bits 3:2 => PR_SWAP response.
     *  - Bits 5:4 => VCONN_SWAP response.
     *  Allowed values are: 0 = ACCEPT, 1 = REJECT, 2 = WAIT, 3 = NOT_SUPPORTED. */
    uint8_t swapResponse;
 
    /** Option to disable automatic VConn swap from the stack. */
    bool autoVcsDisable;
    
    /** Data reset support. */
    bool dataResetEn;
    
} cy_stc_pdstack_dpm_status_t;

/**
 * @brief PD device latest policy status structure. This structure holds all of the
 * configuration and status information associated with PD3.1 update.
 * Structure should not be directly modified by any of
 * the application code.
 */
typedef struct
{
    /** Buffer to hold extended source caps. */
    uint8_t extSrcCap[CY_PD_EXT_SRCCAP_BUF_SIZE];

    /** Buffer to hold extended sink caps. */
    uint8_t extSnkCap[CY_PD_EXT_SNKCAP_BUF_SIZE];

    /** Source information. */
    cy_pd_pd_do_t  srcInfo;
    
    /** PD revision. */
    cy_pd_pd_do_t  revision;

    /** EPR mode status. */
    bool eprActive;
    
    /** EPR flag to indicate limit SNK PDO to SPR. */
    bool eprToSpr;
    
    /** EPR AVS contract status. */
    bool    eprAvsActive;

    /** EPR AVS mode status. */
    uint8_t eprAvsMode;

    /** EPR source PDO count in the last sent EPR source cap. */
    uint8_t curEprSrcPdoCount;
    
    /** EPR sink PDO count in the last sent EPR sink cap. */
    uint8_t curEprSnkPdoCount;
    
    /** EPR structure. */
    cy_stc_pdstack_epr_t epr;

    /** Power LED state. */
    uint8_t pwrLed;

} cy_stc_pdstack_dpm_ext_status_t;

/**
 * @brief Type-C manager status structure. This structure holds all of the
 * configuration and status information associated with the Type-C manager.
 * Structure should not be directly modified by any of the
 * application code.
 */
typedef struct
{
    /** Flag to indicate the start of the drp random timeout update cycle. */
    uint8_t randUpdt;

    /** Flag to indicate rd debounce reset. */
    bool volatile rdDebounce;

    /** Flag to indicate power source enable is going on. */
    bool psrcEn;

    /** Flag to indicate unexpected vbus detected by the source before applying vbus. */
    bool vbusUnexpected;

    /** Flag to indicate port disable is going on. */
    bool portDisable;

    /** Current period (sink or source). */
    uint8_t curPeriod;

    /** Flag to indicate toggle timer ON. */
    bool volatile toggleOn;

    /** Flag to indicate an attempt to discharge if vbus seen before applying vbus is over. */
    bool volatile discharge;

    /** Flag to indicate try snk tDRPTry timer is running. */
    bool volatile trySnkDeadTime;

    /** Flag to indicate cc status debounced to avoid race condition in going to DeepSleep. */
    bool volatile ccStatusDebounced[2];

    /** Flag to indicate that source disables operation is in progress. */
    bool volatile srcDisableWait;

} cy_stc_pdstack_typec_status_t;

/**
 * @brief Structure to hold PD protocol message IDs and flags. Separate structures need to be
 * maintained for each packet type (SOP, SOP', and SOP'').
 */
typedef struct cy_stc_pdstack_prl_cntrs
{
    /** Message ID to be used on the next transmitted message. */
    uint8_t trMsgId;

    /** Message ID of last received message. */
    uint8_t recMsgId;

    /** Flag that indicates whether any message has been received so far. */
    bool volatile firstMsgRcvd;

} cy_stc_pdstack_prl_cntrs_t;

/**
 * @brief Structure to hold protocol layer status.
 */
typedef struct cy_stc_pdstack_pd_status
{
    /** Callback is used to send notifications to the policy engine. */
    cy_pdstack_pd_cbk_t cbk;

    /** Protocol counters for various packet types. */
    cy_stc_pdstack_prl_cntrs_t  ctrs[CY_PD_MAX_SOP_TYPES];

    /** Buffer used to hold the message to be (or being) transmitted. */
    uint32_t txBuf[CY_PD_MAX_PD_PKT_WORDS];

    /** PD header corresponding to the message being transmitted. */
    uint32_t txHeader;

    /** Buffer used to hold received PD message. Can be extended or not. */
    cy_stc_pd_packet_extd_t rxPacket;

    /** Stores message ID (excluding GoodCRC) of the received message. */
    uint8_t  curRecMsgId;

    /** Stores last received SOP type. */
    cy_en_pd_sop_t lastRcvdSop;

    /** Flag to skip retry on CRCReceiveTimer expiry. */
    bool volatile avoidRetry;

    /** Flag indicating TX state machine is busy. */
    bool volatile txBusy;

    /** Stores the SOP type of message being transmitted. */
    cy_en_pd_sop_t txSop;

    /** Holds the extended header for the message being transmitted. */
    cy_pdstack_extd_hdr_t txExtdHdr;

    /** Flag that indicates that the current message is extended. */
    bool txExtd;

    /** Stores the SOP type of the last transmitted message. */
    cy_en_pd_sop_t lastTxSop;

    /** Stores the message type of message being transmitted. */
    uint8_t txMsgType;

    /** Holds the data object count of the message being transmitted. */
    uint8_t txCount;

    /** Flag indicating BIST test data is enabled. */
    bool bistTestEn;

    /** Type of receive state machine event received from the HAL. */
    uint32_t volatile rxEvt;

    /** Flag indicating RX state machine is busy. */
    bool volatile rxBusy;

    /** Flag indicating the PD Revision 3.0 support is enabled. */
    volatile bool rev3Enable;

    /** Flag indicating that FRS transmit support is enabled. */
    volatile bool frsTxEnable;

    /** Flag indicating that FRS receives support is enabled. */
    volatile bool frsRxEnable;

} cy_stc_pdstack_pd_status_t;

/**
 * @brief Structure to hold the policy engine status.
 */
typedef struct {
    /** Pointer to the received packet. */
    cy_stc_pd_packet_extd_t*    rcvdPtr;
    /** Currently received packet. */
    cy_stc_pd_packet_extd_t rcvdPkt;
    /** Eval VDM local buffer. */
    cy_stc_pdstack_pd_packet_t vdmPkt;
    /** Stores the DPM command response. */
    cy_stc_pdstack_pd_packet_t dpmResp;
    /** Stores the current source cap received. */
    cy_stc_pdstack_pd_packet_t srcCap;
    /** Temporary buffer for sending VDM commands. */
    cy_pd_pd_do_t tempDo;
    /** Temporary buffer for storing APP response. */
    app_resp_t appResp;
    /** VDM response from APP stored here. */
    vdm_resp_t vdmResp;
    /** VCONN swap command buffer. */
    cy_stc_pdstack_dpm_pd_cmd_buf_t  vsBuf;
    /** Cable command buffer. */
    cy_stc_pdstack_dpm_pd_cmd_buf_t  cblBuf;
    /** Hard reset counter. */
    uint8_t  hardResetCount;
    /** Source capability counter. */
    uint8_t   srcCapCount;
    /** CBL dsc id count. */
    uint8_t cblDscIdCount;
    /** Indicates power role swap going on. */
    bool prSwap;
    /** Counts PR swap requests if VCONN swap is pending. */
    uint16_t prSwapCount;
    /** Indicates go to min request received from the source. */
    bool snkGoToMin;
    /** Indicates go to min needs to be sent by the source (requested by dpm). */
    bool srcGoToMin;
    /** Indicates VCONN swap needs to be performed. */
    bool isVconnSwapActive;
    /** Current count of VCONN swap tries. */
    uint8_t vsCount;
    /** Flag to indicate waiting for the response. */
    bool vsWait;
    /** Flag to indicate AMS is continued from the previous transition. */
    pd_ams_type_t amsType;
    /** Stores send message sop type. */
    cy_en_pd_sop_t sendMsgSop;
    /** Stores send message class. */
    cy_en_pdstack_pd_msg_class_t sendMsgClass;
    /** Sends message type. */
    uint8_t sendMsgType;
    /** Sends message count. */
    uint8_t sendMsgCount;
    /** Sends extended header. */
    cy_pdstack_extd_hdr_t sendextdhdr;
    /** Sends message timeout. */
    uint16_t sendMsgTimeout;
    /** Sends message data pointer. */
    void* sendMsgPtr;
    /** Stores expected message class. */
    cy_en_pdstack_pd_msg_class_t expMsgClass;
    /** Expected message mask. */
    uint32_t expMsgMask;
    /** Expected message length. */
    uint16_t expMsgLen;
    /** Temporary buffer for APP req response. */
    cy_en_pdstack_app_req_status_t appReqRslt;
    /** Temporary contract status. */
    cy_stc_pdstack_contract_t contract;
    /** Temporary rdo sent by the sink. */
    cy_pd_pd_do_t snkRdo;
    /** Temporary selected pdo (among source cap) by the sink. */
    cy_pd_pd_do_t snkSelPdo;
    /** Temporary selected pdo (among source cap) of the source. */
    cy_pd_pd_do_t srcSelPdo;
    /** Temporary store for the status of app swap complete event. */
    cy_en_pdstack_app_req_status_t swapEvtResp;
    /** Temporary store for app PD contract event status. */
    cy_stc_pdstack_pd_contract_info_t contractEvtResp;
    /** Temporary variable to hold sop type. */
    cy_en_pd_sop_t sopType;
    /** Flag to indicate PSHardResetDelay required. */
    bool psHrDelay;
    /** Flag determines srccap start debounce in progress. */
    bool sendSrcCapDebounce;
    /** Store received/sent swap type. */
    cy_en_pd_ctrl_msg_t swapType;

    /** Flag to indicate spec rev has been determined. */
    bool specRevDetermined;

    /** Flag to indicate spec rev has been determined. */
    bool chunkDetermined;

    /** Temporary variable to hold extended message type to send. */
    cy_en_pdstack_extd_msg_t autoExtdMsgType;

    /** Temporary store AMS type. */
    cy_en_pdstack_ams_type_t amsTypeTemp;

    /** Flag to indicate waiting for the response. */
    bool getSnkCapWait;
    /** Indicates get sink cap needs to be performed. */
    bool isGetSnkCapActive;
    /** Current count of get sink cap tries. */
    uint8_t getSnkCapCount;
    /** Stores USB4 data reset state. */
    uint8_t usb4DataResetState;

    /** Flag indicating PSRC enable call has been delayed. */
    bool pePsrcEnDelayed;

    /** Flag that indicates that hard reset sending is pending. */
    volatile bool peSendHrPending;
   
    /** Buffer that holds EPR enter mode response data object. */
    cy_pd_pd_do_t eprEnterRespDo;
    
    /** Variable to hold the EPR state. */
    uint8_t eprState;
    
    /** Flag that indicates whether EPR multi-messages are active. */
    bool eprMultiMessageFlag;
    
    /** Flag that indicates whether to send PDO in SPR. */
    bool eprSendcapInSpr;
    
    /** Buffer that holds the EPR sink extended messages/responses. */
    uint8_t eprSnkExtdChunkBuffer[26];

    /** EPR chunked message count. */
    uint8_t eprChunkCount;

    /** PD command buffer to send command from policy engine. */
    cy_stc_pdstack_dpm_pd_cmd_buf_t peCmdBuf;
} cy_stc_pdstack_pe_status_t;

/**
 * @brief Structure to hold device policy manager parameters.
 */
typedef struct
{
    /** Default current setting in 10 mA unit (3 A is represented as 300). */
    uint16_t defCur;

    /** The tTypeCSnkWaitCap period to be used in ms. */
    uint16_t typeCSnkWaitCapPeriod;

    /** Delay to be provided (in ms) between MUX enable and VBus turning ON. */
    uint16_t muxEnableDelayPeriod;

    /** Flag for default cable capabilities. */
    uint16_t dpmDefCableCap;

    /** Flag for holding the cc termination current level for an audio accessory. */
    cy_en_pd_rp_term_t dpmRpAudioAcc;

    /** Variable holding the sink wait cap period. */
    uint16_t dpmSnkWaitCapPeriod;

} cy_stc_pdstack_dpm_params_t;

/**
 * @brief Structure to hold the RTOS interface. The application is expected to
 * fill the structure with pointers to RTOS wrapper functions.
 *
 * @warning The application must check the callback pointer passed by the
 * stack is not NULL.
 */
typedef struct
{
    int32_t (*dpm_rtos_evt_give) (
            struct cy_stc_pdstack_context *ptrPdStackContext      /**< PD port index. */
            );                          /**< DPM event gives handler callback. */

    int32_t (*dpm_rtos_evt_take) (
            struct cy_stc_pdstack_context *ptrPdStackContext,     /**< PD port index. */
            uint32_t waitTick                                    /**< Wait tick count. */
            );                            /**< DPM event takes handler callback. */
} cy_stc_pdstack_rtos_context_t;


/**
 * @brief Structure to PDSTACK middleware context information.
 */
typedef struct cy_stc_pdstack_context
{
    /** USB PD port index. **/
    uint8_t port;

    /** PD device policy configuration and status structure. */
    cy_stc_pdstack_dpm_status_t dpmStat;

    /** PD device policy configuration and status structure. */
    cy_stc_pd_dpm_config_t dpmConfig;

    /** Pointer to the application callback structure. */
    cy_stc_pdstack_app_cbk_t *ptrAppCbk;

    /** Protocol layer status structure. */
    cy_stc_pdstack_pd_status_t pdStat;

    /** Policy engine status structure. */
    cy_stc_pdstack_pe_status_t peStat;

    /** Type-C status. */
    cy_stc_pdstack_typec_status_t typecStat;

    /** Pointer to the port configuration structure. */
    const cy_stc_pdstack_port_cfg_t *ptrPortCfg;

    /** Pointer to the USB PD PDL context information. */
    cy_stc_usbpd_context_t *ptrUsbPdContext;

    /** Structure for the various parameters that dpm update functions
     * are updating. */
    const cy_stc_pdstack_dpm_params_t *ptrDpmParams;

    /** PdStack Timer Context */
    cy_stc_pdutils_sw_timer_t *ptrTimerContext;

    /** Pointer to the RTOS callback function structure. */
    cy_stc_pdstack_rtos_context_t *ptrRtosContext;

    /** PD device policy configuration and status structure having PD 3.1 
     *  updated parameters. */
    cy_stc_pdstack_dpm_ext_status_t dpmExtStat;

    /** Pointer to the AltMode context. */
    void *ptrAltModeContext;

    /** Pointer to the HPI context. */
    void *ptrHpiContext;

    /** Pointer to the cy_stc_pdstack_pd_timer_params_t structure. */
    cy_stc_pdstack_pd_timer_params_t *ptrPdTimerParams;
    
    /** Current sender response timer period. */
    uint8_t  senderRspTimeout;

    /** Source recovery timer period. */
    uint16_t srcRecoverTime;

    /** Stores the cable VDM minor version. */
    cy_en_pdstack_std_minor_vdm_ver_t cblVdmMinVersion;

} cy_stc_pdstack_context_t;

/**
 * @brief Structure used to fetch the PD stack configuration supported by the library
 * that is currently in use.
 */
typedef struct
{
    bool sourceOnly;          /**<
                                * - True: CY_PD_SOURCE_ONLY macro is enabled.
                                * - False: CY_PD_SOURCE_ONLY macro is disabled.
                                */
    bool pdRev3;             /**<
                                * - True: CY_PD_REV3_ENABLE macro is enabled.
                                * - False: CY_PD_REV3_ENABLE macro is disabled.
                                */
    bool frsRx;               /**<
                                * - True: FRS receive is enabled.
                                * - False: FRS receive is disabled.
                                */
    bool frsTx;               /**<
                                * - True: FRS transmit is enabled.
                                * - False: FRS transmit is disabled.
                                */
} cy_pd_stack_conf_t;

/** \} group_pdstack_data_structures */

#endif /* CY_PDSTACK_COMMON_H */

/* [] END OF FILE */
