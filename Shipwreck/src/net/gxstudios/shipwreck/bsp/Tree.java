package net.gxstudios.shipwreck.bsp;

import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.LinkedList;

import net.gxstudios.shipwreck.geom.planar.GeomConstants;
import net.gxstudios.shipwreck.geom.planar.GeomUtil;
import net.gxstudios.shipwreck.geom.planar.Plane;
import net.gxstudios.shipwreck.geom.planar.Polygon;
import net.gxstudios.shipwreck.misc.Pair;

public class Tree implements GeomConstants
{
	//################## PRIVATE VARIABLES ##################//
	private BranchNode m_root = null;
	private int m_balanceWeight;
	private int m_splitWeight;

	//################## CONSTRUCTORS ##################//
	/**
	Allows static creation methods to create a blank Tree from scratch.
	*/
	private Tree() {}

	/**
	Constructs a BSP tree from the specified polygons.

	@param polys				The polygons from which to construct the BSP tree
	*/
	public Tree(final Iterable<Polygon> polys)
	{
		this(polys, 8, 1);	// use default weights for the balance and split factors
							// if the client doesn't specify them
	}

	/**
	Constructs a BSP tree from the specified polygons using a metric with the specified
	weights. To see how the metric works (and thus how the parameters should be used),
	see the {@link #choose_splitter_plane} method.

	@param polys				The polygons from which to construct the BSP tree
	@param balanceWeight		The weight given to the balance factor in the metric
	@param splitWeight			The weight given to the split factor in the metric
	@throws java.lang.Error		If polys == null or polys is empty
	*/
	public Tree(final Iterable<Polygon> polys, int balanceWeight, int splitWeight)
	{
		if(polys == null || !polys.iterator().hasNext()) throw new java.lang.Error();

		m_balanceWeight = balanceWeight;
		m_splitWeight = splitWeight;

		m_root = new BranchNode(null);
		build_subtree(m_root, polys);

		/*try
		{
			PrintWriter pw = new PrintWriter(new FileOutputStream("MapEditor\\debug_bsp.txt"));
			output_subtree(pw, m_root, "");
			pw.close();
		}
		catch(Exception e)
		{
			System.err.println("Exception caught while outputting BSP debugging information");
		}*/
	}

	//################## PUBLIC METHODS ##################//
	/**
	Constructs a right-linear tree from a set of planes. In practice, what I mean by this is
	a tree whose branch nodes all have a leaf node as their left child, such that every plane
	in the specified set is the split plane in exactly one branch node.

	<p><b>Preconditions:</b>
	<dl>
	<dd>planes != null && planes.iterator().hasNext()
	</dl>

	@param planes			The set of planes from which to construct the right-linear tree
	@return					A right-linear tree for the planes as described
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static Tree construct_right_linear_tree(final Iterable<Plane> planes)
	{
// TODO: Test this.
		// Check the preconditions.
		if(planes == null || !planes.iterator().hasNext()) throw new java.lang.Error();

		// Main algorithm
		Iterator<Plane> it = planes.iterator();

		Tree tree = new Tree();
		tree.m_root = new BranchNode(null);

		BranchNode cur = tree.m_root;
		while(true)
		{
			cur.m_splitter = it.next();
			cur.m_leftChild = new LeafNode(cur);
			if(it.hasNext())
			{
				cur.m_rightChild = new BranchNode(cur);
				cur = (BranchNode)cur.m_rightChild;
			}
			else
			{
				cur.m_rightChild = new LeafNode(cur);
				break;
			}
		}

		return tree;
	}

	/**
	Outputs the tree into the specified file. This is meant for debugging purposes.

	@param filename		The name of the file into which to output the tree
	*/
	public void output_tree(String filename)
	{
		try
		{
			PrintWriter pw = new PrintWriter(new FileOutputStream("MapEditor\\" + filename));
			output_subtree(pw, m_root, "");
			pw.close();
		}
		catch(Exception e)
		{
			System.err.println("Exception caught while outputting BSP debugging information");
		}
	}

	//################## PACKAGE METHODS ##################//
	IBranchNode get_root()
	{
		return m_root;
	}

	//################## PRIVATE METHODS ##################//
	/**
	Builds the specified subtree from the specified polygons.
	<p><b>Preconditions:</b>
	<dl>
	<dd>node != null
	<dd>polys != null and is non-empty
	</dl>

	@param node			The root of the subtree
	@param polys		The polygons from which to construct the BSP tree
	*/
	private void build_subtree(final BranchNode node, final Iterable<Polygon> polys)
	{
		node.m_splitter = choose_splitter_plane(polys);

		LinkedList<Polygon> frontPolys = new LinkedList<Polygon>();
		LinkedList<Polygon> backPolys = new LinkedList<Polygon>();

		for(Polygon poly: polys)
		{
			switch(GeomUtil.classify_polygon_against_plane(poly, node.m_splitter))
			{
				case CP_BACK:
				{
					backPolys.add(poly);
					break;
				}
				case CP_COPLANAR:
				{
					break;
				}
				case CP_FRONT:
				{
					frontPolys.add(poly);
					break;
				}
				case CP_STRADDLE:
				{
					Pair<Polygon,Polygon> splitPolys = GeomUtil.split_polygon(poly, node.m_splitter);
					frontPolys.add(splitPolys.first);
					backPolys.add(splitPolys.second);
					break;
				}
			}
		}

		if(frontPolys.isEmpty())
		{
			node.m_leftChild = new LeafNode(node);
		}
		else
		{
			BranchNode left = new BranchNode(node);
			build_subtree(left, frontPolys);
			node.m_leftChild = left;
		}

		if(backPolys.isEmpty())
		{
			node.m_rightChild = new LeafNode(node);
		}
		else
		{
			BranchNode right = new BranchNode(node);
			build_subtree(right, backPolys);
			node.m_rightChild = right;
		}
	}

	/**
	Returns a plane to use as the splitter for the specified polygons. We choose
	a plane from the set
	
	<p>P = { p | <font face="Symbol">$</font> poly in polys s.t. poly lies in p }</p>
	
	<p>Which plane to use is determined using a metric which takes into account how
	many polygons would be on each side of the plane (the balance) and how many
	polygons straddle the plane.</p>

	<p><b>Metric Definition:</b>
	<dl>
	<dd>metric = balanceWeight * abs(|polys in front| - |polys behind|) + splitWeight * (|polys straddling|)
	</dl>
	</p>

	@param polys	The polygons
	@return			The plane to be used to split the polygons
	*/
	private Plane choose_splitter_plane(final Iterable<Polygon> polys)
	{
		int bestMetric = Integer.MAX_VALUE;
		Plane bestSplitter = null;
		for(Polygon p: polys)
		{
			Plane splitter = GeomUtil.make_plane(p);
			int backCount = 0, frontCount = 0, straddleCount = 0;
			for(Polygon other: polys)
			{
				if(other == p) continue;

				switch(GeomUtil.classify_polygon_against_plane(other, splitter))
				{
					case CP_BACK:
					{
						++backCount;
						break;
					}
					case CP_COPLANAR:
					{
						break;
					}
					case CP_FRONT:
					{
						++frontCount;
						break;
					}
					case CP_STRADDLE:
					{
						++straddleCount;
						// Note: We don't update the front and back counts here because
						// there's no point: we're only interested in the difference
						// between them, and clearly (f+1) - (b+1) = f - b.
						break;
					}
				}
			}

			int metric = m_balanceWeight * Math.abs(frontCount - backCount) + m_splitWeight * straddleCount;
			if(metric < bestMetric)
			{
				bestMetric = metric;
				bestSplitter = splitter;
			}
		}

		return bestSplitter;
	}

	/**
	Outputs the specified subtree using the specified PrintWriter. This is meant for debugging purposes.

	@param pw		The PrintWriter with which to output
	@param node		The root of the current subtree we're outputting
	@param prefix	What to output on the current line before the node information
	*/
	private void output_subtree(PrintWriter pw, IBranchNode node, String prefix)
	{
		pw.println(prefix + (prefix.equals("") ? "" : " ") + node.splitter());
		if(node.left().is_leaf())
		{
			if(((ILeafNode)node.left()).is_solid()) pw.println(prefix + "--> solid");
			else pw.println(prefix + "--> empty");
		}
		else
		{
			output_subtree(pw, (IBranchNode)node.left(), prefix + "-->");
		}
		if(node.right().is_leaf())
		{
			if(((ILeafNode)node.right()).is_solid()) pw.println(prefix + "--> solid");
			else pw.println(prefix + "--> empty");
		}
		else
		{
			output_subtree(pw, (IBranchNode)node.right(), prefix + "-->");
		}
	}

	//################## PRIVATE NESTED CLASSES ##################//
	private static abstract class Node implements INode
	{
		private BranchNode m_parent = null;

		final public IBranchNode parent()
		{
			return m_parent;
		}

		public Node(BranchNode parent)
		{
			m_parent = parent;
		}
	}

	private static class BranchNode extends Node implements IBranchNode
	{
		private Node m_leftChild = null;
		private Node m_rightChild = null;
		private Plane m_splitter = null;

		public boolean is_leaf()
		{
			return false;
		}

		public INode left()
		{
			return m_leftChild;
		}

		public INode right()
		{
			return m_rightChild;
		}

		public Plane splitter()
		{
			return m_splitter;
		}

		public BranchNode(BranchNode parent)
		{
			super(parent);
		}
	}

	private static class LeafNode extends Node implements ILeafNode
	{
		public boolean is_empty()
		{
			return !is_solid();
		}

		public boolean is_leaf()
		{
			return true;
		}

		public boolean is_solid()
		{
			return this == parent().right();
		}

		public LeafNode(BranchNode parent)
		{
			super(parent);
		}
	}
}