package net.gxstudios.shipwreck.misc;

public class Pair<T1,T2>
{
	public T1 first;
	public T2 second;

	public Pair() {}

	public Pair(T1 t1, T2 t2)
	{
		first = t1;
		second = t2;
	}

	public String toString()
	{
		return "(" + String.valueOf(first) + ", " + String.valueOf(second) + ")";
	}

	public static <T1,T2> Pair<T1,T2> make_pair(T1 t1, T2 t2)
	{
		return new Pair<T1,T2>(t1,t2);
	}
}