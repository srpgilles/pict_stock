#ifndef __NANY_AST_CLASSDECLARATIONNODE_H__
# define __NANY_AST_CLASSDECLARATIONNODE_H__

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
	** \brief Class declaration
	**
	** \code
	** class Identifier { <Declarations> }
	** \endcode
	*/
	class ClassDeclarationNode: public Node
	{
	public:
		template<class StringT>
		ClassDeclarationNode(const StringT& name, DeclarationListNode* decls):
			pClassName(name),
			pDeclarations(decls)
		{}

		virtual ~ClassDeclarationNode()
		{
			delete pDeclarations;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& name() const { return pClassName; }

		DeclarationListNode* declarations() const { return pDeclarations; }

	private:
		//! Identifier representing the name of the unit or program
		Yuni::String pClassName;

		//! The body of the class contains declarations
		DeclarationListNode* pDeclarations;

	}; // class ClassDeclarationNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_CLASSDECLARATIONNODE_H__
