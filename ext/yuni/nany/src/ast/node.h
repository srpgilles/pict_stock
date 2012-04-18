#ifndef __NANY_AST_NODE_H__
# define __NANY_AST_NODE_H__

# include "../typing/type.h"

namespace Nany
{
namespace Ast
{

	//! Forward declaration
	class Visitor;

	/*!
	** \brief Abstract syntax tree. Base class for all types of node.
	*/
	class Node
	{
	public:
		Node(): pType(nullptr), pIsConst(false)
		{}

		Node(bool isConst): pType(nullptr), pIsConst(isConst)
		{}

		Node(Typing::Type* type, bool isConst): pType(type), pIsConst(isConst)
		{}

		virtual ~Node() {}

		//! All nodes must implement visitor acceptance
		virtual void accept(Visitor*) = 0;

		void type(const char* typeId)
		{
			pType = Typing::Type::Get(typeId);
		}

		void type(Typing::Type* newType)
		{
			pType = newType;
		}

		Typing::Type* type() const
		{
			return pType;
		}

		bool isConst() const { return pIsConst; }

		void setConst() { pIsConst = true; }

	protected:
		//! Types are flyweight patterns, stored as a static map in Nany::Ast::Type.
		Typing::Type* pType;

		//! Has the node been identified as const ?
		bool pIsConst;

	}; // class Node



} // namespace Ast
} // namespace Nany


#endif // __NANY_AST_NODE_H__
