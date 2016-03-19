#include "stdafx.h"

#include <CppUnitTest.h>
#include "..\L0QueryCompiler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(TestClassName)
{
public:
	TEST_METHOD(TestMethodName)
	{
		// Run a function under test here.
		Assert::AreEqual(10, 10, L"message", LINE_INFO());
	}
};