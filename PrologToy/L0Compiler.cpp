#include "stdafx.h"
#include "L0Compiler.h"
#include "Query.h"

namespace Toy {

	void L0Compiler::Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg, uint32_t root )
	{
		std::vector<uint32_t> children;
		instructions.push_back( Machine::Instruction(Machine::Opcode::eGet_Structure, mNameTable.GetName(term.mFunctor), term.mArguments.size(), root ));
		for (auto& argument : term.mArguments)
		{
			if (argument.IsVariable())
			{
				if (mVariables.find(argument.mFunctor) != mVariables.end())
				{
					instructions.push_back(Machine::Instruction(Machine::Opcode::eUnify_Value, mVariables[argument.mFunctor]));
				}
				else
				{
					instructions.push_back(Machine::Instruction(Machine::Opcode::eUnify_Variable, reg));
					mVariables[argument.mFunctor] = reg;
					reg++;
				}
			}
			else
			{
				instructions.push_back(Machine::Instruction(Machine::Opcode::eUnify_Variable, reg));
				children.push_back(reg);
				reg++;
			}
		}

		uint32_t index = 0;

		for (auto& argument : term.mArguments)
		{
			if (!argument.IsVariable())
			{
				Compile(argument, instructions, reg, children[index]);
				index++;
			}
		}
	}

	void L0Compiler::Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions)
	{
		uint32_t reg = 1;
		Compile(term, instructions, reg, 0);
		instructions.push_back(Machine::Instruction(Machine::Opcode::eProceed));
	}

	void TestL0Compiler()
	{
		Nametable nametable;
		L0Compiler compiler( nametable );

		ParseTerm t("p", ParseTerm("f", ParseTerm("X")),
			ParseTerm("h", ParseTerm("Y"), ParseTerm("f", ParseTerm("a"))),
			ParseTerm("Y"));
		
		std::vector<Machine::Instruction> instructions;
		compiler.Compile(t, instructions);


		std::stringstream ss;
		t.Serialize(ss);
		ss << std::endl;

		Machine machine;

		machine.Disassemble(&instructions[0], instructions.size(), ss);

		std::cout << ss.str() << std::endl;

		std::stringstream hs;
		ParseTerm q("p", ParseTerm("X"), ParseTerm("Y"), ParseTerm("Z"));
		QueryCompiler qc(nametable);
		std::vector<Machine::Instruction> qinstr;
		qc.Compile(q, qinstr);
		machine.Execute(&qinstr[0]);
		Term* qroot = &machine.mXs[0];
		machine.DumpHeap(hs);
		std::cout << hs.str() << std::endl << std::endl;
		machine.Execute(&instructions[0]);
		machine.DumpHeap(hs);
		machine.SerializeTerm(qroot, hs);
		std::cout << hs.str() << std::endl << std::endl;

	}
}