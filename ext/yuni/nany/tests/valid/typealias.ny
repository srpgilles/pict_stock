#! This test uses type aliases

class Typedef
{
public
	typedef A : Typedef;

	var k := 0;
}

typedef B : Typedef.A;


typedef F: function(a, b);

function main
{
	var f: F := function (a, b) {
		return a + b;
	};
	var b : B;
	b.k := f(1, 2);
	return b.k;
}
