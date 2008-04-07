/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/memory/Pool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-21 14:04:45
// 
// $Id: Pool.h,v 1.2 2006/12/14 09:15:33 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_POOL_H__
#define __STDEXT_MEMORY_POOL_H__

#if (0)
#define WINX_USE_DEFBOOST
#endif

// -------------------------------------------------------------------------

#if defined(WINX_USE_DEFBOOST)

#ifndef BOOST_POOL_HPP
#include <boost/pool/pool.hpp>
#endif

#ifndef BOOST_OBJECT_POOL_HPP
#include <boost/pool/object_pool.hpp>
#endif

#else

#ifndef BOOST_CONFIG_HPP
#include "../../../../boost/boost/config.hpp"
#endif

#ifndef BOOST_POOL_HPP
#include "../../../../boost/boost/pool/pool.hpp"
#endif

#ifndef BOOST_OBJECT_POOL_HPP
#include "../../../../boost/boost/pool/object_pool.hpp"
#endif

#endif

// -------------------------------------------------------------------------
// class TestPool

#if defined(STD_UNITTEST)

template <class LogT>
class TestPool : public TestCase
{
	WINX_TEST_SUITE(TestPool);
		WINX_TEST(testPool);
		WINX_TEST(testOjectPool);
	WINX_TEST_SUITE_END();

public:
	class TestClass
	{
	private:
		int* m_data;
	public:
		TestClass() : m_data(new int) {}
		~TestClass() {
			printf("~TestClass\n");
			delete m_data;
		}
	};

	void testPool(LogT& log)
	{
		boost::pool<> alloc(sizeof(int));
		void* p = alloc.malloc();
	}

	void testOjectPool(LogT& log)
	{
		boost::object_pool<TestClass> alloc;
		TestClass* p = alloc.construct();
		TestClass* p2 = alloc.construct();
		alloc.destroy(p);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Pool.h,v $

#endif /* __STDEXT_MEMORY_POOL_H__ */
