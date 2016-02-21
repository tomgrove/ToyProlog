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

		typedef enum {
			eNop,
			ePut_structure,
			eSet_Variable,
			eSet_Value,
			eProceed
		} Opcode;

		struct Instruction
		{
			Opcode	 mOp;
			uint32_t mArgs[3];
			Instruction()
				: mOp(eNop)
			{}

			Instruction(Opcode op)
				: mOp(op)
			{}

			Instruction(Opcode op, uint32_t arg0)
				: mOp(op)
			{
				mArgs[0] = arg0;
			}

			Instruction(Opcode op, uint32_t arg0, uint32_t arg1)
				: mOp(op)
			{
				mArgs[0] = arg0;
				mArgs[1] = arg1;
			}

			Instruction(Opcode op, uint32_t arg0, uint32_t arg1, uint32_t arg2)
				: mOp(op)
			{
				mArgs[0] = arg0;
				mArgs[1] = arg1;
				mArgs[2] = arg2;
			}
		};

		Term		mXs[ NumTempRegisters ];
		Term*		mH;
		uint32_t	mHeapIndex;

		// opcodes
		void put_structure(FunctorType functor, uint32_t arity, uint32_t reg);
		void set_variable(uint32_t reg );
		void set_value(uint32_t reg);

		void Execute(Instruction* instructions);
		void Disassemble(Instruction* instructions, uint32_t count, std::stringstream& ss);
	private:
		Term* AllocCells( uint32_t numCells );
	};

	void TestMachine();
}