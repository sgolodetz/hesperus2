package net.gxstudios.shipwreck.maps;

import java.io.IOException;

public interface IMapLoader
{
	Map load(String filename) throws IOException;
}