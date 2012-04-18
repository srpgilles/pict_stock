#ifndef __NANY_AST_TYPEOFEXPRESSIONNODE_H__
# define __NANY_AST_TYPEOFEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a typeof expression
	**
	** \code
	** typeof expr
	** \endcode
	*/
	class TypeofExpressionNode: public Node
	{
	public:
		TypeofExpressionNode(Node* expr): Node(expr->isConst()), pExpr(expr)
		{}

		virtual ~TypeofExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* expression() const { return pExpr; }

	public:
		Node* pExpr;

	}; // class TypeofExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_TYPEOFEXPRESSIONNODE_H__
