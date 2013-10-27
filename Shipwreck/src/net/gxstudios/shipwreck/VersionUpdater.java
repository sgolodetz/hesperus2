package net.gxstudios.shipwreck;

import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class VersionUpdater
{
	public static void main(String[] args) throws Exception
	{
		DateFormat versionFormat = new SimpleDateFormat("yyyy.MM.dd.HHmm");
		PrintWriter pw = new PrintWriter(new FileOutputStream("resources/Version.txt"));
		pw.print(versionFormat.format(new Date()));
		pw.close();
	}
}