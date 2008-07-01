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
// Module: tpl/regex/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_BASIC_H
#define TPL_REGEX_BASIC_H

#ifndef TPL_BASIC_H
#include "../Basic.h"
#endif

NS_TPL_BEGIN

template <class RegExT> class Rule;
template <class GrammarT> class Grammar;

// =========================================================================
// class Gr: Wrapper a Rule to Grammar

template <class RegExT>
class Gr : public RegExT
{
public:
	Gr() : RegExT() {}
	Gr(const RegExT& x) : RegExT(x) {}

public:
	const Rule<RegExT>& TPL_CALL rule() const {
		return *(const Rule<RegExT>*)this;
	}

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const {
		skipper_.match(ar, context);
		return RegExT::match(ar, context);
	}
};

// =========================================================================
// struct BindTraits

template <class T1, class T2>
struct BindTraits
{
//	concept:
//
//	typename bind_type;
//
//	static bind_type TPL_CALL bind(const T1& o1, const T2& o2);
};

// =========================================================================
// class Rule

// enum RuleCharacter

enum RuleCharacter
{
	CHARACTER_NONE		= 0x00,
	CHARACTER_MARKED	= 0x01,
	CHARACTER_SKIPPER	= CHARACTER_NONE,
};

// RuleAssigTag

struct TagAssigNone {};
struct TagAssigLst {};
struct TagAssigChar {};
struct TagAssigUInteger {};		// dec radix
struct TagAssigInteger {};		// dec radix
struct TagAssigHexInteger {};	// hex radix
struct TagAssigOctInteger {};	// oct radix
struct TagAssigBinInteger {};	// binary radix
struct TagAssigUReal {};		// unsigned real
struct TagAssigReal {};		// real
struct TagAssigUFraction {};
struct TagAssigFraction {};

template <class Tag1, class Tag2>
struct AndAssigTag {
	typedef TagAssigNone assig_tag;
};

template <class AssigTag>
struct AndAssigTag<AssigTag, AssigTag> {
	typedef AssigTag assig_tag;
};

// ConvertableType

struct SelfConvertible {}; // The Rule itself is a Grammar.
struct AutoConvertible {}; // Convert a Rule to a Grammar automatically.
struct ExplicitConvertible {}; // Should convert a Rule to a Grammar manually.

template <class CT1, class CT2>
struct OrConvertable {
	typedef ExplicitConvertible convertible_type;
};

template <class CT>
struct OrConvertable<AutoConvertible, CT> {
	typedef AutoConvertible convertible_type;
};

template <>
struct OrConvertable<ExplicitConvertible, AutoConvertible> {
	typedef AutoConvertible convertible_type;
};

template <class CT1, class CT2>
struct AndConvertable {
	typedef ExplicitConvertible convertible_type;
};

template <class CT>
struct AndConvertable<CT, CT> {
	typedef CT convertible_type;
};

// class RuleBase

class RuleBase
{
public:
	enum { character = CHARACTER_MARKED };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;
};

// class Rule

template <class RegExT>
class Rule : public RegExT
{
public:
	Rule() : RegExT() {}

	template <class T1>
	Rule(T1& x) : RegExT(x) {}

	template <class T1>
	Rule(const T1& x) : RegExT(x) {}

	template <class T1, class T2>
	Rule(const T1& x, const T2& y) : RegExT(x, y) {}

	template <class T1, class T2>
	Rule(const T1& x, T2& y) : RegExT(x, y) {}

	template <class T1, class T2>
	Rule(T1& x, const T2& y) : RegExT(x, y) {}

	template <class T1, class T2, class T3>
	Rule(const T1& x, const T2& y, const T3& z) : RegExT(x, y, z) {}

	template <class T1, class T2, class T3>
	Rule(const T1& x, const T2& y, T3& z) : RegExT(x, y, z) {}

private:
	const Grammar<Gr<RegExT> >& TPL_CALL cast_grammar_(AutoConvertible) const {
		return *(const Grammar<Gr<RegExT> >*)this;
	}

	const Grammar<RegExT>& TPL_CALL cast_grammar_(SelfConvertible) const {
		return *(const Grammar<RegExT>*)this;
	}

public:
	Grammar<Gr<RegExT> > TPL_CALL cast_grammar() const {
		return cast_grammar_(typename RegExT::convertible_type());
	}

	Grammar<Gr<RegExT> > TPL_CALL grammar() const {
		return *(const Grammar<Gr<RegExT> >*)this;
	}

	template <class T2>
	typename BindTraits<Rule<RegExT>, T2>::bind_type TPL_CALL operator[](const T2& obj) const {
		return BindTraits<Rule<RegExT>, T2>::bind(*this, obj);
	}
	
//	concept:
//
//	enum { character = RegExT::character };
//
//	typedef typename RegExT::convertible_type convertible_type;
//	typedef typename RegExT::assig_tag assig_tag;
//
//	template <class SourceT, class ContextT>
//	bool TPL_CALL match(SourceT& ar, ContextT& context) const;
};

#define TPL_GRAMMAR_RULE_BINARY_OP_(op, Op)									\
template <class T1, class T2>												\
__forceinline Grammar<Op<Gr<T1>, T2> > TPL_CALL operator op(const Rule<T1>& x, const Grammar<T2>& y) \
	{ return x.cast_grammar() op y; }										\
template <class T1, class T2>												\
__forceinline Grammar<Op<T1, Gr<T2> > > TPL_CALL operator op(const Grammar<T1>& x, const Rule<T2>& y) \
	{ return x op y.cast_grammar(); }

#define TPL_SIMPLEST_GRAMMAR_()												\
	template <class SourceT, class ContextT, class SkipperT>				\
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const { \
		return match(ar, context);											\
	}

// =========================================================================
// class Grammar

template <class GrammarT>
class Grammar : public GrammarT
{
public:
	Grammar() : GrammarT() {}

	template <class T1>
	Grammar(T1& x) : GrammarT(x) {}

	template <class T1>
	Grammar(const T1& x) : GrammarT(x) {}

	template <class T1, class T2>
	Grammar(const T1& x, const T2& y) : GrammarT(x, y) {}

	template <class T1, class T2>
	Grammar(T1& x, const T2& y) : GrammarT(x, y) {}

	template <class T1, class T2, class T3>
	Grammar(const T1& x, const T2& y, const T3& z) : GrammarT(x, y, z) {}

//	concept:
//
//	enum { character = GrammarT::character };
//
//	typedef typename GrammarT::assig_tag assig_tag;
//
//	template <class SourceT, class ContextT, class SkipperT>
//	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const;
};

template <class RegExT>
__forceinline const Grammar<Gr<RegExT> >& TPL_CALL gr(const Rule<RegExT>& rule_) {
	return *(const Grammar<Gr<RegExT> >*)(&rule_);
}

// =========================================================================
// class SimpleAction

template <class ActionT>
class SimpleAction : public ActionT
{
public:
	SimpleAction() : ActionT() {}

	template <class T1>
	SimpleAction(const T1& x) : ActionT(x) {}

	template <class T1>
	SimpleAction(T1& x) : ActionT(x) {}

	template <class T1, class T2>
	SimpleAction(const T1& x, const T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	SimpleAction(T1& x, const T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	SimpleAction(const T1& x, T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	SimpleAction(T1& x, T2& y) : ActionT(x, y) {}

	template <class T1, class T2, class T3>
	SimpleAction(T1& x, const T2& y, const T3& z) : ActionT(x, y, z) {}

//	concept:
//
//	void TPL_CALL operator()() const;
};

// =========================================================================
// class Action

template <class ActionT>
class Action : public ActionT
{
public:
	Action() : ActionT() {}

	template <class T1>
	Action(const T1& x) : ActionT(x) {}

	template <class T1>
	Action(T1& x) : ActionT(x) {}

	template <class T1, class T2>
	Action(const T1& x, const T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	Action(T1& x, const T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	Action(const T1& x, T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	Action(T1& x, T2& y) : ActionT(x, y) {}

//	concept:
//
//	typedef typename ActionT::value_type value_type;
//
//	void TPL_CALL operator()(const value_type& val) const;
};

// =========================================================================
// class Condition

enum MatchCode
{
	matchFailed = 0,
	matchOk = 1,
	matchStop = 2,
};

template <class ConditionT>
class Condition : public ConditionT
{
public:
	Condition() {}

	template <class T1>
	Condition(T1& x) : ConditionT(x) {}

	template <class T1>
	Condition(const T1& x) : ConditionT(x) {}

	template <class T1, class T2>
	Condition(const T1& x, const T2& y) : ConditionT(x, y) {}

//	concept:
//
//	enum { character = ConditionT::character };
//
//	typedef typename ContitionT::value_type value_type;
//
//	template <class ValueT, class SourceT, class ContextT>
//	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const;
};

// =========================================================================
// class GCondition

template <class ConditionT>
class GCondition : public ConditionT
{
public:
	GCondition() {}

	template <class T1>
	GCondition(T1& x) : ConditionT(x) {}

	template <class T1>
	GCondition(const T1& x) : ConditionT(x) {}

	template <class T1, class T2>
	GCondition(const T1& x, const T2& y) : ConditionT(x, y) {}

//	concept:
//
//	enum { character = ConditionT::character };
//
//	typedef typename ContitionT::value_type value_type;
//
//	template <class ValueT, class SourceT, class ContextT, class SkipperT>
//	MatchCode TPL_CALL match_if(
//		const ValueT& val, SourceT& ar, ContextT& context, const SkipperT& skipper_) const;
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_BASIC_H */

