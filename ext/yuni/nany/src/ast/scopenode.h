#ifndef __NANY_AST_SCOPENODE_H__
# define __NANY_AST_SCOPENODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a scope. Declarations are local to the scope
	*/
	class ScopeNode: public Node
	{
	public:
		ScopeNode(Node* expr): pExpr(expr)
		{}

		virtual ~ScopeNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* expression() const { return pExpr; }

	public:
		Node* pExpr;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_SCOPENODE_H__
