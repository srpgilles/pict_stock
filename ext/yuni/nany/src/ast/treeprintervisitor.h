#ifndef __NANY_AST_TREEPRINTERVISITOR_H__
# define __NANY_AST_TREEPRINTERVISITOR_H__

# include <iostream>
# include "simplevisitor.h"

namespace Nany
{
namespace Ast
{


	/*!
	** \brief A visitor that prints the names of the nodes to debug if the AST is correct.
	*/
	class TreePrinterVisitor: public SimpleVisitor
	{
	public:

		virtual void visit(ProgramNode* node)
		{
			std::cout << "<Program Node>" << std::endl;
			indent();
			if (node->unitDeclaration())
				node->unitDeclaration()->accept(this);
			node->declarations()->accept(this);
			unindent();
		}


		virtual void visit(UnitDeclarationNode*)
		{
			std::cout << pIndent << "<Unit Declaration Node>" << std::endl;
		}


		virtual void visit(DeclarationListNode* node)
		{
			std::cout << pIndent << "<Declaration List Node>" << std::endl;
			indent();
			DeclarationListNode::List& declList = node->declarations();
			DeclarationListNode::List::iterator end = declList.end();
			for (DeclarationListNode::List::iterator it = declList.begin(); it != end; ++it)
				(*it)->accept(this);
			unindent();
		}


		virtual void visit(FunctionDeclarationNode* node)
		{
			std::cout << pIndent << "<Function Declaration Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->name() << std::endl;
			if (node->params())
				node->params()->accept(this);
			if (node->returnType())
				node->returnType()->accept(this);
			if (node->body())
				node->body()->accept(this);
			unindent();
		}


		virtual void visit(EnumDeclarationNode* node)
		{
			std::cout << pIndent << "<Enum Declaration Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->name() << std::endl;
			EnumDeclarationNode::EnumValueMap* values = node->values();
			EnumDeclarationNode::EnumValueMap::iterator end = values->end();
			for (EnumDeclarationNode::EnumValueMap::iterator it = values->begin(); it != end; ++it)
				std::cout << pIndent << it->first << " : " << it->second << std::endl;
			unindent();
		}


		virtual void visit(ClassDeclarationNode* node)
		{
			std::cout << pIndent << "<Class Declaration Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->name() << std::endl;
			if (node->declarations())
				node->declarations()->accept(this);
			unindent();
		}


		virtual void visit(MethodDeclarationNode* node)
		{
			std::cout << pIndent << "<Method Declaration Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->name() << std::endl;
			if (node->params())
				node->params()->accept(this);
			if (node->returnType())
				node->returnType()->accept(this);
			if (node->body())
				node->body()->accept(this);
			unindent();
		}


		virtual void visit(AttributeDeclarationNode* node)
		{
			std::cout << pIndent << "<Method Declaration Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->name() << std::endl;
			if (node->typeDecl())
				node->typeDecl()->accept(this);
			if (node->value())
				node->value()->accept(this);
			unindent();
		}


		virtual void visit(TypeAliasNode* node)
		{
			std::cout << pIndent << "<Method Declaration Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->name() << std::endl;
			node->expression()->accept(this);
			unindent();
		}


		virtual void visit(ScopeNode* node)
		{
			std::cout << pIndent << "<Scope Node>" << std::endl;
			indent();
			if (node->expression())
				node->expression()->accept(this);
			unindent();
		}


		virtual void visit(VisibilityQualifierNode* node)
		{
			std::cout << pIndent << "<Visibility Qualifier Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->value() << std::endl;
			unindent();
		}


		virtual void visit(IfExpressionNode* node)
		{
			std::cout << pIndent << "<Expression List Node>" << std::endl;
			indent();
			node->condition()->accept(this);
			if (node->thenExpr())
				node->thenExpr()->accept(this);
			if (node->elseExpr())
				node->elseExpr()->accept(this);
			unindent();
		}


		virtual void visit(WhileExpressionNode* node)
		{
			std::cout << pIndent << "<While Expression Node>" << std::endl;
			indent();
			node->condition()->accept(this);
			if (node->expression())
				node->expression()->accept(this);
			unindent();
		}


		virtual void visit(ForExpressionNode* node)
		{
			std::cout << pIndent << "<For Expression Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->identifier() << std::endl;
			node->set()->accept(this);
			if (node->body())
				node->body()->accept(this);
			unindent();
		}


		virtual void visit(ExpressionListNode* node)
		{
			std::cout << pIndent << "<Expression List Node>" << std::endl;
			indent();
			ExpressionListNode::List& exprList = node->expressions();
			ExpressionListNode::List::iterator end = exprList.end();
			for (ExpressionListNode::List::iterator it = exprList.begin(); it != end; ++it)
				(*it)->accept(this);
			unindent();
		}


		virtual void visit(ParameterListNode* node)
		{
			std::cout << pIndent << "<Parameter List Node>" << std::endl;
			indent();
			ParameterListNode::List& paramList = node->parameters();
			ParameterListNode::List::iterator end = paramList.end();
			for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
				(*it)->accept(this);
			unindent();
		}


		virtual void visit(ArgumentListNode* node)
		{
			std::cout << pIndent << "<Argument List Node>" << std::endl;
			indent();
			ArgumentListNode::List& argList = node->arguments();
			ArgumentListNode::List::iterator end = argList.end();
			for (ArgumentListNode::List::iterator it = argList.begin(); it != end; ++it)
				(*it)->accept(this);
			unindent();
		}


		virtual void visit(ParallelExpressionNode* node)
		{
			std::cout << pIndent << "<Parallel Expression Node>" << std::endl;
			indent();
			node->expression()->accept(this);
			unindent();
		}


		virtual void visit(TypeExpressionNode* node)
		{
			std::cout << pIndent << "<Type Expression Node>" << std::endl;
			indent();
			node->expression()->accept(this);
			unindent();
		}


		virtual void visit(VarDeclarationNode* node)
		{
			std::cout << pIndent << "<Var Declaration Node>" << std::endl;
			indent();
			node->typeDecl()->accept(this);
			node->left()->accept(this);
			unindent();
		}


		virtual void visit(AssignmentExpressionNode* node)
		{
			std::cout << pIndent << "<Assignment Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(DotExpressionNode* node)
		{
			std::cout << pIndent << "<Dot Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			std::cout << pIndent << "Symbol : " << node->field() << std::endl;
			unindent();
		}


		virtual void visit(EqualExpressionNode* node)
		{
			std::cout << pIndent << "<Equal Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(NotEqualExpressionNode* node)
		{
			std::cout << pIndent << "<Not Equal Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(InferiorExpressionNode* node)
		{
			std::cout << pIndent << "<Inferior Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(InferiorEqualExpressionNode* node)
		{
			std::cout << pIndent << "<Inferior Equal Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(SuperiorExpressionNode* node)
		{
			std::cout << pIndent << "<Superior Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(SuperiorEqualExpressionNode* node)
		{
			std::cout << pIndent << "<Superior Equal Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(ShiftLeftExpressionNode* node)
		{
			std::cout << pIndent << "<Shift Left Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(ShiftRightExpressionNode* node)
		{
			std::cout << pIndent << "<Shift Right Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(PlusExpressionNode* node)
		{
			std::cout << pIndent << "<Plus Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(MinusExpressionNode* node)
		{
			std::cout << pIndent << "<Minus Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(MultiplyExpressionNode* node)
		{
			std::cout << pIndent << "<Multiply Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(DivideExpressionNode* node)
		{
			std::cout << pIndent << "<Divide Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(ModulusExpressionNode* node)
		{
			std::cout << pIndent << "<Modulus Expression Node>" << std::endl;
			indent();
			node->left()->accept(this);
			node->right()->accept(this);
			unindent();
		}


		virtual void visit(AsExpressionNode* node)
		{
			std::cout << pIndent << "<As Expression Node>" << std::endl;
			indent();
			node->right()->accept(this);
			node->left()->accept(this);
			unindent();
		}


		virtual void visit(IsExpressionNode* node)
		{
			std::cout << pIndent << "<Is Expression Node>" << std::endl;
			indent();
			node->right()->accept(this);
			node->left()->accept(this);
			unindent();
		}


		virtual void visit(TypeofExpressionNode* node)
		{
			std::cout << pIndent << "<Typeof Expression Node>" << std::endl;
			indent();
			node->expression()->accept(this);
			unindent();
		}


		virtual void visit(NewExpressionNode* node)
		{
			std::cout << pIndent << "<New Expression Node>" << std::endl;
			indent();
			node->expression()->accept(this);
			unindent();
		}


		virtual void visit(ReturnExpressionNode* node)
		{
			std::cout << pIndent << "<Return Expression Node>" << std::endl;
			indent();
			node->expression()->accept(this);
			unindent();
		}


		virtual void visit(FunctionCallNode* node)
		{
			std::cout << pIndent << "<Function Call Node>" << std::endl;
			indent();
			node->function()->accept(this);
			if (node->params())
				node->params()->accept(this);
			unindent();
		}


		virtual void visit(IdentifierNode* node)
		{
			std::cout << pIndent << "<Identifier Node>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<bool>* node)
		{
			std::cout << pIndent << "<Literal Node : bool>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<int>* node)
		{
			std::cout << pIndent << "<Literal Node : int>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<unsigned int>* node)
		{
			std::cout << pIndent << "<Literal Node : uint>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<float>* node)
		{
			std::cout << pIndent << "<Literal Node : float>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<double>* node)
		{
			std::cout << pIndent << "<Literal Node : double>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<char>* node)
		{
			std::cout << pIndent << "<Literal Node : char>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<wchar_t>* node)
		{
			std::cout << pIndent << "<Literal Node : wchar_t>" << std::endl;

			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<char*>* node)
		{
			std::cout << pIndent << "<Literal Node : char*>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<const char*>* node)
		{
			std::cout << pIndent << "<Literal Node : const char*>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<void*>* node)
		{
			std::cout << pIndent << "<Literal Node : void*>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : (void*)" << node->data << std::endl;
			unindent();
		}


		virtual void visit(LiteralNode<Typing::Type*>* node)
		{
			std::cout << pIndent << "<Literal Node : Type*>" << std::endl;
			indent();
			std::cout << pIndent << "Symbol : " << node->data->name() << std::endl;
			unindent();
		}

	}; // class TreePrinterVisitor



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_TREEPRINTERVISITOR_H__
