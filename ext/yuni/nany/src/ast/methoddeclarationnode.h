#ifndef __NANY_AST_METHODDECLARATIONNODE_H__
# define __NANY_AST_METHODDECLARATIONNODE_H__

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
	** \brief Method declaration
	**
	** \code
	** method Identifier ( <Params> ) { <Expression> }
	** \endcode
	*/
	class MethodDeclarationNode: public Node
	{
	public:
		template<class StringT>
		MethodDeclarationNode(const StringT& name, ParameterListNode* params, ScopeNode* body, TypeExpressionNode* returnType, bool abstract):
			pMethodName(name),
			pParams(params),
			pBody(body),
			pReturnType(returnType),
			pAbstract(abstract)
		{}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		virtual ~MethodDeclarationNode()
		{
			delete pParams;
			delete pBody;
			delete pReturnType;
		}

		const Yuni::String& name() const { return pMethodName; }

		ParameterListNode* params() const { return pParams; }

		ScopeNode* body() const { return pBody; }

		TypeExpressionNode* returnType() const { return pReturnType; }

	private:
		//! Identifier representing the name of the unit or program
		Yuni::String pMethodName;

		//! Method parameters
		ParameterListNode* pParams;

		//! The body of the method is an expression
		ScopeNode* pBody;

		//! Return type of the method
		TypeExpressionNode* pReturnType;

		//! Is the method abstract ? (no definition)
		bool pAbstract;

	}; // class MethodDeclarationNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_METHODDECLARATIONNODE_H__
