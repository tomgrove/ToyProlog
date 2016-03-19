#pragma once

#include <vector>
#include <map>
#include "parser.h"
#include "Machine.h"
#include "Names.h"

namespace Toy {

	struct FunctionId
	{
		FunctorType		mFunctor;
		uint32_t		mArity;
		FunctionId(FunctorType functor, uint32_t arity)
			: mFunctor(functor)
			, mArity(arity)
		{}

		FunctionId()
			: mFunctor(0)
			, mArity(0)
		{}

		bool operator<(FunctionId const& l) const ;
	};

	typedef std::map< FunctionId, Machine::Instruction*> FunctionTable;

	class Compiler
	{
	public:
		Compiler(Nametable& nametable, FunctionTable& functiontable )
			: mNameTable( nametable )
			, mFunctionTable( functiontable )
		{}
		virtual void Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions) = 0;
	protected:
		std::map< std::string, uint32_t>				 mVariables;
		Nametable&										 mNameTable;
		FunctionTable&									 mFunctionTable;
	};
}