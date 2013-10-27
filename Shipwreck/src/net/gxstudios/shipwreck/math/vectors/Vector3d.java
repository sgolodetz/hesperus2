package net.gxstudios.shipwreck.math.vectors;

import net.gxstudios.shipwreck.test.TestHarnessAdapter;

/**
This class represents 3D vectors, storing their components as doubles.
Note that the class can be used to represent both point vectors and
free vectors. This is by design and is intended to make working with
vectors easier.
*/
public class Vector3d implements IVector<Vector3d>
{
	//################## PUBLIC VARIABLES ##################//
	public double x, y, z;

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a new Vector3d.
	*/
	public Vector3d() {}

	/**
	Constructs a new Vector3d with the specified x, y and z components.

	@param x	The x component of the new vector
	@param y	The y component of the new vector
	@param z	The z component of the new vector
	*/
	public Vector3d(double x, double y, double z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}

	//################## PUBLIC METHODS ##################//
	public Vector3d add(final Vector3d rhs)
	{
		x+=rhs.x;
		y+=rhs.y;
		z+=rhs.z;
		return this;
	}

	public Vector3d clone()
	{
		return new Vector3d(x, y, z);
	}

	/**
	Returns the cross product of this vector and the parameter vector.

	@param rhs	The right-hand operand of the cross product
	@return		The cross product of the two vectors as a Vector3d
	*/
	public Vector3d cross(final Vector3d rhs)
	{
		return new Vector3d(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}

	/**
	Returns the result of the cross product lhs x rhs.

	@param lhs	The left-hand operand of the cross product
	@param rhs	The right-hand operand of the cross product
	@return		The cross product of the two vectors as a Vector3d
	*/
	public static Vector3d cross(final Vector3d lhs, final Vector3d rhs)
	{
		return lhs.cross(rhs);
	}

	public double distance(final Vector3d rhs)
	{
		return Math.sqrt(distance_squared(rhs));
	}

	public double distance_squared(final Vector3d rhs)
	{
		double dx = x - rhs.x, dy = y - rhs.y, dz = z - rhs.z;
		return dx*dx + dy*dy + dz*dz;
	}

	public double dot(final Vector3d rhs)
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	public boolean equals(final Vector3d rhs)
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}

	public double length()
	{
		return Math.sqrt(x*x + y*y + z*z);	// note that the squared length has been put inline for efficiency
	}

	public double length_squared()
	{
		return x*x + y*y + z*z;				// note that dot(this); would do, but it's needlessly inefficient
	}

	public Vector3d negate()
	{
		x = -x;
		y = -y;
		z = -z;
		return this;
	}

	public Vector3d negated()
	{
		return new Vector3d(-x, -y, -z);
	}

	public Vector3d normalize()
	{
		double len = length();
		if(Math.abs(len) < SMALL_EPSILON) throw new java.lang.Error();

		scale(1.0/len);
		return this;
	}

	public Vector3d scale(double factor)
	{
		x*=factor;
		y*=factor;
		z*=factor;
		return this;
	}

	/**
	Returns the result of the operation factor*u + v.

	@param factor	The scaling factor for u
	@param u		The vector to scale
	@param v		The vector to add
	@return			...think about it...
	*/
	public static Vector3d scale_add(double factor, final Vector3d u, final Vector3d v)
	{
		return new Vector3d(factor*u.x + v.x, factor*u.y + v.y, factor*u.z + v.z);
	}

	public Vector3d subtract(final Vector3d rhs)
	{
		x-=rhs.x;
		y-=rhs.y;
		z-=rhs.z;
		return this;
	}

	public String toString()
	{
		StringBuffer sb = new StringBuffer("(");
		sb.append(x + ", ");
		sb.append(y + ", ");
		sb.append(z + ")");
		return sb.toString();
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_add()
		{
			output(new Vector3d(1,2,3).add(new Vector3d(4,5,6)), new Vector3d(5,7,9).toString());
			output(new Vector3d(23,9,84).add(new Vector3d(-17,-10,-51)), new Vector3d(6,-1,33).toString());
		}

		public void test_clone()
		{
			Vector3d v = new Vector3d(17,10,51);
			Vector3d w = v.clone();
			output(w, v.toString());

			v = new Vector3d(1,2,3);
			output(w, new Vector3d(17,10,51).toString());		// demonstrate that w remains unchanged even though v has changed
		}

		public void test_distance()
		{
			output(new Vector3d(1,2,3).distance(new Vector3d(1,2,3)), "0.0");
			output(new Vector3d(1,2,3).distance(new Vector3d(4,6,3)), "5.0");
			output(new Vector3d(23,9,84).distance(new Vector3d(23,2,60)), "25.0");
		}

		public void test_distance_squared()
		{
			output(new Vector3d(1,2,3).distance_squared(new Vector3d(1,2,3)), "0.0");
			output(new Vector3d(1,2,3).distance_squared(new Vector3d(4,6,3)), "25.0");
			output(new Vector3d(23,9,84).distance_squared(new Vector3d(23,2,60)), "625.0");
		}

		public void test_dot()
		{
			output(new Vector3d(1,2,3).dot(new Vector3d(4,5,6)), "32.0");
		}

		public void test_equals()
		{
			// Test reflexivity.
			Vector3d v = new Vector3d(23,9,84);
			output(v.equals(v), "true");

			// Test equality.
			Vector3d w = new Vector3d(23,9,84);
			output(v.equals(w), "true");
			output(w.equals(v), "true");

			// Test inequality.
			Vector3d x = new Vector3d(17,10,51);
			output(v.equals(x), "false");
			output(x.equals(v), "false");
		}

		public void test_length()
		{
			output(new Vector3d(0,0,0).length(), "0.0");
			output(new Vector3d(1,0,0).length(), "1.0");
			output(new Vector3d(0,-23,0).length(), "23.0");
			output(new Vector3d(0,3,-4).length(), "5.0");
			output(new Vector3d(1,2,2).length(), "3.0");
		}

		public void test_length_squared()
		{
			output(new Vector3d(0,0,0).length_squared(), "0.0");
			output(new Vector3d(1,0,0).length_squared(), "1.0");
			output(new Vector3d(0,-23,0).length_squared(), "529.0");
			output(new Vector3d(0,3,-4).length_squared(), "25.0");
			output(new Vector3d(1,2,2).length_squared(), "9.0");
		}

		public void test_negate()
		{
			Vector3d v = new Vector3d(23,9,84);
			output(v.negate(), new Vector3d(-23,-9,-84).toString());
			output(v, new Vector3d(-23,-9,-84).toString());		// demonstrate that v has changed as a result of the negation
		}

		public void test_negated()
		{
			Vector3d v = new Vector3d(7,8,51);
			output(v.negated(), new Vector3d(-7,-8,-51).toString());
			output(v, new Vector3d(7,8,51).toString());			// demonstrate that v hasn't changed
		}

		public void test_normalize()
		{
			output(new Vector3d(23,0,0).normalize(),	"(1.0, 0.0, 0.0)");
			output(new Vector3d(0,-9,0).normalize(),	"(0.0, -1.0, 0.0)");
			output(new Vector3d(0,0,84).normalize(),	"(0.0, 0.0, 1.0)");
			output(new Vector3d(-3,4,0).normalize(),	"(-0.6000000000000001, 0.8, 0.0)");		// close enough to (-0.6, 0.8, 0.0)
			output(new Vector3d(0,7,-24).normalize(),	"(0.0, 0.28, -0.96)");
			output(new Vector3d(1,2,2).normalize(),		"(0.3333333333333333, 0.6666666666666666, 0.6666666666666666)");
		}

		public void test_scale()
		{
			output(new Vector3d(1,2,3).scale(2),		new Vector3d(2,4,6).toString());
			output(new Vector3d(23,9,84).scale(0),		new Vector3d(0,0,0).toString());
			output(new Vector3d(17,10,51).scale(-1),	new Vector3d(-17,-10,-51).toString());
		}

		public void test_scale_add()
		{
			Vector3d u = new Vector3d(1,2,3);
			Vector3d v = new Vector3d(7,8,51);
			output(scale_add(3,u,v), new Vector3d(10,14,60).toString());
			output(u, new Vector3d(1,2,3).toString());			// demonstrate that u hasn't changed
			output(v, new Vector3d(7,8,51).toString());			// demonstrate that v hasn't changed
		}

		public void test_subtract()
		{
			output(new Vector3d(4,5,6).subtract(new Vector3d(1,2,3)), new Vector3d(3,3,3).toString());
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}