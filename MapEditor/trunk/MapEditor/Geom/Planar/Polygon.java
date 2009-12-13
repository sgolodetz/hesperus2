package MapEditor.Geom.Planar;

import MapEditor.Math.MathUtil;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Pair;
import MapEditor.Textures.*;
import java.util.AbstractCollection;

/**
This class represents <i>planar, convex</i> polygons.
<p><b>Datatype Invariant:</b> m_vertices.length >= 3
*/
public class Polygon implements Cloneable
{
	//################## PRIVATE VARIABLES ##################//
	/** The vertices of the polygon */
	private Vector3d[] m_vertices = new Vector3d[] {};
	/** The texture (name) of the polygon */
	private String m_texture;
	/** The texture plane of the polygon */
	private TexturePlane m_texturePlane;
	/** The normal to the polygon */
	private Vector3d m_normal = new Vector3d();

	//################## CONSTRUCTORS ##################//
	/**
	Constructs an uninitialised Polygon (for internal use).
	*/
	private Polygon() {}

// FIXME: We can comment this out to find the bits of code which may need changing.
	public Polygon(Vector3d[] vertices)
	{
		this(vertices, "NULL", null);
	}

	/**
	Constructs a Polygon from an array of vertices, a texture and a texture plane.

	<p><b>Preconditions:</b>
	<dl>
	<dd>vertices != null
	<dd>texture != null
	</dl>

	<p><b>Notes:</b>
	<ul>
	<li><i>Semantic constraint - </i>The vertices must specify a planar polygon.
	<li>If texturePlane == null, a default texture plane is constructed
	</ul>

	@param vertices		The vertices
	@param texture		The texture name
	@param texturePlane	The texture plane
	@throws java.lang.Error	If the preconditions are violated
	*/
	public Polygon(Vector3d[] vertices, String texture, TexturePlane texturePlane)
	{
		if(vertices == null || texture == null) throw new java.lang.Error();

		m_vertices = vertices;
		calculate_normal();
		m_texture = texture;
		set_texture_plane(texturePlane);
	}

	/**
	Constructs a Polygon from an abstract collection of vertices.

	<p><b>Preconditions:</b>
	<dl>
	<dd>vertices != null
	<dd>texture != null
	</dl>

	<p><b>Notes:</b>
	<ul>
	<li><i>Semantic constraint - </i>The vertices must specify a planar polygon.
	<li>If texturePlane == null, a default texture plane is constructed
	</ul>

	@param vertices		The vertices
	@param texture		The texture name
	@param texturePlane	The texture plane
	@throws java.lang.Error	If the preconditions are violated
	*/
	public Polygon(AbstractCollection<Vector3d> vertices, String texture, TexturePlane texturePlane)
	{
		if(vertices == null || texture == null) throw new java.lang.Error();

		m_vertices = new Vector3d[vertices.size()];
		int i = 0;
		for(Vector3d v: vertices)
		{
			m_vertices[i++] = v;
		}
		calculate_normal();
		m_texture = texture;
		set_texture_plane(texturePlane);
	}

	//################## PUBLIC METHODS ##################//
	/**
	Calculates the polygon's normal.

	<p><b>Notes:</b>
	<ul>
	<li>We assume that the polygon is planar. If it is not, this will calculate the normal of the triangle
	specified by the first three vertices instead. If that situation arises, we have major problems anyway.
	</ul>
	*/
	public void calculate_normal()
	{
		m_normal = MathUtil.calculate_normal(m_vertices[0], m_vertices[1], m_vertices[2]);
	}

	/**
	Calculates the centre of the polygon.

	@return	The centre of the polygon as a Point3d
	*/
	public Vector3d centre()
	{
		Vector3d ret = new Vector3d();
		for(Vector3d v: m_vertices) ret.add(v);
		ret.scale(1.0/m_vertices.length);
		return ret;
	}

	/**
	Makes a copy of the polygon.

	@return		A copy of the polygon with vertices and normal distinct in memory from the original
	*/
	public Polygon clone()
	{
		Polygon ret = new Polygon();

		int len = m_vertices.length;
		ret.m_vertices = new Vector3d[len];
		for(int i=0; i<len; ++i) ret.m_vertices[i] = m_vertices[i].clone();
		ret.m_normal = m_normal.clone();
		ret.m_texture = m_texture;
		ret.m_texturePlane = m_texturePlane;

		return ret;
	}

	/**
	Flips the winding order of the polygon's vertices.
	*/
	public void flip_winding()
	{
		// Reverse the vertex array.
		int l = 0, r = m_vertices.length-1;
		while(l < r)
		{
			Vector3d temp = m_vertices[l];
			m_vertices[l] = m_vertices[r];
			m_vertices[r] = temp;
			++l; --r;
		}

		// The normal will now point in the opposite direction.
		m_normal.scale(-1);
	}

	/**
	Gets the polygon's normal.

	@return	...think about it...
	*/
	public Vector3d get_normal()
	{
		return m_normal;
	}

	/**
	Returns the polygon's texture and texture plane.

	@return	...think about it...
	*/
	public Pair<String,TexturePlane> get_texture_details()
	{
		return Pair.make_pair(m_texture, m_texturePlane);
	}

	/**
	Gets the polygon's vertices.

	@return	...think about it...
	*/
	public Vector3d[] get_vertices()
	{
		return m_vertices;
	}

	/**
	Sets the polygon's texture.

	<p><b>Notes:</b>
	<ul>
	<li>If texture == null || TextureManager.instance().get_texture(texture) == null, we use the default NULL texture
	</ul>

	@param texture	The new texture for the polygon
	*/
	public void set_texture(String texture)
	{
		if(texture != null && TextureManager.instance().get_texture(texture) != null) m_texture = texture;
		else m_texture = "NULL";
	}

	/**
	Sets the polygon's texture plane.

	<p><b>Notes:</b>
	<ul>
	<li>If texturePlane == null, an uninitialised texture plane is constructed
	</ul>

	@param texturePlane	The new texture plane for the polygon
	*/
	public void set_texture_plane(TexturePlane texturePlane)
	{
		if(texturePlane != null) m_texturePlane = texturePlane;
		else m_texturePlane = new TexturePlane.Uninitialised(m_normal);
	}

	/**
	Returns a String representation of the polygon.

	@return	...think about it...
	*/
	public String toString()
	{
		StringBuffer sb = new StringBuffer("[");
		for(int i=0, len=m_vertices.length; i<len; ++i)
		{
			sb.append(m_vertices[i].toString());
			if(i < len-1) sb.append(" ");
		}
		sb.append("]");
		return sb.toString();
	}
}