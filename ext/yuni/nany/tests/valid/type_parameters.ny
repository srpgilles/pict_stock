# Test for type parameters on both classes and methods

private class SListItem<:T:>
{
public
    #! Constructor with item value
    method new(value)
	{
		item := value;
	}

	var next: ref SListItem;
    var item: T;
}


class SList<:T := any:>
{
published
	typedef Type: typeof(T);
	property head: ref read write pHead;

public
    #! Get the nth item or nil if there are not enough items
    method get(n)
    {
        if pHead = nil or n < 0 then
			return nil;

        var current := pHead;
        while n > 0 and current != nil do
        {
            current := current.next;
            n--;
        }
        current;
    }

    #! Add an item to the beginning of the list
    method prepend(item)
    {
        var queue := if pHead = nil then nil else pHead.next;
        pHead := new SListItem<:T:>(item);
        pHead.next := queue;
    }

    method addToEach(value)
    {
		for item in self do
			item += value;
    }

	method print
	{
		for item in self do
			io.out << "\t" << item << io.endl;
	}

	operator += (item)
	{
		append(item);
	}

	operator [] (index)
	{
		get(index);
	}

	operator + (list)
	{
		var tmp := self;
		for i in list do
			tmp += i;
		tmp;
	}

	#! Default enumerator
	view default
	{
		return new class
		{
			method value: ref
			{
				i;
			}

			method next
			{
				i := i.next;
			}
			method valid
			{
				i != nil
			}

		private
			var i: ref := pHead;
		};
	}


private
	var pHead: ref;

} // class SList



function main
{
    var l := new SList<:any:>;
    l.prepend(24);
    l.prepend(12);
    l.addToEach(2);
    io.cout << l << io.endl;
    l.get(1);

	var k := new SList;
	k.prepend("foo");

	var m := k + l;
	return 0;
}
