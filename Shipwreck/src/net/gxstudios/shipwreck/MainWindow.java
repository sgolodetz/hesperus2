package net.gxstudios.shipwreck;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.CheckboxMenuItem;
import java.awt.Color;
import java.awt.Dialog;
import java.awt.FileDialog;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.Panel;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import net.gxstudios.shipwreck.brushes.ArchitectureBrush;
import net.gxstudios.shipwreck.brushes.ArchitectureBrushComposite;
import net.gxstudios.shipwreck.brushes.BrushCommands;
import net.gxstudios.shipwreck.brushes.IBrush;
import net.gxstudios.shipwreck.brushes.LandscapeBrush;
import net.gxstudios.shipwreck.brushes.PolyhedralBrush;
import net.gxstudios.shipwreck.brushes.SplitterBrush;
import net.gxstudios.shipwreck.commands.Command;
import net.gxstudios.shipwreck.commands.CommandManager;
import net.gxstudios.shipwreck.commands.MenuManager;
import net.gxstudios.shipwreck.csg.CSGUtil;
import net.gxstudios.shipwreck.geom.AxisPair;
import net.gxstudios.shipwreck.graphics.GraphicsUtil;
import net.gxstudios.shipwreck.gui.AddScrollbars;
import net.gxstudios.shipwreck.gui.Camera;
import net.gxstudios.shipwreck.gui.DesignCanvas;
import net.gxstudios.shipwreck.gui.InputDialog;
import net.gxstudios.shipwreck.gui.MapInformationDialog;
import net.gxstudios.shipwreck.gui.MapTranslationDialog;
import net.gxstudios.shipwreck.gui.StatusBar;
import net.gxstudios.shipwreck.gui.TextureDialog;
import net.gxstudios.shipwreck.gui.View3D;
import net.gxstudios.shipwreck.maps.IMapSaver;
import net.gxstudios.shipwreck.maps.Map;
import net.gxstudios.shipwreck.maps.MapFileMAP;
import net.gxstudios.shipwreck.maps.MapFileMEF;
import net.gxstudios.shipwreck.misc.Options;
import net.gxstudios.shipwreck.textures.TextureManager;

public final class MainWindow extends Frame implements ActionListener
{
	private IBrush m_copiedBrush = null;
	private IMapSaver m_saver = MapFileMEF.instance();		// by default, we save maps as .mef files
	private Map m_map = new Map();
	private MenuManager m_menuManager = MenuManager.instance();
	private String m_editorVersion;
	private String m_mapFilename = "<Untitled Map>";		// the filename of the map we're currently editing
															// set to <Untitled Map> initially for new maps
	private View3D m_view;									// the 3D view

	private void change_map_name(final String mapFilename)
	{
		m_mapFilename = mapFilename;
		set_window_title();
	}

	private void set_window_title()
	{
		setTitle("GX Studios - Shipwreck Map Editor : " + m_mapFilename);
	}

	public MainWindow()
	{
		set_window_title();
		setSize(Toolkit.getDefaultToolkit().getScreenSize());

		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				System.exit(0);
			}
		});

		Panel toolbar = new Panel();
		toolbar.setBackground(Color.lightGray);
		toolbar.setLayout(new GridLayout(30,0,0,5));
		toolbar.add(new Label("Brush Type"));

		Button selectButton = new Button("Select");
		selectButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				final IBrush oldSelection = m_map.get_selected_brush();
				CommandManager.instance().execute_command(new Command("Enter Select Mode")
				{
					public void execute()
					{
						m_map.deselect_selection();
					}

					public void undo()
					{
						m_map.select_brush(oldSelection);
					}
				});
				if(m_map.get_state() == Map.STATE_CREATE) m_map.set_state(Map.STATE_NORMAL);
			}
		});
		toolbar.add(selectButton);

		Button blockButton = new Button("Block");
		blockButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				m_map.set_state(Map.STATE_CREATE);
				m_map.set_brush_creation_type(Map.BrushType.BLOCK);
			}
		});
		toolbar.add(blockButton);

		Button cylinderButton = new Button("Cylinder");
		cylinderButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				m_map.set_state(Map.STATE_CREATE);
				m_map.set_brush_creation_type(Map.BrushType.CYLINDER);
			}
		});
		toolbar.add(cylinderButton);

		Button coneButton = new Button("Cone");
		coneButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				m_map.set_state(Map.STATE_CREATE);
				m_map.set_brush_creation_type(Map.BrushType.CONE);
			}
		});
		toolbar.add(coneButton);

		Button uvSphereButton = new Button("UV Sphere");
		uvSphereButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				m_map.set_state(Map.STATE_CREATE);
				m_map.set_brush_creation_type(Map.BrushType.UV_SPHERE);
			}
		});
		toolbar.add(uvSphereButton);

		Button landscapeButton = new Button("Landscape");
		landscapeButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				m_map.set_state(Map.STATE_CREATE);
				m_map.set_brush_creation_type(Map.BrushType.LANDSCAPE);
			}
		});
		toolbar.add(landscapeButton);

		Button lightButton = new Button("Light");
		lightButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				m_map.set_state(Map.STATE_CREATE);
				m_map.set_brush_creation_type(Map.BrushType.LIGHT);
			}
		});
		toolbar.add(lightButton);

		Panel mainArea = new Panel();
		mainArea.setLayout(new GridLayout(2,2));

		DesignCanvas[] dcArr = new DesignCanvas[3];
		dcArr[0] = new DesignCanvas(m_map, new AxisPair(AxisPair.Axis.X,AxisPair.Axis.Y), 1, -1);
		dcArr[1] = new DesignCanvas(m_map, new AxisPair(AxisPair.Axis.Y,AxisPair.Axis.Z), 1, -1);
		dcArr[2] = new DesignCanvas(m_map, new AxisPair(AxisPair.Axis.X,AxisPair.Axis.Z), 1, -1);

		m_view = new View3D(m_map);
		mainArea.add(m_view.get_canvas());
		m_map.add_repaint_listener(m_view);

		for(DesignCanvas dc: dcArr)
		{
			m_map.add_repaint_listener(dc);
			mainArea.add(new AddScrollbars(dc));
		}

		StatusBar statusBar = new StatusBar();
		for(DesignCanvas dc: dcArr) statusBar.add_status_source(dc);

		setLayout(new BorderLayout());
		add("West", toolbar);
		add("Center", mainArea);
		add("South", statusBar);

		try
		{
			load_menus();
			read_version_number();
		}
		catch(IOException e)
		{
			System.exit(0);
		}

		CommandManager.instance().set_repaint_listener(m_map);

		TextureManager.instance().load_texture("HINT", "textures/hint.jpg");
		TextureManager.instance().load_texture("LANDSCAPE", "textures/landscape.jpg");
		TextureManager.instance().load_texture("NULL", "textures/null.jpg");
	}

	public void actionPerformed(ActionEvent e)
	{
// FIXME: A big, long, nasty if block is clearly NOT the right way to go about this.
		String ac = e.getActionCommand();
		if(ac.equals("File_New"))
		{
			m_map.replace_with(new Map());
			change_map_name("<Untitled Map>");
			CommandManager.instance().reset();
			m_view.reset_camera();
		}
		else if(ac.equals("File_Load..."))
		{
			try
			{
				FileDialog dialog = new FileDialog(this, "Load", FileDialog.LOAD);
				dialog.setVisible(true);

				if(dialog.getFile() != null)
				{
					String filename = dialog.getDirectory() + dialog.getFile();

					// TODO: Allow users to choose which loader they want to use (useful if files have the wrong extension).
					String extension = filename.substring(filename.length() - 3);
					if(extension.equals("mef")) m_map.replace_with(MapFileMEF.instance().load(filename));
					else if(extension.equals("map")) m_map.replace_with(MapFileMAP.instance().load(filename));
					else throw new IOException("Unknown file extension (." + extension + ")");

					change_map_name(filename);	// if the load was a success, store the filename
					CommandManager.instance().reset();
					m_view.reset_camera();
				}
			}
			catch(IOException ex)
			{
				GraphicsUtil.message_box(this, "Error", "Could not load file: " + ex.getMessage());
			}
		}
		else if(ac.equals("File_Save") && !m_mapFilename.equals("<Untitled Map>"))
		{
// FIXME: Somehow make it obvious to the user that their file is being saved.
			m_saver.save(m_map, m_mapFilename);
		}
		else if(ac.equals("File_Save As...") || ac.equals("File_Save"))
		{
			FileDialog dialog = new FileDialog(this, "Save As...", FileDialog.SAVE);
			dialog.setVisible(true);

			if(dialog.getFile() != null)
			{
				// Prevent project-threatening accidents.
				if(dialog.getFile().endsWith(".java"))
				{
					GraphicsUtil.message_box(this, "Error",	"Your less-than-cunning attempt to overwrite project source files",
															"has been thwarted by programmer foresight. Have a nice day!");
				}
				else
				{
					change_map_name(dialog.getDirectory() + dialog.getFile());
					m_saver.save(m_map, m_mapFilename);
				}
			}
		}
		else if(ac.equals("File_Export MEF 2..."))
		{
			FileDialog dialog = new FileDialog(this, "Export MEF 2...", FileDialog.SAVE);
			dialog.setVisible(true);

			if(dialog.getFile() != null)
			{
				// Prevent project-threatening accidents.
				if(dialog.getFile().endsWith(".java"))
				{
					GraphicsUtil.message_box(this, "Error",	"Your less-than-cunning attempt to overwrite project source files",
															"has been thwarted by programmer foresight. Have a nice day!");
				}
				else
				{
					change_map_name(dialog.getDirectory() + dialog.getFile());
					MapFileMEF.instance().save_MEF2(m_map, m_mapFilename);
				}
			}
		}
		else if(ac.equals("File_Exit"))
		{
			System.exit(0);
		}
		else if(ac.equals("Edit_Undo"))
		{
			if(CommandManager.instance().can_undo()) CommandManager.instance().undo();
		}
		else if(ac.equals("Edit_Redo"))
		{
			if(CommandManager.instance().can_redo()) CommandManager.instance().redo();
		}
		else if(ac.equals("Edit_Delete"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null)
			{
				CommandManager.instance().execute_command(BrushCommands.delete_brush(b, m_map));
// TODO: Set the cursors on the various canvases to the default cursor.
			}
		}
		else if(ac.equals("Edit_Cut"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null && b.is_copyable())
			{
				m_copiedBrush = b.copy();	// this is genuinely necessary, consider undoing the cut and modifying the brush before pasting!
				CommandManager.instance().execute_command(BrushCommands.delete_brush(b, m_map));
				MenuManager.instance().get_item("Edit_Paste").setEnabled(true);
			}
		}
		else if(ac.equals("Edit_Copy"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null && b.is_copyable())
			{
				m_copiedBrush = b.copy();
				MenuManager.instance().get_item("Edit_Paste").setEnabled(true);
			}
		}
		else if(ac.equals("Edit_Paste"))
		{
			CommandManager.instance().execute_command(BrushCommands.paste(m_copiedBrush, m_map));
		}
		else if(ac.equals("View_Set Angular Camera Speed..."))
		{
			InputDialog input = new InputDialog(this, "Set Angular Camera Speed", String.valueOf(Camera.get_angular_speed()),
												"Please enter the desired angular camera speed in degrees/second",
												"(must be strictly greater than zero):");
			try
			{
				double d = Double.parseDouble(input.result());
				if(d > 0) Camera.set_angular_speed(d);
				else GraphicsUtil.message_box(this, "Error", "The angular camera speed must be strictly greater than zero.");
			}
			catch(NumberFormatException ex)
			{
				GraphicsUtil.message_box(this, "Error", "The angular camera speed must be a real number.");
			}
		}
		else if(ac.equals("View_Reset Camera Position"))
		{
			m_view.reset_camera();
		}
		else if(ac.equals("View_Set Linear Camera Speed..."))
		{
			InputDialog input = new InputDialog(this, "Set Linear Camera Speed", String.valueOf(Camera.get_linear_speed()),
												"Please enter the desired linear camera speed in units/second",
												"(must be strictly greater than zero):");
			try
			{
				double d = Double.parseDouble(input.result());
				if(d > 0) Camera.set_linear_speed(d);
				else GraphicsUtil.message_box(this, "Error", "The linear camera speed must be strictly greater than zero.");
			}
			catch(NumberFormatException ex)
			{
				GraphicsUtil.message_box(this, "Error", "The linear camera speed must be a real number.");
			}
		}
		else if(ac.equals("Map_Information..."))
		{
			MapInformationDialog dialog = new MapInformationDialog(this, m_map);
		}
		else if(ac.equals("Map_Transform_Translate..."))
		{
			MapTranslationDialog dialog = new MapTranslationDialog(this, m_map);
		}
		else if(ac.equals("Brush_Flip X"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null && b.is_flippable()) CommandManager.instance().execute_command(BrushCommands.flip_x(b));
		}
		else if(ac.equals("Brush_Flip Y"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null && b.is_flippable()) CommandManager.instance().execute_command(BrushCommands.flip_y(b));
		}
		else if(ac.equals("Brush_Flip Z"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null && b.is_flippable()) CommandManager.instance().execute_command(BrushCommands.flip_z(b));
		}
		else if(ac.equals("Brush_Flatten Composite"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b instanceof ArchitectureBrushComposite)
			{
				final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)b;
				CommandManager.instance().execute_command(BrushCommands.flatten(bc, m_map));
			}
		}
		else if(ac.equals("Brush_Group"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b instanceof ArchitectureBrushComposite)
			{
				final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)b;
				if(bc.is_transient()) CommandManager.instance().execute_command(BrushCommands.group(bc));
			}
		}
		else if(ac.equals("Brush_Ungroup"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b instanceof ArchitectureBrushComposite)
			{
				final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)b;
				if(!bc.is_transient()) CommandManager.instance().execute_command(BrushCommands.ungroup(bc));
			}
		}
		else if(ac.equals("Brush_CSG_Hollow Brush..."))
		{
			IBrush b = m_map.get_selected_brush();
			if((!Options.is_set("Texture-Editing Mode")) && (b instanceof PolyhedralBrush))
			{
				InputDialog input = new InputDialog(this, "Hollow Brush", "16", "Please enter the desired wall thickness",
																				"(negative numbers hollow outwards):");
				try
				{
					double thickness = Double.parseDouble(input.result());
					CSGUtil.hollow((PolyhedralBrush)b, thickness, m_map, this);
				}
				catch(NumberFormatException ex)
				{
					GraphicsUtil.message_box(this, "CSG Error", "The wall thickness must be a real number.");
				}
			}
		}
		else if(ac.equals("Brush_CSG_Split Brush"))
		{
			IBrush b = m_map.get_selected_brush();
			if((!Options.is_set("Texture-Editing Mode")) && (b instanceof ArchitectureBrush))
			{
				ArchitectureBrush spb = (ArchitectureBrush)b;
				spb.deghost(m_map);	// if a brush is going to be split, we assume the intention is that it's in the map
				final SplitterBrush sb = new SplitterBrush(spb);
				CommandManager.instance().execute_command(new Command("Enter Split Mode")
				{
					public void execute()
					{
						m_map.select_brush(sb);
					}

					public void undo()
					{
						m_map.deselect_selection();
					}
				});
			}
		}
		else if(ac.equals("Brush_CSG_Carve With Brush"))
		{
			IBrush b = m_map.get_selected_brush();
// FIXME: A better solution would be to provide an IBrush method indicating whether the brush in question can be used as a carve brush.
			if((!Options.is_set("Texture-Editing Mode")) && (b instanceof ArchitectureBrush))
			{
				CSGUtil.carve((ArchitectureBrush)b, m_map);
			}
		}
		else if(ac.equals("Brush_CSG_Intersect Brushes"))
		{
			IBrush b = m_map.get_selected_brush();
			if((!Options.is_set("Texture-Editing Mode")) && (b instanceof ArchitectureBrushComposite))
			{
				CSGUtil.intersect((ArchitectureBrushComposite)b, m_map, this);
			}
		}
		else if(ac.equals("Brush_CSG_Union Brushes"))
		{
			IBrush b = m_map.get_selected_brush();
			if((!Options.is_set("Texture-Editing Mode")) && (b instanceof ArchitectureBrushComposite))
			{
				CSGUtil.union((ArchitectureBrushComposite)b, m_map, this);
			}
		}
		else if(ac.equals("Brush_Landscape_Decrease Resolution"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b instanceof LandscapeBrush)
			{
				LandscapeBrush lb = (LandscapeBrush)b;
				lb.decrease_resolution();
			}
		}
		else if(ac.equals("Brush_Landscape_Increase Resolution"))
		{
			IBrush b = m_map.get_selected_brush();
			if(b instanceof LandscapeBrush)
			{
				LandscapeBrush lb = (LandscapeBrush)b;
				lb.increase_resolution();
			}
		}
		else if(ac.equals("Brush_Properties..."))
		{
			IBrush b = m_map.get_selected_brush();
			if(b != null)
			{
				Dialog dialog = b.properties_dialog(this);
			}
		}
		else if(ac.equals("Texture_Load Texture..."))
		{
// TODO: Rewrite this to load texture package files eventually.
			FileDialog dialog = new FileDialog(this, "Load Texture", FileDialog.LOAD);
			dialog.setVisible(true);

			if(dialog.getFile() != null)
			{
				String filename = dialog.getDirectory() + dialog.getFile();
				String texture = dialog.getFile().substring(0, dialog.getFile().length() - 4).toUpperCase();
				if(TextureManager.instance().get_texture(texture) == null) TextureManager.instance().load_texture(texture, filename);
			}
		}
		else if(ac.equals("Texture_Edit Textures..."))
		{
			TextureDialog td = new TextureDialog(this, m_map);
		}
		else if(ac.equals("Help_About..."))
		{
			GraphicsUtil.message_box(this, "About Shipwreck",	"Shipwreck was designed and written by Stuart Golodetz.",
																"As many rights as possible reserved!",
																"",
																"The original version of Shipwreck (known only as MapEditor)",
																"was my third year Computer Science project at Oxford University,",
																"kindly supervised by Dr. Stephen Cameron.",
																"",
																"Version " + m_editorVersion);
		}
		else System.out.println("Warning: Unbound menu command " + ac);
		// TODO: All the other menu commands.

		// Repaint the 3D view.
		m_view.repaint_full();
	}

	/**
	Loads a menu (which may or may not be a top-level menu) from a file.

	@param menuName				The menu label
	@param menuPrefix			The prefix for menu item action commands, e.g. Brush->CSG->Split Brush would have the prefix "Brush_CSG"
	@param br					The BufferedReader associated with the file from which we're loading the menu
	@return						The loaded menu, as a Menu object
	@throws java.io.IOException	If anything goes wrong (e.g. parse errors)
	*/
	private Menu load_menu(String menuName, String menuPrefix, BufferedReader br) throws IOException
	{
		Menu menu = new Menu(menuName);
		String line = br.readLine();		// read the {
		while(true)
		{
			line = br.readLine().trim();
			if(line == null) throw new IOException("Parse Error in resources/Menu.txt: Expected }");
			if(line.equals("")) continue;
			if(line.equals("}")) break;

			if(line.startsWith("Menu "))
			{
				// Load a submenu
				String submenuName = line.substring(5);
				Menu submenu = load_menu(submenuName, menuPrefix + "_" + submenuName, br);
				menu.add(submenu);
			}
			else
			{
				MenuItem mi = load_menu_item(menuPrefix, line);
				menu.add(mi);
				m_menuManager.add_item(mi.getActionCommand(), mi);
			}
		}
		return menu;
	}

	/**
	Loads a menu item, given a line from the menu file to parse.

	@param menuPrefix			The prefix for the menu item's action command
	@param line					The line from the menu file defining the menu item
	@return						The loaded menu item, as a MenuItem object
	@throws java.io.IOException	If the item definition in the menu file doesn't match the required item pattern
	*/
	private MenuItem load_menu_item(String menuPrefix, String line) throws IOException
	{
		// Roughly speaking, the pattern matches "blah" or "blah<one or more spaces>[k]" or "blah<one or more spaces>[k+]".
		// The [k] is the shortcut key for the menu item; [k+] means you have to press shift as well to activate the item.
		Pattern itemPattern = Pattern.compile("([^\\[]*[^ ])(?: +\\[(.\\+?)\\])?");
		Matcher matcher = itemPattern.matcher(line);
		if(matcher.matches())
		{
			String label = matcher.group(1);
			boolean enabled = true;

			// Arrange for menu commands which are enclosed in brackets to be initially disabled.
			if(label.startsWith("("))
			{
				label = label.substring(1, label.length()-1);
				enabled = false;
			}

			MenuItem mi = new MenuItem(label);
			mi.setActionCommand(menuPrefix + "_" + label);
			mi.setEnabled(enabled);
			mi.addActionListener(this);

			if(matcher.group(2) != null)
			{
				char c = matcher.group(2).charAt(0);
// FIXME: We should be passing in a virtual keycode here, but we don't have a mapping from Unicode to virtual keys!
// Luckily (or perhaps not, since it encourages code like this?), the key codes correspond to their ASCII values as
// things stand at present. With any luck, this won't ever change. If it does, what will happen is that we'll end
// up with different shortcut keys on the menus. This could conceivably cause a major problem if (say) the user presses
// the shortcut key for save and ends up quitting instead, or whatever. I'm trying to design the rest of the editor
// to minimise problems like this in any case, however.
				mi.setShortcut(new MenuShortcut(c, matcher.group(2).length() == 2));
			}

			return mi;
		}
		else throw new IOException("Parse Error in resources/Menus.txt: Invalid item definition");
	}

	/**
	Loads the menus from a file (namely resources/Menus.txt).

	<p><b>Preconditions:</b>
	<dl>
	<dd>The menu file must be in the right format, we're not doing much error-checking
	</dl>

	@throws	java.io.IOException		If anything goes wrong (e.g. file not found, parse errors, etc.)
	*/
	private void load_menus() throws IOException
	{
		MenuBar mb = new MenuBar();
		setMenuBar(mb);

		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream("resources/Menus.txt")));
			String line;
			while((line = br.readLine()) != null)
			{
				line = line.trim();

				if(line.equals("")) continue;

				Menu menu;
				if(line.equals("[Options]")) menu = load_options_menu();	// the options menu merits special-case treatment
				else if(line.startsWith("Menu "))
				{
					String menuName = line.substring(5);
					menu = load_menu(menuName, menuName, br);
				}
				else throw new IOException("Parse Error in resources/Menus.txt: Items added to the menu bar must be menus (i.e. prefixed with 'Menu')");

				mb.add(menu);
			}

			br.close();
		}
		catch(IOException e)
		{
			System.err.println(e.getMessage());
			throw e;
		}
	}

	/**
	Loads the Options menu by building it from a hard-coded set of options.

	@return	The Options menu, as a Menu object
	*/
	private Menu load_options_menu()
	{
		Menu optionsMenu = new Menu("Options");
		Set<String> options = Options.get();
		for(final String s: options)
		{
			CheckboxMenuItem mi = new CheckboxMenuItem(s, Options.is_set(s));
			mi.addItemListener(new ItemListener()
			{
				public void itemStateChanged(ItemEvent e)
				{
					switch(e.getStateChange())
					{
						case ItemEvent.DESELECTED:
							Options.set(s, false);
							break;
						case ItemEvent.SELECTED:
							Options.set(s, true);
							break;
					}
					m_map.repaint_full();
				}
			});
			optionsMenu.add(mi);
		}
		return optionsMenu;
	}

	/**
	Reads the latest version number of the editor from a file, to be displayed in the About box.
	*/
	private void read_version_number() throws IOException
	{
		BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream("resources/Version.txt")));
		m_editorVersion = br.readLine();
	}

	public static void main(String args[]) throws IOException
	{
		MainWindow window = new MainWindow();
		window.setVisible(true);
	}
}
