package MapEditor.BSP;

import MapEditor.Geom.Planar.*;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.MiscUtil;
import MapEditor.Misc.Pair;
import MapEditor.Test.*;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;

public class TreeUtil implements GeomConstants
{
	//################## CONSTANTS ##################//
	final public static int CPTT_EMPTY_FRAGMENTS = 0;
	final public static int CPTT_SOLID_FRAGMENTS = 1;

	//################## PUBLIC METHODS ##################//
	/**
	Clips an entity to a BSP tree and returns the entity fragments that are in the specified
	type of leaf. If fragmentType == CPTT_EMPTY_FRAGMENTS, this returns all fragments that end
	up in the empty leaves; if fragmentType == CPTT_SOLID_FRAGMENTS, it returns all fragments
	that end up in the solid leaves.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The BSP tree must be valid (i.e. there is no ambiguity in the solidness of a leaf)
	<dd>fragmentType == CPTT_EMPTY_FRAGMENTS or CPTT_SOLID_FRAGMENTS
	</dl>

	@param entity			The entity to clip
	@param clipFunctor		The functor which allows us to classify and split entities of this type
	@param tree				The BSP tree to clip against
	@param fragmentType		The type of fragments we're after
	@return					A pair consisting of the fragments of the entity which survived
							being clipped against the tree and a boolean indicating whether
							the whole entity which got passed in survived the clipping process
							(i.e. whether if we combined all the surviving fragments, we'd
							get the original entity)
	@throws java.lang.Error	If an invalid value is passed for fragmentType
	*/
	public static <T> Pair<LinkedList<T>,Boolean> clip_to_tree(final T entity, final IClipFunctor<T> clipFunctor, final Tree tree, int fragmentType)
	{
		if(fragmentType != CPTT_EMPTY_FRAGMENTS && fragmentType != CPTT_SOLID_FRAGMENTS) throw new java.lang.Error();

		return clip_to_subtree(entity, clipFunctor, tree.get_root(), fragmentType);
	}

	/**
	Generates a set of polygons which surround a convex volume from the specified set of planes which bound the volume.

	<p><b>Preconditions:</b>
	<dl>
	<dd>planes != null && planes.iterator().hasNext()
	</dl>

	@param planes			The set of bounding planes
	@return					The set of bounding polygons as a linked list
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static LinkedList<Polygon> generate_convex_volume_polygons_from_surrounding_planes(final Iterable<Plane> planes)
	{
		// Check the preconditions.
		if(planes == null || !planes.iterator().hasNext()) throw new java.lang.Error();

		// Main algorithm
		Tree tree = Tree.construct_right_linear_tree(planes);

		LinkedList<Polygon> ret = new LinkedList<Polygon>();
		final PolygonClipFunctor pcfPassBack = PolygonClipFunctor.complex(true, true);	// pass all coplanar polygons down the back of the tree
		for(Plane plane: planes)
		{
			Polygon universePoly = GeomUtil.make_universe_polygon(plane);
			Pair<LinkedList<Polygon>,Boolean> result = clip_to_tree(universePoly, pcfPassBack, tree, CPTT_SOLID_FRAGMENTS);
			ret.addAll(result.first);
		}

		return ret;
	}

	/**
	TODO: Comment here.
	*/
	public static LinkedList<Polygon> generate_convex_volume_polygons_from_surrounding_polygons(final Iterable<Polygon> polys)
	{
		LinkedList<Plane> planes = new LinkedList<Plane>();
		for(Polygon poly: polys) planes.add(GeomUtil.make_plane(poly));

		Tree tree = Tree.construct_right_linear_tree(planes);

		LinkedList<Polygon> ret = new LinkedList<Polygon>();
		final PolygonClipFunctor pcfPassBack = PolygonClipFunctor.complex(true, true);	// pass all coplanar polygons down the back of the tree
		for(Polygon poly: polys)
		{
			// Generate a large polygon in the same plane as poly and with the same texture and texture plane.
			Polygon universePoly = GeomUtil.make_universe_polygon(GeomUtil.make_plane(poly));
			universePoly.set_texture(poly.get_texture_details().first);
			universePoly.set_texture_plane(poly.get_texture_details().second);

			Pair<LinkedList<Polygon>,Boolean> result = clip_to_tree(universePoly, pcfPassBack, tree, CPTT_SOLID_FRAGMENTS);
			ret.addAll(result.first);
		}

		return ret;
	}

	/**
	Generates a list of convex volumes from the specified tree, where here a convex volume means in practice
	the list of polygons surrounding it. Convex volumes are defined by the solid leaves of the tree, only.

	<p><b>Preconditions:</b>
	<dl>
	<dd>tree != null
	<dd>The tree must be valid, in the sense that each of its solid leaves must define a finite convex subspace
	</dl>

	@param tree				The tree from which to generate the convex volumes
	@return					A linked list of convex volumes, as specified
	@throws java.lang.Error	If tree == null
	*/
	public static LinkedList<LinkedList<Polygon>> generate_convex_volumes_from_tree(final Tree tree)
	{
		if(tree == null) throw new java.lang.Error();

		return generate_convex_volumes_from_subtree(tree.get_root());
	}

	//################## PRIVATE METHODS ##################//
	/**
	Clips an entity to a subtree and returns the polygon fragments that are in the specified
	type of leaf. If fragmentType == CPTT_EMPTY_FRAGMENTS, this returns all fragments that end
	up in the empty leaves; if fragmentType == CPTT_SOLID_FRAGMENTS, it returns all fragments
	that end up in the solid leaves.

	<p><b>Preconditions:</b>
	<dl>
	<dd>fragmentType == CPTT_EMPTY_FRAGMENTS or CPTT_SOLID_FRAGMENTS
	</dl>

	@param entity			The entity to clip
	@param clipFunctor		The functor which allows us to classify and split entities of this type
	@param node				The root of the subtree to clip against
	@param fragmentType		The type of fragments we're after
	@return					A pair consisting of the fragments of the entity which survived
							being clipped against the specified subtree and a boolean indicating
							whether the whole entity which got passed in survived the clipping
							process (i.e. whether if we combined all the surviving fragments,
							we'd get the original entity)
	*/
	private static <T> Pair<LinkedList<T>, Boolean> clip_to_subtree(final T entity, final IClipFunctor<T> clipFunctor, final INode node, int fragmentType)
	{
		if(node.is_leaf())
		{
			ILeafNode leafNode = (ILeafNode)node;
			Pair<LinkedList<T>, Boolean> ret = new Pair<LinkedList<T>, Boolean>();
			ret.first = new LinkedList<T>();
			ret.second = false;
			if((leafNode.is_empty() && fragmentType == CPTT_EMPTY_FRAGMENTS) ||
			   (leafNode.is_solid() && fragmentType == CPTT_SOLID_FRAGMENTS))
			{
				ret.first.add(entity);
				ret.second = true;
			}
			return ret;
		}

		// If we get here, the node wasn't a leaf node, so it must be a branch node.
		IBranchNode branchNode = (IBranchNode)node;
		switch(clipFunctor.classify_against_plane(entity, branchNode.splitter()))
		{
			case CP_BACK:
			{
				return clip_to_subtree(entity, clipFunctor, branchNode.right(), fragmentType);
			}
			case CP_COPLANAR:
			{
				if(clipFunctor.pass_coplanar_back(entity, branchNode.splitter()))	// the clip functor's method determines which side we pass the entity down
				{
					return clip_to_subtree(entity, clipFunctor, branchNode.right(), fragmentType);
				}
				else
				{
					return clip_to_subtree(entity, clipFunctor, branchNode.left(), fragmentType);
				}
			}
			case CP_FRONT:
			{
				return clip_to_subtree(entity, clipFunctor, branchNode.left(), fragmentType);
			}
			case CP_STRADDLE:
			{
				Pair<T,T> p = clipFunctor.split_with_plane(entity, branchNode.splitter());
				Pair<LinkedList<T>, Boolean> fragments = clip_to_subtree(p.first, clipFunctor, branchNode.left(), fragmentType);
				Pair<LinkedList<T>, Boolean> backFragments = clip_to_subtree(p.second, clipFunctor, branchNode.right(), fragmentType);

				if(fragments.second == true && backFragments.second == true)
				{
					// Both parts of the entity survived the clipping process, so the whole entity survived.
					Pair<LinkedList<T>, Boolean> ret = new Pair<LinkedList<T>, Boolean>();
					ret.first = new LinkedList<T>();
					ret.first.add(entity);
					ret.second = true;
					return ret;
				}

				if(fragments.second == true)
				{
					// The front part of the entity survived the clipping process.
					fragments.first = new LinkedList<T>();
					fragments.first.add(p.first);			// the front fragment list is just the front part of the entity
					fragments.second = false;				// we're going to be adding the back fragments into fragments and returning it,
															// so its survival flag needs to be corrected
				}
				else if(backFragments.second == true)
				{
					// The back part of the entity survived the clipping process.
					backFragments.first = new LinkedList<T>();
					backFragments.first.add(p.second);		// the back fragment list is just the back part of the entity
				}

				fragments.first.addAll(backFragments.first);	// note: not ideal, what we really want is a list splice here
				return fragments;
			}
			default:
			{
				// We'll never get here, but we need to convince the compiler that the switch
				// is exhaustive (it is, but because I'm using integers for enums, the compiler
				// can't possibly know that).
				throw new java.lang.Error();
			}
		}
	}

	/**
	Determines the list of planes which surround a particular leaf. Operationally, this
	involves walking up the tree to the root and recording the split planes we meet on
	the way.

	@param leafNode	The leaf node whose planes we wish to determine
	@return			The planes surrounding the leaf, as a list
	*/
	private static LinkedList<Plane> determine_leaf_planes(final ILeafNode leafNode)
	{
		LinkedList<Plane> ret = new LinkedList<Plane>();

		IBranchNode cur = leafNode.parent();
		INode oldCur = leafNode;

		while(cur != null)
		{
			// Note: The facing of the plane depends on which side of it the original
			// leaf node was on. If it was behind the plane, then we just add the plane
			// as is, otherwise we need to negate it. This turns out to be vitally
			// important to obtaining the correct results. (From which observant readers
			// might deduce that I got it wrong the first time!)
			if(oldCur == cur.right()) ret.add(cur.splitter());
			else ret.add(cur.splitter().negate());

			oldCur = cur;
			cur = cur.parent();
		}

		return ret;
	}

	/**
	Generates a list of convex volumes from the specified subtree, where here a convex volume means in practice
	the list of polygons surrounding it. Convex volumes are defined by the solid leaves of the subtree, only.

	<p><b>Preconditions:</b>
	<dl>
	<dd>node != null
	</dl>

	@param node				The root of the subtree from which to generate the convex volumes
	@return					A linked list of convex volumes, as specified, or null if the subtree was an empty leaf
	*/
	private static LinkedList<LinkedList<Polygon>> generate_convex_volumes_from_subtree(final INode node)
	{
		if(node.is_leaf())
		{
			ILeafNode leafNode = (ILeafNode)node;
			if(leafNode.is_solid())
			{
				LinkedList<Polygon> convexVolume = generate_convex_volume_polygons_from_surrounding_planes(determine_leaf_planes(leafNode));
				return MiscUtil.make_singleton_list(convexVolume);
			}
			else
			{
				return null;
			}
		}

		// If we get here, the node wasn't a leaf node, so it must be a branch node.
		IBranchNode branchNode = (IBranchNode)node;
		LinkedList<LinkedList<Polygon>> volumes = generate_convex_volumes_from_subtree(branchNode.left());
		LinkedList<LinkedList<Polygon>> backVolumes = generate_convex_volumes_from_subtree(branchNode.right());

		if(volumes == null) volumes = backVolumes;
		else volumes.addAll(backVolumes);

		return volumes;
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		private static StringBuffer format_clip_result(Pair<LinkedList<Polygon>,Boolean> result)
		{
			StringBuffer sb = new StringBuffer();
			sb.append("(");
			for(Polygon fragment: result.first) sb.append(" " + fragment);
			sb.append(", " + result.second + " )");
			return sb;
		}

		public void test_clip_to_tree()
		{
			// Take a simple cube with the right-hand face missing, and clip a universe polygon on the plane
			// of the missing face to the cube's tree in order to fill the gap.
			LinkedList<Polygon> polys = new LinkedList<Polygon>();
			polys.add(new Polygon(new Vector3d[] {	new Vector3d(20,0,0), new Vector3d(20,0,3),	// left
													new Vector3d(20,3,3), new Vector3d(20,3,0)	}));

			polys.add(new Polygon(new Vector3d[] {	new Vector3d(20,0,0), new Vector3d(23,0,0),	// front
													new Vector3d(23,0,3), new Vector3d(20,0,3)	}));

			polys.add(new Polygon(new Vector3d[] {	new Vector3d(23,3,0), new Vector3d(20,3,0),	// back
													new Vector3d(20,3,3), new Vector3d(23,3,3)	}));

			polys.add(new Polygon(new Vector3d[] {	new Vector3d(20,0,3), new Vector3d(23,0,3),	// top
													new Vector3d(23,3,3), new Vector3d(20,3,3)	}));

			polys.add(new Polygon(new Vector3d[] {	new Vector3d(20,3,0), new Vector3d(23,3,0),	// bottom
													new Vector3d(23,0,0), new Vector3d(20,0,0)	}));
			Tree tree = new Tree(polys);
			Plane plane = new Plane(new Vector3d(1,0,0), 23);
			Polygon universePoly = GeomUtil.make_universe_polygon(plane);
			Pair<LinkedList<Polygon>,Boolean> result = clip_to_tree(universePoly, PolygonClipFunctor.no_coplanars(), tree, CPTT_SOLID_FRAGMENTS);
			output(format_clip_result(result), "( [(23.0, 0.0, 3.0) (23.0, 0.0, 0.0) (23.0, 3.0, 0.0) (23.0, 3.0, 3.0)], false )");

			// Try clipping to a tree a polygon which lies in one of its planes. (In other words, test the
			// coplanar case of clip_to_tree.) The tree in question will be that constructed for a unit cube.
			// We'll test all three possible cases -
			// (i) a clip functor with its pass back flag set
			// (ii) a polygon facing the same way as the plane with a clip functor with its pass back flag not set
			// (iii) a polygon facing the opposite way to the plane with a clip functor with its pass back flag not set
// TODO: Rewrite these to reflect the new version of PolygonClipFunctor.
			/*tree = new Tree(GeomUtil.construct_cuboid(0, 1, 0, 1, 0, 1));
			Polygon poly = new Polygon(new Vector3d[] {	new Vector3d(0, 0.5, 0), new Vector3d(0, 0.5, 1), new Vector3d(0, 1.5, 1), new Vector3d(0, 1.5, 0)	});

			// (i)
			result = clip_to_tree(poly, new PolygonClipFunctor(true), tree, CPTT_EMPTY_FRAGMENTS);
			output(format_clip_result(result), "( [(0.0, 1.0, 1.0) (0.0, 1.5, 1.0) (0.0, 1.5, 0.0) (0.0, 1.0, 0.0)], false )");

			// (ii)
			result = clip_to_tree(poly, new PolygonClipFunctor(false), tree, CPTT_EMPTY_FRAGMENTS);
			output(format_clip_result(result), "( [(0.0, 0.5, 0.0) (0.0, 0.5, 1.0) (0.0, 1.5, 1.0) (0.0, 1.5, 0.0)], true )");

			// (iii)
			poly.flip_winding();
			result = clip_to_tree(poly, new PolygonClipFunctor(false), tree, CPTT_EMPTY_FRAGMENTS);
			output(format_clip_result(result), "( [(0.0, 1.5, 0.0) (0.0, 1.5, 1.0) (0.0, 1.0, 1.0) (0.0, 1.0, 0.0)], false )");*/

			// TESTME: We definitely need more tests for this, it's a vitally important generic method that gets reused for lots
			// of different things.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}