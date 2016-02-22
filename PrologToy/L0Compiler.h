#pragma once

#include "Compiler.h"

namespace Toy {
	class L0Compiler : public Compiler
	{
	public:
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions) override;
	private:
		void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg, uint32_t root );
	};

	void TestL0Compiler();
}