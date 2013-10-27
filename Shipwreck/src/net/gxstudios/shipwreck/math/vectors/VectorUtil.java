package net.gxstudios.shipwreck.math.vectors;

import net.gxstudios.shipwreck.test.TestHarnessAdapter;

final public class VectorUtil
{
	/**
	Returns the result of the addition lhs + rhs.

	@param lhs	The left-hand operand of the addition
	@param rhs	The right-hand operand of the addition
	@return		The result of the addition
	*/
	public static <T extends IVector<T>> T add(final T lhs, final T rhs)
	{
		return lhs.clone().add(rhs);
	}

	/**
	Returns the angle between the two parameter vectors.

	@param lhs	One of the vectors
	@param rhs	The other one
	@return		As specified above
	*/
	public static <T extends IVector<T>> double angle_between(final T lhs, final T rhs)
	{
		/*
		Derivation of the algorithm:

		lhs . rhs = |lhs|*|rhs|*cos theta
		theta = acos((lhs . rhs) / (|lhs|*|rhs|))
		*/
		double arg = lhs.dot(rhs) / (lhs.length() * rhs.length());
		if(arg > 1) arg = 1;
		return Math.acos(arg);
	}

	/**
	Returns the distance between the two parameter vectors. Note that both are being
	treated as point vectors here.

	@param lhs	One of the point vectors
	@param rhs	The other one
	@return		...think about it...
	*/
	public static <T extends IVector<T>> double distance(final T lhs, final T rhs)
	{
		return lhs.distance(rhs);
	}

	/**
	Returns the square of the distance between the two parameter vectors. Note that
	both are being treated as point vectors here.

	@param lhs	One of the point vectors
	@param rhs	The other one
	@return		...think about it...
	*/
	public static <T extends IVector<T>> double distance_squared(final T lhs, final T rhs)
	{
		return lhs.distance_squared(rhs);
	}

	/**
	Returns the dot product of the two parameter vectors.

	@param lhs	One operand of the dot product
	@param rhs	The other operand of the dot product
	@return		...think about it...
	*/
	public static <T extends IVector<T>> double dot(final T lhs, final T rhs)
	{
		return lhs.dot(rhs);
	}

	/**
	Returns the result of the scaling operation factor * v.

	@param v		The vector to scale
	@param factor	The factor by which to scale it
	@return			The result of the scaling operation
	*/
	public static <T extends IVector<T>> T scale(final T v, double factor)
	{
		return v.clone().scale(factor);
	}

	/**
	Returns the result of the subtraction lhs - rhs.

	@param lhs	The left-hand operand of the subtraction
	@param rhs	The right-hand operand of the subtraction
	@return		The result of the subtraction
	*/
	public static <T extends IVector<T>> T subtract(final T lhs, final T rhs)
	{
		return lhs.clone().subtract(rhs);
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_add()
		{
			// Tests with 2D vectors.

			// TODO

			// Tests with 3D vectors.

			output(add(new Vector3d(7,8,9), new Vector3d(10,11,12)), new Vector3d(17,19,21).toString());

			// A test to make sure the operand vectors aren't changed by the addition.
			Vector3d v = new Vector3d(23,9,84);
			Vector3d w = new Vector3d(-2,-5,1);
			output(add(v, w), new Vector3d(21,4,85).toString());
			output(v, new Vector3d(23,9,84).toString());
			output(w, new Vector3d(-2,-5,1).toString());
		}

		public void test_dot()
		{
			// Tests with 2D vectors.

			// TODO

			// Tests with 3D vectors.

			output(dot(new Vector3d(7,8,9), new Vector3d(10,11,12)), "266.0");
		}

		public void test_scale()
		{
			// Tests with 2D vectors.

			// TODO

			// Tests with 3D vectors.

			Vector3d v = new Vector3d(4,5,6);
			output(scale(v, 7), new Vector3d(28,35,42).toString());
			output(v, new Vector3d(4,5,6).toString());		// demonstrate that v hasn't changed
		}

		public void test_subtract()
		{
			// Tests with 2D vectors.

			// TODO

			// Tests with 3D vectors.

			output(subtract(new Vector3d(7,8,9), new Vector3d(10,11,12)), new Vector3d(-3,-3,-3).toString());

			// A test to make sure the operand vectors aren't changed by the subtraction.
			Vector3d v = new Vector3d(23,9,84);
			Vector3d w = new Vector3d(2,5,-1);
			output(subtract(v,w), new Vector3d(21,4,85).toString());
			output(v, new Vector3d(23,9,84).toString());
			output(w, new Vector3d(2,5,-1).toString());
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}