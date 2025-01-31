/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hypercore.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:13:38 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:51:24 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HYPERCORE_H
#define HYPERCORE_H

/**
 * @brief HyperCore Library
 *
 * A comprehensive C library providing various data structures, algorithms,
 * and utility functions for system programming.
 *
 * Features:
 * - Memory management with garbage collection
 * - Command line argument parsing
 * - Generic data structures and algorithms
 * - Comparison utilities for various data types
 */

/**
 * @brief Core Utilities
 *
 * @file argsparser.h Command line argument parsing utilities
 * @file garbage.h Memory management and garbage collection system
 */
#include "lib/argsparser.h"
#include "lib/garbage.h"

/**
 * @brief Comparison Utilities
 *
 * @file compare.h Generic comparison functions for various data types
 */
#include "lib/algorithms/compare.h"

/**
 * @brief Data Structures and Algorithms
 *
 * Collection of generic data structures and algorithms:
 * @file avl_tree.h Self-balancing AVL tree implementation
 * @file list.h Doubly linked list implementation
 * @file map.h Hash map implementation
 * @file rb_tree.h Red-Black tree implementation
 * @file skip_list.h Skip list implementation
 * @file vector.h Dynamic array implementation
 */
#include "lib/algorithms/avl_tree.h"
#include "lib/algorithms/hash_table.h"
#include "lib/algorithms/list.h"
#include "lib/algorithms/map.h"
#include "lib/algorithms/rb_tree.h"
#include "lib/algorithms/skip_list.h"
#include "lib/algorithms/vector.h"

#endif // HYPERCORE_H