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
		std::vector<uint32_t> arguments;
		Machine::Instruction root(Machine::Instruction(Machine::Opcode::ePut_structure, GetName(term.mFunctor), term.mArguments.size(), reg));
		reg++;
		for (auto& term : term.mArguments)
		{
			if (!term.IsVariable())
			{
				arguments.push_back(reg);
				Compile(term, instructions, reg);
			}
		}

		uint32_t index = 0;
		instructions.push_back(root);
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
				instructions.push_back(Machine::Instruction(Machine::Opcode::eSet_Value, arguments[index]));
				index++;
			}
		}
	}

	void QueryTestHelper( ParseTerm& term, std::string disassembly )
	{
		std::stringstream					ss;
		Machine								machine;
		QueryCompiler						compiler;
		std::vector<Machine::Instruction>	code;

		compiler.Compile(term, code);
		machine.Disassemble(&code[0], code.size(), ss);
		{
			std::stringstream termstring;
			term.Serialize(termstring);
			std::cout << "term: " << termstring.str() << std::endl;
		}

		std::cout << "generated:\n\n" << ss.str() << "\ntarget:\n\n" << disassembly << std::endl;
		
		{
			machine.Execute(&code[0]);
			std::stringstream heapdump;
			machine.DumpHeap(heapdump);
			std::cout << "heap:\n\n" << heapdump.str() << std::endl;
			assert(machine.CheckHeap());
		}
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
							   "set_variable\t\tx1\n"
				               "proceed\n");
		}

		{
			ParseTerm t("f", ParseTerm("x"));
			QueryTestHelper(t, "put_structure\t\t1/0 x1\n"
							   "put_structure\t\t0/1 x0\n"
							   "set_value\t\tx1\n"
							   "proceed\n");
		}

		{
			ParseTerm t("p", ParseTerm("Z"), 
							 ParseTerm( "h", ParseTerm("Z"), ParseTerm("W")), 
							 ParseTerm("f", ParseTerm("W")) );

			QueryTestHelper(t, "put_structure\t\t1/2 x1\n"
							   "set_variable\t\tx2\n"
							   "set_variable\t\tx3\n"
							   "put_structure\t\t2/1 x4\n"
							   "set_value\t\tx3\n"
							   "put_structure\t\t0/3 x0\n"
							   "set_value\t\tx2\n"
							   "set_value\t\tx1\n"
							   "set_value\t\tx4\n"
							   "proceed\n");
		}

	}

	void TestQuery()
	{
		query_compiles_to_code();
	}
}