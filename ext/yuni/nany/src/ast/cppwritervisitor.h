#ifndef __NANY_AST_CPPWRITERVISITOR_H__
# define __NANY_AST_CPPWRITERVISITOR_H__

# include <yuni/yuni.h>
# include <yuni/io/file.h>
# include "simplevisitor.h"


namespace Nany
{
namespace Ast
{

	/*!
	** \brief Visitor that writes an equivalent C++ code
	*/
	class CppWriterVisitor: public SimpleVisitor
	{
	public:
		CppWriterVisitor();
		~CppWriterVisitor();


		virtual void visit(ProgramNode* node);


		virtual void visit(DeclarationListNode* node);


		virtual void visit(FunctionDeclarationNode* node);


		virtual void visit(ClassDeclarationNode* node);


		virtual void visit(MethodDeclarationNode* node);


		virtual void visit(AttributeDeclarationNode* node);


		virtual void visit(TypeAliasNode* node);


		virtual void visit(ScopeNode* node);


		virtual void visit(VisibilityQualifierNode* node);


		virtual void visit(ExpressionListNode* node);


		virtual void visit(ParameterListNode* node);


		virtual void visit(ArgumentListNode* node);


		virtual void visit(IfExpressionNode* node);


		virtual void visit(WhileExpressionNode* node);


		virtual void visit(ForExpressionNode* node);


		virtual void visit(ParallelExpressionNode* node);


		virtual void visit(TypeExpressionNode* node);


		virtual void visit(VarDeclarationNode* node);


		virtual void visit(AssignmentExpressionNode* node);


		virtual void visit(DotExpressionNode* node);


		virtual void visit(EqualExpressionNode* node);


		virtual void visit(NotEqualExpressionNode* node);


		virtual void visit(InferiorExpressionNode* node);


		virtual void visit(InferiorEqualExpressionNode* node);


		virtual void visit(SuperiorExpressionNode* node);


		virtual void visit(SuperiorEqualExpressionNode* node);


		virtual void visit(ShiftLeftExpressionNode* node);


		virtual void visit(ShiftRightExpressionNode* node);


		virtual void visit(PlusExpressionNode* node);


		virtual void visit(MinusExpressionNode* node);


		virtual void visit(MultiplyExpressionNode* node);


		virtual void visit(DivideExpressionNode* node);


		virtual void visit(ModulusExpressionNode* node);


		virtual void visit(AsExpressionNode* node);


		virtual void visit(IsExpressionNode* node);


		virtual void visit(TypeofExpressionNode* node);


		virtual void visit(NewExpressionNode* node);


		virtual void visit(ReturnExpressionNode* node);


		virtual void visit(FunctionCallNode* node);


		virtual void visit(IdentifierNode* node);


		virtual void visit(LiteralNode<bool>* node);


		virtual void visit(LiteralNode<int>* node);


		virtual void visit(LiteralNode<unsigned int>* node);


		virtual void visit(LiteralNode<float>* node);


		virtual void visit(LiteralNode<double>* node);


		virtual void visit(LiteralNode<char>* node);


		virtual void visit(LiteralNode<wchar_t>* node);


		virtual void visit(LiteralNode<char*>* node);


		virtual void visit(LiteralNode<const char*>* node);


		virtual void visit(LiteralNode<void*>* node);


		virtual void visit(LiteralNode<Typing::Type*>* node);


	public:
		//! Stream to the destination C++ file
		Yuni::IO::File::Stream out;

	private:
		void writeType(Typing::Type* type);

		void indent()
		{
			pIndent << '\t';
		}

		void unindent()
		{
			pIndent.removeLast();
		}


	private:
		/*!
		** \brief Are we at the lowest scope inside a function ?
		**
		** This is useful to know when we might need to add a return keyword.
		*/
		bool pFunctionScope;

		/*!
		** \brief Is this expression the last one in a list, or a single one ?
		**
		** This is useful to know when we might need to add a return keyword.
		*/
		bool pLastExpression;

		/*!
		** \brief How many times should we skip the COW declaration in types ?
		**
		** This is useful to know when / where we must not use the COW wrapper
		*/
		unsigned int pSkipCOW;

		//! Current indent
		Yuni::String pIndent;

	}; // class CppWriterVisitor





} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_CPPWRITERVISITOR_H__
