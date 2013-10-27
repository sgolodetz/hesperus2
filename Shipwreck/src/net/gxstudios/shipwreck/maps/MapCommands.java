package net.gxstudios.shipwreck.maps;

import java.util.LinkedList;

import net.gxstudios.shipwreck.brushes.IBrush;
import net.gxstudios.shipwreck.brushes.TranslatableBrush;
import net.gxstudios.shipwreck.commands.Command;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.misc.MiscUtil;

public class MapCommands
{
	private interface BrushCommandFunctor
	{
		Command command(IBrush b);
	}

	/**
	Returns a command that (when executed) executes the command specified using the BrushCommandFunctor
	on any brush (in the specified map) for which it's valid. If there are no brushes for which it's valid,
	the method returns null instead of a command to execute. If a command is returned, it has the specified
	description.

	@param map			The map on whose brushes we wish to execute the commands
	@param description	The description to give the returned command
	@param f			The BrushCommandFunctor specifying the command to execute on a given brush (note that
						exactly what executing the command involves will usually depend on the brush in question)
	@return				As specified above
	*/
	private static Command for_all_brushes(final Map map, final String description, final BrushCommandFunctor f)
	{
		LinkedList<Command> commands = new LinkedList<Command>();
		for(IBrush b: map.get_brushes())
		{
			Command c = f.command(b);
			if(c != null) commands.add(c);
		}
		if(!commands.isEmpty()) return Command.Seq(MiscUtil.array_from_list(commands), description);
		else return null;
	}

	/**
	Returns a command that (when executed) translates all the brushes in the
	specified map by the specified offset.

	@param map		The map whose brushes are to be translated
	@param offset	The offset by which they are to be translated
	@return			As specified above
	*/
	public static Command translate_map(final Map map, final Vector3d offset)
	{
		return for_all_brushes(map, "Map Translation", new BrushCommandFunctor()
		{
			public Command command(IBrush b)
			{
				// FIXME: If we can find a way to get rid of the instanceof test at some point, we should.
				if(b instanceof TranslatableBrush)
				{
					TranslatableBrush tb = (TranslatableBrush)b;
					return tb.generate_translation_command(offset);
				}
				else return null;	// translation isn't valid on this brush
			}
		});
	}
}