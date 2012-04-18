# This test checks a simple recursive program with no class, and simple math

#! \brief Fibonacci (recursive way)
immutable function Fibonacci(n) : typeof(n)
{
   if n < 2 then
       return n;
   return Fibonacci(n - 1) + Fibonacci(n - 2);
}

#! \brief Fibonacci (recursive way)
immutable function Fibonacci2(n)
{
	return if n < 2 then n else Fibonacci(n - 1) + Fibonacci(n - 2);
}


function main : int
{
	io.cout << "fibonacci(10) = " << Fibonacci(10) << io.endl;
	return 0;
}
