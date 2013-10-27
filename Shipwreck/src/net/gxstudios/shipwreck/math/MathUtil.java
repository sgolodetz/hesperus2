package net.gxstudios.shipwreck.math;

import net.gxstudios.shipwreck.math.vectors.Vector2d;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.math.vectors.VectorUtil;
import net.gxstudios.shipwreck.misc.Constants;
import net.gxstudios.shipwreck.misc.Pair;
import net.gxstudios.shipwreck.misc.Triple;
import net.gxstudios.shipwreck.test.TestHarnessAdapter;

public class MathUtil implements Constants
{
	/**
	Calculates the normal of the polygon defined by the vertices v1, v2 and v3 (in anticlockwise winding order).

	<p><b>Preconditions:</b>
	<dl>
	<dd>v1 != null && v2 != null && v3 != null
	<dd>The vertices must be mutually distinct
	</dl>

	@param v1	The first vertex of the polygon
	@param v2	The second vertex of the polygon
	@param v3	The third vertex of the polygon
	@return		The normal of the polygon as a Vector3d
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static Vector3d calculate_normal(final Vector3d v1, final Vector3d v2, final Vector3d v3)
	{
		if(v1 == null || v2 == null || v3 == null) throw new java.lang.Error();

		Vector3d l = v2.clone(), r = v3.clone();
		l.subtract(v1);		// l = v2-v1
		r.subtract(v1);		// r = v3-v1

		Vector3d normal = l.cross(r);

		if(normal.length() == 0) throw new java.lang.Error();	// the vertices weren't mutually distinct

		normal.normalize();

		return normal;
	}

	/**
	Rotates vector v anticlockwise about the specified axis by the specified angle (in degrees).

	<p><b>Preconditions:</b>
	<dl>
	<dd>v != null
	<dd>axis != null
	<dd>axis must be normalized
	</dl>

	@param v			The vector to rotate about the axis
	@param degreeAngle	The angle by which to rotate it (in degrees)
	@param axis			The axis about which to rotate it
	@return				A (new) vector containing the result of the rotation
	@throws java.lang.Error	If any of the preconditions are not met
	*/
	public static Vector3d rotate_about_axis(final Vector3d v, final double degreeAngle, final Vector3d axis)
	{
		// Check the preconditions.
		if(v == null || axis == null) throw new java.lang.Error();
		if(Math.abs(axis.length() - 1) >= EPSILON) throw new java.lang.Error();	// make sure axis is normalized

		// Main algorithm
		double radianAngle = degreeAngle*Math.PI/180.0;
		double cosAngle = Math.cos(radianAngle), sinAngle = Math.sin(radianAngle);
		Vector3d aCROSSv = axis.cross(v);

		// ret = v cos radianAngle + (axis x v) sin radianAngle + axis(axis . v)(1 - cos radianAngle)
		// (See Mathematics for 3D Game Programming and Computer Graphics, P.62, for details of why this is (it's not very hard)).
		Vector3d ret = v.clone();
		ret.scale(cosAngle);
		ret = Vector3d.scale_add(sinAngle, aCROSSv, ret);
		ret = Vector3d.scale_add(axis.dot(v) * (1 - cosAngle), axis, ret);
		return ret;
	}

	/**
	Calculates the (signed) angle (in radians) between the two specified 2D vectors. The signed angle is
	measured anticlockwise from u, e.g. the method would return PI/2 if v lay along the anticlockwise
	rotation of u through PI/2 radians.

	@param u	...think about it...
	@param v	...think about it...
	@return		The signed, radian angle between u and v, as specified, in the range (-PI,PI]
	*/
	public static double signed_angle_between(final Vector2d u, final Vector2d v)
	{
		Vector3d u3D = new Vector3d(u.x, u.y, 0), v3D = new Vector3d(v.x, v.y, 0);

		double angle = VectorUtil.angle_between(u3D, v3D);

		Vector3d uCROSSv = u3D.cross(v3D);
		Vector3d n = new Vector3d(0,0,1);
		angle *= uCROSSv.dot(n) >= 0 ? 1 : -1;

		return angle;
	}

	/**
	Finds the distinct real roots of the cubic equation ax^3 + bx^2 + cx + d = 0.

	@param a	The coefficient of x^3 in the equation
	@param b	The coefficient of x^2 in the equation
	@param c	The coefficient of x in the equation
	@param d	The constant coefficient in the equation
	@return		A pair, the first component of which is an integer indicating how many distinct real roots
				the cubic had and the second component of which is a triple containing the roots. If there's
				one root, it's stored in the first component of the triple, if there are two they're stored
				in the first and second components of the triple, and obviously if there are three they're
				stored in all three components of the triple. The unused triple components are guaranteed
				to be Double.NaN.
	@throws java.lang.Error	If a, b and c are all too close to zero (because solve_quadratic equation does)
	*/
	public static Pair<Integer,Triple<Double,Double,Double>> solve_cubic_equation(double a, double b, double c, double d)
	{
		// Handle quadratic and linear equations.
		if(Math.abs(a) < SMALL_EPSILON)
		{
			Pair<Integer,Pair<Double,Double>> result = solve_quadratic_equation(b,c,d);
			return Pair.make_pair(result.first, Triple.make_triple(result.second.first, result.second.second, Double.NaN));
		}

		// Note: The algorithm is derived from its description by R.W.D. Nickalls in the Mathematical Gazette (1993, Vol. 77).
		// Not that I read the Mathematical Gazette, you understand, but that doesn't mean I can't find articles from it
		// using Google!
		double xN = -b/(3*a);
		double yN = ((a*xN + b)*xN + c)*xN + d;		// == a*xN*xN*xN + b*xN*xN + c*xN + d (I used Horner's rule to make it more efficient)
		double deltaSquared = (b*b - 3*a*c)/(9*a*a);
		double hSquared = 4*a*a*deltaSquared*deltaSquared*deltaSquared;	// 4a^2(delta^2)^3 = (2a.delta^3)^2 = h^2

		double discriminant = yN*yN - hSquared;

		if(discriminant > SMALL_EPSILON)
		{
			// There's only one real root of the cubic.
			double sqrtDiscriminant = Math.sqrt(discriminant);
			return Pair.make_pair(1, Triple.make_triple(xN + Math.cbrt((-yN + sqrtDiscriminant)/(2*a)) + Math.cbrt((-yN - sqrtDiscriminant)/(2*a)),
														Double.NaN, Double.NaN));
		}
		else if(discriminant < -SMALL_EPSILON)
		{
			// There are three distinct real roots.
			double h = Math.sqrt(hSquared);
			double delta = Math.cbrt(h/(2*a));	// CHECKME: old version was Math.sqrt(deltaSquared);
			double theta = Math.acos(-yN/h)/3;
			return Pair.make_pair(3, Triple.make_triple(xN + 2*delta*Math.cos(theta),
														xN + 2*delta*Math.cos(2*Math.PI/3 + theta),
														xN + 2*delta*Math.cos(4*Math.PI/3 + theta)));
		}
		else
		{
			double h = Math.sqrt(hSquared);
			if(h != 0)
			{
				// There are two equal roots. The roots are x = xN + delta, x = xN + delta and x = xN - 2*delta.
				double delta = Math.cbrt(yN/(2*a));	// note that the sign of delta is critical - we can't just square root deltaSquared
				return Pair.make_pair(2, Triple.make_triple(xN + delta, xN - 2*delta, Double.NaN));
			}
			else
			{
				// If yN = h = 0, then delta = 0, in which case there are three equal roots at x = xN, which, since
				// we're only returning distinct roots, we'll return as a single root at x = xN.
				return Pair.make_pair(1, Triple.make_triple(xN, Double.NaN, Double.NaN));
			}
		}
	}

	/**
	Finds the root of the linear equation ax + b = 0.

	@param a				The coefficient of x in the equation
	@param b				The constant coefficient in the equation
	@return					The root of the linear equation
	@throws java.lang.Error	If a is too close to zero
	*/
	public static double solve_linear_equation(double a, double b)
	{
		if(Math.abs(a) < SMALL_EPSILON) throw new java.lang.Error();
		else return -b/a;
	}

	/**
	Finds the distinct real roots of the quadratic equation ax^2 + bx + c = 0.

	@param a	The coefficient of x^2 in the equation
	@param b	The coefficient of x in the equation
	@param c	The constant coefficient in the equation
	@return		A pair, the first component of which is an integer indicating how many distinct real roots
				the quadratic had and the second component of which is a pair containing the roots. If there's
				one root, it's stored in the first component of the pair, if there are two then both components
				are used. If there are no roots, none of the components are used (obviously). The unused components
				are always guaranteed to be Double.NaN.
	@throws java.lang.Error	If a and b are both too close to zero (because solve_linear_equation does)
	*/
	public static Pair<Integer,Pair<Double,Double>> solve_quadratic_equation(double a, double b, double c)
	{
		// Handle linear equations.
		if(Math.abs(a) < SMALL_EPSILON) return Pair.make_pair(1, Pair.make_pair(solve_linear_equation(b,c), Double.NaN));

		double discriminant = b*b-4*a*c;

		if(discriminant > SMALL_EPSILON)
		{
			// There are two distinct roots.
			double sqrtDiscriminant = Math.sqrt(discriminant);
			return Pair.make_pair(2, Pair.make_pair((-b+sqrtDiscriminant)/(2*a), (-b-sqrtDiscriminant)/(2*a)));
		}
		else if(discriminant < -SMALL_EPSILON)
		{
			// There are no roots.
			return Pair.make_pair(0, Pair.make_pair(Double.NaN, Double.NaN));
		}
		else
		{
			// There's a single root.
			return Pair.make_pair(1, Pair.make_pair(-b/(2*a), Double.NaN));
		}
	}

	/**
	Solves, if possible, a pair of simultaneous equations. The equations are given by:

	a1*x + b1*y = c1
	a2*x + b2*y = c2

	@param a1	The coefficient of x in the first equation
	@param b1	The coefficient of y in the first equation
	@param c1	The constant in the first equation
	@param a2	The coefficient of x in the second equation
	@param b2	The coefficient of y in the second equation
	@param c2	The constant in the second equation
	@return		The pair (x,y) as a Pair<Double,Double>, if there was a unique solution, or null otherwise
	*/
	public static Pair<Double,Double> solve_simultaneous_equation_pair(double a1, double b1, double c1, double a2, double b2, double c2)
	{
		/*
		We solve the equations using a matrix method:

		|a1 b1|*|x| = |c1|
		|a2 b2| |y|   |c2|

		|x| = |a1 b1|^-1 * |c1|
		|y|   |a2 b2|      |c2|

		Well inverting |a1 b1| gives us 1/(a1*b2 - b1*a2) |b2  -b1|, provided a1*b2 - b1*a2 != 0.
		               |a2 b2|                            |-a2  a1|

		Hence x = 1/(a1*b2 - b1*a2) * (b2*c1 - b1*c2) and y = 1/(a1*b2 - b1*a2) * (a1*c2 - a2*c1),
		provided the above condition is met. If not, there is no unique solution, since the matrix
		isn't invertible.
		*/

		double denom = a1*b2 - b1*a2;

		if(denom != 0)
		{
			double inverseDenom = 1/denom;
			return Pair.make_pair(inverseDenom * (b2*c1 - b1*c2), inverseDenom * (a1*c2 - a2*c1));
		}
		else return null;
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_rotate_about_axis()
		{
			// Use the simplest possible axis for our first few test cases.
			Vector3d axis_a = new Vector3d(0,0,1);

			// The result is near enough to (0,1,0).
			output(rotate_about_axis(new Vector3d(1,0,0), 90.0, axis_a), "(6.123233995736766E-17, 1.0, 0.0)");

			// The result is near enough to (1/sqrt(2), -1/sqrt(2), 23). Note that the z coordinate will always
			// be unchanged by this rotation, whatever it starts out as.
			output(rotate_about_axis(new Vector3d(1,0,23), -45.0, axis_a), "(0.7071067811865476, -0.7071067811865475, 23.0)");

			// The result is near enough to (-1,0,0).
			output(rotate_about_axis(new Vector3d(0,1,0), 90.0, axis_a), "(-1.0, 6.123233995736766E-17, 0.0)");

			// Note that the angle is irrelevant on this one, since the vector lies on the axis.
			output(rotate_about_axis(new Vector3d(0,0,1), 23.9847851171051, axis_a), "(0.0, 0.0, 1.0)");

			// Let's try a more interesting axis.
			Vector3d axis_b = new Vector3d(1,1,0);
			axis_b.normalize();

			// The result is near enough to (0,1,0).
			output(rotate_about_axis(new Vector3d(1,0,0), 180.0, axis_b), "(-2.220446049250313E-16, 0.9999999999999998, -8.659560562354932E-17)");
			
			// The result is near enough to (0.5, 0.5, 1/sqrt(2)).
			output(rotate_about_axis(new Vector3d(1,0,0), -90.0, axis_b), "(0.4999999999999999, 0.49999999999999983, 0.7071067811865475)");

			// Finally, let's try a few error cases.

			// The vector being rotated is null
			try
			{
				output(rotate_about_axis(null, 0.0, axis_b), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// The axis is null
			try
			{
				output(rotate_about_axis(new Vector3d(1,0,0), 0.0, null), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// The axis isn't normalized
			try
			{
				output(rotate_about_axis(new Vector3d(1,0,0), 0.0, new Vector3d(0,0,2)), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// TESTME: More tests might be useful.
		}

		public void test_signed_angle_between()
		{
			output(signed_angle_between(new Vector2d(1,0),	new Vector2d(1,0)),		"0.0");
			output(signed_angle_between(new Vector2d(1,0),	new Vector2d(1,1)),		"0.7853981633974484");
			output(signed_angle_between(new Vector2d(1,1),	new Vector2d(1,0)),		"-0.7853981633974484");
			output(signed_angle_between(new Vector2d(1,0),	new Vector2d(1,-1)),	"-0.7853981633974484");
			output(signed_angle_between(new Vector2d(1,0),	new Vector2d(0,1)),		"1.5707963267948966");
			output(signed_angle_between(new Vector2d(1,0),	new Vector2d(-1,0)),	"3.141592653589793");
			output(signed_angle_between(new Vector2d(-1,0),	new Vector2d(1,0)),		"3.141592653589793");
			output(signed_angle_between(new Vector2d(1,0),	new Vector2d(-1,-1)),	"-2.356194490192345");
		}

		public void test_solve_cubic_equation()
		{
			// I) Test some very simple cubics first, of the form x^3 + k = 0. We expect cbrt(-k) as the only solution.
			output(solve_cubic_equation(1,0,0,0), "(1, (-0.0, NaN, NaN))");
			output(solve_cubic_equation(1,0,0,1), "(1, (-1.0, NaN, NaN))");
			for(int k=2; k<5; ++k) output(solve_cubic_equation(1,0,0,k), "(1, (" + Math.cbrt(-k) + ", NaN, NaN))");

			// II) Now test cubics of the form x^3 + kx = 0. Obviously, since this factorises to x(x^2 + k) = 0,
			// x = 0 is an expected solution and there will only be other expected solutions when k < 0,
			// when the solutions will be +/- sqrt(-k).
			output(solve_cubic_equation(1,0,0,0), "(1, (-0.0, NaN, NaN))");
			for(int k=1; k<5; ++k) output(solve_cubic_equation(1,0,k,0), "(1, (0.0, NaN, NaN))");

			// The result's near enough (2.0, -2.0, 0.0).
			output(solve_cubic_equation(1,0,-4,0), "(3, (2.0, -1.9999999999999996, -4.242300954899627E-16))");

			// The result's near enough (3.0, -3.0, 0.0).
			output(solve_cubic_equation(1,0,-9,0), "(3, (3.0, -2.999999999999999, -6.363451432349441E-16))");

			// III) Next test some cubics of the form x^3 + qx + r = 0.
			output(solve_cubic_equation(1,0,23,9), "(1, (-0.3887499807630337, NaN, NaN))");
			output(solve_cubic_equation(1,0,7,8), "(1, (-0.9999999999999996, NaN, NaN))");
			output(solve_cubic_equation(1,0,17,10), "(1, (-0.5769388825777177, NaN, NaN))");

			// The equation is equivalent to x^3 + 24x + 12 = 0.
			output(solve_cubic_equation(2,0,48,24), "(1, (-0.4949479533002221, NaN, NaN))");

			// IV) Next test some cubics of the form x^3 + px^2 = 0. Obviously, since this factorises
			// to x^2.(x + p) = 0, the expected solutions are x = 0 and x = -p.
			output(solve_cubic_equation(1,3,0,0), "(2, (0.0, -3.0, NaN))");
			output(solve_cubic_equation(1,-7,0,0), "(2, (0.0, 7.0, NaN))");

			// The equation is equivalent to x^3 + (3/5)x^2 = 0.
			output(solve_cubic_equation(5,3,0,0), "(2, (0.0, -0.6000000000000001, NaN))");

			// The equation is equivalent to x^3 + (2/9)x^2 = 0.
			output(solve_cubic_equation(9,2,0,0), "(2, (0.0, -0.2222222222222222, NaN))");

			// V) Next test some cubics of the form x^3 + px^2 + r = 0.
			output(solve_cubic_equation(1,23,0,9), "(1, (-23.016988127899374, NaN, NaN))");

			// The equation is equivalent to x^3 + 17x^2 + 10 = 0.
			output(solve_cubic_equation(2,34,0,20), "(1, (-17.03446221167481, NaN, NaN))");

			// VI) Next test some cubics of the form x^3 + px^2 + qx = 0. Obviously, since this factorises
			// to x(x^2 + px + q) = 0, we expect x = 0 to be one solution and the other solutions to be
			// (-p +/- sqrt(p^2 - 4q))/2.

			// The result's near enough (21.0, 0.0, 4.0).
			output(solve_cubic_equation(1,-25,84,0), "(3, (21.0, 1.7763568394002505E-15, 3.9999999999999964))");

			// The result's near enough (-1.0, 0.0, NaN).
			output(solve_cubic_equation(3,6,3,0), "(2, (-1.0, 1.1102230246251565E-16, NaN))");

			// VII) Test some general cubics and others.
			output(solve_cubic_equation(1,-7,14,-8), "(3, (3.9999999999999996, 0.9999999999999989, 2.000000000000001))");
			output(solve_cubic_equation(1,23,9,84), "(1, (-22.766747163216664, NaN, NaN))");
			output(solve_cubic_equation(1,7,-8,51), "(1, (-8.615612044918578, NaN, NaN))");
			output(solve_cubic_equation(1,-17,-10,51), "(3, (17.40617795839254, -1.9268172950609994, 1.5206393366684523))");
			output(solve_cubic_equation(1,24,-12,-18), "(3, (1.1184492788325109, -24.460502388549713, -0.6579468902828038))");
			output(solve_cubic_equation(-23,9,-84,1), "(1, (0.011919520535967898, NaN, NaN))");

			output(solve_cubic_equation(-349,823,6,0), "(3, (-0.007268000586200141, 2.365434189697957, -8.881784197001252E-15))");
			output(solve_cubic_equation(-239,214,15,1), "(1, (0.9649335332016219, NaN, NaN))");
			output(solve_cubic_equation(-1710,708,51,0), "(3, (-0.0625762707377755, 0.47661135845707364, 2.220446049250313E-16))");

			// VIII) Test some cases which aren't actually cubic equations (they're quadratic or linear). This is merely
			// to make sure the correct functions are called in such situations. Serious testing of the quadratic and
			// linear solvers is done separately.
			output(solve_cubic_equation(0,1,0,1), "(0, (NaN, NaN, NaN))");
			output(solve_cubic_equation(0,1,2,1), "(1, (-1.0, NaN, NaN))");
			output(solve_cubic_equation(0,1,-3,2), "(2, (2.0, 1.0, NaN))");
			output(solve_cubic_equation(0,0,7,-21), "(1, (3.0, NaN, NaN))");

			// TESTME: The more tests here the better, the algorithm's quite a complex one.
		}

		public void test_solve_linear_equation()
		{
			// Test a couple of simple cases.
			output(solve_linear_equation(5,10), "-2.0");
			output(solve_linear_equation(4,-21), "5.25");

			// Test an error case.
			try
			{
				output(solve_linear_equation(0,23), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}
		}

		public void test_solve_quadratic_equation()
		{
			// Test some cases with no roots.
			output(solve_quadratic_equation(23,0,9), "(0, (NaN, NaN))");
			output(solve_quadratic_equation(1,5,7), "(0, (NaN, NaN))");

			// Test some cases with only one root.
			output(solve_quadratic_equation(1,4,4), "(1, (-2.0, NaN))");
			output(solve_quadratic_equation(3,6,3), "(1, (-1.0, NaN))");

			// Test some cases with two distinct roots.
			output(solve_quadratic_equation(1,4,1), "(2, (-0.2679491924311228, -3.732050807568877))");
			output(solve_quadratic_equation(23,1000,9), "(2, (-0.009001863771681716, -43.46925900579354))");

			// Test some cases which aren't actually quadratic equations (they're linear).
			output(solve_quadratic_equation(0,9,-45), "(1, (5.0, NaN))");
			output(solve_quadratic_equation(0,6,18), "(1, (-3.0, NaN))");

			// TESTME: More tests could be done here, but I'm pretty sure it works.
		}

		public void test_solve_simultaneous_equation_pair()
		{
			// Try some nice simple cases first.

			// x = 23, y = 9 (each equation has only one variable)
			Pair<Double,Double> result = solve_simultaneous_equation_pair(1, 0, 23, 0, 1, 9);
			output(result, "(23.0, 9.0)");

			// y = 7, x = 8 (swap the equation order)
			result = solve_simultaneous_equation_pair(0, 1, 7, 1, 0, 8);
			output(result, "(8.0, 7.0)");

			// x = 17, x + y = 10 (one equation has only x)
			result = solve_simultaneous_equation_pair(1, 0, 17, 1, 1, 10);
			output(result, "(17.0, -7.0)");

			// y = 24, x - y = 12 (one equation has only y)
			result = solve_simultaneous_equation_pair(0, 1, 24, 1, -1, 12);
			output(result, "(36.0, 24.0)");

			// x + 2y = 5, 7x - 2y = 3 (general case)
			result = solve_simultaneous_equation_pair(1, 2, 5, 7, -2, 3);
			output(result, "(1.0, 2.0)");

			// 2x + y = 46, x - y = 17 (general case)
			result = solve_simultaneous_equation_pair(2, 1, 46, 1, -1, 17);
			output(result, "(21.0, 4.0)");

			// Now let's try some cases we'd expect not to have unique solutions.

			// x = 5, x = 10
			result = solve_simultaneous_equation_pair(1, 0, 5, 1, 0, 10);
			output(result, "null");

			// x + 2y = 5, 2x + 4y = 12
			result = solve_simultaneous_equation_pair(1, 2, 5, 2, 4, 12);
			output(result, "null");

			// x + y = 23, 0 = 1 (a bit extreme!)
			result = solve_simultaneous_equation_pair(1, 1, 23, 0, 0, 1);
			output(result, "null");

			// TESTME: More tests could be done here, but I'm pretty sure it works.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}