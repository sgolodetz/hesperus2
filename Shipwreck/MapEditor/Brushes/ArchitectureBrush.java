package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisPair;
import MapEditor.Geom.Planar.*;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Pair;
import java.awt.*;
import java.awt.event.*;
import java.util.LinkedList;

/**
This is the base class for the polygonal brushes which represent the level architecture.
*/
public abstract class ArchitectureBrush extends TransformableBrush
{
	//################## ENUMERATIONS ##################//
	protected enum BrushFunction
	{
		COLLISION,		// ¬Render, ¬Vis, Collisions
		DETAIL,			// Render, ¬Vis, ¬Collisions
		HINT,			// ¬Render, Vis (treated specially), ¬Collisions
		MULTIPLE,		// used for composite brushes (when the contained brushes have different functions)
		NORMAL,
		WATER,
	}

	//################## CONSTRUCTORS ##################//
	/**
	Constructs an ArchitectureBrush whose initial state depends on whether or not it's a
	new brush. By "new", I mean one the user is creating, as opposed to one which we're
	loading in, one which is being constructed as a result of a split operation, etc.

	@param isNew	Is the brush a new one that the user is creating?
	*/
	public ArchitectureBrush(boolean isNew)
	{
		super(isNew);
	}

	//################## PUBLIC ABSTRACT METHODS ##################//
	/**
	Classifies the brush relative to the specified plane.

	@param plane	The plane relative to which to classify the brush
	@return			The classification, one of GeomConstants.{CP_BACK, CP_COPLANAR, CP_FRONT, CP_STRADDLE}
	*/
	public abstract int classify_against_plane(final Plane plane);

	/**
	Returns a list of all the brushes in the brush tree rooted at this brush. In essence, this
	is a tree-flattening function.

	@return	The component brushes as a LinkedList
	*/
	public abstract LinkedList<ArchitectureBrush> component_brushes();

	/**
	Returns a list of the faces of all the brushes in the brush tree rooted at this brush.

	@return	The faces as a LinkedList
	*/
	public abstract LinkedList<MapEditor.Geom.Planar.Polygon> faces();

	/**
	Reflects the brush across a plane.

	@param plane	The plane across which we are reflecting
	*/
	public abstract void reflect(Plane plane);

	/**
	Splits the brush using the specified split plane.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The brush must straddle the plane
	<dd>The brush must not have a dimension equal to zero
	</dl>

	@param plane				The plane with which to split the brush
	@return						A pair of brushes, the first of which is the brush fragment in front of the
								splitting plane and the second of which is that behind it
	@throws java.lang.Error		If the preconditions aren't met
	*/
	public abstract Pair<ArchitectureBrush,ArchitectureBrush> split_with_plane(final Plane plane);

	/**
	Returns the polygons which will be used to generate the tree for this brush. In practice,
	what this means is that we return a list of polygon iterables, each of which corresponds
	to a single brush. This list of polygons will then have a CSG union operation performed on
	it before the final tree is generated from the surviving fragments.

	@return	A list of polygon iterables for tree generation, as specified, or null if no tree
			can be generated from this brush
	*/
	public abstract LinkedList<Iterable<MapEditor.Geom.Planar.Polygon>> tree_polygons();

	//################## PROTECTED ABSTRACT METHODS ##################//
	/**
	Returns the brush properties.

	@return	As stated
	*/
	protected abstract Properties get_properties();

	/**
	Sets the brush properties.

	@param properties	The new brush properties
	*/
	protected abstract void set_properties(Properties properties);

	//################## PUBLIC METHODS ##################//
	/**
	Returns the centre of the brush.

	@return	The centre of the brush as a Vector3d
	*/
	final public Vector3d centre()
	{
		return m_boundingBox.centre();
	}

	public ArchitectureBrush copy()
	{
		// This is needed to override the return type to ArchitectureBrush instead of IBrush.
		throw new UnsupportedOperationException();
	}

	public IBrush deselect(IBrushContainer container)
	{
		clear_state();
		return null;
	}

	final public void flip_x()
	{
		reflect(new Plane(new Vector3d(1,0,0), centre().x));
	}

	final public void flip_y()
	{
		reflect(new Plane(new Vector3d(0,1,0), centre().y));
	}

	final public void flip_z()
	{
		reflect(new Plane(new Vector3d(0,0,1), centre().z));
	}

	final public boolean is_flippable()
	{
		return true;
	}

	public Dialog properties_dialog(Frame owner)
	{
		return new PropertiesDialog(owner);
	}

	//################## NESTED CLASSES ##################//
	protected static class Properties
	{
		private BrushFunction m_function;

		public Properties()
		{
			m_function = BrushFunction.NORMAL;
		}

		public Properties(BrushFunction function)
		{
			m_function = function;
		}

		public Properties clone()
		{
			return new Properties(m_function);
		}

		public BrushFunction get_function()
		{
			return m_function;
		}

		public void set_function(BrushFunction function)
		{
			m_function = function;
		}
	}

	private class PropertiesDialog extends Dialog
	{
		//################## CONSTRUCTORS ##################//
		public PropertiesDialog(Frame owner)
		{
			super(owner, "Architecture Brush Properties", true);

			addWindowListener(new WindowAdapter()
			{
				public void windowClosing(WindowEvent e)
				{
					dispose();
				}
			});

			setLayout(new BorderLayout(0, 5));

			Panel top = new Panel();
			top.setLayout(new GridLayout(0, 2, 5, 0));
			add("Center", top);

			top.add(new Label("Brush Function:"));

			final LinkedList<Pair<ArchitectureBrush,Properties>> componentProperties = component_properties();
			BrushFunction oldFunction = determine_brush_function(componentProperties);

			final Choice choice = new Choice();
			for(BrushFunction function: BrushFunction.values())
			{
				choice.add(function.toString());
			}
			choice.select(oldFunction.toString());
			top.add(choice);

			Panel bottom = new Panel();
			bottom.setLayout(new GridLayout(0, 2, 5, 0));
			add("South", bottom);

			Button okButton = new Button("OK");
			okButton.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					if(choice.getSelectedItem() != "MULTIPLE")
					{
						CommandManager.instance().execute_command(new Command("Change Architecture Brush Properties")
						{
							public void execute()
							{
								BrushFunction curFunction = BrushFunction.valueOf(choice.getSelectedItem());
								for(Pair<ArchitectureBrush,Properties> p: componentProperties)
								{
									p.first.set_properties(new Properties(curFunction));
								}
							}

							public void undo()
							{
								for(Pair<ArchitectureBrush,Properties> p: componentProperties)
								{
									p.first.set_properties(p.second);
								}
							}
						});
					}

					dispose();
				}
			});
			bottom.add(okButton);
			Button cancelButton = new Button("Cancel");
			cancelButton.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					dispose();
				}
			});
			bottom.add(cancelButton);

			pack();
			setLocationRelativeTo(owner);		// centre the dialog relative to its owner
			setResizable(false);
			setVisible(true);
		}

		//################## PRIVATE METHODS ##################//
		private LinkedList<Pair<ArchitectureBrush,Properties>> component_properties()
		{
			LinkedList<Pair<ArchitectureBrush,Properties>> ret = new LinkedList<Pair<ArchitectureBrush,Properties>>();

			LinkedList<ArchitectureBrush> componentBrushes = component_brushes();
			for(ArchitectureBrush b: componentBrushes)
			{
				ret.add(Pair.make_pair(b, b.get_properties()));
			}

			return ret;
		}

		private BrushFunction determine_brush_function(LinkedList<Pair<ArchitectureBrush,Properties>> componentProperties)
		{
			BrushFunction function = BrushFunction.MULTIPLE;
			for(Pair<ArchitectureBrush,Properties> p: componentProperties)
			{
				BrushFunction f = p.second.get_function();
				if(function == BrushFunction.MULTIPLE)
				{
					function = f;
				}
				else if(f != function)
				{
					return BrushFunction.MULTIPLE;
				}
			}
			return function;
		}
	}
}