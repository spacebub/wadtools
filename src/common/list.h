// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Very simple mechanism for dynamic array management for use in this project
 *
 * Copyright (c) 2023 spacebub
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#ifndef WDTLS_LIST_H_
#define WDTLS_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

// Base type used by the list implementation.
// Should ideally not be manually created or
// disposed of, instead use list_new for creation
// and list_free for deletion.
typedef struct
{
	void** values;
	int length;
	int capacity;
} list_t;

// Returns a pointer to a basic dynamic list
// of size "capacity", initialized with length 0
// and a void** with dimension "capacity".
list_t*
list_new(int capacity);
// Takes a pointer to a given element of size_t size
// and copies the contents into the given list.
void
list_add(list_t* list, void* value, size_t size);
// Frees the resources allocated by the list
// including all values copied to it during
// insertion.
void
list_free(list_t* list);

#ifdef __cplusplus
}
#endif

#endif //WDTLS_LIST_H_
