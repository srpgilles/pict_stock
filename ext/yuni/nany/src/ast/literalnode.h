#ifndef __NANY_AST_LITERALNODE_H__
# define __NANY_AST_LITERALNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a literal
	*/
	template<class DataT>
	class LiteralNode: public Node
	{
	public:
		typedef DataT DataType;

	public:
		LiteralNode(DataT other): Node(true), data(other)
		{}

		virtual ~LiteralNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

	public:
		DataT data;

	}; // class LiteralNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_LITERALNODE_H__
