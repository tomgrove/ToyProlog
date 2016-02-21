#include "stdafx.h"
#include "machine.h"
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