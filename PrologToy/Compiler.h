#pragma once

#include <vector>
#include <map>
#include "parser.h"
#include "Machine.h"

namespace Toy {

	class Compiler
	{
	public:
		Compiler()
			: mNextName(0)
		{}
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions) = 0;
	protected:
		FunctorType GetName(const std::string& name);

		std::map < std::string, FunctorType >	mNameTable;
		std::map< std::string, uint32_t>		mVariables;
		FunctorType								mNextName;
	};
}