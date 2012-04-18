#ifndef __NANY_AST_MODULUSEXPRESSIONNODE_H__
# define __NANY_AST_MODULUSEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a modulus binary operation
	**
	** \code
	** <expr> '%' <expr>
	** \endcode
	*/
	class ModulusExpressionNode: public Node
	{
	public:
		ModulusExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~ModulusExpressionNode()
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

#endif // __NANY_AST_MODULUSEXPRESSIONNODE_H__
