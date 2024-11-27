# PDStack middleware library 4.0

## Overview

The PDStack middleware implements state machines defined in the **Universal
Serial Bus Type-C Cable and Connector Specification** and the **Universal Serial
Bus Power Delivery Specification**. The middleware provides a set of Device
Policy Manager (DPM) APIs through which the application can initialize, monitor,
and configure the middleware operation.

The PDStack middleware is released in the form of pre-compiled libraries. The
following library variants with a different feature set to help limit the memory
footprint of the stack in applications are released.

1. `pmg1_pd3_snk_lite` – Supports USB Type-C sink operation and USB PD Revision
   3.2 messaging. This library can be used for any sink application which
   requires up to 100W (20V, 5A) of power consumption.

2. `pmg1_pd2_snk_lite` – Supports USB Type-C sink operation and USB PD Revision
   2.0 messaging. Using this library reduces the flash (code) memory usage by
   the application.

3. `pmg1_pd3_drp_lite` – Supports USB Type-C dual-role Standard Power Range
   (SPR) operation and USB PD Revision 3.2 messaging. This library supports all
   but the following features of the `pmg1_pd3_drp` library:
   - Try.SRC and Try.SNK
   - Cable discovery
   - VDM message handling
   - VConn

4. `pmg1_pd3_drp` – Supports USB Type-C dual-role SPR operation and USB PD
   Revision 3.2 messaging. This library can be used in any Power Delivery
   (PD)application.

5. `pmg1_pd3_snk_epr` – Supports USB Type-C sink Extended Power Range (EPR)
   operation and USB PD Revision 3.2 messaging.

6. `pmg1_pd3_drp_epr` - Supports USB Type-C dual-role EPR operation and USB PD
   Revision 3.2 messaging. This library can be used in any Power Delivery
   application that requires EPR feature support.

7. `pmg1_pd3_drp_epr_cfg` - Supports USB Type-C dual role EPR operation, USB PD
   Revision 3.2 messaging and configurability of stack parameters through the
   **EZ-PD&trade; Configuration Utility**.


## Features

- Supports Type-C port configuration and connection detection in the sink,
  source, and dual-role

- Supports EPR in source and sink roles

- Supports Liquid Corrosion Mitigation mode

- Supports USB PD communication with the following features:
   - USB PD revision 3.2 compliance
   - USB PD revision 2.0 compliance (backward compatibility)
   - Power contract negotiation in sink and source roles
   - Ability to respond to SOP and SOP' messages to support designs with a
     captive USB-C cable

- Supports configuration of the port roles and responses through the
  EZ-PD&trade; Configurator

- Supports DPM APIs to initialize, configure, monitor, and control the port
  operation and to send/receive USB PD messages associated with alternate mode
  discovery, authentication, firmware update, etc.


## Quick start

See the "Quick start guide" section of the [PDStack middleware API reference
manual](https://infineon.github.io/pdstack/pdstack_api_reference_manual/html/index.html#section_pdstack_quick_start).

### More information

For more information, see the following resources:
- [PDStack middleware RELEASE.md](./RELEASE.md)

- [PDStack middleware API reference
  guide](https://infineon.github.io/pdstack/pdstack_api_reference_manual/html/index.html)

- [ModusToolbox&trade; software, quick start guide, documentation, and
  videos](https://www.infineon.com/modustoolbox)

- [ModusToolbox&trade; EZ-PD&trade; Configurator user
  guide](https://www.infineon.com/dgdl/Infineon-modustoolbox-ez-pd-configurator-user-guide-UserManual-v06_00-EN.pdf?fileId=8ac78c8c92416ca5019277a01a092392)
 
---
© 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or an
affiliate of Cypress Semiconductor Corporation.
