#ifndef __NANY_AST_VISIBILITYQUALIFIERNODE_H__
# define __NANY_AST_VISIBILITYQUALIFIERNODE_H__

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
	** \brief Visibility qualifier declaration
	**
	** \code
	** published|public|protected|private
	** \endcode
	*/
	class VisibilityQualifierNode: public Node
	{
	public:
		enum VisibilityQualifier
		{
			vqPrivate,
			vqProtected,
			vqPublic,
			vqPublished
		};

	public:
		VisibilityQualifierNode(VisibilityQualifier qualifier):
			pQualifier(qualifier)
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		VisibilityQualifier value() const { return pQualifier; }

	private:
		//! Value of the qualifier
		VisibilityQualifier pQualifier;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_VISIBILITYQUALIFIERNODE_H__
