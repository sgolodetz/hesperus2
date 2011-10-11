package MapEditor.Brushes;

import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Constants;

/**
This class contains brush utility methods.
*/
class BrushUtil implements BrushConstants
{
	public static void draw_dot(final IRenderer renderer, final Vector2d centre)
	{
		final double DOT_SIZE = 12;
		Vector2d p1 = renderer.add_pixel_offset(centre, (int)-DOT_SIZE/2, (int)-DOT_SIZE/2);
		Vector2d p2 = renderer.add_pixel_offset(centre, (int)DOT_SIZE/2, (int)DOT_SIZE/2);
		renderer.fill_oval(p1, p2);
	}

	public static void draw_infinite_line(final IRenderer renderer, final Vector2d p1, final Vector2d p2)
	{
		Vector2d delta = VectorUtil.subtract(p2, p1);
		if(delta.length() > Constants.EPSILON)
		{
			delta.normalize();

			final double LARGE_NUMBER = 100000;		// a reasonably arbitrary large number
			Vector2d q1 = Vector2d.scale_add(-LARGE_NUMBER, delta, p1);
			Vector2d q2 = Vector2d.scale_add(LARGE_NUMBER, delta, p2);

			renderer.draw_line(q1, q2);
		}
	}

	public static boolean near_edge_x(IRenderer renderer, Vector2d p, double k, double minY, double maxY)
	{
		// The edge lies on the line x=k, and is restricted to the range [minY,maxY].
		// Hence the point p is near the edge if p.y satisfies minY <= p.y <= maxY and p.x is within
		// a certain distance of k. The choice of HANDLE_SIZE/2 is somewhat arbitrary, but works fairly
		// well in practice.
		if(minY <= p.y && p.y <= maxY && renderer.pixel_distance(p.x, k) <= HANDLE_SIZE/2) return true;
		else return false;
	}

	public static boolean near_edge_y(IRenderer renderer, Vector2d p, double k, double minX, double maxX)
	{
		// The edge lies on the line y=k, and is restricted to the range [minX,maxX].
		// Hence the point p is near the edge if p.x satisfies minX <= p.x <= maxX and p.y is within
		// a certain distance of k. The choice of HANDLE_SIZE/2 is somewhat arbitrary, but works fairly
		// well in practice.
		if(minX <= p.x && p.x <= maxX && renderer.pixel_distance(p.y, k) <= HANDLE_SIZE/2) return true;
		else return false;
	}

	public static boolean near_handle(IRenderer renderer, Vector2d p, double x, double y)
	{
		// (x,y) is the location of the handle, p is the point we want to check for proximity
		// Our test checks whether the mouse is inside the handle bounds.
		if(renderer.pixel_distance(p.x, x) <= HANDLE_SIZE/2 && renderer.pixel_distance(p.y, y) <= HANDLE_SIZE/2) return true;
		else return false;
	}

	/**
	Draws a circular handle centred on the specified point using the specified renderer.

	@param renderer	The renderer with which to draw the handle
	@param centre	The position on which the handle is centred
	*/
	public static void render_circular_handle(IRenderer renderer, Vector2d centre)
	{
		Vector2d p1 = renderer.add_pixel_offset(centre, -HANDLE_SIZE/2-1, -HANDLE_SIZE/2-1);
		Vector2d p2 = renderer.add_pixel_offset(centre, HANDLE_SIZE/2+1, HANDLE_SIZE/2+1);
		renderer.draw_oval(p1, p2);
	}

	/**
	Draws a diamond-shaped handle centred on the specified point using the specified renderer.
	The diamond	has sides of equal length and axis-aligned diagonals. In other words, it is
	an axis-aligned square rotated through 45 degrees.

	@param renderer	The renderer with which to draw the handle
	@param centre	The position on which the handle is centred
	*/
	public static void render_diamond_handle(IRenderer renderer, Vector2d centre)
	{
		Vector2d[] ps = new Vector2d[5];
		ps[0] = renderer.add_pixel_offset(centre, -HANDLE_SIZE/2-1, 0);
		ps[1] = renderer.add_pixel_offset(centre, 0, -HANDLE_SIZE/2-1);
		ps[2] = renderer.add_pixel_offset(centre, HANDLE_SIZE/2+1, 0);
		ps[3] = renderer.add_pixel_offset(centre, 0, HANDLE_SIZE/2+1);
		ps[4] = ps[0];
		renderer.draw_polyline(ps);
	}

	/**
	Draws a square handle centred on the specified point using the specified renderer.

	@param renderer	The renderer with which to draw the handle
	@param centre	The position on which the handle is centred
	*/
	public static void render_square_handle(IRenderer renderer, Vector2d centre)
	{
		Vector2d p1 = renderer.add_pixel_offset(centre, -HANDLE_SIZE/2, -HANDLE_SIZE/2);
		Vector2d p2 = renderer.add_pixel_offset(centre, HANDLE_SIZE/2, HANDLE_SIZE/2);
		renderer.draw_rectangle(p1, p2);
	}
}