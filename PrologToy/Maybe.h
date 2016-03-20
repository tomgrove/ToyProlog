#pragma once

namespace Toy {
	template<typename T>
	class Maybe
	{
	public:
		Maybe()
			: mIsSet(false)
		{}
		explicit Maybe(T t)
			: mValue(t)
			, mIsSet(true)
		{}

		void Set(T& t)
		{
			mValue = t;
			mIsSet = true;
		}

		T& Get()
		{
			return mValue;
		}

		bool IsSet() const
		{
			return  mIsSet;
		}

	private:
		T	 mValue;
		bool mIsSet;
	};
}