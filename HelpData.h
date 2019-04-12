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
		options.outFilename = inFilenameData;
	}
};

struct DBenchmarkState
{
	std::vector<std::string> inputFilename;
	std::vector<std::int32_t> numVertices;
	std::vector<std::array<double, 3>> resData;

public:
	void parseInputFiles(const std::string& filename)
	{
		std::fstream fin;
		fin.open(filename, std::ios::in);
		assert(fin.is_open());

		std::string s;
		while (std::getline(fin, s))
		{
			int32_t numV;
			fin >> numV;

			std::string filenameStr;
			fin >> filenameStr;
			inputFilename.emplace_back(std::move(filenameStr));
			numVertices.push_back(numV);
		}
		fin.close();
	}

	void writeOutput(const std::string& filename)
	{
		std::fstream out;
		out.open(filename, std::ios::out);
		assert(out.is_open());

		for (std::size_t i = 0; i < resData.size(); ++i)
		{
			out << filename << ";" << resData[i][0] << ";" << resData[i][1] << ";" << resData[i][2] << "\n";
		}

		out.close();
	}
};

struct DNode
{
	std::vector<DNode*> children;
	std::vector<uint8_t> flow;

public:
	static DNode parseFromFile(const std::string& filename, std::size_t numVertices)
	{
		std::fstream fin;
		fin.open(filename, std::ios::in);
		assert(fin.is_open());
		std::string s;


		DNode;
		while (std::getline(fin, s))
		{
			for (std::size_t i = 0; i < numVertices; ++i)
			{

			}
		}


		fin.close();
	}
};
