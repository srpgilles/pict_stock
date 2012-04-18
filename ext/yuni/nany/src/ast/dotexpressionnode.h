#ifndef __NANY_AST_DOTEXPRESSIONNODE_H__
# define __NANY_AST_DOTEXPRESSIONNODE_H__

# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for an attribute / method access operator
	**
	** \code
	** <expr> '.' <identifier>
	** \endcode
	*/
	class DotExpressionNode: public Node
	{
	public:
		template<class StringT>
		DotExpressionNode(Node* left, const StringT& field):
			pLeft(left),
			pField(field)
		{}

		virtual ~DotExpressionNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* left() const { return pLeft; }
		const Yuni::String& field() const { return pField; }

	public:
		Node* pLeft;
		Yuni::String pField;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_DOTEXPRESSIONNODE_H__
