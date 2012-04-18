#ifndef __NANY_AST_SIMPLEVISITOR_H__
# define __NANY_AST_SIMPLEVISITOR_H__

# include "yuni/yuni.h"
# include "yuni/core/string.h"
# include "visitor.h"
# include "all.h"


namespace Nany
{
namespace Ast
{


	/*!
	** \brief Base empty visitor. Visits the tree but does nothing.
	*/
	class SimpleVisitor: public Visitor
	{
	public:

		virtual void visit(ProgramNode* node)
		{
			if (node->unitDeclaration())
				node->unitDeclaration()->accept(this);
			node->declarations()->accept(this);
		}

		virtual void visit(UnitDeclarationNode*)
		{}

		virtual void visit(DeclarationListNode* node)
		{
			DeclarationListNode::List& declList = node->declarations();
			DeclarationListNode::List::iterator end = declList.end();
			for (DeclarationListNode::List::iterator it = declList.begin(); it != end; ++it)
				(*it)->accept(this);
		}

		virtual void visit(FunctionDeclarationNode* node)
		{
			if (node->params())
				node->params()->accept(this);
			if (node->returnType())
				node->returnType()->accept(this);
			if (node->body())
				node->body()->accept(this);
		}

		virtual void visit(EnumDeclarationNode*)
		{
		}

		virtual void visit(ClassDeclarationNode* node)
		{
			if (node->declarations())
				node->declarations()->accept(this);
		}

		virtual void visit(MethodDeclarationNode* node)
		{
			if (node->params())
				node->params()->accept(this);
			if (node->returnType())
				node->returnType()->accept(this);
			if (node->body())
				node->body()->accept(this);
		}

		virtual void visit(AttributeDeclarationNode* node)
		{
			if (node->typeDecl())
				node->typeDecl()->accept(this);
			if (node->value())
				node->value()->accept(this);
		}

		virtual void visit(TypeAliasNode* node)
		{
			node->expression()->accept(this);
		}

		virtual void visit(ScopeNode* node)
		{
			if (node->expression())
				node->expression()->accept(this);
		}

		virtual void visit(VisibilityQualifierNode*)
		{}

		virtual void visit(IfExpressionNode* node)
		{
			node->condition()->accept(this);
			if (node->thenExpr())
				node->thenExpr()->accept(this);
			if (node->elseExpr())
				node->elseExpr()->accept(this);
		}

		virtual void visit(WhileExpressionNode* node)
		{
			node->condition()->accept(this);
			if (node->expression())
				node->expression()->accept(this);
		}

		virtual void visit(ForExpressionNode* node)
		{
			node->set()->accept(this);
			if (node->body())
				node->body()->accept(this);
		}

		virtual void visit(ExpressionListNode* node)
		{
			ExpressionListNode::List& exprList = node->expressions();
			ExpressionListNode::List::iterator end = exprList.end();
			for (ExpressionListNode::List::iterator it = exprList.begin(); it != end; ++it)
				(*it)->accept(this);
		}

		virtual void visit(ParameterListNode* node)
		{
			ParameterListNode::List& paramList = node->parameters();
			ParameterListNode::List::iterator end = paramList.end();
			for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
				(*it)->accept(this);
		}

		virtual void visit(ArgumentListNode* node)
		{
			ArgumentListNode::List& argList = node->arguments();
			ArgumentListNode::List::iterator end = argList.end();
			for (ArgumentListNode::List::iterator it = argList.begin(); it != end; ++it)
				(*it)->accept(this);
		}

		virtual void visit(ParallelExpressionNode* node)
		{
			node->expression()->accept(this);
		}

		virtual void visit(TypeExpressionNode* node)
		{
			node->expression()->accept(this);
		}

		virtual void visit(TypeQualifierListNode*)
		{}

		virtual void visit(VarDeclarationNode* node)
		{
			node->left()->accept(this);
			node->typeDecl()->accept(this);
		}

		virtual void visit(AssignmentExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(DotExpressionNode* node)
		{
			node->left()->accept(this);
		}

		virtual void visit(EqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(NotEqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(InferiorExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(InferiorEqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(SuperiorExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(SuperiorEqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(ShiftLeftExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(ShiftRightExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(PlusExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(MinusExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(MultiplyExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(DivideExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(ModulusExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(AsExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(IsExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
		}

		virtual void visit(TypeofExpressionNode* node)
		{
			node->expression()->accept(this);
		}

		virtual void visit(NewExpressionNode* node)
		{
			node->expression()->accept(this);
		}

		virtual void visit(ReturnExpressionNode* node)
		{
			node->expression()->accept(this);
		}

		virtual void visit(FunctionCallNode* node)
		{
			node->params()->accept(this);
		}

		virtual void visit(IdentifierNode*)
		{}

		virtual void visit(LiteralNode<bool>*)
		{}

		virtual void visit(LiteralNode<int>*)
		{}

		virtual void visit(LiteralNode<unsigned int>*)
		{}

		virtual void visit(LiteralNode<float>*)
		{}

		virtual void visit(LiteralNode<char>*)
		{}

		virtual void visit(LiteralNode<wchar_t>*)
		{}

		virtual void visit(LiteralNode<char*>*)
		{}

		virtual void visit(LiteralNode<const char*>*)
		{}

		virtual void visit(LiteralNode<void*>*)
		{}

		virtual void visit(LiteralNode<Typing::Type*>*)
		{}


	protected:
		void indent()
		{
			pIndent << '\t';
		}

		void unindent()
		{
			pIndent.removeLast();
		}


	protected:
		//! Current indent, used by several inherited visitors
		Yuni::String pIndent;

	}; // class SimpleVisitor



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_SIMPLEVISITOR_H__
