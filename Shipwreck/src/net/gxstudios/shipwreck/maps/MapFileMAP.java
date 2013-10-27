package net.gxstudios.shipwreck.maps;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.LinkedList;

import net.gxstudios.shipwreck.brushes.PolyhedralBrush;
import net.gxstudios.shipwreck.bsp.TreeUtil;
import net.gxstudios.shipwreck.geom.planar.Polygon;
import net.gxstudios.shipwreck.math.MathUtil;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.textures.TextureManager;
import net.gxstudios.shipwreck.textures.TexturePlane;

public class MapFileMAP implements IMapLoader	// .map files are output by editors like Worldcraft
{
	//################## SINGLETON IMPLEMENTATION ##################//
	private MapFileMAP() {}		// deliberately prevent instantiation

	private static MapFileMAP s_instance = null;

	public static MapFileMAP instance()
	{
		if(s_instance == null) s_instance = new MapFileMAP();
		return s_instance;
	}

	//################## NESTED CLASSES ##################//
	private interface EntityProcessor
	{
		void process(BufferedReader br, Map map, HashMap<String,String> properties) throws IOException;
	}

	//################## PRIVATE VARIABLES ##################//
	private HashMap<String,EntityProcessor> m_entityProcessors = new HashMap<String,EntityProcessor>();
	{
		m_entityProcessors.put("default", new EntityProcessor()
		{
			public void process(BufferedReader br, Map map, HashMap<String,String> properties) throws IOException
			{
				System.err.println("Warning: Missing the proper entity processor, staggering on with the default one");

				// Read to the end of the current entity description.
				int bracketCount = 2;	// we're initially nested inside two levels of brackets
				String line;
				while((line = br.readLine()) != null)
				{
					if(line.equals("{")) ++bracketCount;
					else if(line.equals("}")) --bracketCount;

					if(bracketCount == 0) break;
				}
			}
		});

		m_entityProcessors.put("worldspawn", new EntityProcessor()
		{
			private Polygon parse_brush_plane(String line)
			{
				String[] tokens = line.split(" ", 0);

				Vector3d[] verts = new Vector3d[3];
				for(int i=0; i<3; ++i)
				{
					int offset = i*5 + 1;
					verts[i] = new Vector3d(	Double.parseDouble(tokens[offset]),
											Double.parseDouble(tokens[offset+1]),
											Double.parseDouble(tokens[offset+2])	);
				}

				// The vertices are specified in clockwise winding order, we need to swap them round.
				Vector3d temp = verts[1]; verts[1] = verts[2]; verts[2] = temp;

				//String texture = tokens[15];
				String texture = "NULL";	// TODO: Eventually I need to find a way of reading the relevant .wad file.

				double offsetU = Double.parseDouble(tokens[20]);
				double offsetV = Double.parseDouble(tokens[26]);
				double angleDegrees = Double.parseDouble(tokens[28]);
				double scaleU = Double.parseDouble(tokens[29]);
				double scaleV = Double.parseDouble(tokens[30]);
				Vector3d normal = MathUtil.calculate_normal(verts[0], verts[1], verts[2]);
				TexturePlane texturePlane = new TexturePlane(normal, scaleU, scaleV, offsetU, offsetV, angleDegrees);

				//System.err.println("---- " + normal + " " + scaleU + " " + scaleV + " " + offsetU + " " + offsetV + " " + angleDegrees);	// temporary

				return new Polygon(verts, texture, texturePlane);	// TODO: We could pass the normal in here to avoid it being recalculated.
			}

			private boolean read_brush(BufferedReader br, Map map) throws IOException
			{
				// At the entry to this function, we've just read the opening brace of a brush description.

				//System.err.println("Brush");	// temporary

				LinkedList<Polygon> polys = new LinkedList<Polygon>();

				String line;
				while((line = br.readLine()) != null)
				{
					if(line.equals("}")) break;

					//System.err.println("*** " + line);	// temporary

					polys.add(parse_brush_plane(line));
				}

				LinkedList<Polygon> newPolys = TreeUtil.generate_convex_volume_polygons_from_surrounding_polygons(polys);
				PolyhedralBrush b = new PolyhedralBrush(newPolys);
				map.add_brush(b);

				line = br.readLine();
				if(line.equals("{")) return true;			// there's another brush to process
				else if(line.equals("}")) return false;		// we've processed all the brushes
				else throw new IOException("Expected another brush or } while processing worldspawn");
			}

			public void process(BufferedReader br, Map map, HashMap<String,String> properties) throws IOException
			{
				// At the entry to this function, we've just read the opening brace of the first brush description.

				// FIXME: Eventually we want to load textures from the .wad file.
				TextureManager.instance().unload_all();
				TextureManager.instance().load_texture("NULL", "textures/null.jpg");

				// Check the .map version number is the one we can read (namely 220).
				String version = properties.get("mapversion");
				if(version == null || !version.equals("220")) throw new IOException("Invalid .map version");

				while(read_brush(br, map)) {}
			}
		});
	}

	//################## PUBLIC METHODS ##################//
	public Map load(String filename) throws IOException
	{
		Map map = new Map();

		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(filename)));

			while(read_entity(br, map)) {}

			br.close();
		}
		catch(IOException e)
		{
			//e.printStackTrace(System.out);
			throw e;
		}

		return map;
	}

	//################## PRIVATE METHODS ##################//
	/**
	Reads an entity and does whatever's necessary to add it to the map.

	@param br	The buffered reader associated with the .map file we're loading
	@param map	The map to which we're adding entities
	@return		true, if we successfully read an entity, or false otherwise (e.g. if we reached the end of the file)
	*/
	private boolean read_entity(BufferedReader br, Map map) throws IOException
	{
		br.readLine();	// read the opening brace

		HashMap<String,String> properties = new HashMap<String,String>();

		String line;
		while((line = br.readLine()) != null)
		{
			if(line.startsWith("\""))
			{
				String[] tokens = line.split(" ", 0);
				String key = tokens[0].substring(1, tokens[0].length()-1);
				String value = tokens[1].substring(1, tokens[1].length()-1);
				properties.put(key, value);
			}
			else if(line.equals("{"))	// this entity contains brushes that we need to process
			{
				String classname = properties.get("classname");
				EntityProcessor p = null;
				if(classname != null) p = m_entityProcessors.get(classname);
				if(p == null) p = m_entityProcessors.get("default");

				p.process(br, map, properties);

				return true;
			}
			else if(line.equals("}"))	// this entity only has properties, so process those and we're done
			{
				String classname = properties.get("classname");
				if(classname != null)
				{
					EntityProcessor p = m_entityProcessors.get(classname);

					if(p != null) p.process(br, map, properties);
					else System.err.println("Warning: Missing entity processor for type " + classname);
				}
				else System.err.println("Warning: Missing classname in entity description!");

				return true;
			}
			else throw new IOException("Invalid line in entity description");
		}
		return false;
	}

	// TEMPORARY TEST
	/*public static void main(String[] args) throws Exception
	{
		MapFileMAP test = MapFileMAP.instance();
		//try
		{
			test.load("MapEditor\\tricky.map");
		}
		//catch(Exception e) { System.err.println(e.getMessage()); }
	}*/
}