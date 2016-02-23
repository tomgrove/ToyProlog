#include "stdafx.h"
#include "Names.h"

namespace Toy {

	FunctorType Nametable::GetName(const std::string& name) 
	{
		if (mNameTable.find(name) == mNameTable.end())
		{
			mNameTable[name] = mNextName;
			mNextName++;
		}

		return mNameTable[name];

	}
}

