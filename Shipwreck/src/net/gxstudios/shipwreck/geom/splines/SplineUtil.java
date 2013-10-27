package net.gxstudios.shipwreck.geom.splines;

import net.gxstudios.shipwreck.geom.planar.Plane;
import net.gxstudios.shipwreck.geom.splines.CubicBSpline.GradientConstraint;
import net.gxstudios.shipwreck.geom.splines.CubicBSpline.IConstraint;
import net.gxstudios.shipwreck.math.MathUtil;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.misc.Constants;
import net.gxstudios.shipwreck.misc.MiscUtil;
import net.gxstudios.shipwreck.misc.Pair;
import net.gxstudios.shipwreck.misc.Triple;
import net.gxstudios.shipwreck.test.TestHarnessAdapter;

/**
This class contains spline utility functions.
*/
public class SplineUtil implements Constants
{
	//################## PUBLIC METHODS ##################//
	/**
	Constructs a meshHeight x meshWidth mesh by using cubic B-splines to interpolate the specified grid
	of points, taking into account the constraints given.

	<p><b>Preconditions:</b>
	<dl>
	<dd>grid.length > 0 && grid[0].length > 0
	<dd>grid is a rectangular two-dimensional array of points
	</dl>

	@param grid			The grid of points the mesh should interpolate
	@param meshHeight	The desired height of the mesh (number of rows)
	@param meshWidth	The desired width of the mesh (number of columns)
	@param xConstraint1	An extra constraint when calculating the splines in the x direction
	@param xConstraint2	Likewise
	@param yConstraint1	An extra constraint when calculating the splines in the y direction
	@param yConstraint2	Likewise
	@return				The specified mesh, as a two-dimensional grid of points
	*/
	public static Vector3d[][] construct_interpolating_mesh(final Vector3d[][] grid, int meshHeight, int meshWidth,
														   IConstraint xConstraint1, IConstraint xConstraint2,
														   IConstraint yConstraint1, IConstraint yConstraint2)
	{
		final int rows = grid.length;

		// Calculate the splines in the x direction by interpolating grid points.
		CubicBSpline[] xSplines = new CubicBSpline[rows];
		
		for(int r=0; r<rows; ++r)
		{
			xSplines[r] = CubicBSpline.construct_interpolating_spline(grid[r], xConstraint1, xConstraint2);
		}
		
		// Calculate the splines in the y direction by calculating points on the x splines and interpolating those.
		CubicBSpline[] ySplines = new CubicBSpline[meshWidth];

		for(int j=0; j<meshWidth; ++j)
		{
			Vector3d[] data = new Vector3d[rows];

			double t = (double)j/(meshWidth-1);

			for(int r=0; r<rows; ++r)
			{
				data[r] = xSplines[r].calculate_curve_point(t);
			}
			ySplines[j] = CubicBSpline.construct_interpolating_spline(data, yConstraint1, yConstraint2);
		}

		// Finally, calculate the mesh points by calculating points on the y splines.
		Vector3d[][] mesh = new Vector3d[meshHeight][meshWidth];
		for(int i=0; i<meshHeight; ++i)
		{
			double t = (double)i/(meshHeight-1);

			for(int j=0; j<meshWidth; ++j)
			{
				mesh[i][j] = ySplines[j].calculate_curve_point(t);
			}
		}

		return mesh;
	}

	/**
	TODO: Comment here.
	*/
	public static Vector3d[][] construct_interpolating_mesh_ex(final Vector3d[][] grid, int meshHeight, int meshWidth,
															  IConstraint xConstraint1, IConstraint xConstraint2,
															  IConstraint yConstraint1, IConstraint yConstraint2)
	{
// TODO: Check this thoroughly!
		final int cols = grid[0].length, rows = grid.length;
		final double minX = grid[0][0].x, maxX = grid[0][cols-1].x, minY = grid[0][0].y, maxY = grid[rows-1][0].y;

		// If something goes wrong with the grid dimensions, just return the original grid.
		if(minX > maxX || minY > maxY || maxX-minX < SMALL_EPSILON || maxY-minY < SMALL_EPSILON) return grid;

		// Calculate the splines in the x direction by interpolating grid points.
		CubicBSpline[] xSplines = new CubicBSpline[rows];
		
		for(int r=0; r<rows; ++r)
		{
			xSplines[r] = CubicBSpline.construct_interpolating_spline(grid[r], xConstraint1, xConstraint2);
		}

		// Calculate points on the y splines by intersecting the x splines with the planes in which the y splines are to lie,
		// and thus calculate the splines in the y direction by interpolating intermediate mesh points.
		CubicBSpline[] ySplines = new CubicBSpline[meshWidth];
		Vector3d[][] intermediateMesh = new Vector3d[meshWidth][rows];	// note the reverse order of the dimensions! it makes things easier
		for(int j=0; j<meshWidth; ++j)
		{
			double x = j*(maxX-minX)/(meshWidth-1) + minX;
			Plane plane = new Plane(new Vector3d(1,0,0), x);
			int knot = (int)((cols-1)*(x-minX)/(maxX-minX));

			for(int r=0; r<rows; ++r)
			{
				Vector3d result = intersect_spline_segment_with_plane(xSplines[r], knot, plane);

				// If we couldn't find an intersection point, something went wrong, but we'll try and stagger on.
				if(result == null)
				{
					System.err.println("Warning: Spline segment-plane intersection point not found!");
					System.err.println(r);
					System.err.println(MiscUtil.string_from_array(grid[r]));
					System.err.println(MiscUtil.string_from_array(xSplines[r].get_control_points()));
					System.err.println(knot);
					System.err.println(plane);
					System.err.println("***");
					System.err.println(xSplines[r].calculate_curve_point((double)knot/(xSplines[r].get_control_points().length-3)));
					System.err.println(xSplines[r].calculate_curve_point((double)(knot+1)/(xSplines[r].get_control_points().length-3)));
					return grid;
				}

				intermediateMesh[j][r] = result;
			}

			ySplines[j] = CubicBSpline.construct_interpolating_spline(intermediateMesh[j], yConstraint1, yConstraint2);
		}

		// Finally, calculate the true mesh points by intersecting the y splines with the relevant planes.
		Vector3d[][] mesh = new Vector3d[meshHeight][meshWidth];
		for(int i=0; i<meshHeight; ++i)
		{
			double y = i*(maxY-minY)/(meshHeight-1) + minY;
			Plane plane = new Plane(new Vector3d(0,1,0), y);
			int knot = (int)((rows-1)*(y-minY)/(maxY-minY));

			for(int j=0; j<meshWidth; ++j)
			{
				Vector3d result = intersect_spline_segment_with_plane(ySplines[j], knot, plane);

				// If we couldn't find an intersection point, something went wrong, but we'll try and stagger on.
				if(result == null)
				{
					System.err.println("Warning: Spline segment-plane intersection point not found!");
					return grid;
				}

				mesh[i][j] = result;
			}
		}

		return mesh;
	}

	/**
	Returns an intersection point of the spline segment between t' = knot and t' = knot+1 (inclusive) with the specified plane.
	Note that by t', I'm referring to the unscaled spline parameter, rather than the normalized one, in other words:

	t' = t*(<control_point_count>-3)

	@param spline	The spline whose segment we want to intersect with the plane
	@param knot		The knot at the start of the spline segment we're intersecting
	@param plane	The plane with which to intersect the spline
	@return			An intersection point, if possible, or null otherwise
	*/
	public static Vector3d intersect_spline_segment_with_plane(final CubicBSpline spline, int knot, final Plane plane)
	{
// TODO: Check this thoroughly!
		/*
		Derivation of the algorithm:

		The specified spline segment is defined by C(t) = sum{k=knot-3,knot} N{k,3}(t) . d[k] for t in the range [knot, knot+1].

		Define u = t - knot, then u ranges over [0,1].

		Well, we have (in the specified range):
		N{knot-3,3}(t)	= 1/6(4-(t-(knot-3)))^3
						= 1/6(4-t+(knot-3))^3
						= 1/6(knot+1-t)^3
						= 1/6(1-(t-knot))^3
						= 1/6(1-u)^3
						= u^3(-1/6) + u^2(1/2) + u(-1/2) + 1/6
						= 1/6(-u^3 + 3u^2 - 3u + 1)		(déjà vu - these are the spline matrix coefficients)
		N{knot-2,3}(t)	= 1/6(4-(t-(knot-2)))^3 - 2/3(4-(t-(knot-2))-1)^3
						= 1/6(4-t+(knot-2))^3 - 2/3(3-t+(knot-2))^3
						= 1/6(2-(t-knot))^3 - 2/3(1-(t-knot))^3
						= 1/6(2-u)^3 - 2/3(1-u)^3
						= u^3(-1/6 + 2/3) + u^2(1 - 2) + u(-2 + 2) + (4/3 - 2/3)
						= 1/6(3u^3 - 6u^2 + 4)			(again, these are the spline matrix coefficients, as we'd expect)
		N{knot-1,3}(t)	= 1/6(t-(knot-1))^3 - 2/3((t-(knot-1))-1)^3
						= 1/6(1+u)^3 - 2/3u^3
						= u^3(1/6-2/3) + u^2(1/2) + u(1/2) + 1/6
						= 1/6(-3u^3 + 3u^2 + 3u + 1)	(again, these are the spline matrix coefficients, as we'd expect)
		N{knot,3}(t)	= 1/6(t-knot)^3 = 1/6u^3		(and the same with this one)

		Now let our plane be n . x = d.

		We want to find t' in the range [knot,knot+1] s.t. n . C(t) = d.

		So we have n . sum{k=knot-3,knot} N{k,3}(t) . d[k] = d
		<-> sum{k=knot-3,knot} N{k,3}(t) (n . d[k]) = d
		<-> (sum{k=knot-3,knot} N{k,3}(t) (n . d[k])) - d = 0
		<->	1/6(-u^3 + 3u^2 - 3u + 1) . (n . d[knot-3]) +
			1/6(3u^3 - 6u^2 + 4) . (n . d[knot-2]) +
			1/6(-3u^3 + 3u^2 + 3u + 1) . (n . d[knot-1]) +
			1/6u^3 . (n . d[knot]) - d = 0

		Let a[i] = n . d[knot+i], then

		u^3(-a[-3] + 3a[-2] - 3a[-1] + a[0]) + u^2(3a[-3] - 6a[-2] + 3a[-1]) + u(-3a[-3] + 3[a-1]) + (a[-3] + 4a[-2] + a[-1]) - 6d = 0

		This is a cubic equation which can easily be solved for a root u', whence we can calculate t' as u' + knot.

		Of course, it's possible that we will get more than one root back, in which case we just return any intersection point
		in the correct range.
		*/

		Vector3d n = plane.get_normal();
		Vector3d[] d = spline.get_control_points();

		// Calculate the values of a[i] as in the above description. Note that a[j] here corresponds to a[j-3] above, because of
		// the requirement that arrays start from 0 (rather than -3, which would be more intuitive here!).
		double[] a = new double[4];
		for(int i=0; i<4 && knot+i < d.length; ++i) a[i] = n.dot(d[knot+i]);

		// Solve the cubic derived above.
		double cubicCoefficient = -a[0] + 3*a[1] - 3*a[2] + a[3];
		double quadraticCoefficient = 3*a[0] - 6*a[1] + 3*a[2];
		double linearCoefficient = -3*a[0] + 3*a[2];
		double constantCoefficient = a[0] + 4*a[1] + a[2] - 6*plane.get_distance_value();
		Pair<Integer,Triple<Double,Double,Double>> result = MathUtil.solve_cubic_equation(cubicCoefficient, quadraticCoefficient, linearCoefficient, constantCoefficient);

		// Note: The fall-throughs in this switch statement are deliberate, don't "fix" them!
		Double t;
		switch(result.first)
		{
		case 3:
			// Account for errors in the output from the equation solver.
			t = clamp_endpoint_to_unit_range((result.second.third + knot)/(d.length-3));
			if(t != null) return spline.calculate_curve_point(t);
		case 2:
			// Likewise.
			t = clamp_endpoint_to_unit_range((result.second.second + knot)/(d.length-3));
			if(t != null) return spline.calculate_curve_point(t);
		case 1:
			// Likewise.
			t = clamp_endpoint_to_unit_range((result.second.first + knot)/(d.length-3));
			if(t != null) return spline.calculate_curve_point(t);
		default:
			// If there were solutions to the cubic, but no results, it merits outputting some debugging information.
			/*if(result.first != 0)
			{
				// First, output enough to check that the cubic was correctly solved.
				System.err.println(cubicCoefficient + "t^3 + " + quadraticCoefficient + "t^2 + " + linearCoefficient + "t + " + constantCoefficient + " = 0");
				System.err.println(result.first + " " + result.second.first + " " + result.second.second + " " + result.second.third);

				// Then output as much other data as possible.
				System.err.println(net.gxstudios.shipwreck.misc.MiscUtil.string_from_array(d));
				System.err.println(knot);
				System.err.println(plane);
			}*/
			return null;
		}
	}

	//################## PRIVATE METHODS ##################//
	/**
	Clamps values slightly outside the range [0,1] to 0 or 1. Clearly "slightly" is
	an imprecise term here, which in practice we take to mean "whatever gives us
	results with which we're happy". In practice, it makes sense to pick a liberal
	tolerance value, because in the usage context, we're likely to either have values
	in the right range (or close to it), or ones a long way away.

	@param d	The value to clamp
	@return		0 if -tolerance < d < 0, or 1 if 1 < d < 1+tolerance, or d if d is in [0,1], or null otherwise
	*/
	private static Double clamp_endpoint_to_unit_range(final Double d)
	{
		final double TOLERANCE = EPSILON;

		if(-TOLERANCE < d && d < 0.0) return 0.0;
		if(1.0 < d && d < 1.0+TOLERANCE) return 1.0;
		if(0.0 <= d && d <= 1.0) return d;

		//System.err.println(d);

		return null;
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_intersect_spline_segment_with_plane()
		{
			// Test a simple case first.

			Vector3d[] data = new Vector3d[]
			{
				new Vector3d(0,0,0),
				new Vector3d(1,0,2),
				new Vector3d(2,0,0),
				new Vector3d(3,0,2)
			};
			IConstraint constraint1 = new GradientConstraint(0)
			{
				public Vector3d determine_value(Vector3d[] data)
				{
					return new Vector3d(1,0,0);
				}
			};
			IConstraint constraint2 = new GradientConstraint(3)
			{
				public Vector3d determine_value(Vector3d[] data)
				{
					return new Vector3d(1,0,0);
				}
			};
			CubicBSpline spline = CubicBSpline.construct_interpolating_spline(data, constraint1, constraint2);
			output(intersect_spline_segment_with_plane(spline, 0, new Plane(new Vector3d(1,0,0), 0.25)), "");
			output(intersect_spline_segment_with_plane(spline, 0, new Plane(new Vector3d(1,0,0), 0.5)), "");
			output(intersect_spline_segment_with_plane(spline, 0, new Plane(new Vector3d(1,0,0), 0.75)), "");

			data = new Vector3d[]
			{
				new Vector3d(0,0,0),
				new Vector3d(1,0,2),
				new Vector3d(2,0,1),
				new Vector3d(3,0,5)
			};
			spline = CubicBSpline.construct_interpolating_spline(data, constraint1, constraint2);
			output(intersect_spline_segment_with_plane(spline, 0, new Plane(new Vector3d(1,0,0), 0.5)), "");
			output(intersect_spline_segment_with_plane(spline, 1, new Plane(new Vector3d(1,0,0), 1.5)), "");

			// TODO: Figure out whether the results for the tests are correct, and add more tests.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}