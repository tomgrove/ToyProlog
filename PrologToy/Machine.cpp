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
		assert( term->IsReference() );

		if (term->mType == eVariableRef &&
			term->mReference != term)
		{
			return DeRef(term->mReference);
		}

		return term;
	}

	void Machine::Bind(Term* term, Term* ref)
	{
		assert( term->IsReference()) ;

		term->mReference = ref;
	}

	void Machine::Unify(Term* t0, Term* t1)
	{
		std::vector<Term*> pdl;
		pdl.push_back(t0);
		pdl.push_back(t1);
		mFail = false;
		while(!(pdl.empty() || mFail))
		{ 
			Term* d0 = DeRef( pdl[ pdl.size() - 1] );
			Term* d1 = DeRef( pdl[ pdl.size() - 2] );
			pdl.pop_back();
			pdl.pop_back();
			if (d0 != d1)
			{
				if (d0->mType == eVariableRef ||
					d1->mType == eVariableRef)
				{
					Bind(d0, d1);
				}
				else
				{
					if (d0->mReference->mStructure.mFunctor == d1->mReference->mStructure.mFunctor &&
						d0->mReference->mStructure.mArity == d1->mReference->mStructure.mArity)
					{
						for (uint32_t i = 0; i < d0->mReference->mStructure.mArity; i++)
						{
							pdl.push_back(d0 + i);
							pdl.push_back(d1 + i);
						}
					}
					else
					{
						mFail = true;
					}
				}
			}
		}
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

	void Machine::get_structure(uint32_t functor, uint32_t arity, uint32_t reg)
	{
		Term* t = DeRef(&mXs[reg]);
		switch (t->mType)
		{
			case eVariableRef:
			{
				auto str = AllocStructure(functor, arity);
				Bind(t, str);
				mMode = eWrite;
			}
			break;
			case eStructureRef:
				if (t->mReference->mStructure.mArity == arity &&
					t->mReference->mStructure.mFunctor == functor)
				{
					mS = t->mReference + 1;
					mMode = eRead;
				}
				else
				{
					mFail = true;
				}
				break;
			default:
				mFail = true;
		}
	}

	void Machine::unify_variable(uint32_t reg)
	{
		switch (mMode)
		{
		case eRead:
			mXs[reg] = *mS;
			break;
		case eWrite:
			set_variable(reg);
			break;
		}
		mS++;
	}

	void Machine::unify_value(uint32_t reg)
	{
		switch (mMode)
		{
		case eRead:
			Unify(&mXs[reg], mS);
			break;
		case eWrite:
			set_value(reg);
			break;
		}
		mS++;
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
				case eGet_Structure:
					get_structure(instr->mArgs[0], instr->mArgs[1], instr->mArgs[2]);
					break;
				case eUnify_Variable:
					unify_variable(instr->mArgs[0]);
					break;
				case eUnify_Value:
					unify_value(instr->mArgs[0]);
					break;
				case eCall:
					mP = instr->mTarget;
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
				case eCall:
					ss << "call\t\t" << instr.mTarget << std::endl;
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


	void Machine::SerializeTerm(Term* term, std::stringstream& ss)
	{
		switch ( term->mType ) 
		{
		case eStructureRef:
			SerializeTerm(term->mReference, ss);
			break;
		case eVariableRef:
			if (term->IsUnassignedVariable())
			{
				ss << "X_" << (term->mReference - mH);
			}
			else
			{
				SerializeTerm(term->mReference, ss);
			}
			break;
		case eStructure:
			ss << "f_" << term->mStructure.mFunctor;
			if (term->mStructure.mArity > 0)
			{
				ss << "(";
				for (uint32_t i = 0; i < term->mStructure.mArity; i++)
				{
					SerializeTerm(term + 1 + i, ss);
					if (i < (term->mStructure.mArity - 1))
					{
						ss << ", ";
					}
				}
				ss << ")";
			}
			break;
		default:
			assert(0);
		}
	}
};