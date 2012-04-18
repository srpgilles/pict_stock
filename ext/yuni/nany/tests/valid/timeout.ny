# This test is another twist on Fibonacci with timeout features added

//! Fibonacci (recursive way)
immutable function Fibonacci(n) : typeof n
{
	if n < 2 then
		return n;
	else
		return Fibonacci(n - 1) + Fibonacci(n - 2);
}

function main
{
	var x := & Fibonacci(50) ^ Fibonacci(10);
	var countdown := 5;
	timeout 1.seconds do
	{
		// we wait here for the asynchronous result of x
		println("Fibonacci(50) ^ Fibonacci(10) = ", x);
	}
	else
	{
		if --countdown then
		{
			io.cout << countdown << '\n';
			// continue the execution of the expression and wait for 1s again
			continue;
		}
		else
		{
			abort(x);
			io.cout << "Operation timeout\n";
			return -1;
		}
	}
	return 0;
}
