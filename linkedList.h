/* 
 * File:   linkedList.h
 * Author: Pascal-Emmanuel Lachance
 *         https://github.com/Raesangur/
 *
 * Created on August 2nd 2019, 18:55
 */
#pragma once
#include <stdint.h>

/*****************************************************************************/
/* Typedefs */

typedef struct link_t link_t;
typedef struct linkedList_t linkedList_t;


/*****************************************************************************/
/* Function declarations */

linkedList_t* createLinkedList(uint8_t elementSize);
void deleteLinkedList(linkedList_t* list);

link_t* createLink(linkedList_t* list, void* data);
void deleteLink(linkedList_t* list, link_t* link);

void changeData(linkedList_t* list, link_t* link, void* newData);
inline void* getData(link_t* link);

/* Search functions returns the first element of the list corresponding 
 * to search parameters. */
/* If no elements were found, it returns NULL */
link_t* searchByData(linkedList_t* list, void* data);
link_t* searchByData_Offset(linkedList_t* list, void* data, uint16_t offset);
link_t* searchByNumber(linkedList_t* list, uint16_t num);


inline link_t* getNextElement(link_t* link);
inline link_t* getPreviousElement(link_t* link);

inline link_t* getFirstElement(linkedList_t* list);
inline link_t* getLastElement(linkedList_t* list);
