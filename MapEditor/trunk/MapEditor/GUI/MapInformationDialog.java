package MapEditor.GUI;

import MapEditor.Maps.Map;
import MapEditor.Maps.MapDetails;
import java.awt.*;
import java.awt.event.*;

public class MapInformationDialog extends Dialog
{
	public MapInformationDialog(Frame owner, Map map)
	{
		super(owner, "Map Information", true);

		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				dispose();
			}
		});

		setLayout(new GridLayout(0,1));

		MapDetails details = map.details();
		add(new Label("Brush Count: " + details.m_brushCount));
		add(new Label("Face Count: " + details.m_faceCount));

		Button okButton = new Button("OK");
		okButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				dispose();
			}
		});
		add(okButton);

		pack();
		setLocationRelativeTo(owner);		// centre the dialog relative to its owner
		setVisible(true);
	}
}