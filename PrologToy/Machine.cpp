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
	
	Term* Machine::DeRef(Term* term)
	{
		if (term->mType == eVariableRef &&
			term->mReference != term)
		{
			return DeRef(term->mReference);
		}

		return term;
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
				case ePut_Structure:
					put_structure(instr->mArgs[0], instr->mArgs[1], instr->mArgs[2]);
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
				case ePut_Structure:
					ss << "put_structure\t\t" << instr.mArgs[0] << "/" << instr.mArgs[1] << " x" << instr.mArgs[2] << std::endl;
					break;
				case eGet_Structure:
					ss << "get_structure\t\t" << instr.mArgs[0] << "/" << instr.mArgs[1] << " x" << instr.mArgs[2] << std::endl;
					break;
				case eSet_Value:
					ss << "set_value\t\tx" << instr.mArgs[0] << std::endl;
					break;
				case eSet_Variable:
					ss << "set_variable\t\tx" << instr.mArgs[0] << std::endl;
					break;
				case eUnify_Variable:
					ss << "unify_variable\t\tx" << instr.mArgs[0] << std::endl;
					break;
				case eUnify_Value:
					ss << "unify_value\t\tx" << instr.mArgs[0] << std::endl;
					break;
				case eProceed:
					ss << "proceed" << std::endl;
					break;
				default:
					ss << "illegal instruction: " << instr.mOp << std::endl;
			}
		}
	}

	bool Machine::CheckHeap()
	{
		for (uint32_t i = 0; i < mHeapIndex; i++)
		{
			auto& term = mH[i];
			switch (term.mType)
			{
				case eStructureRef:
					if (term.mReference->mType != eStructure)
					{
						return false;
					}
					break;
				case eVariableRef:
					{
						auto deref = DeRef(&term);
						if (!(deref->IsUnassignedVariable() ||
							 (deref->mType == eStructureRef) ))
						{
							return  false;
						}
					}
					break;
				case eStructure:
					break;
				default:
					return false;
			}
		}

		return true;
	}

	void Machine::DumpHeap(std::stringstream& ss)
	{
		for (uint32_t i = 0; i < mHeapIndex; i++)
		{
			auto& term = mH[i];
			ss << i << "\t";
			switch (term.mType)
			{
				case eStructure:
					ss << term.mStructure.mFunctor << "/" << term.mStructure.mArity << std::endl;
					break;
				case eStructureRef:
					ss << "STR " << (term.mReference - mH) << std::endl;
					break;
				case eVariableRef:
					ss << "REF " << (term.mReference - mH) << std::endl;
					break;
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