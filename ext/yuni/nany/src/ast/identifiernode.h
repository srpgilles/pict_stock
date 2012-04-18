#ifndef __NANY_AST_IDENTIFIERNODE_H__
# define __NANY_AST_IDENTIFIERNODE_H__

# include "yuni/core/string.h"
# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for an identifier
	*/
	class IdentifierNode: public Node
	{
	public:
		template<class StringT>
		IdentifierNode(const StringT& other): data(other)
		{}

		virtual ~IdentifierNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

	public:
		const Yuni::String data;

	}; // class IdentifierNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_IDENTIFIERNODE_H__
