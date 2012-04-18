#ifndef __NANY_AST_DECLARATIONLISTNODE_H__
# define __NANY_AST_DECLARATIONLISTNODE_H__

# include <list>
# include "fwd.h"
# include "node.h"
# include "visitor.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief A list of declarations at program scope or class scope
	*/
	class DeclarationListNode: public Node
	{
	public:
		typedef std::list<Node*>  List;

	public:
		DeclarationListNode()
		{}

		virtual ~DeclarationListNode()
		{
			List::iterator end = pDeclarations.end();
			for (List::iterator it = pDeclarations.begin(); it != end; ++it)
				delete *it;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		void prepend(Node* child)
		{
			pDeclarations.push_front(child);
		}

		List& declarations() { return pDeclarations; }

	private:
		List pDeclarations;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_DECLARATIONLISTNODE_H__
