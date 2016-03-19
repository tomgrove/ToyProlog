#include "stdafx.h"

#include <CppUnitTest.h>
#include <string>
#include "..\Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Toy
{
	TEST_CLASS(ParserTests)
	{
		std::string SerializeHelper(ParseTerm& t)
		{
			std::stringstream ss;
			t.Serialize(ss);
			return ss.str();
		}

	public:
		TEST_METHOD(term_f_serialize_to_string_f)
		{

			ParseTerm t("f");
			Assert::AreEqual(SerializeHelper(t).c_str(), "f", L"message", LINE_INFO());
		}

		TEST_METHOD(term_fa0_serialize_to_string_fa0)
		{
			ParseTerm t("f", ParseTerm("a0"));
			Assert::AreEqual(SerializeHelper(t).c_str(), "f(a0)", L"message", LINE_INFO());
		}

		TEST_METHOD(term_fa0a1_serialize_to_string_fa0a1)
		{
			ParseTerm t("f", ParseTerm("a0"), ParseTerm("a1"));
			Assert::AreEqual(SerializeHelper(t).c_str(), "f(a0,a1)", L"message", LINE_INFO());
		}
	};
}