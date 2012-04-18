#ifndef __NANY_AST_ATTRIBUTEDECLARATIONNODE_H__
# define __NANY_AST_ATTRIBUTEDECLARATIONNODE_H__

# include "yuni/core/string.h"
# include "node.h"
# include "typeexpressionnode.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a local variable declaration
	**
	** \code
	** [const] attribute <expr> [: <expr>] [:= <expr>]
	** \endcode
	*/
	class AttributeDeclarationNode: public Node
	{
	public:
		template<class StringT>
		AttributeDeclarationNode(const StringT& name, TypeExpressionNode* type, Node* value):
			pName(name),
			pTypeDecl(type),
			pValue(value)
		{}

		virtual ~AttributeDeclarationNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& name() const { return pName; }
		TypeExpressionNode* typeDecl() const { return pTypeDecl; }
		Node* value() const { return pValue; }

	private:
		//! Identifier
		Yuni::String pName;
		//! Type
		TypeExpressionNode* pTypeDecl;
		//! Value for initialization, used for typing when typing is missing
		Node* pValue;

	}; // class AttributeDeclarationNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_ATTRIBUTEDECLARATIONNODE_H__
