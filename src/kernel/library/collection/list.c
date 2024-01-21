#include "list.h"

struct node{
	uint16_t* prev;
	uint16_t value;
	uint16_t* next;
}

struct List{
	struct node* firstitem;
	struct node* lastitem;
}


