package net.gxstudios.shipwreck.math.matrices;

/**
This exception is thrown when a matrix couldn't be inverted because the matrix was singular.
*/
public class SingularMatrixException extends Exception
{
	public SingularMatrixException()
	{
		super("The matrix couldn't be inverted because it was singular");
	}
}