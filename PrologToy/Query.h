#pragma once

#include "compiler.h"

namespace Toy {

	class QueryCompiler : public Compiler
	{
	public:
		QueryCompiler(Nametable& nametable)
			: Compiler(nametable)
		{}
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions ) override;
	private:
		void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg);
	};

	void TestQuery();
}