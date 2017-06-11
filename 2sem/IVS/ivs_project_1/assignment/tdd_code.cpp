//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     TOMAS LAPSANSKY <xlapsa00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author TOMAS LAPSANSKY
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

static PriorityQueue::Element_t *root = NULL;

PriorityQueue::PriorityQueue()
{
			root = NULL;
}

PriorityQueue::~PriorityQueue()
{
	PriorityQueue::Element_t *tmp = PriorityQueue::GetHead();
	PriorityQueue::Element_t *next = NULL;
	
	while(tmp != NULL)
	{
		next = tmp->pNext;
		tmp->pNext = NULL;
		tmp->pPrev = NULL;
		tmp->value = 0;
		free(tmp);
		tmp = next;
	}
	
	root = NULL;
}

void PriorityQueue::Insert(int value)
{
	/* Empty tree */
	PriorityQueue::Element_t *pointer;
	pointer = PriorityQueue::GetHead();

	if(pointer == NULL)
	{
		root = (PriorityQueue::Element_t*)malloc(sizeof(PriorityQueue::Element_t));
		root->pPrev = NULL;
		root->pNext = NULL;
		root->value = value;
	} else
	{
		/* Non-empty tree */
		pointer = PriorityQueue::Find(value);
		if(pointer == NULL)
		{
			pointer = PriorityQueue::GetHead(); //hladanie prvku v zozname
			PriorityQueue::Element_t *entity = (PriorityQueue::Element_t*)malloc(sizeof(PriorityQueue::Element_t)); //alokacia pamate pre novy prvok
			entity->value = value; //pridelenie value
			
			if(pointer->value > value)//Zaradenie na zaciatok zoznamu
			{
				entity->pPrev = NULL;
				entity->pNext = pointer;
				pointer->pPrev = entity;
				root = entity;
				return;
			}
			
			if((pointer->pNext == NULL) && (pointer->value < value))//Zaradenie na zaciatok zoznamu
			{
				entity->pPrev = pointer;
				entity->pNext = NULL;
				pointer->pNext = entity;
				return;
			}

			while((pointer->pNext->value < value))
			{
				pointer = pointer->pNext;
				if(pointer->pNext == NULL)
				{
					entity->pPrev = pointer;
					entity->pNext = NULL;
					pointer->pNext = entity;
					return;
				}
			}
			
			entity->pPrev = pointer;
			entity->pNext = pointer->pNext;
			pointer->pNext = entity;
			entity->pNext->pPrev = entity;

		} else
		{
			PriorityQueue::Element_t *entity = (PriorityQueue::Element_t*)malloc(sizeof(PriorityQueue::Element_t)); //alokacia pamate pre novy prvok
			entity->value = value; //definicia noveho prvku
			entity->pNext = pointer;
			entity->pPrev = pointer->pPrev;
			pointer->pPrev = pointer;
			if(pointer->pPrev == NULL) //prestavenie root v pripade ze prvok je prvy
				root = entity;
			else
				entity->pPrev->pNext = entity;			
		}
	}	
}

bool PriorityQueue::Remove(int value)
{
	PriorityQueue::Element_t *pointer = PriorityQueue::Find(value);
	
	if(pointer == NULL)
		return false;
	
	if((pointer->pPrev == NULL) && (pointer->pNext == NULL)) //delete jedineho prvku, root = NULL
	{
		root = NULL;
	} else if(pointer->pPrev == NULL) //pPrev == NULL, prehodenie root
	{
		pointer->pNext->pPrev = NULL;
		root = pointer->pNext;
	} else if(pointer->pNext == NULL) //Koniec zoznamu, pripisanie iba pNext
	{
		pointer->pPrev->pNext = NULL;
	} else //default
	{
		pointer->pNext->pPrev = pointer->pPrev;
		pointer->pPrev->pNext = pointer->pNext;
	}

	pointer->pPrev = NULL; //Vynulovanie + free
	pointer->pNext = NULL;
	free(pointer);

	pointer = NULL;
	
	return true;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	PriorityQueue::Element_t *pointer = PriorityQueue::GetHead();

	while(pointer != NULL)
	{
		if(pointer->value == value) {
			return pointer;
		}
		pointer = pointer->pNext;
	}
	
    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return root;
}

/*** Konec souboru tdd_code.cpp ***/
