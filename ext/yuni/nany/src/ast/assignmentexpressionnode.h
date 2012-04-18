#ifndef __NANY_AST_ASSIGNMENTEXPRESSIONNODE_H__
# define __NANY_AST_ASSIGNMENTEXPRESSIONNODE_H__

# include "yuni/core/string.h"
# include "node.h"
# include "typeexpressionnode.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for an assignment expression
	**
	** \code
	** <expr> := <expr>
	** \endcode
	*/
	class AssignmentExpressionNode: public Node
	{
	public:
		AssignmentExpressionNode(Node* left, Node* right):
			pLeft(left),
			pRight(right)
		{}

		virtual ~AssignmentExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* left() const { return pLeft; }
		Node* right() const { return pRight; }

	public:
		//! Left part of the assignment
		Node* pLeft;
		//! Right part of the assignment
		Node* pRight;

	}; // class AssignmentExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_ASSIGNMENTEXPRESSIONNODE_H__
