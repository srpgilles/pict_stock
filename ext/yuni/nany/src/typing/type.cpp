
#include "type.h"
#include <iostream>

using namespace Yuni;


namespace Nany
{
namespace Typing
{

	//! Defining the static map
	Type::Map Type::sTypeMap;

	Type* Type::Get(const Yuni::String& id)
	{
		if (sTypeMap.empty())
			RegisterBuiltinTypes();

		// Find the type
		Map::iterator it = sTypeMap.find(id);
		if (sTypeMap.end() == it)
			return nullptr;
		return it->second;
	}


	void Type::Add(const Yuni::String& id, Type* newType)
	{
		if (sTypeMap.empty())
			RegisterBuiltinTypes();

		String::Size bracket = id.find('[');
		if (bracket < id.size())
		{
			String::Size finalBracket = id.find(']', bracket + 1);
			if (finalBracket < id.size())
			{
				AnyString realType(id, 0, bracket);
				realType.trim();
				AnyString cardinality(id, bracket + 1, finalBracket - bracket - 1);
				cardinality.trim();
				std::cout << realType << ", cardinality: " << cardinality << std::endl;
				String s;
				s << "std::vector<" << realType << '>';
				Add(s, newType);
				return;
			}
		}
		Map::iterator it = sTypeMap.find(id);
		if (sTypeMap.end() != it)
		{
			delete it->second;
			it->second = newType;
		}
		else
		{
			sTypeMap[id] = newType;
		}
	}


	void Type::RegisterBuiltinTypes()
	{
		Yuni::String s;
		sTypeMap[(s = "void")] = new Type("void", true, true);
		sTypeMap[(s = "bool")] = new Type("bool", true);
		sTypeMap[(s = "int")] = new Type("int", true);
		sTypeMap[(s = "uint")] = new Type("unsigned int", true);
		sTypeMap[(s = "int64")] = new Type("long long", true);
		sTypeMap[(s = "uint64")] = new Type("unsigned long long", true);
		sTypeMap[(s = "int32")] = new Type("long", true);
		sTypeMap[(s = "uint32")] = new Type("unsigned long", true);
		sTypeMap[(s = "int16")] = new Type("short", true);
		sTypeMap[(s = "uint16")] = new Type("unsigned short", true);
		sTypeMap[(s = "float")] = new Type("float", true);
		sTypeMap[(s = "double")] = new Type("double", true);
		sTypeMap[(s = "char")] = new Type("char", true);
		sTypeMap[(s = "string")] = new Type("Yuni::String", false);
		sTypeMap[(s = "niltype")] = new Type("void*", false);
	}



} // namespace Typing
} // namespace Nany
