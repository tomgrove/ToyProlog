#pragma once

#include "compiler.h"

namespace Toy {

	class L0QueryCompiler : public Compiler
	{
		static FunctionTable mDummyTable;
	public:
		L0QueryCompiler(Nametable& nametable)
			: Compiler(nametable, mDummyTable)
		{}
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions ) override;
	private:
		void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg);
	};

	__declspec(dllexport) void TestQuery();
}