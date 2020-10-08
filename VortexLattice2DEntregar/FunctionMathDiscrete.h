#pragma once
#include "FunctionMath.h"

// Bibliotecas padrão C++
#include <fstream>
#include <vector>

// Classe que implementa a linha de camber para um perfil armazenado em um arquivo ponto e virgula
template<class T>
class FunctionMathDiscrete : public FunctionMath<T>
{
public:
	FunctionMathDiscrete(T chord, const std::string& arqCsv)
		:
		cChord(chord)
	{
		std::ifstream arq(arqCsv);

		if (arq.is_open())
		{
			char* buf = new char[255];
			while(!arq.eof())
			{
				double x, y;
				
				char c;

				arq >> x;

				arq >> c;

				arq >> y;

				cPontosX.push_back(x);
				cPontosZ.push_back(y);	
			}

			delete[] buf;
		}

		if (cPontosX.back() == cPontosX[cPontosX.size() - 2])
		{
			cPontosX.pop_back();
			cPontosZ.pop_back();
		}
	}

	T operator()(T x) const override
	{
		T res = 0;

		x = x / cChord;

		if (x >= 1.0)
		{
			res = 0.0;
		}
		else
		{
			for (size_t i = 0; i < cPontosX.size()-1; i++)
			{
				if (x < cPontosX[i])
				{
					res = cPontosZ[i] + (((cPontosZ[i + 1] - cPontosZ[i])*(x - cPontosX[i]) / (cPontosX[i + 1] - cPontosX[i])));
					break;
				}
			}
		}
			
		return res;
	}

	T GetChord() const
	{
		return cChord;
	}

	T GetDerivative(T x) const override
	{
		T res = 0.0;

		x = x / cChord;
		
		if (!(x >= 1.0 || x < 0.0))
		{
			for (size_t i = 0; i < cPontosX.size() - 1; i++)
			{
				if (x < cPontosX[i])
				{
					res = ((cPontosZ[i + 1] - cPontosZ[i])/(cPontosX[i + 1] - cPontosX[i]));
					break;
				}
			}
		}

		return res;
	}

protected:
	const T cChord;

	std::vector<T> cPontosX;
	std::vector<T> cPontosZ;
};