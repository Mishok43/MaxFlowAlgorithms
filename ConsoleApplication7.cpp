// ConsoleApplication7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "HelpData.h"
#include "Benchmark.h"
#include "Algorithms.h"
#include <iostream>

#define FORD_FULKRESON_ENABLED
#define EDMOND_KARP_ENABLED
#define DINIC_ENABLED
#define OUTPUTING_GRAPHS


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
		std::vector<DNode> graph = DNode::parseFromFile(inFilename, numVerticies);
		std::vector<DNode> rGraph = DNode::makeResidualGraph(graph); // make residual graph before the executing of algorithms for correcting counting time of execution

		size_t numRepeatings = 10;

#ifdef FORD_FULKRESON_ENABLED
		std::vector<DNode> rGraph0 = rGraph;
		BENCHMARK_MEAN(state.resData[i][0], FordFulkerson, numRepeatings, rGraph0);
#endif
		
#ifdef EDMOND_KARP_ENABLED
		std::vector<DNode> rGraph1 = rGraph;
		BENCHMARK_MEAN(state.resData[i][1], EdmondsKarp, numRepeatings, rGraph1);
#endif

#ifdef DINIC_ENABLED
		std::vector<DNode> rGraph2 = rGraph;
		BENCHMARK_MEAN(state.resData[i][2], Dinic, numRepeatings, rGraph2);
#endif
		
#ifdef OUTPUTING_GRAPHS
		DNode::fromResidualGraphToNormal(graph, rGraph1);
		resultsData[i] = std::move(graph);
		resultsFilenames[i] = &inFilename;
#endif

		std::cout << i << std::endl;
	}

#ifdef OUTPUTING_GRAPHS
	for (size_t i = 0; i < state.numExperiments; ++i)
	{
		DNode::writeFlowsToFile(resultsData[i], *resultsFilenames[i]);
	}
#endif

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
