
#pragma once
#include "pch.h"
#include <string>
#include <vector>
#include <fstream>
#include <array>

char* getCMDOption(char **begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return nullptr;
}

bool cmdOptionExists(char** begin, char ** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}


struct DOptions
{
	std::string outFilename;
	std::string inFilename;

	static DOptions createByCMDOptions(int argc, char* argv[])
	{
		DOptions options;

		char** argBegin = argv;
		char** argEnd = argv + argc;

		char* outFilenameData = getCMDOption(argBegin, argEnd, "-o");
		assert(outFilenameData != nullptr);
		options.outFilename = outFilenameData;

		char* inFilenameData = getCMDOption(argBegin, argEnd, "-i");
		assert(inFilenameData != nullptr);
		options.inFilename = inFilenameData;

		return options;
	}
};

struct DBenchmarkState
{
	std::vector<std::string> inputFilename;
	std::vector<std::int32_t> numVertices;
	std::vector<std::array<double, 3>> resData;

	uint32_t numExperiments;

public:
	void parseInputFiles(const std::string& filename)
	{
		std::fstream fin;
		fin.open(filename, std::ios::in);
		assert(fin.is_open());

		std::string s;
		do
		{
			int32_t numV;
			fin >> numV;

			std::string filenameStr;
			fin >> filenameStr;
			inputFilename.emplace_back(std::move(filenameStr));
			numVertices.push_back(numV);
		} while (!fin.eof());
		fin.close();

		numExperiments = inputFilename.size();

		resData.resize(numExperiments);
	}

	void writeOutput(const std::string& filename)
	{
		std::fstream out;
		out.open(filename, std::ios::out);
		assert(out.is_open());

		for (std::size_t i = 0; i < resData.size(); ++i)
		{
			out << inputFilename[i] << ";" << resData[i][0] << ";" << resData[i][1] << ";" << resData[i][2] << "\n";
		}

		out.close();
	}
};

struct DNode
{
	// stores children by index for saving space and increasing cache locality. the same for flow
	std::vector<uint16_t> children;
	std::vector<uint8_t> flow;

public:
	static std::vector<DNode> parseFromFile(const std::string& filename, size_t numVertices)
	{
		std::fstream fin;
		fin.open("inputs/"+filename, std::ios::in);
		assert(fin.is_open());



		std::vector<DNode> nodes(numVertices);

		
		for(std::size_t i=0; i < numVertices; ++i)
		{
			DNode& node = nodes[i];
			for (size_t j = 0; j < numVertices; ++j)
			{
				uint16_t d;
				uint8_t capacity;
				fin >> d;
				capacity = (uint8_t)d;
				if (capacity != 0)
				{
					node.children.push_back(j);
					node.flow.push_back(capacity);
				}
			}
		}

		fin.close();

		return nodes;
	}

	static void writeFlowsToFile(const std::vector<DNode> data, const std::string& filename)
	{
		std::fstream fout;
		fout.open("results/"+filename, std::ios::out);
		assert(fout.is_open());

		uint32_t maxFlow = 0;
		const DNode& rootNode = data[0];
		for (size_t i = 0; i < rootNode.flow.size(); ++i)
		{
			maxFlow += rootNode.flow[i];
		}

		fout << maxFlow << "\n";
		for (size_t i = 0; i < data.size(); ++i)
		{
			const DNode& node = data[i];
			for (size_t j = 0; j < node.flow.size(); ++j)
			{
				fout << i << " -> " << node.children[j] << ": " << (uint16_t)(node.flow[j]) << "\n";
			}
		}
		fout.close();
	}
};
