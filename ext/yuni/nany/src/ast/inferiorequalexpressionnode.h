#ifndef __NANY_AST_INFERIOREQUALEXPRESSIONNODE_H__
# define __NANY_AST_INFERIOREQUALEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a strict inferior or equal binary operation
	**
	** \code
	** <expr> '<=' <expr>
	** \endcode
	*/
	class InferiorEqualExpressionNode: public Node
	{
	public:
		InferiorEqualExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~InferiorEqualExpressionNode()
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

	}; // class InferiorEqualExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_INFERIOREQUALEXPRESSIONNODE_H__
