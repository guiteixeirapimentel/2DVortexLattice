	#include "MathUtilsM.h"
	#include <assert.h>

	// Definições das funções definidas em "MathUtilsM.h"

	Matriz CriaMatrizAleatoria(size_t numlinhas, size_t numColunas)
	{
		std::vector<double> buf;
		buf.resize(numlinhas * numColunas);

		for (size_t i = 0; i < buf.size(); i++)
		{
			buf[i] = ((double(rand()) / RAND_MAX) - 0.5);
		}
		Matriz m(std::move(buf), numlinhas, numColunas);

		return m;
	}

	Matriz SubstSucessivas(const Matriz& L, const Matriz& b)
	{
		std::vector<double> res;
		res.resize(b.cMatriz.size());
		res[0] = b.cMatriz[0] / L.cMatriz[0 + (0 * L.cNumColunas)];

		for (size_t index = 1; index < res.size(); index++)
		{
			double soma = 0.0;
			for (size_t j = 0; j < index; j++)
			{
				soma += L.cMatriz[j + (index * L.cNumColunas)] * res[j];
			}
			res[index] = (b.cMatriz[index] - soma) / L.cMatriz[index + (index * L.cNumColunas)];
		}

		Matriz m(std::move(res), b.cNumLinhas, b.cNumColunas);

		return m;
	}

	Matriz SubstRetroativas(const Matriz& U, const Matriz& b)
	{
		std::vector<double> res;
		res.resize(b.cMatriz.size());

		res[b.cMatriz.size() - 1] = b.cMatriz.back() / U.cMatriz[(U.cNumColunas - 1) + ((U.cNumLinhas - 1) * U.cNumColunas)];

		for (size_t index = res.size() - 2; index > 0; index--)
		{
			double soma = 0.0;
			for (size_t j = U.cNumColunas - 1; j > index; j--)
			{
				soma += U.cMatriz[j + (index * U.cNumColunas)] * res[j];
			}
			res[index] = (b.cMatriz[index] - soma) / U.cMatriz[index + (index * U.cNumColunas)];
		}

		double soma = 0.0;
		for (size_t j = U.cNumColunas - 1; j > 0; j--)
		{
			soma += U.cMatriz[j + (0 * U.cNumColunas)] * res[j];
		}
		res[0] = (b.cMatriz[0] - soma) / U.cMatriz[0 + (0 * U.cNumColunas)];

		Matriz m(std::move(res), b.cNumLinhas, b.cNumColunas);

		return m;
	}

	void MostrarMatriz(const Matriz& M)
	{
		std::cout << "= {";
		for (size_t i = 0; i < M.cNumLinhas; i++)
		{
			std::cout << "[";

			for (size_t j = 0; j < M.cNumColunas; j++)
			{
				std::cout << M.cMatriz[j + (i * M.cNumColunas)] << ", ";
			}

			std::cout << "]," << std::endl;
		}

		std::cout << "};" << std::endl;
	}

	void DecompPALU(const Matriz& A, Matriz& POut, Matriz& LOut, Matriz& UOut)
	{
		/*
		L = {[1 0 0 .. 0],
			 [M 1 0 .. 0],
			 [M M 1 .. 0],
			 [M M M .. 1]};

		U = {[ Linha Pivô 1],
			 [ Linha Pivô 2],
			 [ Linha Pivô n],};

		P = {[ 1 na coluna = linha pivô 1],
			 [ 1 na coluna = linha pivô 2],
			 [ 1 na coluna = linha pivô n]};
		*/

		const size_t m = A.cNumLinhas;
		const size_t n = A.cNumColunas;

		Matriz ML = MatrizZeros(n, n);
		Matriz MP = MatrizI(A.cNumColunas);
		Matriz MU = A;

		std::vector<double>& L = ML.cMatriz;
		std::vector<double>& U = MU.cMatriz;
		std::vector<double>& P = MP.cMatriz;


		for (size_t k = 0; k < n; k++)
		{
			const size_t ipiv = AchaIndicePivo(MU, k, k);
			TrocaLinha(MU, k, ipiv);
			TrocaLinha(MP, k, ipiv);
			TrocaLinha(ML, k, ipiv);

			for (size_t j = k + 1; j < n; j++)
			{
				L[k + (j * ML.cNumColunas)] = U[k + (j * MU.cNumColunas)] / U[k + (k * MU.cNumColunas)];
				for (size_t index = k; index < n; index++)
				{
					U[index + (j * MU.cNumColunas)] -= L[k + (j * MU.cNumColunas)] * U[index + (k * MU.cNumColunas)];
				}
			}

		}

		ML += MatrizI(n);

		POut = std::move(MP);
		LOut = std::move(ML);
		UOut = std::move(MU);
	}

	void TrocaLinha(Matriz& matriz, size_t l1, size_t l2)
	{
		for (size_t j = 0; j < matriz.cNumColunas; j++)
		{
			double tmp = matriz.cMatriz[j + (l2*matriz.cNumColunas)];
			matriz.cMatriz[j + (l2*matriz.cNumColunas)] = matriz.cMatriz[j + (l1*matriz.cNumColunas)];
			matriz.cMatriz[j + (l1*matriz.cNumColunas)] = tmp;
		}
	}

	Matriz MatrizI(size_t n)
	{
		std::vector<double> res;
		res.resize(n * n);

		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < n; j++)
			{
				if (i != j)
				{
					res[j + (i * n)] = 0.0;
				}
				else
				{
					res[j + (i * n)] = 1.0;
				}
			}
		}

		Matriz mres(std::move(res), n, n);

		return mres;
	}

	size_t AchaIndicePivo(const Matriz& m, size_t nColuna, size_t nLinInicial)
	{
		size_t res = 0;
		double maior = 0.0;
		for (size_t i = nLinInicial; i < m.cNumLinhas; ++i)
		{
			if (nLinInicial == m.cNumColunas)
			{
				int x = 0;
			}
			const double val = fabs(m.cMatriz[nColuna + (i * m.cNumColunas)]);

			if (val > maior)
			{
				res = i;
				maior = fabs(val);
			}
		}

		return res;
	}

	Matriz MatrizZeros(size_t n, size_t m)
	{
		std::vector<double> res;
		res.resize(n * m);

		for (size_t i = 0; i < res.size(); i++)
		{
			res[i] = 0.0;
		}

		return Matriz(std::move(res), n, m);
	}

	Matriz ResSistLinearPALU(const Matriz& A, const Matriz& b)
	{
		Matriz P = MatrizZeros(1, 1);
		Matriz U = MatrizZeros(1, 1);
		Matriz L = MatrizZeros(1, 1);

		DecompPALU(A, P, L, U);

		Matriz Pb = P * b;

		Matriz K = SubstSucessivas(L, Pb);

		// K = UX

		Matriz x = SubstRetroativas(U, K);

		return x;
	}