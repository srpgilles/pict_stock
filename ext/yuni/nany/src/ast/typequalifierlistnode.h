#ifndef __NANY_AST_TYPEQUALIFIERLISTNODE_H__
# define __NANY_AST_TYPEQUALIFIERLISTNODE_H__

# include "yuni/core/string.h"
# include "node.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief AST node for a list of type qualifiers
	**
	** \code
	** const ref volatile
	** \endcode
	*/
	class TypeQualifierListNode: public Node
	{
	public:
		enum TypeQualifier
		{
			tqConst,
			tqRef,
			tqVolatile
		};

	public:
		typedef std::list<TypeQualifier> List;

	public:
		TypeQualifierListNode()
		{}

		virtual ~TypeQualifierListNode()
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		List& qualifier() { return pQualifier; }
		const List& qualifier() const { return pQualifier; }

		void prepend(TypeQualifier tq) { pQualifier.push_front(tq); }

	public:
		List pQualifier;

	}; // class TypeQualifierListNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_TYPEQUALIFIERLISTNODE_H__
