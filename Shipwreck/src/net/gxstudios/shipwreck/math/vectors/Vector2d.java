package net.gxstudios.shipwreck.math.vectors;

/**
This class represents 2D vectors, storing their components as doubles.
Note that the class can be used to represent both point vectors and
free vectors. This is by design and is intended to make working with
vectors easier.
*/
public class Vector2d implements IVector<Vector2d>
{
	//################## PUBLIC VARIABLES ##################//
	public double x, y;

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a new Vector2d.
	*/
	public Vector2d() {}

	/**
	Constructs a new Vector2d with the specified x and y components.

	@param x	The x component of the new vector
	@param y	The y component of the new vector
	*/
	public Vector2d(double x, double y)
	{
		this.x = x;
		this.y = y;
	}

	//################## PUBLIC METHODS ##################//
	public Vector2d add(final Vector2d rhs)
	{
		x+=rhs.x;
		y+=rhs.y;
		return this;
	}

	public Vector2d clone()
	{
		return new Vector2d(x, y);
	}

	public double distance(final Vector2d rhs)
	{
		return Math.sqrt(distance_squared(rhs));
	}

	public double distance_squared(final Vector2d rhs)
	{
		double dx = x - rhs.x, dy = y - rhs.y;
		return dx*dx + dy*dy;
	}

	public double dot(final Vector2d rhs)
	{
		return x*rhs.x + y*rhs.y;
	}

	public boolean equals(final Vector2d rhs)
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	public double length()
	{
		return Math.sqrt(x*x + y*y);	// note that the squared length has been put inline for efficiency
	}

	public double length_squared()
	{
		return x*x + y*y;				// note that dot(this); would do, but it's needlessly inefficient
	}

	public Vector2d negate()
	{
		x = -x;
		y = -y;
		return this;
	}

	public Vector2d negated()
	{
		return new Vector2d(-x, -y);
	}

	public Vector2d normalize()
	{
		double len = length();
		if(Math.abs(len) < SMALL_EPSILON) throw new java.lang.Error();

		scale(1.0/len);
		return this;
	}

	public Vector2d scale(double factor)
	{
		x*=factor;
		y*=factor;
		return this;
	}

	/**
	Returns the result of the operation factor*u + v.

	@param factor	The scaling factor for u
	@param u		The vector to scale
	@param v		The vector to add
	@return			...think about it...
	*/
	public static Vector2d scale_add(double factor, final Vector2d u, final Vector2d v)
	{
		return new Vector2d(factor*u.x + v.x, factor*u.y + v.y);
	}

	public Vector2d subtract(final Vector2d rhs)
	{
		x-=rhs.x;
		y-=rhs.y;
		return this;
	}

	public String toString()
	{
		StringBuffer sb = new StringBuffer("(");
		sb.append(x + ", ");
		sb.append(y + ")");
		return sb.toString();
	}
}