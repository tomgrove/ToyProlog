#include "stdafx.h"
#include "machine.h"
#include <assert.h>

namespace Toy {

	Term* Machine::AllocVariable()
	{
		Term*  term = new Term();
		term->mType = eVariableRef;
		term->mReference = term;
		return term;
	}

	Term* Machine::AllocateStruct(FunctorType functor, uint32_t arity )
	{
		Term* term = new Term[ arity + 1];
		term[0].mType					= eStructure;
		term[0].mStructure.mArity		= arity;
		term[0].mStructure.mFunctor		= functor;

		Term* ref = new Term();
		ref->mType		= eStructureRef;
		ref->mReference   = term;

		return ref;
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


	void TestMachine()
	{
		MachineFixture fixture;
		new_variable_is_unassigned(fixture);
	}
};