# PDStack Middleware Library 1.20

### Overview

The PDStack middleware implements state machines defined in the **Universal Serial Bus Type-C Cable and Connector Specification** and the **Universal Serial Bus Power Delivery Specification**. The middleware provides a set of Device Policy Manager (DPM) APIs through which the application can initialize, monitor and configure the middleware operation.

The PDStack Middleware is released in the form of pre-compiled libraries. The following library variants with different feature set to help limit the memory footprint of the stack in applications are released.
1)	pmg1_pd3_snk_lite – Library with support for USB Type-C sink operation and USB-PD Revision 3.1 messaging.
2)	pmg1_pd2_snk_lite – Library with support for USB Type-C sink operation and USB-PD Revision 2.0 messaging. Using this library will reduce the flash (code) memory usage by the application.
3)	pmg1_pd3_drp      – Library with support for USB Type-C dual role operation and USB-PD Revision 3.1 messaging. This library can be used in any Power-Delivery application.
4)	pmg1_pd3_snk_epr  – Library with support for USB Type-C sink Extended Power Range(EPR) operation and USB-PD Revision 3.1 messaging.

### Features
1) Support Type-C port configuration and connection detection in the Sink, Source and Dual Role
2) Support Extended Power Range (EPR) in Sink Role.
2) Support USBPD Communication with the following features:
    * USB PD revision 3.1 compliance
    * USB PD revision 2.0 compliance (backward compatibility)
    * Power contract negotiation in Sink Role
    * Ability to respond to SOP and SOP’ messages to support designs with a Captive USB-C cable.
3) Support configuration of the port roles and responses through the EZ-PD Configurator.
4) Support Device Policy Manager APIs to initialize, configure, monitor and control the port operation; as well as to send/receive USBPD messages associated with Alternate Mode discovery, Authentication, Firmware update etc.


### Quick Start

### More information
The following resources contain more information:
* [PDStack Middleware RELEASE.md](./RELEASE.md)
* [PDStack Middleware API Reference Guide](https://infineon.github.io/pdstack/pdstack_api_reference_manual/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [ModusToolbox Device Configurator Tool Guide](https://www.cypress.com/ModusToolboxDeviceConfig)
* [Cypress Semiconductor](http://www.cypress.com)
 
---
© 2021, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
