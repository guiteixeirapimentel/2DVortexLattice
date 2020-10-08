#pragma once
#include <math.h>
#include "Matriz.h"
#include <iostream>

// Declarações das funções a serem definidas em "MathUtilsM.cpp"

// Função que retorna matriz com valores aleatórios (0,0 - 1,0)
Matriz CriaMatrizAleatoria(size_t numlinhas, size_t numColunas);

// Função que realiza substituições sucessivas (resolução de sistemas triangulares inferior)
Matriz SubstSucessivas(const Matriz& L, const Matriz& b);

// Função que realiza substituições sucessivas (resolução de sistemas triangulares superior)
Matriz SubstRetroativas(const Matriz& U, const Matriz& b);

// Função que realiza a decomposição LU da matriz (com pivotação)
void DecompPALU(const Matriz& A, Matriz& POut, Matriz& LOut, Matriz& UOut);

// Função auxiliar que escreve matriz na tela
void MostrarMatriz(const Matriz& M);

// Função que resolve sistemas lineares através da decomposição LU
Matriz ResSistLinearPALU(const Matriz& A, const Matriz& b);

// Função auxiliar que troca linhas em uma matriz
void TrocaLinha(Matriz& m, size_t l1, size_t l2);

// Função que retorna uma matriz identidade nxn
Matriz MatrizI(size_t n);
// Função que retorna uma matriz nula nxm
Matriz MatrizZeros(size_t n, size_t m);

// Função que retorna o indice do pivo da linha
size_t AchaIndicePivo(const Matriz& m, size_t nColuna, size_t nLinInicial);