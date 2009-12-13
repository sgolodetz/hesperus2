package MapEditor.GUI;

import MapEditor.Commands.*;
import MapEditor.Maps.*;
import MapEditor.Math.Vectors.Vector3d;
import java.awt.*;
import java.awt.event.*;

public class MapTranslationDialog extends Dialog
{
	//################## PRIVATE VARIABLES ##################//
	private Map m_map;

	private TextField m_offsetXField = new TextField("0");
	private TextField m_offsetYField = new TextField("0");
	private TextField m_offsetZField = new TextField("0");
	{
		set_caret(m_offsetXField);
		set_caret(m_offsetYField);
		set_caret(m_offsetZField);
	}

	//################## CONSTRUCTORS ##################//
	public MapTranslationDialog(Frame owner, Map map)
	{
		super(owner, "Translate Map", true);

		m_map = map;

		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				dispose();
			}
		});

		setLayout(new BorderLayout(0, 5));

		Panel top = new Panel();
		top.setLayout(new GridLayout(0, 2, 5, 5));
		top.add(new Label("X Offset:"));	top.add(m_offsetXField);
		top.add(new Label("Y Offset:"));	top.add(m_offsetYField);
		top.add(new Label("Z Offset:"));	top.add(m_offsetZField);
		add("Center", top);

		Panel bottom = new Panel();
		bottom.setLayout(new GridLayout(0, 2, 5, 0));
		Button okButton = new Button("OK");
		okButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				translate_map();
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
		add("South", bottom);

		pack();
		setLocationRelativeTo(owner);		// centre the dialog relative to its owner
		setResizable(false);
		setVisible(true);
	}

	//################## PRIVATE METHODS ##################//
	private static void set_caret(TextField tf)
	{
		tf.setCaretPosition(tf.getText().length());
	}

	/**
	Translates the map by the offset the user specified (if valid).
	Otherwise logs the error and does nothing.
	*/
	private void translate_map()
	{
		try
		{
			Vector3d offset = new Vector3d();
			offset.x = Double.parseDouble(m_offsetXField.getText());
			offset.y = Double.parseDouble(m_offsetYField.getText());
			offset.z = Double.parseDouble(m_offsetZField.getText());

			Command c = MapCommands.translate_map(m_map, offset);
			if(c != null) CommandManager.instance().execute_command(c);
		}
		catch(Exception e)
		{
			System.err.println("Warning: Invalid translation offsets");
		}
	}
}