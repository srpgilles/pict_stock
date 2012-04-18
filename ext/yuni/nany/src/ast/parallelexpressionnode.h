#ifndef __NANY_AST_PARALLELEXPRESSIONNODE_H__
# define __NANY_AST_PARALLELEXPRESSIONNODE_H__

# include "node.h"
# include "visitor.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief Expression that parallelizes the execution of a contained expression
	*/
	class ParallelExpressionNode: public Node
	{
	public:
		ParallelExpressionNode(Node* expression):
			pExpression(expression)
		{}

		virtual ~ParallelExpressionNode()
		{
			delete pExpression;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* expression() { return pExpression; }

	private:
		Node* pExpression;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_PARALLELEXPRESSIONNODE_H__
