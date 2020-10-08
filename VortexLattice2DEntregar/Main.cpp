#define _USE_MATH_DEFINES

// Bibliotecas padrão C++
#include <iostream>
#include <string>
#include <fstream>

// Biblioteca Win32
#include <Windows.h>

// Bibliotecas criadas
#include "MathUtilsM.h"
#include "Matriz.h"
#include "CamberLine4412.h"
#include "CamberLinePlacaPlana.h"
#include "FunctionMathDiscrete.h"

constexpr UINT WM_USERDEFINED_DRAWPERFIL = 0x1010;

// Estrutura para armazenar os dados calculados pelo método vortex lattice
struct PropriedadesPerfil
{
	double CL;
	double CM;
	double CP;
	double alfa; // Graus
	std::vector<std::pair<double, double>> Vortices; // 1 -> intensidade, 2 -> posição x
	FunctionMath<double>* pCamberFunction = nullptr;
	double corda;
};

// Estrutura básica de vetor bidimensional
struct Vetor2D
{
	double x;
	double y;
};

// Declaração de função responsável por alterar a cor de um pixel em uma posição de um buffer especifico.
void ColocarPixel(int x, int y, int cor, int* pBuffer, int pitch);

// Declaração de função que cria a janela que será responsável por exibir linhas de corrente.
void MostrarEscoamento(const PropriedadesPerfil& perfil);

// Declaração de função que calcula as propriedades do perfil através do método vortex lattice
PropriedadesPerfil CalculaPerfil(const FunctionMath<double>& perfil, size_t nVortices, double corda, double alfaGraus);

// Declaração de função auxiliar que retorna a velocidade em um ponto especifico do escoamento dado os parametros. 
// A posição é adimensionalizada em relação a corda do perfil e o ponto (0, 0) corresponde ao bordo de ataque.
Vetor2D PegarVelocidadeNoEscoamento(const Vetor2D& posRel, double vinf, double alfa, const PropriedadesPerfil& perfil);

// Declaração de função padrão que faz o handling da janela (responde às mensagens/comandos enviados pelo sistema/usuário
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

// Função principal do programa (executada ao abrir o programa)
int main()
{
	int opt = 0;
	int nVortices = 0;
	std::cout << "Digite o numero de vortices: ";
	std::cin >> nVortices;

	std::cout << "Digite 1 para perfis definidos por arquivo .csv e 2 para perfis pre-definidos: ";
	std::cin >> opt;
	std::cout << std::endl << std::endl;
	if (opt == 1)
	{
		std::string nomeArq;
		
		std::cout << "Digite o nome do arquivo: ";

		std::cin >> nomeArq;

		double corda;
		std::cout << "Digite o tamanho da corda: ";
		std::cin >> corda;
		std::ifstream aq(nomeArq);
		if (aq.is_open())
		{
			aq.close();

			FunctionMathDiscrete<double> camber(corda, nomeArq);

			double alfa = 0.0;

			while (abs(alfa) < 90.0)
			{
				std::cout << "Digite o angulo de ataque em graus (Digite qualquer numero maior que 90 para sair): ";
				std::cin >> alfa;


				PropriedadesPerfil propPerfil = CalculaPerfil(camber, nVortices, corda, alfa);

				std::cout << "CL " << alfa << " ang. Ataque: " << propPerfil.CL << std::endl;
				std::cout << "Cm1/4 " << propPerfil.CM << std::endl;
				std::cout << "CP " << propPerfil.CP << std::endl;

				propPerfil.pCamberFunction = &camber;
				propPerfil.corda = camber.GetChord();

				MostrarEscoamento(propPerfil);
			}
			
		}
		else
		{
			std::cout << "Arquivo nao encontrado." << std::endl;
		}
	}
	else
	{
		std::cout << "Digite 1 para placa plana ou 2 para o perfil NACA 4412:";
		std::cin >> opt;

		if (opt == 1)
		{
			double alfa = 0.0;
			while (abs(alfa) < 90.0)
			{
				CamberLinePlacaPlana<double> c4412(1.0);
				std::cout << "Digite o angulo de ataque em graus (Digite qualquer numero maior que 90 para sair): ";
				std::cin >> alfa;


				PropriedadesPerfil propPerfil = CalculaPerfil(c4412, nVortices, 1.0, alfa);

				std::cout << "CL " << alfa << " ang. Ataque: " << propPerfil.CL << std::endl;
				std::cout << "Cm1/4 " << propPerfil.CM << std::endl;
				std::cout << "CP " << propPerfil.CP << std::endl;

				propPerfil.pCamberFunction = &c4412;
				propPerfil.corda = c4412.GetChord();

				MostrarEscoamento(propPerfil);
			}
		}
		else
		{
			double alfa = 0.0;
			while (abs(alfa) < 90.0)
			{
				CamberLine4412<double> c4412(1.0);
				std::cout << "Digite o angulo de ataque em graus (Digite qualquer numero maior que 90 para sair): ";
				std::cin >> alfa;


				PropriedadesPerfil propPerfil = CalculaPerfil(c4412, nVortices, 1.0, alfa);

				std::cout << "CL " << alfa << " ang. Ataque: " << propPerfil.CL << std::endl;
				std::cout << "Cm1/4 " << propPerfil.CM << std::endl;
				std::cout << "CP " << propPerfil.CP << std::endl;

				propPerfil.pCamberFunction = &c4412;
				propPerfil.corda = c4412.GetChord();

				MostrarEscoamento(propPerfil);
			}
		}
	}
	
	return 0;
}

// Definições das funções declaradas anteriormente

PropriedadesPerfil CalculaPerfil(const FunctionMath<double>& perfil, size_t nVort, double c, double alfaGraus)
{
	const size_t n = nVort; 
	const double corda = c;
	const double dx = 1.0 / (n);

	// Declara matriz que armazenará os coeficientes de influência
	Matriz a = MatrizZeros(n, n);

	std::vector<double> *aptr = a.GetPtrMatriz();


	std::vector<double> bM;
	for (size_t k = 0; k < n; k++)
	{
		const double xk = (k * dx) + (0.75*dx);

		// Velocidade no ponto (efeito do ang. ataque + arqueamento)
		bM.push_back(perfil.GetDerivative(xk * corda) - ((M_PI / 180.0)*alfaGraus));
		for (size_t j = 0; j < n; j++)
		{
			const double xj = (j * dx) + (0.25*dx);
			// Coeficiente de influência "k" "j"
			(*aptr)[j + (k*a.GetNumColunas())] = -1.0 / (2.0*M_PI*(xk - xj));
		}
	}

	Matriz b(bM, n, 1);
	// Resolve sistema linear
	Matriz x = ResSistLinearPALU(a, b);


	PropriedadesPerfil res;
	res.alfa = alfaGraus;

	double circTotal = 0.0;

	// Calcula a circulação total do perfil
	for (size_t i = 0; i < x.GetPtrMatriz()->size(); i++)
	{
		circTotal += (*x.GetPtrMatriz())[i];

		const double xj = (i * dx) + (0.25*dx);

		res.Vortices.push_back({ (*x.GetPtrMatriz())[i], xj });
	}

	double momUmQuarto = 0.0;

	// Calcula o momento em relação a um quarto de corda
	for (size_t i = 0; i < x.GetPtrMatriz()->size(); i++)
	{
		const double xj = (i * dx) + (0.25*dx);
		momUmQuarto += ((*x.GetPtrMatriz())[i] * 2.0 * ((0.25*c) - xj));
	}

	double momBordoAtaque = 0.0;

	// Calcula o momento em relação ao bordo de ataque
	for (size_t i = 0; i < x.GetPtrMatriz()->size(); i++)
	{
		const double xj = (i * dx) + (0.25*dx);
		momBordoAtaque += ((*x.GetPtrMatriz())[i] * 2.0 * (-xj));
	}
	
	res.CL = circTotal*2.0;
	res.CM = momUmQuarto;
	res.CP = -momBordoAtaque / (circTotal*2.0);

	// Retorna resultados
	return res;
}

void MostrarEscoamento(const PropriedadesPerfil& perfil)
{
	const wchar_t* nomeClasse = L"Linhasdecorrenteaero";

	std::wstring nmjanela;
	nmjanela = L"Linhas de Corrente para o perfil - CL:";
	nmjanela += std::to_wstring(perfil.CL);

	const wchar_t* nomeJanela = nmjanela.c_str();

	HINSTANCE hInst = GetModuleHandle(NULL);
	WNDCLASS wc = {};
	wc.hInstance = hInst;
	wc.lpszClassName = nomeClasse;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = MsgProc;

	RegisterClass(&wc);

	// Cria janela
	HWND hWnd = CreateWindow(nomeClasse, nomeJanela, WS_OVERLAPPED | WS_SYSMENU, 0, 0, 800, 400, NULL, NULL, hInst, NULL);
	
	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	
	RECT r = {};

	GetClientRect(hWnd, &r);

	const int width = r.right - r.left;
	const int height = r.bottom - r.top;

	char* buffer = new char[width * height * 4];

	ZeroMemory(buffer, width * height * 4);

	int* pIntBuffer = reinterpret_cast<int*>(buffer);

	// Número de linhas de corrente
	const size_t numeroLinhas = 50;

	// Número máximo de iterações para o calculo das linhas
	const size_t maxIt = 1500;

	// Número de pixels para a corda do perfil
	const size_t cordaPerfilMostrado = 300;
	
	// Distância horizontal da borda da janela (esquerda)
	const double xOffsetPerfil = 300.0;
	 
	// Distância vertical da borda da janela (inferior)
	const double yOffsetPerfil = 200.0;
	 
	// Passo no tempo de cada "particula"
	const double dt = 0.7;

	for (size_t i = 0; i < numeroLinhas; i++)
	{
		double posx = 0.0;
		double posy = (i * (height / numeroLinhas));

		size_t nIt = 0;

		// Repete até o número maximo de iterações não for alcançado ou se a particula ultrapassa alguma das bordas da janela.
		while (nIt < maxIt && (posx < width) && (posy < height) && posy >= 0 && posx >= 0)
		{
			const Vetor2D vel = PegarVelocidadeNoEscoamento({ (posx - xOffsetPerfil) / cordaPerfilMostrado, (posy - yOffsetPerfil) / cordaPerfilMostrado }, 1.0, perfil.alfa, perfil);

			int cor = 0xFFFFFFFF;
			ColocarPixel((int)posx, (int)posy, cor, pIntBuffer, width);

			posx += vel.x * dt;
			posy += vel.y * dt;

			nIt++;
		}

#ifdef _DEBUG
		std::cout << "Finalizado com " << nIt << " iteracoes" << std::endl;
#endif
	}

	const double corda = perfil.corda;
	for (size_t i = 0; i < cordaPerfilMostrado; i++)
	{
		double z = (*perfil.pCamberFunction)(double(corda * i / double(cordaPerfilMostrado))) * cordaPerfilMostrado;
		ColocarPixel(int(i + xOffsetPerfil), int(z + yOffsetPerfil), 0x0000FF, pIntBuffer, width);
	}
   
	// Envia buffer para ser desenhado na janela.
	PostMessage(hWnd, WM_USERDEFINED_DRAWPERFIL, reinterpret_cast<UINT>(buffer), 0);
	   
#ifdef _WIN64 // Se compilar para 64 bits
	Não vai funcionar, ponteiro possui tamanho diferente que um int.
#endif

	MSG msg = {};

	// Enquanto o comando/mensagem for diferente de WM_QUIT; processa comandos da janela
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete[]buffer;

	// Destroi janela
	DestroyWindow(hWnd);
	UnregisterClass(nomeClasse, hInst);
}

void ColocarPixel(int x, int y, int cor, int* pBuffer, int pitch)
{
	pBuffer[x + (y * pitch)] = cor;
}

Vetor2D PegarVelocidadeNoEscoamento(const Vetor2D& posRel, double vinf, double alfa, const PropriedadesPerfil& perfil)
{
	Vetor2D resp = {};

	alfa = alfa * M_PI / 180.0;

	resp.x += cos(alfa) * vinf;
	resp.y += sin(alfa) * vinf;


	for (size_t i = 0; i < perfil.Vortices.size(); i++)
	{
		// posicao vertical do vortice
		double zCam = (*perfil.pCamberFunction)(perfil.Vortices[i].second);
		// Distancia até o ponto
		double r = sqrt(pow(posRel.x - perfil.Vortices[i].second, 2)+ pow(posRel.y - zCam, 2));
	
		// velocidade tangencial
		double vtheta = -perfil.Vortices[i].first / (2.0 * M_PI * r);
	
		// angulo que a velocidade tangencial faz com a horizontal
		const double beta = M_PI_2 - atan2((posRel.y - zCam), posRel.x - perfil.Vortices[i].second);

		resp.x += vtheta*cos(beta);
		resp.y += vtheta * sin(beta);
	}
	
	return resp;
}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_USERDEFINED_DRAWPERFIL:
	{
		char* buffer = reinterpret_cast<char*>(wparam);

		RECT r = {};

		GetClientRect(hWnd, &r);

		const int width = r.right - r.left;
		const int height = r.bottom - r.top;

		BITMAPINFO bi = {};
		bi.bmiHeader.biSize = sizeof(BITMAPINFO);
		bi.bmiHeader.biWidth = width;
		bi.bmiHeader.biHeight = height;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biCompression = BI_RGB;
		bi.bmiHeader.biSizeImage = width * height * 4;

		ValidateRect(hWnd, nullptr);
		HDC hdc = GetDC(hWnd);

		SetStretchBltMode(hdc, HALFTONE);
		// Exibe na janela o buffer onde foi armazenado os pixels.
		int res = StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, buffer, &bi, DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(hWnd, hdc);

		if (res == 0)
		{
			MessageBox(NULL, L"Erro ao desenhar na tela.", L"Erro", 0);
		}
	}break;
	
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wparam, lparam);
		break;
	}
	return 0;
}