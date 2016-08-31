#ifndef _STACK_H_
#define _STACK_H_

#define STACK_INIT_SIZE 16

typedef struct
{
	void * data;
	size_t el_size;
	size_t size;
	size_t top;
} Stack;

Stack * stack_new(size_t el_size)
{
	Stack * s = (Stack*)malloc(sizeof(Stack));
	s->data = malloc(STACK_INIT_SIZE * el_size);
	s->el_size = el_size;
	s->size = STACK_INIT_SIZE;
	s->top = 0;

	return s;
}

void stack_free(Stack * s)
{
	free(s->data);
	free(s);
}

void push(Stack * s, void * val)
{
	if (s->top + 1 == s->size)
	{
		s->size *= 2;
		s->data = realloc(s->data, s->size * s->el_size);
	}

	if (val)
		memcpy((void*)((size_t)s->data + (s->top * s->el_size)), val, s->el_size);
	s->top++;
}

int pop(Stack * s, void * ret)
{
	if (s->top == 0)
		return 0;
	
	if (ret)
		memcpy(ret, (void*)((size_t)s->data + ((--s->top) * s->el_size)), s->el_size);
	return 1;
}

int peek(Stack * s, void * ret)
{
	if (s->top == 0)
		return 0;

	if (ret)
		memcpy(ret, (void*)((size_t)s->data + ((s->top - 1) * s->el_size)), s->el_size);
	return 1;
}

#endif