// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Very simple mechanism for dynamic array management for use in this project
 *
 * Copyright (c) 2023
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#include <memory.h>
#include <malloc.h>
#include "list.h"

// The factor by which the list grows when at capacity
#define GROWTH_FACTOR 2

list_t*
list_new(int capacity)
{
	if (capacity <= 1)
	{
		capacity = 2;
	}

	list_t* list = malloc(sizeof(list_t));
	void** values = malloc(sizeof(void*) * capacity);

	list->length = 0;
	list->capacity = capacity;
	list->values = values;

	return list;
}

void
list_add(list_t* list, void* value, size_t size)
{
	if (!list)
	{
		return;
	}
	
	if (list->capacity <= list->length + 1)
	{
		list->capacity *= GROWTH_FACTOR;
		list->values = realloc(list->values, size * list->capacity);
	}

	if ((list->values[list->length] = calloc(1, size + 1)))
	{
		memcpy(list->values[list->length], value, size);
		list->length++;
	}
}

void
list_free(list_t* list)
{
	if (!list)
	{
		return;
	}
	
	for (int i = 0; i < list->length; ++i)
	{
		free(list->values[i]);
	}

	free(list->values);
	free(list);
}