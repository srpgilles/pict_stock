#! Testing inheritance and automatic compliance for heterogeneous arrays

class Shape
{
	//! Surface area
	abstract method surface;
}

class Rectangle : Shape
{
public
	method new
	{
		width  := 50u;
		height := 50u;
	}

	method new(w, h)
	{
		width  := w;
		height := h;
	}

	method surface
	{
		return width * height;
	}

	operator := (copy)
	{
		width  := copy.width;
		height := copy.height;
	}

	//! Width of the rectangle
	var width;
	//! Height of the rectangle
	var height;
}


class Circle : Shape
{
public
	method new
	{
		radius := 1.0;
	}

	method new(r)
	{
		radius := r;
	}

	method surface
	{
		return Math.pi * radius ^ 2;
	}

	//! Circle radius
	var radius;
}





function main
{
	var l := [
		new Rectangle(10, 200),
		new class
		{
			method surface { return "some strange text here"; }
		},
		new Circle ];

	for i in l do
		io.out << i.classname << ": " << i.surface << io.endl;
}
