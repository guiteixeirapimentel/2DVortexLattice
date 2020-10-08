#pragma once
#include "FunctionMath.h"

// Classe que implementa a linha de camber para uma placa plana
template<class T>
class CamberLinePlacaPlana : public FunctionMath<T>
{
public:
	CamberLinePlacaPlana(T chord)
		:
		cChord(chord)
	{}

	T operator()(T x) const override
	{
		T res;

		res = 0.0;
		return res;
	}

	T GetChord() const
	{
		return cChord;
	}

	T GetDerivative(T x) const override
	{
		T res;

		res = 0.0;
		return res;
	}

protected:
	const T cChord;
};