package net.gxstudios.shipwreck.misc;

import java.util.LinkedList;

import net.gxstudios.shipwreck.test.TestHarnessAdapter;

/**
This class contains miscellaneous utility functions.
*/
public class MiscUtil
{
	//################## PUBLIC METHODS ##################//
	/**
	Generates an array containing precisely the same elements as the specified (non-empty) list.

	<p><b>Preconditions:</b>
	<dl>
	<dd>list != null && !list.isEmpty()
	</dl>

	@param list	The list from which to generate the array
	@return		An array whose elements are identical to, and in the same order as,
				those of the list passed in
	*/
	public static <T> T[] array_from_list(LinkedList<T> list)
	{
		// Check the preconditions.
		if(list == null || list.isEmpty()) throw new java.lang.Error();

		T[] arr = (T[])java.lang.reflect.Array.newInstance(list.getFirst().getClass(), list.size());

		// Copy the list elements into the array.
		int i = 0;
		for(T t: list) arr[i++] = t;

		return arr;
	}

	/**
	Generates a list containing precisely the same elements as the specified iterable.

	<p><b>Notes:</b>
	<dl>
	<dd>Since LinkedList is a subtype of Iterable, it's possible that the iterable
	passed in will already be a list. If so, we don't need to generate a new list,
	we can just cast the type of the existing iterable.
	</dl>

	@param it	The iterable from which to generate the list
	@return		A LinkedList whose elements are identical to, and in the same order as,
				those of the iterable passed in
	*/
	public static <T> LinkedList<T> list_from_iterable(Iterable<T> it)
	{
		if(it instanceof LinkedList) return (LinkedList<T>)it;

		LinkedList<T> ret = new LinkedList<T>();
		for(T t: it) ret.add(t);
		return ret;
	}

	/**
	Makes a singleton list (of the right type) from the specified object passed in.

	<p><b>Preconditions:</b>
	<dl>
	<dd>t != null
	</dl>

	@param t				The object from which to make the singleton list
	@return					A singleton list containing the object
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static <T> LinkedList<T> make_singleton_list(T t)
	{
		if(t == null) throw new java.lang.Error();

		LinkedList<T> ret = new LinkedList<T>();
		ret.add(t);
		return ret;
	}

	/**
	Reverses (in place) the specified array.

	<p><b>Preconditions:</b>
	<dl>
	<dd>arr != null
	</dl>

	@param arr				The array to reverse
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static <T> void reverse_array(T[] arr)
	{
		if(arr == null) throw new java.lang.Error();

		for(int i=0, len=arr.length/2; i<len; ++i)
		{
			int j = arr.length - 1 - i;
			T temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}

	/**
	Same as the generic version, but for double arrays.
	(Note that needing this is hugely irritating!)
	*/
	public static void reverse_array(double[] arr)
	{
		if(arr == null) throw new java.lang.Error();

		for(int i=0, len=arr.length/2; i<len; ++i)
		{
			int j = arr.length - 1 - i;
			double temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}

	/**
	Returns a string containing the contents of the specified array.

	<p><b>Preconditions:</b>
	<dl>
	<dd>arr != null
	</dl>

	@param arr				The array for which to generate a string
	@return					...think about it...
	@throws java.lang.Error	If the preconditions are violated
	*/
	public static <T> String string_from_array(T[] arr)
	{
		if(arr == null) throw new java.lang.Error();

		StringBuffer sb = new StringBuffer("[ ");
		for(int i=0, len=arr.length; i<len; ++i)
		{
			sb.append(arr[i]);
			if(i < len-1) sb.append(", ");
		}
		sb.append(" ]");
		return sb.toString();
	}

	/**
	Same as the generic version, but for double arrays.
	(Note that needing this is hugely irritating!)
	*/
	public static String string_from_array(double[] arr)
	{
		if(arr == null) throw new java.lang.Error();

		StringBuffer sb = new StringBuffer("[ ");
		for(int i=0, len=arr.length; i<len; ++i)
		{
			sb.append(arr[i]);
			if(i < len-1) sb.append(", ");
		}
		sb.append(" ]");
		return sb.toString();
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_array_from_list()
		{
			// Test what happens when we erroneously pass in null.
			try
			{
				output(string_from_array(array_from_list(null)), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			LinkedList<Integer> list_a = new LinkedList<Integer>();

			// Test the empty list case.
			try
			{
				output(string_from_array(array_from_list(list_a)), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}
			
			// Test using a simple list of Integers.
			list_a.add(3); list_a.add(1); list_a.add(4);
			output(string_from_array(array_from_list(list_a)), string_from_array(new Integer[] {3,1,4}));

			// Test using Strings.
			LinkedList<String> list_b = new LinkedList<String>();
			list_b.add("This"); list_b.add("is"); list_b.add("a"); list_b.add("test!");
			output(string_from_array(array_from_list(list_b)), string_from_array(new String[] {"This", "is", "a", "test!"}));

			// TODO: More tests might be useful here.
		}

		public void test_reverse_array()
		{
			// Test arrays with 0-3 elements, thus checking both the limit cases,
			// and whether both even- and odd-length arrays are handled correctly.

			Double[] arr = new Double[] {};
			reverse_array(arr);
			output(string_from_array(arr), string_from_array(new Double[] {}));

			arr = new Double[] {23.};
			reverse_array(arr);
			output(string_from_array(arr), string_from_array(new Double[] {23.}));

			arr = new Double[] {23., 9.};
			reverse_array(arr);
			output(string_from_array(arr), string_from_array(new Double[] {9., 23.}));

			arr = new Double[] {23., 9., 84.};
			reverse_array(arr);
			output(string_from_array(arr), string_from_array(new Double[] {84., 9., 23.}));

			// Test an "arbitrary" longer array.

			arr = new Double[] {3.,1.,4.,1.,5.,9.};
			reverse_array(arr);
			output(string_from_array(arr), string_from_array(new Double[] {9., 5., 1., 4., 1., 3.}));

			// Finally, test an error case.

			try
			{
				reverse_array((Double[])null);
				output("", "Error");
			}
			catch(Error e)
			{
				output("", "");
			}
		}

		// TODO: Write the other tests.
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}