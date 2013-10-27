package net.gxstudios.shipwreck.test;

import java.util.LinkedList;
import java.util.Map;
import java.util.TreeMap;
import java.lang.reflect.*;

public abstract class TestHarnessAdapter implements ITestHarness
{
	//################## PRIVATE VARIABLES ##################//
	private int m_testIndex;
	private int m_testsDone = 0, m_testsSucceeded = 0;
	private LinkedList<String> m_failedTests = new LinkedList<String>();
	private LinkedList<String> m_untestedMethods = new LinkedList<String>();
	private String m_funcName;

	//################## PUBLIC METHODS ##################//
	final public void run_tests()
	{
		start_tests();

		Class harnessClass = getClass();						// the test harness class derived from this
		Class testeeClass = harnessClass.getDeclaringClass();	// the class in which the harness is declared (i.e. the thing we're testing)

		// Keep a record of which methods have been tested so that we can tell which ones haven't been afterwards and output a warning accordingly.
		Map<String,Boolean> testedMap = new TreeMap<String,Boolean>();
		for(Method m: testeeClass.getDeclaredMethods())
		{
			// We'd usually expect to test all (non-abstract) public methods (except main and toString).
			if(!m.getName().equals("main") && !m.getName().equals("toString"))
			{
				int modifiers = m.getModifiers();
				if(Modifier.isPublic(modifiers) && !Modifier.isAbstract(modifiers))
				{
					testedMap.put(m.getName(), false);
				}
			}
		}

		// Find all test methods and run them.
		for(Method m: harnessClass.getDeclaredMethods())
		{
			String methodName = m.getName();
			if(methodName.startsWith("test_"))
			{
				String testeeMethod = methodName.substring("test_".length());

				begin_test(testeeMethod);
				testedMap.put(testeeMethod, true);

				try { m.invoke(this, new Object[] {}); }
				catch(Exception e) { System.err.println(e); }
			}
		}

		// Check the map of tested methods and add any marked as untested to the untested list.
		for(Map.Entry<String,Boolean> entry: testedMap.entrySet())
		{
			if(!entry.getValue()) m_untestedMethods.add(entry.getKey());
		}

		end_tests();
	}

	//################## PROTECTED METHODS ##################//
	/**
	Gets the test harness ready to test a new method.

	@param funcName		The name of the new method to test
	*/
	final protected void begin_test(String funcName)
	{
		m_funcName = funcName;
		m_testIndex = 1;
	}

	/**
	Outputs something based on the results of a test and what we were expecting
	the results to be.

	@param o			The results of the test
	@param expected		What we were expecting the results to be (as a String)
	*/
	final protected void output(Object o, String expected)
	{
		String prefix = m_funcName + " #" + String.valueOf(m_testIndex++) + ": ";
		System.out.print(prefix);
		String result = String.valueOf(o);
		if(result.equals(expected))
		{
			System.out.println("OK");
			++m_testsSucceeded;
		}
		else
		{
			String failure = result + "\n--> Expected: " + expected;
			System.out.println(failure);

			// Append the file name, method name and line number where the test failed to
			// aid debugging. We do this by creating an exception and then using the resulting
			// stack trace. (Some might term this "cunning".)
			Throwable t = new Throwable();

			// The element we want is the second one in the stack trace (the bit referring
			// to the method that called this one).
			StackTraceElement e = t.getStackTrace()[1];

			String trace = "\n   <" +	e.getFileName() + ", " +
										e.getMethodName() + ", " +
										e.getLineNumber() + ">";
			m_failedTests.add(prefix + failure + trace);
		}
		++m_testsDone;
	}

	//################## PRIVATE METHODS ##################//
	private void end_tests()
	{
		System.out.println("\nTest Results: " + m_testsDone + " done, " + m_testsSucceeded + " succeeded");
		if(!m_failedTests.isEmpty())
		{
			System.out.print("\nFailed Tests:\n");
			for(String s: m_failedTests) System.out.println("\n" + s);
		}
		if(!m_untestedMethods.isEmpty())
		{
			System.out.print("\nUntested Methods:\n\n");
			for(String s: m_untestedMethods) System.out.println(s);
		}
	}

	private void start_tests()
	{
		System.out.print("Testing: ");

		// Output the class name of what we're testing. Use the same trick as in the output method
		// above to get the name.
		Throwable t = new Throwable();

		// The element we want is the third one in the stack trace (the bit referring
		// to the main method in the class in which the calling TestHarness is nested).
		StackTraceElement e = t.getStackTrace()[2];

		System.out.println(e.getClassName() + "\n");
	}
}