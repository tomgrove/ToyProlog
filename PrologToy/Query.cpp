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
		if (term.IsStructure())
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
		}
	}
}