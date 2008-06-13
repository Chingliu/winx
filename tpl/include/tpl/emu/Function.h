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
// Module: tpl/emu/Function.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Function.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_FUNCTION_H
#define TPL_EMU_FUNCTION_H

#ifndef TPL_BASIC_H
#include "../Basic.h"
#endif

#pragma warning(disable:4819)

#ifndef BOOST_TT_IS_CONVERTIBLE_HPP_INCLUDED
#include <boost/type_traits/is_convertible.hpp>
#endif

#ifndef NS_TPL_EMU_BEGIN
#define NS_TPL_EMU_BEGIN	namespace tpl { namespace emu {
#define NS_TPL_EMU_END		} }
#define NS_TPL_EMU			tpl::emu
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// class Caller

template <int nArity>
struct Caller {};

template <>
struct Caller<1> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0]);
	}
};

template <>
struct Caller<2> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1]);
	}
};

template <>
struct Caller<3> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2]);
	}
};

template <>
struct Caller<4> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2], args[3]);
	}
};

template <>
struct Caller<5> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2], args[3], args[4]);
	}
};

template <>
struct Caller<6> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2], args[3], args[4], args[5]);
	}
};

// =========================================================================
// class Function

template <class Op, int nArity>
class Function
{
private:
	const Op m_op;

public:
	Function() : m_op() {}

	template <class T1>
	Function(const T1& arg) : m_op(arg) {}

public:
	template <class StackT>
	void TPL_CALL operator()(StackT& stk) const {
		typedef typename StackT::value_type value_type;
		TPL_ASSERT(stk.size() >= nArity);
		value_type args[nArity];
		for (int i = nArity; i--;) {		
			args[i] = stk.back();
			stk.pop_back();
		}
		stk.push_back(Caller<nArity>::call(m_op, args));
	}
};

// class Unary-Function

template <class Op>
class Function<Op, 1>
{
private:
	const Op m_op;

public:
	Function() : m_op() {}

	template <class T1>
	Function(const T1& arg) : m_op(arg) {}

public:
	template <class StackT>
	void TPL_CALL operator()(StackT& stk) const {
		typedef typename StackT::value_type value_type;
		TPL_ASSERT(stk.size() >= 1);
		value_type x = stk.back();
		stk.pop_back();
		stk.push_back(m_op(x));
	}
};

// =========================================================================
// class ArityTratis

template <int isUnary, int isBinary, int isTernary = 0>
struct ArityTratis_ {};

template <>
struct ArityTratis_<1, 0, 0> { enum { value = 1 }; };

template <>
struct ArityTratis_<0, 1, 0> { enum { value = 2 }; };

template <>
struct ArityTratis_<0, 0, 1> { enum { value = 3 }; };

template <template <class Type> class Op_, class Ty_ = int>
class ArityTraits
{
private:
	typedef Ty_ Ty;
	typedef Op_<Ty_> Op;
	enum { is_unary = boost::is_convertible<Op, std::unary_function<Ty, Ty> >::value };
	enum { is_binary = boost::is_convertible<Op, std::binary_function<Ty, Ty, Ty> >::value };

public:
	enum { value = ArityTratis_<is_unary, is_binary>::value };
};

// =========================================================================
// struct OpTraits

template <int nArity, class Ty>
struct OpTraits {};

template <class Ty>
struct OpTraits<1, Ty> {
	typedef Ty (*op_type)(Ty);
};

template <class Ty>
struct OpTraits<2, Ty> {
	typedef Ty (*op_type)(Ty, Ty);
};

template <class Ty>
struct OpTraits<3, Ty> {
	typedef Ty (*op_type)(Ty, Ty, Ty);
};

template <class Ty>
struct OpTraits<4, Ty> {
	typedef Ty (*op_type)(Ty, Ty, Ty, Ty);
};

template <class Ty>
struct OpTraits<5, Ty> {
	typedef Ty (*op_type)(Ty, Ty, Ty, Ty, Ty);
};

template <class Ty>
struct OpTraits<6, Ty> {
	typedef Ty (*op_type)(Ty, Ty, Ty, Ty, Ty, Ty);
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_FUNCTION_H */

