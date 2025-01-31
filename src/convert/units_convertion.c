/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units_convertion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvங்களைத் <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:42:42 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:42:42 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/convert/units_convertion.h>

// Conversion functions for octets (bytes)
double octet_to_kb(uint64_t octets, bool use_ib) {
	return (use_ib ? (double)octets / 1024 : (double)octets / 1000);
}

double octet_to_mb(uint64_t octets, bool use_ib) {
	return (use_ib ? octet_to_kb(octets, use_ib) / 1024 : octet_to_kb(octets, use_ib) / 1000);
}

double octet_to_gb(uint64_t octets, bool use_ib) {
	return (use_ib ? octet_to_mb(octets, use_ib) / 1024 : octet_to_mb(octets, use_ib) / 1000);
}

double octet_to_tb(uint64_t octets, bool use_ib) {
	return (use_ib ? octet_to_gb(octets, use_ib) / 1024 : octet_to_gb(octets, use_ib) / 1000);
}

// Conversion functions for kilobytes
double kb_to_octet(uint64_t kb, bool use_ib) {
	return (use_ib ? (double)kb * 1024 : (double)kb * 1000);
}

double kb_to_mb(uint64_t kb, bool use_ib) {
	return (use_ib ? (double)kb / 1024 : (double)kb / 1000);
}

double kb_to_gb(uint64_t kb, bool use_ib) {
	return (use_ib ? kb_to_mb(kb, use_ib) / 1024 : kb_to_mb(kb, use_ib) / 1000);
}

double kb_to_tb(uint64_t kb, bool use_ib) {
	return (use_ib ? kb_to_gb(kb, use_ib) / 1024 : kb_to_gb(kb, use_ib) / 1000);
}

// Conversion functions for megabytes
double mb_to_octet(uint64_t mb, bool use_ib) {
	return (use_ib ? mb_to_kb(mb, use_ib) * 1024 : mb_to_kb(mb, use_ib) * 1000);
}

double mb_to_kb(uint64_t mb, bool use_ib) {
	return (use_ib ? (double)mb * 1024 : (double)mb * 1000);
}

double mb_to_gb(uint64_t mb, bool use_ib) {
	return (use_ib ? (double)mb / 1024 : (double)mb / 1000);
}

double mb_to_tb(uint64_t mb, bool use_ib) {
	return (use_ib ? mb_to_gb(mb, use_ib) / 1024 : mb_to_gb(mb, use_ib) / 1000);
}

// Conversion functions for gigabytes
double gb_to_octet(uint64_t gb, bool use_ib) {
	return (use_ib ? gb_to_mb(gb, use_ib) * 1024 : gb_to_mb(gb, use_ib) * 1000);
}

double gb_to_kb(uint64_t gb, bool use_ib) {
	return (use_ib ? gb_to_mb(gb, use_ib) * 1024 : gb_to_mb(gb, use_ib) * 1000);
}

double gb_to_mb(uint64_t gb, bool use_ib) {
	return (use_ib ? (double)gb * 1024 : (double)gb * 1000);
}

double gb_to_tb(uint64_t gb, bool use_ib) {
	return (use_ib ? (double)gb / 1024 : (double)gb / 1000);
}

// Conversion functions for terabytes
double tb_to_octet(uint64_t tb, bool use_ib) {
	return (use_ib ? tb_to_gb(tb, use_ib) * 1024 : tb_to_gb(tb, use_ib) * 1000);
}

double tb_to_kb(uint64_t tb, bool use_ib) {
	return (use_ib ? tb_to_gb(tb, use_ib) * 1024 : tb_to_gb(tb, use_ib) * 1000);
}

double tb_to_mb(uint64_t tb, bool use_ib) {
	return (use_ib ? tb_to_gb(tb, use_ib) * 1024 : tb_to_gb(tb, use_ib) * 1000);
}

double tb_to_gb(uint64_t tb, bool use_ib) {
	return (use_ib ? (double)tb * 1024 : (double)tb * 1000);
}
