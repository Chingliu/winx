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
// Module: tpl/regex/Composition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_COMPOSITION_H
#define TPL_REGEX_COMPOSITION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_BASIC_H
#include "Terminal.h"
#endif

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// operator>> (boost::spirit, xpressive compatible)
// operator+

// Usage: Rule1 >> Rule2
// Usage: Rule1 + Rule2

template <class RegExT1, class RegExT2>
class And // Rule1 Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	And(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		typename ContextT::trans_type trans(context);
		if (m_x.match(ar, context)) {
			if (m_y.match(ar, context))
				return true;
		}
		trans.rollback();
		return false;
	}
};

template <class T1, class T2> __forceinline
Exp<And<T1, T2> > TPL_CALL operator>>(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<And<T1, T2> >(x, y);
}

template <class T1, class T2> __forceinline
Exp<And<T1, T2> > TPL_CALL operator+(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<And<T1, T2> >(x, y);
}

TPL_REGEX_CH_OP_(>>, And)
TPL_REGEX_CH_OP_(+, And)

// -------------------------------------------------------------------------
// operator*

// Usage: Rule1 * Rule2		--- means: Rule1 Rule2 (but no transaction)

template <class RegExT1, class RegExT2>
class UAnd // Rule1 * Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	UAnd(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		return m_x.match(ar, context) && m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Exp<UAnd<T1, T2> > TPL_CALL operator*(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<UAnd<T1, T2> >(x, y);
}

// -------------------------------------------------------------------------
// operator|

// Usage: Rule1 | Rule2

template <class RegExT1, class RegExT2>
class Or // Rule1 | Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	Or(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		if (m_x.match(ar, context))
			return true;
		return m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Exp<Or<T1, T2> > TPL_CALL operator|(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<Or<T1, T2> >(x, y);
}

TPL_REGEX_CH_OP_(|, Or)

// -------------------------------------------------------------------------
// operator* (Unary)

// Usage: *Rule

template <class RegExT>
class Repeat0 // Rule*
{
private:
	RegExT m_x;

public:
	Repeat0(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		while (m_x.match(ar, context))
			;
		return true;
	}
};

template <class T1> __forceinline
Exp<Repeat0<T1> > TPL_CALL operator*(const Exp<T1>& x) {
	return Exp<Repeat0<T1> >(x);
}

// -------------------------------------------------------------------------
// operator+ (Unary)

// Usage: +Rule

template <class RegExT>
class Repeat1 // Rule+
{
private:
	RegExT m_x;

public:
	Repeat1(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		unsigned n = 0;
		while (m_x.match(ar, context))
			++n;
		return n > 0;
	}
};

template <class T1> __forceinline
Exp<Repeat1<T1> > TPL_CALL operator+(const Exp<T1>& x) {
	return Exp<Repeat1<T1> >(x);
}

// -------------------------------------------------------------------------
// operator!

// Usage: !Rule
// Note: Sorry that we don't have operator?

template <class RegExT>
class Repeat01 // Rule?
{
private:
	RegExT m_x;

public:
	Repeat01(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		m_x.match(ar, context);
		return true;
	}
};

template <class T1> __forceinline
Exp<Repeat01<T1> > TPL_CALL operator!(const Exp<T1>& x) {
	return Exp<Repeat01<T1> >(x);
}

// -------------------------------------------------------------------------
// function repeat

// Usage:
//	1. repeat<n>(Rule)		--- means: Rule{n}
//	2. repeat<n, m>(Rule)	--- means: Rule{n, m}
//	3. repeat<0, 1>(Rule)	--- means: Rule{0, 1}, that is: Rule?
//	4. repeat_ge<n>(Rule)	--- means: Rule{n,}

template <class RegExT, unsigned nMin, unsigned nMax = UINT_MAX>
class Repeat // Rule{nMin, nMax}
{
private:
	RegExT m_x;

public:
	Repeat(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		unsigned n;
		typename SourceT::iterator pos = ar.position();
		typename ContextT::trans_type trans(context);
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context))
				break;
		}
		if (n >= nMin)
			return true;
		ar.seek(pos);
		trans.rollback();
		return false;
	}
};

template <class RegExT>
class Repeat<RegExT, 0, 1> : public Repeat01<RegExT>
{
public:
	Repeat<RegExT, 0, 1>(const RegExT& x) : Repeat01<RegExT>(x) {}
};

template <unsigned nMin, class T1> __forceinline
Exp<Repeat<T1, nMin> > TPL_CALL repeat_ge(const Exp<T1>& x) {
	return Exp<Repeat<T1, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Exp<Repeat<T1, nMin, nMin> > TPL_CALL repeat(const Exp<T1>& x) {
	return Exp<Repeat<T1, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Exp<Repeat<T1, nMin, nMax> > TPL_CALL repeat(const Exp<T1>& x) {
	return Exp<Repeat<T1, nMin, nMax> >(x);
}

// -------------------------------------------------------------------------
// operator%

// Usage: Rule1 % Rule2		--- means: Rule1 (Rule2 Rule1)*
// Example: Arg % ','		--- means: matching Function ArgList

template <class T1, class T2> __forceinline
Exp<And<T1, Repeat0<And<T2, T1> > > > TPL_CALL operator%(const Exp<T1>& x, const Exp<T2>& y)
{
	return x + *(y + x);
}

template <class T1> __forceinline
Exp<And<T1, Repeat0<And<MatchCh, T1> > > > TPL_CALL operator%(const Exp<T1>& x, int c)
{
	Exp<MatchCh> y = c;
	return x + *(y + x);
}

template <class T2> __forceinline
Exp<And<MatchCh, Repeat0<And<T2, MatchCh> > > > TPL_CALL operator%(int c, const Exp<T2>& y)
{
	Exp<MatchCh> x = c;
	return x + *(y + x);
}

// -------------------------------------------------------------------------
// function csymbol, integer, etc.

// Usage: csymbol()			--- means: matching a CSymbol. that is: [a-zA-Z_][0-9a-zA-Z_]*
// Usage: integer()			--- means: matching a CSymbol. that is: [0-9]+

inline Exp<Repeat0<Blank> > skipws()
{
	Exp<Blank> b;
	return *b;
}

inline Exp<UAnd<CSymbolFirstChar, Repeat0<CSymbolNextChar> > > TPL_CALL csymbol()
{
	Exp<CSymbolFirstChar> f;
	Exp<CSymbolNextChar> n;
	return f * *n;
}

inline Exp<Repeat1<Digit> > TPL_CALL integer()
{
	Exp<Digit> d;
	return +d;
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_COMPOSITION_H */