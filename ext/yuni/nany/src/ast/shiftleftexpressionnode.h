#ifndef __NANY_AST_SHIFTLEFTEXPRESSIONNODE_H__
# define __NANY_AST_SHIFTLEFTEXPRESSIONNODE_H__

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
	class ShiftLeftExpressionNode: public Node
	{
	public:
		ShiftLeftExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~ShiftLeftExpressionNode()
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

#endif // __NANY_AST_SHIFTLEFTEXPRESSIONNODE_H__
