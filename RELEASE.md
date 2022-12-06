# PDStack middleware library 3.0

### What's included?
See the [README.md](./README.md).

### New features/PD compliance specification updates

- Added library for USB Type-C dual-role and sink Extended Power Range (EPR) operation.
- Moved software timer and utility functionality to separate PDUtils middleware library.
- Added APIs for changing source/sink pdo capabilities.
- Updated PDStack to use 27 ms and 29 ms for the tSenderResponse timer while in PD.
  2.0 and PD3.x contracts respectively. Added API to update tSenderResponse
  value from application space.

### Defect fixes

- Fixed an issue with the EPR Keepalive process that is incorrectly getting disabled during some pd transactions.


### Supported software and tools
This version of the PDStack middleware is validated for compatibility with the following software and tools:

| Software and tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox&trade; software                            | 3.0     |
| MTB CAT2 peripheral driver library (PDL)                | 2.1.0   |
| GCC compiler                                            | 10.3.1  |
| IAR compiler                                            | 8.42.2  |
| Arm&reg; compiler 6                                     | 6.13    |

### More information
For more information, see the following documents:
* [PDStack middleware README.md](./README.md)
* [Infineon Technologies](https://www.infineon.com)
  
---
© 2021-2022, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
