package net.gxstudios.shipwreck.textures;

/**
This class represents 2D texture coordinates. Note that TextureCoord objects are immutable once created.
*/
public class TextureCoord
{
	//################## PUBLIC VARIABLES ##################//
	final public double u;
	final public double v;

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a new texture coordinate with the specified u and v values.

	@param u	The u value of the new texture coordinate
	@param v	The v value of the new texture coordinate
	*/
	public TextureCoord(double u, double v)
	{
		this.u = u;
		this.v = v;
	}

	//################## PUBLIC METHODS ##################//
	/**
	Returns a texture coordinate which has u = (1-t)*c1.u + t*c2.u and v = (1-t)*c1.v + t*c2.v, thus
	interpolating between the two specified texture coordinates by the specified amount. In other
	words, if c1 is the texture coordinate at v1, and c2 is the texture coordinate at v2, we are
	calculating the texture coordinate at the point v1 + t(v2-v1).

	<p><b>Preconditions:</b>
	<dl>
	<dd>c1 != null
	<dd>c2 != null
	<dd>0 <= t <= 1
	</dl>

	@param c1	One texture coordinate
	@param c2	The other texture coordinate
	@param t	The value of the parameter t of the line v1 + t(v2-v1) at the point whose texture coordinate
				we wish to calculate
	@return		The interpolated texture coordinate, as specified
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static TextureCoord interpolate(final TextureCoord c1, final TextureCoord c2, double t)
	{
		// Check the preconditions.
		if(c1 == null || c2 == null || t < 0 || t > 1) throw new java.lang.Error();

		// Main algorithm
		return new TextureCoord((1-t)*c1.u + t*c2.u, (1-t)*c1.v + t*c2.v);
	}

	/**
	Returns a string representation of the texture coordinate.

	@return		...think about it...
	*/
	public String toString()
	{
		return "(" + u + ", " + v + ")";
	}
}