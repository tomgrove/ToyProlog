#pragma once

#include <map>
#include "term.h"
#include "parser.h"
#include "machine.h"

namespace Toy {

	class QueryCompiler
	{
	public:
		QueryCompiler() 
			: mNextName(0)
		{}
		void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg );
		void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructionsn );
	private:
		FunctorType GetName(const std::string& name);
		std::map < std::string, FunctorType > mNameTable;
		std::map< std::string, uint32_t> mVariables;
		FunctorType mNextName;
	};

	void TestQuery();
}