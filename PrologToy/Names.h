#pragma once

#include <map>
#include "Term.h"

namespace Toy {

	class Nametable
	{
	public:
		Nametable()
			: mNextName(0)
		{}
		FunctorType GetName(const std::string& name) ;
	private:
		std::map < std::string, FunctorType >	mNameTable;
		FunctorType								mNextName;
	};
}