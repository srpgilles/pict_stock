#ifndef __NANY_AST_MULTIPLYEXPRESSIONNODE_H__
# define __NANY_AST_MULTIPLYEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a multiplication binary operation
	**
	** \code
	** <expr> '*' <expr>
	** \endcode
	*/
	class MultiplyExpressionNode: public Node
	{
	public:
		MultiplyExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~MultiplyExpressionNode()
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

#endif // __NANY_AST_MULTIPLYEXPRESSIONNODE_H__
