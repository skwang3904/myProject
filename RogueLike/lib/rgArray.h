#pragma once

struct rgxArray
{
	void* data;
	rgxArray* prev;
	rgxArray* next;
};

typedef void (*RGARRAY_METHOD)(void* data);

class rgArray
{
public:
	rgArray(RGARRAY_METHOD m = 0);
	virtual ~rgArray();

	void addObject(int index, void* data);
	void addObject(void* data);

	void remove();
	void remove(int index);
	void removeAll();

	void* objectAtIndex(int index);
	void printArray();
public:
	rgxArray* head;
	rgxArray* curr;
	rgxArray* tail;

	int count;
	int currIndex;
	RGARRAY_METHOD method;
};


