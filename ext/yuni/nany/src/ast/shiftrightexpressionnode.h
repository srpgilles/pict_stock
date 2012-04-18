#ifndef __NANY_AST_SHIFTRIGHTEXPRESSIONNODE_H__
# define __NANY_AST_SHIFTRIGHTEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for an addition binary operation
	**
	** \code
	** <expr> '<<' <expr>
	** \endcode
	*/
	class ShiftRightExpressionNode: public Node
	{
	public:
		ShiftRightExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~ShiftRightExpressionNode()
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

#endif // __NANY_AST_SHIFTRIGHTEXPRESSIONNODE_H__
