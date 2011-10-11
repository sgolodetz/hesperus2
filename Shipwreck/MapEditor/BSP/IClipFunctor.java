package MapEditor.BSP;

import MapEditor.Geom.Planar.Plane;
import MapEditor.Misc.Pair;

public interface IClipFunctor<T>
{
	int classify_against_plane(final T entity, final Plane plane);

	/**
	Returns a boolean indicating whether the (coplanar) entity should
	be passed down the back side of the current subtree.

	@param entity	The entity in question
	@param plane	The splitting plane at the root node of the current subtree
	@return			true, if it should be passed down the back side of the subtree, or false otherwise
	*/
	boolean pass_coplanar_back(final T entity, final Plane plane);

	Pair<T,T> split_with_plane(final T entity, final Plane plane);
}