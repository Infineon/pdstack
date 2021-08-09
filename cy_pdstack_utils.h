/***************************************************************************//**
* \file cy_pdstack_utils.h
* \version 1.10
*
* Provides general utility macros and definitions for the PDStack Middleware.
*
********************************************************************************
* \copyright
* Copyright 2021, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PDSTACK_UTILS_H)
#define CY_PDSTACK_UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*****************************************************************************
* MACRO Definition
*****************************************************************************/

#define GET_MAX(a,b)    (((a) > (b)) ? (a) : (b))
/**<  Get the maximum from among two numbers. */

#define GET_MIN(a,b)    (((a) > (b)) ? (b) : (a))
/**<  Get the minimum from among two numbers. */

#define DIV_ROUND_UP(x, y) (((x) + ((y) - 1)) / (y))
/**< Integer division - round up to next integer. */

#define DIV_ROUND_NEAREST(x, y) (((x) + ((y) / 2)) / (y))
/**< Integer division - round to nearest integer. */

#define MAKE_WORD(hi,lo)                        (((uint16_t)(hi) << 8) | ((uint16_t)(lo)))
/**< Combine two bytes to create one 16-bit WORD. */

/**
 * @brief Combine four bytes to create one 32-bit DWORD.
 */
#define MAKE_DWORD(b3,b2,b1,b0)                         \
    (((uint32_t)(b3) << 24) | ((uint32_t)(b2) << 16) |  \
     ((uint32_t)(b1) << 8) | ((uint32_t)(b0)))

#define MAKE_DWORD_FROM_WORD(hi,lo)             (((uint32_t)(hi) << 16) | ((uint32_t)(lo)))
/**< Combine two WORDs to create one DWORD. */

#define WORD_GET_MSB(w)         ((uint8_t)((w) >> 8))
/**< Retrieve the MSB from a WORD. */

#define WORD_GET_LSB(w)         ((uint8_t)((w) & 0xFF))
/**< Retrieve the LSB from a WORD. */

#define BYTE_GET_UPPER_NIBBLE(w)         ((uint8_t)(((w) >> 4) & 0xF))
/**< Retrieve the Upper nibble from a BYTE. */

#define BYTE_GET_LOWER_NIBBLE(w)         ((uint8_t)((w) & 0xF))
/**< Retrieve the Lower nibble from a BYTE. */

#define DWORD_GET_BYTE0(dw)     ((uint8_t)((dw) & 0xFF))
/**< Retrieve the LSB from a DWORD. */

#define DWORD_GET_BYTE1(dw)     ((uint8_t)(((dw) >> 8) & 0xFF))
/**< Retrieve the bits 15-8 from a DWORD. */

#define DWORD_GET_BYTE2(dw)     ((uint8_t)(((dw) >> 16) & 0xFF))
/**< Retrieve the bits 23-16 from a DWORD. */

#define DWORD_GET_BYTE3(dw)     ((uint8_t)(((dw) >> 24) & 0xFF))
/**< Retrieve the MSB from a DWORD. */

#define MEM_COPY(dest, src, size)               memcpy ((uint8_t *)(dest), (uint8_t *)(src), (size))
/**< memcpy abstraction macro. */

#define MEM_CMP(buf1, buf2, size)               memcmp ((uint8_t *)(buf1), (uint8_t *)(buf2), (size))
/**< memcmp abstraction macro. */

#define MEM_SET(dest, byte, size)               memset ((uint8_t *)(dest), (byte), (size))
/**< memset abstraction macro. */

#define BUSY_WAIT_US(us)        CyDelayCycles ((uint32_t)(us))
/**< Insert delay of the desired number of us using a busy spin-loop. */

/**
 * @brief Macro to combine 4 bytes in reverse order to make a 32-bit integer.
 * @param n Final 32 bit number
 * @param b Input array of bytes to be combined.
 * @param i Index into the input byte array.
 */
#define REV_BYTE_ORDER(n,b,i)                           \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \

#define REG_FIELD_UPDATE(reg,field,value)               \
    (reg) = ((reg & ~(field##_MASK)) | ((value) << field##_POS))
/**< Update a single field in a register. It first clears the field and then updates the data. */

#define REG_FIELD_GET(reg,field)                        \
    (((reg) & field##_MASK) >> field##_POS)
/**< Retrieve a specific field from a register. */

/*****************************************************************************
* Global Function Declaration
*****************************************************************************/

/**
 * @brief Calculate the 2's complement binary checksum over a BYTE array.
 *
 * This function calculates the checksum of the specified byte array. The
 * checksum is a simple function calculated as the 2's complement of the binary
 * sum of all bytes in the array.
 *
 * @param ptr Pointer to the data array.
 * @param size Size of the array in BYTE elements.
 *
 * @return Checksum of the data array.
 */
uint8_t mem_calculate_byte_checksum(uint8_t *ptr, uint32_t  size);

/**
 * @brief Calculate the 2's complement binary checksum over a WORD array.
 *
 * This function calculates the checksum of the specified WORD array. The
 * checksum is a simple function calculated as the 2's complement of the binary
 * sum of all words in the array.
 *
 * @param ptr Pointer to the data array.
 * @param size Size of the array in WORD elements.
 *
 * @return Checksum of the data array.
 */
uint16_t mem_calculate_word_checksum(uint16_t *ptr, uint32_t  size);

/**
 * @brief Calculate the 2's complement binary checksum over a DWORD array.
 *
 * This function calculates the checksum of the specified DWORD array. The
 * checksum is a simple function calculated as the 2's complement of the binary
 * sum of all d-words in the array.
 *
 * @param ptr Pointer to the data array.
 * @param size Size of the array in DWORD elements.
 *
 * @return Checksum of the data array.
 */
uint32_t mem_calculate_dword_checksum(uint32_t *ptr, uint32_t  size);

/**
 * @brief Function to calculate 16-bit CRC.
 *
 * The function implements CRC-16 with polynomial x^16 + x^15 + x^2 + 1 (0xA001).
 *
 * @param crc Original CRC value.
 * @param data Data byte to be included in CRC computation.
 *
 * @return Updated CRC value including the new data byte.
 */
uint16_t crc16(uint16_t crc, uint8_t data);

/**
 * @brief Function to copy 32-bit data from one location to another.
 *
 * @param dest Pointer to Destination.
 * @param source Pointer to source.
 * @param size Size of data in 32-bit DWORD units.
 * @return None.
 */
void mem_copy_word(uint32_t* dest, const uint32_t* source, uint32_t size);

/**
 * @brief Function to do a byte-by-byte copy of data. Replacement for memcpy function.
 * This function is used in cases where we need a memcpy equivalent that can be placed
 * in a specific memory region (such as ROM).
 *
 * @param dest Pointer to destination buffer.
 * @param source Pointer to source buffer.
 * @param size Size of data to be copied (in bytes).
 * @return None
 */
void mem_copy (uint8_t* dest, const uint8_t* source, uint32_t size);

/**
 * @brief Function to initialize a memory buffer. Replacement for memset function.
 * This function is used in cases where we need a memset equivalent that can be placed
 * in a specific memory region (such as ROM).
 *
 * @param dest Pointer to destination buffer.
 * @param c The data to be copied into each byte of the buffer.
 * @param size Size of the buffer (in bytes).
 * @return None
 */
void mem_set (uint8_t* dest, uint8_t c, uint32_t size);

/**
 * @brief Function to divide two unsigned integers and return the quotient rounded up
 * to the nearest integer.
 *
 * @param x The dividend value.
 * @param y The divisor value. Must be non-zero.
 * @return Quotient rounded up to nearest integer.
 */
uint32_t div_round_up(uint32_t x, uint32_t y);

/**
 * @brief Function to calculate threshold value based on input value and percentage which
 * input value should be increased
 *
 * @param val Input value.
 * @param percentage This value means how much (in percents) input value should be increased
 * @return Sum of input value and calculated threshold
 */
int32_t apply_threshold(int32_t val, int8_t percentage);

/**
 * @brief Function to set a bit in an event group variable.
 * Event groups are DWORD (uint32_t) variables which hold a set of request bits which can
 * be set/cleared independently. An event group facilitates management of a set of related
 * tasks or status bits.
 *
 * @param event_map This holds the current status of the event group.
 * @param event_sel This specifies the position of the event/task to be set.
 * @return None
 */
void event_group_set_event(uint32_t *event_map, uint8_t event_sel);

/**
 * @brief Function to set one or more task values in an event group.
 * Event groups are DWORD (uint32_t) variables which hold a set of request bits which can
 * be set/cleared independently. An event group facilitates management of a set of related
 * tasks or status bits.
 *
 * @param event_map This holds the current status of the event group.
 * @param event_sel This specifies a set of event/task bits which should be set.
 * @return None
 */
void event_group_set_events_by_val(uint32_t *event_map, uint32_t event_sel);

/**
 * @brief Function to clear a bit in an event group variable.
 *
 * @param event_map This holds the current status of the event group.
 * @param event_sel This specifies the position of the event/task to be cleared.
 * @return None
 */
void event_group_clear_event(uint32_t *event_map, uint8_t event_sel);

/**
 * @brief Function to clear a bit in an event group variable.
 *
 * @param event_map This holds the current status of the event group.
 * @param event_sel This specifies one or more event/task bits to be cleared.
 * @return None
 */
void event_group_clear_events_by_val(uint32_t *event_map, uint32_t event_sel);

/**
 * @brief Get the next pending event/task from an event group variable.
 *
 * @param event_map Original status of the event group.
 * @param clr_stat Whether the event returned should be cleared.
 * @return Position of the event/task which has been detected and needs to be processed.
 */
uint8_t event_group_get_event(volatile uint32_t *event_map, bool clr_stat);

#endif /* CY_PDSTACK_UTILS_H */

/* [] END OF FILE */
