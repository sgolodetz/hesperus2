package net.gxstudios.shipwreck.math.matrices;

/**
This exception is thrown when the matrix wasn't square in a situation where it needed to be,
for instance when calculating its determinant.
*/
public class NonSquareMatrixException extends Exception
{
	public NonSquareMatrixException()
	{
		super("The matrix isn't square.");
	}
}