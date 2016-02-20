#pragma once

namespace Toy {

	typedef enum TermType {
		eNull,
		eVariableRef,
		eStructureRef,
		eStructure
	};

	typedef uint32_t FunctorType;
	typedef uint32_t VarType;

	struct StructType
	{
		FunctorType	mFunctor;
		uint32_t    mArity;
	};

	struct Term {
		TermType mType;
		union {
			VarType	   mReference;
			StructType mStructure;
		};
	};
}