#ifndef __NANY_AST_VARDECLARATIONNODE_H__
# define __NANY_AST_VARDECLARATIONNODE_H__

# include "yuni/core/string.h"
# include "node.h"
# include "typeexpressionnode.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a local variable declaration
	**
	** \code
	** <expr> : <expr>
	** \endcode
	*/
	class VarDeclarationNode: public Node
	{
	public:
		VarDeclarationNode(Node* left, TypeExpressionNode* type):
			pLeft(left),
			pType(type)
		{}

		virtual ~VarDeclarationNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* left() const { return pLeft; }
		TypeExpressionNode* typeDecl() const { return pType; }

	protected:
		//! Identifier
		Node* pLeft;
		//! Type
		TypeExpressionNode* pType;

	}; // class VarDeclarationNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_VARDECLARATIONNODE_H__
