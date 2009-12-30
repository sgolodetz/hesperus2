package MapEditor.Textures;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.*;
import java.nio.ByteBuffer;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import javax.imageio.*;
import javax.media.opengl.*;

/**
This class represents texture managers, which (obviously) manage textures.
*/
public class TextureManager implements TextureConstants
{
	//################## SINGLETON IMPLEMENTATION ##################//
	private TextureManager() {}		// deliberately prevent instantiation

	private static TextureManager s_instance = null;

	public static TextureManager instance()
	{
		if(s_instance == null) s_instance = new TextureManager();
		return s_instance;
	}

	//################## PRIVATE VARIABLES ##################//
	// Note: We could use HashMap here instead, but it's nice to be able to retrieve an ordered list of texture names
	// in MapEditor.GUI.TextureDialog. It's generally more efficient to use HashMap, so we might need to reconsider
	// this if performance becomes a problem.
	private Map<String,Texture> m_textures = new TreeMap<String,Texture>();

	//################## PUBLIC METHODS ##################//
	/**
	Returns a set containing the names of the textures managed by this manager.

	@return	...think about it...
	*/
	public Set<String> get_names()
	{
		return m_textures.keySet();
	}

	/**
	Returns the texture associated with the specified name, if any.

	@param name	The name of the texture to get
	@return		The associated texture, if the manager contains it, or null, if it doesn't
	*/
	public Texture get_texture(String name)
	{
		return m_textures.containsKey(name) ? m_textures.get(name) : null;
	}

	/**
	Loads a texture from the specified file, sets it to be managed by this
	manager, and returns a representative Texture object.

	<p><b>Preconditions:</b>
	<dl>
	<dd>filename != null
	</dl>

	@param name				The internal texture name to use
	@param filename			The filename of the texture to load
	@return					A representation of the texture as a Texture object, or null if the file was not found or was of an invalid type
	@throws java.lang.Error	If the preconditions are violated or there's an IOException while reading the file
	*/
	public Texture load_texture(String name, String filename)
	{
		if(filename == null) throw new java.lang.Error();

		try
		{
			BufferedImage bi = ImageIO.read(new FileInputStream(filename));

			// TODO: Check that null is what ImageIO.read returns if it couldn't read an image.
			if(bi == null) return null;

			int width = bi.getWidth();
			int height = bi.getHeight();

			int[] pixels = null;
			pixels = bi.getRaster().getPixels(0, 0, width, height, pixels);

			ByteBuffer buffer = ByteBuffer.allocateDirect(width * height * 3);
			for(int y=0; y<height; ++y)
			{
				for(int x=0; x<width; ++x)
				{
					int offset = (y*width + x) * 3;
					buffer.put((byte)pixels[offset]);
					buffer.put((byte)pixels[offset+1]);
					buffer.put((byte)pixels[offset+2]);
				}
			}

			// Generate the thumbnail.
			Image thumbnail = bi.getScaledInstance(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, Image.SCALE_SMOOTH);

			Texture texture = new Texture(buffer, width, height, filename, thumbnail);
			m_textures.put(name, texture);
			return texture;
		}
		catch(FileNotFoundException e)
		{
			return null;
		}
		catch(IOException e)
		{
			throw new java.lang.Error();
		}
	}

	/**
	Loads textures from the specified files, sets them to be managed by this
	manager, and returns an array of representative Texture objects.

	<p><b>Preconditions:</b>
	<dl>
	<dd>None of the filenames can be null
	<dd>names.length == filenames.length
	</dl>

	@param filenames		The filenames of the textures to load
	@return					An array of Texture objects, as described, some of which may be null if the file was not found
	@throws java.lang.Error	If the preconditions are violated
	*/
	public Texture[] load_textures(String[] names, String[] filenames)
	{
		if(names.length != filenames.length) throw new java.lang.Error();

		int len = names.length;
		Texture[] ret = new Texture[len];
		for(int i=0; i<len; ++i)
		{
			ret[i] = load_texture(names[i], filenames[i]);
		}
		return ret;
	}

	/**
	Unloads the specified texture if it's managed by this manager.

	@param texture	The texture to unload
	@return			true, if the texture was successfully unloaded, or false if it was null or was not managed by this manager
	*/
	public boolean unload(Texture texture)
	{
		if(texture == null) return false;

		if(m_textures.containsValue(texture))
		{
			texture.unload();
			m_textures.remove(texture);
			return true;
		}
		else return false;
	}

	/**
	Unloads all textures managed by this texture manager. Should always be called on program termination.
	*/
	public void unload_all()
	{
		for(Texture texture: m_textures.values()) texture.unload();
		m_textures.clear();
	}
}