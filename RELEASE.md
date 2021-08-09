# PDStack Middleware Library 1.10

### What's Included?
Please refer to the [README.md](./README.md).

### PD compliance specification updates

- Updated Type-C Error Recovery period to 250 ms.
- Updated the size of the status extended message to 7 bytes.

### Defect fixes

- USBPD wakeup interrupt was incorrectly getting disabled as part of the Software timer operation. This has been corrected.


### Supported Software and Tools
This version of the PDStack Middleware was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 2.3     |
| MTB CAT2 Peripheral Driver Library (PDL)                | 1.1.0   |
| GCC Compiler                                            | 9.3.1   |
| IAR Compiler                                            | 8.42.2  |
| ARM Compiler 6                                          | 6.13    |

### More information
For more information, refer to the following documents:
* [PDStack Middleware README.md](./README.md)
* [Cypress Semiconductor](http://www.cypress.com)
  
---
© 2021, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
