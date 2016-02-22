#include "stdafx.h"
#include "Compiler.h"

namespace Toy {

	FunctorType Compiler::GetName(const std::string& name)
	{
		if (mNameTable.find(name) == mNameTable.end())
		{
			mNameTable[name] = mNextName;
			mNextName++;
		}

		return mNameTable[name];
	
	}
}

