

private class DoublyLinkedList
{
	var first: Node;
	var last: Node;

	view default
	{
		return new class
		{
			var i := first;

			property value read i write i;

			method next
			{
				i := i.next;
			}
			method previous
			{
				i := i.previous;
			}
			method valid
			{
				return i != nil;
			}
		}
	}
} // class DoublyLinkedList

function main
{
	var l := new DoublyLinkedList;
	for i in l do
		io.out << i << io.endl;
	return 0;
}
