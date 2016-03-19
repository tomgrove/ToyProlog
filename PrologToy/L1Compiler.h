#pragma once

#pragma once

#include "Compiler.h"

namespace Toy {
	class L1Compiler : public Compiler
	{
	public:
		L1Compiler(Nametable& nametable)
			: Compiler(nametable)
		{}
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions) override;
	private:
		//void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg, uint32_t root);
		std::map< uint32_t, Machine::Instruction*> mCodeTable;
	};

	__declspec(dllexport) void TestL1Compiler();
}