#ifndef __NANY_AST_ARGUMENTLISTNODE_H__
# define __NANY_AST_ARGUMENTLISTNODE_H__

# include <list>
# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a list of function call arguments separated by ','
	**
	** \code
	** <expr> ',' <expr> ',' <expr>
	** \endcode
	*/
	class ArgumentListNode: public Node
	{
	public:
		typedef std::list<Node*>  List;

	public:
		ArgumentListNode()
		{}

		virtual ~ArgumentListNode()
		{
			List::iterator end = pArguments.end();
			for (List::iterator it = pArguments.begin(); it != end; ++it)
				delete *it;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		void prepend(Node* child)
		{
			pArguments.push_front(child);
		}

		List& arguments() { return pArguments; }

	public:
		List pArguments;

	}; // class ArgumentListNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_ARGUMENTLISTNODE_H__
