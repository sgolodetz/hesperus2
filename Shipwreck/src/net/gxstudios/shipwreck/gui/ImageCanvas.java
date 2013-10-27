package net.gxstudios.shipwreck.gui;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;

/**
This class implements a canvas that is completely filled by a given image. It's used
for rendering thumbnails in the texture dialog.
*/
public class ImageCanvas extends Canvas
{
	//################## PRIVATE VARIABLES ##################//
	private Image m_image = null;

	//################## CONSTRUCTORS ##################//
	public ImageCanvas()
	{
		setBackground(Color.black);
	}

	//################## PUBLIC METHODS ##################//
	public void paint(Graphics g)
	{
		if(m_image != null) g.drawImage(m_image, 0, 0, null);
	}

	/**
	Sets the image to be rendered on the canvas.

	<p><b>Notes:</b>
	<ul>
	<li>image can safely be null
	</ul>

	@param image	The image to be rendered
	*/
	public void set_image(Image image)
	{
		m_image = image;
	}
}