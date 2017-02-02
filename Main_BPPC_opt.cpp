/*******************************************
* PTR 5001 - Trabalho Final - Luísa Cavalcanti
*
* Problema de bin packing com bins homogêneos e incompatibilidades por categorias
* 	Instâncias do problema de roteirização
*
*
*******************************************/

#include "HeaderBPP.h"



/*************************************ESTRUTURAS DE DADOS GLOBAIS*************************************************/

//matriz de compatibilidades, m por m, em que 0 indica incompatibilidade entre a categoria da linha e da coluna em questão
vector< vector<int> > compatibilityMatrix;

//vetor de categorias do problema
vector<Categories> allCategories;


/*************************************FUNÇÕES sem CLASSE ***********************************************************/
//função que lê os dados de compatibilidade problema, armazena na compatibilityMatrix e no vetor allCategories
int loadCMatrix(string inputFileName) {

	//variável para armazenar o número de categorias do problema (retorna valor zero caso a leitura não seja realizada com sucesso)
	int nCategories = 0;

	//abre o arquivo de entrada da instância
	ifstream dataFile;
	dataFile.open(inputFileName);

	//se der erro na abertura do arquivo, imprime aviso
	if (dataFile.bad()) {
		cerr << "Nao foi possivel abrir arquivo de entrada!\n";
	}

	//se abrir com sucesso, lê e armazena os dados do arquivo
	else {

		cout << "\nArquivo com compatibilidades aberto com sucesso!" << endl;

		//variável inteira para armazenar os dados lidos
		int input;

		//string line para armazenar linhas inúteis
		string line;

		//ignora primeira linhas
		getline(dataFile, line);

		//usa segunda linha, que tem o cabeçalho das colunas, para verificar quantas categorias existem
		getline(dataFile, line);
		istringstream iss(line);
		while (iss >> nCategories) {}
		//cout << "Ha " << nCategories << "categorias no problema" << endl;

		//lê a matriz de compatibilidade linha a linha
		for (int lines = 1; lines <= nCategories; lines++) {

			//cria vetor temporário para guardar valores da linha
			vector<int> lineInput;

			//cria variável para guardar o grau de compatibilidade da categoria
			int degree = 0;

			//ignora primeira coluna, que tem apenas índice da categoria
			dataFile >> input;

			//...loop que lê os dados coluna a coluna
			for (int columns = 1; columns <= nCategories; columns++) {

				//lê valor
				dataFile >> input;

				//soma no grau de compatibilidade da categoria
				degree += input;

				//armazena no vetor lineInput
				lineInput.push_back(input);

			}//fim do loop que lê e grava dados da linha

			 //guarda linha lida na matriz de compatibilidades
			compatibilityMatrix.push_back(lineInput);

			//cria categoria
			Categories k(lines - 1, degree);

			//armazena na estrutura de dados
			allCategories.push_back(k);

		}//fim do loop que lê e armazena a matriz de compatibilidade entre as nCategories categorias

	}//fim do procedimento realizado caso o arquivo de entrada tenha sido aberto com sucesso

	dataFile.close();

	return nCategories;

}



/*************************************MAIN **********************************************************************/
int main() {

	//vetor com todas as instâncias que devem ser executadas
	vector<string> allInstances;

	//abre o arquivo de entrada da instância
	ifstream dataFile;

	dataFile.open("ArquivosIntancias-saidas.txt");

	string line;

	while (getline(dataFile, line)) {
		allInstances.push_back(line);
	}

	//carrega matriz de compatibilidade e salva qtde. de categorias na variável nCategories
	int m = loadCMatrix("Reduced_set.vrp");

	//se houver sucesso na leitura da matriz de compatibilidade, carrega dados dos objetos, já guardando nas estruturas de dados apropriadas
	if (m > 0) {

		cout << "instancias: " << allInstances.size() << endl;

		//para cada uma das instâncias do arquivo de entrada, executa o gurobi e gera arquivo de saída com a solução encontrada 
		for (unsigned i = 0; i < allInstances.size(); i++) {

			//cria instância usando o nome do arquivo de entrada e a qtde de categorias do problema
			Instances inst(allInstances.at(i), m);
			
#ifdef M_GRANDE

			//*****encontra LB do problema, pela otimização, usando o Gurobi
			Timer t;
			Solution s(inst.getName(), inst.findOptimum());
			s.setTime(t.elapsed());

#endif
		
			//nome do arquivo com tabela 1 - resumo dos resultados
			ostringstream outputFile;
			//outputFile << "out-" << inst.getName() << "-" << Cap_factor << "xCap.txt";
			outputFile << "out" << i+1 << "-" << Cap_factor << "xCap.txt";

			//cria arquivo de resultados
			ofstream tabela;
			tabela.open(outputFile.str());

			//cria cabeçalho da tabela
			tabela << "Fator Capacidade;Instancia;Capacidade Bin;Numero Itens;LB BPPrelaxado;Gurobi-#Bins;TempoGurobi(s)\n";

			//salva saídas da tabela 
			tabela << Cap_factor << ";"
				<< inst.getName() << ";"
				<< inst.getBinCapacity() << ";"
				<< inst.getN() << ";"
				<< inst.getRelaxedLB() << ";";
	#ifdef M_GRANDE
			tabela << s.getNBins() << ";";
			tabela << s.getTime();
	#endif // M_GRANDE

			tabela << "\n\n\n";

			//cria cabeçalho da tabela 2
			tabela << "Bin;Peso total;Elementos(j)\n";
			for (unsigned b = 0; b < s.solution.size(); b++) {

				//variável inteira para armazenar o peso total do bin (dos elementos nele armazenados)
				int binWeight = 0;
				
				//calcula o peso total do bin
				for (unsigned j = 0; j < s.solution.at(b).size(); j++) {
					binWeight += inst.getElementWeight(s.solution.at(b).at(j));
				}

				//grava no arq de saída
				tabela << b + 1 << ";"
					<< binWeight << ";";

				for (unsigned j = 0; j < s.solution.at(b).size(); j++) {
					tabela << s.solution.at(b).at(j) << ",";
				}

				tabela << "\n";

			}//fim da impressão da asolução em forma matricial (tabela 2)

			tabela.close();

		}//fim do loop que percorre todas as intâncias do vetor allInstances


	}//fim da condicional que só roda as instâncias se a matriz de compatibilidade tiver sido carregada com sucesso

	system("pause");

	return 0;

}
