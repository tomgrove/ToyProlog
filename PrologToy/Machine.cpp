#include "stdafx.h"
#include "machine.h"

namespace Toy {

	Term& Machine::AllocVariable()
	{
		Term term;
		term.mType = eVariableRef;
		term.mReference = mHeap.size();

		mHeap.push_back(term);

		return mHeap[mHeap.size() - 1];
	}

	Term& Machine::AllocateStruct(FunctorType functor, uint32_t arity )
	{
		Term term;
		term.mType					= eStructure;
		term.mStructure.mArity		= arity;
		term.mStructure.mFunctor	= functor;

		uint32_t structureindex = mHeap.size();
		mHeap.push_back(term);

		for (uint32_t i = 0; i < arity; i++)
		{
			Term null;
			null.mType = eNull;
			mHeap.push_back(null);
		}

		Term ref;
		ref.mType		= eStructureRef;
		ref.mReference   = structureindex;

		mHeap.push_back(ref);

		return mHeap[mHeap.size() - 1];
	}
	
};