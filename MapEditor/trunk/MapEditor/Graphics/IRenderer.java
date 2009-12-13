package MapEditor.Graphics;

import MapEditor.Geom.AxisPair;
import MapEditor.Math.Vectors.*;
import java.awt.*;

/**
This class insulates the outside world from the details of how things are rendered in
pixels on a particular rendering surface. It allows us to render in level coordinates
rather than pixels. Rendering surfaces pass implementations of IRenderer to things
which need to render onto them, rather than passing (for instance) an AWT Graphics
object to use.
*/
public interface IRenderer
{
	/**
	Adds a 2D offset in Pixels to a 2D point in Coords. The mapping between Pixels and
	Coords is dependent on the rendering surface (e.g. a DesignCanvas) associated with
	this renderer.

	<p><b>Notes:</b>
	<ul>
	<li>The returned Vector2d, call it q, is guaranteed to satisfy q != p, in other words
	it is distinct in memory from the point passed in as a parameter.
	</ul>

	@param p	The 2D point in Coords to which to add the offset
	@param dx	The x offset in Pixels
	@param dy	The y offset in Pixels
	@return		A 2D point in Coords representing the result of adding the offset to p
	*/
	Vector2d add_pixel_offset(final Vector2d p, double dx, double dy);

	/**
	Returns an integer constant specifying the horizontal axis. (For use with
	the is_inverted_axis method.)

	@return		...think about it...
	*/
	int axis_h();

	/**
	Returns an integer constant specifying the vertical axis. (For use with
	the is_inverted_axis method).
	*/
	int axis_v();

	/**
	Returns the grid size multiple nearest to the specified size in Coords. This is
	based on the current grid size of the rendering surface (e.g. a DesignCanvas)
	associated with this renderer.

	<p><b>Example:</b>
	<dl>
	<dd>If size == 37 and the grid size is 16, then the nearest grid size multiple is 2*16 = 32
	</dl>

	@param size	The size for which we want to find the nearest grid size multiple
	@return		The nearest grid size multiple as a double
	*/
	double determine_nearest_grid_size_multiple_in_coords(final double size);

	/**
	Returns the square of the distance (in Pixels) between the mappings of the two
	specified points onto the rendering surface (e.g. a DesignCanvas) associated with
	this renderer. Or, to give an operational specification, this method converts both
	points in Coords into points in Pixels using the relevant method from the rendering
	surface with which this renderer is associated and then calculates the square of the
	distance between the points in Pixels.

	@param p1	The first point
	@param p2	The second point
	@return		The square of the distance between their conversions into Pixels as a double
	*/
	double distance_squared(final Vector2d p1, final Vector2d p2);

	/**
	Returns the square of the minimum distance (in Pixels) between the mappings of p and
	the line segment e1-e2 onto the rendering surface (e.g. a DesignCanvas) associated with
	this renderer. Or, to give an operational specification, this method calculates the
	minimum distance squared between the point and the line segment in Coords^2, then converts
	the result into Pixels^2.

	@param p	The point whose distance squared from the line segment is to be determined
	@param e1	One endpoint of the line segment
	@param e2	The other endpoint of the line segment
	@return		The distance squared in Pixels^2 as specified
	*/
	double distance_squared_from_linesegment(final Vector2d p, final Vector2d e1, final Vector2d e2);

	/**
	Draws a line segment on the rendering surface (e.g. a DesignCanvas) for which
	the renderer was created. The endpoints of the line are specified in 2D Coords.

	@param p1	One endpoint of the line segment
	@param p2	The other endpoint of the line segment
	*/
	void draw_line(final Vector2d p1, final Vector2d p2);

	/**
	Draws a line segment on the rendering surface (e.g. a DesignCanvas) for which
	the renderer was created. The endpoints of the line are specified in 3D Coords.

	@param p1	One endpoint of the line segment
	@param p2	The other endpoint of the line segment
	*/
	void draw_line(final Vector3d p1, final Vector3d p2);

	/**
	Draws the outline of the oval bounded by the specified points in 2D Coords.

	@param p1	One corner of the oval bounds
	@param p2	The other corner of the oval bounds
	*/
	void draw_oval(final Vector2d p1, final Vector2d p2);

	/**
	Draws the outline of the oval bounded by the specified points in 3D Coords.

	@param p1	One corner of the oval bounds
	@param p2	The other corner of the oval bounds
	*/
	void draw_oval(final Vector3d p1, final Vector3d p2);

	/**
	Draws a sequence of connected lines defined by an array of points in 2D Coords.
	If ps.length == n, it draws lines from ps[0] to ps[1], from ps[1] to ps[2], ...,
	from ps[n-2] to ps[n-1].

	@param ps	An array of 2D points which together specify the lines to draw
	*/
	void draw_polyline(final Vector2d[] ps);

	/**
	Draws the outline of the rectangle bounded by the specified points in 2D Coords.

	@param p1	One corner of the rectangle bounds
	@param p2	The other corner of the rectangle bounds
	*/
	void draw_rectangle(final Vector2d p1, final Vector2d p2);

	/**
	Fills the oval bounded by the specified points in 2D Coords.

	@param p1	One corner of the oval bounds
	@param p2	The other corner of the oval bounds
	*/
	void fill_oval(final Vector2d p1, final Vector2d p2);

	/**
	Finds the nearest grid intersection to the specified point. This depends on the
	current grid of the rendering surface (e.g. a DesignCanvas) associated with the
	renderer.

	@param p	The point whose nearest grid intersection we want to find
	@return		The 2D point in Coords containing the nearest grid intersection
	*/
	Vector2d find_nearest_grid_intersect_in_coords(final Vector2d p);

	/**
	Returns the axis pair associated with the rendering surface (e.g. a DesignCanvas)
	for which the renderer was created.

	@return		...think about it...
	*/
	AxisPair get_axis_pair();

	/**
	Gets the current brush stroke.

	@return		...think about it...
	*/
	Stroke get_stroke();

	/**
	Returns whether the specified axis on the rendering surface (e.g. a DesignCanvas)
	associated with the renderer is inverted.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The axis parameter must be one of the two values returned by axis_h() and axis_v()
	</dl>

	@param axis		The axis we want to check for inversion
	@return			A boolean indicating whether the specified axis is inverted
	*/
	boolean is_inverted_axis(int axis);

	/**
	Returns the distance in Pixels between two coordinates in Coords on the same axis.
	In practice, this means converting Math.abs(y - x) from Coords to Pixels.

	@param x	One of the coordinates in Coords
	@param y	The other coordinate in Coords
	@return		The distance in Pixels between them as specified above
	*/
	double pixel_distance(final double x, final double y);

	/**
	Sets the rendering colour to use.

	@param c		The new rendering colour
	*/
	void set_colour(final Color c);

	/**
	Sets the cursor.

	@param c		The new cursor
	*/
	void set_cursor(final Cursor c);

	/**
	Sets the brush stroke to use.

	@param stroke	The new brush stroke
	*/
	void set_stroke(final Stroke stroke);
}