#include "stdafx.h"
#include "parser.h"

namespace Toy {
	static void QueryTestHelper(ParseTerm& term, std::string disassembly)
	{

	}

	void test_compiles()
	{
		QueryTestHelper(
			ParseTerm("p", ParseTerm("h", ParseTerm("X"), ParseTerm("Y")), ParseTerm("f", ParseTerm("Z"))),
			"set_variable\t\tx3\n"
			"set_value\t\tx5\n");
	}

	void TestL1QueryCompiler()
	{
		test_compiles();
	}
}