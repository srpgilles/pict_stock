#ifndef __NANY_AST_FUNCTIONDECLARATIONNODE_H__
# define __NANY_AST_FUNCTIONDECLARATIONNODE_H__

# include "fwd.h"
# include "node.h"
# include "visitor.h"
# include "yuni/yuni.h"
# include "yuni/core/string.h"
# include "parameterlistnode.h"
# include "scopenode.h"
# include "typeexpressionnode.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief Function declaration
	**
	** \code
	** function Identifier ( <Params> ) { <Expression> }
	** \endcode
	*/
	class FunctionDeclarationNode: public Node
	{
	public:
		template<class StringT>
		FunctionDeclarationNode(const StringT& name, ParameterListNode* params, ScopeNode* body):
			pFunctionName(name),
			pParams(params),
			pBody(body),
			pReturnType(nullptr)
		{}

		template<class StringT>
		FunctionDeclarationNode(const StringT& name, ParameterListNode* params, ScopeNode* body, TypeExpressionNode* returnType):
			pFunctionName(name),
			pParams(params),
			pBody(body),
			pReturnType(returnType)
		{}

		virtual ~FunctionDeclarationNode()
		{
			delete pParams;
			delete pBody;
			delete pReturnType;
		}

		virtual void accept(Visitor* visitor)
		{
			visitor->visit(this);
		}

		const Yuni::String& name() const { return pFunctionName; }

		ParameterListNode* params() const { return pParams; }

		ScopeNode* body() const { return pBody; }

		TypeExpressionNode* returnType() const { return pReturnType; }

	private:
		//! Identifier representing the name of the unit or program
		Yuni::String pFunctionName;

		//! Function parameters
		ParameterListNode* pParams;

		//! The body of the function is an expression
		ScopeNode* pBody;

		//! Return type of the function
		TypeExpressionNode* pReturnType;

	}; // class FunctionDeclarationNode



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_FUNCTIONDECLARATIONNODE_H__
