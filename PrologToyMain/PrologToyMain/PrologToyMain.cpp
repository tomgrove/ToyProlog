// PrologToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CppUnitTest.h>
#include "..\..\PrologToy\term.h"
#include "..\..\PrologToy\L0Compiler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


int _tmain(int argc, _TCHAR* argv[])
{
	Toy::TestL0Compiler();
	return 0;
}