#ifndef __NANY_AST_FWD_H__
# define __NANY_AST_FWD_H__

namespace Nany
{
namespace Ast
{

	//! Abstract base node
	class Node;

	//! Root node, the full file : either a program or a unit
	class ProgramNode;

	//! Unit or program declaration
	class UnitDeclarationNode;

	//! List of declarations at program scope
	class DeclarationListNode;

	//! Function declaration
	class FunctionDeclarationNode;

	//! Enum declaration
	class EnumDeclarationNode;

	//! Class declaration
	class ClassDeclarationNode;

	//! Method declaration
	class MethodDeclarationNode;

	//! Attribute declaration
	class AttributeDeclarationNode;

	//! Attribute declaration
	class TypeAliasNode;

	//! Scope
	class ScopeNode;

	//! List of expressions separated by semi-commas
	class ExpressionListNode;

	//! List of parameters in a function / method declaration
	class ParameterListNode;

	//! List of arguments in a function / method call
	class ArgumentListNode;

	//! Visibility qualifier
	class VisibilityQualifierNode;

	//! Conditional expression
	class IfExpressionNode;

	//! Repetitive expression
	class WhileExpressionNode;

	//! Repetitive expression for iterating on a set of data
	class ForExpressionNode;

	//! Expression that parallelizes the execution of a contained expression
	class ParallelExpressionNode;

	//! Expression representing a type
	class TypeExpressionNode;

	//! List of type qualifiers: ref, const, volatile
	class TypeQualifierListNode;

	//! Variable declaration
	class VarDeclarationNode;

	//! Assignment or variable declaration
	class AssignmentExpressionNode;

	//! Access to a class field
	class DotExpressionNode;

	//! Equal comparison
	class EqualExpressionNode;

	//! Not equal comparison
	class NotEqualExpressionNode;

	//! Inferior binary comparison
	class InferiorExpressionNode;

	//! Inferior or equal binary comparison
	class InferiorEqualExpressionNode;

	//! Superior binary comparison
	class SuperiorExpressionNode;

	//! Superior or equal binary comparison
	class SuperiorEqualExpressionNode;

	//! Shift left operator
	class ShiftLeftExpressionNode;

	//! Shift right operator
	class ShiftRightExpressionNode;

	//! Addition
	class PlusExpressionNode;

	//! Subtraction
	class MinusExpressionNode;

	//! Multiplication
	class MultiplyExpressionNode;

	//! Division
	class DivideExpressionNode;

	//! Modulus
	class ModulusExpressionNode;

	//! Type-testing
	class IsExpressionNode;

	//! Type-casting
	class AsExpressionNode;

	//! typeof operator
	class TypeofExpressionNode;

	//! new operator
	class NewExpressionNode;

	//! Expression following a `return` keyword
	class ReturnExpressionNode;

	//! Function or method call
	class FunctionCallNode;

	//! A type or variable identifier
	class IdentifierNode;

	//! Template for any kind of literal node
	template<class DataT>
	class LiteralNode;


} // namespace Ast
} // namespace Nany

#endif // __NANY_AST_FWD_H__
