/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units_convertion.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2025/01/31 10:42:01 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:42:42 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNITS_CONVERTION_H
#define UNITS_CONVERTION_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Unit Conversion Library
 *
 * Provides functions for converting between different digital storage units.
 * Supports both binary (IEC) and decimal (SI) prefixes:
 * - Binary (IEC): 1 KiB = 1024 bytes
 * - Decimal (SI): 1 KB = 1000 bytes
 *
 * @param use_ib If true, uses binary (IEC) units, otherwise uses decimal (SI) units
 */

/**
 * @brief Octet (Byte) Conversion Functions
 * Convert from bytes to larger units
 * @param octets Number of bytes to convert
 * @param use_ib If true, uses 1024 (binary), if false uses 1000 (decimal)
 * @return Converted value as double
 */
double octet_to_kb(uint64_t octets, bool use_ib); // Convert to kilobytes
double octet_to_mb(uint64_t octets, bool use_ib); // Convert to megabytes
double octet_to_gb(uint64_t octets, bool use_ib); // Convert to gigabytes
double octet_to_tb(uint64_t octets, bool use_ib); // Convert to terabytes

/**
 * @brief Kilobyte Conversion Functions
 * Convert from kilobytes to other units
 * @param kb Number of kilobytes to convert
 * @param use_ib If true, uses 1024 (binary), if false uses 1000 (decimal)
 * @return Converted value as double
 */
double kb_to_octet(uint64_t kb, bool use_ib); // Convert to bytes
double kb_to_mb(uint64_t kb, bool use_ib);	  // Convert to megabytes
double kb_to_gb(uint64_t kb, bool use_ib);	  // Convert to gigabytes
double kb_to_tb(uint64_t kb, bool use_ib);	  // Convert to terabytes

/**
 * @brief Megabyte Conversion Functions
 * Convert from megabytes to other units
 * @param mb Number of megabytes to convert
 * @param use_ib If true, uses 1024 (binary), if false uses 1000 (decimal)
 * @return Converted value as double
 */
double mb_to_octet(uint64_t mb, bool use_ib); // Convert to bytes
double mb_to_kb(uint64_t mb, bool use_ib);	  // Convert to kilobytes
double mb_to_gb(uint64_t mb, bool use_ib);	  // Convert to gigabytes
double mb_to_tb(uint64_t mb, bool use_ib);	  // Convert to terabytes

/**
 * @brief Gigabyte Conversion Functions
 * Convert from gigabytes to other units
 * @param gb Number of gigabytes to convert
 * @param use_ib If true, uses 1024 (binary), if false uses 1000 (decimal)
 * @return Converted value as double
 */
double gb_to_octet(uint64_t gb, bool use_ib); // Convert to bytes
double gb_to_kb(uint64_t gb, bool use_ib);	  // Convert to kilobytes
double gb_to_mb(uint64_t gb, bool use_ib);	  // Convert to megabytes
double gb_to_tb(uint64_t gb, bool use_ib);	  // Convert to terabytes

/**
 * @brief Terabyte Conversion Functions
 * Convert from terabytes to other units
 * @param tb Number of terabytes to convert
 * @param use_ib If true, uses 1024 (binary), if false uses 1000 (decimal)
 * @return Converted value as double
 */
double tb_to_octet(uint64_t tb, bool use_ib); // Convert to bytes
double tb_to_kb(uint64_t tb, bool use_ib);	  // Convert to kilobytes
double tb_to_mb(uint64_t tb, bool use_ib);	  // Convert to megabytes
double tb_to_gb(uint64_t tb, bool use_ib);	  // Convert to gigabytes

#endif
