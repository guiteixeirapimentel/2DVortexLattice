#pragma once
#include "FunctionMath.h"

// Classe que implementa a linha de camber para o perfil NACA4412

template<class T>
class CamberLine4412 : public FunctionMath<T>
{
public:
	CamberLine4412(T chord)
		:
		cChord(chord)
	{}

	T operator()(T x) const override
	{
		T res;

		x = x / cChord;

		if (x <= 0.4)
		{
			res = 0.25*((0.8*x) - (x*x));
		}
		else
		{
			res = 0.111*(0.2 + (0.8*x) - (x*x));
		}
		return res;
	}

	T GetChord() const
	{
		return cChord;
	}

	T GetDerivative(T x) const override
	{
		T res;

		x = x / cChord;

		if (x <= 0.4)
		{
			res = 0.2 - (0.5*x);
		}
		else
		{
			res = 0.0888 - (0.222*x);
		}
		return res;
	}

protected:
	const T cChord;
};