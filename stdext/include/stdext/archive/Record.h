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
// Module: stdext/archive/Record.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 0:19:33
// 
// $Id: Record.h,v 1.2 2006/11/30 03:19:24 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_RECORD_H__
#define __STDEXT_ARCHIVE_RECORD_H__

__NS_STD_BEGIN

#pragma pack(1)

// =========================================================================
// class MemRecordArchive

struct RecordHeaderType
{
	enum { recIdEOF = 0 };

	UINT32 recId;
	UINT32 cbData;
};

template <class BaseWriter, class HeaderT = RecordHeaderType>
class RecordWriter : public BaseWriter
{
private:
	typedef BaseWriter _Base;

public:
	typedef HeaderT header_type;
	typedef typename _Base::pos_type pos_type;

	enum { recIdEOF = HeaderT::recIdEOF };

private:
	UINT m_recId;
	pos_type m_offset;
	
public:
	template <class AllocT>
	explicit RecordWriter(AllocT& alloc)
		: _Base(alloc)
	{
		m_recId = recIdEOF;
	}

	template <class AllocT, class InitArgT>
	RecordWriter(AllocT& alloc, InitArgT file)
		: _Base(alloc, file)
	{
		m_recId = recIdEOF;
	}

	HRESULT winx_call beginRecord(UINT recId)
	{
		WINX_ASSERT(m_recId == recIdEOF);

		header_type hdr = { recIdEOF, 0 };
		_WinxByteSwapStruct(hdr);
		_Base::put((const char*)&hdr, sizeof(header_type));
		m_recId = recId;
		m_offset = _Base::tell();
		return S_OK;
	}
	
	HRESULT winx_call endRecord()
	{
		const UINT32 cbData = _Base::tell() - m_offset;
		header_type hdr = { m_recId, cbData };
		_WinxByteSwapStruct(hdr);
		_Base::reput(
			cbData + sizeof(header_type), (const char*)&hdr, sizeof(header_type));
		m_recId = recIdEOF;
		return S_OK;
	}
};

// =========================================================================
// class RecordInfo

template <class HeaderT>
struct RecordInfo : public HeaderT
{
	const char* rgData;

public:
	using HeaderT::cbData;

public:
	typedef char value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef const char* pointer;
	typedef const char* const_pointer;
	typedef const char* iterator;
	typedef const char* const_iterator;

	const_iterator winx_call begin() const {
		return rgData;
	}

	const_iterator winx_call end() const {
		return rgData + cbData;
	}

	size_type winx_call size() const {
		return cbSize;
	}
};

// -------------------------------------------------------------------------
// class RecordReader

template <class ReadArchiveT, class HeaderT = RecordHeaderType>
class RecordReader : private ReadArchiveT
{
private:
	typedef ReadArchiveT _Base, BaseClass;

public:
	typedef HeaderT header_type;
	typedef RecordInfo<HeaderT> record_info;
	typedef typename ReadArchiveT::char_type char_type;

	enum { recIdEOF = HeaderT::recIdEOF };

public:
	template <class AllocT>
	explicit RecordReader(AllocT& alloc)
		: _Base(alloc) {}

	template <class AllocT, class InitArgT>
	RecordReader(AllocT& alloc, InitArgT file)
		: _Base(alloc, file) {}

	template <class AllocT, class InitArgT1, class InitArgT2>
	RecordReader(AllocT& alloc, InitArgT1 arg1, InitArgT2 arg2)
		: _Base(alloc, arg1, arg2) {}

public:
	const char_type* winx_call next_record(header_type& hdr)
	{
		const HeaderT* hdr0 = (const HeaderT*)_Base::get(sizeof(HeaderT));
		if (hdr0 == NULL)
		{
			hdr.recId = recIdEOF;
			hdr.cbData = 0;
			return NULL;
		}

		hdr = *hdr0;
		_WinxByteSwapStruct(hdr);
		return _Base::get(hdr.cbData);
	}

	const char_type* winx_call next(record_info& info)
	{
		info.rgData = next_record(info);
		return info.rgData;
	}
};

// =========================================================================
// $Log: Record.h,v $

#pragma pack()

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_RECORD_H__ */
