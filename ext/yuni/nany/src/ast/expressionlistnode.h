#ifndef __NANY_AST_EXPRESSIONLISTNODE_H__
# define __NANY_AST_EXPRESSIONLISTNODE_H__

# include <list>
# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a list of expressions separated by ';'
	**
	** By definition, the type of the list is the type of the last expression
	** \code
	** <expr> ';' <expr> ';' <expr>
	** \endcode
	*/
	class ExpressionListNode: public Node
	{
	public:
		typedef std::list<Node*>  List;

	public:
		ExpressionListNode()
		{}

		virtual ~ExpressionListNode()
		{
			List::iterator end = pExpressions.end();
			for (List::iterator it = pExpressions.begin(); it != end; ++it)
				delete *it;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		void prepend(Node* child)
		{
			pExpressions.push_front(child);
		}

		List& expressions() { return pExpressions; }

	public:
		List pExpressions;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_EXPRESSIONLISTNODE_H__
