package MapEditor.GUI;

import MapEditor.Brushes.*;
import MapEditor.Commands.*;
import MapEditor.Event.ISelectionListener;
import MapEditor.Geom.Planar.Polygon;
import MapEditor.GUI.ImageCanvas;
import MapEditor.Maps.Map;
import MapEditor.Misc.Options;
import MapEditor.Misc.Pair;
import MapEditor.Textures.*;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Choice;
import java.awt.Dialog;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextField;
import java.awt.event.*;
import java.util.LinkedList;

public class TextureDialog extends Dialog implements ISelectionListener, TextureConstants
{
	//################## PRIVATE VARIABLES ##################//
	private boolean m_forceRenderTexturesOption;	// true iff we turned on the Render Textures option for texture editing
	private Choice m_nameChoice = new Choice();
	private ImageCanvas m_thumbnail = new ImageCanvas();
	private Map m_map;
	private TextField m_offsetUField = new TextField();
	private TextField m_offsetVField = new TextField();
	private TextField m_scaleUField = new TextField();
	private TextField m_scaleVField = new TextField();
	private TextField m_rotationField = new TextField();

	//################## CONSTRUCTORS ##################//
	public TextureDialog(Frame owner, Map map)
	{
		super(owner, "Edit Texture Details", false);

		m_map = map;

		Options.set_internal("Texture-Editing Mode", true);

		// We can't edit textures if we can't see them, so temporarily turn on the Render Textures option if necessary.
		m_forceRenderTexturesOption = !Options.is_set("Render Textures");
		if(m_forceRenderTexturesOption)
		{
			Options.set("Render Textures", true);
		}

		// Check whether there's a brush currently selected and, if so, select its faces, if any. If there weren't any faces to select,
		// set the selected face to null.
// FIXME: If we can find a way to get rid of the instanceof test at some point, we should.
		IBrush b = m_map.get_selected_brush();
		if(b instanceof ArchitectureBrush)
		{
			ArchitectureBrush ab = (ArchitectureBrush)b;
			m_map.select_faces(ab.faces());
		}
		else
		{
			m_map.select_face(null);
		}

		m_map.set_selection_listener(this);
		m_map.repaint_full();

		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				dispose();
			}
		});

		setLayout(new BorderLayout(0, 5));

		Panel thumbnailArea = new Panel();
		m_thumbnail.setSize(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
		thumbnailArea.add(m_thumbnail);
		add("North", thumbnailArea);

		Panel top = new Panel();
		top.setLayout(new GridLayout(0, 3, 5, 0));

		top.add(new Label("Name:"));		top.add(m_nameChoice);			top.add(new Panel());
		top.add(new Label("Offset U:"));	top.add(m_offsetUField);		top.add(create_arrows_panel(m_offsetUField, 1.0));
		top.add(new Label("Offset V:"));	top.add(m_offsetVField);		top.add(create_arrows_panel(m_offsetVField, 1.0));
		top.add(new Label("Scale U:"));		top.add(m_scaleUField);			top.add(create_arrows_panel(m_scaleUField, 1.0));
		top.add(new Label("Scale V:"));		top.add(m_scaleVField);			top.add(create_arrows_panel(m_scaleVField, 1.0));
		top.add(new Label("Rotation:"));	top.add(m_rotationField);		top.add(create_arrows_panel(m_rotationField, 1.0));

		for(String s: TextureManager.instance().get_names()) m_nameChoice.add(s);
		set_selected_texture("NULL");
		m_nameChoice.addItemListener(new ItemListener()
		{
			public void itemStateChanged(ItemEvent e)
			{
				update_thumbnail();
			}
		});

		face_selection_changed();

		add("Center", top);

		Panel bottom = new Panel();
		bottom.setLayout(new GridLayout(0, 2, 5, 5));

		Button browseButton = new Button("Browse Textures...");
		browseButton.setEnabled(false);
		bottom.add(browseButton);

		final Button toggleMaskButton = new Button(Options.is_set("Render Texture Mask") ? "Hide Mask" : "Show Mask");
		toggleMaskButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				Options.set_internal("Render Texture Mask", !Options.is_set("Render Texture Mask"));
				toggleMaskButton.setLabel(Options.is_set("Render Texture Mask") ? "Hide Mask" : "Show Mask");
				m_map.repaint_full();
			}
		});
		bottom.add(toggleMaskButton);

		Button applyButton = new Button("Apply Texture Details");
		applyButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				apply_details();
			}
		});
		bottom.add(applyButton);
		Button exitButton = new Button("Exit Texture Mode");
		exitButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				dispose();
			}
		});
		bottom.add(exitButton);
		add("South", bottom);

		pack();
		setLocationRelativeTo(owner);		// centre the dialog relative to its owner
		setResizable(false);
		setVisible(true);
	}

	//################## PUBLIC METHODS ##################//
	public void dispose()
	{
		super.dispose();
		Options.set_internal("Texture-Editing Mode", false);

		// If we temporarily turned on the Render Textures option for texture-editing, turn it off again.
		if(m_forceRenderTexturesOption)
		{
			Options.set("Render Textures", false);
		}

		m_map.set_selection_listener(ISelectionListener.NULL);
		m_map.repaint_full();
	}

	public void face_selection_changed()
	{
		LinkedList<Polygon> selectedFaces = m_map.get_selected_faces();
		if(selectedFaces.size() == 1)
		{
			Pair<String,TexturePlane> textureDetails = selectedFaces.getFirst().get_texture_details();

			set_selected_texture(textureDetails.first);

			m_offsetUField.setText(String.valueOf((float)textureDetails.second.offset().u));
			m_offsetVField.setText(String.valueOf((float)textureDetails.second.offset().v));
			m_scaleUField.setText(String.valueOf((float)textureDetails.second.scale_u()));
			m_scaleVField.setText(String.valueOf((float)textureDetails.second.scale_v()));
			m_rotationField.setText(String.valueOf((float)textureDetails.second.rotation()));

			set_caret(m_offsetUField);
			set_caret(m_offsetVField);
			set_caret(m_scaleUField);
			set_caret(m_scaleVField);
			set_caret(m_rotationField);
		}
	}

	//################## PRIVATE METHODS ##################//
	private void apply_details()
	{
		try
		{
			final String texture = m_nameChoice.getSelectedItem();
			final double scaleU = Double.parseDouble(m_scaleUField.getText());
			final double scaleV = Double.parseDouble(m_scaleVField.getText());
			final double offsetU = Double.parseDouble(m_offsetUField.getText());
			final double offsetV = Double.parseDouble(m_offsetVField.getText());
			final double angle = Double.parseDouble(m_rotationField.getText());

			if(TextureManager.instance().get_texture(texture) != null)
			{
				final LinkedList<Pair<Polygon,Pair<String,TexturePlane>>> oldTextureDetails = new LinkedList<Pair<Polygon,Pair<String,TexturePlane>>>();
				for(Polygon p: m_map.get_selected_faces()) oldTextureDetails.add(Pair.make_pair(p, p.get_texture_details()));

				CommandManager.instance().execute_command(new Command("Texture Application")
				{
					public void execute()
					{
						for(Pair<Polygon,Pair<String,TexturePlane>> p: oldTextureDetails)
						{
							p.first.set_texture(texture);
							p.first.set_texture_plane(new TexturePlane(p.first.get_normal(), scaleU, scaleV, offsetU, offsetV, angle));
						}
					}

					public void undo()
					{
						for(Pair<Polygon,Pair<String,TexturePlane>> p: oldTextureDetails)
						{
							p.first.set_texture(p.second.first);
							p.first.set_texture_plane(p.second.second);
						}
					}
				});
			}
			else
			{
				System.err.println("Warning: Missing texture " + texture);
			}
		}
		catch(Exception e)
		{
			System.err.println("Warning: Invalid texture parameters");
			return;
		}

		// TODO: Need to think about things like better alignment across multiple faces.
	}

	private Button create_arrow_button(String label, final TextField tf, final double amount)
	{
		Button b = new Button(label);
		b.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				try
				{
					double d = Double.parseDouble(tf.getText());
					d += amount;
					tf.setText(String.valueOf(d));
					apply_details();
				}
				catch(Exception ex) { System.err.println("Warning: Invalid texture parameters"); }
			}
		});
		return b;
	}

	private Panel create_arrows_panel(final TextField tf, final double amount)
	{
		Panel p = new Panel();
		p.setLayout(new GridLayout(1,0));
		p.add(create_arrow_button("<", tf, -amount));
		p.add(create_arrow_button(">", tf, amount));
		return p;
	}

	private static void set_caret(TextField tf)
	{
		tf.setCaretPosition(tf.getText().length());
	}

	private void set_selected_texture(String texture)
	{
		m_nameChoice.select(texture);
		update_thumbnail();
	}

	private void update_thumbnail()
	{
		Texture texture = TextureManager.instance().get_texture(m_nameChoice.getSelectedItem());
		if(texture != null) m_thumbnail.set_image(texture.get_thumbnail());
		else m_thumbnail.set_image(null);
		m_thumbnail.repaint();
	}
}