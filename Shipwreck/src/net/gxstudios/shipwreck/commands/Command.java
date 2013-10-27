package net.gxstudios.shipwreck.commands;

import net.gxstudios.shipwreck.test.TestHarnessAdapter;

public abstract class Command
{
	//################## PRIVATE VARIABLES ##################//
	private String m_description;

	//################## CONSTRUCTORS ##################//
	public Command(String description) { m_description = description; }

	//################## PUBLIC ABSTRACT METHODS ##################//
	public abstract void execute();
	public abstract void undo();

	//################## PUBLIC METHODS ##################//
	final public String get_description()
	{
		return m_description;
	}

	/**
	Generates a Command representing c1; c2. Note that null; c2 == c2 and c1; null == c1.

	@param c1			The command to execute first (and undo second)
	@param c2			The command to execute second (and undo first)
	@param description	The description to give the new command
	@return				A Command as described above
	*/
	public static Command seq(final Command c1, final Command c2, final String description)
	{
		if(c1 == null) return c2;
		else if(c2 == null) return c1;
		else
		{
			return new Command(description)
			{
				public void execute()
				{
					c1.execute();
					c2.execute();
				}

				public void undo()
				{
					c2.undo();
					c1.undo();
				}
			};
		}
	}

	/**
	Generates a Command representing cs[0]; cs[1]; ...; cs[cs.length-1].

	<p><b>Preconditions:</b>
	<dl>
	<dd>cs != null && description != null
	<dd>cs[i] must not be null for any 0 <= i < cs.length
	</dl>

	@param cs				The commands to execute (in the order in which they should
							be executed)
	@param description		The description to give the new command
	@return					A Command as described above
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static Command Seq(final Command[] cs, final String description)
	{
		// Check the preconditions.
		if(cs == null) throw new java.lang.Error("Command.Seq: cs must not be null");
		if(description == null) throw new java.lang.Error("Command.Seq: description must not be null");
		for(int i=0, len=cs.length; i<len; ++i)
		{
			if(cs[i] == null) throw new java.lang.Error("Command.Seq: cs[" + i + "] must not be null");
		}

		return new Command(description)
		{
			public void execute()
			{
				for(int i=0, len=cs.length; i<len; ++i)
				{
					cs[i].execute();
				}
			}

			public void undo()
			{
				for(int i=cs.length-1; i>=0; --i)
				{
					cs[i].undo();
				}
			}
		};
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_Seq()
		{
			// Test a valid case.
			final StringBuffer sb = new StringBuffer();

			Command[] cs = new Command[3];
			for(int i=0; i<3; ++i)
			{
				final Integer j = i;
				cs[i] = new Command("")
				{
					public void execute()
					{
						sb.append("[Execute " + j + "]");
					}

					public void undo()
					{
						sb.append("[Undo " + j + "]");
					}
				};
			}

			Command c = Seq(cs, "");
			c.execute();
			c.undo();
			output(sb, "[Execute 0][Execute 1][Execute 2][Undo 2][Undo 1][Undo 0]");

			// Test some invalid cases.

			// Command array is null.
			try
			{
				Seq(null, "");
				output("", "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Description is null.
			try
			{
				Seq(cs, null);
				output("", "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// One of the commands is null.
			cs[0] = null;
			try
			{
				Seq(cs, "");
				output("", "Error");
			}
			catch(Error e)
			{
				output("", "");
			}
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}