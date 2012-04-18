
#include "cppwritervisitor.h"


namespace Nany
{
namespace Ast
{
	
	CppWriterVisitor::CppWriterVisitor() :
		pFunctionScope(false),
		pSkipCOW(0u),
		pIndent()
	{}


	CppWriterVisitor::~CppWriterVisitor()
	{
	}



	void CppWriterVisitor::visit(ProgramNode* node)
	{
		out << "#include <yuni/yuni.h>\n";
		out << "#include <yuni/core/cow.h>\n";
		out << "#include <yuni/core/string.h>\n";
		out << "#include <yuni/io/file.h>\n";
		out << "#include <yuni/io/io.h>\n";
		out << "#include <typeinfo>\n";
		out << "#include <iostream>\n";
		out << "#include <set>\n";
		out << "#include <list>\n";
		out << "#include <vector>\n";
		out << "#include <map>\n";
		out << "#include <cassert>\n";
		out << '\n';
		out << "using namespace ::Yuni;\n";
		out << '\n';
		out << '\n';
		if (node->unitDeclaration())
			node->unitDeclaration()->accept(this);
		node->declarations()->accept(this);
	}

	void CppWriterVisitor::visit(DeclarationListNode* node)
	{
		DeclarationListNode::List& declList = node->declarations();
		DeclarationListNode::List::iterator end = declList.end();
		for (DeclarationListNode::List::iterator it = declList.begin(); it != end; ++it)
			(*it)->accept(this);
	}


	void CppWriterVisitor::visit(FunctionDeclarationNode* node)
	{
		out << pIndent;

		// Generate a template parameter for untyped function parameters
		if (node->params())
		{
			ParameterListNode::List& paramList = node->params()->parameters();
			ParameterListNode::List::iterator end = paramList.end();
			unsigned int count = 0;
			for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
			{
				if (!(*it)->type())
					++count;
			}
			if (count > 0)
			{
				out << "template<";
				for (unsigned int increment = 1; increment <= count; ++increment)
				{
					out << "class MT" << increment;
					if (increment < count)
						out << ", ";
				}
				out << '>' << '\n' << pIndent;
			}
		}

		// Return type
		if (node->returnType() && node->returnType()->type())
			writeType(node->returnType()->type());
		else if (node->type())
			writeType(node->type());
		else // Default to void
			out << "void";
		// Function name
		out << ' ' << node->name() << '(';
		// Function parameters
		if (node->params())
		{
			ParameterListNode::List& paramList = node->params()->parameters();
			unsigned int size = (unsigned int) paramList.size();
			unsigned int count = 1;

			ParameterListNode::List::iterator end = paramList.end();
			for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
			{
				// If there is no type, we have declared a template parameter for it
				if (!(*it)->type())
					out << "COW<MT" << count++ << "> ";
				(*it)->accept(this);
				if (--size)
					out << ", ";
			}
		}
		out << ')' << '\n' << pIndent;

		// Body
		pFunctionScope = true;
		if (node->body())
			node->body()->accept(this);
		else
			out << "{}\n";
		out << '\n';
		pFunctionScope = false;
	}


	void CppWriterVisitor::visit(ClassDeclarationNode* node)
	{
		out << "class " << node->name() << '\n';
		out << '{' << '\n';
		indent();
		if (node->declarations())
			node->declarations()->accept(this);
		unindent();
		out << "};\n";
		out << '\n';
	}


	void CppWriterVisitor::visit(MethodDeclarationNode* node)
	{
		out << pIndent;

		// Generate a template parameter for untyped method parameters
		if (node->params())
		{
			ParameterListNode::List& paramList = node->params()->parameters();
			ParameterListNode::List::iterator end = paramList.end();
			unsigned int count = 0;
			for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
			{
				if (!(*it)->type())
					++count;
			}
			if (count > 0)
			{
				out << "template<";
				for (unsigned int increment = 1; increment <= count; ++increment)
				{
					out << "class MT" << increment;
					if (increment < count)
						out << ", ";
				}
				out << '>' << '\n' << pIndent;
			}
		}

		// Return type
		if (node->returnType() && node->returnType()->type())
			writeType(node->returnType()->type());
		else if (node->type())
			writeType(node->type());
		else // Default to void
			out << "void";
		// Method name
		out << ' ' << node->name() << '(';
		// Method parameters
		if (node->params())
		{
			ParameterListNode::List& paramList = node->params()->parameters();
			unsigned int size = (unsigned int) paramList.size();
			unsigned int count = 1;

			ParameterListNode::List::iterator end = paramList.end();
			for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
			{
				// If there is no type, we have declared a template parameter for it
				if (!(*it)->type())
					out << "COW<MT" << count++ << "> ";
				(*it)->accept(this);
				if (--size)
					out << ", ";
			}
		}
		out << ')' << '\n' << pIndent;

		// Body
		pFunctionScope = true;
		if (node->body())
			node->body()->accept(this);
		else
			out << "{}\n";
		out << '\n';
		pFunctionScope = false;
	}


	void CppWriterVisitor::visit(AttributeDeclarationNode* node)
	{
		out << pIndent;
		if (node->typeDecl() && node->typeDecl()->type())
			writeType(node->typeDecl()->type());
		else if (node->type())
			writeType(node->type());
		else // Default to int...
			out << "int";
		out << " " << node->name();
		if (node->value())
		{
			out << " = ";
			node->value()->accept(this);
		}
		out << ';' << '\n';
	}


	void CppWriterVisitor::visit(TypeAliasNode* node)
	{
		out << pIndent;
		std::cout << pIndent << "typedef ";
		node->expression()->accept(this);
		out << "  " << node->name() << '\n';
	}


	void CppWriterVisitor::visit(ScopeNode* node)
	{
		out << '{' << '\n';
		indent();
		if (node->expression())
		{
			bool isList = (dynamic_cast<ExpressionListNode*>(node->expression()));
			// On single expressions, write the indent
			// On expression lists, leave this work to the visit(expressionlistnode)
			if (!isList)
			{
				out << pIndent;
				// On one-liners, add `return` when necessary
				if (pFunctionScope && node->expression()->type() &&
					node->expression()->type() != Typing::Type::Get("void") &&
					!dynamic_cast<ReturnExpressionNode*>(node->expression()))
					out << "return ";
			}
			node->expression()->accept(this);
			if (!isList)
				out << ';' << '\n';
		}
		unindent();
		out << pIndent << '}' << '\n';
	}


	void CppWriterVisitor::visit(VisibilityQualifierNode* node)
	{
		// Visibility qualifiers are displayed at the same indent level as
		// the underlying class, so one level under the rest of the declarations
		unindent();
		out << pIndent;
		switch (node->value())
		{
			case 0:
				out << "private:\n";
				break;
			case 1:
				out << "protected:\n";
				break;
			default: // public + published
				out << "public:\n";
				break;
		}
		indent();
	}


	void CppWriterVisitor::visit(ExpressionListNode* node)
	{
		ExpressionListNode::List& exprList = node->expressions();
		unsigned int size = (unsigned int) exprList.size();

		ExpressionListNode::List::iterator end = exprList.end();
		for (ExpressionListNode::List::iterator it = exprList.begin(); it != end; ++it)
		{
			out << pIndent;

			// If on last expression, check if we may add `return`
			if (!--size && pFunctionScope && !dynamic_cast<ReturnExpressionNode*>(*it))
			{
				// If the type is null, we might as well treat is as `void`
				if ((*it)->type() && (*it)->type() != Typing::Type::Get("void"))
					out << "return ";
			}
			(*it)->accept(this);
			out << ';' << '\n';
		}
	}


	void CppWriterVisitor::visit(ParameterListNode* node)
	{
		ParameterListNode::List& paramList = node->parameters();
		unsigned int size = (unsigned int) paramList.size();

		ParameterListNode::List::iterator end = paramList.end();
		for (ParameterListNode::List::iterator it = paramList.begin(); it != end; ++it)
		{
			(*it)->accept(this);
			if (--size)
				out << ", ";
		}
	}


	void CppWriterVisitor::visit(ArgumentListNode* node)
	{
		ArgumentListNode::List& argList = node->arguments();
		unsigned int size = (unsigned int) argList.size();

		ArgumentListNode::List::iterator end = argList.end();
		for (ArgumentListNode::List::iterator it = argList.begin(); it != end; ++it)
		{
			(*it)->accept(this);
			if (--size)
				out << ", ";
		}
	}


	void CppWriterVisitor::visit(IfExpressionNode* node)
	{
		out << "if (";
		node->condition()->accept(this);
		out << ')' << '\n';
		bool isScope;
		if (node->thenExpr())
		{
			isScope = (dynamic_cast<ScopeNode*>(node->thenExpr()));
			if (!isScope)
				indent();
			out << pIndent;
			node->thenExpr()->accept(this);
			if (!isScope)
			{
				out << ';' << '\n';
				unindent();
			}
		}
		if (node->elseExpr())
		{
			out << pIndent << "else\n";
			isScope = (dynamic_cast<ScopeNode*>(node->elseExpr()));
			if (!isScope)
				indent();
			out << pIndent;
			node->elseExpr()->accept(this);
			if (!isScope)
			{
				out << ';' << '\n';
				unindent();
			}
		}
	}


	void CppWriterVisitor::visit(WhileExpressionNode* node)
	{
		out << "while (";
		node->condition()->accept(this);
		out << ')' << '\n';
		if (node->expression())
		{
			bool isScope =
				(dynamic_cast<ScopeNode*>(node->expression())) ||
				(dynamic_cast<ExpressionListNode*>(node->expression()));
			if (!isScope)
				indent();
			out << pIndent;
			node->expression()->accept(this);
			if (!isScope)
			{
				unindent();
			}
		}
		else
			out << "{}";
	}


	void CppWriterVisitor::visit(ForExpressionNode* node)
	{
		out << "for (auto " << node->identifier() << " = ";
		node->set()->accept(this);
		out << "->begin(); " << node->identifier() << " != ";
		node->set()->accept(this);
		out << "->end(); ++" << node->identifier() << ')' << '\n';
		if (node->body())
		{
			bool isScope = dynamic_cast<ScopeNode*>(node->body());
			if (!isScope)
				indent();
			out << pIndent;
			node->body()->accept(this);

			if (!isScope)
				unindent();
		}
		else
			out << "{}";
	}


	void CppWriterVisitor::visit(ParallelExpressionNode* node)
	{
		// TODO : parallelize the expression
		node->expression()->accept(this);
	}


	void CppWriterVisitor::visit(TypeExpressionNode* node)
	{
		if (!pSkipCOW)
		{
			if (node->isArray())
			{
				out << "COW<std::vector<";
				node->expression()->accept(this);
				out << " > >";
			}
			else if (!node->type()->isValue())
			{
				out << "COW<";
				node->expression()->accept(this);
				out << " >";
			}
			else
				node->expression()->accept(this);
		}
		else
		{
			if (node->isArray())
			{
				out << "std::vector<";
				node->expression()->accept(this);
				out << " >";
			}
			else
				node->expression()->accept(this);
		}
	}


	void CppWriterVisitor::visit(VarDeclarationNode* node)
	{
		node->typeDecl()->accept(this);
		out << ' ';

		node->left()->accept(this);
		if (node->typeDecl()->isArray() && node->typeDecl()->arrayCardinality() > 0)
		{
			++pSkipCOW;
			out << " = COW<";
			node->typeDecl()->accept(this);
			out << " >(new ";
			node->typeDecl()->accept(this);
			out << '(' << node->typeDecl()->arrayCardinality() << "))";
			--pSkipCOW;
		}
	}


	void CppWriterVisitor::visit(AssignmentExpressionNode* node)
	{
		node->left()->accept(this);
		out << " = ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(DotExpressionNode* node)
	{
		node->left()->accept(this);
		out << '.' << node->field();
	}


	void CppWriterVisitor::visit(EqualExpressionNode* node)
	{
		if (node->left()->type() && node->left()->type()->isValue())
			node->left()->accept(this);
		else
		{
			out << "(*";
			node->left()->accept(this);
			out << ")";
		}

		out << " == ";

		if (node->right()->type() && node->right()->type()->isValue())
			node->right()->accept(this);
		else
		{
			out << "(*";
			node->right()->accept(this);
			out << ")";
		}
	}


	void CppWriterVisitor::visit(NotEqualExpressionNode* node)
	{
		node->left()->accept(this);
		out << " != ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(InferiorExpressionNode* node)
	{
		node->left()->accept(this);
		out << " < ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(InferiorEqualExpressionNode* node)
	{
		node->left()->accept(this);
		out << " <= ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(SuperiorExpressionNode* node)
	{
		node->left()->accept(this);
		out << " > ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(SuperiorEqualExpressionNode* node)
	{
		node->left()->accept(this);
		out << " >= ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(ShiftLeftExpressionNode* node)
	{
		node->left()->accept(this);
		out << " << ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(ShiftRightExpressionNode* node)
	{
		node->left()->accept(this);
		out << " >> ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(PlusExpressionNode* node)
	{
		node->left()->accept(this);
		out << " + ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(MinusExpressionNode* node)
	{
		node->left()->accept(this);
		out << " - ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(MultiplyExpressionNode* node)
	{
		node->left()->accept(this);
		out << " * ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(DivideExpressionNode* node)
	{
		node->left()->accept(this);
		out << " / ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(ModulusExpressionNode* node)
	{
		node->left()->accept(this);
		out << " % ";
		node->right()->accept(this);
	}


	void CppWriterVisitor::visit(AsExpressionNode* node)
	{
		out << "((";
		node->right()->accept(this);
		out << ")";
		node->left()->accept(this);
		out << ")";
	}


	void CppWriterVisitor::visit(IsExpressionNode* node)
	{
		if (!node->type() || node->type()->isValue())
		{
			out << "(typeid(";
			node->left()->accept(this);
			out << ") == typeid(";
			node->right()->accept(this);
			out << "))";
		}
		else
		{
			out << "(0 != dynamic_cast<";
			node->right()->accept(this);
			out << ">(";
			node->left()->accept(this);
			out << "))";
		}
	}


	void CppWriterVisitor::visit(TypeofExpressionNode* node)
	{
		out << "typeof (";
		node->expression()->accept(this);
		out << ")";
	}


	void CppWriterVisitor::visit(NewExpressionNode* node)
	{
		out << "new ";
		pSkipCOW++;
		node->expression()->accept(this);
		pSkipCOW--;
	}


	void CppWriterVisitor::visit(ReturnExpressionNode* node)
	{
		out << "return ";
		node->expression()->accept(this);
	}


	void CppWriterVisitor::visit(FunctionCallNode* node)
	{
		node->function()->accept(this);
		out << '(';
		if (node->params())
			node->params()->accept(this);
		out << ')';
	}


	void CppWriterVisitor::visit(IdentifierNode* node)
	{
		out << node->data;
	}


	void CppWriterVisitor::visit(LiteralNode<bool>* node)
	{
		out << (node->data ? "true" : "false");
	}


	void CppWriterVisitor::visit(LiteralNode<int>* node)
	{
		out << node->data;
	}


	void CppWriterVisitor::visit(LiteralNode<unsigned int>* node)
	{
		out << node->data << 'u';
	}


	void CppWriterVisitor::visit(LiteralNode<float>* node)
	{
		out << node->data << 'f';
	}


	void CppWriterVisitor::visit(LiteralNode<double>* node)
	{
		out << node->data;
	}


	void CppWriterVisitor::visit(LiteralNode<char>* node)
	{
		out << '\'' << node->data << '\'';
	}


	void CppWriterVisitor::visit(LiteralNode<wchar_t>* node)
	{
		size_t len = wcslen(&node->data);
		char* buffer = new char[len];
		wctomb(buffer, node->data);
		buffer[len] = '\0';

		out << '\'' << buffer << '\'';
	}


	void CppWriterVisitor::visit(LiteralNode<char*>* node)
	{
		out << "COW<String>(new String(\"" << node->data << "\"))";
	}


	void CppWriterVisitor::visit(LiteralNode<const char*>* node)
	{
		out << "COW<String>(new String(\"" << node->data << "\"))";
	}


	void CppWriterVisitor::visit(LiteralNode<void*>* node)
	{
		out << "(void*)" << node->data;
	}


	void CppWriterVisitor::visit(LiteralNode<Typing::Type*>* node)
	{
		writeType(node->data);
	}


	void CppWriterVisitor::writeType(Typing::Type* type)
	{
		if (!type)
			return;
		if (type->isValue())
		{
			out << type->name();
			return;
		}
		if (type->isConst())
			out << "const ";
		out << "COW<" << type->name()<< " >";
	}





} // namespace Ast
} // namespace Nany
