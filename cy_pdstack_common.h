/***************************************************************************//**
* \file cy_pdstack_common.h
* \version 1.10
*
* Provides Common Header File of the PDStack middleware.
*
********************************************************************************
* \copyright
* Copyright 2021, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PDSTACK_COMMON_H)
#define CY_PDSTACK_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "cy_pdstack_port_config.h"
#include "cy_usbpd_defines.h"
#include "cy_usbpd_common.h"
#include "cy_sw_timer.h"

/**
********************************************************************************
* \mainpage PDStack Middleware Library
*
* The PDStack middleware implements state machines defined in the **Universal
* Serial Bus Type-C Cable and Connector Specification** and the **Universal
* Serial Bus Power Delivery Specification**. The middleware provides a set of
* Device Policy Manager (DPM) APIs through which the application can
* initialize, monitor and configure the middleware operation.
*
* The PDStack middleware operates on the top of the USBPD driver included in
* the MTB PDL CAT2(mtb-pdl-cat2) Peripheral Driver Library
*
* <b>Features:</b>
* 1. Support Type-C port configuration and connection detection in the Sink Role
* 2. Support USBPD Communication with the following features:
*    * USB PD revision 3.0 compliance
*    * USB PD revision 2.0 compliance (backward compatibility)
*    * Power contract negotiation in Sink Role
*    * Ability to respond to SOP and SOP' messages to support designs with a
*    Captive USB-C cable.
* 3. Support configuration of the port roles and responses through the EZ-PD
* Configurator.
* 4. Support Device Policy Manager APIs to initialize, configure, monitor and
* control the port operation; as well as to send/receive USBPD messages
* associated with Alternate Mode discovery, Authentication, Firmware update
* etc.
*
********************************************************************************
* \section section_pdstack_general_description General Description
********************************************************************************
*
* Include cy_pdstack_common.h, cy_pdstack_dpm.h, cy_pdstack_port_config.h to
* get access to all functions and other declarations in this library. See the
* \ref section_pdstack_quick_start to start using the PDStack.
*
* Refer to the \ref section_pdstack_toolchain section for compatibility
* information.
*
* Refer to the \ref section_pdstack_changelog section for change history.
*
* PDStack operates on the top of the usbpd driver. The usbpd driver has
* some prerequisites for proper operation.
* Refer to the "USBPD (USB Power Delivery)" section of the MTB PDL CAT2(mtb-pdl-cat2)
* Peripheral Driver Library API Reference Manual.
* Also, refer to the \ref section_pdstack_miscellaneous section for
* the different PDStack middleware restrictions and limitations.
*
* The PDStack Middleware is released in the form of pre-compiled libraries. Two
* library variants with different feature set to help limit the memory
* footprint of the stack in applications are released.
* 1. pmg1_pd3_snk_lite - Library with support for USB Type-C sink operation and
* USB-PD Revision 3.0 messaging.
* 2. pmg1_pd2_snk_lite - Library with support for USB Type-C sink operation and
* USB-PD Revision 2.0 messaging. Using this library will reduce the flash
* (code) memory usage by the application.
*
********************************************************************************
* \section section_pdstack_quick_start Quick Start Guide
********************************************************************************
*
* PDStack middleware can be used in various Development
* Environments such as ModusToolbox, MBED, etc. Refer to the
* \ref section_pdstack_toolchain section.
*
* The below steps describe the simplest way of enabling the PDStack
* middleware in the application.
*
* 1. Open/Create an application where to add PDStack function
*
* 2. Add the PDStack middleware to your project. This quick start guide
* assumes that the environment is configured to use the MTB CAT2 Peripheral
* Driver Library(PDL) for development and the PDL is included in the project.
* If you are using the ModusToolbox development environment select the
* application in the Project Explorer window and select the PDStack Middleware
* in the Library Manager.
*
* 3. Include cy_pdstack_common.h, cy_pdstack_dpm.h to get access to all
* functions and other declarations in this library.
*    \snippet pdstack/main.c snippet_configuration_include
*
* 4. Define the following data structures required by the PDStack Middleware:
*    * USB-C port configuration
*    \snippet pdstack/main.c snippet_configuration_data
*    * Device Policy Manager parameters
*    \snippet pdstack/main.c snippet_configuration_dpm
*    * Register application callback functions
*    \snippet pdstack/main.c snippet_configuration_app_cbk
* The PDStack library uses these set of callbacks registered by the application
* to perform board specific tasks such as turning the consumer power path
* ON/OFF, identifying the optimal source power profile to be used for charging
* etc. The library also provides notification of various connection and PD
* policy state changes, so that the rest of the system can be configured as
* required.
*
* 5. Initialize the PDStack middleware once at the start:
*    \snippet pdstack/main.c snippet_configuration_dpm_init
*
* 6. Start the device policy manager operation.
*    \snippet pdstack/main.c snippet_configuration_dpm_start
*    This initializes the USB-PD block to detect Type-C connection state
*    changes and USB-PD messages and notify the application through callback
*    functions.
*
* 7. Invoke Cy_PdStack_Dpm_Task function from the main processing loop of the
* application to handle the device policy tasks for each PD Port.
*    \snippet pdstack/main.c snippet_configuration_dpm_task
*
********************************************************************************
* \section section_pdstack_configuration_considerations Configuration Considerations
********************************************************************************
*
* This section consists of instructions of how to configure, and use
* the PDStack Middleware in a design.
* 1. After selecting the PDStack middleware through the library manager, the
* user can select the library variant by editing the Code Example Makefile:
*    * Add 'PMG1_PD3_SNK_LITE' to the COMPONENTS for using the pmg1_pd3_snk_lite library
*    * Add 'PMG1_PD2_SNK_LITE' to the COMPONENTS for using the pmg1_pd2_snk_lite library
* 2. Configure the USB-C port properties
*    * The USB-C port properties are controlled with the \ref
*    cy_stc_pdstack_port_cfg_t structure. Please see its description to learn
*    about the parameters and values.
*    * Use the EZ-PD Configurator to configure the properties of the USB-C port
*    including port role and the default response to various USB-PD messages.
*    Refer to step 4 of the \ref section_pdstack_quick_start guide for the
*    reference configuration data.
*
* Refer to the \ref section_pdstack_miscellaneous for the existing
* restrictions.
*
********************************************************************************
* \section section_pdstack_miscellaneous Limitations and Restrictions
********************************************************************************
* 1. This version of the PDStack Middleware supports the operation of a USB-C
* port as only a power sink/data UFP.
*
********************************************************************************
* \section section_pdstack_toolchain Supported Software and Tools
********************************************************************************
*
* This version of the PDStack Middleware was validated for the compatibility
* with the following software and tools:
*
* <table class="doxtable">
*   <tr>
*     <th>Software and Tools</th>
*     <th>Version</th>
*   </tr>
*   <tr>
*     <td>ModusToolbox Software Environment</td>
*     <td>2.3</td>
*   </tr>
*   <tr>
*     <td>mtb-pdl-cat2</td>
*     <td>1.1.0</td>
*   </tr>
*   <tr>
*     <td>GCC Compiler</td>
*     <td>9.3.1</td>
*   </tr>
*   <tr>
*     <td>IAR Compiler</td>
*     <td>8.42.2</td>
*   </tr>
*   <tr>
*     <td>Arm Compiler 6</td>
*     <td>6.13</td>
*   </tr>
* </table>
*
********************************************************************************
* \section section_pdstack_changelog Changelog
********************************************************************************
*
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td rowspan="4">1.10</td>
*     <td>Updated Type-C Error Recovery period to 250 ms.</td>
*     <td>PD compliance specification updates.</td>
*   </tr>
*   <tr>
*     <td>Updated the size of the status extended message to 7 bytes.
*     <td>PD compliance specification updates.</td>
*   </tr>
*   <tr>
*     <td>USBPD wakeup interrupt was incorrectly getting disabled as part of the Software timer operation. This has been corrected.
*     <td>Defect Fixes.</td>
*   </tr>
*   <tr>
*     <td>API description updated.</td>
*     <td>Documentation update and clarification.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial Version</td>
*     <td></td>
*   </tr>
* </table>
*
********************************************************************************
* \section section_pdstack_more_information More Information
********************************************************************************
*
* For more information, refer to the following documents:
*
* * <a href="https://www.cypress.com/products/modustoolbox-software-environment">
*      <b>ModusToolbox Software Environment, Quick Start Guide, Documentation,
*         and Videos</b>
*   </a>
*
* * <a href="http://www.cypress.com/an232553">
*      <b>AN232553 Getting Started with PMG1 MCU on ModusToolbox</b>
*   </a>
*
* * <a href="http://www.cypress.com/an232565">
*      <b>AN232565 EZ-PD PMG1 Hardware Design Guidelines and Checklist</b>
*   </a>
*
* * <a href="https://infineon.github.io/mtb-pdl-cat2/pdl_api_reference_manual/html/index.html">
*   <b>PDL API Reference</b></a>
*
* * <a href="https://www.cypress.com/documentation/technical-reference-manuals/pmg1-family-pmg1-s0-architecture-technical-reference">
*      <b>PMG1-S0 Architecture Technical Reference Manual</b>
*   </a>
* * <a href="https://www.cypress.com/documentation/technical-reference-manuals/pmg1-family-pmg1-s0-registers-technical-reference-manual">
*      <b>PMG1-S0 Technical Reference Manual</b>
*   </a>
* * <a href="https://www.cypress.com/documentation/technical-reference-manuals/pmg1-family-pmg1-s1-architecture-technical-reference">
*      <b>PMG1-S1 Architecture Technical Reference Manual</b>
*   </a>
* * <a href="https://www.cypress.com/documentation/technical-reference-manuals/pmg1-family-pmg1-s1-registers-technical-reference-manual">
*      <b>PMG1-S1 Technical Reference Manual</b>
*   </a>
* * <a href="https://www.cypress.com/documentation/technical-reference-manuals/pmg1-family-pmg1-s2-architecture-technical-reference">
*      <b>PMG1-S2 Architecture Technical Reference Manual</b>
*   </a>
* * <a href="https://www.cypress.com/documentation/technical-reference-manuals/pmg1-family-pmg1-s2-registers-technical-reference-manual">
*      <b>PMG1-S2 Technical Reference Manual</b>
*   </a>
*
* * <a href="http://www.cypress.com/ds231596">
*      <b>PMG1-S0 Datasheet</b>
*   </a>
* * <a href="http://www.cypress.com/ds231597">
*      <b>PMG1-S1 Datasheet</b>
*   </a>
* * <a href="http://www.cypress.com/ds231598">
*      <b>PMG1-S2 Datasheet</b>
*   </a>
*
* * <a href="http://www.cypress.com">
*      <b>Cypress Semiconductor</b>
*   </a>
*
* \note
* The links to the other software component's documentation (middleware and PDL)
* point to GitHub to the latest available version of the software.
* To get documentation of the specified version, download from GitHub and unzip
* the component archive. The documentation is available in
* the <i>docs</i> folder.
*
********************************************************************************
*
* \defgroup group_pdstack_macros Macros
* \brief
* This section describes the PDStack Macros.
*
* \defgroup group_pdstack_functions Functions
* \brief
* This section describes the PDStack Function Prototypes.
*
* \defgroup group_pdstack_data_structures Data Structures
* \brief
* This section describes the data structures defined by the PDStack.
*
* \defgroup group_pdstack_enums Enumerated Types
* \brief
* This section describes the enumeration types defined by the PDStack.
*
*/

/*******************************************************************************
*                              Type Definitions
*******************************************************************************/

/* Provide default values for feature selection macros where not already defined. */
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

/**
* \addtogroup group_pdstack_macros
* \{
*/

/** Number of un-acknowledged Source CAP messages used to determine if the device connected is PD Capable or not. */
#define CY_PD_MAX_SRC_CAP_TRY                     (6u)

/** Mask for the give-back supported bit in the snkPdoMinMaxCur field of cy_stc_pdstack_port_cfg_t. */
#define CY_PD_GIVE_BACK_MASK                      (0x8000u)

/** Mask to extract the actual min/max current value from the snkPdoMinMaxCur field of cy_stc_pdstack_port_cfg_t. */
#define CY_PD_SNK_MIN_MAX_MASK                    (0x3FFu)

/** Maximum retries of Source Capability messages. */
#define CY_PD_MAX_SRC_CAP_COUNT                   (50u)

/** Maximum hard reset retry count. */
#define CY_PD_MAX_HARD_RESET_COUNT                (3u)

/** Maximum number of cable discovery DISCOVER_IDENTITY messages that should be sent out. */
#define CY_PD_MAX_CBL_DSC_ID_COUNT                (20u)

/** Number of PR_SWAP messages for which DUT sends a WAIT response to allow VConn_SWAP completion. */
#define CY_PD_MAX_PR_SWAP_WAIT_COUNT              (2u)

/** Maximum number of DOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_DO                        (7u)

/** Maximum number of PDOs in a packet. Limited by PD message definition. */
#define CY_PD_MAX_NO_OF_PDO                       (CY_PD_MAX_NO_OF_DO)

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

/** Index of Request data object in a received message. */
#define CY_PD_RDO_IDX                             (0u)

/** Maximum extended message size in bytes. */
#define CY_PD_MAX_EXTD_PKT_SIZE                   (260u)

/** Maximum extended message 32-bit words. Each word is 32 bit. */
#define CY_PDSTACK_MAX_EXTD_PKT_WORDS                  (65u)

/** Maximum legacy Extended message size in bytes. */
#define CY_PD_MAX_EXTD_MSG_LEGACY_LEN             (26u)

/** Maximum message id value in PD Header. */
#define CY_PD_MAX_MESSAGE_ID                      (7u)

/** Max SOP types excluding hard reset, cable reset, SOP_PDEBUG and SOP_DPDEBUG. */
#define CY_PD_MAX_SOP_TYPES                       (3U)

/** Standard SVID defined by USB-PD specification. */
#define CY_PD_STD_SVID                            (0xFF00UL)

/** Displayport SVID defined by VESA specification. */
#define CY_PD_DP_SVID                             (0xFF01UL)

/** Thunderbolt SVID defined by Intel specification. */
#define CY_PD_TBT_SVID                            (0x8087UL)

/** UFP Supports Non-Physical Alternate Mode UFP VDO1 mask. */
#define CY_PD_UFP_NON_PH_ALT_MODE_SUPP_MASK       (0x4u)

/** Cypress VID defined by Cypress for field upgrades. */
#define CY_PD_CY_VID                              (0x04B4UL)

/** Position of VDM version field in structured VDM header. */
#define CY_PD_STD_VDM_VERSION_IDX                 (13u)

/** VDM version 2.0. Used under USB-PD Revision 3.0. */
#define CY_PD_STD_VDM_VERSION_REV3                (1u)

/** VDM version 1.0. Used under USB-PD Revision 2.0. */
#define CY_PD_STD_VDM_VERSION_REV2                (0u)

/** Default VDM version used. Corresponds to VDM version 1.0. */
#define CY_PD_STD_VDM_VERSION                     (0u)

/** Maximum voltage allowed when PS_RDY is received during a SNK to SRC PR_SWAP. This is set to 3.0 V. */
#define CY_PD_VSAFE_0V_PR_SWAP_SNK_SRC            (3000u)

/** Maximum voltage allowed at the end of a Hard Reset when PMG1 is SNK. This is set to 3.0 V. */
#define CY_PD_VSAFE_0V_HARD_RESET                 (3000u)

/** Voltage unit used in PDOs. */
#define CY_PD_VOLT_PER_UNIT                    (50u)

/** Voltage unit (mV) used in PPS PDOs. */
#define CY_CY_PD_VOLT_PER_UNIT_PPS                (100u)

/** Multiplier used to convert from current unit used in other PDOs to that used in PPS PDO/RDO. */
#define CY_PD_CURRENT_PPS_MULTIPLIER           (5u)

/** VBus current usage = 0 A. */
#define CY_PD_ISAFE_0A                            (0u)

/** VBus current usage = 0.5 A. */
#define CY_PD_ISAFE_DEF                           (50u)

/** VBus current usage = 0.9 A. */
#define CY_PD_I_0P9A                              (90u)

/** VBus current usage = 1.0 A. */
#define CY_PD_I_1A                                (100)

/** VBus current usage = 1.5 A. */
#define CY_PD_I_1P5A                              (150)

/** VBus current usage = 2.0 A. */
#define CY_PD_I_2A                                (200)

/** VBus current usage = 3.0 A. */
#define CY_PD_I_3A                                (300)

/** VBus current usage = 4.0 A. */
#define CY_PD_I_4A                                (400)

/** VBus current usage = 5.0 A. */
#define CY_PD_I_5A                                (500)

/** Current unit used in PDOs. */
#define CY_PD_CUR_PER_UNIT                     (10u)

/** Number of VBus checks used to detect detach as sink. */
#define CY_PD_SNK_DETACH_VBUS_POLL_COUNT          (5u)

/** Minimum DRP toggling period, in ms. See Table 4-16 of the Type-C spec Rev1. */
#define CY_PD_DRP_TOGGLE_PERIOD                   (75u)

/** Minimum percentage of DRP period for a source. See Table 4-16 of the Type-C spec Rev1. */
#define CY_PD_SRC_DRP_MIN_DC                      (30)

/** Type-C state machine inactive mode. */
#define CY_PD_TYPEC_FSM_NONE                      (0x00000000u)

/** Type-C state machine active mode. */
#define CY_PD_TYPEC_FSM_GENERIC                   (0x00000001u)

/** Minimum HPD receiver timer period in ms. */
#define CY_PD_HPD_RX_ACTIVITY_TIMER_PERIOD_MIN    (5u)

/** Maximum HPD receiver timer period in ms. */
#define CY_PD_HPD_RX_ACTIVITY_TIMER_PERIOD_MAX    (105u)

/** PD No response timer period in ms. See Section 6.5.7 of USBPD Spec Rev2 v1.2 */
#define CY_PD_NO_RESPONSE_TIMER_PERIOD         (5000u)

/** tVConnStable delay required by cable marker to power up. */
#define CY_PD_CBL_POWER_UP_TIMER_PERIOD        (55u)

/** Cable discovery timer period in ms. See Section 6.5.15 of USBPD Spec Rev2 v1.2 */
#define CY_PD_CBL_DSC_ID_TIMER_PERIOD          (49u)

/** Cable discovery start period in ms. See Section 6.5.15 of USBPD Spec Rev2 v1.2 */
#define CY_PD_CBL_DSC_ID_START_TIMER_PERIOD    (43u)

/** Cable delay timer period in ms. See Section 6.5.14 of USBPD Spec Rev2 v1.2 */
#define CY_PD_CBL_DELAY_TIMER_PERIOD           (2u)

/** Period of timer used internally by stack to prevent PHY lockup in TX state. */
#define CY_PD_PHY_BUSY_TIMER_PERIOD            (15u)

/** Hard reset transmit timer period in ms. See Section 6.3.13 of USBPD Spec Rev2 v1.2 */
#define CY_PD_HARD_RESET_TX_TIMER_PERIOD       (20u)

/** This timer used by stack to do auto retry VCONN swap before PR swap (if DUT is sink).
 * Minimum gap between VCONN swap request shall be a minimum 100ms, to be safe 110ms
 * is used.
 */
#define CY_PD_VCONN_SWAP_INITIATOR_TIMER_PERIOD (110u)

/** Delay between VConn Swap attempts when 5V supply source is not present. */
#define CY_PD_VCONN_SWAP_INITIATOR_DELAY_PERIOD (500u)

/** VBus ON timer period in ms. See Table 7-22 of USBPD Spec Rev2 v1.2 */
#define CY_PD_VBUS_TURN_ON_TIMER_PERIOD        (275u)

/** VBus OFF timer period in ms. See Table 7-22 of USBPD Spec Rev2 v1.2 */
#define CY_PD_VBUS_TURN_OFF_TIMER_PERIOD       (625u)

/** Source transition timer period in ms. See Section 6.5.6.1 of USBPD Spec Rev2 v1.2 */
#define CY_PD_PS_SRC_TRANS_TIMER_PERIOD        (400u)

/** Source off timer period in ms. See Section 6.5.6.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_PS_SRC_OFF_TIMER_PERIOD          (900u)

/** Source on timer period in ms. See Section 6.5.6.3 of USBPD Spec Rev2 v1.2 */
#define CY_PD_PS_SRC_ON_TIMER_PERIOD           (450u)

/** Sink transition period in ms. See Section 6.5.6.1 of USBPD Spec Rev2 v1.2 */
#define CY_PD_PS_SNK_TRANSITION_TIMER_PERIOD   (500u)

/** Source Recover timer period in ms. See Section 7.6.1 of USBPD Spec Rev2 v1.2 */
#define CY_PD_SRC_RECOVER_TIMER_PERIOD         (800u)

/** Sender response timeout period in ms. See Section 6.5.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_SENDER_RESPONSE_TIMER_PERIOD     (27u)

/** Receiver response timeout period in ms. See Section 6.5.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_RECEIVER_RESPONSE_TIMER_PERIOD   (15u)

/** Sink wait cap period in ms. See Section 6.5.4.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_SINK_WAIT_CAP_TIMER_PERIOD       (400u)

/** Source cap timer period in ms. See Section 6.5.4.1 of USBPD Spec Rev2 v1.2 */
#define CY_PD_SRC_CAP_TIMER_PERIOD             (180u)

/** Source start (during PR_SWAP) period in ms. See Section 6.5.9.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_SWAP_SRC_START_TIMER_PERIOD      (55u)

/** Source voltage transition timer period in ms. See Table 7-22 of USBPD Spec Rev2 v1.2 */
#define CY_PD_SOURCE_TRANSITION_TIMER_PERIOD   (28u)

/** VConn off timer period in ms. See Section 6.5.13 of USBPD Spec Rev2 v1.2 */
#define CY_PD_VCONN_OFF_TIMER_PERIOD           (25u)

/** VConn on timer period in ms. */
#define CY_PD_VCONN_ON_TIMER_PERIOD            (100u)

/** Duration for which the UFP discharges VConn during Data_Reset sequence. */
#define CY_PD_UFP_VCONN_DISCHARGE_DURATION     (10u)

/** VConnSourceDischarge timer period. */
#define CY_PD_VCONN_SRC_DISC_TIMER_PERIOD      (200u)

/** tVConnReapplied period from USB-PD specification. */
#define CY_PD_VCONN_REAPPLIED_TIMER_PERIOD     (18u)

/** Time between Data_Reset is accepted and Data_Reset_Complete message has to be sent. */
#define CY_PD_DATA_RESET_TIMER_PERIOD          (220u)

/** Data_Reset_Complete timeout period. */
#define CY_PD_DATA_RESET_TIMEOUT_PERIOD        (250u)

/** Delay applied before DFP sends Data_Reset_Complete message. */
#define CY_PD_DATA_RESET_COMPLETION_DELAY      (225u)

/** Period of VConn monitoring checks done internally. */
#define CY_PD_VCONN_TURN_ON_TIMER_PERIOD       (10u)

/** This timer is the delay between PD startup and sending cable Discover ID request
 * to ensure cable is ready to respond. */
#define CY_PD_CBL_READY_TIMER_PERIOD           (50u)

/** VDM response timer period in ms. See Section 6.5.12.1 of USBPD Spec Rev2 v1.2 */
#define CY_PD_VDM_RESPONSE_TIMER_PERIOD        (27u)

/** Enter mode response timeout period in ms. See Section 6.5.12.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_VDM_ENTER_MODE_RESPONSE_TIMER_PERIOD (45u)

/** Exit mode response timeout period in ms. See Section 6.5.12.3 of USBPD Spec Rev2 v1.2 */
#define CY_PD_VDM_EXIT_MODE_RESPONSE_TIMER_PERIOD  (45u)

/** VDM receiver response timer period in ms. See Section 6.5.12.1 of USBPD Spec Rev2 v1.2.
 * This timer is slightly relaxed from the spec value.
 */
#define CY_PD_DPM_RESP_REC_RESP_PERIOD         (20u)

/** BIST continuous mode period in ms. See Section 6.5.8.4 of USBPD Spec Rev2 v1.2 */
#define CY_PD_BIST_CONT_MODE_TIMER_PERIOD      (55u)

/** Time in ms allowed for VBus turn off during hard reset. */
#define CY_PD_SINK_VBUS_TURN_OFF_TIMER_PERIOD  (750u)

/** Time in ms allowed for VBus to turn on during hard reset. */
#define CY_PD_SINK_CY_PD_VBUS_TURN_ON_TIMER_PERIOD   (1300u)

/** Hard reset timer period in ms. See Section 6.5.11.2 of USBPD Spec Rev2 v1.2 */
#define CY_PD_PS_HARD_RESET_TIMER_PERIOD       (27u)

/** Time for which PD 3.0 source will keep Rp as SinkTxNG after returning to Ready state. */
#define CY_PD_COLLISION_SRC_COOL_OFF_TIMER_PERIOD      (5u)

/** Delay between AMS initiation attempts by PD 3.0 sink while Rp = SinkTxNG. */
#define CY_PD_SINK_TX_TIMER_PERIOD                     (18u)

/** PPS timer period in ms. */
#define CY_PD_PPS_SRC_TIMER_PERIOD                     (14000u)

/** CC debounce period in ms from Type-C spec. */
#define CY_PD_TYPEC_CC_DEBOUNCE_TIMER_PERIOD              (140u)

/** PD debounce period in ms. */
#define CY_PD_TYPEC_PD_DEBOUNCE_TIMER_PERIOD              (11u)

/** Rd debounce period (detach detection) in ms. */
#define CY_PD_TYPEC_RD_DEBOUNCE_TIMER_PERIOD              (12u)

/** Delay between Attached.Wait state entry and start of checks for detached status. */
#define CY_PD_TYPEC_ATTACH_WAIT_ENTRY_DELAY_PERIOD        (10u)

/** Debounce period used to detect detach when we are source. */
#define CY_PD_TYPEC_SRC_DETACH_DEBOUNCE_PERIOD            (2u)

/** Period in ms used to detect Rp change in a PD 3.0 contract. */
#define CY_PD_TYPEC_PD3_RPCHANGE_DEBOUNCE_PERIOD          (2u)

/** Type-C error recovery timer period in ms. */
#define CY_PD_TYPEC_ERROR_RECOVERY_TIMER_PERIOD           (250u)

/** Type-C Try DRP timer period in ms. */
#define CY_PD_TYPEC_DRP_TRY_TIMER_PERIOD                  (110u)

/** Type-C Try Timeout timer period in ms. */
#define CY_PD_TYPEC_TRY_TIMEOUT_PERIOD                    (800u)

/** Period in ms to check whether the solution state allows to move forward with Type-C connection. */
#define CY_PD_SLN_STATUS_CHECK_PERIOD                     (10u)

/** FRS transmit enable flag in config table setting. */
#define CY_PD_FRS_TX_ENABLE_MASK                      (0x02u)

/** FRS receive enable flag in config table setting. */
#define CY_PD_FRS_RX_ENABLE_MASK                      (0x01u)

/** Size of extended source capabilities message in bytes. */
#define CY_PD_EXT_SRCCAP_SIZE                      (24u)

/** Index of Source Inputs field in extended source caps. */
#define CY_PD_EXT_SRCCAP_INP_INDEX                 (21u)

/** Mask for unconstrained source input in extended source caps. */
#define CY_PD_EXT_SRCCAP_INP_UNCONSTRAINED         (0x02u)

/** Index of PDP field in extended source caps. */
#define CY_PD_EXT_SRCCAP_PDP_INDEX                 (23u)

/** Size of extended sink cap message in bytes. */
#define CY_PD_EXT_SNKCAP_SIZE                      (21u)

/** Size of buffer allocated for extended sink cap data. */
#define CY_PD_EXT_SNKCAP_BUF_SIZE                  (24u)

/** Offset of SKEDB version field in Ext. Snk Cap. This field must be non-zero for a valid response. */
#define CY_PD_EXT_SNKCAP_VERS_INDEX                (10u)

/** Size of status extended message in bytes. */
#define CY_PD_EXT_STATUS_SIZE                      (7u)

/** Size of PPS status extended message in bytes. */
#define CY_PD_EXT_PPS_STATUS_SIZE                  (4u)

/** Externally powered bit position in Source PDO mask. */
#define CY_PD_EXTERNALLY_POWERED_BIT_POS       (7u)

/** Mask to be applied on Fixed Supply Source PDO for PD Rev 2.0 */
#define CY_PD_FIX_SRC_PDO_MASK_REV2                (0xFE3FFFFFu)

/** Mask to be applied on Fixed Supply Source PDO for PD Rev 3.0 */
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

/** No additional DPM error information available. */
#define CY_PD_DPM_ERROR_NONE                          (0u)

/** DPM command failed due to absence of VConn. */
#define CY_PD_DPM_ERROR_NO_VCONN                      (1u)

/** Enter USB DO USB4 mode mask. */
#define CY_PD_USB4_EUDO_USB4_EN_MASK                  (0x26000000u)

/** USB4 data mode as defined in Enter USB DO. */
#define CY_PD_USB_MODE_USB4                           (2u)

/** USB 3.2 data mode as defined in Enter USB DO. */
#define CY_PD_USB_MODE_USB3                           (1u)

/** USB 2.0 data mode as defined in Enter USB DO. */
#define CY_PD_USB_MODE_USB2                           (0u)

/** TBT Gen 3 cable identifier in the TBT cable Disc Mode VDO response. */
#define CY_PD_TBT_GEN_3                               (3u)

/** TBT Gen 3 cable identifier in the TBT cable Disc Mode VDO response. */
#define CY_PD_UFP_VDO_1_RECFG_ALT_MODE_PARAM_MASK     (0x20u)

/* Macros used internally in the PD stack. Exclude from API guide. */

/** @cond DOXYGEN_HIDE */
#define CY_PD_LENGTH_CHECK_SKIP                (0xFFFFU)    /* Macro to skip length check in pd_is_msg function. */
#define CY_PD_CBL_VDO_INDEX                    (4U)        /* Index of cable VDO in received VDM. */
#define CY_PD_CBL_VDO2_INDEX                   (5U)        /* Index of active cable VDO 2 in received VDM. */
#define CY_PD_CBL_VDO_COUNT                    (5U)        /* Number of expected data objects in D_ID response VDM. */

#define CY_PD_MAX_PD_PKT_WORDS                 (8U)        /* Maximum PD words in one packet. */
#define CY_PD_WORD_SIZE                        (4U)        /* Size of each PD word in bytes. */
#define CY_PD_MAX_PD_PKT_BYTES                 (CY_PD_MAX_PD_PKT_WORDS * CY_PD_WORD_SIZE)

/* Create PD 2.0 header given message type, id and count. */
#define CY_PD_HEADER(type, id, cnt)            \
    (((uint32_t)type) | (CY_PD_REV_V2_0 << 6) | (((uint32_t)id) << 9) | (((uint32_t)cnt) << 12))

/* Create PD 3.0 header given message type, id, count and extended message indication. */
#define CY_PD_HEADER_REV3(type, id, cnt, ext)  \
    (((uint32_t)type) | (((uint32_t)id) << 9) | (((uint32_t)cnt) << 12) | (((uint32_t)ext) << 15))

/* Return message count from header. */
#define CY_PD_GET_PD_HDR_CNT(header)              (((header) >> 12) & 0x7)

/* Return PR role bit from header. */
#define CY_PD_GET_PD_HDR_PR_ROLE(header)          (((header) >> 8) & 0x1)

/* Return Cable Plug bit from header. */
#define CY_PD_GET_PD_HDR_CBL_PLUG(header)         (((header) >> 8) & 0x1)

/* Return spec revision from header. */
#define CY_PD_GET_PD_HDR_SPEC_REV(header)         (((header) >> 6) & 0x3)

/* Return DR role bit from header. */
#define CY_PD_GET_PD_HDR_DR_ROLE(header)          (((header) >> 5) & 0x1)

/* Return message ID from header. */
#define CY_PD_GET_PD_HDR_ID(header)               (((header) >> 9) & 0x7)

/* Return Message Type from header. */
#define CY_PD_GET_PD_HDR_TYPE(header)             ((header) & 0xF)

/* PD header mask. */
#define CY_PD_HDR_MASK                         (0x0000FFFFu)

/* Message ID mask. */
#define CY_PD_MSG_ID_MASK                         ((0x7UL) << 9u)

/* Return complete PD header Rx buffer[0]. */
#define CY_PD_GET_PD_HDR(buf0)                    ((buf0) & CY_PD_HDR_MASK)

#define CY_PD_GET_RDO_OBJ_POS(rdo)                (((rdo) >> 28) & 0x7)       /* Get object position from RDO. */
#define CY_PD_GET_RDO_GV_BACK(rdo)                (((rdo) >> 27) & 0x1)       /* Get GiveBack flag from RDO. */
#define CY_PD_GET_RDO_CAP_MIS(rdo)                (((rdo) >> 26) & 0x1)       /* Get Cap. mismatch flag from RDO. */
#define CY_PD_GET_RDO_USB_COM(rdo)                (((rdo) >> 25) & 0x1)       /* Get USB comm. support flag from RDO. */
#define CY_PD_GET_RDO_NO_SSPND(rdo)               (((rdo) >> 24) & 0x1)       /* Get USB suspend support flag from RDO. */
#define CY_PD_GET_RDO_OP_CUR(rdo)                 (((rdo) >> 10) & 0x3FF)     /* Get Op. current field from RDO. */
#define CY_PD_GET_RDO_OP_PWR(rdo)                 (((rdo) >> 10) & 0x3FF)     /* Get Op. power field from battery RDO. */
#define CY_PD_GET_RDO_MAX_OP_CUR(rdo)             (((rdo)) & 0x3FF)           /* Get max. op. current from RDO. */
#define CY_PD_GET_RDO_MIN_OP_CUR(rdo)             (((rdo)) & 0x3FF)           /* Get min. op. current from RDO. */
#define CY_PD_GET_RDO_MAX_OP_PWR(rdo)             (((rdo)) & 0x3FF)           /* Get max. op. power from battery RDO. */
#define CY_PD_GET_RDO_MIN_OP_PWR(rdo)             (((rdo)) & 0x3FF)           /* Get min. op. power from battery RDO. */

#define CY_PD_GET_VID(vdm_hdr)                    ((vdm_hdr) >> 16)           /* Get VID from VDM header. */
#define CY_PD_GET_VDM_TYPE(vdm_hdr)               (((vdm_hdr) >> 15) & 0x1)   /* Get VDM type from VDM header. */
#define CY_PD_GET_SVDM_VDM_VER(vdm_hdr)           (((vdm_hdr) >> 13) & 0x3)   /* Get VDM version from VDM header. */
#define CY_PD_GET_SVDM_OBJ_POS(vdm_hdr)           (((vdm_hdr) >> 8) & 0x7)    /* Get object position from VDM header. */
#define CY_PD_GET_SVDM_CMD_TYPE(vdm_hdr)          (((vdm_hdr) >> 6) & 0x3)    /* Get command type from VDM header. */
#define CY_PD_GET_SVDM_CMD(vdm_hdr)               (((vdm_hdr)) & 0x1F)        /* Get command code from VDM header. */

/* VDM header for D_ID command. */
#define CY_PD_STD_VDM_HEADER_IDENTITY_REQ                                             \
                                            (                                   \
                                                (CY_PD_STD_SVID << 16) |              \
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

#define CY_PD_GET_BIST_MODE(bist_hdr)             ((bist_hdr) >> 28)          /* Get BIST mode from BIST header. */

/* Message masks for data messages. */
#define CY_PD_DATA_MSG_SRC_CAP_MASK               (0x1UL << CY_PDSTACK_DATA_MSG_SRC_CAP)
#define CY_PD_DATA_MSG_REQUEST_MASK               (0x1UL << CY_PDSTACK_DATA_MSG_REQUEST)
#define CY_PD_DATA_MSG_BIST_MASK                  (0x1UL << CY_PDSTACK_DATA_MSG_BIST)
#define CY_PD_DATA_MSG_SNK_CAP_MASK               (0x1UL << CY_PDSTACK_DATA_MSG_SNK_CAP)
#define CY_PD_DATA_MSG_BAT_STATUS_MASK            (0x1UL << CY_PDSTACK_DATA_MSG_BAT_STATUS)
#define CY_PD_DATA_MSG_SRC_ALERT_MASK             (0x1UL << CY_PDSTACK_DATA_MSG_ALERT)
#define CY_PD_DATA_MSG_VDM_MASK                   (0x1UL << CY_PDSTACK_DATA_MSG_VDM)
#define CY_PD_DATA_MSG_ENTER_USB_MASK             (0x1UL << CY_PDSTACK_DATA_MSG_ENTER_USB)

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

#define CY_PD_CBL_CAP_0A                          (0u)        /* Cable current capability: 0 A. */
#define CY_PD_CBL_CAP_DFLT                        (90u)       /* Cable current capability: 900 mA (Type-C default). */
#define CY_PD_CBL_CAP_3A                          (300u)      /* Cable current capability: 3 A. */
#define CY_PD_CBL_CAP_5A                          (500u)      /* Cable current capability: 5 A. */

#define CY_PD_CBL_VDO_VERS_1_0                    (0u)        /* Active Cable VDO version 1.0 */
#define CY_PD_CBL_VDO_VERS_1_2                    (2u)        /* Active Cable VDO version 1.2 */
#define CY_PD_CBL_VDO_VERS_1_3                    (3u)        /* Active Cable VDO version 1.3 */

/*
 * Default cable current capability assumed by the stack. Please use
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

/* Masks for Policy Engine related events. */
#define CY_PD_PE_EVT_NONE                         (0x00000000u)       /* No Policy Engine events pending. */
#define CY_PD_PE_EVT_HARD_RESET_RCVD              (0x00000001u)       /* Hard Reset received event pending. */
#define CY_PD_PE_EVT_SOFT_RESET_RCVD              (0x00000002u)       /* Soft Reset received event pending. */
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
#define CY_PD_PE_EVT_ALL_MASK                     (0xFFFFFFFFu)       /* Mask to extract all pending events. */

/* Rp combinations macros. */
#define CY_PD_RP_CC1_OPEN_CC2_OPEN                ((((uint16_t)CY_PD_RP_OPEN) << 8) | (uint16_t)CY_PD_RP_OPEN) /* PMG1 Rp: No external termination on both CC. */
#define CY_PD_RP_CC1_OPEN_CC2_RD                  ((((uint16_t)CY_PD_RP_RD) << 8)   | (uint16_t)CY_PD_RP_OPEN) /* PMG1 Rp: external Rd on CC2. */
#define CY_PD_RP_CC1_OPEN_CC2_RA                  ((((uint16_t)CY_PD_RP_RA) << 8)   | (uint16_t)CY_PD_RP_OPEN) /* PMG1 Rp: external Ra on CC2. */
#define CY_PD_RP_CC1_RD_CC2_OPEN                  ((((uint16_t)CY_PD_RP_OPEN) << 8) | (uint16_t)CY_PD_RP_RD)   /* PMG1 Rp: external Rd on CC1. */
#define CY_PD_RP_CC1_RA_CC2_OPEN                  ((((uint16_t)CY_PD_RP_OPEN) << 8) | (uint16_t)CY_PD_RP_RA)   /* PMG1 Rp: external Ra on CC1. */
#define CY_PD_RP_CC1_RA_CC2_RD                    ((((uint16_t)CY_PD_RP_RD) << 8)   | (uint16_t)CY_PD_RP_RA)   /* PMG1 Rp: Rd on CC2, Ra on CC1. */
#define CY_PD_RP_CC1_RA_CC2_RA                    ((((uint16_t)CY_PD_RP_RA) << 8)   | (uint16_t)CY_PD_RP_RA)   /* PMG1 Rp: Ra on both CC. */
#define CY_PD_RP_CC1_RD_CC2_RA                    ((((uint16_t)CY_PD_RP_RA) << 8)   | (uint16_t)CY_PD_RP_RD)   /* PMG1 Rp: Rd on CC1, Ra on CC2. */
#define CY_PD_RP_CC1_RD_CC2_RD                    ((((uint16_t)CY_PD_RP_RD) << 8)   | (uint16_t)CY_PD_RP_RD)   /* PMG1 Rp: Rd on both CC. */

/* Rd combinations macros. */
#define CY_PD_RD_CC1_RA_CC2_USB                   ((((uint16_t)CY_PD_RD_USB) << 8)  | (uint16_t)CY_PD_RD_RA)   /* PMG1 Rd: External Default Rp on CC2. */
#define CY_PD_RD_CC1_RA_CC2_1_5A                  ((((uint16_t)CY_PD_RD_1_5A) << 8) | (uint16_t)CY_PD_RD_RA)   /* PMG1 Rd: 1.5A Rp on CC2. */
#define CY_PD_RD_CC1_RA_CC2_3A                    ((((uint16_t)CY_PD_RD_3A) << 8)   | (uint16_t)CY_PD_RD_RA)   /* PMG1 Rd: 3A Rp on CC2. */
#define CY_PD_RD_CC1_USB_CC2_RA                   ((((uint16_t)CY_PD_RD_RA) << 8)   | (uint16_t)CY_PD_RD_USB)  /* PMG1 Rd: External Default Rp on CC1. */
#define CY_PD_RD_CC1_1_5A_CC2_RA                  ((((uint16_t)CY_PD_RD_RA) << 8)   | (uint16_t)CY_PD_RD_1_5A) /* PMG1 Rd: 1.5A Rp on CC1. */
#define CY_PD_RD_CC1_3A_CC2_RA                    ((((uint16_t)CY_PD_RD_RA) << 8)   | (uint16_t)CY_PD_RD_3A)   /* PMG1 Rd: 3A Rp on CC1. */
#define CY_PD_RD_CC1_RA_CC2_RA                    ((((uint16_t)CY_PD_RD_RA) << 8)   | (uint16_t)CY_PD_RD_RA)   /* PMG1 Rd: No Rp on either CC. */
#define CY_PD_RD_CC1_ERR_CC2_ERR                  ((((uint16_t)CY_PD_RD_ERR) << 8)  | (uint16_t)CY_PD_RD_ERR)  /* PMG1 Rd: Error state. */

#define CY_PD_RD_CC1_USB_CC2_USB                  ((((uint16_t)CY_PD_RD_USB) << 8)  | (uint16_t)CY_PD_RD_USB)  /* PMG1 Rd: External Default Rp on CC1 and CC2. */
#define CY_PD_RD_CC1_1_5A_CC2_1_5A                ((((uint16_t)CY_PD_RD_1_5A) << 8) | (uint16_t)CY_PD_RD_1_5A) /* PMG1 Rd: 1.5A Rp on both CC1 and CC2. */
#define CY_PD_RD_CC1_3A_CC2_3A                    ((((uint16_t)CY_PD_RD_3A) << 8)   | (uint16_t)CY_PD_RD_3A)   /* PMG1 Rd: 3A Rp on CC1 and CC2. */

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
    CY_PDSTACK_ERR_RECOV_HR_FAIL,                  /**< Error recovery due to hard reset failure. */
    CY_PDSTACK_ERR_RECOV_PROTECT_FAULT,            /**< Error recovery due to protection (OVP/OCP) fault. */
    CY_PDSTACK_ERR_RECOV_POWER_FAULT,              /**< Error recovery due to voltage fault. */
    CY_PDSTACK_ERR_RECOV_BAD_DATA_ROLE,            /**< Error recovery due to bad data role in incoming PD message. */
    CY_PDSTACK_ERR_RECOV_FRS_FAIL,                 /**< Error recovery due to Fast Role Swap error. */
    CY_PDSTACK_ERR_RECOV_DATA_RESET_FAIL           /**< Error recovery due to failed Data_Reset sequence. */
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
 * @brief Enumeration of reasons for issuing a Cable Reset.
 */
typedef enum {
    CY_PDSTACK_EMCA_CABLE_RES_NONE = 0,            /**< No Cable Reset performed. */
    CY_PDSTACK_EMCA_CABLE_RES_SR_TIMEOUT           /**< SOP' or SOP'' SoftReset timed out. */
} cy_en_pdstack_cable_reset_reason_t;

/**
 * @typedef cy_en_pdstack_hard_reset_reason_t
 * @brief Enumeration of reasons for issuing a hard reset.
 */
typedef enum {
    CY_PDSTACK_HARDRES_REASON_NONE = 0,         /**< HardReset not issued. */
    CY_PDSTACK_HARDRES_REASON_NO_SRC_CAP,       /**< No Source Capability messages received. */
    CY_PDSTACK_HARDRES_REASON_HOSTCONN,         /**< TBT Host Connect state change. */
    CY_PDSTACK_HARDRES_REASON_SR_ERROR,         /**< SoftReset failed. */
    CY_PDSTACK_HARDRES_REASON_CONTRACT_ERROR,   /**< Power contract failed. */
    CY_PDSTACK_HARDRES_REASON_DRSWAP,           /**< DR Swap received while in Alternate Mode. */
    CY_PDSTACK_HARDRES_REASON_VBUS_OVP,         /**< Over-Voltage condition detected. */
    CY_PDSTACK_HARDRES_REASON_VBUS_OCP,         /**< Over-Current condition detected. */
    CY_PDSTACK_HARDRES_REASON_AMS_ERROR,        /**< PD Atomic Message Sequence error. */
} cy_en_pdstack_hard_reset_reason_t;

/**
 * @typedef cy_en_pdstack_soft_reset_reason_t
 * @brief Enumeration of reasons for issuing a soft reset.
 */
typedef enum {
    CY_PDSTACK_SOFTRES_REASON_NONE = 0,         /**< SoftReset not issued. */
    CY_PDSTACK_SOFTRES_REASON_SRCNEG_ERROR,     /**< Contract negotiation error when PMG1x is source. */
    CY_PDSTACK_SOFTRES_REASON_SNKNEG_ERROR,     /**< Contract negotiation error when PMG1x is sink. */
    CY_PDSTACK_SOFTRES_REASON_AMS_ERROR         /**< PD protocol error. */
} cy_en_pdstack_soft_reset_reason_t;

/**
 * @typedef app_swap_resp_t
 *
 * @brief Possible responses to various USB-PD swap requests from the application layer.
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
    CY_PDSTACK_EXTD_MSG_SNK_CAP_EXTD               /**< 0x0F: Sink_Capabilities_Extended message. */
} cy_en_pdstack_extd_msg_t;

/**
 * @typedef cy_en_pdstack_pdo_t
 * @brief Enum of the PDO types.
 */
typedef enum
{
    CY_PDSTACK_PDO_FIXED_SUPPLY = 0,               /**< Fixed (voltage) supply power data object. */
    CY_PDSTACK_PDO_BATTERY,                        /**< Battery based power data object. */
    CY_PDSTACK_PDO_VARIABLE_SUPPLY,                /**< Variable (voltage) supply power data object. */
    CY_PDSTACK_PDO_AUGMENTED                       /**< Augmented power data object. */
} cy_en_pdstack_pdo_t;

/**
 * @typedef cy_en_pdstack_apdo_t
 * @brief Enum of the Augmented PDO types.
 */
typedef enum
{
    CY_PDSTACK_APDO_PPS = 0,                       /**< Programmable Power Supply PDO. */
    CY_PDSTACK_APDO_RSVD1,                         /**< Reserved for future use. */
    CY_PDSTACK_APDO_RSVD2,                         /**< Reserved for future use. */
    CY_PDSTACK_APDO_RSVD3                          /**< Reserved for future use. */
} cy_en_pdstack_apdo_t;

/**
 * @typedef cy_en_pdstack_peak_cur_cap_t
 * @brief Enum of Peak Current Capability levels.
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
    CY_PDSTACK_BIST_RETURN_COUNTERS_MODE,          /**< Send Returned BIST counters response. */
    CY_PDSTACK_BIST_CARRIER_MODE_0,                /**< BIST carrier mode 0. */
    CY_PDSTACK_BIST_CARRIER_MODE_1,                /**< BIST carrier mode 1. */
    CY_PDSTACK_BIST_CARRIER_MODE_2,                /**< BIST carrier mode 2. */
    CY_PDSTACK_BIST_CARRIER_MODE_3,                /**< BIST carrier mode 3. */
    CY_PDSTACK_BIST_EYE_PATTERN_MODE,              /**< BIST eye pattern. */
    CY_PDSTACK_BIST_TEST_DATA_MODE                 /**< BIST test data mode. */
} cy_en_pdstack_bist_mode_t;

/**
 * @typedef cy_en_pdstack_fr_swap_supp_t
 * @brief Enum to hold FR swap options in sink capabilities
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
 * @brief Enum of the PD Request results. Enum fields map to the control
 * message field in the PD spec.
 */
typedef enum
{
    CY_PDSTACK_REQ_SEND_HARD_RESET = 1,            /**< Invalid message. Send Hard Reset. */
    CY_PDSTACK_REQ_ACCEPT = 3,                     /**< Send Accept message. */
    CY_PDSTACK_REQ_REJECT = 4,                     /**< Send Reject message. */
    CY_PDSTACK_REQ_WAIT = 12,                      /**< Send Wait message. */
    CY_PDSTACK_REQ_NOT_SUPPORTED = 16              /**< Send Not_Supported message. Will translate to Reject message under PD 2.0 */
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
 * @brief Enum of the DPM (Device Policy Manager) command types.
 */
typedef enum
{
    CY_PDSTACK_DPM_CMD_SRC_CAP_CHNG = 0,           /**< 00: Source Caps changed notification. Used to trigger fresh contract. */
    CY_PDSTACK_DPM_CMD_SNK_CAP_CHNG,               /**< 01: Sink Caps changed notification. Used to trigger fresh contract. */
    CY_PDSTACK_DPM_CMD_SEND_GO_TO_MIN,             /**< 02: Send GotoMin message to port partner. */
    CY_PDSTACK_DPM_CMD_GET_SNK_CAP,                /**< 03: Send Get_Sink_Cap message to port partner. */
    CY_PDSTACK_DPM_CMD_GET_SRC_CAP,                /**< 04: Send Get_Source_Cap message to port partner. */
    CY_PDSTACK_DPM_CMD_SEND_HARD_RESET,            /**< 05: Send Hard Reset. */
    CY_PDSTACK_DPM_CMD_SEND_SOFT_RESET,            /**< 06: Send Soft Reset to port partner. */
    CY_PDSTACK_DPM_CMD_SEND_CABLE_RESET,           /**< 07: Send Cable Reset. */
    CY_PDSTACK_DPM_CMD_SEND_SOFT_RESET_EMCA,       /**< 08: Send Soft Reset to cable marker. */
    CY_PDSTACK_DPM_CMD_SEND_DR_SWAP,               /**< 09: Send DR_Swap request. */
    CY_PDSTACK_DPM_CMD_SEND_PR_SWAP,               /**< 0A: Send PR_Swap request. */
    CY_PDSTACK_DPM_CMD_SEND_VCONN_SWAP,            /**< 0B: Send VCONN_Swap request. */
    CY_PDSTACK_DPM_CMD_SEND_VDM,                   /**< 0C: Send VDM message. */
    CY_PDSTACK_DPM_CMD_SEND_EXTENDED,              /**< 0D: Send extended data message. */
    CY_PDSTACK_DPM_CMD_GET_SRC_CAP_EXTENDED,       /**< 0E: Send Get_Source_Cap_Extended message. */
    CY_PDSTACK_DPM_CMD_GET_STATUS,                 /**< 0F: Send Get_Status message. */
    CY_PDSTACK_DPM_CMD_SEND_BATT_STATUS,           /**< 10: Send Battery_Status data message. */
    CY_PDSTACK_DPM_CMD_SEND_ALERT,                 /**< 11: Send Alert message. */
    CY_PDSTACK_DPM_CMD_SEND_NOT_SUPPORTED,         /**< 12: Send Not_Supported message. */
    CY_PDSTACK_DPM_CMD_INITIATE_CBL_DISCOVERY,     /**< 13: Initiate cable discovery (preceded by VConn Swap if required). */
    CY_PDSTACK_DPM_CMD_SEND_DATA_RESET,            /**< 14: Send a USB4 Data_Reset message. */
    CY_PDSTACK_DPM_CMD_SEND_ENTER_USB,             /**< 15: Send a USB4 Enter_USB message to port partner or cable marker. */
    CY_PDSTACK_DPM_CMD_GET_SNK_CAP_EXTENDED,       /**< 16: Send Get_Sink_Cap_Extended message. */
    CY_PDSTACK_DPM_CMD_SEND_INVALID = 0xFFu        /**< FF: Invalid command code. */
} cy_en_pdstack_dpm_pd_cmd_t;

/**
 * @typedef cy_en_pdstack_vdm_type_t
 * @brief Enum of the VDM types.
 */
typedef enum
{
    CY_PDSTACK_VDM_TYPE_UNSTRUCTURED = 0,       /**< Unstructured VDM. */
    CY_PDSTACK_VDM_TYPE_STRUCTURED,              /**< Structured VDM. */
} cy_en_pdstack_vdm_type_t;

/**
 * @typedef cy_en_pdstack_std_vdm_cmd_t
 * @brief Enum of the standard VDM commands.
 */
typedef enum
{
    CY_PDSTACK_VDM_CMD_DSC_IDENTITY = 1,           /**< Discover Identity command. */
    CY_PDSTACK_VDM_CMD_DSC_SVIDS = 2,              /**< Discover SVIDs command. */
    CY_PDSTACK_VDM_CMD_DSC_MODES = 3,         /**< Discover Modes command. */
    CY_PDSTACK_VDM_CMD_ENTER_MODE = 4,        /**< Enter Mode command. */
    CY_PDSTACK_VDM_CMD_EXIT_MODE = 5,         /**< Exit Mode command. */
    CY_PDSTACK_VDM_CMD_ATTENTION = 6,         /**< Attention message. */
    CY_PDSTACK_VDM_CMD_DP_STATUS_UPDT = 16,   /**< DisplayPort Status Update message. */
    CY_PDSTACK_VDM_CMD_DP_CONFIGURE = 17      /**< DisplayPort Configure command. */
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
    CY_PDSTACK_PROD_TYPE_PSD      = 3,             /**< Power Sink Device. */
    CY_PDSTACK_PROD_TYPE_PAS_CBL  = 3,             /**< Passive Cable. */
    CY_PDSTACK_PROD_TYPE_ACT_CBL  = 4,             /**< Active Cable. */
    CY_PDSTACK_PROD_TYPE_AMA      = 5,             /**< Alternate Mode Accessory. */
    CY_PDSTACK_PROD_TYPE_VPD      = 6,             /**< VConn powered device. */
    CY_PDSTACK_PROD_TYPE_RSVD     = 7              /**< Reserved. Shall not be used. */
} cy_en_pdstack_std_vdm_prod_t;

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
 * @typedef cy_en_pdstack_cbl_vbus_cur_t
 * @brief Enum of the cable current levels.
 */
typedef enum
{
    CY_PDSTACK_CBL_VBUS_CUR_DFLT = 0,              /**< Cable can support a maximum of 900mA. */
    CY_PDSTACK_CBL_VBUS_CUR_3A,                    /**< Cable can support a maximum of 3A. */
    CY_PDSTACK_CBL_VBUS_CUR_5A,                    /**< Cable can support a maximum of 5A. */
    CY_PDSTACK_CBL_VBUS_CUR_0A                     /**< Cable does not conduct VBus power through. */
} cy_en_pdstack_cbl_vbus_cur_t;

/**
 * @typedef cy_en_pdstack_cbl_type_t
 * @brief Enum of the cable types.
 */
typedef enum
{
    CY_PDSTACK_CBL_TYPE_PASSIVE = 0,               /**< Passive cable. */
    CY_PDSTACK_CBL_TYPE_ACTIVE_RETIMER,            /**< Active Re-timer cable. */
    CY_PDSTACK_CBL_TYPE_ACTIVE_REDRIVER,           /**< Active Re-driver cable. */
    CY_PDSTACK_CBL_TYPE_OPTICAL                    /**< Optically Isolated cable. */
} cy_en_pdstack_cbl_type_t;

/**
 * @typedef cy_en_pdstack_cbl_term_t
 * @brief Enum of the cable termination types.
 */
typedef enum
{
    CY_PDSTACK_CBL_TERM_BOTH_PAS_VCONN_NOT_REQ = 0,        /**< Passive cable, VConn not required. */
    CY_PDSTACK_CBL_TERM_BOTH_PAS_VCONN_REQ,                /**< Passive cable, VConn required. */
    CY_PDSTACK_CBL_TERM_ONE_ACT_ONE_PAS_VCONN_REQ,         /**< One end active, one end passive, VConn required. */
    CY_PDSTACK_CBL_TERM_BOTH_ACT_VCONN_REQ                 /**< Both ends of cable are active, VConn required. */
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
    CY_PDSTACK_USB_GEN_3                 /**< [USB 4.0] Gen 3. */
} cy_en_pdstack_usb_sig_supp_t;

/**
 * @typedef cy_en_pdstack_usb_dev_cap_t
 * @brief Enum of the USB device capabilities masks.
 */
typedef enum
{
    CY_PDSTACK_DEV_CAP_USB_2_0 = (1<<0),     /**< [USB 2.0] Device Capable. */
    CY_PDSTACK_DEV_CAP_USB_BB_ONLY = (1<<1), /**< Device Capable (Billboard only). */
    CY_PDSTACK_DEV_CAP_USB_3_2 = (1<<2),     /**< [USB 3.2] Device Capable. */
    CY_PDSTACK_DEV_CAP_USB_4_0 = (1<<3)      /**< [USB4] Device Capable. */
} cy_en_pdstack_usb_dev_cap_t;

/**
 * @typedef cy_en_pdstack_usb_host_cap_t
 * @brief Enum of the USB host capabilities masks.
 */
typedef enum
{
    CY_PDSTACK_HOST_CAP_USB_2_0 = (1<<0),  /**< [USB 2.0] Host Capable. */
    CY_PDSTACK_HOST_CAP_USB_3_2 = (1<<1),  /**< [USB 3.2] Host Capable. */
    CY_PDSTACK_HOST_CAP_USB_4_0 = (1<<2)   /**< [USB4] Host Capable. */
} cy_en_pdstack_usb_host_cap_t;

/**
 * @typedef cy_en_pdstack_usb_role_t
 * @brief Enum of the USB role types.
 */
typedef enum
{
    CY_PDSTACK_USB_ROLE_DEV = 0,                   /**< USB data role Device. */
    CY_PDSTACK_USB_ROLE_HOST,                      /**< USB data role Host. */
    CY_PDSTACK_USB_ROLE_DRD                        /**< USB data role Device and Host. */
} cy_en_pdstack_usb_role_t;

/**
 * @typedef cy_en_pdstack_pe_cbl_state_t
 * @brief Enum of the Policy Engine cable discovery states.
 */
typedef enum
{
    CY_PDSTACK_CBL_FSM_DISABLED = 0,                       /**< Cable state machine is inactive. */
    CY_PDSTACK_CBL_FSM_ENTRY,                              /**< Cable state machine starting up. */
    CY_PDSTACK_CBL_FSM_SEND_SOFT_RESET,                    /**< Cable state machine sending Soft Reset to cable marker. */
    CY_PDSTACK_CBL_FSM_SEND_DSC_ID                         /**< Cable state machine waiting for cable response. */
} cy_en_pdstack_pe_cbl_state_t;

/**
 * @typedef cy_en_pdstack_try_src_snk_t
 * @brief Enum of the Try Source/ Try Sink options.
 */
typedef enum
{
    CY_PDSTACK_TRY_SRC_TRY_SNK_DISABLED = 0,       /**< Try.SRC and Try.SNK disabled. */
    CY_PDSTACK_TRY_SRC_ENABLED,                    /**< Try.SRC enabled. */
    CY_PDSTACK_TRY_SNK_ENABLED                     /**< Try.SNK enabled. */
} cy_en_pdstack_try_src_snk_t;

/**
 * @typedef cy_en_pdstack_dpm_typec_cmd_t
 * @brief Enum of the DPM (Device Policy Manager) command types that can be initiated through
 * the dpm_typec_command API.
 * @see dpm_typec_command
 */
typedef enum
{
    CY_PDSTACK_DPM_CMD_SET_RP_DFLT = 0,            /**< Command to select Default Rp. */
    CY_PDSTACK_DPM_CMD_SET_RP_1_5A,                /**< Command to select 1.5 A Rp. */
    CY_PDSTACK_DPM_CMD_SET_RP_3A                   /**< Command to select 3 A Rp. */,
    CY_PDSTACK_DPM_CMD_PORT_DISABLE,               /**< Command to disable the USB-PD port. */
    CY_PDSTACK_DPM_CMD_TYPEC_ERR_RECOVERY,         /**< Command to initiate Type-C error recovery. */
    CY_PDSTACK_DPM_CMD_TYPEC_INVALID               /**< Invalid command type. */
} cy_en_pdstack_dpm_typec_cmd_t;

/**
 * @typedef cy_en_pdstack_dpm_typec_cmd_resp_t
 * @brief Enum of the DPM (Device Policy Manager) response types.
 */
typedef enum
{
    CY_PDSTACK_DPM_RESP_FAIL = 0,                  /**< Command failed. */
    CY_PDSTACK_DPM_RESP_SUCCESS                    /**< Command succeeded. */
} cy_en_pdstack_dpm_typec_cmd_resp_t;

/**
 * @typedef cy_en_pdstack_typec_fsm_state_t
 * @brief Enum of the Type-C FSM states. This is for internal stack usage.
 * @warning The ordering of elements must not be altered unless the state table
 * is also updated to match.
 */
typedef enum
{
    CY_PDSTACK_TYPEC_FSM_DISABLED = 0,             /**< Type-C state machine is disabled. */
    CY_PDSTACK_TYPEC_FSM_ERR_RECOV,                /**< Error Recovery state. */
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
 * @brief Enumeration of Policy Engine states for a USB-PD port. This is for internal stack usage.
 * @warning The ordering of elements must not be altered unless the state table in the stack
 * source is also updated.
 */
typedef enum
{
    CY_PDSTACK_PE_FSM_OFF = 0,                             /**< 00: Policy Engine not started. */
    CY_PDSTACK_PE_FSM_HR_SEND,                             /**< 01: Send HardReset */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_PE_FSM_HR_SRC_TRANS_DFLT,                   /**< 02: PE_SRC_Transition_to_default */
    CY_PDSTACK_PE_FSM_HR_SRC_RECOVER,                      /**< 03: Policy Engine waiting for recovery before enabling VBus. */
    CY_PDSTACK_PE_FSM_HR_SRC_VBUS_ON,                      /**< 04: Policy Engine enabling VBus after Hard Reset completion. */
#endif /* (!CY_PD_SINK_ONLY) */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_PE_FSM_HR_SNK_TRANS_DFLT,                   /**< 05: PE_SNK_Transition_to_default */
    CY_PDSTACK_PE_FSM_HR_SNK_WAIT_VBUS_OFF,                /**< 06: Policy Engine waiting for VBus turning off. */
    CY_PDSTACK_PE_FSM_HR_SNK_WAIT_VBUS_ON,                 /**< 07: Policy Engine waiting for VBus to turn back on. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    CY_PDSTACK_PE_FSM_BIST_TEST_DATA,                      /**< 08: BIST test data state. */
    CY_PDSTACK_PE_FSM_BIST_CM2,                            /**< 09: PE_BIST_Carrier_Mode */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_PE_FSM_SNK_STARTUP,                         /**< 10: PE_SNK_Startup */
    CY_PDSTACK_PE_FSM_SNK_WAIT_FOR_CAP,                    /**< 11: PE_SNK_Wait_for_Capabilities */
    CY_PDSTACK_PE_FSM_SNK_EVAL_CAP,                        /**< 12: PE_SNK_Evaluate_Capability */
    CY_PDSTACK_PE_FSM_SNK_SEL_CAP,                         /**< 13: PE_SNK_Select_Capability */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
#if (!CY_PD_SINK_ONLY)
    CY_PDSTACK_PE_FSM_SRC_STARTUP,                         /**< 14: PE_SRC_Startup */
    CY_PDSTACK_PE_FSM_SRC_WAIT_NEW_CAP,                    /**< 15: PE_SRC_Wait_New_Capabilities */
#if (!(CY_PD_CBL_DISC_DISABLE))
    CY_PDSTACK_PE_FSM_SRC_SEND_CBL_SR,                     /**< 16: PE_CBL_Soft_Reset */
    CY_PDSTACK_PE_FSM_SRC_SEND_CBL_DSCID,                  /**< 17: PE_CBL_Get_Identity */
#endif /* (!(CY_PD_CBL_DISC_DISABLE)) */
    CY_PDSTACK_PE_FSM_SRC_SEND_CAP,                        /**< 18: PE_SRC_Send_Capabilities */
    CY_PDSTACK_PE_FSM_SRC_DISCOVERY,                       /**< 19: PE_SRC_Discovery */
    CY_PDSTACK_PE_FSM_SRC_NEG_CAP,                         /**< 20: PE_SRC_Negotiate_Capability */
    CY_PDSTACK_PE_FSM_SRC_TRANS_SUPPLY,                    /**< 21: PE_SRC_Transition_Supply */
    CY_PDSTACK_PE_FSM_SRC_SEND_PS_RDY,                     /**< 22: Policy Engine waiting to send PS_RDY. */
#endif /* (!CY_PD_SINK_ONLY) */
#if (!(CY_PD_SOURCE_ONLY))
    CY_PDSTACK_PE_FSM_SNK_TRANS,                           /**< 23: PE_SNK_Transition_Sink */
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    CY_PDSTACK_PE_FSM_SR_SEND,                             /**< 24: Policy Engine sending Soft Reset. */
    CY_PDSTACK_PE_FSM_SR_RCVD,                             /**< 25: Policy Engine received Soft Reset. */
    CY_PDSTACK_PE_FSM_VRS_VCONN_ON,                        /**< 26: Policy Engine waiting for VConn to turn on. */
    CY_PDSTACK_PE_FSM_VRS_VCONN_OFF,                       /**< 27: Policy Engine waiting for VConn to turn off. */
    CY_PDSTACK_PE_FSM_SWAP_EVAL,                           /**< 28: Evaluate received swap command. */
    CY_PDSTACK_PE_FSM_SWAP_SEND,                           /**< 29: Waiting to send swap command. */
    CY_PDSTACK_PE_FSM_DRS_CHANGE_ROLE,                     /**< 30: Change data role. */
#if ((!(CY_PD_SOURCE_ONLY)) && (!CY_PD_SINK_ONLY))
    CY_PDSTACK_PE_FSM_PRS_SRC_SNK_TRANS,                   /**< 31: Source to Sink PR_Swap transition start. */
    CY_PDSTACK_PE_FSM_PRS_SRC_SNK_VBUS_OFF,                /**< 32: Initial source waiting for VBus turning off. */
    CY_PDSTACK_PE_FSM_PRS_SRC_SNK_WAIT_PS_RDY,             /**< 33: Initial source waiting for PS_RDY. */
    CY_PDSTACK_PE_FSM_PRS_SNK_SRC_WAIT_PS_RDY,             /**< 34: Initial sink waiting for PS_RDY. */
    CY_PDSTACK_PE_FSM_PRS_SNK_SRC_VBUS_ON,                 /**< 35: Initial sink turning VBus ON. */
    CY_PDSTACK_PE_FSM_FRS_CHECK_RP,                        /**< 36: Initial sink checking Rp to send FR_Swap message. */
    CY_PDSTACK_PE_FSM_FRS_SRC_SNK_CC_SIGNAL,               /**< 37: Initial source sending FR_Swap signal. */
#endif /* ((!(CY_PD_SOURCE_ONLY)) && (!CY_PD_SINK_ONLY)) */
    CY_PDSTACK_PE_FSM_READY,                               /**< 38: PE_Ready state. */
    CY_PDSTACK_PE_FSM_SEND_MSG,                            /**< 39: Policy Engine sending new AMS. */
    CY_PDSTACK_PE_FSM_EVAL_DATA_RESET,                     /**< 40: Policy Engine Handling Data_Reset request. */
    CY_PDSTACK_PE_FSM_SEND_DATA_RESET,                     /**< 41: Policy Engine initiating Data_Reset request. */
    CY_PDSTACK_PE_FSM_EVAL_ENTER_USB,                      /**< 42: Policy Engine handling Enter USB request. */
    CY_PDSTACK_PE_FSM_MAX_STATES                           /**< 43: Invalid Policy Engine state. */
}cy_en_pdstack_pe_fsm_state_t;

/**
 * @typedef cy_en_pdstack_contract_status_t
 * @brief Enum of possible PD contract negotiation scenarios that are used to
 * signal the application event handler. This status will be reported in byte 0
 * of the event data passed along with the APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE
 * event. Bytes 3:1 of the event data are not used; and bytes 7:4 will report
 * the RDO where applicable.
 */
typedef enum
{
    CY_PDSTACK_CONTRACT_NEGOTIATION_SUCCESSFUL      = 0x01,     /**< PD contract negotiation successful. */
    CY_PDSTACK_CONTRACT_CAP_MISMATCH_DETECTED       = 0x03,     /**< PD contract negotiated, but capability mismatch
                                                             is present. */
    CY_PDSTACK_CONTRACT_REJECT_CONTRACT_VALID       = 0x00,     /**< Contract rejected by PMG1, but previous contract
                                                             is still valid. */
    CY_PDSTACK_CONTRACT_REJECT_CONTRACT_NOT_VALID   = 0x04,     /**< Contract rejected by PMG1 and previous contract
                                                             became invalid. */
    CY_PDSTACK_CONTRACT_REJECT_NO_CONTRACT          = 0x08,     /**< Contract rejected by PMG1 and there was no previous
                                                             contract. */
    CY_PDSTACK_CONTRACT_REJECT_EXPLICIT_CONTRACT    = 0x0C,     /**< Request rejected by port partner while in previous
                                                             explicit contract. */
    CY_PDSTACK_CONTRACT_REJECT_NO_EXPLICIT_CONTRACT = 0x10,     /**< Request rejected by port partner with no previous
                                                             explicit contract. */
    CY_PDSTACK_CONTRACT_PS_READY_NOT_RECEIVED       = 0x14,     /**< Failed to receive PS_RDY after Accept. */
    CY_PDSTACK_CONTRACT_PS_READY_NOT_SENT           = 0x18      /**< Failed to send PS_RDY after Accept. */
} cy_en_pdstack_contract_status_t;

/**
 * @typedef cy_en_pdstack_app_evt_t
 * @brief Enum of events that are signalled to the application.
 */
typedef enum
{
    APP_EVT_UNEXPECTED_VOLTAGE_ON_VBUS,         /**< 0x00: Unexpected high voltage seen on VBus. */
    APP_EVT_TYPE_C_ERROR_RECOVERY,              /**< 0x01: Type-C error recovery initiated. */
    APP_EVT_CONNECT,                            /**< 0x02: Type-C connect detected. */
    APP_EVT_DISCONNECT,                         /**< 0x03: Type-C disconnect(detach) detected. */
    APP_EVT_EMCA_DETECTED,                      /**< 0x04: Cable (EMCA) discovery successful. */
    APP_EVT_EMCA_NOT_DETECTED,                  /**< 0x05: Cable (EMCA) discovery timed out. */
    APP_EVT_ALT_MODE,                           /**< 0x06: Alternate mode related event. */
    APP_EVT_APP_HW,                             /**< 0x07: MUX control related event. */
    APP_EVT_BB,                                 /**< 0x08: Billboard status change. */
    APP_EVT_RP_CHANGE,                          /**< 0x09: Rp termination change detected. */
    APP_EVT_HARD_RESET_RCVD,                    /**< 0x0A: Hard Reset received. */
    APP_EVT_HARD_RESET_COMPLETE,                /**< 0x0B: Hard Reset processing completed. */
    APP_EVT_PKT_RCVD,                           /**< 0x0C: New PD message received. */
    APP_EVT_PR_SWAP_COMPLETE,                   /**< 0x0D: PR_SWAP process completed. */
    APP_EVT_DR_SWAP_COMPLETE,                   /**< 0x0E: DR_SWAP process completed. */
    APP_EVT_VCONN_SWAP_COMPLETE,                /**< 0x0F: VConn_SWAP process completed. */
    APP_EVT_SENDER_RESPONSE_TIMEOUT,            /**< 0x10: Sender response timeout occurred. */
    APP_EVT_VENDOR_RESPONSE_TIMEOUT,            /**< 0x11: Vendor message response timeout occurred. */
    APP_EVT_HARD_RESET_SENT,                    /**< 0x12: Hard Reset sent by PMG1. */
    APP_EVT_SOFT_RESET_SENT,                    /**< 0x13: Soft Reset sent by PMG1. */
    APP_EVT_CBL_RESET_SENT,                     /**< 0x14: Cable Reset sent by PMG1. */
    APP_EVT_PE_DISABLED,                        /**< 0x15: PE.Disabled state entered. */
    APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE,   /**< 0x16: Contract negotiation completed. */
    APP_EVT_VBUS_OVP_FAULT,                     /**< 0x17: VBus Over Voltage fault detected. */
    APP_EVT_VBUS_OCP_FAULT,                     /**< 0x18: VBus Over Current fault detected. */
    APP_EVT_VCONN_OCP_FAULT,                    /**< 0x19: VConn Over Current fault detected. */
    APP_EVT_VBUS_PORT_DISABLE,                  /**< 0x1A: PD port disable completed. */
    APP_EVT_TYPEC_STARTED,                      /**< 0x1B: PD port enable (start) completed. */
    APP_EVT_FR_SWAP_COMPLETE,                   /**< 0x1C: FR_SWAP process completed. */
    APP_EVT_TEMPERATURE_FAULT,                  /**< 0x1D: Over Temperature fault detected. */
    APP_EVT_HANDLE_EXTENDED_MSG,                /**< 0x1E: Extended message received and needs to be handled. */
    APP_EVT_VBUS_UVP_FAULT,                     /**< 0x1F: VBus Under Voltage fault detected. */
    APP_EVT_VBUS_SCP_FAULT,                     /**< 0x20: VBus Short Circuit fault detected. */
    APP_EVT_TYPEC_ATTACH_WAIT,                  /**< 0x21: Type-C AttachWait state entered. */
    APP_EVT_TYPEC_ATTACH_WAIT_TO_UNATTACHED,    /**< 0x22: Type-C transition from AttachWait to Unattached. */
    APP_EVT_TYPEC_ATTACH,                       /**< 0x23: Type-C attach event. */
    APP_EVT_CC_OVP,                             /**< 0x24: Over Voltage on CC/VConn line detected. */
    APP_EVT_SBU_OVP,                            /**< 0x25: Over Voltage on SBU1/SBU2 line detected. */
    APP_EVT_ALERT_RECEIVED,                     /**< 0x26: Alert message received. */
    APP_EVT_SRC_CAP_TRIED_WITH_NO_RESPONSE,     /**< 0x27: Src Cap tried with no response. */
    APP_EVT_PD_SINK_DEVICE_CONNECTED,           /**< 0x28: Sink device connected. */
    APP_EVT_VBUS_RCP_FAULT,                     /**< 0x29: VBus Reverse Current fault detected. */
    APP_EVT_STANDBY_CURRENT,                    /**< 0x2A: Standby Current. */
    APP_EVT_DATA_RESET_RCVD,                    /**< 0x2B: USB4 Data Reset message received. USB connection
                                                           should be disabled by DFP on receiving this event. */
    APP_EVT_DATA_RESET_SENT,                    /**< 0x2C: USB4 Data Reset message sent. USB connection should
                                                           be disabled by DFP on receiving this event. */
    APP_EVT_DATA_RESET_CPLT,                    /**< 0x2D: USB4 Data Reset process complete. No handling required. */
    APP_EVT_USB_ENTRY_CPLT,                     /**< 0x2E: USB4 entry process complete. */
    APP_EVT_DATA_RESET_ACCEPTED,                /**< 0x2F: USB4 Data Reset Accepted. USB connection can be
                                                           re-enabled by DFP on receiving this event. */
    APP_EVT_CONFIG_ERROR,                       /**< 0x30: Stack configuration error event. */
    APP_EVT_POWER_CYCLE,                        /**< 0x31: Power cycle / Reset event. */
    APP_EVT_VBUS_IN_UVP_FAULT,                  /**< 0x32: Vbus_in undervoltage fault detected. */
    APP_EVT_VBUS_IN_OVP_FAULT,                  /**< 0x33: Vbus_in overvoltage fault detected. */
    APP_EVT_SYSTEM_OT_FAULT,                    /**< 0x34: System overtemperature fault detected. */
    APP_EVT_CRC_ERROR,                          /**< 0x35: PD CRC error detected. */
    APP_EVT_HR_PSRC_ENABLE,                     /**< 0x36: PSRC enable is about to be called after Hard reset. */

    APP_EVT_TYPEC_RP_DETACH,                    /**< 0x37: Rp removal detected while in the Attached.SNK state. */
    APP_EVT_PR_SWAP_ACCEPTED,                   /**< 0x38: PR-SWAP accepted by PMG1 or port partner. */
    APP_EVT_HR_SENT_RCVD_DEFERRED,              /**< 0x39: Deferred Hard Reset Sent/Received event handling to
                                                           accommodate retimer communication delay timing. */
    APP_EVT_BAD_SINK_APDO_SEL,                  /**< 0x3A: APDO selection in PD 2.0 or less revision */
    APP_TOTAL_EVENTS                            /**< 0x3B: Total number of application events. */
} cy_en_pdstack_app_evt_t;

/**
 * @typedef cy_en_pdstack_ams_type_t
 * @brief Type of USB-PD Atomic Message Sequence (AMS).
 */
typedef enum
{
    CY_PDSTACK_AMS_NONE = 0,                            /**< No AMS active. */
    CY_PDSTACK_AMS_NON_INTR,                            /**< Non-interruptible AMS is active. */
    CY_PDSTACK_AMS_INTR                                 /**< Interruptible AMS is active. */
} cy_en_pdstack_ams_type_t;

/**
 * Enum to Hold expected response result
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
    CY_PDSTACK_VDM_AMS_RESP_READY = 0,                     /**< Response is ready */
    CY_PDSTACK_VDM_AMS_RESP_NOT_REQ,                       /**< No response required */
    CY_PDSTACK_VDM_AMS_RESP_FROM_EC,                       /**< Response will come from EC */
    CY_PDSTACK_VDM_AMS_RESP_NOT_SUPP                       /**< Send a NOT_SUPPORTED response. */
} cy_en_pdstack_vdm_ams_t;

/**
 * @typedef cy_en_pdstack_usb_data_sig_t
 * @brief Enumeration of USB signalling supported by a device or cable.
 */
typedef enum
{
    CY_PDSTACK_USB_2_0_SUPP = 0,                           /**< Only USB 2.0 support. */
    CY_PDSTACK_USB_GEN_1_SUPP,                             /**< USB 3.1 Gen1 (5 Gbps) support. */
    CY_PDSTACK_USB_GEN_2_SUPP,                             /**< USB 3.1 Gen2 (10 Gbps) support. */
    CY_PDSTACK_USB_GEN_3_SUPP,                             /**< USB 4 Gen3 (20 Gbps) support. */
    CY_PDSTACK_USB_BB_SUPP,                                /**< USB Billboard device support. */
    CY_PDSTACK_USB_SIG_UNKNOWN                             /**< USB data signalling support unknown. */
} cy_en_pdstack_usb_data_sig_t;

/**
 * @typedef cy_en_pdstack_data_reset_state_t
 * @brief Enumeration of sub-states associated with Data Reset AMS.
 */
typedef enum
{
    CY_PDSTACK_DATA_RESET_IDLE = 0,                        /**< No Data_Reset related operation pending. */
    CY_PDSTACK_DATA_RESET_WAIT_ACCEPT,                     /**< Sender waiting for Data_Reset Acceptance. */
    CY_PDSTACK_DATA_RESET_ACCEPTED,                        /**< Waiting for next step after sending Accept response. */
    CY_PDSTACK_DATA_RESET_WAIT_PS_RDY,                     /**< Waiting for PS_RDY at the end of Data_Reset handshake. */
    CY_PDSTACK_DATA_RESET_WAIT_VCONN_OFF,                  /**< Waiting for VConn turn-off completion before sending PS_RDY. */
    CY_PDSTACK_DATA_RESET_SENDING_PS_RDY,                  /**< In the process of sending PS_RDY after Data_Reset handshake. */
    CY_PDSTACK_DATA_RESET_WAIT_VCONN_ON,                   /**< DFP waiting to turn VConn ON. */
    CY_PDSTACK_DATA_RESET_WAIT_COMPLETION,                 /**< UFP waiting for Data_Reset completion. */
    CY_PDSTACK_DATA_RESET_COMPLETE_DELAY                   /**< DFP waiting to send Data_Reset_Complete message. */
} cy_en_pdstack_data_reset_state_t;

/**
 * @typedef cy_en_pdstack_pdo_sel_alg_t
 * @brief Algorithm selection for pdo evaluation.
 * Only fixed SRC_PDOs take part for current and voltage algorithms.
 */
typedef enum
{
    CY_PDSTACK_HIGHEST_POWER = 1,                           /**< Algorithm to select contract with highest power. */
    CY_PDSTACK_HIGHEST_CURRENT,                             /**< Algorithm to select contract with highest current. */
    CY_PDSTACK_HIGHEST_VOLTAGE                              /**< Algorithm to select contract with highest voltage. */
} cy_en_pdstack_pdo_sel_alg_t;

/**
 * @brief Interface status codes
 *
 * Enumeration to hold status codes for all PMG1 interfaces. These values
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
    CY_PDSTACK_STAT_INVALID_COMMAND = 3,       /**< Operation failed due to invalid command. */
    CY_PDSTACK_STAT_FLASH_UPDATE_FAILED = 5,   /**< Flash write operation failed. */
    CY_PDSTACK_STAT_INVALID_FW,                /**< Special status code indicating invalid firmware */
    CY_PDSTACK_STAT_INVALID_ARGUMENT,          /**< Operation failed due to invalid arguments. */
    CY_PDSTACK_STAT_NOT_SUPPORTED,             /**< Feature not supported. */
    CY_PDSTACK_STAT_INVALID_SIGNATURE,         /**< Invalid signature parameter identified. */
    CY_PDSTACK_STAT_TRANS_FAILURE,             /**< Transaction failure status. */
    CY_PDSTACK_STAT_CMD_FAILURE,               /**< Command failure status */
    CY_PDSTACK_STAT_FAILURE,                   /**< Generic failure status. */
    CY_PDSTACK_STAT_READ_DATA,                 /**< Special status code indicating read data
                                             availability. */
    CY_PDSTACK_STAT_NOT_READY,                 /**< Operation failed due to device/stack not ready. */
    CY_PDSTACK_STAT_BUSY,                      /**< Operation failed due to device/stack busy status. */
    CY_PDSTACK_STAT_TIMEOUT,                   /**< Operation timed out. */
    CY_PDSTACK_STAT_INVALID_PORT               /**< Invalid port number. */
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
        uint16_t dataSize  : 9;                /**< Bits 08:00 - Extended message size in bytes. */
        uint16_t rsvd1      : 1;                /**< Bit     09 - Reserved. */
        uint16_t request    : 1;                /**< Bit     10 - Chunk request. */
        uint16_t chunkNum   : 4;                /**< Bits 14:11 - Chunk number. */
        uint16_t chunked    : 1;                /**< Bit     15 - Chunked message. */
    } extd;                                     /**< Extended header broken down into respective fields. */
} cy_pdstack_extd_hdr_t;

/**
 * @brief PD port status corresponding to the Status Data Block (SSDB)
 * See Table 6-39 of USB-PD R3 specification.
 */
typedef struct
{
    uint8_t  intlTemperature;                  /**< Port's internal temperature. 0 if not supported. */
    uint8_t  presentInput;                     /**< Reports current input power status. */
    uint8_t  batteryInput;                     /**< Reports the current battery status. */
    uint8_t  eventFlags;                       /**< Event flags. */
    uint8_t  tempStatus;                       /**< Temperature status. */
    uint8_t  powerStatus;                      /**< Power status. */
    uint8_t  dummy[2];                          /**< Reserved field used for 4 byte alignment. */
} cy_stc_pdstack_pd_power_status_t;

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
    cy_pd_pd_do_t     respBuf[CY_PD_MAX_NO_OF_DO]; /**< Data objects buffer */
    uint8_t     doCount;               /**< Data objects count */
    cy_en_pdstack_vdm_ams_t   noResp;                /**< Response type. */
} vdm_resp_t;

/**
 * @brief Struct to hold PD command buffer.
 * @warning When providing pointer to the extended data make sure original buffer
 * is always 4-byte aligned. i.e, even if 1 byte data is required, 4 bytes should be used to
 * store that data.
 */
typedef struct
{
    cy_en_pd_sop_t       cmdSop;                /**< SOP type */
    cy_en_pdstack_extd_msg_t                extdType;              /**< Extended Message Type */
    cy_pdstack_extd_hdr_t extdHdr;               /**< Extended Header */
    uint8_t                   noOfCmdDo;           /**< No of data objects including VDM header */
    uint8_t*                  datPtr;                /**< Data Pointer in case of extended message only*/
    uint8_t                   timeout;                /**< Timeout value, in ms, for a response.
                                                       *   If set to zero, the PD stack will not wait for a VDM response
                                                       *   and jump to ready state after this buffer has been sent.
                                                       */
    cy_pd_pd_do_t                   cmdDo[CY_PD_MAX_NO_OF_DO];   /**< Command data objects. */
} cy_stc_pdstack_dpm_pd_cmd_buf_t;

/**
 * @brief Structure to hold PD Contract information.
 */
typedef struct
{
    uint16_t curPwr;           /**< PD contract current/power. */
    uint16_t maxVolt;          /**< PD contract max voltage in mV */
    uint16_t minVolt;          /**< PD contract min voltage in mV */
} cy_stc_pdstack_contract_t;

/**
 * @brief Structure to hold PD Contract information passed with APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE
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
    uint8_t     sop;                    /**< Packet type. */
    uint8_t     len;                    /**< Length in data objects. */
    uint8_t     msg;                    /**< Message code. */
    uint8_t     dataRole;              /**< Data role. */
    cy_pd_pd_hdr_t    hdr;                    /**< Message header. */
    cy_pd_pd_do_t     dat[CY_PD_MAX_NO_OF_DO];      /**< Data objects associated with the message. */
} cy_stc_pdstack_pd_packet_t;

/* Forward declarations of structures. */
struct cy_stc_pdstack_context;

/**
 * PD callback prototype.
 * This is a stack internal callback function used by the USB-PD Protocol
 * layer to send events to the Policy Engine. The events notified correspond
 * to Policy Engine events such as HARD RESET or SOFT RESET received.
 *
 * \param ptrPdStackContext
 * PdStack Library Context pointer.
 *
 * @param Type of event being notified.
 */
typedef void (*cy_pdstack_pd_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, uint32_t event);

/**
 * @brief DPM PD command callback. This is the type of callback function used by the Policy Engine
 * to report results of a command to the application layer.
 *
 * @param port PD port index.
 * @param resp Response code.
 * @param pkt_ptr Pointer to any PD packet associated with the response.
 */
typedef void (*cy_pdstack_dpm_pd_cmd_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, cy_en_pdstack_resp_status_t resp, const cy_stc_pdstack_pd_packet_t* pkt_ptr);

/**
 * @brief Application response callback. This is the type of callback used by the stack to
 * receive application level responses associated with a PD message such as a SWAP request.
 *
 * @param port PD port index.
 * @param resp Pointer to the structure holding response information.
 */
typedef void (*cy_pdstack_app_resp_cbk_t)(struct cy_stc_pdstack_context *ptrPdStackContext, app_resp_t* resp);

/**
 * @brief VDM response callback. This is the type of callback used by the stack to receive
 * application level responses to a VDM received from the port partner or cable marker.
 *
 * @param port PD port index.
 * @param resp Pointer to structure holding response information.
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
 * notification that the sink discharge circuit has been turned off.
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
 * fill the structure with pointers to functions that use the on-board circuitry to
 * accomplish tasks like source/sink power turn on/off. All the functions in this
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
            );                          /**< Set power source current in 10mA units. */

    void (*psrc_enable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_pwr_ready_cbk_t pwr_ready_handler   /**< Function to be called after power enable. */
            );                          /**< Enable the power supply. The pwr_ready_handler, if not NULL, must be
                                         *   called when the power supply is ready.
                                         */

    void (*psrc_disable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_pwr_ready_cbk_t pwr_ready_handler   /**< Function to be called after power disable. */
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
            );                          /**< Check whether VConn supply is ON. */

    bool (*vbus_is_present) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t volt,              /**< Expected voltage in mV units. */
            int8_t per                  /**< Allowed voltage deviation in percentage units. */
            );                          /**< Check whether VBus voltage is within expected range. */

    void (*vbus_discharge_on) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Turn on VBUS discharge circuit. */

    void (*vbus_discharge_off) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Turn off VBUS discharge circuit. */

#if (!(CY_PD_SOURCE_ONLY))
    void (*psnk_set_voltage) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t volt_mV            /**< Target voltage in mV units. */
            );                          /**< Set power sink voltage, in mV units. */

    void (*psnk_set_current) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            uint16_t cur_10mA           /**< Operating current in 10 mA units. */
            );                          /**< Set power sink current, in 10mA units. */

    void (*psnk_enable) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Enable power sink related circuitry. */

    void (*psnk_disable) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_sink_discharge_off_cbk_t snk_discharge_off_handler  /**< Callback to be called after discharge is done. */
            );                          /**< Disable power sink related circuitry. */

    void (*eval_src_cap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            const cy_stc_pdstack_pd_packet_t* srcCap, /**< Pointer to list of received source caps. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Evaluate received source caps and provide the RDO to be used
                                             to negotiate contract. */
#endif /* (!(CY_PD_SOURCE_ONLY)) */

#if (!CY_PD_SINK_ONLY)
    void (*eval_rdo)(
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pd_pd_do_t rdo,                /**< Received RDO object. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Evaluate sink request message. */
#endif /* (!CY_PD_SINK_ONLY) */

    void (*eval_dr_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handles DR swap request received by port. */
    void (*eval_pr_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handles pr swap request received by port. */

    void (*eval_vconn_swap) (
            struct cy_stc_pdstack_context *ptrPdStackContext,               /**< PD port index. */
            cy_pdstack_app_resp_cbk_t app_resp_handler     /**< Return parameter to report response through. */
            );                          /**< Handles VCONN swap request received by port. */

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
            );                          /**< Get current VBUS value in mV from application. */

#if (!CY_PD_SINK_ONLY)
    uint32_t (*psrc_get_voltage) (
            struct cy_stc_pdstack_context *ptrPdStackContext                /**< PD port index. */
            );                          /**< Get expected VBUS value in mV from application. This is to include any
                                              additional compensation done for drops. */
#endif /* (!CY_PD_SINK_ONLY) */
} cy_stc_pdstack_app_cbk_t;

/**
 * @brief PD Device Policy Status structure. This structure holds all of the
 * configuration and status information associated with a port on the PMG1
 * device. Members of this structure should not be directly modified by any of
 * the application code.
 */
typedef struct
{
    /** Port role: Sink, Source or Dual. */
    uint8_t portRole;

    /** Default port role: Sink or Source. */
    uint8_t dfltPortRole;

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

    /** Sink PDO count from the config table or updated at runtime by the EC. */
    uint8_t snkPdoCount;

    /** Sink PDO mask from the config table or updated at runtime by the EC. */
    uint8_t snkPdoMask;

    /** Supported Rp values bit mask.
      - Bit 0 => Default Rp supported.
      - Bit 1 => 1.5 A Rp supported.
      - Bit 2 => 3 A Rp supported.
      */
    uint8_t rpSupported;

    /** USB-PD supported. */
    uint8_t pdSupport;

    /** Try Source/ Try Sink control knob. */
    uint8_t trySrcSnk;

    /** Dead battery support control knob. */
    uint8_t dbSupport;

    /** Error recovery control knob.*/
    bool errRecov;

    /** PD port disable flag. */
    uint8_t portDisable;

    /** FRS enable flags. */
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

    /** B29:B20 of the first 5V Fixed Source PDO */
    uint8_t srcPdoFlags[2];

    /** B29:B20 of the first 5V Fixed Sink PDO */
    uint8_t snkPdoFlags[2];

    /** Flag to indicate chip bootup, used to check dead battery. */
    bool volatile bootup;

    /** Flag to indicate dead battery operation. */
    bool volatile deadBat;

    /** Time period for DRP toggling. */
    uint8_t drpPeriod;

    /** Time period for which to stay as a SRC for a DRP device. */
    uint8_t srcPeriod;

    /** Time period for which to stay as a SNK for a DRP device. */
    uint8_t snkPeriod;

    /** Port role when the port moved to the attached state. */
    cy_en_pd_port_role_t roleAtConnect;

    /** Ports are PD connected indication. */
    bool volatile pdConnected;

    /** PD disabled indication. */
    bool volatile pdDisabled;

    /** Ra present indication. */
    bool volatile raPresent;

    /** BIST Carrier Mode 2 going on */
    bool volatile bistCm2Enabled;

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

    /** Type C generic FSM state. */
    cy_en_pdstack_typec_fsm_state_t typecFsmState;

    /** Current DPM PD command. */
    cy_en_pdstack_dpm_pd_cmd_t dpmPdCmd;

    /** Indicate DPM PD command was registered. */
    bool dpmPdCmdActive;

    /** Indicate DPM Type C command was registered. */
    bool dpmTypecCmdActive;

    /** DPM Initialized flag. */
    bool dpmInit;

    /** DPM safe disable flag. Used to make sure that the port is disabled
     * correctly after a fault occurrence. */
    uint8_t dpmSafeDisable;

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

    /** Holds the current state of Policy Engine (PE). */
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
    uint16_t curSnkMaxMin[CY_PD_MAX_NO_OF_PDO];

    /** Current source PDOs sent in source cap messages. */
    cy_pd_pd_do_t curSrcPdo[CY_PD_MAX_NO_OF_PDO];

    /** Current sink PDOs sent in sink cap messages. */
    cy_pd_pd_do_t curSnkPdo[CY_PD_MAX_NO_OF_PDO];

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

    /** Selected PDO which resulted in contract (when sink). */
    cy_pd_pd_do_t snkSelPdo;

    /** Selected PDO which resulted in contract (when source). */
    cy_pd_pd_do_t srcSelPdo;

    /** Pointer to the current/last source cap message received. May be NULL.
     * Data pointed to by this should not be changed. */
    cy_stc_pdstack_pd_packet_t *srcCapP;

    /** Fields below need to be properly aligned to 4 byte boundary */
    uint32_t padval;

    /** Port power status. */
    cy_stc_pdstack_pd_power_status_t portStatus;

    /** Buffer to hold extended source caps. */
    uint8_t extSrcCap[CY_PD_EXT_SRCCAP_SIZE];

    /** Buffer to hold PPS status. */
    uint8_t ppsStatus[CY_PD_EXT_PPS_STATUS_SIZE];

    /** Additional error status for DPM commands. */
    uint8_t dpmErrInfo;

    /** Whether SRC PDOs have been limited due to cable capability. */
    bool    pwrLimited;

    /** Stores the last received Active Cable VDO #2. */
    cy_pd_pd_do_t cblVdo2;

    /** Buffer to hold extended sink caps. */
    uint8_t extSnkCap[CY_PD_EXT_SNKCAP_BUF_SIZE];

    /** Temporary variable used for random number generation. */
    uint32_t randBase;

    /** Whether we should keep Rp at SinkTxNG as a PD 3.0 Source. */
    bool    pd3SrcCcBusy;

    /** Checks whether the solution state allows Type-C transitions */
    bool    isSolnBusy;

#if DPM_DEBUG_SUPPORT
    /** RDO that has been accepted last. */
    cy_pd_pd_do_t activeRdo;

    /** Stores the contract flags. */
    uint8_t contractFlags;

    /** Stores the source PDO flags */
    uint8_t srcPdoFlags;

    /** Reason for port entering Type-C error recovery. */
    uint8_t errRecovReason;

    /** Reason for port issuing SOP'' SoftReset. */
    uint8_t sopdpSoftResetReason;

    /** Reason for port issuing SOP' SoftReset. */
    uint8_t soppSoftResetReason;

    /** Reason for port issuing Cable Reset. */
    uint8_t cableResetReason;

    /** Reason for port issuing Hard Reset. */
    uint8_t hardResetReason;

    /** Reason for port issuing Soft Reset. */
    uint8_t softResetReason;

    /** Whether SOP'' cable controller is present. */
    uint8_t sopdpPresent;

    /* Debug counters  */
    /** Number of connections since power-up. */
    uint8_t  connectionCount;

    /** Number of faults in current connection. */
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

    /** Flag indicating that Hardware based DRP toggling is enabled. */
    uint8_t hwDrpToggleEn;

    /** Flag indicating that Try.SRC Try.SNK is disabled. */
    uint8_t trySrcSnkDis;

    /** Flag indicating that FRS as Initial Sink is supported. */
    uint8_t frsRxEn;

    /** Flag indicating that FRS as Initial Source is supported. */
    uint8_t frsTxEn;

    /** Flag indicating that PPS source is supported. */
    uint8_t ppsSrcEn;

    /** Flag indicating that USB4 messages are supported. */
    uint8_t usb4En;

    /** Flag used Rp Detach detect. */
    bool typecRpDetachDisabled;

    /** Flag for accessory support */
    bool typecAccessorySuppDisabled;

    /** Number of cable discovery attempts to be made
     *  Range: 0x00 - 0x14 */
    uint8_t cableDiscCount;

    /** Length of Manufacturer Information.
     * 0 - For no information.
     * 5 to 26: For valid information. */
    uint8_t mfgLenInfo;

    /** Variable added for padding to a DWORD aligned address. */
    uint32_t dWordPad0;

    /** Manufacturer Name. Null terminated string. Should be 4-byte aligned. */
    uint8_t mfgName[26];

} cy_stc_pdstack_dpm_status_t;

/**
 * @brief Type C Manager Status structure. This structure holds all of the
 * configuration and status information associated with the Type C Manager.
 * Members of this structure should not be directly modified by any of the
 * application code.
 */
typedef struct
{
#if (!(CY_PD_SOURCE_ONLY))
    /** Flag to indicate start of drp random timeout update cycle */
    uint8_t randUpdt;

    /** Flag to indicate rd debounce reset */
    bool volatile rdDebounce;
#endif /* (!(CY_PD_SOURCE_ONLY)) */

    /** Flag to indicate power source enable is going on */
    bool psrcEn;

    /** Flag to indicate unexpected vbus detected by source before applying vbus */
    bool vbusUnexpected;

    /** Flag to indicate port disable is going on */
    bool portDisable;

    /** Current period (sink or source) */
    uint8_t curPeriod;

    /** Flag to indicate toggle timer On */
    bool volatile toggleOn;

    /** Flag to indicate attempt to discharge if vbus seen before applying vbus is over */
    bool volatile discharge;
#if (!(CY_PD_TRY_SRC_SNK_DISABLE))
    /** Flag to indicate try snk tDRPTry timer is running */
    bool volatile trySnkDeadTime;
#endif /* (!(CY_PD_TRY_SRC_SNK_DISABLE)) */

    /** Flag to indicate cc status debounced to avoid race condition in going to deepsleep */
    bool volatile ccStatusDebounced[2];

    /** Flag to indicate that source disable operation is in progress. */
    bool volatile srcDisableWait;

} cy_stc_pdstack_typec_status_t;

/**
 * @brief Struct to hold PD protocol message IDs and flags. Separate structures need to be
 * maintained for each packet type (SOP, SOP' and SOP'').
 */
typedef struct cy_stc_pdstack_prl_cntrs
{
    /** The message ID to be used on the next transmitted message. */
    uint8_t trMsgId;

    /** The message ID of last received message. */
    uint8_t recMsgId;

    /** Flag that indicates whether any message has been received so far. */
    bool volatile firstMsgRcvd;

} cy_stc_pdstack_prl_cntrs_t;

/**
 * @brief Structure to hold protocol layer status.
 */
typedef struct cy_stc_pdstack_pd_status
{
    /** Callback used to send notifications to Policy Engine. */
    cy_pdstack_pd_cbk_t cbk;

    /** Protocol counters for various packet types. */
    cy_stc_pdstack_prl_cntrs_t  ctrs[CY_PD_MAX_SOP_TYPES];

    /** Buffer used to hold message to be (or being) transmitted. */
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

    /** Stores the SOP type of last transmitted message. */
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

    /** Flag indicating that FRS receive support is enabled. */
    volatile bool frsRxEnable;

} cy_stc_pdstack_pd_status_t;

/**
 * @brief Structure to hold the Policy Engine Status.
 */
typedef struct {
    /** Pointer to received packet */
    cy_stc_pd_packet_extd_t*    rcvdPtr;
    /** Currently received packet */
    cy_stc_pd_packet_extd_t rcvdPkt;
    /** Eval VDM local buffer */
    cy_stc_pdstack_pd_packet_t vdmPkt;
    /** Stores the DPM command response */
    cy_stc_pdstack_pd_packet_t dpmResp;
    /** Stores the current source cap received */
    cy_stc_pdstack_pd_packet_t srcCap;
    /** Temporary buffer for sending VDM commands */
    cy_pd_pd_do_t tempDo;
    /** Temporary buffer for storing APP response */
    app_resp_t appResp;
    /** VDM response from APP stored here */
    vdm_resp_t vdmResp;
    /** VCONN swap command buffer */
    cy_stc_pdstack_dpm_pd_cmd_buf_t  vsBuf;
    /** Cable command buffer */
    cy_stc_pdstack_dpm_pd_cmd_buf_t  cblBuf;
    /** Hard reset counter */
    uint8_t  hardResetCount;
    /** Source capability counter */
    uint8_t   srcCapCount;
    /** CBL dsc id count */
    uint8_t cblDscIdCount;
    /** Indicate power role swap going on */
    bool prSwap;
    /** Counts PR swap requests if VCONN swap is pending */
    uint16_t prSwapCount;
#if (!(CY_PD_SOURCE_ONLY))
    /** Indicate go to min request received from source */
    bool snkGoToMin;
#endif /* (!(CY_PD_SOURCE_ONLY)) */
    /** Indicate go to min need to be sent by source(requested by dpm) */
    bool srcGoToMin;
    /** Indicate VCONN swap need to be performed */
    bool isVconnSwapActive;
    /** Current count of VCONN Swap tries */
    uint8_t vsCount;
    /** Flag to indicate waiting for response */
    bool vsWait;
    /** Flag to indicate AMS is continued from previous transition */
    pd_ams_type_t amsType;
    /** Stores send message sop type */
    cy_en_pd_sop_t sendMsgSop;
    /** Stores send message class */
    cy_en_pdstack_pd_msg_class_t sendMsgClass;
    /** Send message type */
    uint8_t sendMsgType;
    /** Send message count */
    uint8_t sendMsgCount;
    /** Send extended header */
    cy_pdstack_extd_hdr_t sendextdhdr;
    /** Send message timeout */
    uint16_t sendMsgTimeout;
    /** Send message data pointer */
    void* sendMsgPtr;
    /** Stores expected message class */
    cy_en_pdstack_pd_msg_class_t expMsgClass;
    /** Expected message mask */
    uint32_t expMsgMask;
    /** Expected message length */
    uint16_t expMsgLen;
    /** Temporary buffer for APP req response*/
    cy_en_pdstack_app_req_status_t appReqRslt;
    /** Temporary contract status */
    cy_stc_pdstack_contract_t contract;
    /** Temporary rdo sent by sink */
    cy_pd_pd_do_t snkRdo;
    /** Temporary Selected pdo(among source cap) by sink */
    cy_pd_pd_do_t snkSelPdo;
    /** Temporary Selected pdo(among source cap) of source */
    cy_pd_pd_do_t srcSelPdo;
    /** Temporary store for status of app swap complete event */
    cy_en_pdstack_app_req_status_t swapEvtResp;
    /** Temporary store for app PD contract event status */
    cy_stc_pdstack_pd_contract_info_t contractEvtResp;
    /** Temporary variable to hold sop type */
    cy_en_pd_sop_t sopType;
    /** Flag to indicate PSHardResetDelay required */
    bool psHrDelay;
    /** Flag determines srccap start debounce in progress */
    uint8_t sendSrcCapDebounce;
    /** Store received/sent swap type */
    cy_en_pd_ctrl_msg_t swapType;

#if CY_PD_REV3_ENABLE

    /** Flag to indicate spec rev has been determined */
    bool specRevDetermined;

    /** Flag to indicate spec rev has been determined */
    bool chunkDetermined;

    /** Temporary variable to hold extended message type to send */
    cy_en_pdstack_extd_msg_t autoExtdMsgType;

    /** Temporary store AMS type */
    cy_en_pdstack_ams_type_t amsTypeTemp;

#if CY_PD_FRS_RX_ENABLE
    /** Flag to indicate waiting for response */
    bool getSnkCapWait;
    /** Indicate Get Sink Cap need to be performed */
    bool isGetSnkCapActive;
    /** Current count of Get Sink Cap tries */
    uint8_t getSnkCapCount;
#endif /* CY_PD_FRS_RX_ENABLE */

#endif /* CY_PD_REV3_ENABLE */

    /** Flag indicating PSRC enable call has been delayed. */
    bool pePsrcEnDelayed;

    /** Flag that indicates that Hard Reset sending is pending. */
    volatile bool peSendHrPending;

} cy_stc_pdstack_pe_status_t;

/**
 * @brief Structure to hold Device Policy Manager Parameters.
 */
typedef struct
{
    /** Default current setting in 10mA unit (3A is represented as 300). */
    uint16_t defCur;

    /** The tTypeCSnkWaitCap period to be used, in ms. */
    uint16_t typeCSnkWaitCapPeriod;

    /** Delay to be provided (in ms) between MUX enable and VBus turning ON. */
    uint16_t muxEnableDelayPeriod;

    /** Flag for default cable capabilities. */
    uint16_t dpmDefCableCap;

    /** Flag for holding the cc termination current level for audio accessory. */
    cy_en_pd_rp_term_t dpmRpAudioAcc;

    /** Variable holding the Sink wait cap period. */
    uint16_t dpmSnkWaitCapPeriod;

} cy_stc_pdstack_dpm_params_t;

/**
 * @brief Structure to PDSTACK Middleware context information.
 */
typedef struct cy_stc_pdstack_context
{
    /** USBPD port Index. **/
    uint8_t port;

    /** PD Device Policy Configuration and Status structure. */
    cy_stc_pdstack_dpm_status_t dpmStat;

    /** PD Device Policy Configuration and Status structure. */
    cy_stc_pd_dpm_config_t dpmConfig;

    /** Pointer to the application callback structure. */
    cy_stc_pdstack_app_cbk_t *ptrAppCbk;

    /** Protocol layer status structure. */
    cy_stc_pdstack_pd_status_t pdStat;

    /** Policy Engine status structure. */
    cy_stc_pdstack_pe_status_t peStat;

    /** Type c Status. */
    cy_stc_pdstack_typec_status_t typecStat;

    /** Pointer to the Port Configuration structure. */
    const cy_stc_pdstack_port_cfg_t *ptrPortCfg;

    /** Pointer to the USBPD PDL context information. */
    cy_stc_usbpd_context_t *ptrUsbPdContext;

    /** Structure members for the various parameters that dpm update functions
     * are updating. */
    const cy_stc_pdstack_dpm_params_t *ptrDpmParams;

    /** PdStack Timer Context */
    cy_stc_sw_timer_t *ptrTimerContext;
} cy_stc_pdstack_context_t;


/** \} group_pdstack_data_structures */

#endif /* CY_PDSTACK_COMMON_H */

/* [] END OF FILE */
