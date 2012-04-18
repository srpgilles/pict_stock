#! Several function declarations and calls

function foo(a: int, b: int, c: char)
{
	var c: int := a + b;
}

function bar(k: uint): bool
{
	return true;
}

function foobar()
{
	var k: string := "test";
}

function mini
{
}

function main: int
{
	var a: int := 22;
	foo(a, 44, '@');
	bar(1u);
	foobar();
	mini;
	return 0;
}
