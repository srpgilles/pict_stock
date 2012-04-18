#! A function declaration and call with no type

function foo(a: ref, b: uint, c)
{
	a << b << c;
}

function main: int
{
	var s := "Plop";
	foo(s, 42, '@');
	return 0;
}
