#ifndef __NANY_AST_INFERIOREXPRESSIONNODE_H__
# define __NANY_AST_INFERIOREXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a strict inferior binary operation
	**
	** \code
	** <expr> '<' <expr>
	** \endcode
	*/
	class InferiorExpressionNode: public Node
	{
	public:
		InferiorExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~InferiorExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* left() const { return pLeft; }
		Node* right() const { return pRight; }

	public:
		Node* pLeft;
		Node* pRight;

	}; // class InferiorExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_INFERIOREXPRESSIONNODE_H__
