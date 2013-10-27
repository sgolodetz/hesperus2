package net.gxstudios.shipwreck.gui;

import java.awt.Button;
import java.awt.Dialog;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import net.gxstudios.shipwreck.maps.Map;
import net.gxstudios.shipwreck.maps.MapDetails;

@SuppressWarnings("serial")
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