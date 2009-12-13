package MapEditor.Textures;

import MapEditor.Geom.AxisPair;
import MapEditor.Math.MathUtil;
import MapEditor.Math.Matrices.*;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import MapEditor.Test.*;

/**
This class represents Worldcraft-style texture planes. A texture is tiled over one of the axis-aligned planes
through the origin and then projected onto a face. Which axis-aligned plane is chosen for each face depends on
the orientation of its normal.
*/
public class TexturePlane implements Constants
{
	//################## NESTED CLASSES ##################//
	public static class Uninitialised extends TexturePlane
	{
		public Uninitialised(final Vector3d faceNormal)
		{
			super(faceNormal, 1, 1, 0, 0, 0);
		}

		public TexturePlane generate_equivalent_plane(Vector3d[] oldVerts, Vector3d[] newVerts)
		{
			return new Uninitialised(MathUtil.calculate_normal(newVerts[0], newVerts[1], newVerts[2]));
		}
	}

	//################## PRIVATE VARIABLES ##################//
	final private static int TEXTURE_WIDTH = 128;
	final private static int TEXTURE_HEIGHT = 128;

	final private double m_scaleU;
	final private double m_scaleV;
	final private TextureCoord m_offset;	// the (ou, ov) texture offset, in the texture coordinate system
	final private Vector2d m_uAxis;			// the u texture axis (note that |m_uAxis| is always 1), in the texture coordinate system
	final private Vector2d m_vAxis;			// the v texture axis (note that |m_vAxis| is always 1), in the texture coordinate system
	final private Vector3d m_horizAxis;		// the base horizontal axis of the texture plane (e.g. (0,1,0) when the face normal's x component is largest),
											// in the world coordinate system
	final private Vector3d m_vertAxis;		// the base vertical axis of the texture plane (e.g. (0,0,-1) when the face normal's x component is largest),
											// in the world coordinate system

	//################## CONSTRUCTORS ##################//
	private TexturePlane(double scaleU, double scaleV, double offsetU, double offsetV, final Vector2d uAxis, final Vector2d vAxis,
						 final Vector3d horizAxis, final Vector3d vertAxis)
	{
		if(scaleU == 0.0) scaleU = 1.0;
		if(scaleV == 0.0) scaleV = 1.0;

		m_scaleU = scaleU;
		m_scaleV = scaleV;
		m_offset = new TextureCoord(offsetU, offsetV);
		m_uAxis = uAxis;
		m_vAxis = vAxis;
		m_horizAxis = horizAxis;
		m_vertAxis = vertAxis;
	}

	/**
	Constructs a texture plane for a face, given the face normal and a set of texture parameters.

	<p><b>Preconditions:</b>
	<dl>
	<dd>faceNormal != null && faceNormal.length() > 0
	</dl>

	<p><b>Notes:</b>
	<ul>
	<li>If either of the scale parameters is 0, it gets set to 1 instead, since 0 isn't valid.
	</ul>

	@param faceNormal	The face's normal
	@param scaleU		The scale on the u texture axis
	@param scaleV		The scale on the v texture axis
	@param offsetU		The offset on the u texture axis
	@param offsetV		The offset on the v texture axis
	@param angleDegrees	The (clockwise) rotation (in degrees) of the texture from the base axes
	*/
	public TexturePlane(final Vector3d faceNormal, double scaleU, double scaleV, double offsetU, double offsetV, double angleDegrees)
	{
		if(scaleU == 0.0) scaleU = 1.0;
		if(scaleV == 0.0) scaleV = 1.0;

		m_scaleU = scaleU;
		m_scaleV = scaleV;
		m_offset = new TextureCoord(offsetU, offsetV);

		final double angleRadians = Math.toRadians(angleDegrees);
		m_uAxis = new Vector2d(Math.cos(angleRadians), Math.sin(angleRadians));
		m_vAxis = new Vector2d(-Math.sin(angleRadians), Math.cos(angleRadians));

		Pair<Vector3d,Vector3d> axisVectors = determine_axis_vectors(faceNormal);
		m_horizAxis = axisVectors.first;
		m_vertAxis = axisVectors.second;
	}

	//################## PUBLIC METHODS ##################//
	/**
	Calculates the 3D texture axes (used for rendering).

	@return	A pair, the first component of which is the u axis and the second component of which is the v axis
	*/
	public Pair<Vector3d,Vector3d> axes()
	{
		Vector3d u = new Vector3d();
		u = Vector3d.scale_add(m_uAxis.x, m_horizAxis, u);
		u = Vector3d.scale_add(m_uAxis.y, m_vertAxis, u);

		Vector3d v = new Vector3d();
		v = Vector3d.scale_add(m_vAxis.x, m_horizAxis, v);
		v = Vector3d.scale_add(m_vAxis.y, m_vertAxis, v);

		return Pair.make_pair(u, v);
	}

	/**
	Calculates the (u,v) texture coordinate pair of the specified 3D point.

	@param p3D				The 3D point whose texture coordinates we wish to calculate
	@return					The calculated texture coordinates as a TextureCoord object
	*/
	public TextureCoord calculate_coordinates(final Vector3d p3D)
	{
		Vector2d p = project_to_texture_plane(p3D);
		return new TextureCoord((p.dot(m_uAxis)/m_scaleU + m_offset.u)/TEXTURE_WIDTH, (p.dot(m_vAxis)/m_scaleV + m_offset.v)/TEXTURE_HEIGHT);
	}

	/**
	Generates a texture plane which is equivalent to this texture plane in the sense that it maps the first three
	points of newVerts to the texture coordinates to which the respective points of oldVerts were mapped by this
	texture plane, assuming that this is possible.

	More formally, the method returns (if possible) a texture plane p s.t. for i in [0,3):
	p.calculate_coordinates(newVerts[i]) == calculate_coordinates(oldVerts[i])

	<p><b>Preconditions:</b>
	<dl>
	<dd>oldVerts != null && newVerts != null
	<dd>oldVerts.length == newVerts.length
	<dd>oldVerts.length >= 3 && for all i in [0,3), oldVerts[i] != null && newVerts[i] != null
	</dl>

	@param oldVerts			The old vertices
	@param newVerts			The new vertices
	@return					An equivalent texture plane, if possible, or null
	*/
	public TexturePlane generate_equivalent_plane(Vector3d[] oldVerts, Vector3d[] newVerts)
	{
		return fit_plane(newVerts[0], newVerts[1], newVerts[2],
						 calculate_coordinates(oldVerts[0]),
						 calculate_coordinates(oldVerts[1]),
						 calculate_coordinates(oldVerts[2]));
	}

	/**
	Returns the texture offset.

	@return	...think about it...
	*/
	public TextureCoord offset()
	{
		return m_offset;
	}

	/**
	Returns the texture rotation in degrees.

	@return	...think about it...
	*/
	public double rotation()
	{
		return Math.toDegrees(MathUtil.signed_angle_between(new Vector2d(1,0), m_uAxis));
	}

	/**
	Returns the scale on the u axis.

	@return	...think about it...
	*/
	public double scale_u()
	{
		return m_scaleU;
	}

	/**
	Returns the scale on the v axis.

	@return	...think about it...
	*/
	public double scale_v()
	{
		return m_scaleV;
	}

	/**
	Returns a string representation of the texture plane.

	@return	...think about it...
	*/
	public String toString()
	{
		return "[ " + offset().u + " " + offset().v + " " + scale_u() + " " + scale_v() + " " + rotation() + " ]";
	}

	//################## PRIVATE METHODS ##################//
	/**
	Determines the appropriate horizontal and vertical axis vectors for a texture plane
	for a face with the specified normal.

	@param faceNormal	The normal for the face we want to texture
	@return				A pair, the first component of which is the horizontal axis vector and
						the second component of which is the vertical axis vector
	*/
	private static Pair<Vector3d,Vector3d> determine_axis_vectors(final Vector3d faceNormal)
	{
		double xAbs = Math.abs(faceNormal.x), yAbs = Math.abs(faceNormal.y), zAbs = Math.abs(faceNormal.z);
		if(xAbs >= yAbs && xAbs >= zAbs)
		{
			// The x component of the face normal has the largest (absolute) value.
			return Pair.make_pair(new Vector3d(0,1,0), new Vector3d(0,0,-1));
		}	
		else if(yAbs >= zAbs)
		{
			// The y component of the face normal has the largest (absolute) value.
			return Pair.make_pair(new Vector3d(1,0,0), new Vector3d(0,0,-1));
		}
		else
		{
			// The z component of the face normal has the largest (absolute) value.
			return Pair.make_pair(new Vector3d(1,0,0), new Vector3d(0,-1,0));
		}
	}

	/**
	Constructs a texture plane tp based on three vertices and their texture coordinates, and the dimensions of the texture being applied.
	The plane is such that for all (relevant) i, tp.calculate_coordinates(v[i], textureWidth, textureHeight) == c[i], provided this is possible.

	@param v1				The first vertex
	@param v2				The second vertex
	@param v3				The third vertex
	@param c1				The texture coordinates of v1
	@param c2				The texture coordinates of v2
	@param c3				The texture coordinates of v3
	@return					A texture plane for the specified data, as described above, or null if a plane couldn't be fit to the specified data
	*/
	private static TexturePlane fit_plane(final Vector3d v1, final Vector3d v2, final Vector3d v3, TextureCoord c1, TextureCoord c2, TextureCoord c3)
	{
		/*
		Derivation of the algorithm:

		We need to find values for the following things (namely the data members of the texture plane):

		m_scaleU, m_scaleV, m_offset, m_uAxis, m_vAxis, m_horizAxis, m_vertAxis

		We first observe that m_horizAxis and m_vertAxis are easily found from the face normal, which is itself
		easily found from v1, v2 and v3.

		Let p[i] = project_to_texture_plane(v[i]), for all (relevant) i, then:

		((p[i] . m_uAxis)/m_scaleU + m_offset.u)/TEXTURE_WIDTH = c[i].u
		((p[i] . m_vAxis)/m_scaleV + m_offset.v)/TEXTURE_HEIGHT = c[i].v

		So for calculating the u axis, we have:

		((p[1].x*m_uAxis.x + p[1].y*m_uAxis.y)/m_scaleU + m_offset.u)/TEXTURE_WIDTH = c[1].u
		((p[2].x*m_uAxis.x + p[2].y*m_uAxis.y)/m_scaleU + m_offset.u)/TEXTURE_WIDTH = c[2].u
		((p[3].x*m_uAxis.x + p[3].y*m_uAxis.y)/m_scaleU + m_offset.u)/TEXTURE_WIDTH = c[3].u

		The unknowns in this are m_uAxis.x, m_uAxis.y, m_scaleU and m_offset.u, so we have four unknowns
		but only three equations -> the system of equations is underspecified. However, we observe that
		our system looks like:

		p[1].x*(m_uAxis.x/m_scaleU) + p[1].y*(m_uAxis.y/m_scaleU) + m_offset.u = c[1].u*TEXTURE_WIDTH
		p[2].x*(m_uAxis.x/m_scaleU) + p[2].y*(m_uAxis.y/m_scaleU) + m_offset.u = c[2].u*TEXTURE_WIDTH
		p[3].x*(m_uAxis.x/m_scaleU) + p[3].y*(m_uAxis.y/m_scaleU) + m_offset.u = c[3].u*TEXTURE_WIDTH

		So we can solve for m_uAxis.x/m_scaleU, m_uAxis.y/m_scaleU and m_offset.u, and then note that
		m_uAxis.x^2 + m_uAxis.y^2 = 1, hence (m_uAxis.x/m_scaleU)^2 + (m_uAxis.y/m_scaleU)^2 = 1/(m_scaleU^2),
		whence we can deduce the value of m_scaleU and hence work out what m_uAxis.x and m_uAxis.y should be.
		Note that the sign of m_scaleU can't be determined directly; we have some freedom about what we choose
		to do here.

		Note that determining the v axis involves solving a very similar system of equations, to the extent
		that the matrix we have to invert for each (if we solve it that way) is identical. Note that the
		right-hand sides are of the form c[i].v*TEXTURE_HEIGHT, though.

		@@@
		Note that the sign of m_scaleU is important here and we can't work it out directly. We observe, however,
		that either the signs of scale u and scale v are the same, or they're different, and if they are different
		then it doesn't matter which of the two is negative, since we can change the rotation. If they're the same,
		they can both be positive, since again we can change the rotation. Either way, we can make m_scaleU positive,
		calculate the v axis explicitly (it has to be the clockwise rotation of the u axis through 90 degrees) and
		determine the sign of m_scaleV to fit the data, assuming that's possible.
		@@@
		*/

// TODO: Test this thoroughly!
// TODO: It would be more efficient to pass the face normal in here explicitly as a parameter.
		Vector3d faceNormal = MathUtil.calculate_normal(v1, v2, v3);
		Pair<Vector3d,Vector3d> axisVectors = determine_axis_vectors(faceNormal);

		Vector2d p1 = project_to_texture_plane(v1, axisVectors.first, axisVectors.second);
		Vector2d p2 = project_to_texture_plane(v2, axisVectors.first, axisVectors.second);
		Vector2d p3 = project_to_texture_plane(v3, axisVectors.first, axisVectors.second);

		/*System.err.println("p1: " + p1);
		System.err.println("p2: " + p2);
		System.err.println("p3: " + p3);
		System.err.println("c1: " + c1);
		System.err.println("c2: " + c2);
		System.err.println("c3: " + c3);*/

		Matrix mat = new Matrix(new double[][]
		{
			{p1.x, p1.y, 1},
			{p2.x, p2.y, 1},
			{p3.x, p3.y, 1}
		});

		Matrix inv = null;
		try					{ inv = mat.inverse(); }
		catch(Exception e)	{ return null; }

		double[][] elements = inv.get_elements();
		double[] uUnknowns = new double[3], vUnknowns = new double[3];
		for(int i=0; i<3; ++i)
		{
			uUnknowns[i] = (elements[i][0]*c1.u + elements[i][1]*c2.u + elements[i][2]*c3.u)*TEXTURE_WIDTH;
			vUnknowns[i] = (elements[i][0]*c1.v + elements[i][1]*c2.v + elements[i][2]*c3.v)*TEXTURE_HEIGHT;
		}

		double scaleU = Math.sqrt(1/(uUnknowns[0]*uUnknowns[0] + uUnknowns[1]*uUnknowns[1]));
		double scaleV = Math.sqrt(1/(vUnknowns[0]*vUnknowns[0] + vUnknowns[1]*vUnknowns[1]));

		Vector2d uAxis = new Vector2d(scaleU*uUnknowns[0], scaleU*uUnknowns[1]);
		Vector2d vAxis = new Vector2d(scaleV*vUnknowns[0], scaleV*vUnknowns[1]);

		//System.err.println("uAxis: " + uAxis);
		//System.err.println("vAxis: " + vAxis);

		// Check that the u and v axes are unit vectors (just to be safe).
		if(Math.abs(uAxis.length_squared() - 1) >= EPSILON)
		{
			System.err.println("Warning: Unexpected renormalization of u axis needed whilst constructing TexturePlane");
			uAxis.normalize();
		}
		if(Math.abs(vAxis.length_squared() - 1) >= EPSILON)
		{
			System.err.println("Warning: Unexpected renormalization of v axis needed whilst constructing TexturePlane");
			vAxis.normalize();
		}

		// Check that the u and v axes are perpendicular and that the v axis is 90 degrees anticlockwise
		// from the u axis (although it looks clockwise because we're dealing in a coordinate system with
		// an inverted vertical axis, i.e. increasing right and down, rather than right and up). If it's
		// 90 degrees clockwise instead, negate the v axis and the v scale, otherwise give up.
		double angle = MathUtil.signed_angle_between(uAxis, vAxis);
		if(Math.abs(angle + Math.PI/2) < SMALL_EPSILON)			// i.e. if(angle == -Math.PI/2)
		{
			vAxis.negate();
			scaleV = -scaleV;
		}
		else if(Math.abs(angle - Math.PI/2) >= SMALL_EPSILON)	// i.e. if(angle != Math.PI/2)
		{
			return null;
		}

		return new TexturePlane(scaleU, scaleV, uUnknowns[2], vUnknowns[2], uAxis, vAxis, axisVectors.first, axisVectors.second);
	}

	/**
	Returns an internal string representation of the texture plane.

	@return	...think about it...
	*/
	private String internal_string()
	{
		StringBuffer sb = new StringBuffer("[");
		sb.append(m_scaleU + " ");
		sb.append(m_scaleV + " ");
		sb.append(m_offset + " ");
		sb.append(m_uAxis + " ");
		sb.append(m_vAxis + " ");
		sb.append(m_horizAxis + " ");
		sb.append(m_vertAxis);
		sb.append("]");
		return sb.toString();
	}

	/**
	Projects the specified point to the texture plane.

	@param p3D	The 3D point to project
	@return		The projection of the point
	*/
	private Vector2d project_to_texture_plane(final Vector3d p3D)
	{
		return project_to_texture_plane(p3D, m_horizAxis, m_vertAxis);
	}

	/**
	Projects the specified point to a texture plane with the specified base axes.

	@param p3D			The 3D point to project
	@param horizAxis	The texture plane's horizontal base axis
	@param vertAxis		The texture plane's vertical base axis
	@return				The projection of the point
	*/
	private static Vector2d project_to_texture_plane(final Vector3d p3D, final Vector3d horizAxis, final Vector3d vertAxis)
	{
		return new Vector2d(horizAxis.dot(p3D), vertAxis.dot(p3D));
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_TexturePlane()
		{
			output(new TexturePlane(new Vector3d(5,2,3), 2, 3, 5, 10, 45).internal_string(),
			"[2.0 3.0 (5.0, 10.0) (0.7071067811865476, 0.7071067811865475) (-0.7071067811865475, 0.7071067811865476) (0.0, 1.0, 0.0) (0.0, 0.0, -1.0)]");

			// TODO
		}

		public void test_calculate_coordinates()
		{
			// I) Test one of the simplest possible cases first.
			TexturePlane texturePlane = new TexturePlane(new Vector3d(0,1,0), 1, 1, 0, 0, 0);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(0.0, 0.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(23,9,84)), "(23.0, -84.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(7,8,-51)), "(7.0, 51.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(-17,-10,51)), "(-17.0, -51.0)");

			// II) Ones with offset only.
			texturePlane = new TexturePlane(new Vector3d(23,9,0), 1, 1, 7, 8, 0);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(7.0, 8.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(-17,10,51)), "(17.0, -43.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(24,-12,-18)), "(-5.0, 26.0)");

			// III) Ones with scale only.

			// Simple scale case.
			texturePlane = new TexturePlane(new Vector3d(7,8,51), 2, 3, 0, 0, 0);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(0.0, 0.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(23,9,84)), "(11.5, -3.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(17,-10,51)), "(8.5, 3.3333333333333335)");
			output(texturePlane.calculate_coordinates(new Vector3d(-24,-12,-18)), "(-12.0, 4.0)");

			// Negative scale case.
			texturePlane = new TexturePlane(new Vector3d(1,0,0), -1, 1, 0, 0, 0);
			output(texturePlane.calculate_coordinates(new Vector3d(23,1,9)), "(-1.0, -9.0)");

			// IV) Ones with rotation only.
			texturePlane = new TexturePlane(new Vector3d(2,10,0), 1, 1, 0, 0, 45);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(0.0, 0.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(1,23,-1)), "(1.414213562373095, 1.1102230246251565E-16)");
			output(texturePlane.calculate_coordinates(new Vector3d(0,9,1)), "(-0.7071067811865475, -0.7071067811865476)");

			// V) Ones with offset and scale only.
			texturePlane = new TexturePlane(new Vector3d(24,12,18), 2, 4, 5, 10, 0);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(5.0, 10.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(51,10,17)), "(10.0, 5.75)");
			output(texturePlane.calculate_coordinates(new Vector3d(84,-23,-9)), "(-6.5, 12.25)");

			// VI) Ones with scale and rotation only.
			texturePlane = new TexturePlane(new Vector3d(10,-17,0), 4, 2, 0, 0, -30);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(0.0, 0.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(Math.sqrt(3)/2,0,0.5)), "(0.25, -5.551115123125783E-17)");
			output(texturePlane.calculate_coordinates(new Vector3d(1,0,-Math.sqrt(3))), "(5.551115123125783E-17, 1.0)");

			// VII) Ones with offset and rotation only.

			// Test the same case as one of those for rotation only, but now with an offset as well.
			texturePlane = new TexturePlane(new Vector3d(2,10,0), 1, 1, 3, 4, 45);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(3.0, 4.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(1,23,-1)), "(4.414213562373095, 4.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(0,9,1)), "(2.2928932188134525, 3.2928932188134525)");

			// VIII) General ones.
			texturePlane = new TexturePlane(new Vector3d(0,0,-1), 5, 10, -2, -4, 90);
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(-2.0, -4.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(51,10,17)), "(-3.999999999999999, -9.1)");
		}

		public void test_fit_plane()
		{
			// Test a nice simple case first.
			TexturePlane texturePlane = fit_plane(new Vector3d(0,0,0),		new Vector3d(1,0,0),		new Vector3d(0,0,-1),
												  new TextureCoord(0,0),	new TextureCoord(1,0),	new TextureCoord(0,1));
			output(texturePlane.internal_string(), "[1.0 1.0 (0.0, 0.0) (1.0, 0.0) (0.0, 1.0) (1.0, 0.0, 0.0) (0.0, 0.0, -1.0)]");
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(0.0, 0.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(1,0,0)), "(1.0, 0.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,-1)), "(0.0, 1.0)");

			// Now test a more complicated case.
			texturePlane = fit_plane(new Vector3d(0,0,0),		new Vector3d(1,1,0),			new Vector3d(0,1,0),
									 new TextureCoord(23,9),	new TextureCoord(24,9),		new TextureCoord(23.5,10));
			output(texturePlane.internal_string(), "[1.4142135623730951 -0.7071067811865476 (23.0, 9.0) (0.7071067811865476, -0.7071067811865476) (0.7071067811865476, 0.7071067811865476) (1.0, 0.0, 0.0) (0.0, -1.0, 0.0)]");
			output(texturePlane.calculate_coordinates(new Vector3d(0,0,0)), "(23.0, 9.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(1,1,0)), "(24.0, 9.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(0,1,0)), "(23.5, 10.0)");
			output(texturePlane.calculate_coordinates(new Vector3d(0,4,0)), "(25.0, 13.0)");

			// TESTME: We need lots of test cases here, fit_plane is quite tricky mathematically and I might have
			// missed something.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}