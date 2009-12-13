package MapEditor.GUI;

import java.awt.*;
import java.awt.event.*;

/**
This class implements a general input dialog which can be used to query the
user for a string.
*/
public class InputDialog extends Dialog
{
	//################## PRIVATE VARIABLES ##################//
	private TextField m_input;

	//################## CONSTRUCTORS ##################//
	/**
	Constructs an InputDialog with the specified owner, title, initial input and set of
	descriptive messages.

	@param owner		The parent frame of the dialog (in practice, generally the main window)
	@param title		The title for the dialog
	@param initialInput	The initial value of the input, before it's changed by the user
	@param messages		A varargs array of descriptive messages describing what the user needs to input
	*/
	public InputDialog(Frame owner, String title, String initialInput, String ... messages)
	{
		super(owner, title, true);

		setLayout(new GridLayout(messages.length + 2, 0));
		for(String m: messages) add(new Label(m));

		m_input = new TextField(initialInput);
		m_input.setCaretPosition(initialInput.length());
		add(m_input);

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

	//################## PUBLIC METHODS ##################//
	/**
	Returns the string typed into the dialog by the user. To be called after the dialog
	has terminated, since input dialogs are modal.

	@return	The string typed by the user
	*/
	public String result()
	{
		return m_input.getText();
	}
}