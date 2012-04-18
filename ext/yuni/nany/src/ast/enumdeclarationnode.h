#ifndef __NANY_AST_ENUMDECLARATIONNODE_H__
# define __NANY_AST_ENUMDECLARATIONNODE_H__

# include "fwd.h"
# include "node.h"
# include "visitor.h"
# include "yuni/yuni.h"
# include "yuni/core/string.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief Enum declaration
	**
	** \code
	** enum Identifier { <Declarations> }
	** \endcode
	*/
	class EnumDeclarationNode: public Node
	{
	public:
		typedef std::map<Yuni::String, unsigned int>  EnumValueMap;

	public:
		template<class StringT>
		EnumDeclarationNode(const StringT& name, EnumValueMap* values):
			pEnumName(name),
			pValues(values)
		{}

		virtual ~EnumDeclarationNode()
		{
			delete pValues;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& name() const { return pEnumName; }

		EnumValueMap* values() const { return pValues; }

	private:
		//! Identifier representing the name of the unit or program
		Yuni::String pEnumName;

		//! The values define in the enum
		EnumValueMap* pValues;

	}; // class EnumDeclarationNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_ENUMDECLARATIONNODE_H__
