#include "stdafx.h"
#include "Query.h"

namespace Toy {

	FunctorType QueryCompiler::GetName(const std::string& name)
	{
		if (mNameTable.find(name) == mNameTable.end())
		{
			mNameTable[name] = mNextName;
			mNextName++;
		}

		return mNameTable[name];
	}

	void QueryCompiler::Compile(ParseTerm& term, Machine& machine, uint32_t& reg)
	{
		if (term.IsStructure() || term.IsConstant() )
		{
			machine.put_structure(GetName(term.mFunctor), term.mArguments.size(), reg );
			reg++;
			for (auto& term : term.mArguments)
			{
				Compile(term, machine, reg );
			}
		}
		else if (term.IsVariable())
		{
			if (mVariables.find(term.mFunctor) != mVariables.end())
			{
				machine.set_value(mVariables[term.mFunctor]);
			}
			else
			{
				machine.set_variable(reg);
				mVariables[term.mFunctor] = reg;
				reg++;
			}
		}
	}
}