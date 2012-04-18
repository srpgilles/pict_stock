#ifndef __NANY_AST_PARAMETERLISTNODE_H__
# define __NANY_AST_PARAMETERLISTNODE_H__

# include <list>
# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a list of function call parameters separated by ','
	**
	** \code
	** <expr> ',' <expr> ',' <expr>
	** \endcode
	*/
	class ParameterListNode: public Node
	{
	public:
		typedef std::list<Node*>  List;

	public:
		ParameterListNode()
		{}

		virtual ~ParameterListNode()
		{
			List::iterator end = pParameters.end();
			for (List::iterator it = pParameters.begin(); it != end; ++it)
				delete *it;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		void prepend(Node* child)
		{
			pParameters.push_front(child);
		}

		List& parameters() { return pParameters; }

	public:
		List pParameters;

	}; // class ParameterListNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_PARAMETERLISTNODE_H__
