#ifndef  LINKED_LIST_H
#define  LINKED_LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct _linked_list_item
{
    void* contents;
    struct _linked_list_item* next;
} linked_list_item;

/*
Creates a new list element of type `linked_list_item` from the supplied `contents` pointer.
*/
linked_list_item* new_item(void* contents, size_t contents_size)
{
    linked_list_item* new_item;
    
    // Allocate memory for the linked list item and its contents:
    new_item = (linked_list_item*) malloc(sizeof(linked_list_item));
    new_item->contents = (void *) malloc(contents_size);

    // Copy contents into the list item, one byte at a time:
    for(size_t byte = 0; byte < contents_size; ++byte)
    {
        ((uint8_t*)new_item)[byte] = *((uint8_t*) contents);
    }
    
    new_item->next = NULL;
    return new_item;
}

uint8_t bytes_compare(uint8_t* compare, uint8_t* source, size_t num_bytes)
{
    for(size_t byte = 0; byte < num_bytes; ++byte)
    {
        if(source[byte] != compare[byte])
        {
            return 0;
        }
    }

    return 1;
}

/*
Inserts a new element into a singly-linked list.

NOTE: duplicate entries are not checked for!
*/
void InsertThing(linked_list_item** head, linked_list_item* new_item, size_t num_bytes)
{
	linked_list_item** tracer = head;

    uint8_t* current = (uint8_t*) new_item->contents;
    uint8_t* next = (uint8_t*) (*tracer)->contents;

    // While the tracer pointer is valid and the bytes don't match:
	while( (*tracer) && !bytes_compare(next, current, num_bytes) )
	{
		tracer = &(*tracer)->next;
	}

	new_item->next = *tracer;
	*tracer = new_item;
}

/*
Delete first element on list whose item field matches the given text

NOTE: delete requests for elements not in the list are silently ignored :-).
*/
void RemoveThing(linked_list_item** head, void* content, size_t num_bytes)
{
	uint8_t present = 0; // Starts as false.
	linked_list_item* prev;
	linked_list_item** tracer = head;
	
	while( (*tracer) &&
    !(present = (bytes_compare((uint8_t*) content, (uint8_t*) (*tracer)->contents, num_bytes))) )
    {
		tracer = &(*tracer)->next;
    }

	if(present)
	{
		prev = *tracer;
		*tracer = (*tracer)->next;

        // Free memory used by the deleted list item.
		free(prev->contents);
		free(prev);
	}
}

#endif //LINKED_LIST_H