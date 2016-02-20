#pragma once

#include <vector>
#include "term.h"

namespace Toy {
	class Machine
	{
		std::vector<Term>	mHeap;
	public:
		Term& AllocVariable();
		Term& AllocateStruct(FunctorType functor, uint32_t arity);
	};
}