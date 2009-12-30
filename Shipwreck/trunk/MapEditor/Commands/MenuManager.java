package MapEditor.Commands;

import java.awt.*;
import java.util.*;

/**
This singleton class is currently just a simple wrapper around
a TreeMap&ltString, MenuItem&gt. It may eventually become something
more involved, or it may not, depending on how much we want to
modify the menus after initialisation.
*/
public class MenuManager
{
	//################## SINGLETON IMPLEMENTATION ##################//
	private MenuManager() {}		// deliberately prevent instantiation

	private static MenuManager s_instance = null;

	public static MenuManager instance()
	{
		if(s_instance == null) s_instance = new MenuManager();
		return s_instance;
	}

	//################## PRIVATE VARIABLES ##################//
	private TreeMap<String, MenuItem> m_itemMap = new TreeMap<String, MenuItem>();

	//################## PUBLIC METHODS ##################//
	/**
	Adds the specified (name, item) pair to the item map.

	@param name					The name we want to use to refer to the item
	@param item					The item itself
	*/
	public void add_item(String name, MenuItem item)
	{
		m_itemMap.put(name, item);
	}

	/**
	Returns the item associated with the specified name.

	@param name					The name associated with the item we want
	@throws java.lang.Error		If no item with that name is in the map
	*/
	public MenuItem get_item(String name)
	{
		if(m_itemMap.containsKey(name)) return m_itemMap.get(name);
		else throw new java.lang.Error();
	}
}