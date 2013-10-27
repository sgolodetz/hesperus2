package net.gxstudios.shipwreck.brushes;

import net.gxstudios.shipwreck.bsp.IClipFunctor;
import net.gxstudios.shipwreck.geom.planar.Plane;
import net.gxstudios.shipwreck.misc.Pair;

public class BrushClipFunctor implements IClipFunctor<ArchitectureBrush>
{
	//################## SINGLETON IMPLEMENTATION ##################//
	private BrushClipFunctor() {}		// deliberately prevent instantiation

	private static BrushClipFunctor s_instance = null;

	public static BrushClipFunctor instance()
	{
		if(s_instance == null) s_instance = new BrushClipFunctor();
		return s_instance;
	}

	//################## PUBLIC METHODS ##################//
	public int classify_against_plane(final ArchitectureBrush brush, final Plane plane)
	{
		return brush.classify_against_plane(plane);
	}

	public boolean pass_coplanar_back(final ArchitectureBrush brush, final Plane plane)
	{
		throw new java.lang.Error("Brushes should never lie entirely on a plane!");
	}

	public Pair<ArchitectureBrush,ArchitectureBrush> split_with_plane(final ArchitectureBrush brush, final Plane plane)
	{
		return brush.split_with_plane(plane);
	}
}