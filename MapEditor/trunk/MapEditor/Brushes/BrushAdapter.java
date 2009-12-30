package MapEditor.Brushes;

import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import java.awt.*;
import java.io.PrintWriter;
import javax.media.opengl.*;
import javax.media.opengl.glu.GLU;

/**
This class exists so that brushes that don't need to implement all the IBrush methods
don't have to provide empty implementations of all of them. Instead, they just have
to inherit from BrushAdapter.
*/
abstract class BrushAdapter implements IBrush
{
	public void deghost(IBrushContainer container)								{}
	public BrushDetails details()												{ return new BrushDetails(0); }
	public IBrush deselect(IBrushContainer container)							{ return null; }
	public void flip_x()														{}
	public void flip_y()														{}
	public void flip_z()														{}
	public boolean is_flippable()												{ return false; }
	public void mouse_moved(IRenderer renderer, Vector2d p)						{}
	public void mouse_released()												{}
	public PickResults pick(final Vector3d start, final Vector3d direction)		{ return null; }
	public Dialog properties_dialog(Frame owner)								{ return null; }

	public void render(IRenderer renderer, Color overrideColour)				{ throw new UnsupportedOperationException(); }
	public void render3D(GL2 gl, GLU glu)										{ throw new UnsupportedOperationException(); }

	public void select(IBrushContainer container)								{}

	// Saving
	public void save_MEF2(PrintWriter pw)	{ throw new UnsupportedOperationException(); }
	public void save_MEF3(PrintWriter pw)	{ save_MEF2(pw); }
}