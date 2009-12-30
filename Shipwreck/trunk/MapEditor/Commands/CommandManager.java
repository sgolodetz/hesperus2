package MapEditor.Commands;

import MapEditor.Event.IRepaintListener;
import MapEditor.Misc.Pair;
import java.awt.*;
import java.util.*;

/**
This singleton class manages the execution and undoing of commands.

<p><b>Formally:</b>
<dl>
<dd><b>represents</b> (executed, undone)
</dl>
*/
public class CommandManager
{
	//################## SINGLETON IMPLEMENTATION ##################//
	private CommandManager() {}		// deliberately prevent instantiation

	private static CommandManager s_instance = null;

	public static CommandManager instance()
	{
		if(s_instance == null) s_instance = new CommandManager();
		return s_instance;
	}

	//################## PRIVATE VARIABLES ##################//
	private IRepaintListener m_repaintListener = null;
	private MenuManager m_menuManager = MenuManager.instance();
	private Stack<Command> m_executed = new Stack<Command>();
	private Stack<Command> m_undone = new Stack<Command>();

	//################## PUBLIC METHODS ##################//
	/**
	Returns true if there is a command to redo, or false otherwise.

	<p><b>Formally:</b>
	<dl>
	<dd><b>returns</b> undone != <>
	</dl>
	*/
	public boolean can_redo()
	{
		return !m_undone.empty();
	}

	/**
	Returns true if there is a command to undo, or false otherwise.

	<p><b>Formally:</b>
	<dl>
	<dd><b>returns</b> executed != <>
	</dl>
	*/
	public boolean can_undo()
	{
		return !m_executed.empty();
	}

	/**
	Executes the specified command c.

	<p><b>Formally:</b>
	<dl>
	<dd><b>ensures</b> executed = c : <b>old</b>(executed), undone = <>
	<dd><b>invokes</b> c.execute()
	</dl>

	@param c	The command to execute
	*/
	public void execute_command(Command c)
	{
		m_executed.push(c);
		m_undone.clear();
		c.execute();
		if(m_repaintListener != null) m_repaintListener.repaint_full();
		update_menu_commands();
	}

	/**
	Executes the specified command c. If the executed stack is non-empty and the get_description
	method of the most recent command returns one of the specified precursors, then we compress
	the previous command and this one into a single command. Otherwise, we just execute the
	specified command as is. The idea is to avoid making undo/redoing more user-intensive
	than it needs to be. There's no point, for example, in forcing the user to undo both
	brush creation and resizing when a new brush is dragged out.

	@param c			The command to execute
	@param precursors	A list of String pairs, each of which has a first component specifying
						the description of a previous command with which this one can be combined
						and a second component specifying the description to give the new command
						resulting from such a combination
	*/
	public void execute_compressible_command(Command c, Pair<String,String> ... precursors)
	{
		if(m_executed.empty()) execute_command(c);
		else
		{
			Command last = m_executed.peek();
			String lastDescription = last.get_description();
			for(Pair<String,String> p: precursors)
			{
				if(p.first.equals(lastDescription))
				{
					m_executed.pop();
					m_executed.push(Command.seq(last, c, p.second));
					m_undone.clear();
					c.execute();
					if(m_repaintListener != null) m_repaintListener.repaint_full();
					update_menu_commands();
					return;
				}
			}

			// If we got here, we couldn't combine the last command with this one, so just execute
			// the command that was passed in as normal.
			execute_command(c);
		}
	}

	/**
	Redoes the last command undone, if any, otherwise throws an error.

	<p><b>Formally:</b>
	<dl>
	<dd><b>requires</b> undone != <>
	<dd><b>ensures</b> executed = head(<b>old</b>(undone)) : <b>old</b>(executed) and undone = tail(<b>old</b>(undone))
	<dd><b>invokes</b> head(<b>old</b>(undone)).execute()
	</dl>

	@throws java.lang.Error		If there is no command to redo (i.e. if the precondition is violated).
	*/
	public void redo()
	{
		if(can_redo())
		{
			Command c = m_undone.pop();
			m_executed.push(c);
			c.execute();
			if(m_repaintListener != null) m_repaintListener.repaint_full();
			update_menu_commands();
		}
		else throw new java.lang.Error();
	}

	/**
	Resets the command manager.

	<p><b>Formally:</b>
	<dl>
	<dd><b>ensures</b> executed = <> and undone = <>
	</dl>
	*/
	public void reset()
	{
		m_executed.clear();
		m_undone.clear();
		update_menu_commands();
	}

	/**
	Sets the repaint listener associated with the command manager.

	@param listener	The repaint listener to associate with the command manager
	*/
	public void set_repaint_listener(IRepaintListener listener)
	{
		m_repaintListener = listener;
	}

	/**
	Undoes the last command executed, if any, otherwise throws an error.

	<p><b>Formally:</b>
	<dl>
	<dd><b>requires</b> executed != <>
	<dd><b>ensures</b> executed = tail(<b>old</b>(executed)) and undone = head(<b>old</b>(executed)) : <b>old</b>(undone)
	<dd><b>invokes</b> head(<b>old</b>(executed)).undo()
	</dl>

	@throws java.lang.Error		If there is no command to undo (i.e. if the precondition is violated).
	*/
	public void undo()
	{
		if(can_undo())
		{
			Command c = m_executed.pop();
			m_undone.push(c);
			c.undo();
			if(m_repaintListener != null) m_repaintListener.repaint_full();
			update_menu_commands();
		}
		else throw new java.lang.Error();
	}

	//################## PRIVATE METHODS ##################//
	private void update_menu_commands()
	{
		MenuItem undoItem = m_menuManager.get_item("Edit_Undo");
		MenuItem redoItem = m_menuManager.get_item("Edit_Redo");
		undoItem.setEnabled(can_undo());
		undoItem.setLabel(can_undo() ? "Undo " + m_executed.peek().get_description() : "Undo");
		redoItem.setEnabled(can_redo());
		redoItem.setLabel(can_redo() ? "Redo " + m_undone.peek().get_description() : "Redo");
	}
}