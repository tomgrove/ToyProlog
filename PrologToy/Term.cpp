#include "stdafx.h"
#include <string>
#include <CppUnitTest.h>
#include "Term.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Toy {
	std::wstring ToString(const Toy::TermType& term)
	{
		switch (term)
		{
		case Toy::eStructureRef:
			return L"eStructureRef";
		case Toy::eStructure:
			return L"eStructure";
		case Toy::eVariableRef:
			return L"eVariableRef";
		default:
			return L"??";
		}
	}
}
