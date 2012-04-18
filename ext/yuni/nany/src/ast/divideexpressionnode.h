#ifndef __NANY_AST_DIVIDEEXPRESSIONNODE_H__
# define __NANY_AST_DIVIDEEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a division binary operation
	**
	** \code
	** <expr> '/' <expr>
	** \endcode
	*/
	class DivideExpressionNode: public Node
	{
	public:
		DivideExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~DivideExpressionNode()
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

#endif // __NANY_AST_DIVIDEEXPRESSIONNODE_H__
