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
// Module: stdext/archive/ReadArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: ReadArchive.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_READARCHIVE_H__
#define __STDEXT_ARCHIVE_READARCHIVE_H__

#ifndef __STDEXT_ARCHIVE_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class ReadArchive

template <class Handle, class StreamHandle, class CacheT = ArchiveCache>
class ReadArchive
{
public:
	enum { endch = StreamHandle::endch };

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

   	typedef typename StreamHandle::int_type	int_type;
	typedef typename StreamHandle::char_type char_type;
	typedef typename StreamHandle::uchar_type uchar_type;

	typedef typename StreamHandle::pos_type	pos_type;
	typedef typename StreamHandle::off_type	off_type;
	
protected:
	char_type*	m_lpBufStart;
	size_type	m_nBufSize;
	char_type*	m_lpBufCur;
	char_type*	m_lpBufMax;

	StreamHandle m_handle;
	CacheT m_alloc;

private:
	ReadArchive(const ReadArchive&);
	void operator=(const ReadArchive&);
	
public:
	template <class AllocT>
	explicit ReadArchive(AllocT& alloc)
		: m_alloc(alloc)
	{
		m_lpBufStart = (char_type*)m_alloc.item_alloc();
		m_nBufSize	 = m_alloc.alloc_size() / sizeof(char_type);
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		// ��״̬��m_lpBufMax - m_lpBufCur Ϊ�Ѿ�����Ԥ�������ݣ�
	}

	template <class AllocT>
	ReadArchive(AllocT& alloc, Handle hFile)
		: m_alloc(alloc)
	{
		m_lpBufStart = (char_type*)m_alloc.item_alloc();
		m_nBufSize	 = m_alloc.alloc_size() / sizeof(char_type);
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.open_handle(hFile);
	}

	template <class AllocT>
	ReadArchive(AllocT& alloc, LPCWSTR szFile)
		: m_alloc(alloc)
	{
		m_lpBufStart = (char_type*)m_alloc.item_alloc();
		m_nBufSize	 = m_alloc.alloc_size() / sizeof(char_type);
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.open_to_read(szFile);
	}

	template <class AllocT>
	ReadArchive(AllocT& alloc, LPCSTR szFile)
		: m_alloc(alloc)
	{
		m_lpBufStart = (char_type*)m_alloc.item_alloc();
		m_nBufSize	 = m_alloc.alloc_size() / sizeof(char_type);
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.open_to_read(szFile);
	}

	template <class AllocT>
	ReadArchive(AllocT& alloc, const ReadArchive& rhs)
		: m_alloc(alloc), m_handle(rhs.m_handle, /* fClone=*/ true)
	{
		m_lpBufStart = (char_type*)m_alloc.item_alloc();
		m_nBufSize	 = m_alloc.alloc_size() / sizeof(char_type);
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
	}

public:
	void winx_call clear_cache()
	{
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	pos_type winx_call tell() const
	{
		return m_handle.tell() - (INT32)(m_lpBufMax - m_lpBufCur);
	}

	void winx_call seek(pos_type offset)
	{
		// ��״̬��m_lpBufMax - m_lpBufStart����m_nBufSize��������Ч���ݷ�Χ��
		// ??			--- m_lpBufStart
		// position		--- m_lpBufMax
		// offset		--- m_lpBufCur
		pos_type position = m_handle.tell();
		pos_type delta = position - offset;
		if (delta <= (pos_type)(INT)(m_lpBufMax - m_lpBufStart))
		{
			m_lpBufCur = m_lpBufMax - toUInt(delta);
		}
		else
		{
			m_handle.seek(static_cast<off_type>(offset), SEEK_SET);
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
		}
	}

	void winx_call seek_end()
	{
		m_handle.seek(0, SEEK_END);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

public:
	size_type winx_call skip(size_type nMax)
	{
		if (nMax + m_lpBufCur <= m_lpBufMax)
		{
			// 1���ڻ��������㹻ʱ���������ݲ�����
			m_lpBufCur  += nMax;
		}
		else
		{
			// 2���ڻ������ݲ���ʱ
			// ??			--- m_lpBufStart
			// position		--- m_lpBufMax		= �ļ�ָ���Ӧ��
			// offset		--- m_lpBufCur
			m_handle.seek(nMax - (m_lpBufMax - m_lpBufCur), SEEK_CUR);
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
		}
		return nMax;
	}

	size_type winx_call get(char_type* lpBuf, size_type nMax)
	{
		WINX_ASSERT(lpBuf && m_lpBufStart && m_lpBufCur);
		
		// 1���ڻ��������㹻ʱ���������ݲ�����
		if (nMax + m_lpBufCur <= m_lpBufMax)
		{
			copyMemory(lpBuf, m_lpBufCur, nMax);
			m_lpBufCur += nMax;
			return nMax;
		}
		
		// 2���ڻ������ݲ���ʱ
		
		// nRead	--- ��������ֽ���
		// nMax		--- ʣ���ֽ���
		// nBlkRead --- �ļ������ķ���ֵ
		size_type nBlkRead, nRead;
		
		// a���ȶ��뻺���е�����
		nRead = (size_type)(m_lpBufMax - m_lpBufCur);
		copyMemory(lpBuf, m_lpBufCur, nRead);
		nMax -= nRead;
		m_lpBufCur = m_lpBufMax;
		
		// b����m_nBufSize�����������ݶ���
		size_type nTemp = nMax - nMax % m_nBufSize;
		if (nTemp)
		{
			nBlkRead = m_handle.get(lpBuf + nRead, nTemp);
			nRead += nBlkRead;
			if (nBlkRead < nTemp)
			{
				// �����Ѿ������ļ�ĩ�ˣ�
				return nRead;
			}
			nMax -= nTemp;
		}
		WINX_ASSERT(nMax < m_nBufSize);
		
		// c��ʣ����ȶ��뵽�������У���д��Ŀ���ڴ�
		nBlkRead	= m_handle.get(m_lpBufStart, m_nBufSize);
		m_lpBufMax	= (m_lpBufCur = m_lpBufStart) + nBlkRead;
		if (nBlkRead < nMax)
			nMax = nBlkRead;
		copyMemory(lpBuf + nRead, m_lpBufCur, nMax);
		m_lpBufCur += nMax;
		nRead += nMax;
		
		return nRead;
	}

	int_type winx_call get()
	{
		if (m_lpBufCur < m_lpBufMax)
			return (uchar_type)*m_lpBufCur++;

		char_type ch;
		return get(&ch, 1) ? (uchar_type)ch : (int_type)endch;
	}

	void winx_call reget(size_type offset, char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);
		
		if (offset <= (size_type)(UINT)(m_lpBufCur - m_lpBufStart))
		{
			copyMemory(lpBuf, m_lpBufCur - offset, cch);
		}
		else
		{
			pos_type curpos = tell();
			seek(curpos - offset);
			get(lpBuf, cch);
			seek(curpos);
		}
	}

	int winx_call unget()
	{
		if (m_lpBufCur <= m_lpBufStart)
		{
			pos_type delta = m_nBufSize >> 2;
			pos_type current = tell();
			if (current < delta)
				delta = current;
			m_handle.seek(current-delta, SEEK_SET);
			m_lpBufMax = m_lpBufStart + m_handle.get(m_lpBufStart, m_nBufSize);
			m_lpBufCur = m_lpBufStart + toUInt(delta);

			if (m_lpBufCur <= m_lpBufStart)
				return 0;
		}
		--m_lpBufCur;
		return 1;
	}

	void winx_call unget(int_type ch)
	{
		if (ch != endch)
			unget();
	}

	int_type winx_call peek()
	{
		if (m_lpBufCur < m_lpBufMax)
			return (uchar_type)*m_lpBufCur;

		char_type ch;
		if (!get(&ch, 1))
			return endch;

		--m_lpBufCur;
		return (uchar_type)ch;
	}

public:
	HRESULT winx_call open(LPCWSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_read(szFile);
	}

	HRESULT winx_call open(LPCSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_read(szFile);
	}

	void winx_call open_handle(Handle hFile)
	{
		if (!m_handle.good())
			m_handle.open_handle(hFile);
	}

	void winx_call close()
	{
		clear_cache();
		m_handle.close();
	}

	void winx_call attach(Handle hFile)
	{
		clear_cache();
		m_handle.attach(hFile);
	}

	Handle winx_call detach()
	{
		clear_cache();
		return m_handle.detach();
	}

public:
	pos_type winx_call size() const
	{
		return m_handle.size();
	}

	int winx_call operator!() const
	{
		return m_handle.bad();
	}

	int winx_call good() const
	{
		return m_handle.good();
	}

	int winx_call bad() const
	{
		return m_handle.bad();
	}
};

// -------------------------------------------------------------------------
// $Log: ReadArchive.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_READARCHIVE_H__ */
