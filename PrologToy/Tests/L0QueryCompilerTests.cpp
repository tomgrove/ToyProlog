#include "stdafx.h"

#include <CppUnitTest.h>
#include <string>
#include "..\L0QueryCompiler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Toy
{
	TEST_CLASS(L0QueryCompilerTests)
	{
		std::string QueryTestHelper(ParseTerm& term)
		{
			std::stringstream					ss;
			Machine								machine;
			Nametable							nametable;
			L0QueryCompiler						compiler(nametable);
			std::vector<Machine::Instruction>	code;

			compiler.Compile(term, code);
			machine.Disassemble(&code[0], code.size(), ss);
			{
				std::stringstream termstring;
				term.Serialize(termstring);
				//	std::cout << "term: " << termstring.str() << std::endl;
			}

			//std::cout << "generated:\n\n" << ss.str() << "\ntarget:\n\n" << disassembly << std::endl;

		{
			machine.Execute(&code[0]);
			std::stringstream heapdump;
			machine.DumpHeap(heapdump);
			//	std::cout << "heap:\n\n" << heapdump.str() << std::endl;
			//		assert(machine.CheckHeap());
		}

		return ss.str();
		}
	public:
		TEST_METHOD(f_generates_expected_code)
		{
			ParseTerm t("f");
			auto disasm = QueryTestHelper(t);
			std::string target = "put_structure\t\t0/0 x0\n"
				"proceed\n";
			Assert::AreEqual(disasm, target, L"message", LINE_INFO());
		}
		
		TEST_METHOD(fX_generates_expected_code)
		{
			ParseTerm t("f", ParseTerm("X"));
			auto disasm = QueryTestHelper(t); 
			std::string target = "put_structure\t\t0/1 x0\n"
							     "set_variable\t\tx1\n"
								 "proceed\n";
			Assert::AreEqual(disasm, target, L"message", LINE_INFO());
		}

		TEST_METHOD(fx_generates_expected_code)
		{
			ParseTerm t("f", ParseTerm("x"));
			auto disasm = QueryTestHelper(t); 
			std::string target = "put_structure\t\t1/0 x1\n"
								 "put_structure\t\t0/1 x0\n"
								 "set_value\t\tx1\n"
								 "proceed\n";
			Assert::AreEqual(disasm, target, L"message", LINE_INFO());
		}
		
		TEST_METHOD(pZ_hZW_fW_generates_expected_code)
		{
			ParseTerm t("p", 
				ParseTerm("Z"),
				ParseTerm("h", 
					ParseTerm("Z"), ParseTerm("W")),
				ParseTerm("f", 
					ParseTerm("W")));

			auto disasm = QueryTestHelper(t);
			std::string target = "put_structure\t\t1/2 x1\n"
								 "set_variable\t\tx2\n"
								 "set_variable\t\tx3\n"
								 "put_structure\t\t2/1 x4\n"
								 "set_value\t\tx3\n"
								 "put_structure\t\t0/3 x0\n"
								 "set_value\t\tx2\n"
								 "set_value\t\tx1\n"
								 "set_value\t\tx4\n"
								 "proceed\n";

			Assert::AreEqual(disasm, target, L"message", LINE_INFO());
			}
	};

}