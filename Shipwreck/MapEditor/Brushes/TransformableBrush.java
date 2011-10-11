package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisAlignedBox;
import MapEditor.Geom.AxisPair;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.MathUtil;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Stroke;

/**
This is the base class for brushes which can be generally transformed (in addition
to being resized or translated, they can also be rotated or sheared).
*/
abstract class TransformableBrush extends ResizableTranslatableBrush implements Constants
{	
	//################## CONSTRUCTORS ##################//
	/**
	Constructs a TransformableBrush whose initial state depends on whether or not it's
	a new brush. By "new", I mean one the user is creating, as opposed to one which we're
	loading in, one which is being constructed as a result of a split operation, etc.

	@param isNew	Is the brush a new one that the user is creating?
	*/
	public TransformableBrush(boolean isNew)
	{
		super(isNew);
	}

	//################## PROTECTED ABSTRACT METHODS ##################//
	/**
	Rotates the brush through the specified angle. Exactly how we do this depends on the brush type.

	@param centre	The centre about which to rotate (as a 2D point on the canvas
					on which we're rotating)
	@param angle	The angle through which to rotate, in the range (-PI,PI]
	@param ap		The axis-pair of the above canvas
	*/
	protected abstract void rotate(Vector2d centre, double angle, AxisPair ap);

	/**
	Shears the brush by the specified factor in the horizontal direction on the axes given by the
	AxisPair.

	@param anchor	The anchor of the shear (i.e. the origin of the transformation)
	@param factor	The shear factor
	@param ap		The axis-pair of the canvas on which we're shearing
	*/
	protected abstract void shear_horizontal(Vector2d anchor, double factor, AxisPair ap);

	//################## PROTECTED METHODS ##################//
	protected Transformation generate_handle_transformation(int handleIndex)
	{
		switch(m_handleState)
		{
			case RESIZE:
				return new HandleResizeTransformation(handleIndex);
			case ROTATE:
				return new RotationTransformation(handleIndex);
			case SHEAR:
				return new ShearTransformation(handleIndex);
		}

		throw new java.lang.Error();	// we should never get here
	}

	protected void right_mouse_pressed()
	{
		switch(m_handleState)
		{
			case RESIZE:
				m_handleState = HandleState.ROTATE;
				break;
			case ROTATE:
				m_handleState = HandleState.SHEAR;
				break;
			case SHEAR:
				m_handleState = HandleState.RESIZE;
				break;
		}

		super.right_mouse_pressed();
	}

	/**
	Shears the brush by the specified factor in the vertical direction on the axes given by the
	AxisPair, anchored at the specified anchor.

	@param anchor	The anchor of the shear (i.e. the origin of the transformation)
	@param factor	The shear factor
	@param ap		The axis-pair of the canvas on which we're shearing
	*/
	final protected void shear_vertical(Vector2d anchor, double factor, AxisPair ap)
	{
		// We flip the axes round and just do a horizontal shear in the new coordinate system.
		// Note that this entails converting the anchor into the new coordinate system as well!
		Vector2d flippedAnchor = new Vector2d(anchor.y, anchor.x);
		shear_horizontal(flippedAnchor, factor, ap.flip());
	}

	//################## PRIVATE NESTED CLASSES ##################//
	private class RotationTransformation extends Transformation
	{
		private double m_angle;					// the angle by which we've rotated at present
		private Vector2d m_handle;				// the initial location of the handle we're using to rotate

		/**
		Constructs a rotation transformation for the brush.

		@param handleIndex	The index of the handle with which we're going to rotate
		*/
		public RotationTransformation(int handleIndex)
		{
			begin_transform();

			m_angle = 0;
			m_handle = m_boundingBox.get_corner_handle(m_renderer.get_axis_pair(), handleIndex);
		}

		public void execute_command()
		{
			if(m_angle == 0) return;		// don't bother with no-op rotations

			final AxisPair ap = m_renderer.get_axis_pair();
			final Vector2d centre = ap.select_components(m_boundingBox.centre());
			final double angle = m_angle;

			rotate(centre, 0, ap);		// reset the brush to its pre-rotation state

			CommandManager.instance().execute_command(new Command("Rotation")
			{
				public void execute()
				{
					begin_transform();
					rotate(centre, angle, ap);
					end_transform();
				}

				public void undo()
				{
					begin_transform();
					rotate(centre, -angle, ap);
					end_transform();
				}
			});
		}

		public void transform(Vector2d p)
		{
			AxisPair ap = m_renderer.get_axis_pair();

			Vector2d c = ap.select_components(m_boundingBox.centre());

			Vector2d u = VectorUtil.subtract(m_handle, c);
			Vector2d v = VectorUtil.subtract(p, c);

			m_angle = MathUtil.signed_angle_between(u,v);

			m_angle = Math.toRadians(Math.round(Math.toDegrees(m_angle)));

			// TODO: Snap the angle to 45 degree marks if it's close enough.
			double angleDegrees = Math.toDegrees(m_angle);

			Vector2d rotationCentre = ap.select_components(m_boundingBox.centre());
			rotate(rotationCentre, m_angle, ap);
		}
	}

	private class ShearTransformation extends Transformation
	{
		// For the actual transformation
		final private Axis m_axis;				// which axis we're shearing along (horizontal or vertical)
		final private Vector2d m_anchor;			// the location of the anchor for our shear transformation
		final private Vector2d m_handle;			// the initial location of the handle we're using to shear
		private double m_factor;				// the shear factor (obviously)

		// For the rendering effects
		final private Vector2d m_anchorCorner;	// the corner with the same index as the anchor
		final private Vector2d m_handleCorner;	// the corner with the same index as the handle
		private double m_displacement;			// the shear displacement (used to calculate the shear factor, but needed itself for rendering)

		/**
		Constructs a shear transformation for the brush.

		@param handleIndex	The index of the handle with which we're going to shear
		*/
		public ShearTransformation(int handleIndex)
		{
			begin_transform();

			AxisPair ap = m_renderer.get_axis_pair();

			m_axis = (handleIndex % 2 == 0) ? Axis.X : Axis.Y;
			m_anchor = m_boundingBox.get_opposite_edge_handle(ap, handleIndex);
			m_handle = m_boundingBox.get_edge_handle(ap, handleIndex);
			m_factor = 0;

			m_anchorCorner = m_boundingBox.get_opposite_corner_handle(ap, handleIndex);
			m_handleCorner = m_boundingBox.get_corner_handle(ap, handleIndex);
			m_displacement = 0;
		}

		public void execute_command()
		{
			if(m_factor == 0) return;	// don't bother with no-op shears

			final AxisPair ap = m_renderer.get_axis_pair();
			final Vector2d anchor = m_anchor.clone();
			final double factor = m_factor;

			switch(m_axis)
			{
				case X:
				{
					shear_horizontal(anchor, 0, ap);	// reset the brush to its pre-shear state

					CommandManager.instance().execute_command(new Command("Horizontal Shear")
					{
						public void execute()
						{
							begin_transform();
							shear_horizontal(anchor, factor, ap);
							end_transform();
						}

						public void undo()
						{
							begin_transform();
							shear_horizontal(anchor, -factor, ap);
							end_transform();
						}
					});
					break;
				}
				case Y:
				{
					shear_vertical(anchor, 0, ap);		// reset the brush to its pre-shear state

					CommandManager.instance().execute_command(new Command("Vertical Shear")
					{
						public void execute()
						{
							begin_transform();
							shear_vertical(anchor, factor, ap);
							end_transform();
						}

						public void undo()
						{
							begin_transform();
							shear_vertical(anchor, -factor, ap);
							end_transform();
						}
					});
					break;
				}
			}
		}

		public void render_effects(IRenderer renderer)
		{
			/*
			Renders a sheared bounding box so the user can see what's happening, provided
			the specified renderer is the one associated with the canvas on which we're
			manipulating the brush. Otherwise does nothing.
			*/
			if(renderer == m_renderer)
			{
				renderer.set_colour(Color.white);

				float[] dash = {5.0f};
				Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_MITER, 10.0f, dash, 0.0f);
				renderer.set_stroke(stroke);

				switch(m_axis)
				{
					case X:
					{
						Vector2d[] ps = new Vector2d[5];
						ps[0] = m_anchorCorner;
						ps[1] = new Vector2d(m_handleCorner.x, m_anchorCorner.y);
						ps[2] = new Vector2d(m_handleCorner.x + m_displacement, m_handleCorner.y);
						ps[3] = new Vector2d(m_anchorCorner.x + m_displacement, m_handleCorner.y);
						ps[4] = ps[0];
						renderer.draw_polyline(ps);
						break;
					}
					case Y:
					{
						Vector2d[] ps = new Vector2d[5];
						ps[0] = m_anchorCorner;
						ps[1] = new Vector2d(m_anchorCorner.x, m_handleCorner.y);
						ps[2] = new Vector2d(m_handleCorner.x, m_handleCorner.y + m_displacement);
						ps[3] = new Vector2d(m_handleCorner.x, m_anchorCorner.y + m_displacement);
						ps[4] = ps[0];
						renderer.draw_polyline(ps);
						break;
					}
				}

				renderer.set_stroke(new BasicStroke());
			}
		}

		public void transform(Vector2d p)
		{
			switch(m_axis)
			{
				case X:
				{
					m_displacement = p.x - m_handle.x;
					if(Options.is_set("Snap To Grid"))
					{
						m_displacement = m_renderer.determine_nearest_grid_size_multiple_in_coords(m_displacement);
					}
					m_factor = m_displacement / (m_handle.y - m_anchor.y);
					shear_horizontal(m_anchor, m_factor, m_renderer.get_axis_pair());
					break;
				}
				case Y:
				{
					m_displacement = p.y - m_handle.y;
					if(Options.is_set("Snap To Grid"))
					{
						m_displacement = m_renderer.determine_nearest_grid_size_multiple_in_coords(m_displacement);
					}
					m_factor = m_displacement / (m_handle.x - m_anchor.x);
					shear_vertical(m_anchor, m_factor, m_renderer.get_axis_pair());
					break;
				}
			}
		}
	}
}