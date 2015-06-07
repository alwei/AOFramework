#ifndef __AO_SYSTEM_DEFINE__
#define	__AO_SYSTEM_DEFINE__

#include<string>
#include<Windows.h>
#include<cstdint>

#if defined(DEBUG) || defined(_DEBUG)

#define __AO_DEBUG__

#elif defined(RELEASE_DEBUG)

#define __AO_RDEBUG__

#else

#define __AO_RELEASE__

#endif



#if defined(__AO_DEBUG__) || defined(__AO_RDEBUG__)

#define AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(_hr,_str)										\
	if( FAILED(_hr) )																		\
	{																						\
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[%s]\n%s"),__AO_FUNCTION__,_str);	\
		return _hr;																			\
	}

#define AO_CHECK_ERROR_RETURN_HRESULT(_hr)	\
	if( FAILED(hr) )						\
	{										\
		return hr;							\
	}
#define AO_CHECK_ERROR_RETURN_TEMPLATE(_hr,_return)	\
	if( FAILED(hr) )								\
	{												\
		return _return;								\
	}

#define AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(_bool,_return,_str)						\
	if( _bool )																				\
	{																						\
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[%s]\n%s"),__AO_FUNCTION__,_str);	\
		return _return;																		\
	}

#define AO_FAILED_FUNCTION_MESSAGE(_str)	ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[%s]\n%s"),__AO_FUNCTION__,_str);
#define AO_SUCCEEDED_FUNCTION_MESSAGE(_str)	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[%s]\n%s"),__AO_FUNCTION__,_str);

#define AO_ASSERT(_flag,_str)	

#else

#define AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(_hr,_str) _hr;
#define AO_CHECK_ERROR_RETURN_HRESULT(_hr) _hr;
#define AO_CHECK_ERROR_RETURN_TEMPLATE(_hr,_return) _hr;
#define AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(_bool,_return,_str) _return;
#define AO_FAILED_FUNCTION_MESSAGE(_str)
#define AO_SUCCEEDED_FUNCTION_MESSAGE(_str)

#endif


namespace ao
{
#if UNICODE
	typedef std::wstring		string;
	typedef std::wostringstream	ostringstream;
	template<class T>
	inline std::wstring to_string(const T& _val)
	{
		return std::to_wstring(_val);
	}
#else
	typedef std::string			string;
	typedef std::ostringstream	ostringstream;
	template<class T>
	inline std::string to_string(const T& _val)
	{
		return std::to_string(_val);
	}
#endif
}

#if UNICODE
typedef	wchar_t				AOCHAR;
typedef LPWSTR				AOLPSTR;
typedef	LPCWSTR				AOLPCSTR;
typedef LPCWCHAR			AOLPCCHAR;

#define ao_strcmp			lstrcmpiW
#define	ao_strcpy_s			wcscpy_s
#define	_ao_vsnprintf_s		_vsnwprintf_s
#define ao_vsprintf_s		vswprintf_s
#define ao_fopen_s			_wfopen_s
#define ao_strlen			wcslen
#define ao_sprintf_s		swprintf_s
#define ao_to_f				_wtof
#define ao_to_i				_wtoi

#define __AO_FUNCTION__		__FUNCTIONW__

#else
typedef char				AOCHAR;
typedef LPSTR				AOLPSTR;
typedef LPCTSTR				AOLPCSTR;
typedef LPCWCHAR			AOLPCCHAR;

#define ao_strcmp			lstrcmpiA
#define	ao_strcpy_s			strcpy_s
#define	_ao_vsnprintf_s		_vsnprintf_s
#define	ao_vsprintf_s		vsprintf_s
#define ao_fopen_s			fopen_s
#define ao_strlen			strlen
#define ao_sprintf_s		sprintf_s
#define ao_to_f				atof
#define ao_to_i				atoi

#define __AO_FUNCTION__		__FUNCTION__

#endif


#ifdef _WIN64	//64bit
#define AO_GW_USERDATA GWLP_USERDATA
#else			//32bit
#define	AO_GW_USERDATA GWL_USERDATA
#endif


typedef int8_t	s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef float_t		f32;
typedef double_t	d64;

typedef intptr_t	ao_intptr;


#endif