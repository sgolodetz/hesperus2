package net.gxstudios.shipwreck.geom.splines;

import net.gxstudios.shipwreck.math.matrices.Matrix;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.misc.Constants;
import net.gxstudios.shipwreck.test.TestHarnessAdapter;

/**
This class represents uniform cubic B-splines.
*/
public class CubicBSpline implements Constants
{
	//################## PRIVATE VARIABLES ##################//
	// Datatype Invariant: m_controlPoints[i] = d[i-3], where d[j] is a control point,
	// as in d[-3]*N{-3,3}(0) + d[-2]*N[-2,3}(0) + ...
	private Vector3d[] m_controlPoints;

	//################## CONSTRUCTORS ##################//
	private CubicBSpline() {}	// for internal use

	//################## PUBLIC METHODS ##################//
	/**
	Calculates the point on the spline with the specified parametric value (note that the spline
	is parametrised so that t = 0 at one end and t = 1 at the other).

	<p><b>Preconditions:</b>
	<dl>
	<dd>The value of t must be s.t. 0 <= t <= 1 (to within a given tolerance)
	</dl>

	@param t				A value of the spline parameter
	@return					The corresponding point on the spline
	@throws java.lang.Error	If the preconditions are violated
	*/
	public Vector3d calculate_curve_point(double t)
	{
		// Check the preconditions. If t is negligibly close to 0 or 1, but not
		// technically in the range [0,1], clamp it to the range. This helps us
		// be tolerant of minor precision errors (floating-point arithmetic isn't
		// perfect, after all).
		if(t < 0)
		{
			if(t > -SMALL_EPSILON) t = 0;
			else throw new java.lang.Error(String.valueOf(t));
		}
		else if(t > 1)
		{
			if(t < 1+SMALL_EPSILON) t = 1;
			else throw new java.lang.Error(String.valueOf(t));
		}

// TODO: This can be optimized if necessary (and it might well be necessary, this will get called a lot).
// In particular, I'm talking about it being possible to get rid of the N(...) function calls and put
// them inline.

		t*=(m_controlPoints.length-3);

		int loKnot = (int)t, hiKnot = loKnot+3;
		if(hiKnot >= m_controlPoints.length) hiKnot = m_controlPoints.length-1;

		Vector3d ret = new Vector3d();
		for(int i=loKnot; i<=hiKnot; ++i)
		{
			ret = Vector3d.scale_add(N(i-3,t), m_controlPoints[i], ret);
		}
		return ret;
	}

	/**
	Constructs a spline that interpolates the specified points, given two additional specified constraints.

	@param data			The points we want to interpolate
	@param constraint1	The first additional constraint (gives us an extra equation, since the points aren't sufficient
						to uniquely determine a spline)
	@param constraint2	The second additional constraint
	@return				The spline, or null if it couldn't be constructed because the constraints were invalid (which would
						result in us not being able to invert the matrix)
	*/
	public static CubicBSpline construct_interpolating_spline(final Vector3d[] data, IConstraint constraint1, IConstraint constraint2)
	{
		int n = data.length+2;

		Matrix m = construct_spline_equation_matrix(n, constraint1, constraint2);
		try
		{
			/*System.out.println(m.inverse());
			for(int i=0; i<data.length; ++i) System.out.println(data[i]);
			System.out.println(constraint1.determine_value(data));
			System.out.println(constraint2.determine_value(data));*/

			double[][] elements = m.inverse().get_elements();

			CubicBSpline spline = new CubicBSpline();
			spline.m_controlPoints = new Vector3d[n];
			for(int r=0; r<n; ++r)
			{
				//System.out.println("---");
				spline.m_controlPoints[r] = new Vector3d();
				for(int c=0; c<n-2; ++c)
				{
					spline.m_controlPoints[r] = Vector3d.scale_add(elements[r][c], data[c], spline.m_controlPoints[r]);
					//System.out.println(spline.m_controlPoints[r]);
				}
				spline.m_controlPoints[r] = Vector3d.scale_add(elements[r][n-2], constraint1.determine_value(data), spline.m_controlPoints[r]);
				//System.out.println(spline.m_controlPoints[r]);
				spline.m_controlPoints[r] = Vector3d.scale_add(elements[r][n-1], constraint2.determine_value(data), spline.m_controlPoints[r]);
				//System.out.println(spline.m_controlPoints[r]);
			}

			/*System.out.println("***");
			for(int i=0; i<n; ++i)
			{
				System.out.println(spline.m_controlPoints[i]);
			}*/

			return spline;
		}
		catch(Exception e) { return null; }
	}

	/**
	Returns the spline's control points.

	@return	...think about it...
	*/
	public Vector3d[] get_control_points()
	{
		return m_controlPoints;
	}

	//################## PRIVATE METHODS ##################//
	/**
	Constructs the spline equation matrix that we need to invert to interpolate a given set of points.

	@param n			The size of the matrix to construct
	@param constraint1	The first additional constraint
	@param constraint2	The second additional constraint
	@return				The matrix we'll need to invert, as specified
	*/
	private static Matrix construct_spline_equation_matrix(int n, IConstraint constraint1, IConstraint constraint2)
	{
		double[][] elements = new double[n][];

		for(int r=0; r<n-2; ++r)
		{
			elements[r] = new double[n];
			elements[r][r] = elements[r][r+2] = 1.0 / 6.0;
			elements[r][r+1] = 2.0 / 3.0;
		}

		elements[n-2] = constraint1.determine_coefficients(n);
		elements[n-1] = constraint2.determine_coefficients(n);

		return new Matrix(elements);
	}

	/**
	Constructs a spline from an (ordered) array of its control points.

	@param controlPoints	The control points for the spline
	@return					The spline, as specified
	*/
	private static CubicBSpline construct_spline_from_control_points(Vector3d[] controlPoints)	// for internal use (at least at present)
	{
		CubicBSpline spline = new CubicBSpline();
		spline.m_controlPoints = controlPoints;
		return spline;
	}

	/**
	The cubic spline basis function N{a,3}(t).

	@param a	Which curve of the basis
	@param t	The value of the spline parameter
	@return		...think about it...
	*/
	private static double N(int a, double t)
	{
		if(t <= a || a+4 <= t) return 0;
		else if(t < a+1) return Math.pow(t-a,3)/6;
		else if(t < a+2) return Math.pow(t-a,3)/6 - 2*Math.pow((t-a)-1,3)/3;
		else if(t < a+3) return Math.pow(4-(t-a),3)/6 - 2*Math.pow(4-(t-a)-1,3)/3;
		else return Math.pow(4-(t-a),3)/6;
	}

	//################## NESTED CLASSES ##################//
	/**
	This is the interface for a constraint needed when constructing an interpolating spline.
	*/
	public interface IConstraint
	{
		/**
		Determines a row of the spline equation matrix corresponding to this constraint.
		*/
		double[] determine_coefficients(int n);

		/**
		Determines the result of multiplying the above row by the spline control points.
		*/
		Vector3d determine_value(Vector3d[] data);
	}

	/**
	This class allows us to derive a gradient constraint which requires the
	gradient at control point i to be a certain vector.
	*/
	public static abstract class GradientConstraint implements IConstraint
	{
		private int i;

		public GradientConstraint(int i)	{ this.i = i; }

		final public double[] determine_coefficients(int n)
		{
			double[] ret = new double[n];
			ret[i] = -0.5;
			ret[i+2] = 0.5;
			return ret;
		}
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_calculate_curve_point()
		{
			// We isolate these tests from how the spline is constructed in practice by explicitly giving
			// a list of control points.

			CubicBSpline spline = construct_spline_from_control_points(new Vector3d[]
			{
				new Vector3d(-23,0,9),
				new Vector3d(-7,0,8),
				new Vector3d(17,0,-10),
				new Vector3d(24,0,-12),
				new Vector3d(32,0,-8),
				new Vector3d(42,0,0)
			});

			// Test the obvious points first.
			output(spline.calculate_curve_point(0), "(-5.666666666666667, 0.0, 5.166666666666666)");
			output(spline.calculate_curve_point(1.0/3), "(14.166666666666666, 0.0, -7.333333333333333)");
			output(spline.calculate_curve_point(2.0/3), "(24.166666666666664, 0.0, -11.0)");
			output(spline.calculate_curve_point(1), "(32.33333333333333, 0.0, -7.333333333333333)");

			// Now test a couple of arbitrary points.
			output(spline.calculate_curve_point(0.25), "(9.825520833333332, 0.0, -4.419270833333333)");
			output(spline.calculate_curve_point(0.5), "(20.166666666666668, 0.0, -10.541666666666666)");
			output(spline.calculate_curve_point(0.75), "(26.075520833333332, 0.0, -10.567708333333332)");

			// Finally, test some expected error cases.
			try				{ output(spline.calculate_curve_point(-SMALL_EPSILON), "Error"); }
			catch(Error e)	{ output("", ""); }

			try				{ output(spline.calculate_curve_point(1+SMALL_EPSILON), "Error"); }
			catch(Error e)	{ output("", ""); }

			// TESTME: More tests would be helpful.
		}

		public void test_construct_interpolating_spline()
		{
			// Test a simple case first.

			Vector3d[] data = new Vector3d[]
			{
				new Vector3d(0,0,0),
				new Vector3d(1,0,2),
				new Vector3d(2,0,1),
				new Vector3d(3,0,5)
			};
			IConstraint constraint1 = new GradientConstraint(0)
			{
				public Vector3d determine_value(Vector3d[] data)
				{
					return new Vector3d(1,0,0);
				}
			};
			IConstraint constraint2 = new GradientConstraint(data.length-1)
			{
				public Vector3d determine_value(Vector3d[] data)
				{
					return new Vector3d(1,0,0);
				}
			};
			CubicBSpline spline = construct_interpolating_spline(data, constraint1, constraint2);
			output(spline.calculate_curve_point(0),		"(-2.7755575615628914E-17, 0.0, 0.0)");
			output(spline.calculate_curve_point(1.0/3),	"(1.0, 0.0, 2.0)");
			output(spline.calculate_curve_point(2.0/3),	"(2.0, 0.0, 1.0)");
			output(spline.calculate_curve_point(1),		"(3.0, 0.0, 5.0)");

			// Test a more realistic case.

			data = new Vector3d[]
			{
				new Vector3d(80.0, 80.0, 80.0),
				new Vector3d(146.66666666666669, 80.0, 80.0),
				new Vector3d(213.33333333333334, 80.0, 80.0),
				new Vector3d(280.0, 80.0, 80.0),
				new Vector3d(346.6666666666667, 80.0, 80.0),
				new Vector3d(413.3333333333333, 80.0, 80.0),
				new Vector3d(480.0, 80.0, 80.0)
			};
			constraint1 = new CubicBSpline.GradientConstraint(0)
			{
				public Vector3d determine_value(Vector3d[] data) { return new Vector3d(1,0,0); }
			};
			constraint2 = new CubicBSpline.GradientConstraint(data.length-1)
			{
				public Vector3d determine_value(Vector3d[] data) { return new Vector3d(1,0,0); }
			};
			spline = construct_interpolating_spline(data, constraint1, constraint2);
			output(spline.calculate_curve_point(0), "(80.00000000000003, 80.00000000000001, 80.00000000000001)");
			output(spline.calculate_curve_point(1.0/6), "(146.66666666666674, 79.99999999999999, 79.99999999999999)");
			output(spline.calculate_curve_point(2.0/6), "(213.33333333333337, 79.99999999999997, 79.99999999999997)");
			output(spline.calculate_curve_point(3.0/6), "(280.0, 80.0, 80.0)");
			output(spline.calculate_curve_point(4.0/6), "(346.66666666666663, 79.99999999999999, 79.99999999999999)");
			output(spline.calculate_curve_point(5.0/6), "(413.3333333333333, 80.0, 80.0)");
			output(spline.calculate_curve_point(1), "(480.0, 80.00000000000001, 80.00000000000001)");

			// TESTME: More tests would be helpful, particularly one whose constraints are such that the spline
			// equation matrix can't be inverted - construct_interpolating_spline should return null in that case.
		}

		public void test_construct_spline_equation_matrix()
		{
			IConstraint constraint1 = new GradientConstraint(0)
			{
				public Vector3d determine_value(Vector3d[] data) { return null; }		// irrelevant since not used here
			};
			IConstraint constraint2 = new GradientConstraint(3)
			{
				public Vector3d determine_value(Vector3d[] data) { return null; }		// irrelevant since not used here
			};
			Matrix expected = new Matrix(new double[][]
			{
				{0.16666666666666666, 0.6666666666666666, 0.16666666666666666, 0.0, 0.0, 0.0},
				{0.0, 0.16666666666666666, 0.6666666666666666, 0.16666666666666666, 0.0, 0.0},
				{0.0, 0.0, 0.16666666666666666, 0.6666666666666666, 0.16666666666666666, 0.0},
				{0.0, 0.0, 0.0, 0.16666666666666666, 0.6666666666666666, 0.16666666666666666},
				{-0.5, 0.0, 0.5, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, -0.5, 0.0, 0.5}
			});
			output(construct_spline_equation_matrix(6, constraint1, constraint2), expected.toString());

			// TESTME: More tests would be helpful.
		}

		public void test_N()
		{
			// Thoroughly test N{0,3}(t).
			output(N(0,-1),		"0.0");
			output(N(0,-0.5),	"0.0");
			output(N(0,0),		"0.0");
			output(N(0,0.5),	"0.020833333333333332");
			output(N(0,1),		"0.16666666666666666");
			output(N(0,1.5),	"0.4791666666666667");
			output(N(0,2),		"0.6666666666666666");
			output(N(0,2.5),	"0.4791666666666667");
			output(N(0,3),		"0.16666666666666666");
			output(N(0,3.5),	"0.020833333333333332");
			output(N(0,4),		"0.0");
			output(N(0,4.5),	"0.0");
			output(N(0,5),		"0.0");

			// Now test a shifted one, say N{-3,3}(t).
			output(N(-3,-3),	"0.0");
			output(N(-3,-2),	"0.16666666666666666");
			output(N(-3,-1),	"0.6666666666666666");
			output(N(-3,0),		"0.16666666666666666");
			output(N(-3,1),		"0.0");

			// TESTME: More tests would be helpful.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}