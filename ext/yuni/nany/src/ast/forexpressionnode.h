#ifndef __NANY_AST_FOREXPRESSIONNODE_H__
# define __NANY_AST_FOREXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a for expression.
	**
	** \code
	** 'for' <identifier> 'in' <expr> 'do' <expr>
	** \endcode
	*/
	class ForExpressionNode: public Node
	{
	public:
		template<class StringT>
		ForExpressionNode(const StringT& identifier, Node* set, Node* body):
			pIdentifier(identifier),
			pSet(set),
			pBody(body)
		{}

		virtual ~ForExpressionNode()
		{
			delete pSet;
			delete pBody;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& identifier() const { return pIdentifier; }
		Node* set() const { return pSet; }
		Node* body() const { return pBody; }

	public:
		Yuni::String pIdentifier;

		Node* pSet;
		Node* pBody;

	}; // class ForExpressionNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_FOREXPRESSIONNODE_H__
