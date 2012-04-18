# This test uses a `while` clause

function main
{
	var w: int := 0;
	var x: uint := 42;
	while x > w do
	{
		x := x - 1;
	}
    return 0;
}
