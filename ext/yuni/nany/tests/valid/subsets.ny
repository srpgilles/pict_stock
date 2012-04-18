
function main()
{
	io.cout << each([:0..24:] where i | i.isPrime) << io.endl;
	return 0;
}

