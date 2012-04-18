#ifndef __NANY_AST_NEWEXPRESSIONNODE_H__
# define __NANY_AST_NEWEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a new expression
	**
	** \code
	** new expr
	** \endcode
	*/
	class NewExpressionNode: public Node
	{
	public:
		NewExpressionNode(Node* expr): pExpr(expr)
		{}

		virtual ~NewExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* expression() const { return pExpr; }

	public:
		Node* pExpr;

	}; // class NewExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_NEWEXPRESSIONNODE_H__
