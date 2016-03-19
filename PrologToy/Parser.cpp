#include "stdafx.h"
#include "Parser.h"
#include <assert.h>

namespace Toy {

	void ParseTerm::Serialize(std::stringstream& ss)
	{
		ss << mFunctor;
		if (mArguments.size())
		{
			ss << "(";
			for ( uint32_t i = 0; i < mArguments.size(); i++)
			{
				mArguments[i].Serialize(ss);
				if ( i < mArguments.size()-1 )
				{
					ss << ",";
				}
			}
			ss << ")";
		}
	}
}