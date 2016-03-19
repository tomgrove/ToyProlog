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
			, mMode( eRead )
			, mFail( false )
			, mS( nullptr )
			, mP( nullptr )
		{}

		Term* AllocVariable();
		Term* AllocStructure(FunctorType functor, uint32_t arity);
		
		static const uint32_t NumTempRegisters = 256;
		static const uint32_t MaxHeapSize	   = 1024 * 1024;

		typedef enum {
			eRead,
			eWrite
		} Mode;

		typedef enum {
			eNop,
			ePut_Structure,
			eSet_Variable,
			eSet_Value,
			eGet_Structure,
			eUnify_Variable,
			eUnify_Value,
			eCall,
			eProceed
		} Opcode;

		struct Instruction
		{
			Opcode	 mOp;
			union {
				uint32_t mArgs[3];
				Instruction* mTarget;
			};

			Instruction()
				: mOp(eNop)
			{}

			Instruction(Instruction* target)
				: mOp(eCall)
				, mTarget(target)
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

		Term*		mH;
		uint32_t	mHeapIndex;

		Term			mXs[ NumTempRegisters ];
		Term*			mS;
		Instruction*	mP;
		Mode			mMode;
		bool			mFail;

		Term* DeRef(Term* term);
		void  Bind(Term* term, Term* ref);
		void  Unify(Term* t0, Term* t1);


		// opcodes
		void put_structure(FunctorType functor, uint32_t arity, uint32_t reg);
		void set_variable(uint32_t reg );
		void set_value(uint32_t reg);
		void get_structure(FunctorType functor, uint32_t arity, uint32_t reg);
		void unify_variable(uint32_t reg);
		void unify_value(uint32_t reg);

		void Execute(Instruction* instructions);
		void Disassemble(Instruction* instructions, uint32_t count, std::stringstream& ss);
		void DumpHeap(std::stringstream& ss);
		void SerializeTerm(Term* term, std::stringstream& ss);
		bool CheckHeap();
	private:
		Term* AllocCells( uint32_t numCells );
	};

	__declspec(dllexport) void TestMachine();
}