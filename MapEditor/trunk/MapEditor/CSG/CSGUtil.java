package MapEditor.CSG;

import MapEditor.BSP.*;
import MapEditor.Brushes.*;
import MapEditor.Brushes.IBrush.IBrushContainer;
import MapEditor.Commands.*;
import MapEditor.Geom.Planar.*;
import MapEditor.Graphics.GraphicsUtil;
import MapEditor.Misc.*;
import MapEditor.Math.Vectors.Vector3d;
import MapEditor.Test.*;
import java.awt.Frame;
import java.util.Iterator;
import java.util.LinkedList;

public class CSGUtil implements Constants, GeomConstants
{
	//################## PRIVATE VARIABLES ##################//
	final private static ArchitectureBrush s_nullBrush = new ArchitectureBrushComposite();

	//################## PUBLIC METHODS ##################//
	/**
	Uses the specified carve brush to carve all the other brushes in the map. Obviously those brushes
	that share no volume with the carve brush remain entirely unaffected.

	@param carveBrush	The brush with which to carve
	@param container	The brush container containing all the other brushes (in practice, this is a Map)
	*/
	public static void carve(final ArchitectureBrush carveBrush, final IBrushContainer container)
	{
		final LinkedList<ArchitectureBrush> brushesToAdd = new LinkedList<ArchitectureBrush>();
		final LinkedList<ArchitectureBrush> brushesToDelete = new LinkedList<ArchitectureBrush>();

		Tree tree = build_brush_tree(carveBrush);
		if(tree == null) return;	// if we couldn't build a tree for the carve brush, the carve's a no-op

		for(IBrush b: container.get_brushes())
		{
			if(b == carveBrush) continue;
			if(!(b instanceof ArchitectureBrush)) continue;

			ArchitectureBrush ab = (ArchitectureBrush)b;

			ArchitectureBrush result = difference_tree_op(ab, tree);
			if(result != null)
			{
				brushesToDelete.add(ab);
				if(result != s_nullBrush) brushesToAdd.add(result);
			}
		}

		if(brushesToDelete.size() > 0)
		{
			CommandManager.instance().execute_command(new Command("Brush Carve")
			{
				public void execute()
				{
					for(ArchitectureBrush ab: brushesToDelete) container.delete_brush(ab);
					for(ArchitectureBrush ab: brushesToAdd) container.add_brush(ab);
				}

				public void undo()
				{
					for(ArchitectureBrush ab: brushesToAdd) container.delete_brush(ab);
					for(ArchitectureBrush ab: brushesToDelete) container.add_brush(ab);
				}
			});
		}
	}

	/**
	Hollows out the selected polygonal brush, such that it is surrounded by walls of the specified thickness, or displays
	a message box explaining why it can't be done.

	<p><b>Notes for successful hollowing:</b>
	<dl>
	<dd>Math.abs(thickness) >= EPSILON
	<dd>The brush must be large enough to allow hollowing with the specified thickness
	<dd>If thickness < 0, we simply hollow outwards instead of inwards
	</dl>

	@param brush			The brush to hollow
	@param thickness		The thickness of the resultant walls
	@param container		The brush container containing the brush (in practice, this is a Map)
	*/
	public static void hollow(final PolyhedralBrush brush, double thickness, final IBrushContainer container, final Frame owner)
	{
		/**
		Derivation of the algorithm:

		Hollowing out a brush is the same as carving it with a brush similar to it, but slightly smaller.
		We therefore construct the carve brush, by moving all the planes of the original brush in by the
		amount specified by thickness, and carve with it. Note that we can't use the carve method we've
		got already because we only want to carve the brush we're hollowing, not the whole map.
		*/

		if(Math.abs(thickness) < EPSILON)
		{
			GraphicsUtil.message_box(owner, "CSG Error", "The specified thickness is too close to zero, unable to hollow.");
			return;
		}

		LinkedList<Plane> planes = new LinkedList<Plane>();
		for(Polygon p: brush)
		{
			planes.add(GeomUtil.make_plane(p).translate_along_normal(-thickness));	// -thickness because the planes face outwards
		}

		// We need to check that it's alright to hollow. The method chosen merely involves observing that we can hollow
		// if the convex volume surrounded by the carve brush (strictly) contains the centre of the original brush.
		Vector3d centre = brush.centre();
		for(Plane plane: planes)
		{
			if(GeomUtil.classify_point_against_plane(centre, plane) != CP_BACK)
			{
				GraphicsUtil.message_box(owner, "CSG Error", "The specified thickness is too great, unable to hollow.",
															 "Perhaps try a smaller value for the thickness?");
				return;
			}
		}

		final ArchitectureBrush result;
		if(thickness > 0)
		{
			// When we're hollowing inwards, we just need to construct the tree of the smaller brush to carve against.
			result = difference_tree_op(brush, Tree.construct_right_linear_tree(planes));
		}
		else	// thickness < 0
		{
			// When we're hollowing outwards, we need to construct the larger brush itself, because we need to carve it
			// with this brush.
			LinkedList<Polygon> carvePolys = TreeUtil.generate_convex_volume_polygons_from_surrounding_planes(planes);
			PolyhedralBrush carveBrush = new PolyhedralBrush(carvePolys);
			result = difference_op(carveBrush, brush);
		}

		if(result != null)
		{
			CommandManager.instance().execute_command(new Command("Hollow Brush")
			{
				public void execute()
				{
					container.delete_brush(brush);
					container.add_brush(result);
					container.change_selection(result);
				}

				public void undo()
				{
					container.delete_brush(result);
					container.add_brush(brush);
					container.change_selection(brush);
				}
			});
		}
	}

	/**
	Performs a CSG intersection operation on the brushes in the specified composite. If the operation isn't possible for
	any reason, a message box is display explaining why.

	@param bc			The composite whose brushes we wish to intersect
	@param container	The brush container containing the composite (in practice, this is a Map)
	@param owner		The window in which the intersection is taking place (in practice, this is generally the main window)
	*/
	public static void intersect(final ArchitectureBrushComposite bc, final IBrushContainer container, final Frame owner)
	{
		general_symmetric_operation(bc, container, TreeUtil.CPTT_SOLID_FRAGMENTS, "Intersect Brushes", owner);
	}

	/**
	Performs a CSG union operation on the brushes in the specified composite. If the operation isn't possible for
	any reason, a message box is display explaining why.

	@param bc			The composite whose brushes we wish to union
	@param container	The brush container containing the composite (in practice, this is a Map)
	@param owner		The window in which the unioning is taking place (in practice, this is generally the main window)
	*/
	public static void union(final ArchitectureBrushComposite bc, final IBrushContainer container, final Frame owner)
	{
		general_symmetric_operation(bc, container, TreeUtil.CPTT_EMPTY_FRAGMENTS, "Union Brushes", owner);
	}

	//################## PRIVATE METHODS ##################//
	/**
	Builds a BSP tree for the specified brush.

	@param B	The brush for which to build the tree
	@return		A tree for the brush, or null if we couldn't build one because the brush in question had no faces
	*/
	private static Tree build_brush_tree(final ArchitectureBrush B)
	{
		LinkedList<Iterable<Polygon>> treePolys = B.tree_polygons();
		if(treePolys != null)	// we can build a tree for the brush
		{
			LinkedList<Polygon> fragments = union_op(treePolys);
			return new Tree(fragments);
		}
		else					// we can't build a tree for the brush
		{
			return null;
		}
	}

	/**
	Performs a CSG difference operation using the two specified brushes. In particular, calculates A \ B.

	@param A	The left-hand difference operand, i.e. the brush that gets carved by the other
	@param B	The right-hand difference operand, i.e. the brush that is used to carve the other
	@return		The results of the carve, as an ArchitectureBrush, or null, if the operation had no results either
				because B could not be used as a carve brush or because A was unaffected by the operation
	*/
	private static ArchitectureBrush difference_op(final ArchitectureBrush A, final ArchitectureBrush B)
	{
		Tree tree = build_brush_tree(B);
		if(tree != null) return difference_tree_op(A, tree);
		else return null;
	}

	/**
	TODO: Comment here.
	*/
	private static ArchitectureBrush difference_tree_op(final ArchitectureBrush A, final Tree tree)
	{
		Pair<LinkedList<ArchitectureBrush>,Boolean> p = TreeUtil.clip_to_tree(A, BrushClipFunctor.instance(), tree, TreeUtil.CPTT_EMPTY_FRAGMENTS);

		if(p.first.isEmpty())		// the carve completely eliminated A
		{
			return s_nullBrush;
		}
		else if(p.second == false)	// not all of A survived the carve
		{
			ArchitectureBrushComposite bc = new ArchitectureBrushComposite();
			for(ArchitectureBrush ab: p.first) bc.add_brush(ab);
			bc.group();
			return bc.optimal_brush();
		}
		else return null;			// the whole of A survived, so the carve had no results
	}

	/**
	Performs a symmetric CSG operation (for our purposes, this means union or intersection) on the brushes in
	the specified composite.

	@param bc			The composite on whose brushes we want to perform the operation
	@param container	The brush container containing the composite (in practice, this is a Map)
	@param fragmentType	The type of fragment to keep during the operation (see Notes in symmetric_op)
	@param description	A description for the command to be executed
	@param owner		The window in which the operation is taking place (in practice, this is generally the main window)
	*/
	private static void general_symmetric_operation(final ArchitectureBrushComposite bc, final IBrushContainer container, final int fragmentType,
													final String description, final Frame owner)
	{
		LinkedList<Iterable<Polygon>> treePolys = bc.tree_polygons();
		if(treePolys != null)
		{
			LinkedList<Polygon> polys = symmetric_op(treePolys, fragmentType);

			if(polys.isEmpty())
			{
				GraphicsUtil.message_box(owner, "CSG Error", "The attempted operation didn't produce a valid brush.");
			}
			else
			{
				LinkedList<LinkedList<Polygon>> convexVolumes = TreeUtil.generate_convex_volumes_from_tree(new Tree(polys));

				ArchitectureBrushComposite resultBrush = new ArchitectureBrushComposite();
				for(LinkedList<Polygon> polyList: convexVolumes)
				{
					resultBrush.add_brush(new PolyhedralBrush(polyList));
				}
				resultBrush.group();

				final ArchitectureBrush ob = resultBrush.optimal_brush();
				if(ob != null)
				{
					CommandManager.instance().execute_command(new Command(description)
					{
						public void execute()
						{
							container.delete_brush(bc);
							container.add_brush(ob);
							container.change_selection(ob);
						}

						public void undo()
						{
							container.delete_brush(ob);
							container.add_brush(bc);
							container.change_selection(bc);
						}
					});
				}
			}
		}
		else
		{
			GraphicsUtil.message_box(owner, "CSG Error", "The selected composite doesn't contain any polygons.");
		}
	}

	/**
	Performs a symmetric CSG operation (for our purposes, this means union or intersection) on the polygon
	iterables and returns the surviving polygon fragments.

	<p><b>Preconditions:</b>
	<dl>
	<dd>polyLists != null && polyLists.iterator().hasNext()
	<dd>fragmentType == TreeUtil.CPTT_EMPTY_FRAGMENTS || fragmentType == TreeUtil.CPTT_SOLID_FRAGMENTS
	</dl>

	<p><b>Notes:</b>
	<ul>
	<li>fragmentType should be TreeUtil.CPTT_EMPTY_FRAGMENTS for union, and TreeUtil.CPTT_SOLID_FRAGMENTS for intersection
	</ul>

	@param polyLists		The iterable of polygon iterables
	@param fragmentType		The type of fragment to keep during the operation (see Notes)
	@return					A list of the polygon fragments that survived the operation
	@throws java.lang.Error	If the preconditions are not met
	*/
	private static LinkedList<Polygon> symmetric_op(Iterable<Iterable<Polygon>> polyLists, int fragmentType)
	{
		// Check the preconditions.
		if(polyLists == null || !polyLists.iterator().hasNext()) throw new java.lang.Error();
		if(fragmentType != TreeUtil.CPTT_EMPTY_FRAGMENTS && fragmentType != TreeUtil.CPTT_SOLID_FRAGMENTS) throw new java.lang.Error();

		// Main algorithm
		Iterator<Iterable<Polygon>> it = polyLists.iterator();

		// Note that we pass opposite-facing coplanar polygons in different directions depending on whether it's a union or an intersection.
		final PolygonClipFunctor pcfPassSameBack = PolygonClipFunctor.complex(true, fragmentType == TreeUtil.CPTT_EMPTY_FRAGMENTS);
		final PolygonClipFunctor pcfPassSameFront = PolygonClipFunctor.complex(false, fragmentType == TreeUtil.CPTT_EMPTY_FRAGMENTS);

		Iterable<Polygon> currentFragments = it.next();

		// Note: We have to check whether there are any current fragments because it's perfectly possible
		// that a pass through the loop will leave us with an empty iterable of fragments.
		while(it.hasNext() && currentFragments.iterator().hasNext())
		{
			Tree currentTree = new Tree(currentFragments);
			Iterable<Polygon> otherFragments = it.next();
			Tree otherTree = new Tree(otherFragments);
			LinkedList<Polygon> results = new LinkedList<Polygon>();

			// Clip all the current fragments to the tree of the polygons with which we're unioning.
			// Note that our pass back flag is set here but not below - it could have been the other
			// way round, but one of the two must be set.
// FIXME: This comment is correct for union, but not for intersection. I must have forgotten to change it when I combined them.
			for(Polygon p: currentFragments)
			{
				Pair<LinkedList<Polygon>,Boolean> result = TreeUtil.clip_to_tree(p, pcfPassSameBack, otherTree, fragmentType);
				for(Polygon fragment: result.first) results.add(fragment);
			}

			// Clip all the polygons with which we're unioning to the tree of the current fragments.
// FIXME: Likewise.
			for(Polygon p: otherFragments)
			{
				Pair<LinkedList<Polygon>,Boolean> result = TreeUtil.clip_to_tree(p, pcfPassSameFront, currentTree, fragmentType);
				for(Polygon fragment: result.first) results.add(fragment);
			}

			currentFragments = results;
		}

		return MiscUtil.list_from_iterable(currentFragments);
	}

	/**
	Performs a CSG union operation on the polygon iterables and returns the surviving polygon fragments.

	<p><b>Preconditions:</b>
	<dl>
	<dd>polyLists != null && polyLists.iterator().hasNext()
	</dl>

	@param polyLists		The iterable of polygon iterables
	@return					A list of the polygon fragments that survived the operation
	@throws java.lang.Error	If the preconditions are not met
	*/
	private static LinkedList<Polygon> union_op(Iterable<Iterable<Polygon>> polyLists)
	{
		return symmetric_op(polyLists, TreeUtil.CPTT_EMPTY_FRAGMENTS);
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_union_op()
		{
			// Note: The output from these tests is too complicated to check except by hand. Thus this method doesn't
			// actually specify any automated tests.

			// Test a case with two cubes side-by-side.
			LinkedList<Iterable<Polygon>> polyLists = new LinkedList<Iterable<Polygon>>();
			polyLists.add(GeomUtil.construct_cuboid(0, 1, 0, 1, 0, 1));
			polyLists.add(GeomUtil.construct_cuboid(1, 2, 0, 1, 0, 1));
			Tree tree = new Tree(union_op(polyLists));
			tree.output_tree("tgu01.txt");

			// Test a case with two cubes in the same place.
			polyLists = new LinkedList<Iterable<Polygon>>();
			polyLists.add(GeomUtil.construct_cuboid(0, 1, 0, 1, 0, 1));
			polyLists.add(GeomUtil.construct_cuboid(0, 1, 0, 1, 0, 1));
			tree = new Tree(union_op(polyLists));
			tree.output_tree("tgu02.txt");

			// Test a case with two overlapping cubes.
			polyLists = new LinkedList<Iterable<Polygon>>();
			polyLists.add(GeomUtil.construct_cuboid(0, 1, 0, 1, 0, 1));
			polyLists.add(GeomUtil.construct_cuboid(0.5, 1.5, 0.5, 1.5, 0, 1));
			tree = new Tree(union_op(polyLists));
			tree.output_tree("tgu03.txt");

			// TESTME: We need many more test cases for this one, it's quite a complex method. In particular,
			// we need tests with more than two polygon iterables.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}