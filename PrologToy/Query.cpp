#include "stdafx.h"
#include <assert.h>
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

	void QueryCompiler::Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions)
	{
		uint32_t reg = 0;
		Compile(term, instructions, reg);
		instructions.push_back(Machine::Instruction(Machine::Opcode::eProceed));
	}

	void QueryCompiler::Compile(ParseTerm& term, std::vector<Machine::Instruction>& instructions, uint32_t& reg)
	{
		uint32_t parent = reg;
		reg++;
		for (auto& term : term.mArguments)
		{
			if (term.IsVariable())
			{
				if (mVariables.find(term.mFunctor) != mVariables.end())
				{
					instructions.push_back(Machine::Instruction(Machine::Opcode::eSet_Value, mVariables[term.mFunctor]));
				}
				else
				{
					instructions.push_back(Machine::Instruction(Machine::Opcode::eSet_Variable, reg));
					mVariables[term.mFunctor] = reg;
					reg++;
				}
			}
			else
			{
				Compile(term, instructions, reg);
			}
		}
		instructions.push_back(Machine::Instruction(Machine::Opcode::ePut_structure, GetName( term.mFunctor ), term.mArguments.size(), parent ));
	}

	void QueryTestHelper( ParseTerm& term, std::string disassembly )
	{
		std::stringstream					ss;
		Machine								machine;
		QueryCompiler						compiler;
		std::vector<Machine::Instruction>	code;

		compiler.Compile(term, code);
		machine.Disassemble(&code[0], code.size(), ss);
		std::cout << "generated:\n\n" << ss.str() << "\ntarget:\n\n" << disassembly << std::endl;
		assert(ss.str() == disassembly);
	}

	void query_compiles_to_code()
	{
		{
			ParseTerm t("f");
			QueryTestHelper( t, "put_structure\t\t0/0 x0\n"
				                "proceed\n");
		}

		{
			ParseTerm t("f", ParseTerm("X"));
			QueryTestHelper(t, "put_structure\t\t0/1 x0\n"
							   "set_variable\t\t x1\n"
				               "proceed\n");
		}

		{
			ParseTerm t("f", ParseTerm("x"));
			QueryTestHelper(t, "put_structure\t\t0/0 x1\n"
							   "put_structure\t\t1/1 x0\n"
							   "set_value\t\t x1\n"
							   "proceed\n");
		}

		{
			ParseTerm t("f", ParseTerm("Z"), 
							 ParseTerm( "h", ParseTerm("Z"), ParseTerm("W")), 
							 ParseTerm("f", ParseTerm("W")) );

			QueryTestHelper(t, "put_structure\t\t0/0 x1\n"
				"put_structure\t\t1/1 x0\n"
				"set_value\t\t x1\n"
				"proceed\n");
		}

	}

	void TestQuery()
	{
		query_compiles_to_code();
	}
}