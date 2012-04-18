#ifndef __NANY_AST_TYPEEXPRESSIONNODE_H__
# define __NANY_AST_TYPEEXPRESSIONNODE_H__

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
	** \brief AST node for an expression representing a reference to a type
	*/
	class TypeExpressionNode: public Node
	{
	public:
		TypeExpressionNode(Node* expr):
			pExpression(expr),
			pIsArray(false),
			pCardinality(0u)
		{}

		virtual ~TypeExpressionNode()
		{
			delete pExpression;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		Node* expression() const { return pExpression; }

		void toArray() { pIsArray = true; }
		void toArray(unsigned int cardinality)
		{
			pIsArray = true;
			pCardinality = cardinality;
		}

		bool isArray() const { return pIsArray; }

		unsigned int arrayCardinality() const { return pCardinality; }

	private:
		//! Expression to evaluate to get the type
		Node* pExpression;

		//! Is it an array ?
		bool pIsArray;

		//! Array cardinality, 0 for unknown size
		unsigned int pCardinality;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_TYPEEXPRESSIONNODE_H__
