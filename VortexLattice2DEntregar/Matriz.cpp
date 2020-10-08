#include "Matriz.h"

Matriz::Matriz(const std::vector<double>& M, size_t numLinhas, size_t numColunas)
	:
	cMatriz(M),
	cNumLinhas(numLinhas),
	cNumColunas(numColunas)
{}

Matriz::Matriz(std::vector<double>&& M, size_t numLinhas, size_t numColunas)
	:
	cMatriz(std::move(M)),
	cNumLinhas(numLinhas),
	cNumColunas(numColunas)
{}

Matriz Matriz::Transposta() const
{
	std::vector<double> res;
	res.resize(cMatriz.size());

	for (size_t i = 0; i < cNumLinhas; i++)
	{
		for (size_t j = 0; j < cNumColunas; j++)
		{
			res[i + (j * cNumLinhas)] = cMatriz[j + (i * cNumColunas)];
		}
	}
	Matriz m(std::move(res), cNumColunas, cNumLinhas);

	return m;
}

Matriz Matriz::operator+(const Matriz& rhs) const
{
	std::vector<double> res;
	res.resize(cNumColunas * cNumLinhas);
	if (rhs.cNumColunas == cNumColunas && rhs.cNumLinhas == cNumLinhas)
	{
		for (size_t i = 0; i < cMatriz.size(); i++)
		{
			res[i] = cMatriz[i] + rhs.cMatriz[i];
		}

		Matriz m(std::move(res), cNumLinhas, cNumColunas);

		return m;
	}
	else
	{
		return Matriz({ 0.0 }, 0, 0);
	}
}

Matriz Matriz::operator-(const Matriz& rhs) const
{
	std::vector<double> res;
	res.resize(cNumColunas * cNumLinhas);
	if (rhs.cNumColunas == cNumColunas && rhs.cNumLinhas == cNumLinhas)
	{
		for (size_t i = 0; i < cMatriz.size(); i++)
		{
			res[i] = cMatriz[i] - rhs.cMatriz[i];
		}

		Matriz m(std::move(res), cNumLinhas, cNumColunas);

		return m;
	}
	else
	{
		return Matriz({ 0.0 }, 0, 0);
	}
}

Matriz Matriz::operator*(double escalar) const
{
	std::vector<double> res;
	res.resize(cNumColunas * cNumLinhas);

	for (size_t i = 0; i < cMatriz.size(); i++)
	{
		res[i] = cMatriz[i] * escalar;
	}
	Matriz m(std::move(res), cNumLinhas, cNumColunas);

	return m;
}

Matriz Matriz::operator/(double escalar) const
{
	std::vector<double> res;
	res.resize(cNumColunas * cNumLinhas);

	for (size_t i = 0; i < cMatriz.size(); i++)
	{
		res[i] = cMatriz[i] / escalar;
	}
	Matriz m(std::move(res), cNumLinhas, cNumColunas);

	return m;
}

Matriz& Matriz::operator*=(double escalar)
{
	for (size_t i = 0; i < cMatriz.size(); i++)
	{
		cMatriz[i] *= escalar;
	}

	return *this;
}

Matriz& Matriz::operator/=(double escalar)
{
	for (size_t i = 0; i < cMatriz.size(); i++)
	{
		cMatriz[i] /= escalar;
	}

	return *this;
}

Matriz& Matriz::operator*=(const Matriz& rhs)
{
	if (cNumColunas != rhs.cNumColunas)
	{
		return *this;
	}

	std::vector<double> res;
	res.resize(cNumLinhas * rhs.cNumColunas);

	for (size_t i = 0; i < cNumLinhas; i++)
	{
		for (size_t j = 0; j < rhs.cNumColunas; j++)
		{
			double soma = 0.0;

			for (size_t k = 0; k < rhs.cNumColunas; k++)
			{
				soma += cMatriz[k + (i*cNumColunas)] * rhs.cMatriz[j + (k*rhs.cNumColunas)];
			}

			res[j + (i * cNumColunas)] = soma;
		}
	}
	
	cMatriz = std::move(res);
	cNumColunas = rhs.cNumColunas;

	return *this;
}

Matriz Matriz::operator-() const
{
	std::vector<double> res;
	res.resize(cNumColunas * cNumLinhas);

	for (size_t i = 0; i < cMatriz.size(); i++)
	{
		res[i] = -cMatriz[i];
	}
	Matriz m(std::move(res), cNumLinhas, cNumColunas);

	return m;
}

Matriz Matriz::operator*(const Matriz& rhs) const
{
	if (cNumColunas != rhs.cNumLinhas)
	{
		return Matriz({ 0.0 }, 0, 0);
	}

	std::vector<double> res;
	res.resize(cNumLinhas * rhs.cNumColunas);
	
	for (size_t i = 0; i < cNumLinhas; i++)
	{
		for (size_t j = 0; j < rhs.cNumColunas; j++)
		{
			double soma = 0.0;

			for (size_t k = 0; k < cNumColunas; k++)
			{
				soma += cMatriz[k + (i*cNumColunas)] * rhs.cMatriz[j + (k*rhs.cNumColunas)];
			}

			res[j + (i * rhs.cNumColunas)] = soma;
		}
	}

	Matriz m(std::move(res), cNumLinhas, rhs.cNumColunas);

	return m;
}

Matriz& Matriz::operator+=(const Matriz& rhs)
{
	if (rhs.cNumColunas == cNumColunas && rhs.cNumLinhas == cNumLinhas)
	{
		for (size_t i = 0; i < cMatriz.size(); i++)
		{
			cMatriz[i] += rhs.cMatriz[i];
		}

		return *this;
	}
	else
	{
		return *this;
	}
}