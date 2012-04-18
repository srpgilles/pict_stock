# This test checks for a simple if-else

function main : int
{
	# Normally we would write : return if false then 1 else 0
	# However this is harder to translate to C++ for now...
    if false then return 1 else return 0
}
