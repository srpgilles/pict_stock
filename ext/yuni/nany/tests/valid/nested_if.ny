# This test checks for complex nested if-else situations

function main : int
{
	var n: int;
	if true then
		if 4 > 0 then
		{
			n := 0;
			if 32 != 21 then
			{
				n := 21;
			}
		}
		else if false then
			n := 1;
		else
			n:= 3;
	else
		n := 2;
	return 0;
}
