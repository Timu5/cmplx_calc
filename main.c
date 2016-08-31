#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "stack.h"
#include "cmplx.h"

int error = 0;

char * expr; 

char prevchar;
char lastchar;

int nextchar()
{
	while (isspace(*expr) && *expr != 0)
	{
		expr++;
	}
	prevchar = lastchar;
	if (*expr == 0)
		return lastchar = -1;

	lastchar = *expr;
	expr++;
	return lastchar;
}
 
Stack * operators; 
Stack * operands;

int precedence(char ch)
{
	if (ch == '*' || ch == '/')
		return 2;
	else if (ch == '+' || ch == '-')
		return 1;
	else
		return 0;
}

Cmplx bin_op(char op)
{
	Cmplx a, b;
	if (!pop(operands, &b) || !pop(operands, &a))
		goto end;

	switch (op)
	{
	case '+':
		return cmplx_add(a, b);
	case '-':
		return cmplx_sub(a, b);
	case '*':
		return cmplx_mul(a, b);
	case '/':
		return cmplx_div(a, b);
	}
	
end:
	error = 1;
	return CMPLX_ZERO;
}

void calc(char * input)
{
	error = 0;
	prevchar = 0;
	lastchar = 0;
	expr = input;

	operators = stack_new(sizeof(char));
	operands = stack_new(sizeof(Cmplx));
	
	if (nextchar() == -1)
		error = 1;

	while (lastchar > 0 && !error)
	{
		if (isdigit(lastchar) || lastchar == '.' || lastchar == 'i')
		{
			char buff[128];
			char * snum = buff;
			int i = 0;
			
			do
			{
				if (lastchar == 'i')
				{
					i++;
				}
				else
				{
					*snum = lastchar;
					snum++;
				}
				nextchar();
			} while (isdigit(lastchar) || lastchar == '.' || lastchar == 'i');

			if (i != 0 && snum == buff)
			{
				*snum = '1';
				snum++;
			}
			*snum = 0;
			
			i %= 4;
			Cmplx num = CMPLX_ZERO;
			switch (i)
			{
			case 0: num.Re =  atof(buff); break;
			case 1: num.Im =  atof(buff); break;
			case 2: num.Re = -atof(buff); break;
			case 3: num.Im = -atof(buff); break;
			}

			push(operands, &num);

			continue;
		}
		else
		{
			switch (lastchar)
			{
			case '(':
				push(operators, &lastchar);
				break;
			case ')':;
				int flag = 0;
				char op;
				while (operators->top > 0)
				{
					pop(operators, &op);

					if (op == '(')
					{
						if(operands->top > 0)
							flag = 1;
						break;
					}
					else
					{
						Cmplx result = bin_op(op);
						push(operands, &result);
					}
				}
				if (!flag)
					error = 1;
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				if ((prevchar ==  0  || prevchar == '(') &&
					(lastchar == '-' || lastchar == '+'))
				{
					Cmplx v = CMPLX_ZERO;
					push(operands, &v);
					push(operators, &lastchar);
					break;
				}
				int prec = precedence(lastchar);
				int tmp;
				while (operators->top > 0)
				{
					char op;
					peek(operators, &op);
					tmp = precedence(op);
					if (tmp >= prec)
					{
						pop(operators, &op);
						Cmplx result = bin_op(op);
						push(operands, &result);
					}
					else
					{
						break;
					}
				}
				push(operators, &lastchar);
				break;
			default:
				error = 1;
				break;
			}
		}
		nextchar();
	}
	while (operators->top > 0)
	{
		char op;
		pop(operators, &op);
		if (op == '(')
		{
			error = 1;
			break;
		}
		Cmplx result = bin_op(op);
		push(operands, &result);
	}

	if (!error && operands->top == 1)
	{
		Cmplx ret;
		pop(operands, &ret);
		printf("%g%c%gi\n", ret.Re, ret.Im < 0 ? '-' : '+', fabs(ret.Im));
	}
	else
	{
		puts("Syntax ERROR!");
	}

	stack_free(operators);
	stack_free(operands);
}

int main(int argc, char ** argv)
{
	if (argc >= 2)
	{
		for (int i = 1; i < argc; i++)
			calc(argv[i]);
		return 0;
	}

	while (1)
	{
		char buff[255];
		putchar('>');
		gets(buff);
		if (toupper(buff[0]) == 'E' || toupper(buff[0]) == 'Q')
		{
			break;
		}
		calc(buff);
	}
	
	return 0;
}