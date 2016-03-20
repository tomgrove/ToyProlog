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

		TEST_METHOD(whitespace_parser_strips_whitespace)
		{
			Parser parser;
			char*  rest;
			parser.ParseWhitespace("\t\v\fnonwhitespace", &rest);
			Assert::AreEqual(rest, "nonwhitespace", L"message", LINE_INFO());
		}

		TEST_METHOD(functor_ids_are_unique_for_different_strings)
		{
			Parser parser;
			FunctorType f0 = parser.GetOrAddName("f");
			FunctorType f1 = parser.GetOrAddName("p");
			
			Assert::AreNotEqual(f0, f1, L"message", LINE_INFO());
		}

		TEST_METHOD(functor_ids_are_same_for_same_string)
		{
			Parser parser;
			FunctorType f0 = parser.GetOrAddName("f");
			FunctorType f1 = parser.GetOrAddName("f");

			Assert::AreEqual(f0, f1, L"message", LINE_INFO());
		}

		TEST_METHOD(can_parse_a_valid_alphanum_functor_name)
		{
			Parser parser;
			char* rest;
			auto functor = parser.ParseFunctor("functorName10", &rest);
			Assert::IsTrue(functor.IsSet(), L"message", LINE_INFO());
		}

		TEST_METHOD(functor_name_starting_with_upper_is_unset)
		{
			Parser parser;
			char* rest;
			auto functor = parser.ParseFunctor("Functor", &rest);
			Assert::IsFalse(functor.IsSet(), L"message", LINE_INFO());
		}

		TEST_METHOD(functor_names_can_only_contain_alphanumeric)
		{
			Parser parser;
			char* rest;
			auto functor = parser.ParseFunctor("functor_%", &rest);
			Assert::AreEqual(*rest, '_', L"message", LINE_INFO());
		}

		TEST_METHOD(can_parse_a_valid_alphanum_variable_name)
		{
			Parser parser;
			char* rest;
			auto functor = parser.ParseVariable("Variable10", &rest);
			Assert::IsTrue(functor.IsSet(), L"message", LINE_INFO());
		}

		TEST_METHOD(variable_name_starting_with_lower_is_unset)
		{
			Parser parser;
			char* rest;
			auto functor = parser.ParseVariable("variable", &rest);
			Assert::IsFalse(functor.IsSet(), L"message", LINE_INFO());
		}
	};
}