package MapEditor.Textures;

/**
This interface defines global texture constants. Classes which want to import
the constants, i.e. use unqualified names like THUMBNAIL_WIDTH instead of
TextureConstants.THUMBNAIL_WIDTH, simply have to declare themselves as
implementing the TextureConstants interface. Whether it's a good idea to use
the unqualified names is a design decision, of course.
*/
public interface TextureConstants
{
	final public static int THUMBNAIL_HEIGHT = 90;
	final public static int THUMBNAIL_WIDTH = 90;
}