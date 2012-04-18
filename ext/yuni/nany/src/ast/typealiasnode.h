#ifndef __NANY_AST_TYPEALIASNODE_H__
# define __NANY_AST_TYPEALIASNODE_H__

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
	** \brief AST node for a type alias (typedef)
	**
	** \code
	** 'type' Identifier := <Expr>
	** \endcode
	*/
	class TypeAliasNode: public Node
	{
	public:
		template<class StringT>
		TypeAliasNode(const StringT& name, TypeExpressionNode* expr):
			pName(name),
			pExpression(expr)
		{}

		virtual ~TypeAliasNode()
		{
			delete pExpression;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& name() const { return pName; }

		TypeExpressionNode* expression() const { return pExpression; }

	private:
		//! Alias
		Yuni::String pName;

		//! Expression to evaluate to get the type
		TypeExpressionNode* pExpression;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_TYPEALIASNODE_H__
