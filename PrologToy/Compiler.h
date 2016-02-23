#pragma once

#include <vector>
#include <map>
#include "parser.h"
#include "Machine.h"
#include "Names.h"

namespace Toy {

	class Compiler
	{
	public:
		Compiler(Nametable& nametable )
			: mNameTable( nametable )
		{}
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions) = 0;
	protected:
		std::map< std::string, uint32_t>		mVariables;
		Nametable&								mNameTable;
	};
}