#include "stdafx.h"
#include "Parser.h"
#include <assert.h>

namespace Toy {

	void ParseTerm::Serialize(std::stringstream& ss)
	{
		ss << mFunctor;
		if (mArguments.size())
		{
			ss << "(";
			for ( uint32_t i = 0; i < mArguments.size(); i++)
			{
				mArguments[i].Serialize(ss);
				if ( i < mArguments.size()-1 )
				{
					ss << ",";
				}
			}
			ss << ")";
		}
	}

	void Parser::ParseWhitespace(char* input, char** rest)
	{
		char* cursor = input;
		while (isspace(*cursor))
		{
			cursor++;
		}
		*rest = cursor;
	}

	std::string Parser::ParseAlphaNumeric(char* input, char** rest)
	{
		char* cursor = input;
		std::string alphanums;
		while (isalnum(*cursor))
		{
			alphanums.push_back(*cursor);
			cursor++;
		}
		*rest = cursor;
		return alphanums;
	}

	FunctorType Parser::GetOrAddName(std::string name)
	{
		if (mNameTable.find(name) == mNameTable.end())
		{
			mNameTable[name] = ++mFunctorId;
		}

		return mNameTable[name];
	}

	Maybe<FunctorType> Parser::ParseFunctor(char* input, char** rest)
	{
		char* cursor = input;
		std::string name;
		if (!islower(*cursor))
		{
			return Maybe<FunctorType>();
		}
		name.push_back(*cursor);
		cursor++;
		name.append( ParseAlphaNumeric(cursor, rest));
		return Maybe<FunctorType>( GetOrAddName( name ) );
	}

	Maybe<FunctorType> Parser::ParseVariable(char* input, char** rest)
	{
		char* cursor = input;
		std::string name;
		if (!isupper(*cursor))
		{
			return Maybe<FunctorType>();
		}
		name.push_back(*cursor);
		cursor++;
		name.append(ParseAlphaNumeric(cursor, rest));
		return Maybe<FunctorType>(GetOrAddName(name));
	}

	Maybe<ParseTerm> Parser::ParseOneTerm(char* input, char** rest)
	{
		return Maybe<ParseTerm>();
	}
}