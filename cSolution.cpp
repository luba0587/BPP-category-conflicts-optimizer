#include "HeaderBPP.h"

//construtor que cria matrix de solução decodificando a representação da solução em vetor único
Solution::Solution(string inst, vector<int> solutionCode)
	: instanceName(inst), codifiedSolution(solutionCode)
{
	vector<int> elementsOnBin;
	for (unsigned j = 0; j < solutionCode.size(); j++) {
		if (solutionCode.at(j) >= 0) elementsOnBin.push_back(solutionCode.at(j));
		else if (elementsOnBin.size()>0) {
			solution.push_back(elementsOnBin);
			elementsOnBin.clear();
		}
	}
	solution.push_back(elementsOnBin);
	numberOfBins = solution.size();
}
