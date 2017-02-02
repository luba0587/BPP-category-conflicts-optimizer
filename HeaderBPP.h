#pragma once


#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>


#define M_GRANDE 1000000

//altere aqui o fator de capacidade desejado:
#define Cap_factor 1.2


#ifdef M_GRANDE
#include "gurobi_c++.h"
#endif


using namespace std;


/*************************************CLASSE para medir tempo de processamento **************************************/
class Timer
{
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	double elapsed() const {
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

/*************************************CLASSE CATEGORIES E SEUS M�TODOS **************************************/
class Categories {

public:

	int index; 		//�ndice da categoria, de 0 a nCategorias-1
	int compDegree;	//grau de compatibilidade da categoria

	//m�todo que inicializa Categoria usando seu �ndice e seu grau de compatibilidade
	Categories(int k, int degree)
		: index(k), compDegree(degree)
	{}

};

/*************************************ESTRUTURAS DE DADOS e FUN��ES GLOBAIS****************************************/

//matriz de compatibilidades, m por m, em que 0 indica incompatibilidade entre a categoria da linha e da coluna em quest�o (estrutura declarada no mesmo arquivo do main)
extern vector< vector<int> > compatibilityMatrix;

//vetor de categorias do problema (estrutura declarada no mesmo arquivo do main)
extern vector<Categories> allCategories;

/*************************************CLASSE INSTANCIAS E SEUS M�TODOS ****************************************/
class Instances {

private:

	string inputFile;		//nome do arquivo com dados de entrada
	int binCapacity;		//capacidade do bin
	int mCategories;		//n�mero de categorias da inst�ncia
	int nElements;			//n�mero de elementos da inst�ncia
	int lowerBound;			//lower bound, dado pela solu��o do problema relaxado (sem compatiblidades e com lineariza��o de xij)

	/*************************************ESTRUTURAS DE DADOS ****************************************/

	//vetor de objetos que devem ser inseridos nos bins, ordenados por categoria
	//obs: a posi��o � o �ndice do objeto e o valor armazenado indica o peso
	vector<int> allElements;

	//vetor que associa loja a cada objeto de allElements
	//obs: a posi��o � o �ndice do objeto e o valor armazenado � o �ndice da loja
	vector<int> storeOfElement;

	//vetor com a posi��o do primeiro elemento de cada categoria no vetor que guarda todos os objetos 
	//obs: a posi��o indica categoria e o valor indica posi��o no vetor allElements
	vector<int> firstObjectPosition;

	//m�todo que l� e grava a capacidade dos bins e os dados referentes aos objetos, armazenados nas estruturas de dados 
	void load();

	//m�todo que resolve problema relaxado
	void solveRelaxedProblem();

public:

	//construtor que cria inst�ncia usando o nome do arquivo de entrada
	Instances(string str, int m);

	//m�todo que retorna o vetor com todos os objetos
	vector<int> getElements() { return allElements; }

	//m�todo que retorna o peso do objeto j
	int getElementWeight(int j) { return allElements.at(j); }

	//m�todo que retorna o vetor com primeira posi��o de cada categoria
	vector<int> getCategoryPosition() { return firstObjectPosition; }
	
	//m�todo que retorna informa��o do nome da inst�ncia
	string getName() { return inputFile; }

	//m�todo que retorna informa��o de capacidade dos bins da inst�ncia
	int getBinCapacity() { return binCapacity; }

	//m�todo que retorna informa��o de quantidade de categorias
	int getM() { return mCategories; }
	
	//m�todo que retorna informa��o de quantidade de elementos da inst�ncia
	int getN() { return nElements; }

	//m�todo que retorna o lower bound do problema relaxado
	int getRelaxedLB() { return lowerBound; }

#ifdef M_GRANDE
	//m�todo que acha solu��o �tima do problema usando o Gurobi e retorna o n�mero de bins
	vector<int> findOptimum();
#endif

};


/*************************************CLASSE ELEMENTS E SEUS M�TODOS ****************************************/
class Elements {

public:

	int index;		//�ndice do elemento, de 0 a n-1
	int weight;		//peso do elemento
	int category;	//�ndice da categoria do elemento, de 0 a m-1

	//construtor: inicializa objeto usando seus �ndice, peso e categoria (todos inteiros)
	Elements(int j, int p, int k);

};


/*************************************CLASSE SOLUTION E SEUS M�TODOS ****************************************/
class Solution {

private:

	string instanceName;			//nome do arquivo de entrada da inst�ncia
	vector<int> codifiedSolution;	//solu��o codificada
	int numberOfBins;				//n�mero de bins utilizados	
	double fitnessValue;			//valor na fun��o fitness
	float processingTime;			//tempo transcorrido para alcan�ar essa solu��o

public:

	//vector<Bins> binsVector;		//vetor de bins - acessa primeiro bin, depois seu vetor de elementos
	vector< vector<int> > solution;	//solu��o em formato matricial: cada linha � um bin
							
	//construtor 1: dado nome da inst�ncia e solu��o em vetor �nico, cria matrix de solu��o  e calcula o n�mero de bins usado
	Solution(string inst, vector<int> solutionCode);

	//m�todo que retorna o nome da inst�ncia de uma solu��o
	string getInstName() { return instanceName; }

	//m�todo que retorna n�mero de bins da solu��o
	int getNBins() { return numberOfBins; }

	//m�todo que grava o tempo de processamento
	void setTime(float t) { processingTime = t; }

	//m�todo que retorna o tempo de rpocessamento de uma solu��o
	float getTime() { return processingTime; }

	//m�todo que imprime o vetor solu��o codificada
	vector<int> getCode() { return codifiedSolution; }

	//m�todo que redefine n�mero de bins de uma solu��o
	void resetNumberBins(int n) { numberOfBins = n; }

};
