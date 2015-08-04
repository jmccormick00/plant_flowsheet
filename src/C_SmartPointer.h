/*=========================================================================
/ James McCormick - C_SmartPointer.h
/ The header file for the SmarPointer system.  Any object that wants to use
/ a smart pointer must use this base class. 
/==========================================================================*/

#ifndef _SMARTPOINTER_
#define _SMARTPOINTER_

/*=========================================================================
/ C_Object - Class
/ An object class that will be used with the smart pointer class
/ This is a super class for any class that wants to use the smart 
/ pointer class
/==========================================================================*/
class C_SmartPointerObject
{
public:
	// Constructor
	C_SmartPointerObject() : d_sNumRef(0) {}
	virtual ~C_SmartPointerObject() {}

private:
	unsigned short d_sNumRef;		
	void DecrementRef();
	void IncrementRef() { d_sNumRef++; }

	template <class T> friend class C_SmartPointer;
};

//------------------------------------------------------------------------------
// Decrements the reference count for the object
// If the count has reached zero, the object deletes itself
//------------------------------------------------------------------------------
inline void C_SmartPointerObject::DecrementRef()
{
	if( --d_sNumRef == 0 ) 
		delete this;
}

/*=========================================================================
/ A class that will do dynamic memory deletion if the object that the pointer
/ points to is no longer referenced.  Can be used with any class that 
/ inherents from the C_SmartPointerObject class
/==========================================================================*/
template <class T> class C_SmartPointer
{
private:
		T* d_ObjectPtr;		// The pointer to the object
	
public:
		// Constructor
		C_SmartPointer(T* obj = 0);
		
		// Copy Constructor
		C_SmartPointer(const C_SmartPointer<T> &obj);
		
		// Destructor
		~C_SmartPointer();
	
		// Implicit Conversion
		operator T* () const { return d_ObjectPtr; }		// Allows you to pass this to a function that takes a T* pointer as an argument
		T& operator* () const { return *d_ObjectPtr; }
		T* operator-> () const { return d_ObjectPtr; }
	
		// Assigment operations
		C_SmartPointer& operator= (T* obj);
		C_SmartPointer& operator= (const C_SmartPointer<T> obj);
	
		// Comparisons
		bool operator== (const T* obj) const { return d_ObjectPtr == obj; }
		bool operator!= (const T* obj) const { return d_ObjectPtr != obj; }
		bool operator== (const C_SmartPointer<T> obj) const { return d_ObjectPtr == obj.d_ObjectPtr; }
		bool operator!= (const C_SmartPointer<T> obj) const { return d_ObjectPtr != obj.d_ObjectPtr; }
};


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
template <class T> C_SmartPointer<T>::C_SmartPointer(T* obj)
{
	d_ObjectPtr = obj;
	if( d_ObjectPtr )
		d_ObjectPtr->IncrementRef();
}

template <class T>
C_SmartPointer<T>::C_SmartPointer(const C_SmartPointer<T> &obj)
{
	d_ObjectPtr = obj;
	if( d_ObjectPtr )
		d_ObjectPtr->IncrementRef();
}


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
template <class T> 
C_SmartPointer<T>::~C_SmartPointer()
{
	if(d_ObjectPtr) 
		d_ObjectPtr->DecrementRef();
}


template <class T>
C_SmartPointer<T>& C_SmartPointer<T>::operator= (T* obj)
{
	if( d_ObjectPtr != obj)
	{
		// If the new pointer is valid, then increment the ref count 
		// for the object
		if(obj)
			obj->IncrementRef();
		
		// If the classes pointer points to an object, then decrement the count
		// for that object
		if(d_ObjectPtr)
			d_ObjectPtr->DecrementRef();
		
		d_ObjectPtr = obj;
	}
	return *this;
}


template <class T>
C_SmartPointer<T>& C_SmartPointer<T>::operator= (const C_SmartPointer<T> obj)
{
	if(d_ObjectPtr != obj.d_ObjectPtr)
	{
		// If there is an object being pointed to by the SmartPtr on the right 
		// hand side, then increment the count
		if(obj.d_ObjectPtr)
			obj->IncrementRef();
		
		// If the classes pointer points to an object, then decrement the count
		// for that object
		if(d_ObjectPtr)
			d_ObjectPtr->DecrementRef();
		
		d_ObjectPtr = obj.d_ObjectPtr;
	}
	return *this;
}

#endif // _SMARTPOINTER_