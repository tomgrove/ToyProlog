#pragma once

namespace Toy {

	struct Term;

	typedef enum TermType {
		eNull,
		eVariableRef,
		eStructureRef,
		eStructure
	};

	typedef uint32_t FunctorType;
	typedef Term*    VarType;

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

		Term() : mType(eNull)
		{}

		bool IsUnassignedVariable() const
		{
			return mType == eVariableRef && mReference == this;
		}
	};
}