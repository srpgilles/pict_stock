#ifndef __NANY_AST_ASEXPRESSIONNODE_H__
# define __NANY_AST_ASEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a cast operation
	**
	** \code
	** <expr> 'as' <expr>
	** \endcode
	*/
	class AsExpressionNode: public Node
	{
	public:
		AsExpressionNode(Node* left, TypeExpressionNode* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~AsExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* left() const { return pLeft; }
		TypeExpressionNode* right() const { return pRight; }

	public:
		Node* pLeft;
		TypeExpressionNode* pRight;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_ASEXPRESSIONNODE_H__
