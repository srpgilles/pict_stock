#ifndef __NANY_AST_FUNCTIONCALLNODE_H__
# define __NANY_AST_FUNCTIONCALLNODE_H__

# include "fwd.h"
# include "node.h"
# include "visitor.h"
# include "yuni/yuni.h"
# include "yuni/core/string.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief Function call
	**
	** \code
	** <Expr> ( <Args> )
	** \endcode
	*/
	class FunctionCallNode: public Node
	{
	public:
		FunctionCallNode(Node* function, ArgumentListNode* params):
			pFunction(function),
			pParams(params)
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* function() const { return pFunction; }

		ArgumentListNode* params() const { return pParams; }

	private:
		//! Expr representing the function
		Node* pFunction;

		//! Function parameters
		ArgumentListNode* pParams;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_FUNCTIONCALLNODE_H__
