#include "stdafx.h"
#include "L0Compiler.h"

namespace Toy {

	void L0Compiler::Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg )
	{
		Machine::Instruction root(Machine::Instruction(Machine::Opcode::ePut_Structure, GetName(term.mFunctor), term.mArguments.size(), reg));
		reg++;
		for (auto& argument : term.mArguments)
		{
			if (argument.IsVariable())
			{
				if (mVariables.find(term.mFunctor) != mVariables.end())
				{
					instructions.push_back(Machine::Instruction(Machine::Opcode::eUnify_Value, mVariables[term.mFunctor]));
				}
				else
				{
					instructions.push_back(Machine::Instruction(Machine::Opcode::eUnify_Variable, reg));
					mVariables[term.mFunctor] = reg;
					reg++;
				}
			}
			else
			{
				instructions.push_back(Machine::Instruction(Machine::Opcode::eUnify_Value, reg));
				reg++;
			}
		}

		for (auto& argument : term.mArguments)
		{
			if (!argument.IsVariable())
			{
				Compile(argument, instructions, reg);
			}
		}
	}

	void L0Compiler::Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions)
	{
		uint32_t reg = 0;
		Compile(term, instructions, reg);
		instructions.push_back(Machine::Instruction(Machine::Opcode::eProceed));
	}

}