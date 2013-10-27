package net.gxstudios.shipwreck.misc;

import java.util.*;

/**
This class manages Boolean editor options.
*/
public final class Options
{
	//################## PRIVATE VARIABLES ##################//
	/** The map which stores internally-settable options */
	private static TreeMap<String,Boolean> s_internalOptionsMap = new TreeMap<String,Boolean>();

	/** The map which stores user-settable options */
	private static TreeMap<String,Boolean> s_optionsMap = new TreeMap<String,Boolean>();

	static
	{
		// Do we want to render the texture mask?
		s_internalOptionsMap.put("Render Texture Mask", true);

		// Do we want to be in texture-editing mode?
		s_internalOptionsMap.put("Texture-Editing Mode", false);

		// Do we want brushes to be automatically added to the map when we create them?
		s_optionsMap.put("Automatic Brush Deghosting", true);

		// Do we want brushes in the 3D view to be flat-shaded when not in texture or wireframe mode?
		s_optionsMap.put("Flat-Shading", false);

		// Do we want to invert the mouse's y axis when looking around in the 3D view?
		s_optionsMap.put("Invert Mouse Y Axis", false);

		// Do we want to try and preserve texture coordinates as much as possible when transforming?
		// (There are times when it makes sense to turn this off, if only temporarily. An example
		//  might be when we're trying to copy something from one place to another.)
		s_optionsMap.put("Preserve Texture Coordinates", true);

		// Do we want to render the brush creation anchor?
		s_optionsMap.put("Render Brush Creation Anchor", false);

		// Do we want to render detail brushes in a easily distinguishable way in the 3D view?
		s_optionsMap.put("Render Distinguishable Details", true);

		// Do we want to render hint brushes?
		s_optionsMap.put("Render Hint Brushes", true);

		// Do we want to render all the brushes in the same colour?
		s_optionsMap.put("Render In Unicolour", false);

		// Do we want to render brushes in the 3D view in wireframe?
		s_optionsMap.put("Render In Wireframe", false);

		// Do we want to render the falloff radii for the lights?
		s_optionsMap.put("Render Light Falloff Radii", false);

		// Do we want to render polygon normals?
		s_optionsMap.put("Render Polygon Normals", false);

		// Do we want our entities to be textured?
		s_optionsMap.put("Render Textures", false);

		// Do we want to snap to the grid when editing?
		s_optionsMap.put("Snap To Grid", true);
	}

	//################## PUBLIC METHODS ##################//
	/**
	Returns the set of user-settable options.

	@return	...think about it...
	*/
	public static Set<String> get()
	{
		return s_optionsMap.keySet();
	}

	/**
	Returns whether the specified (existing) option is set.

	@param option			The option whose value we want to check
	@return					A boolean indicating whether the option is set
	@throws java.lang.Error	If option == null or the option doesn't exist
	*/
	public static boolean is_set(String option)
	{
		if(option == null) throw new java.lang.Error();

		Boolean b = s_optionsMap.get(option);
		if(b != null) return b;

		b = s_internalOptionsMap.get(option);
		if(b != null) return b;

		throw new java.lang.Error();
	}

	/**
	Sets the boolean value of the specified (existing) user-settable option.

	@param option			The option to set
	@param value			The new value
	@throws java.lang.Error	If the option doesn't already exist
	*/
	public static void set(String option, boolean value)
	{
		// Only allow modifications to existing options; prevent new ones being added.
		if(!s_optionsMap.containsKey(option)) throw new java.lang.Error();

		s_optionsMap.put(option, value);
	}

	/**
	Sets the boolean value of the specified (existing) internal option.

	@param option			The option to set
	@param value			The new value
	@throws java.lang.Error	If the option doesn't already exist
	*/
	public static void set_internal(String option, boolean value)
	{
		// Only allow modifications to existing options; prevent new ones being added.
		if(!s_internalOptionsMap.containsKey(option)) throw new java.lang.Error();

		s_internalOptionsMap.put(option, value);
	}
}