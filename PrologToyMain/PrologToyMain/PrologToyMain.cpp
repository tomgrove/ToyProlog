// PrologToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CppUnitTest.h>
#include "..\..\PrologToy\term.h"
#include "..\..\PrologToy\machine.h"
#include "..\..\PrologToy\parser.h"
#include "..\..\PrologToy\L0QueryCompiler.h"
#include "..\..\PrologToy\L0Compiler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


int _tmain(int argc, _TCHAR* argv[])
{
	Toy::TestMachine();
	Toy::TestParser();
	Toy::TestQuery();
	Toy::TestL0Compiler();

	return 0;
}