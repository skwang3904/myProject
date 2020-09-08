#include "rgArray.h"

#include "iStd.h"

rgArray::rgArray(RGARRAY_METHOD m)
{
	head = (rgxArray*)malloc(sizeof(rgxArray));
	curr = NULL;
	tail = (rgxArray*)malloc(sizeof(rgxArray));

	count = 0;
	currIndex = -1;
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

	a = head->next;
	for (int i = 0; i < count; i++)
	{
		if (i == index)
		{
			currIndex = i;

			x->next = a;
			x->prev = a->prev;

			a->prev->next = x;
			a->prev = x;

			count++;
			return;
		}
		a = a->next;
	}

	printf("rgArray add error");
}

void rgArray::addObject(void* data)
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

		currIndex = 0;
		count++;
		return;
	}

	a->next = tail;
	tail->prev->next = a;
	a->prev = tail->prev;
	tail->prev = a;

	curr = a;
	currIndex = count;
	count++;
}

void rgArray::remove()
{
	if (count == 1)
		return;

	rgxArray* a;
	if (curr->prev == head)
	{
		a = curr->next;
		head->next = curr->next;
		curr->next->prev = head;
		curr = a;

		currIndex = 0;
	}
	else if (curr->next == tail)
	{
		a = curr->prev;
		tail->prev = curr->prev;
		curr->prev->next = tail;
		curr = a;

		currIndex = count - 2;
	}
	else
	{
		a = curr->prev;
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		curr = a;

		currIndex--;
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
			//a 삭제
			if (b)
			{
				b->next = a->next;
				a->next->prev = b;
				curr = b;

				currIndex = i - 1;
			}
			else
			{
				a->next->prev = head;
				head->next = a->next;
				curr = a->next;

				currIndex = 0;
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
	if (count)
	{
		curr = tail->prev;
		while (curr)
		{
			rgxArray* c = curr->prev;
			if (method)
				method(curr->data);
			free(curr);
			curr = c;

			if (curr == head)
				break;
		}
	}

	count = 0;
	currIndex = 0;
}

void* rgArray::objectAtIndex(int index)
{
	if (count == 1)
		return curr->data;

	printf("index %d\n", index);
	int ind = index;
	if (ind < 0)
		ind = count - 1;
	else if (ind > count - 1)
		ind = 0;
	printf("ind %d\n", ind);

	rgxArray* a = head->next;
	for (int i = 0; i < count; i++)
	{
		if (i == ind)
		{
			currIndex = i;
			//curr = a;
			return a->data;
		}
		a = a->next;
	}

}

void rgArray::printArray(PRINT_METHOD b)
{
	rgxArray* a = head->next;
	for (int i = 0; i < count; i++)
	{
		printf("currIndex %d : ", currIndex);
		if (b)
			b(a->data);
		else
			printf("%d\n", a->data);

		a = a->next;
		if (a == tail)
		{
			printf("\ntail\n");
			break;
		}
	}
}




