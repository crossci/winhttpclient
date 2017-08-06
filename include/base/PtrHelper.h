#pragma once
#include "define.h"
template <class T>
class CPtrHelper;

template <class T1>
class  U_Ptr
{
	friend class CPtrHelper<T1>;
	T1* m_Ptr;
	int m_count;
	U_Ptr(T1* lp) :m_Ptr(lp), m_count(1)
	{
	}
	~U_Ptr()
	{
		delete m_Ptr;
		m_Ptr = nullptr;
	}
};

template <class T>
class  CPtrHelper
{
private:
	// Pointer variable
	U_Ptr<T>* m_Ptr;
public:
	// Constructors
	CPtrHelper()
		:m_Ptr(0)
	{
	}

	CPtrHelper(T* lp)
	{
		m_Ptr = NULL;
		if (lp != NULL)
		{
			m_Ptr = new U_Ptr<T>(lp);
		}
	}

	CPtrHelper(const CPtrHelper<T>& lp)
	{
		m_Ptr = lp.m_Ptr;
		if (m_Ptr)
		{
			addRef();
		}
	}
	// Destructor
	virtual ~CPtrHelper()
	{
		if (m_Ptr != NULL)
		{
			release();
		}
	}
	void addRef()
	{
		m_Ptr->m_count++;
	}
	void release()
	{
		m_Ptr->m_count--;
		if (m_Ptr->m_count == 0)
		{
			delete m_Ptr;
		}
		m_Ptr = NULL;
	}
	// Conversion
public:
	operator T*()
	{
		if (m_Ptr)
		{
			return m_Ptr->m_Ptr;
		}
		return NULL;
	}
	//T* operator *() { return m_Ptr; }
public:
	// Pointer operations
	//T& operator*()  { return *m_Ptr ;}
	//T** operator&() { return &m_Ptr ;}
	T* operator->() const
	{
		return m_Ptr->m_Ptr;
	}

	// Assignment from the same interface
	T* operator=(const CPtrHelper& pI)
	{
		if (m_Ptr != pI.m_Ptr)
		{
			if (m_Ptr != NULL)
			{
				release()			// Release the old interface.
			}
			m_Ptr = pI.m_Ptr;
			if (m_Ptr != NULL)
			{
				addRef();
			}

		}
		return m_Ptr;
	}

	T* operator=(T* pI)
	{
		if (m_Ptr)
		{
			if (m_Ptr->m_Ptr != pI)
			{
				release();
				if (pI != NULL)
				{
					m_Ptr = new U_Ptr<T>(pI);
				}
			}
			else
			{
				addRef();
			}
		}
		else
		{
			if (pI != NULL)
			{
				m_Ptr = new U_Ptr<T>(pI);
			}
		}
		return pI;
	}

	// Boolean functions
	//bool operator!() { return (m_Ptr == nullptr) ? true : false ;}

	// Requires a compiler that supports bool
	// 	operator bool() const
	// 	{ 
	// 		return (m_Ptr != nullptr) ? true : false ; 
	// 	}

	// 	bool operator==(const CPtrHelper& pI)
	//  	{
	//  		return (m_Ptr == pI.m_Ptr) ? true : false;
	//  	}
	// 
	// 	bool operator==(const T* pI)
	// 	{
	// 		return (m_Ptr == pI) ? true : false;
	// 	}
	// 
	// 	bool operator!=(const CPtrHelper& pI)
	// 	{
	// 		return (m_Ptr == pI.m_Ptr) ? true : false;
	// 	}
	// 
	//  	bool operator!=(const T* pI) 
	// 	{
	// 		return (m_Ptr != nullptr) ? true : false; 
	// 	}
private:
	//T* operator==(T* pI)
	//{
	//	return (m_Ptr == pI) ? true : false;
	//}

public:
	/*template <class T1>
	CPtrHelper<T1> cast()
	{
	CPtrHelper<T1> ptr;
	if (m_Ptr)
	{
	ptr = dynamic_cast<T1*>(m_Ptr);
	}
	return ptr;
	}*/
};