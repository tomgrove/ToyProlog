// PrologToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "term.h"
#include "machine.h"
#include "parser.h"
#include "query.h"
#include "L0Compiler.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Toy::TestMachine();
	Toy::TestParser();
	Toy::TestQuery();
	Toy::TestL0Compiler();

	return 0;
}


