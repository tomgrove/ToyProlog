#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <sstream>

#include "Maybe.h"
#include "Term.h"

namespace Toy {

	class ParseTerm
	{
	public:

		ParseTerm()
		{}

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

		ParseTerm(std::string name, ParseTerm a0, ParseTerm a1, ParseTerm a2)
			: mFunctor(name)
		{
			mArguments.push_back(a0);
			mArguments.push_back(a1);
			mArguments.push_back(a2);
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

		friend class L0QueryCompiler;
		friend class L1QueryCompielr;
		friend class L0Compiler;
		friend class L1Compiler;
	};

	class Parser
	{
	public:
		Parser()
			: mFunctorId(0)
		{}
		Maybe<ParseTerm> ParseOneTerm(char* input, char** rest);
	public:
		void ParseWhitespace(char* input, char** rest);
		std::string ParseAlphaNumeric(char* input, char** rest);
		Maybe<FunctorType> ParseFunctor(char* input, char** rest);
		Maybe<FunctorType> ParseVariable(char* input, char** rest);
		FunctorType GetOrAddName(std::string name);
	private:
		std::map < std::string, FunctorType > mNameTable;
		FunctorType							  mFunctorId;
	};
}