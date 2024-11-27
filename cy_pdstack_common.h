/***************************************************************************//**
* \file cy_pdstack_common.h
* \version 4.0
*
* Common header file of the PDStack middleware.
*
********************************************************************************
* \copyright
* Copyright 2021-2024, Cypress Semiconductor Corporation. All rights reserved.
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
* the ModusToolbox(TM) CAT2 (mtb-pdl-cat2) Peripheral Driver Library (PDL).
*
* <b>Features:</b>
* 1. Supports Type-C port configuration and connection detection in sink,
* source, and dual roles 
* 2. Supports Extended Power Range (EPR) in source and sink roles
* 3. Supports Liquid Corrosion Mitigation mode
* 4. Support USB PD communication with the following features:
*    * USB PD revision 3.2 compliance
*    * USB PD revision 2.0 compliance (backward compatibility)
*    * Power contract negotiation in sink and source roles
*    * Ability to respond to SOP and SOP' messages to support designs with a
*    captive USB-C cable
* 5. Supports configuration of the port roles and responses through the EZ-PD(TM)
* Configurator tool
* 6. Support DPM APIs to initialize, configure, monitor, and
* control the port operation and send/receive USB PD messages
* associated with alternate mode discovery, authentication, firmware update,
* etc.
*
********************************************************************************
* \section section_pdstack_general_description General description
********************************************************************************
*
* Include cy_pdstack_common.h and cy_pdstack_dpm.h to
* get access to all functions and other declarations in this library. See the
* \ref section_pdstack_quick_start to start using the PDStack.
*
* See the \ref section_pdstack_toolchain section for compatibility
* information.
*
* See the \ref section_pdstack_changelog section for the change history.
*
* PDStack operates on top of the USB PD driver. The USB PD driver has
* some prerequisites for proper operation.
* See the "USB PD (USB Power Delivery)" section of the ModusToolbox(TM) CAT2 (mtb-pdl-cat2)
* PDL API reference manual.
* In addition, see the \ref section_pdstack_miscellaneous section for
* the different PDStack middleware restrictions and limitations.
*
* The PDStack middleware is released in the form of pre-compiled libraries. The
* following library variants with a different feature set to help limit the
* memory footprint of the stack in applications are released.
* 1. pmg1_pd3_snk_lite - Supports USB Type-C sink operation and
* USB PD Revision 3.2 messaging
* 2. pmg1_pd2_snk_lite - Supports USB Type-C sink operation and
* USB PD Revision 2.0 messaging. Using this library reduces the flash
* (code) memory usage by the application.
* 3. pmg1_pd3_drp_lite - Supports USB Type-C dual-role Standard Power Range
* (SPR) operation and USB PD Revision 3.2 messaging. This library supports all
* but the following features of the pmg1_pd3_drp library:
* - Try.SRC and Try.SNK
* - Cable discovery
* - VDM message handling
* - VConn
* Using this library reduces the flash (code) memory usage by the application.
* 4. pmg1_pd3_drp - Supports USB Type-C dual-role operation and
* USB PD Revision 3.2 messaging. This library can be used in any Power Delivery
* application.
* 5. pmg1_pd3_snk_epr - Supports USB Type-C sink Extended Power
* Range (EPR). This library can be used in applications that require EPR.
* 6. pmg1_pd3_drp_epr - Supports USB Type-C dual role 
* EPR operation and USB PD Revision 3.2 messaging.
* 7. pmg1_pd3_drp_epr_cfg - Supports USB Type-C dual role EPR operation, 
* USB PD Revision 3.2 messaging and configurability of stack parameters
* through the **EZ-PD&trade; Configuration Utility**.
********************************************************************************
* \section section_pdstack_quick_start Quick start guide
********************************************************************************
*
* PDStack middleware can be used in various development
* environments such as ModusToolbox(TM), Mbed OS, etc. See the
* \ref section_pdstack_toolchain section.
*
* These steps describe the simplest way of enabling the PDStack
* middleware in the application.
*
* 1. Open/create the application where to add the PDStack function.
*
* 2. Add the PDStack middleware to your project. This quick start guide
* assumes that the environment is configured to use the CAT2 PDL for development and the PDL is included in the project.
* If you are using the ModusToolbox(TM) development environment, select the
* application in the Project Explorer window, and select the PDStack middleware
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
* on/off, identifying the optimal source power profile to be used for charging,
* etc. The library also provides notification of various connections and PD
* policy state changes; therefore, the rest of the system can be configured as
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
* This section consists of instructions on how to configure and use
* the PDStack middleware in a design.
* 1. After selecting the PDStack middleware through the Library Manager, you
* can select the library variant by editing the code example Makefile:
*    * Add 'PMG1_PD3_SNK_LITE' to the COMPONENTS for using the pmg1_pd3_snk_lite library
*    * Add 'PMG1_PD2_SNK_LITE' to the COMPONENTS for using the pmg1_pd2_snk_lite library
*    * Add 'PMG1_PD3_DRP_LITE' to the COMPONENTS for using the pmg1_pd3_drp_lite library
*    * Add 'PMG1_PD3_DRP' to the COMPONENTS for using the pmg1_pd3_drp library
*    * Add 'PMG1_PD3_SNK_EPR' to the COMPONENTS for using the pmg1_pd3_snk_epr library
*    * Add 'PMG1_PD3_DRP_EPR' to the COMPONENTS for using the pmg1_pd3_drp_epr library
*    * Add 'PMG1_PD3_DRP_EPR_CFG' to the COMPONENTS for using the pmg1_pd3_drp_epr_cfg library
* 2. Configure the USB-C port properties.
*    * The USB-C port properties are controlled with the
*    cy_stc_pdstack_port_cfg_t structure defined in cy_usbpd_config_table.h or cy_pdstack_port_config.h file.
*    See its description to learn about the parameters and values.
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
* Version 4.0 of the PDStack middleware is compatible with EZ-PD&trade; Configurator 2.0.
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
*     <td>3.3</td>
*   </tr>
*   <tr>
*     <td>mtb-pdl-cat2</td>
*     <td>2.13.0</td>
*   </tr>
*   <tr>
*     <td>GCC Compiler</td>
*     <td>11.3.1</td>
*   </tr>
*   <tr>
*     <td>IAR Compiler</td>
*     <td>9.50.2</td>
*   </tr>
*   <tr>
*     <td>Arm(R) Compiler 6</td>
*     <td>6.22</td>
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
*     <td> 4.0</td>
*     <td>Updated to USB PD Revision 3.2 Version 1.0
* \note
*      This version of the PDStack middleware is compatible with EZ-PD&trade;
*      Configurator 2.0.
*     </td>
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td> 3.20.1</td>
*     <td> Corrected page header of API reference manual
*     </td>
*     <td>Defect fix.</td>
*   </tr>
*   <tr>
*     <td rowspan="3">3.20</td>
*     <td>Updated to USB PD Revision 3.1 Version 1.8
*     </td>
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td>Added a new library variant 'pmg1_pd3_drp_epr_cfg' that supports
*     configurability of stack parameters through the EZ-PD&trade; Configuration
*     Utility
*     </td>
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td>PdStack has been updated to send Hard Reset if the PDO value in the
*     EPR Request does not match the value of the PDO sent in
*     EPR_Source_Capabilities.
*     </td>
*     <td>Defect fixes </td>
*   </tr>
*   <tr>
*     <td rowspan="3">3.10</td>
*     <td>Updated to USB PD Revision 3.1 Version 1.7
*     </td>
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td>Added EPR AVS feature support in the EPR sink library
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td>Updated the handling of the Get Source Info message for DRP
*     <td>Defect fixes</td>
*   </tr>
*   <tr>
*     <td rowspan="4">3.0</td>
*     <td>Added EZ-PD(TM) PMG1 DRP EPR library compliant to USB PD Revision 3.1
*     </td>
*     <td>New library</td>
*   </tr>
*   <tr>
*     <td>Moved software timer and utility functionality to separate PDUtils
*     middleware library
*     <td>Feature addition</td>
*   </tr>
*   <tr>
*     <td>Updated PDStack to use 27 ms and 29 ms for the tSenderResponse timer
*     while in PD 2.0 and PD3.x contracts respectively. Added API to update
*     the tSenderResponse value from the application space.
*     <td>PD specification updates</td>
*   </tr>
*   <tr>
*     <td>EPR keepalive process is incorrectly getting disabled during some pd
*     transactions. This has been corrected.
*     <td>Defect fixes</td>
*   </tr>
*   <tr>
*     <td>2.0</td>
*     <td>Added WLC1 sink library with support for USB Type-C sink operation and
*     USB PD Revision 3.1 messaging
* \note
*      This version of the PDStack middleware supports only the wlc1_pd3_snk
*      library and is intended to be used in any Power Delivery WLC1
*      application
*     </td>
*     <td>WLC1 device support</td>
*   </tr>
*   <tr>
*   <td rowspan="3">1.20</td>
*     <td> Added libraries for USB Type-C dual role and sink Extended Power
*     Range (EPR) operation </td>
*     <td> New feature addition </td>
*   </tr>
*   <tr>
*    <td> Updated the library to work with RTOS </td>
*    <td> New feature addition </td>
*   </tr>
*   <tr>
*     <td> Added APIs for changing source/sink pdo capabilities</td>
*     <td> Feature addition</td>
*   </tr>
*   <tr>
*     <td rowspan="4">1.10</td>
*     <td>Updated Type-C error recovery period to 250 ms/8</td>
*     <td>PD compliance specification updates</td>
*   </tr>
*   <tr>
*     <td>Updated the size of the status extended message to 7 bytes</td>
*     <td>PD compliance specification updates</td>
*   </tr>
*   <tr>
*     <td>USB PD wakeup interrupt is incorrectly getting disabled as part of the
*     software timer operation. This has been corrected.
*     <td>Defect fixes</td>
*   </tr>
*   <tr>
*     <td>API description updated</td>
*     <td>Documentation update and clarification</td>
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
* The links to other software component's documentation (middleware and PDL)
* point to GitHub for the latest available version of the software.
* To get documentation of the specified version, download from GitHub, and unzip
* the component archive. The documentation is available in
* the <i>docs</i> folder.
*
********************************************************************************
*
* \defgroup group_pdstack_macros Macros
* \brief
* Describes the PDStack macros
*
* \defgroup group_pdstack_functions Functions
* \brief
*  Describes the PDStack function prototypes
*
* \defgroup group_pdstack_data_structures Data structures
* \brief
* Describes the data structures defined by the PDStack
*
* \defgroup group_pdstack_enums Enumerated types
* \brief
* Describes the enumeration types defined by the PDStack
*
*/

#if defined(CY_DEVICE_PAG2S)
#include "cy_pdstack_common_pag2s.h"

#elif defined(CY_DEVICE_CCG6DF_CFP)
#include "cy_pdstack_common_ccg6df_cfp.h"

#else
#include "cy_pdstack_common_default.h"

#endif /* CCGx */

#endif /* CY_PDSTACK_COMMON_H */

/* [] END OF FILE */
