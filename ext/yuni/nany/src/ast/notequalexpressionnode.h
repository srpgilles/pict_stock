#ifndef __NANY_AST_NOTEQUALEXPRESSIONNODE_H__
# define __NANY_AST_NOTEQUALEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a strict equal binary operation
	**
	** \code
	** <expr> '!=' <expr>
	** \endcode
	*/
	class NotEqualExpressionNode: public Node
	{
	public:
		NotEqualExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~NotEqualExpressionNode()
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

#endif // __NANY_AST_NOTEQUALEXPRESSIONNODE_H__
