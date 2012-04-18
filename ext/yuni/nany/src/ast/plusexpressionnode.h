#ifndef __NANY_AST_PLUSEXPRESSIONNODE_H__
# define __NANY_AST_PLUSEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for an addition binary operation
	**
	** \code
	** <expr> '+' <expr>
	** \endcode
	*/
	class PlusExpressionNode: public Node
	{
	public:
		PlusExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~PlusExpressionNode()
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

#endif // __NANY_AST_PLUSEXPRESSIONNODE_H__
