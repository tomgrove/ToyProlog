#pragma once

#include <map>
#include "term.h"
#include "parser.h"
#include "machine.h"

namespace Toy {

	class QueryCompiler
	{
	public:
		void Compile(ParseTerm& term, Machine& machine, uint32_t& reg );
	private:
		FunctorType GetName(const std::string& name);
		std::map < std::string, FunctorType > mNameTable;
		std::map< std::string, uint32_t> mVariables;
		FunctorType mNextName;
	};
}