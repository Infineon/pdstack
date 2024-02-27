# PDStack middleware library 3.20.1

### What's included?
See the [README.md](./README.md).

### New features/PD compliance specification updates

- Updated the PDStack middleware to be compliant to **Universal Serial Bus Power Delivery Specification Revision 3.1 Version 1.8**.
- Added a new library variant 'pmg1_pd3_drp_epr_cfg' that supports USB Type-C dual role EPR operation, USB PD Revision 3.1 messaging and configurability of stack parameters through the **EZ-PD&trade; Configuration Utility**.

### Defect fixes

- PdStack has been updated to send Hard Reset if the PDO value in the EPR Request does not match the value of the PDO sent in the EPR_Source_Capabilities.
- Corrected page header of API reference manual.

### Supported software and tools
This version of the PDStack middleware is validated for compatibility with the following software and tools:

| Software and tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox&trade; software                            | 3.1     |
| MTB CAT2 peripheral driver library (PDL)                | 2.8.0   |
| GCC compiler                                            | 10.3.1  |
| IAR compiler                                            | 8.42.2  |
| Arm&reg; compiler 6                                     | 6.13    |

### More information
For more information, see the following documents:
* [PDStack middleware README.md](./README.md)
* [Infineon Technologies](https://www.infineon.com)
  
---
© 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
