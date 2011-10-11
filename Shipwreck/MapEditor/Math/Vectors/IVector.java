package MapEditor.Math.Vectors;

import MapEditor.Misc.Constants;

interface IVector<T> extends Constants
{
	/**
	Adds the parameter vector to this vector.

	@param rhs	The vector to add to this vector
	@return		This vector
	*/
	T add(final T rhs);

	/**
	Returns a copy of this vector (which is distinct in memory from the original).

	@return	...think about it...
	*/
	T clone();

	/**
	Returns the distance between this vector and the parameter vector. Note that both
	are being treated as point vectors here.

	@param rhs	The point whose distance from this point we wish to determine
	@return		...think about it...
	*/
	double distance(final T rhs);

	/**
	Returns the square of the distance between this vector and the parameter vector.
	Note that both are being treated as point vectors here.

	@param rhs	The point whose distance squared from this point we wish to determine
	@return		...think about it...
	*/
	double distance_squared(final T rhs);

	/**
	Returns the dot product of this vector and the parameter vector.

	@param rhs	The other operand of the dot product
	@return		...think about it...
	*/
	double dot(final T rhs);

	/**
	Determines if this vector is equal to the parameter object.

	@param rhs	The object we wish to check for equality with this vector
	@return		true if the parameter is a vector equal to this one, otherwise false
	*/
	boolean equals(Object rhs);

	/**
	Returns this vector's length.

	@return	...think about it...
	*/
	double length();

	/**
	Returns the square of this vector's length.

	@return	...think about it...
	*/
	double length_squared();

	/**
	Negates this vector.

	@return	This vector
	*/
	T negate();

	/**
	Returns a negated version of this vector (which is distinct in memory from the original).

	@return	...think about it...
	*/
	T negated();

	/**
	Normalizes this vector.

	@return	This vector
	@throws java.lang.Error	If the vector's length is too close to zero
	*/
	T normalize();

	/**
	Scales this vector by the specified factor.

	@param factor	The factor by which to scale
	@return			This vector
	*/
	T scale(double factor);

	/**
	Subtracts the parameter vector from this vector.

	@param rhs	The vector to subtract from this vector
	@return		This vector
	*/
	T subtract(final T rhs);

	/**
	Returns a string representation of the vector.

	@return	...think about it...
	*/
	String toString();
}