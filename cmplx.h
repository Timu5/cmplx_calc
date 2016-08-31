#ifndef _CMPLX_H_
#define _CMPLX_H_

#define CMPLX_ZERO (Cmplx){ 0.0, 0.0 }

typedef struct
{
	double Re, Im;
} Cmplx;

Cmplx cmplx_add(Cmplx a, Cmplx b)
{
	return (Cmplx){ a.Re + b.Re, a.Im + b.Im };
}

Cmplx cmplx_sub(Cmplx a, Cmplx b)
{
	return (Cmplx){ a.Re - b.Re, a.Im - b.Im };
}

Cmplx cmplx_mul(Cmplx a, Cmplx b)
{
	return (Cmplx){
		(a.Re * b.Re) - (a.Im * b.Im),
		(a.Im * b.Re) + (a.Re * b.Im)
	};
}

Cmplx cmplx_div(Cmplx a, Cmplx b)
{
	return (Cmplx){
		((a.Re * b.Re) + (a.Im * b.Im)) / ((b.Re * b.Re) + (b.Im * b.Im)),
		((a.Im * b.Re) - (a.Re * b.Im)) / ((b.Re * b.Re) + (b.Im * b.Im))
	};
}

#endif