#pragma once

#include <cstdint>
#include <unordered_map>
#include <type_traits>
#include "../common.h"
struct Vector3f
{
	float x;
	float y;
	float z;
};
static void hexdump(void *pAddressIn, long  lSize)
{
	char szBuf[100];
	long lIndent = 1;
	long lOutLen, lIndex, lIndex2, lOutLen2;
	long lRelPos;
	struct { char *pData; unsigned long lSize; } buf;
	unsigned char *pTmp, ucTmp;
	unsigned char *pAddress = (unsigned char *)pAddressIn;

	buf.pData = (char *)pAddress;
	buf.lSize = lSize;

	while (buf.lSize > 0)
	{
		pTmp = (unsigned char *)buf.pData;
		lOutLen = (int)buf.lSize;
		if (lOutLen > 16)
			lOutLen = 16;

		// create a 64-character formatted output line:
		sprintf(szBuf, " >                            "
			"                      "
			"    %08lX", pTmp - pAddress);
		lOutLen2 = lOutLen;

		for (lIndex = 1 + lIndent, lIndex2 = 53 - 15 + lIndent, lRelPos = 0;
			lOutLen2;
			lOutLen2--, lIndex += 2, lIndex2++
			)
		{
			ucTmp = *pTmp++;

			sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
			if (!isprint(ucTmp))  ucTmp = '.'; // nonprintable char
			szBuf[lIndex2] = ucTmp;

			if (!(++lRelPos & 3))     // extra blank after 4 bytes
			{
				lIndex++; szBuf[lIndex + 2] = ' ';
			}
		}

		if (!(lRelPos & 3)) lIndex--;

		szBuf[lIndex] = '<';
		szBuf[lIndex + 1] = ' ';

		printf("%s\n", szBuf);

		buf.pData += lOutLen;
		buf.lSize -= lOutLen;
	}
}
namespace Native
{
	typedef uintptr_t(*GetNativeAddressFunc)(uint64_t hash);
	class Context;
	typedef void(__cdecl *Handler)(Context * context);
	
	

	class Context
	{
		// Internal RAGE stuff
		uint64_t* retVal = spaceForResults;
		uint64_t argCount = 0;
		uint64_t* stackPtr = stack;
		uint64_t dataCount = 0;
		uint64_t spaceForResults[24];

		// Our stack
		uint64_t stack[24]{ 0 };

	public:

		void Push(uint32_t number) {
			*reinterpret_cast<uint32_t*>(stack + argCount++) = number;
		}
		void Push(uint64_t number) {
			*reinterpret_cast<uint64_t*>(stack + argCount++) = number;
		}
		void Push(float number) {
			*reinterpret_cast<float*>(stack + argCount++) = number;
		}void Push(bool number) {
			*reinterpret_cast<bool*>(stack + argCount++) = number;
		}
		void Push(const char* string) {
			*reinterpret_cast<const char**>(stack + argCount++) = string;
		}
		template<class T>
		T Result()
		{
			return *reinterpret_cast<T*>(retVal);
		}
		uint64_t ResultPointer() {
			return spaceForResults[0];
		}
		


		Vector3f Result()
		{
			Vector3f vec;
			vec.x = *(float*)((uintptr_t)retVal + 0);
			vec.y = *(float*)((uintptr_t)retVal + 8);
			vec.z = *(float*)((uintptr_t)retVal + 16);
			return vec;
		}

		void Reset()
		{
			argCount = 0;
			dataCount = 0;
			for (int i = 0;i < 24; i++) {
				stack[i] = 0x00;
				spaceForResults[i] = 0x00;
			}

		}

		void CopyResults()
		{
			uint64_t a1 = (uint64_t)this;

			uint64_t result;

			for (; *(uint32_t *)(a1 + 24); *(uint32_t*)(*(uint64_t *)(a1 + 8i64 * *(signed int *)(a1 + 24) + 32) + 16i64) = result)
			{
				--*(uint32_t*)(a1 + 24);
				**(uint32_t **)(a1 + 8i64 * *(signed int *)(a1 + 24) + 32) = *(uint32_t*)(a1 + 16 * (*(signed int *)(a1 + 24) + 4i64));
				*(uint32_t*)(*(uint64_t *)(a1 + 8i64 * *(signed int *)(a1 + 24) + 32) + 8i64) = *(uint32_t*)(a1
					+ 16i64
					* *(signed int *)(a1 + 24)
					+ 68);
				result = *(unsigned int *)(a1 + 16i64 * *(signed int *)(a1 + 24) + 72);
			}
			--*(uint32_t*)(a1 + 24);
		}
	};

	inline void Invoke(Handler fn, Context & ctx)
	{
		if (!fn)
			return;
		//printf("INVOKE\n");
		//hexdump(&ctx, sizeof(Context));
		__try {
			fn(&ctx);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			printf("EXCEPTION_EXECUTE_HANDLER A ");
		}
		__try {
			ctx.CopyResults();
			//printf("Copied Results");
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			printf("EXCEPTION_EXECUTE_HANDLER B");
		}




		return;
	}
	inline GetNativeAddressFunc& GetFunc()
	{
		static GetNativeAddressFunc _nativeAddressGetFunc;
		return _nativeAddressGetFunc;
	}

	inline void SetEssentialFunction(GetNativeAddressFunc func)
	{
		GetFunc() = func;
	}

	inline Handler GetHandler(uint64_t hashName)
	{
		return (Handler)GetFunc()(hashName);
	}

	
};