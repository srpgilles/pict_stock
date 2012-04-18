#ifndef __NANY_AST_SUPERIOREQUALEXPRESSIONNODE_H__
# define __NANY_AST_SUPERIOREQUALEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a strict superior or equal binary operation
	**
	** \code
	** <expr> '>=' <expr>
	** \endcode
	*/
	class SuperiorEqualExpressionNode: public Node
	{
	public:
		SuperiorEqualExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~SuperiorEqualExpressionNode()
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

	}; // class SuperiorEqualExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_SUPERIOREQUALEXPRESSIONNODE_H__
