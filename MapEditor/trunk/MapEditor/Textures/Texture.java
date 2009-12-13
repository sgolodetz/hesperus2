package MapEditor.Textures;

import MapEditor.Misc.Pair;
import java.awt.Image;
import java.nio.ByteBuffer;
import java.util.LinkedList;
import net.java.games.jogl.*;

/**
This class represents textures. Note that there is no way to instantiate textures from outside
the package - this is to ensure that all textures are managed by a TextureManager. To obtain
a texture, use the load_texture or load_textures methods of a TextureManager object.
*/
public class Texture
{
	//################## PRIVATE VARIABLES ##################//
	private ByteBuffer m_buffer;
	private Image m_thumbnail;
	private int m_height;
	private int m_width;
	private LinkedList<Pair<GL,Integer>> m_ids = new LinkedList<Pair<GL,Integer>>();	// corresponding (OpenGL Context, Texture ID) pairs
	private String m_filename;

	//################## PRIVATE METHODS ##################//
	/**
	Creates the texture in the OpenGL context specified.

	<p><b>Preconditions:</b>
	<dl>
	<dd>There is no element p of m_ids such that p.first == gl
	</dl>

	@param gl	The OpenGL context in which to create the texture
	@param glu	The corresponding GLU context
	@return		The texture's ID in this OpenGL context
	*/
	private int create(GL gl, GLU glu)
	{
		int[] textures = new int[1];
		gl.glGenTextures(1, textures);

		gl.glBindTexture(GL.GL_TEXTURE_2D, textures[0]);
		glu.gluBuild2DMipmaps(GL.GL_TEXTURE_2D, GL.GL_RGB, m_width, m_height, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, m_buffer);

		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST_MIPMAP_LINEAR);
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, GL.GL_REPEAT);
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, GL.GL_REPEAT);

		m_ids.add(Pair.make_pair(gl, textures[0]));
		return textures[0];
	}

	/**
	Returns the texture's ID in the specified OpenGL context.

	@param gl	The OpenGL context for which to get the texture's ID
	@param glu	The corresponding GLU context
	@return		The relevant texture ID, as specified
	*/
	private int get_texture_id(GL gl, GLU glu)
	{
		// Try to find an existing (gl, <identifier>) pair.
		for(Pair<GL,Integer> p: m_ids)
		{
			if(p.first == gl) return p.second;
		}

		// If we didn't find one, create one and return the identifier.
		return create(gl, glu);
	}

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a texture from a ByteBuffer and a pair of dimensions. We also pass
	the filename so that we can keep track of where the texture came from.

	@param buffer		The buffer containing the texture data (in unsigned byte format)
	@param width		The width of the texture
	@param height		The height of the texture
	@param filename		The name of the file from which the texture was loaded
	@param thumbnail	A thumbnail of the texture
	*/
	Texture(ByteBuffer buffer, int width, int height, String filename, Image thumbnail)
	{
		m_buffer = buffer;
		m_width = width;
		m_height = height;
		m_filename = filename;
		m_thumbnail = thumbnail;
	}

	//################## PACKAGE METHODS ##################//
	/**
	Deletes the texture from every OpenGL context with which it's associated.
	*/
	void unload()
	{
		for(Pair<GL,Integer> p: m_ids) p.first.glDeleteTextures(1, new int[] { p.second });
		m_ids.clear();
	}

	//################## PUBLIC METHODS ##################//
	/**
	Makes the texture current in the OpenGL context specified.

	@param gl	The OpenGL context in which to make the texture current
	@param glu	The corresponding GLU context
	*/
	public void bind(GL gl, GLU glu)
	{
		gl.glBindTexture(GL.GL_TEXTURE_2D, get_texture_id(gl, glu));
	}

	/**
	Returns the name of the file from which this texture was loaded.

	@return	...think about it...
	*/
	public String get_filename()
	{
		return m_filename;
	}

	/**
	Returns the height of the texture.

	@return	...think about it...
	*/
	public int get_height()
	{
		return m_height;
	}

	/**
	Returns the texture's thumbnail.

	@return	...think about it...
	*/
	public Image get_thumbnail()
	{
		return m_thumbnail;
	}

	/**
	Returns the width of the texture.
	*/
	public int get_width()
	{
		return m_width;
	}
}