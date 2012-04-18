#! This test concerns dynamic type-checking and casting

function main : int
{
	var a: int;

	var b: int := a as int;
	var c: bool := b is typeof(b);
	return 0;
}
