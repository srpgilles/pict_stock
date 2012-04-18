#ifndef __NANY_AST_UNITDECLARATIONNODE_H__
# define __NANY_AST_UNITDECLARATIONNODE_H__

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
	** \brief Unit or program declaration.
	*/
	class UnitDeclarationNode: public Node
	{
	public:
		template<class StringT>
		UnitDeclarationNode(bool isProgram, const StringT& name):
			pIsProgram(isProgram),
			pUnitName(name)
		{}

		virtual ~UnitDeclarationNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& name() { return pUnitName; }

	private:
		//! Is it a program declaration (true) or a unit declaration (false) ?
		bool pIsProgram;
		//! Identifier representing the name of the unit or program
		Yuni::String pUnitName;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_UNITDECLARATIONNODE_H__
