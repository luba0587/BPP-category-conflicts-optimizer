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

/*************************************CLASSE CATEGORIES E SEUS MÉTODOS **************************************/
class Categories {

public:

	int index; 		//índice da categoria, de 0 a nCategorias-1
	int compDegree;	//grau de compatibilidade da categoria

	//método que inicializa Categoria usando seu índice e seu grau de compatibilidade
	Categories(int k, int degree)
		: index(k), compDegree(degree)
	{}

};

/*************************************ESTRUTURAS DE DADOS e FUNÇÕES GLOBAIS****************************************/

//matriz de compatibilidades, m por m, em que 0 indica incompatibilidade entre a categoria da linha e da coluna em questão (estrutura declarada no mesmo arquivo do main)
extern vector< vector<int> > compatibilityMatrix;

//vetor de categorias do problema (estrutura declarada no mesmo arquivo do main)
extern vector<Categories> allCategories;

/*************************************CLASSE INSTANCIAS E SEUS MÉTODOS ****************************************/
class Instances {

private:

	string inputFile;		//nome do arquivo com dados de entrada
	int binCapacity;		//capacidade do bin
	int mCategories;		//número de categorias da instância
	int nElements;			//número de elementos da instância
	int lowerBound;			//lower bound, dado pela solução do problema relaxado (sem compatiblidades e com linearização de xij)

	/*************************************ESTRUTURAS DE DADOS ****************************************/

	//vetor de objetos que devem ser inseridos nos bins, ordenados por categoria
	//obs: a posição é o índice do objeto e o valor armazenado indica o peso
	vector<int> allElements;

	//vetor que associa loja a cada objeto de allElements
	//obs: a posição é o índice do objeto e o valor armazenado é o índice da loja
	vector<int> storeOfElement;

	//vetor com a posição do primeiro elemento de cada categoria no vetor que guarda todos os objetos 
	//obs: a posição indica categoria e o valor indica posição no vetor allElements
	vector<int> firstObjectPosition;

	//método que lê e grava a capacidade dos bins e os dados referentes aos objetos, armazenados nas estruturas de dados 
	void load();

	//método que resolve problema relaxado
	void solveRelaxedProblem();

public:

	//construtor que cria instância usando o nome do arquivo de entrada
	Instances(string str, int m);

	//método que retorna o vetor com todos os objetos
	vector<int> getElements() { return allElements; }

	//método que retorna o peso do objeto j
	int getElementWeight(int j) { return allElements.at(j); }

	//método que retorna o vetor com primeira posição de cada categoria
	vector<int> getCategoryPosition() { return firstObjectPosition; }
	
	//método que retorna informação do nome da instância
	string getName() { return inputFile; }

	//método que retorna informação de capacidade dos bins da instância
	int getBinCapacity() { return binCapacity; }

	//método que retorna informação de quantidade de categorias
	int getM() { return mCategories; }
	
	//método que retorna informação de quantidade de elementos da instância
	int getN() { return nElements; }

	//método que retorna o lower bound do problema relaxado
	int getRelaxedLB() { return lowerBound; }

#ifdef M_GRANDE
	//método que acha solução ótima do problema usando o Gurobi e retorna o número de bins
	vector<int> findOptimum();
#endif

};


/*************************************CLASSE ELEMENTS E SEUS MÉTODOS ****************************************/
class Elements {

public:

	int index;		//índice do elemento, de 0 a n-1
	int weight;		//peso do elemento
	int category;	//índice da categoria do elemento, de 0 a m-1

	//construtor: inicializa objeto usando seus índice, peso e categoria (todos inteiros)
	Elements(int j, int p, int k);

};


/*************************************CLASSE SOLUTION E SEUS MÉTODOS ****************************************/
class Solution {

private:

	string instanceName;			//nome do arquivo de entrada da instância
	vector<int> codifiedSolution;	//solução codificada
	int numberOfBins;				//número de bins utilizados	
	double fitnessValue;			//valor na função fitness
	float processingTime;			//tempo transcorrido para alcançar essa solução

public:

	//vector<Bins> binsVector;		//vetor de bins - acessa primeiro bin, depois seu vetor de elementos
	vector< vector<int> > solution;	//solução em formato matricial: cada linha é um bin
							
	//construtor 1: dado nome da instância e solução em vetor único, cria matrix de solução  e calcula o número de bins usado
	Solution(string inst, vector<int> solutionCode);

	//método que retorna o nome da instância de uma solução
	string getInstName() { return instanceName; }

	//método que retorna número de bins da solução
	int getNBins() { return numberOfBins; }

	//método que grava o tempo de processamento
	void setTime(float t) { processingTime = t; }

	//método que retorna o tempo de rpocessamento de uma solução
	float getTime() { return processingTime; }

	//método que imprime o vetor solução codificada
	vector<int> getCode() { return codifiedSolution; }

	//método que redefine número de bins de uma solução
	void resetNumberBins(int n) { numberOfBins = n; }

};
