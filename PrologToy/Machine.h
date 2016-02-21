#pragma once

#include <vector>
#include "term.h"

namespace Toy {
	class Machine
	{
	public:
		Machine() 
			: mHeapIndex(0)
			, mH(new Term[ MaxHeapSize ])
		{}

		Term* AllocVariable();
		Term* AllocStructure(FunctorType functor, uint32_t arity);
		
		static const uint32_t NumTempRegisters = 256;
		static const uint32_t MaxHeapSize	   = 1024 * 1024;

		Term		mXs[ NumTempRegisters ];
		Term*		mH;
		uint32_t	mHeapIndex;

		// opcodes
		void put_structure(FunctorType functor, uint32_t arity, uint32_t reg);
		void set_variable(uint32_t reg );
		void set_value(uint32_t reg);
	private:
		Term* AllocCells( uint32_t numCells );
	};

	void TestMachine();
}