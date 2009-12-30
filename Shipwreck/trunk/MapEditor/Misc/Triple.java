package MapEditor.Misc;

public class Triple<T1,T2,T3>
{
	public T1 first;
	public T2 second;
	public T3 third;

	public Triple() {}

	public Triple(T1 t1, T2 t2, T3 t3)
	{
		first = t1;
		second = t2;
		third = t3;
	}

	public String toString()
	{
		return "(" + String.valueOf(first) + ", " + String.valueOf(second) + ", " + String.valueOf(third) + ")";
	}

	public static <T1,T2,T3> Triple<T1,T2,T3> make_triple(T1 t1, T2 t2, T3 t3)
	{
		return new Triple<T1,T2,T3>(t1,t2,t3);
	}
}