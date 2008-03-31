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
// Module: stdext/registry/WinRegistry.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-7 13:35:33
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_REGISTRY_WINREGISTRY_H__
#define __STDEXT_REGISTRY_WINREGISTRY_H__

#if defined(STD_NO_WINSDK)
#error "Require windows sdk!!!"
#endif

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#ifndef __STDEXT_TEXT_BASICSTRING_H__
#include "../text/BasicString.h"
#endif

#ifndef __STDEXT_MSVCRT_SPECSTRINGS_H__
#include "../msvcrt/specstrings.h"
#endif

#ifndef REG_QWORD
#define REG_QWORD                   ( 11 )  // 64-bit number
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class WinRegKeyHandle

class WinRegKeyHandle
{
private:
	HKEY m_hKey;

public:
	WinRegKeyHandle() { m_hKey = NULL; }
	WinRegKeyHandle(HKEY hKey) : m_hKey(hKey) {}

	operator HKEY() const { return m_hKey; }

	LONG winx_call close()
	{
		if (m_hKey != NULL)
		{
			LONG lRes = ::RegCloseKey(m_hKey);
			m_hKey = NULL;
			return lRes;
		}
		return ERROR_SUCCESS;
	}

	void winx_call flush()
	{
		WINX_ASSERT(m_hKey != NULL);
		::RegFlushKey(m_hKey);
	}

	int winx_call good() const
	{
		return m_hKey != NULL;
	}

	LONG winx_call create(
		__in HKEY hKeyParent,
		__in_z LPCTSTR lpszKeyName,
		__in_z LPTSTR lpszClass = REG_NONE,
		__in DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		__in REGSAM samDesired = KEY_READ | KEY_WRITE,
		__in_opt LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		__in_opt LPDWORD lpdwDisposition = NULL)
	{
		WINX_ASSERT(hKeyParent != NULL);
		DWORD dw;
		HKEY hKey = NULL;
		LONG lRes = ::RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
			lpszClass, dwOptions, samDesired, NULL, &hKey, &dw);
		if (lpdwDisposition != NULL)
			*lpdwDisposition = dw;
		if (lRes == ERROR_SUCCESS)
		{
			lRes = close();
			m_hKey = hKey;
		}
		return (lRes);
	}

	LONG winx_call open(
		__in HKEY hKeyParent,
		__in_z LPCTSTR lpszKeyName,
		__in REGSAM samDesired = KEY_ALL_ACCESS)
	{
		WINX_ASSERT(hKeyParent != NULL);
		HKEY hKey = NULL;
		LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
		if (lRes == ERROR_SUCCESS)
		{
			lRes = close();
			WINX_ASSERT(lRes == ERROR_SUCCESS);
			m_hKey = hKey;
		}
		return (lRes);
	}

	LONG winx_call enumChild(
		__in DWORD iIndex,
		__out_ecount_part_z(*pnNameLength, *pnNameLength) LPTSTR pszName,
		__inout LPDWORD pnNameLength,
		__out_opt FILETIME* pftLastWriteTime = NULL)
	{
		FILETIME ftLastWriteTime;

		WINX_ASSERT(m_hKey != NULL);
		if (pftLastWriteTime == NULL)
			pftLastWriteTime = &ftLastWriteTime;

		return (
			::RegEnumKeyEx(m_hKey, iIndex, pszName, pnNameLength, NULL, NULL, NULL, pftLastWriteTime));
	}

	LONG winx_call notifyChangeValue(
		__in BOOL bWatchSubtree,
		__in DWORD dwNotifyFilter,
		__in_opt HANDLE hEvent,
		__in BOOL bAsync = TRUE)
	{
		WINX_ASSERT(m_hKey != NULL);
		WINX_ASSERT((hEvent != NULL) || !bAsync);

		return (
			::RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent, bAsync));
	}

public:
	LONG winx_call eraseChild(
		__in_z LPCTSTR lpszSubKey)
	{
		WINX_ASSERT(m_hKey != NULL);
		return (::RegDeleteKey(m_hKey, lpszSubKey));
	}

	LONG winx_call erase(
		__in_z LPCTSTR lpszValue)
	{
		WINX_ASSERT(m_hKey != NULL);
		return (::RegDeleteValue(m_hKey, (LPTSTR)lpszValue));
	}

public:
	LONG winx_call putString(
		__in_z LPCTSTR pszValueName,
		__in_z LPCTSTR pszValue,
		__in DWORD dwType = REG_SZ)
	{
		WINX_ASSERT(m_hKey != NULL);
		WINX_ASSERT(pszValue != NULL);
		WINX_ASSERT((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ));

		return ::RegSetValueEx(
			m_hKey, pszValueName, NULL, dwType,
			reinterpret_cast<const BYTE*>(pszValue),
			(std::length(pszValue)+1) * sizeof(TCHAR));
	}

	LONG winx_call putString(
		__in_z LPCTSTR pszValueName,
		__in const basic_string<TCHAR>& strValue,
		__in DWORD dwType = REG_SZ)
	{
		WINX_ASSERT(m_hKey != NULL);
		WINX_ASSERT((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ));

		return ::RegSetValueEx(
			m_hKey, pszValueName, NULL, dwType,
			reinterpret_cast<const BYTE*>(strValue.c_str()),
			(strValue.size()+1) * sizeof(TCHAR));
	}

	LONG winx_call putString(
		__in_z LPCTSTR pszValueName,
		__in const TempString<TCHAR> strValue,
		__in DWORD dwType = REG_SZ)
	{
		WINX_ASSERT(m_hKey != NULL);
		WINX_ASSERT((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ));

		std::vector<TCHAR> s;
		s.reserve(strValue.size() + 1);
		s.insert(s.end(), strValue.begin(), strValue.end());
		s.push_back(0);
		return ::RegSetValueEx(
			m_hKey, pszValueName, NULL, dwType,
			reinterpret_cast<const BYTE*>(_ConvIt(s.begin())),
			s.size() * sizeof(TCHAR));
	}

	LONG winx_call putBinary(LPCTSTR pszValueName, const void* pData, ULONG nBytes)
	{
		WINX_ASSERT(m_hKey != NULL);
		return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_BINARY, reinterpret_cast<const BYTE*>(pData), nBytes);
	}

	LONG winx_call putDWord(LPCTSTR pszValueName, DWORD dwValue)
	{
		WINX_ASSERT(m_hKey != NULL);
		return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_DWORD, reinterpret_cast<const BYTE*>(&dwValue), sizeof(DWORD));
	}

	LONG winx_call putInt(LPCTSTR pszValueName, int nValue)
	{
		return putDWord(pszValueName, (DWORD)nValue);
	}

	LONG winx_call putQWord(LPCTSTR pszValueName, ULONGLONG qwValue)
	{
		WINX_ASSERT(m_hKey != NULL);
		return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_QWORD, reinterpret_cast<const BYTE*>(&qwValue), sizeof(ULONGLONG));
	}

	LONG winx_call putGuid(
		__in_z LPCTSTR pszValueName,
		__in REFGUID guidValue)
	{
		WINX_ASSERT(m_hKey != NULL);

		OLECHAR szGUID[64];
		::StringFromGUID2(guidValue, szGUID, 64);

		WINX_USES_CONVERSION;
		LPCTSTR lpstr = WINX_W2CT(szGUID);

#ifndef _UNICODE
		if (lpstr == NULL)
			return E_OUTOFMEMORY;
#endif	
		return putString(pszValueName, lpstr);
	}

public:
	template <class StringT>
	LONG winx_call getString(
		__in_z_opt LPCTSTR pszValueName,
		__out StringT& strValue)
	{
		DWORD dwType;
		ULONG nBytes = 0;
		LONG lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, NULL, &nBytes);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		if(dwType != REG_SZ && dwType != REG_EXPAND_SZ || nBytes < 2)
			return ERROR_INVALID_DATA;
		
		TCHAR* pszData = std::resize(strValue, nBytes/sizeof(TCHAR));
		lRes = ::RegQueryValueEx(
			m_hKey, pszValueName, NULL, &dwType, (BYTE*)pszData, &nBytes);
		strValue.erase(strValue.end() - 1);
		return lRes;
	}

	template <class AllocT>
	LONG winx_call getString(
		__in_z_opt LPCTSTR pszValueName,
		__in AllocT& alloc,
		__out BasicString<TCHAR>& strValue)
	{
		OutputBasicString<TCHAR, AllocT> s1(alloc, strValue);
		return getString(pszValueName, s1);
	}

	LONG winx_call getString(
		__in_z_opt LPCTSTR pszValueName,
		__out_ecount_part_z_opt(*pnChars, *pnChars) LPTSTR pszValue,
		__inout ULONG* pnChars)
	{
		LONG lRes;
		DWORD dwType;
		ULONG nBytes;

		WINX_ASSERT(m_hKey != NULL);
		WINX_ASSERT(pnChars != NULL);

		nBytes = (*pnChars)*sizeof(TCHAR);
		*pnChars = 0;
		lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue), &nBytes);
		
		if (lRes != ERROR_SUCCESS)
		{
			return (lRes);
		}
		if(dwType != REG_SZ && dwType != REG_EXPAND_SZ)
		{
			return (ERROR_INVALID_DATA);
		}

		if (pszValue != NULL)
		{
			if (nBytes!=0)
			{
				if ((nBytes % sizeof(TCHAR) != 0) || (pszValue[nBytes / sizeof(TCHAR) -1] != 0))
				{
 					return (ERROR_INVALID_DATA);
				}
			}
			else
			{
				pszValue[0] = '\0';
			}
		}

 		*pnChars = nBytes/sizeof(TCHAR);
		
		return ERROR_SUCCESS;
	}

	LONG winx_call getGuid(
		__in_z_opt LPCTSTR pszValueName,
		__out GUID& guidValue)
	{
		TCHAR szGUID[64];
		LONG lRes;
		ULONG nCount;
		LONG hr;

		WINX_ASSERT(m_hKey != NULL);

		guidValue = GUID_NULL;

		nCount = 64;
		lRes = getString(pszValueName, szGUID, &nCount);

		if (lRes != ERROR_SUCCESS)
			return lRes;

		if(szGUID[0] != '{')
			return ERROR_INVALID_DATA;

		WINX_USES_CONVERSION;
		LPOLESTR lpstr = WINX_T2W(szGUID);
#ifndef _UNICODE
		if(lpstr == NULL) 
			return E_OUTOFMEMORY;
#endif	
			
		hr = ::CLSIDFromString(lpstr, &guidValue);
		if (FAILED(hr))
			return ERROR_INVALID_DATA;

		return ERROR_SUCCESS;
	}

	LONG winx_call getInt(
		__in_z_opt LPCTSTR pszValueName,
		__out int& nValue)
	{
		WINX_ASSERT( sizeof(int) == sizeof(DWORD) ); //@@todo: 64bits cpu
		return getDWord(pszValueName, (DWORD&)nValue);
	}

	LONG winx_call getDWord(
		__in_z_opt LPCTSTR pszValueName,
		__out DWORD& dwValue)
	{
		LONG lRes;
		ULONG nBytes;
		DWORD dwType;

		WINX_ASSERT(m_hKey != NULL);

		nBytes = sizeof(DWORD);
		lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&dwValue),
			&nBytes);
		if (lRes != ERROR_SUCCESS)
			return (lRes);
		if (dwType != REG_DWORD)
			return (ERROR_INVALID_DATA);

		return ERROR_SUCCESS;
	}

	LONG winx_call getQWord(
		__in_z_opt LPCTSTR pszValueName,
		__out ULONGLONG& qwValue)
	{
		LONG lRes;
		ULONG nBytes;
		DWORD dwType;

		WINX_ASSERT(m_hKey != NULL);

		nBytes = sizeof(ULONGLONG);
		lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&qwValue),
			&nBytes);
		if (lRes != ERROR_SUCCESS)
			return (lRes);
		if (dwType != REG_QWORD)
			return (ERROR_INVALID_DATA);

		return ERROR_SUCCESS;
	}

	LONG winx_call getBinary(
		__in_z_opt LPCTSTR pszValueName,
		__out_bcount_opt(*pnBytes) void* pValue,
		__inout_opt ULONG* pnBytes)
	{
		LONG lRes;
		DWORD dwType;

		WINX_ASSERT(pnBytes != NULL);
		WINX_ASSERT(m_hKey != NULL);

		lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pValue),
			pnBytes);
		if (lRes != ERROR_SUCCESS)
			return (lRes);
		if (dwType != REG_BINARY)
			return (ERROR_INVALID_DATA);

		return ERROR_SUCCESS;
	}
};

// -------------------------------------------------------------------------
// class WinRegReadKey, WinRegWriteKey

class WinRegReadKey : public WinRegKeyHandle
{
private:
	WinRegReadKey(const WinRegReadKey&);
	void operator=(const WinRegReadKey&);

public:
	WinRegReadKey() {}
	WinRegReadKey(
		__in HKEY hKeyParent,
		__in_z LPCTSTR lpszKeyName,
		__in REGSAM samDesired = KEY_ALL_ACCESS)
	{
		open(hKeyParent, lpszKeyName, samDesired);
	}
	~WinRegReadKey()
	{
		close();
	}
};

class WinRegWriteKey : public WinRegKeyHandle
{
private:
	WinRegWriteKey(const WinRegWriteKey&);
	void operator=(const WinRegWriteKey&);

public:
	WinRegWriteKey() {}
	WinRegWriteKey(
		__in HKEY hKeyParent,
		__in_z LPCTSTR lpszKeyName,
		__in_z LPTSTR lpszClass = REG_NONE,
		__in DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		__in REGSAM samDesired = KEY_READ | KEY_WRITE,
		__in_opt LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		__in_opt LPDWORD lpdwDisposition = NULL)
	{
		create(hKeyParent, lpszKeyName, lpszClass, dwOptions, samDesired, lpSecAttr, lpdwDisposition);
	}
	~WinRegWriteKey()
	{
		close();
	}
};

// -------------------------------------------------------------------------
// class TestWinRegKey

#define _WINX_TEST_KEY	WINX_TEXT("Software\\winx\\TestStdExt\\TestWinRegKey")

template <class LogT>
class TestWinRegKey
{
	WINX_TEST_SUITE(TestWinRegKey);
		WINX_TEST(testWrite);
		WINX_TEST(testRead);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testWrite(LogT& log)
	{
		std::WinRegWriteKey(HKEY_CURRENT_USER, _WINX_TEST_KEY)
			.putDWord(WINX_TEXT("dword"), 123);

		std::WinRegWriteKey(HKEY_CURRENT_USER, _WINX_TEST_KEY)
			.putGuid(WINX_TEXT("guid"), IID_IClassFactory);

		std::WinRegWriteKey(HKEY_CURRENT_USER, _WINX_TEST_KEY)
			.putString(WINX_TEXT("string"), WINX_TEXT("abc"));
	
		std::WinRegWriteKey(HKEY_CURRENT_USER, _WINX_TEST_KEY)
			.putString(WINX_TEXT("string2"), std::vector<TCHAR>(257, '!'));

		std::basic_string<TCHAR> s3(WINX_TEXT("string3"));
		std::WinRegWriteKey(HKEY_CURRENT_USER, _WINX_TEST_KEY)
			.putString(WINX_TEXT("string3"), s3);
	}

	void testRead(LogT& log)
	{
		std::WinRegReadKey key(HKEY_CURRENT_USER, _WINX_TEST_KEY);
		
		DWORD dword = 0;
		key.getDWord(WINX_TEXT("dword"), dword);
		AssertExp(dword == 123);

		GUID iid;
		key.getGuid(WINX_TEXT("guid"), iid);
		AssertExp(iid == IID_IClassFactory);

		std::basic_string<TCHAR> str;
		key.getString(WINX_TEXT("string"), str);
		AssertExp(str.size() == 3);
		AssertExp(str == WINX_TEXT("abc"));

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::BasicString<TCHAR> s2;
		key.getString(WINX_TEXT("string2"), alloc, s2);
		AssertExp(s2 == std::String(alloc, 257, '!'));
	
		std::basic_string<TCHAR> s3;
		key.getString(WINX_TEXT("string3"), s3);
		AssertExp(s3 == WINX_TEXT("string3"));
	}
};

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_REGISTRY_WINREGISTRY_H__ */
