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

	std::string SerializeHelper(ParseTerm& t)
	{
		std::stringstream ss;
		t.Serialize(ss);
		return ss.str();
	}

	void structures_serialize_to_string()
	{
		{
			ParseTerm t("f");
			assert(SerializeHelper(t) == "f");
		}

		{
			ParseTerm t("f", ParseTerm("a0"));
			assert(SerializeHelper(t) == "f(a0)");
		}

		{
			ParseTerm t("f", ParseTerm("a0"), ParseTerm("a1"));
			assert(SerializeHelper(t) == "f(a0,a1)");
		}
	}

	void TestParser()
	{
		structures_serialize_to_string();
	}
}