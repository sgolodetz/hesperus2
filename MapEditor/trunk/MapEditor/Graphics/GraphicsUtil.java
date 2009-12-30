package MapEditor.Graphics;

import MapEditor.Math.Vectors.Vector3d;
import java.awt.*;
import java.awt.event.*;
import javax.media.opengl.*;

public class GraphicsUtil
{
	public static void draw_cuboid(GL2 gl, Vector3d p1, Vector3d p2)
	{
		gl.glBegin(GL.GL_LINE_LOOP);
			gl.glVertex3d(p1.x, p1.y, p1.z);
			gl.glVertex3d(p2.x, p1.y, p1.z);
			gl.glVertex3d(p2.x, p2.y, p1.z);
			gl.glVertex3d(p1.x, p2.y, p1.z);
		gl.glEnd();

		gl.glBegin(GL.GL_LINE_LOOP);
			gl.glVertex3d(p1.x, p1.y, p2.z);
			gl.glVertex3d(p2.x, p1.y, p2.z);
			gl.glVertex3d(p2.x, p2.y, p2.z);
			gl.glVertex3d(p1.x, p2.y, p2.z);
		gl.glEnd();

		gl.glBegin(GL.GL_LINES);
			gl.glVertex3d(p1.x, p1.y, p1.z);
			gl.glVertex3d(p1.x, p1.y, p2.z);

			gl.glVertex3d(p2.x, p1.y, p1.z);
			gl.glVertex3d(p2.x, p1.y, p2.z);

			gl.glVertex3d(p2.x, p2.y, p1.z);
			gl.glVertex3d(p2.x, p2.y, p2.z);

			gl.glVertex3d(p1.x, p2.y, p1.z);
			gl.glVertex3d(p1.x, p2.y, p2.z);
		gl.glEnd();
	}

	public static void draw_line(Graphics g, double x1, double y1, double x2, double y2)
	{
		g.drawLine((int)Math.round(x1), (int)Math.round(y1), (int)Math.round(x2), (int)Math.round(y2));
	}

	public static void draw_oval(Graphics g, double x1, double y1, double x2, double y2)
	{
		draw_oval(g, (int)Math.round(x1), (int)Math.round(y1), (int)Math.round(x2), (int)Math.round(y2));
	}

	public static void draw_oval(Graphics g, int x1, int y1, int x2, int y2)
	{
		int minx = x1 < x2 ? x1 : x2;
		int miny = y1 < y2 ? y1 : y2;
		int width = Math.abs(x2 - x1), height = Math.abs(y2 - y1);
		g.drawOval(minx, miny, width, height);
	}

	public static void draw_rectangle(Graphics g, double x1, double y1, double x2, double y2)
	{
		draw_rectangle(g, (int)Math.round(x1), (int)Math.round(y1), (int)Math.round(x2), (int)Math.round(y2));
	}

	public static void draw_rectangle(Graphics g, int x1, int y1, int x2, int y2)
	{
		int minx = x1 < x2 ? x1 : x2;
		int miny = y1 < y2 ? y1 : y2;
		int width = Math.abs(x2 - x1), height = Math.abs(y2 - y1);
		g.drawRect(minx, miny, width, height);
	}

	public static void fill_oval(Graphics g, double x1, double y1, double x2, double y2)
	{
		fill_oval(g, (int)Math.round(x1), (int)Math.round(y1), (int)Math.round(x2), (int)Math.round(y2));
	}

	public static void fill_oval(Graphics g, int x1, int y1, int x2, int y2)
	{
		int minx = x1 < x2 ? x1 : x2;
		int miny = y1 < y2 ? y1 : y2;
		int width = Math.abs(x2 - x1), height = Math.abs(y2 - y1);
		g.fillOval(minx, miny, width, height);
	}

	public static void message_box(Frame owner, String title, String ... messages)
	{
		final Dialog dialog = new Dialog(owner, title, true);
		dialog.setLayout(new GridLayout(messages.length + 1, 0));
		for(String m: messages) dialog.add(new Label(m));
		Button okButton = new Button("OK");
		okButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				dialog.dispose();
			}
		});
		dialog.add(okButton);
		dialog.pack();
		dialog.setLocationRelativeTo(owner);		// centre the dialog relative to its owner
		dialog.setVisible(true);
	}
}