#ifndef __NANY_AST_WHILEEXPRESSIONNODE_H__
# define __NANY_AST_WHILEEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a while expression.
	**
	** \code
	** 'while' <expr> 'do' <expr>
	** \endcode
	*/
	class WhileExpressionNode: public Node
	{
	public:
		WhileExpressionNode(Node* condition, Node* expr):
			pCondition(condition),
			pExpr(expr)
		{}

		virtual ~WhileExpressionNode()
		{
			delete pCondition;
			delete pExpr;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* condition() const { return pCondition; }
		Node* expression() const { return pExpr; }

	public:
		Node* pCondition;
		Node* pExpr;

	}; // class WhileExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_WHILEEXPRESSIONNODE_H__
