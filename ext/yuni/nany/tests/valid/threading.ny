# This test is a remix of the Fibonacci test with threading added

//! Fibonacci (recursive way)
immutable function Fibonacci(n) : typeof n
{
	if n < 2 then
		return n;
	else
		return Fibonacci(n - 1) + Fibonacci(n - 2);
}

function main: int
{
	// Compute the result of `a` into another thread
	var a := & Fibonacci(40);
	// Compute the result of `b` into another thread
	var b := & Fibonacci(60);
	// Wait for the result of a and b, then compute a+b
	var c := a + b;
	// Print the result
	println("a + b = ", c);
	return 0;
}
