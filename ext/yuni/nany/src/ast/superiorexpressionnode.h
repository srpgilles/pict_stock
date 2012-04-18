#ifndef __NANY_AST_SUPERIOREXPRESSIONNODE_H__
# define __NANY_AST_SUPERIOREXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a strict superior binary operation
	**
	** \code
	** <expr> '>' <expr>
	** \endcode
	*/
	class SuperiorExpressionNode: public Node
	{
	public:
		SuperiorExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~SuperiorExpressionNode()
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

	}; // class SuperiorExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_SUPERIOREXPRESSIONNODE_H__
