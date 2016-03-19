#include "stdafx.h"

#include <CppUnitTest.h>
#include <string>
#include "..\Machine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Toy::TermType>(const Toy::TermType& t) { return Toy::ToString(t); }
		}
	}
}

namespace Toy
{
	TEST_CLASS(MachineTests)
	{
	public:
		TEST_METHOD(new_variable_is_unassigned) 
		{
			Machine machine;
			auto term = machine.AllocVariable();
			Assert::IsTrue(term->IsUnassignedVariable(), L"message", LINE_INFO());
		}

		TEST_METHOD( new_structure_is_reachable_from_ref) 
		{
			Machine machine;
			auto term = machine.AllocStructure(12345, 3);
			Assert::AreEqual(term->mType, eStructureRef, L"message", LINE_INFO());
			Assert::AreEqual(term->mReference->mType, eStructure, L"message", LINE_INFO());
			Assert::AreEqual(term->mReference->mStructure.mArity, (uint32_t)3, L"message", LINE_INFO());
			Assert::AreEqual(term->mReference->mStructure.mFunctor, FunctorType(12345), L"message", LINE_INFO());
		}

		TEST_METHOD(bound_variable_deferences)
		{
			Machine machine;
			auto term = machine.AllocVariable();
			auto atom = machine.AllocStructure(12345, 0);
			machine.Bind(term, atom);
			Term* dr = machine.DeRef(term);
			Assert::AreEqual(dr->mType, eStructureRef, L"message", LINE_INFO());
			Assert::AreEqual(dr->mReference->mStructure.mFunctor,FunctorType(12345), L"message", LINE_INFO());
		}
	};
}