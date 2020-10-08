#pragma once
// Bibliotecas padrão C++
#include <vector>
#include <math.h>

// Classe responsável por implementar todas as operações matriciais e armazenar os valores de cada elemento.
class Matriz
{
public:
	Matriz(const std::vector<double>& M, size_t numLinhas, size_t numColunas);
	Matriz(std::vector<double>&& M, size_t numLinhas, size_t numColunas);
	~Matriz()
	{}

	Matriz Transposta() const;

	Matriz operator*(const Matriz& rhs) const;
	Matriz operator+(const Matriz& rhs) const;
	Matriz operator-(const Matriz& rhs) const;
	Matriz operator*(double escalar) const;
	Matriz operator/(double escalar) const;

	Matriz& operator*=(double escalar);
	Matriz& operator/=(double escalar);
	Matriz& operator*=(const Matriz& rhs);

	Matriz& operator+=(const Matriz& rhs);

	Matriz operator-() const;

	std::vector<double>* GetPtrMatriz()
	{
		return &(cMatriz);
	}
	size_t GetNumLinhas() const
	{
		return cNumLinhas;
	}
	size_t GetNumColunas() const
	{
		return cNumColunas;
	}
public:
	std::vector<double> cMatriz;
	size_t cNumLinhas;
	size_t cNumColunas;
};