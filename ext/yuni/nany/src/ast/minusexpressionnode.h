#ifndef __NANY_AST_MINUSEXPRESSIONNODE_H__
# define __NANY_AST_MINUSEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a subtraction binary operation
	**
	** \code
	** <expr> '-' <expr>
	** \endcode
	*/
	class MinusExpressionNode: public Node
	{
	public:
		MinusExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~MinusExpressionNode()
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
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_MINUSEXPRESSIONNODE_H__
