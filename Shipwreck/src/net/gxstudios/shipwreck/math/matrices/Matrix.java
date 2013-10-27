package net.gxstudios.shipwreck.math.matrices;

import java.util.LinkedList;

import net.gxstudios.shipwreck.misc.Constants;
import net.gxstudios.shipwreck.misc.Pair;
import net.gxstudios.shipwreck.misc.Triple;
import net.gxstudios.shipwreck.test.TestHarnessAdapter;

/**
This class represents arbitrary-size matrices.
*/
public class Matrix implements Constants
{
	//################## CONSTANTS ##################//
	private double[][] m_elements;

	//################## CONSTRUCTORS ##################//
	private Matrix() {}		// for internal use

	/**
	Constructs a matrix from the specified rectangular two-dimensional array of elements.

	<p><b>Preconditions:</b>
	<dl>
	<dd>elements != null && elements.length > 0
	<dd>elements must be a rectangular two-dimensional array of doubles
	</dl>

	@param elements	The elements from which to construct the matrix
	*/
	public Matrix(double[][] elements)
	{
		m_elements = elements;
	}

	//################## PUBLIC METHODS ##################//
	/**
	Returns the number of columns in the matrix.

	@return	...think about it...
	*/
	public int columns()
	{
		return m_elements[0].length;
	}

	/**
	Returns a copy of the matrix whose elements are distinct in memory from the original.

	@return	...think about it...
	*/
	public Matrix copy()
	{
		final int cols = columns(), rows = rows();

		Matrix ret = new Matrix();
		ret.m_elements = new double[rows][cols];
		for(int r=0; r<rows; ++r)
		{
			for(int c=0; c<cols; ++c)
			{
				ret.m_elements[r][c] = m_elements[r][c];
			}
		}
		return ret;
	}

	/**
	Returns the elements of the matrix as a rectangular two-dimensional array of doubles.

	@return	...think about it...
	*/
	public double[][] get_elements()
	{
		return m_elements;
	}

	/**
	Constructs an n x n identity matrix.

	@param n	The size of the (square) identity matrix
	@return		An n x n identity matrix as specified
	*/
	public static Matrix identity(int n)
	{
		Matrix ret = new Matrix();
		ret.m_elements = new double[n][n];
		for(int i=0; i<n; ++i) ret.m_elements[i][i] = 1;
		return ret;
	}

	/**
	Calculates the inverse of the matrix, if possible.

	@return	The inverse of the matrix, if there is one
	@throws NonSquareMatrixException	If the matrix was non-invertible because it was square
	@throws SingularMatrixException		If the matrix was non-invertible because it was singular
	*/
	public Matrix inverse() throws NonSquareMatrixException, SingularMatrixException
	{
		if(columns() != rows()) throw new NonSquareMatrixException();

		Triple<Matrix,LinkedList<IElementaryRowOperation>,Integer> p = reduce_to_rref(this);
		if(p.third == rows())	// if all the rows of the matrix are linearly independent (i.e. its row-rank == rows())
		{
			Matrix ret = identity(rows());
			for(IElementaryRowOperation ero: p.second) ero.apply(ret);
			return ret;
		}
		else throw new SingularMatrixException();
	}

	/**
	Returns the result of multiplying lhs by rhs.

	<p><b>Preconditions:</b>
	<dl>
	<dd>lhs.columns() == rhs.rows()
	</dl>

	@param lhs				The left-hand matrix operand
	@param rhs				The right-hand matrix operand
	@return					The result of the multiplication lhs * rhs
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static Matrix multiply(final Matrix lhs, final Matrix rhs)
	{
		// Check the preconditions.
		if(lhs.columns() != rhs.rows()) throw new java.lang.Error();

		final int lColumns = lhs.columns(), rColumns = rhs.columns(), lRows = lhs.rows();
		final double[][] lElements = lhs.get_elements(), rElements = rhs.get_elements();

		double[][] elements = new double[lRows][rColumns];	// note that these are all initialised to 0
		
		for(int i=0; i<lRows; ++i)
		{
			for(int j=0; j<rColumns; ++j)
			{
				for(int k=0; k<lColumns; ++k)
				{
					elements[i][j] += lElements[i][k]*rElements[k][j];
				}
			}
		}

		return new Matrix(elements);
	}

	/**
	Returns the number of rows in the matrix.

	@return	...think about it...
	*/
	public int rows()
	{
		return m_elements.length;
	}

	/**
	Returns a String representation of the matrix.

	@return	...think about it...
	*/
	public String toString()
	{
		StringBuffer sb = new StringBuffer();
		for(int i=0, rows=rows(); i<rows; ++i)
		{
			sb.append("(\t");
			for(int j=0, cols=columns(); j<cols; ++j)
			{
				sb.append(m_elements[i][j] + "\t");
			}
			sb.append(")\n");
		}
		return sb.toString();
	}

	//################## PRIVATE METHODS ##################//
	/**
	Finds the location of the leftmost non-zero entry in the submatrix
	[(startRow,startCol)-(rows()-1,columns()-1)], if any. Note that there
	may be several entries which are equally far left - if so, we choose
	the one with the largest absolute value, since it improves our numerical
	stability.

	@param m		The matrix in which we want to find our entry
	@param startRow	A lower row bound for our search for a non-zero entry
	@param startCol	A lower column bound for our search for a non-zero entry
	@return			The location of the non-zero entry, if found, or null otherwise
	*/
	private static Pair<Integer,Integer> find_leftmost_nonzero_entry(Matrix m, int startRow, int startCol)
	{
		final int cols = m.columns(), rows = m.rows();

		for(int c=startCol; c<cols; ++c)
		{
			Double best = null;
			int bestRow = -1;
			for(int r=startRow; r<rows; ++r)
			{
				double elementAbs = Math.abs(m.m_elements[r][c]);
				if(elementAbs >= EPSILON)
				{
					if(best == null || elementAbs > best)
					{
						best = elementAbs;
						bestRow = r;
					}
				}
			}
			if(best != null) return Pair.make_pair(bestRow,c);
		}

		return null;
	}

	/**
	Reduces the specified matrix to row-reduced echelon form.

	<p><b>Notes:</b>
	<ul>
	<li>The parameter matrix is unchanged by the method, an internal copy is reduced instead of the original
	</ul>

	@param mat	The matrix to reduce to RREF
	@return		A triple whose first element is the reduced matrix, whose second element is the sequence of
				elementary row operations performed in the reduction, and whose third element is the row-rank
				of the reduced matrix
	*/
	private static Triple<Matrix,LinkedList<IElementaryRowOperation>,Integer> reduce_to_rref(final Matrix mat)
	{
		Matrix m = mat.copy();
		LinkedList<IElementaryRowOperation> eroSequence = new LinkedList<IElementaryRowOperation>();

		final int rows = m.rows();

		int startCol = 0;

		int r;
		for(r=0; r<rows; ++r)
		{
			Pair<Integer,Integer> nzEntry = find_leftmost_nonzero_entry(m, r, startCol);
			if(nzEntry == null) break;	// if we couldn't find a non-zero entry remaining, the matrix is in RREF

			startCol = nzEntry.second+1;	// update the starting column for the next iteration

			//System.out.println("Begin iteration " + r);
			//System.out.println(m);

			if(nzEntry.first != r)		// if the row with the non-zero entry isn't the current one, we need to swap them
			{
				//System.out.println("Swap rows " + r + " and " + nzEntry.first);

				SwapRows ero = new SwapRows(r, nzEntry.first);
				ero.apply(m);
				eroSequence.add(ero);

				//System.out.println(m);
			}

			if(m.m_elements[r][nzEntry.second] != 1)	// if the leading entry of our row isn't 1, we need to scale it
			{
				//System.out.println("Scale row " + r + " by 1/" + m.m_elements[r][nzEntry.second]);

				ScaleRow ero = new ScaleRow(1/m.m_elements[r][nzEntry.second], r);
				ero.apply(m);
				eroSequence.add(ero);

				//System.out.println(m);
			}

			for(int k=0; k<rows; ++k)
			{
				if(k == r) continue;	// we mustn't clear the current row

				if(m.m_elements[k][nzEntry.second] != 0)	// if the other row's entry in our column isn't 0, we need to clear it
				{
					//System.out.println("Add " + -m.m_elements[k][nzEntry.second] + " x row " + r + " to row " + k);

					ScaleAddRow ero = new ScaleAddRow(-m.m_elements[k][nzEntry.second], r, k);
					ero.apply(m);
					eroSequence.add(ero);

					//System.out.println(m);
				}
			}
		}

		return Triple.make_triple(m, eroSequence, r);
	}

	//################## NESTED CLASSES ##################//
	private interface IElementaryRowOperation
	{
		void apply(Matrix m);
	}

	// Scale row i by (non-zero) factor s
	private static class ScaleRow implements IElementaryRowOperation
	{
		private double s;
		private int i;

		public ScaleRow(double s, int i)
		{
			if(Math.abs(s) < SMALL_EPSILON)
			{
				// TODO: Either reenable this and sort it out, or get rid of it altogether.
				//System.err.println("Warning: Factor in row scaling operation (" + s + ") is very small");
				//new Throwable().printStackTrace();
			}

			this.s = s;
			this.i = i;
		}

		public void apply(Matrix m)
		{
			for(int j=0, cols=m.columns(); j<cols; ++j) m.m_elements[i][j] *= s;
		}
	}

	// Do row j += s * row i
	private static class ScaleAddRow implements IElementaryRowOperation
	{
		private double s;
		private int i, j;

		public ScaleAddRow(double s, int i, int j)
		{
			this.s = s;
			this.i = i;
			this.j = j;
		}

		public void apply(Matrix m)
		{
			for(int k=0, cols=m.columns(); k<cols; ++k) m.m_elements[j][k] += s * m.m_elements[i][k];
		}
	}

	// Swap rows i and j
	private static class SwapRows implements IElementaryRowOperation
	{
		private int i, j;

		public SwapRows(int i, int j)
		{
			this.i = i;
			this.j = j;
		}

		public void apply(Matrix m)
		{
			double[] temp = m.m_elements[i];
			m.m_elements[i] = m.m_elements[j];
			m.m_elements[j] = temp;
		}
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		private static String matrix_line(double ... ds)
		{
			StringBuffer sb = new StringBuffer();
			sb.append("(\t");
			for(Object d: ds) sb.append(d + "\t");
			sb.append(")\n");
			return sb.toString();
		}

		public void test_find_leftmost_nonzero_entry()
		{
			Matrix mat = new Matrix(new double[][]
			{
				{0,0,0,0,2,6},
				{0,5,5,20,5,5},
				{0,0,-3,-3,-3,-9},
				{0,2,2,8,2,2}
			});
			output(find_leftmost_nonzero_entry(mat,0,0), "(1, 1)");

			mat = new Matrix(new double[][]
			{
				{0,1,1,4,1,1},
				{0,0,0,0,2,6},
				{0,0,-3,-3,-3,-9},
				{0,0,0,0,0,0}
			});
			output(find_leftmost_nonzero_entry(mat,1,2), "(2, 2)");

			// TESTME: More tests are definitely needed here.
		}

		public void test_identity()
		{
			Matrix i2 = identity(2);
			output(i2, "(\t1.0\t0.0\t)\n(\t0.0\t1.0\t)\n");

			Matrix i3 = identity(3);
			output(i3, "(\t1.0\t0.0\t0.0\t)\n(\t0.0\t1.0\t0.0\t)\n(\t0.0\t0.0\t1.0\t)\n");
		}

		public void test_inverse()
		{
			// First try a few invertible matrices.

			Matrix mat = new Matrix(new double[][]
			{
				{1,-1,0,0},
				{1,0,-1,0},
				{1,0,0,-1},
				{0,1,1,1}
			});
			Matrix expected = new Matrix(new double[][]
			{
				{0.33333333333333337, 0.33333333333333337, 0.33333333333333333, 0.33333333333333337},
				{-0.6666666666666666, 0.33333333333333337, 0.33333333333333333, 0.33333333333333337},
				{0.33333333333333337, -0.6666666666666666, 0.33333333333333333, 0.33333333333333337},
				{0.33333333333333333, 0.33333333333333333, -0.6666666666666666, 0.33333333333333333}
			});
			try
			{
				output(mat.inverse(), expected.toString());
			}
			catch(Exception e) { output("Error", expected.toString()); }

			mat = new Matrix(new double[][]
			{
				{1,1,2,2},
				{2,1,1,2},
				{2,2,1,2},
				{3,3,1,3}
			});
			expected = new Matrix(new double[][]
			{
				{-1.0, 1.0, 3.0, -1.9999999999999998},
				{0.0, -1.0, 1.0, -1.1102230246251565E-16},
				{0.0, 0.0, 3.0, -1.9999999999999998},
				{1.0, -0.0, -5.0, 3.0}
			});
			try
			{
				// The slight numerical imprecisions in the answer are negligible, but for the test to
				// pass, we have to pass in the exact values we're getting.
				output(mat.inverse(), expected.toString());
			}
			catch(Exception e) { output("Error", expected.toString()); }

			mat = new Matrix(new double[][]
			{
				{1,-1,1,2},
				{0,1,2,-1},
				{3,1,1,1},
				{3,2,1,0}
			});
			expected = new Matrix(new double[][]
			{
				{-1.9999999999999996, 0.49999999999999994, 4.499999999999999, -3.5},
				{2.9999999999999996, -1.0, -6.999999999999999, 6.0},
				{0.0, 0.5, 0.5000000000000001, -0.5},
				{3.0, -1.0, -6.0, 5.0}
			});
			try
			{
				output(mat.inverse(), expected.toString());
			}
			catch(Exception e) { output("Error", expected.toString()); }

			// Now let's try some we can't invert.

			mat = new Matrix(new double[][]
			{
				{1,0,0},
				{0,1,0},
				{0,1,0}
			});
			try
			{
				output(mat.inverse(), "SingularMatrixException");
			}
			catch(SingularMatrixException e)	{ output("", ""); }
			catch(NonSquareMatrixException e)	{ output("NonSquareMatrixException", "SingularMatrixException"); }

			mat = new Matrix(new double[][]
			{
				{1,2},
				{3,4},
				{5,6}
			});
			try
			{
				output(mat.inverse(), "NonSquareMatrixException");
			}
			catch(NonSquareMatrixException e)	{ output("", ""); }
			catch(SingularMatrixException e)	{ output("SingularMatrixException", "NonSquareMatrixException"); }

			// TESTME: More tests would be helpful here.
		}

		public void test_multiply()
		{
			// Test some simple cases first.

			Matrix lhs = new Matrix(new double[][]
			{
				{1,2,3},
				{4,5,6}
			});
			Matrix rhs = new Matrix(new double[][]
			{
				{7},
				{8},
				{9}
			});
			Matrix expected = new Matrix(new double[][]
			{
				{50},
				{122}
			});
			output(multiply(lhs, rhs), expected.toString());

			lhs = new Matrix(new double[][]
			{
				{1,2},
				{3,4}
			});
			rhs = new Matrix(new double[][]
			{
				{5,6},
				{7,8}
			});
			expected = new Matrix(new double[][]
			{
				{19,22},
				{43,50}
			});
			output(multiply(lhs, rhs), expected.toString());

			// TESTME: More tests would be helpful here.
		}

		public void test_reduce_to_rref()
		{
			Matrix mat = new Matrix(new double[][]
			{
				{0,0,0,0,2,6},
				{0,5,5,20,5,5},
				{0,0,-3,-3,-3,-9},
				{0,2,2,8,2,2}
			});
			Triple<Matrix,LinkedList<IElementaryRowOperation>,Integer> result = reduce_to_rref(mat);
			Matrix expected = new Matrix(new double[][]
			{
				{ 0.0,  1.0, 0.0, 3.0, 0.0, -2.0},
				{-0.0, -0.0, 1.0, 1.0, 0.0,  0.0},
				{ 0.0,  0.0, 0.0, 0.0, 1.0,  3.0},
				{ 0.0,  0.0, 0.0, 0.0, 0.0,  0.0}
			});
			output(Pair.make_pair(result.first, result.third), "(" + expected + ", 3)");

			mat = new Matrix(new double[][]
			{
				{3,1,1},
				{0,2,2},
				{6,8,8}
			});
			result = reduce_to_rref(mat);
			expected = new Matrix(new double[][]
			{
				{1.0, 0.0, 0.0},
				{-0.0, 1.0, 1.0},
				{0.0, 0.0, 0.0}
			});
			output(Pair.make_pair(result.first, result.third), "(" + expected + ", 2)");
				   

			mat = new Matrix(new double[][]
			{
				{1,-2,4},
				{2,-3,6},
				{0,3,-6}
			});
			result = reduce_to_rref(mat);
			expected = new Matrix(new double[][]
			{
				{1.0, 0.0,  0.0},
				{0.0, 1.0, -2.0},
				{0.0, 0.0,  0.0}
			});
			output(Pair.make_pair(result.first, result.third), "(" + expected + ", 2)");	   

			// TESTME: More tests would be helpful here.
		}

		public void test_ScaleAddRow_apply()
		{
			begin_test("ScaleAddRow.apply");

			Matrix mat = identity(3);
			new ScaleAddRow(1,1,0).apply(mat);
			new ScaleAddRow(2,0,2).apply(mat);
			output(mat, "(\t1.0\t1.0\t0.0\t)\n(\t0.0\t1.0\t0.0\t)\n(\t2.0\t2.0\t1.0\t)\n");

			mat = new Matrix(new double[][]
			{
				{1,2},
				{3,4},
				{5,6}
			});
			new ScaleAddRow(-1,1,2).apply(mat);
			new ScaleAddRow(-0.5,2,0).apply(mat);
			output(mat, "(\t0.0\t1.0\t)\n(\t3.0\t4.0\t)\n(\t2.0\t2.0\t)\n");
		}

		public void test_ScaleRow_apply()
		{
			begin_test("ScaleRow.apply");

			Matrix mat = new Matrix(new double[][]
			{
				{1,2,3},
				{4,5,6}
			});
			new ScaleRow(3,0).apply(mat);
			output(mat, "(\t3.0\t6.0\t9.0\t)\n(\t4.0\t5.0\t6.0\t)\n");

			mat = new Matrix(new double[][]
			{
				{1,2},
				{3,4},
				{5,6}
			});
			new ScaleRow(2,1).apply(mat);
			output(mat, "(\t1.0\t2.0\t)\n(\t6.0\t8.0\t)\n(\t5.0\t6.0\t)\n");
		}

		public void test_SwapRows_apply()
		{
			begin_test("SwapRows.apply");

			Matrix mat = identity(2);
			new SwapRows(0,1).apply(mat);
			output(mat, "(\t0.0\t1.0\t)\n(\t1.0\t0.0\t)\n");

			mat = new Matrix(new double[][]
			{
				{1,2,3,4},
				{5,6,7,8},
				{9,10,11,12}
			});
			new SwapRows(2,0).apply(mat);
			output(mat, "(\t9.0\t10.0\t11.0\t12.0\t)\n(\t5.0\t6.0\t7.0\t8.0\t)\n(\t1.0\t2.0\t3.0\t4.0\t)\n");
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}