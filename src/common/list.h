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
#ifndef _LIST_H_
#define _LIST_H_

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
// Adds a new element to given list. If the
// new length exceeds or equals the list
// capacity, doubles the previous capacity,
// moves old elements to the new memory
// and frees the old allocation.
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

#endif //_LIST_H_
