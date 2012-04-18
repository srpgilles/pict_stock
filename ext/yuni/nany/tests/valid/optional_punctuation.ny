#! This test uses optional semi-colons and commas, and the parser should ideally allow them

private class OptionalPunctuation
{
public
	enum e
	{
		Value1,
		Value2 # not Optional comma
	}

	method foo(a): int
	{
		if a > 1 then
			if a < 4 then
				return a;
			else
				return foo(a - 1);
		else
			return foo(a + 1);
	}
}


function main()
{
	return 0;
}

