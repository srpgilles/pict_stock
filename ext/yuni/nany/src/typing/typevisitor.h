#ifndef __NANY_AST_TYPEVISITOR_H__
# define __NANY_AST_TYPEVISITOR_H__

# include <iostream>
# include "../ast/simplevisitor.h"

namespace Nany
{
namespace Typing
{


	/*!
	** \brief A visitor that infers types and checks typing
	*/
	class TypeVisitor: public Ast::SimpleVisitor
	{
	public:

		virtual void visit(Ast::FunctionDeclarationNode* node)
		{
			if (node->params())
				node->params()->accept(this);
			if (node->returnType())
				node->returnType()->accept(this);
			if (node->body())
				node->body()->accept(this);

			if (node->returnType())
			{
				// If the return type was given, use it as the whole node's type
				node->type(node->returnType()->type());
				// And make a few checks
				if (node->body() && node->returnType() &&
					node->body()->type() && node->returnType()->type() &&
					node->returnType()->type() != node->body()->type())
				{
					std::cerr << "Type mismatch for method \"" << node->name() << "\" :" << std::endl
							  << "\tReturn type is : " << node->body()->type()->name() << std::endl
							  << "\tExpected : " << node->returnType()->type()->name() << std::endl;
				}
			}
			else
				// If the type was not given, try to get the type from the body
				node->type(node->body() ? node->body()->type() : Type::Get("void"));
		}


		virtual void visit(Ast::MethodDeclarationNode* node)
		{
			if (node->params())
				node->params()->accept(this);
			if (node->returnType())
				node->returnType()->accept(this);
			if (node->body())
				node->body()->accept(this);

			if (node->returnType())
			{
				// If the return type was given, use it as the whole node's type
				node->type(node->returnType()->type());
				// And make a few checks
				if (node->body() && node->returnType() &&
					node->body()->type() && node->returnType()->type() &&
					node->returnType()->type() != node->body()->type())
				{
					std::cerr << "Type mismatch for method \"" << node->name() << "\" :" << std::endl
							  << "\tReturn type is : " << node->body()->type()->name() << std::endl
							  << "\tExpected : " << node->returnType()->type()->name() << std::endl;
				}
			}
			else
				// If the type was not given, try to get the type from the body
				node->type(node->body()->type());
		}

		virtual void visit(Ast::AttributeDeclarationNode* node)
		{
			if (node->typeDecl())
				node->typeDecl()->accept(this);
			if (node->value())
				node->value()->accept(this);
			if (node->typeDecl() && node->value() &&
				node->typeDecl()->type() && node->value()->type() &&
				node->typeDecl()->type() != node->value()->type())
				std::cerr << "Type mismatch in declaration of attribute " << node->name() << " :" << std::endl
						  << "\tDeclared type : " << node->typeDecl()->type()->name() << std::endl
						  << "\tAssigned type : " << node->value()->type()->name() << std::endl;
			// If there is no declared type, try to use the one from the value
			if ((!node->typeDecl() || !node->typeDecl()->type()) &&
				node->value() && node->value()->type())
			{
				node->type(node->value()->type());
			}
		}


		virtual void visit(Ast::TypeAliasNode* node)
		{
			node->expression()->accept(this);
			node->type(node->expression()->type());
		}


		virtual void visit(Ast::ScopeNode* node)
		{
			if (node->expression())
			{
				node->expression()->accept(this);
				node->type(node->expression()->type());
			}
			else
				node->type("void");
		}


		virtual void visit(Ast::IfExpressionNode* node)
		{
			node->condition()->accept(this);
			if (Type::Get("bool") != node->condition()->type())
				std::cerr << "Type mismatch :" << std::endl
						  << "\t`if` condition clause must be boolean. Found type : "
						  << node->condition()->type()->name() << std::endl;
			if (node->thenExpr())
				node->thenExpr()->accept(this);
			if (node->elseExpr())
				node->elseExpr()->accept(this);
			if (node->thenExpr() && node->elseExpr() &&
				node->thenExpr()->type() != node->elseExpr()->type())
			 	std::cerr << "Type mismatch :" << std::endl
			 			  << "\t`then` and `else` branches must return the same type !" << std::endl
						  << "\t`then` clause is :" << std::endl
						  << "\t\t" << node->thenExpr()->type()->name() << std::endl
						  << "\t`else` clause is :" << std::endl
						  << "\t\t" << node->elseExpr()->type()->name() << std::endl;
			node->type(node->thenExpr()->type());
		}


		virtual void visit(Ast::WhileExpressionNode* node)
		{
			node->condition()->accept(this);
			if (Type::Get("bool") != node->condition()->type())
			{
				std::cerr << "Type mismatch :" << std::endl
						  << "\t`while` condition clause must be boolean. Found type : "
						  << node->condition()->type()->name() << std::endl;
			}
			if (node->expression())
				node->expression()->accept(this);
			node->type("void");
		}


		virtual void visit(Ast::ForExpressionNode* node)
		{
			node->set()->accept(this);
			if (node->body())
				node->body()->accept(this);
			node->type("void");
		}


		virtual void visit(Ast::ExpressionListNode* node)
		{
			Ast::ExpressionListNode::List& exprList = node->expressions();
			Ast::ExpressionListNode::List::iterator end = exprList.end();
			for (Ast::ExpressionListNode::List::iterator it = exprList.begin(); it != end; ++it)
				(*it)->accept(this);
			if (!exprList.size())
				node->type("void");
			else
				node->type(exprList.back()->type());
		}


		virtual void visit(Ast::ParameterListNode* node)
		{
			Ast::ParameterListNode::List& paramList = node->parameters();
			Ast::ParameterListNode::List::iterator end = paramList.end();
			for (Ast::ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
				(*it)->accept(this);
		}


		virtual void visit(Ast::ArgumentListNode* node)
		{
			Ast::ArgumentListNode::List& argList = node->arguments();
			Ast::ArgumentListNode::List::iterator end = argList.end();
			for (Ast::ArgumentListNode::List::iterator it = argList.begin(); it != end; ++it)
				(*it)->accept(this);
		}


		virtual void visit(Ast::ParallelExpressionNode* node)
		{
			node->expression()->accept(this);
			// TODO : actually, should be a special type for thread management
			node->type(node->expression()->type());
		}


		virtual void visit(Ast::TypeExpressionNode* node)
		{
			node->expression()->accept(this);
			node->type(node->expression()->type());
		}


		virtual void visit(Ast::VarDeclarationNode* node)
		{
			node->typeDecl()->accept(this);
			node->left()->accept(this);
			node->type("void");
		}


		virtual void visit(Ast::AssignmentExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("void");
		}


		virtual void visit(Ast::DotExpressionNode* node)
		{
			node->left()->accept(this);
			// TODO : Resolve the call to node->field, and find the type
			// This is wrong, it is just a temporary placeholder
			node->type("void");
		}


		virtual void visit(Ast::EqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::NotEqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::InferiorExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::InferiorEqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::SuperiorExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::SuperiorEqualExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::ShiftLeftExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type(node->left()->type());
		}


		virtual void visit(Ast::ShiftRightExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			node->type(node->left()->type());
		}


		virtual void visit(Ast::PlusExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			// Fixme
			node->type(node->left()->type());
		}


		virtual void visit(Ast::MinusExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			// Fixme
			node->type(node->left()->type());
		}


		virtual void visit(Ast::MultiplyExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			// Fixme
			node->type(node->left()->type());
		}


		virtual void visit(Ast::DivideExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			// Fixme
			node->type(node->left()->type());
		}


		virtual void visit(Ast::ModulusExpressionNode* node)
		{
			node->left()->accept(this);
			node->right()->accept(this);
			// Fixme
			node->type(node->left()->type());
		}


		virtual void visit(Ast::AsExpressionNode* node)
		{
			node->right()->accept(this);
			node->left()->accept(this);
			node->type(node->right()->type());
		}


		virtual void visit(Ast::IsExpressionNode* node)
		{
			node->right()->accept(this);
			node->left()->accept(this);
			node->type("bool");
		}


		virtual void visit(Ast::TypeofExpressionNode* node)
		{
			node->expression()->accept(this);
			// ???
			node->type(node->expression()->type());
		}


		virtual void visit(Ast::NewExpressionNode* node)
		{
			node->expression()->accept(this);
			node->type(node->expression()->type());
		}


		virtual void visit(Ast::ReturnExpressionNode* node)
		{
			node->expression()->accept(this);
			node->type(node->expression()->type());
		}


		virtual void visit(Ast::FunctionCallNode*)
		{
			// TODO : Resolve the function or method to the proper declaration
			// The type of this node is the inferred return type
		}


		virtual void visit(Ast::IdentifierNode*)
		{
			// TODO : See if the identifier already exists
			// in the scope to get its type, otherwise associate
			// it with a new type class.
		}


		virtual void visit(Ast::LiteralNode<bool>* node)
		{
			node->type(Type::Get("bool"));
		}


		virtual void visit(Ast::LiteralNode<int>* node)
		{
			node->type(Type::Get("int"));
		}


		virtual void visit(Ast::LiteralNode<unsigned int>* node)
		{
			node->type(Type::Get("unsigned int"));
		}


		virtual void visit(Ast::LiteralNode<float>* node)
		{
			node->type(Type::Get("float"));
		}


		virtual void visit(Ast::LiteralNode<double>* node)
		{
			node->type(Type::Get("double"));
		}


		virtual void visit(Ast::LiteralNode<char>* node)
		{
			node->type(Type::Get("char"));
		}


		virtual void visit(Ast::LiteralNode<wchar_t>* node)
		{
			node->type(Type::Get("wchar_t"));
		}


		virtual void visit(Ast::LiteralNode<char*>* node)
		{
			node->type(Type::Get("char*"));
		}


		virtual void visit(Ast::LiteralNode<const char*>* node)
		{
			node->type(Type::Get("const char*"));
		}


		virtual void visit(Ast::LiteralNode<void*>* node)
		{
			node->type(Type::Get("void*"));
		}


		virtual void visit(Ast::LiteralNode<Typing::Type*>* node)
		{
			node->type(node->data);
		}

	}; // class TypeVisitor



} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_TYPEVISITOR_H__
