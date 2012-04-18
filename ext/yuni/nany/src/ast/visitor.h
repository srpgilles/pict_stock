#ifndef __NANY_AST_VISITOR_H__
# define __NANY_AST_VISITOR_H__

# include "fwd.h"
# include "../typing/type.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief Base empty visitor. Visits the tree but does nothing.
	*/
	class Visitor
	{
	public:
		virtual void visit(ProgramNode*) = 0;

		virtual void visit(UnitDeclarationNode*) = 0;

		virtual void visit(DeclarationListNode*) = 0;

		virtual void visit(FunctionDeclarationNode*) = 0;

		virtual void visit(EnumDeclarationNode*) = 0;

		virtual void visit(ClassDeclarationNode*) = 0;

		virtual void visit(MethodDeclarationNode*) = 0;

		virtual void visit(AttributeDeclarationNode*) = 0;

		virtual void visit(TypeAliasNode*) = 0;

		virtual void visit(ScopeNode*) = 0;

		virtual void visit(VisibilityQualifierNode*) = 0;

		virtual void visit(IfExpressionNode*) = 0;

		virtual void visit(WhileExpressionNode*) = 0;

		virtual void visit(ForExpressionNode*) = 0;

		virtual void visit(ExpressionListNode*) = 0;

		virtual void visit(ParameterListNode*) = 0;

		virtual void visit(ArgumentListNode*) = 0;

		virtual void visit(ParallelExpressionNode*) = 0;

		virtual void visit(TypeExpressionNode*) = 0;

		virtual void visit(TypeQualifierListNode*) = 0;

		virtual void visit(VarDeclarationNode*) = 0;

		virtual void visit(AssignmentExpressionNode*) = 0;

		virtual void visit(DotExpressionNode*) = 0;

		virtual void visit(EqualExpressionNode*) = 0;

		virtual void visit(NotEqualExpressionNode*) = 0;

		virtual void visit(InferiorExpressionNode*) = 0;

		virtual void visit(InferiorEqualExpressionNode*) = 0;

		virtual void visit(SuperiorExpressionNode*) = 0;

		virtual void visit(SuperiorEqualExpressionNode*) = 0;

		virtual void visit(ShiftLeftExpressionNode*) = 0;

		virtual void visit(ShiftRightExpressionNode*) = 0;

		virtual void visit(PlusExpressionNode*) = 0;

		virtual void visit(MinusExpressionNode*) = 0;

		virtual void visit(MultiplyExpressionNode*) = 0;

		virtual void visit(DivideExpressionNode*) = 0;

		virtual void visit(ModulusExpressionNode*) = 0;

		virtual void visit(AsExpressionNode*) = 0;

		virtual void visit(IsExpressionNode*) = 0;

		virtual void visit(TypeofExpressionNode*) = 0;

		virtual void visit(NewExpressionNode*) = 0;

		virtual void visit(ReturnExpressionNode*) = 0;

		virtual void visit(FunctionCallNode*) = 0;

		virtual void visit(IdentifierNode*) = 0;

		virtual void visit(LiteralNode<bool>*) = 0;

		virtual void visit(LiteralNode<int>*) = 0;

		virtual void visit(LiteralNode<unsigned int>*) = 0;

		virtual void visit(LiteralNode<float>*) = 0;

		virtual void visit(LiteralNode<char>*) = 0;

		virtual void visit(LiteralNode<wchar_t>*) = 0;

		virtual void visit(LiteralNode<char*>*) = 0;

		virtual void visit(LiteralNode<const char*>*) = 0;

		virtual void visit(LiteralNode<void*>*) = 0;

		virtual void visit(LiteralNode<Typing::Type*>*) = 0;
	};



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_VISITOR_H__
