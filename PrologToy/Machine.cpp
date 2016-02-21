#include "stdafx.h"
#include "machine.h"
#include <iostream>
#include <sstream>
#include <assert.h>

namespace Toy {

	Term* Machine::AllocCells( uint32_t numCells )
	{
		assert(mHeapIndex < (MaxHeapSize-numCells) );
		mHeapIndex += numCells;
		return &mH[mHeapIndex - numCells ];
	}

	Term* Machine::AllocVariable()
	{
		Term*  term = AllocCells(1);
		term->mType = eVariableRef;
		term->mReference = term;
		return term;
	}

	Term* Machine::AllocStructure(FunctorType functor, uint32_t arity )
	{
		Term* term = AllocCells( 2 );
		term[1].mType					= eStructure;
		term[1].mStructure.mArity		= arity;
		term[1].mStructure.mFunctor		= functor;

		term[0].mType					= eStructureRef;
		term[0].mReference				= &term[1];

		return &term[0];
	}
	
	void Machine::put_structure(FunctorType functor, uint32_t arity, uint32_t reg)
	{
		mXs[reg] = *AllocStructure(functor, arity);
	}

	void Machine::set_variable(uint32_t reg)
	{
		mXs[reg] = *AllocVariable();
	}

	void Machine::set_value(uint32_t reg)
	{
		auto term = AllocVariable();
		*term = mXs[reg];
	}

	void Machine::Execute(Instruction* instructions)
	{
		Instruction* instr = instructions;
		while (instr->mOp != eProceed)
		{
			switch (instr->mOp)
			{
				case ePut_structure:
					put_structure(instr->mArgs[0], instr->mArgs[1], instr->mArgs[1]);
					break;
				case eSet_Value:
					set_value(instr->mArgs[0]);
					break;
				case eSet_Variable:
					set_variable(instr->mArgs[0]);
					break;
				case eNop:
					break;
				default:
					assert(0);
			}
			instr++;
		}
	}

	void Machine::Disassemble(Instruction* instructions, uint32_t count, std::stringstream& ss)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			auto& instr = instructions[i];
			switch (instr.mOp)
			{
				case eNop:
					ss << "nop" << std::endl;
					break;
				case ePut_structure:
					ss << "put_structure\t\t" << instr.mArgs[0] << "/" << instr.mArgs[1] << " x" << instr.mArgs[2] << std::endl;
					break;
				case eSet_Value:
					ss << "set_value\t\tx" << instr.mArgs[0] << std::endl;
					break;
				case eSet_Variable:
					ss << "set_variable\t\tx" << instr.mArgs[0] << std::endl;
					break;
				case eProceed:
					ss << "proceed" << std::endl;
					break;
				default:
					ss << "illegal instruction: " << instr.mOp << std::endl;
			}
		}
	}

	struct MachineFixture
	{
		Machine machine;
	};

	void new_variable_is_unassigned(MachineFixture& fixture )
	{
		auto term = fixture.machine.AllocVariable();
		assert(term->IsUnassignedVariable() );
	}

	void new_structure_is_reachable_from_ref(MachineFixture& fixture)
	{
		auto term = fixture.machine.AllocStructure(12345, 3);
		assert(term->mType == eStructureRef);
		assert(term->mReference->mType == eStructure);
		assert(term->mReference->mStructure.mArity == 3);
		assert(term->mReference->mStructure.mFunctor == 12345);
	}

	void TestMachine()
	{
		MachineFixture fixture;
		new_variable_is_unassigned(fixture);
		new_structure_is_reachable_from_ref(fixture);
	}
};