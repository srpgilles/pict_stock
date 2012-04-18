# Test for regular expression syntax

function name
{
	var s := "MyName";
	return s;
}

function main
{
	return if name ~ "[a-zA-Z]+/i" then 0 else 1;
}
