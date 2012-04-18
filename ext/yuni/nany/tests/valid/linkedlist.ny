#! Sample implementation for a generic singly linked list

private class SList<:T := any:>
{
public
	property next  read write pNext;
	property data  read write pData;

	method new
	{
		// pData takes default value of T
		// pNext takes nil
	}

	method prepend(n)
	{
		var oldData := pData;
		var oldNext := pNext;
		pData := n;
		pNext := new SList;
		pNext.next := oldNext;
		pNext.data := oldData;
	}

	method append(n)
	{
		if not pNext then
		{
			pNext := new SList;
			pNext.data := n;
		}
		else
			pNext.append(n);
	}

	method count
	{
		return if not pNext then 0 else 1 + pNext.count;
	}

	method print
	{
		io.cout << pData << io.endl;
		pNext.print;
	}

private
	var pData : T;
	var pNext : SList;
}


function testList
{
	var l := new SList; // Here, the type parameter is not yet known
	l.append(42); // Here, we understand that the type is `int`
	l.prepend("Coucou"); // OK, the inferred type for T is a subtype of `int` and `string`
	io.cout << "List contains " << l.count << " elements." << io.endl;
	l.prepend(new class {}); // Error ! `io.cout` does not define operator << for this anonymous class
	l.print;
}
