#include "rgArray.h"

#include <stdio.h>
#include <stdlib.h>

rgArray::rgArray(RGARRAY_METHOD m)
{
	head = (rgxArray*)malloc(sizeof(rgxArray));
	curr = NULL;
	tail = (rgxArray*)malloc(sizeof(rgxArray));

	count = 0;
	method = m;
}

rgArray::~rgArray()
{
	removeAll();
}

void rgArray::addObject(int index, void* data)
{
	rgxArray* x = (rgxArray*)malloc(sizeof(rgxArray));
	x->data = data;

	rgxArray* a;
	if (index < count / 2)
	{
		a = head->next; //head
		for (int i = 0; i < count; i++)
		{
			if (i == index)
			{
				x->next = a;
				x->prev = a->prev;

				a->prev->next = x;
				a->prev = x;

				count++;
				return;
			}
			a = a->next;
		}
	}
	else
	{
		a = tail->prev; //tail
		for (int i = 0; i < count; i++)
		{
			if (i == index)
			{
				x->prev = a;
				x->next = a->next;

				a->next->prev = x;
				a->next = x;

				count++;
				return;
			}
			a = a->prev;
		}
	}
	
	printf("rgArray add error");
}

void rgArray::addObject(void* data, bool ishead)
{
	rgxArray* a = (rgxArray*)malloc(sizeof(rgxArray));
	a->data = data;

	if (curr == NULL)
	{
		a->prev = head;
		head->next = a;

		a->next = tail;
		tail->prev = a;
		curr = a;
		count++;
		return;
	}

	a->next = tail;
	a->prev = tail->prev;
	tail->prev->next = a;
	tail->prev = a;

	curr = a;
	count++;
}

void rgArray::remove()
{
	if (count < 2)
		return;

	rgxArray* a;
	if (curr->prev == head)
	{
		a = curr->next;
		head->next = curr->next;
		curr->next->prev = head;
		curr = a;
	}
	else if (curr->next == tail)
	{
		a = curr->prev;
		tail->prev = curr->prev;
		curr->prev->next = tail;
		curr = a;
	}
	else
	{
		a = curr->prev;
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		curr = a;
	}
	count--;
}

void rgArray::remove(int index)
{
	if (count == 1)
		return;
	rgxArray* a = head->next;
	rgxArray* b = NULL;
	for (int i = 0; i < count; i++)
	{
		if (i == index)
		{
			if (b)
			{
				b->next = a->next;
				a->next->prev = b;
			}
			else
			{
				a->next->prev = head;
				head->next = a->next;
			}
			
			count--;
			return;
		}
		b = a;
		a = a->next;
	}
}

void rgArray::removeAll()
{
	tail->prev = curr;
	while (curr)
	{
		rgxArray* c = curr->prev;
		if (method)
			method(curr->data);
		free(curr);
		curr = c;
	}
	count = 0;
}

void* rgArray::objectAtIndex(int index)
{
	if (count < 2)
		return curr->data;
	rgxArray* a;

	a = head->next;
	for (int i = 0; ; i++)
	{
		if (i == index)
		{
			//curr = a;
			return a->data;
		}
		a = a->next;
	}

	//else
	//{
	//	a = tail->prev;
	//	for (int i = 0; ; i++)
	//	{
	//		if (i == index - count - 1)
	//		{
	//			curr = a;
	//			return a->data;
	//		}
	//		a = a->prev;
	//	}
	//}
}

void rgArray::printArray()
{
	rgxArray* a = head->next;
	for (int i = 0; i < count; i++)
	{
		printf("%d, ", a->data );
		a = a->next;
		if (a == tail)
		{
			printf("\ntail\n");
			break;
		}
	}
}




