package net.gxstudios.shipwreck.graphics;

import java.awt.Color;

/**
This class represents colour cycles. These are used to produce a cyclic sequence of colours.
*/
public class ColourCycle
{
	//################## CONSTANTS ##################//
	final private static Color[] COLOUR_ARRAY = new Color[] { Color.blue, Color.cyan, Color.green, Color.magenta, Color.pink,
															  Color.yellow };

	//################## PRIVATE VARIABLES ##################//
	private int m_colourIndex = 0;

	//################## PUBLIC METHODS ##################//
	/**
	Returns the next colour in the cyclic sequence.

	@return	...think about it...
	*/
	public Color next_colour()
	{
		Color c = COLOUR_ARRAY[m_colourIndex];
		m_colourIndex = (m_colourIndex+1)%COLOUR_ARRAY.length;
		return c;
	}
}