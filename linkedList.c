/* linkedList.c
 * Pascal-Emmanuel Lachance
 * https://www.github.com/Raesangur 
 * 
 * Generic Linked Lists */
 
/* MIT License

Copyright (c) 2019 Pascal-Emmanuel Lachance
https://www.github.com/Raesangur 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

/*****************************************************************************/
/* File includes */
#include <stdlib.h>
#include "linkedList.h"


/*****************************************************************************/
/* Typedefs */
struct link_t
{
	link_t* previous;
	link_t* next;
	void* data;
};

struct linkedList_t
{
	link_t* firstElement;
	link_t* lastElement;
	uint8_t elementSize;
};


/*****************************************************************************/
/* Private function declaration */
static link_t* searchByNullData(linkedList_t* list);


/*****************************************************************************/
/* Public function definitions */

linkedList_t* createLinkedList(uint8_t elementSize)
{
	/* Memory allocation */
	linkedList_t* list = (linkedList_t*)malloc(sizeof(linkedList_t));
	if (list == NULL)                   /* Error during memory allocation */
	{
		return NULL;
	}

	/* Fill information */
	list->elementSize = elementSize;
	list->firstElement = NULL;
	list->lastElement = NULL;

	return list;
}

void deleteLinkedList(linkedList_t* list)
{
	if (list == NULL)                   /* List was already deleted? */
	{
		return;
	}

    /* Delete all elements in the list */
    link_t* currentLink = list->firstElement;
    do
    {
        deleteLink(list, currentLink);  /* Delete link */
        currentLink = currentLink->next;/* Select next link */
    }while(currentLink->next != NULL);
    
	/* Free allocated memory for the linked list */
	free(list);
}


link_t* createLink(linkedList_t* list, void* data)
{
	if (list == NULL)                   /* Invalid pointer */
	{
		return NULL;
	}

	link_t* link;

	if (list->firstElement == NULL)     /* New link is first element */
	{
		/* Memory allocation */
		link = (link_t*)malloc(sizeof(link_t));
		if (link == NULL)               /* Error during memory allocation */
		{
			return NULL;
		}

		link->data = malloc(list->elementSize);
		if (link->data == NULL)			/* Error during memory allocation */
		{
			return NULL;
		}

		/* Insert new link in the list */
		list->firstElement = link;      /* link is the only list member */
		list->lastElement = list->firstElement;

		/* Link pointers */
		link->next = NULL;
		link->previous = NULL;

		/* Data */
		if (data == NULL)
		{
			link->data = NULL;
		}
		else
		{
			changeData(list, link, data);
		}

		return link;
	}
	else
	{
		/* Memory allocation */
		link = (link_t*)malloc(list->elementSize);
		if (link == NULL)               /* Error during memory allocation */
		{
			return NULL;
		}

		link->data = malloc(list->elementSize);
		if (link->data == NULL)			/* Error during memory allocation */
		{
			return NULL;
		}

		/* Insert new link in the list */
		list->lastElement->next = link;

		/* Link poiners */
		link->next = NULL;
		link->previous = list->lastElement;

		/* Change lastElement to the new link */
		list->lastElement = link;

		/* Data */
		if (data == NULL)
		{
			link->data = NULL;
		}
		else
		{
			changeData(list, link, data);
		}

		return link;
	}
}

void deleteLink(linkedList_t* list, link_t* link)
{
	if (list == NULL || link == NULL)   /* Invalid pointer */
	{
		return;
	}

  /* Check if link is the only element */
  if (link == list->firstElement)
  {
    list->firstElement = NULL;
  }
  else
  {
    /* Change pointers to bypass link to be deleted */
    link->previous->next = link->next;
    link->next->previous = link->previous;

    /* Check if link is last element */
    if (link == list->lastElement)
    {
      list->lastElement = link->previous;
      list->lastElement->next = NULL;
    }
  }
	
	/* Free data pointer */
	free(link->data);

	/* Free link memory */
	free(link);
}


void changeData(linkedList_t* list, link_t* link, void* newData)
{
	if (list == NULL || link == NULL)   /* Invalid pointer */
	{
		return;
	}

	uint8_t i;
	for (i = 0; i < list->elementSize; i++)
	{
		*(uint8_t*)link->data = *(uint8_t*)newData;
		link->data = (uint8_t*)link->data + 1;
		newData = (uint8_t*)newData + 1;
	}
	link->data = (uint8_t*)link->data - list->elementSize;
}
inline void* getData(link_t* link)
{
	return link->data;
}


link_t* searchByData(linkedList_t* list, void* data)
{
	if (list == NULL)                   /* Invalid pointer */
	{
		return NULL;
	}

	if (data == NULL)                   /* Call specialized function if data */
	{                                   /* searched is NULL */
		return searchByNullData(list);
	}

	link_t* currentLink = list->firstElement;
	do
	{
		/* Search the first byte of data in current link */
		if (*(uint8_t*)currentLink->data == *(uint8_t*)data)
		{
			/* Search every following bytes of data in current link */
			uint8_t i;
			for (i = 1; i < list->elementSize; i++)
			{
				if (*((uint8_t*)currentLink->data + i) != *((uint8_t*)data + i))
				{
					break;
				}
			}
			if (i == list->elementSize)
			{
				return currentLink;
			}
		}

		/* Get to next link */
		currentLink = currentLink->next;
	} while (currentLink->next != NULL);

	return NULL;                        /* Search was unsuccessful */
}

link_t* searchByData_Offset(linkedList_t* list, void* data, uint16_t offset)
{
	if (list == NULL)                   /* Invalid pointer */
	{
		return NULL;
	}

	link_t* currentLink = searchByNumber(list, offset);
	do
	{
		/* Search the first byte of data in current link */
		if (*(uint8_t*)currentLink->data == *(uint8_t*)data)
		{
			/* Search every following bytes of data in current link */
			uint8_t i;
			for (i = 1; i < list->elementSize; i++)
			{
				if (*((uint8_t*)currentLink->data + i) != *((uint8_t*)data + i))
				{
					break;
				}
			}
			if (i == list->elementSize)
			{
				return currentLink;
			}
		}

		/* Get to next link */
		currentLink = currentLink->next;
	} while (currentLink->next != NULL);

	return NULL;                        /* Search was unsuccessful */
}

link_t* searchByNumber(linkedList_t* list, uint16_t num)
{
	uint16_t i;
	link_t* currentLink = list->firstElement;
	for (i = 0; i < num; i++)
	{
		/* Get to next link*/
		if (currentLink->next == NULL)
		{
			return NULL;
		}
		else
		{
			currentLink = currentLink->next;
		}
	}
	return currentLink;
}


inline link_t* getNextElement(link_t* link)
{
	return link->next;
}
inline link_t* getPreviousElement(link_t* link)
{
	return link->previous;
}


inline link_t* getFirstElement(linkedList_t* list)
{
	return list->firstElement;
}
inline link_t* getLastElement(linkedList_t* list)
{
	return list->lastElement;
}


/*****************************************************************************/
/* Private function definition */
static link_t* searchByNullData(linkedList_t* list)
{
	link_t* currentLink = list->firstElement;
	do
	{
		/* Check value of current link */
		if (currentLink->data == NULL)
		{
			return currentLink;
		}

		/* Get to next link */
		currentLink = currentLink->next;
	} while (currentLink->next != NULL);

	return NULL;                        /* Null data was not found */
}