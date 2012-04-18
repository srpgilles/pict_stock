#ifndef __NANY_AST_EQUALEXPRESSIONNODE_H__
# define __NANY_AST_EQUALEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a strict equal binary operation
	**
	** \code
	** <expr> '=' <expr>
	** \endcode
	*/
	class EqualExpressionNode: public Node
	{
	public:
		EqualExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~EqualExpressionNode()
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

	}; // class EqualExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_EQUALEXPRESSIONNODE_H__
