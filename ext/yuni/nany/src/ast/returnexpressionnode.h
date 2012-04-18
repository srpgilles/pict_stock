#ifndef __NANY_AST_RETURNEXPRESSIONNODE_H__
# define __NANY_AST_RETURNEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a return expression
	**
	** \code
	** return expr
	** \endcode
	*/
	class ReturnExpressionNode: public Node
	{
	public:
		ReturnExpressionNode(Node* expr): Node(expr->isConst()), pExpr(expr)
		{}

		virtual ~ReturnExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* expression() const { return pExpr; }

	public:
		Node* pExpr;

	}; // class ReturnExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_RETURNEXPRESSIONNODE_H__
