#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <sstream>

#include "Term.h"

namespace Toy {

	class ParseTerm
	{
	public:
		explicit ParseTerm(std::string name) 
			: mFunctor(name)
		{}

		ParseTerm(std::string name, ParseTerm a0)
			: mFunctor(name)
		{
			mArguments.push_back(a0);
		}

		ParseTerm(std::string name, ParseTerm a0, ParseTerm a1)
			: mFunctor(name)
		{
			mArguments.push_back(a0);
			mArguments.push_back(a1);
		}

		const bool IsVariable()
		{
			return mArguments.size() == 0 && mFunctor[0] >= 'A' && mFunctor[0] <= 'Z';
		}

		const bool IsConstant()
		{
			return mArguments.size() == 0 && mFunctor[0] >= 'a' && mFunctor[0] <= 'z';
		}

		const bool IsStructure()
		{
			return mArguments.size() > 0;
		}

	private:
		std::string				mFunctor;
		std::vector<ParseTerm>	mArguments;

	public:
		void Serialize(std::stringstream& ss);

		friend class QueryCompiler;
	};

	class Parser
	{
	private:
		std::map < std::string, FunctorType > mNameTable;
	};

	void TestParser();
}