// ConsoleApplication7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "HelpData.h"
#include "Benchmark.h"
#include "Algorithms.h"
#include <iostream>


int main(int argc, char* argv[])
{
	DOptions options = DOptions::createByCMDOptions(argc, argv);
	DBenchmarkState state;
	state.parseInputFiles(options.inFilename);

	std::vector<std::vector<DNode>> resultsData(state.numExperiments);
	std::vector<std::string*> resultsFilenames(state.numExperiments);
	for (size_t i = 0; i < state.numExperiments; ++i)
	{
		std::string& inFilename = state.inputFilename[i];
		int32_t numVerticies = state.numVertices[i];
		std::vector<DNode> nodes = DNode::parseFromFile(inFilename, numVerticies);
		
		size_t numRepeatings = std::max(2, numVerticies / 20);

		std::vector<DNode> nodes0 = nodes;
		BENCHMARK_MEAN(state.resData[i][0], FordFulkerson, numRepeatings, nodes0);

		std::vector<DNode> nodes1 = nodes;
		double resEK;
		BENCHMARK_MEAN(state.resData[i][1], EdmondsKarp, numRepeatings, nodes1);

		std::vector<DNode> nodes2 = nodes;
		double resD;
		BENCHMARK_MEAN(state.resData[i][2], Dinic, numRepeatings, nodes2);
		
		resultsData[i] = std::move(nodes0);
		resultsFilenames[i] = &inFilename;
	}
	
	for (size_t i = 0; i < state.numExperiments; ++i)
	{
		DNode::writeFlowsToFile(resultsData[i], *resultsFilenames[i]);
	}

	state.writeOutput(options.outFilename);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
