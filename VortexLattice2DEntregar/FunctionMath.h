#pragma once

// Classe base para classes utilizadas para armazenar os do camber do perfil

template<class T>
class FunctionMath
{
public:
	FunctionMath() {}

	virtual T operator()(double x) const = 0;
	virtual T GetDerivative(T x) const = 0;
};