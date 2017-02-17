//=============================================================================
// ■ V.hpp
//-----------------------------------------------------------------------------
//   注意：这个目录里的很多文件都是模板类，它们有单独的头文件。
//=============================================================================

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#ifndef _INCLUDE_V_HPP
#define _INCLUDE_V_HPP

#ifndef error
	#include <cstdio>
	#define error(x) fprintf(stderr, "%s\n", x)
#endif

#include "VExplosion.hpp"
namespace V {
	//-------------------------------------------------------------------------
	// ● 模板类和子头
	//-------------------------------------------------------------------------
	#include "VMath.hpp"
	#include "VVector.hpp"
	#include "VRingBuffer.hpp"
	//-------------------------------------------------------------------------
	// ● Endian detecting
	//-------------------------------------------------------------------------
	bool is_little_endian();
	//-------------------------------------------------------------------------
	// ● VBinaryFileReader
	//-------------------------------------------------------------------------
	class VBinaryFileReader {
	public:
		FILE* f;
		VBinaryFileReader(const char* filename);
		~VBinaryFileReader();
		int8_t read_i8();
		int32_t read_i32();
		int64_t read_i64();
		uint8_t read_u8();
		uint32_t read_u32();
		uint64_t read_u64();
		float read_float();
		double read_double();
		template <class T> T read_directly() {
			T r;
			if (fread(&r, sizeof(T), 1, f) < 1) error("read_directly");
			return r;
		}
	};
	//-------------------------------------------------------------------------
	// ● VBinaryFileWriter
	//-------------------------------------------------------------------------
	class VBinaryFileWriter {
	public:
		FILE* f;
		VBinaryFileWriter(const char* filename);
		~VBinaryFileWriter();
		template <class T> void write_directly(T value) {
			if (fwrite(&value, sizeof(T), 1, f) < 1) error("write_directly");
		}
		void write_i8(int8_t v);
		void write_i32(int32_t v);
		void write_i64(int64_t v);
		void write_u8(uint8_t v);
		void write_u32(uint32_t v);
		void write_u64(uint64_t v);
		void write_float(float v);
		void write_double(double v);
	};
}

// 自杀式命名空间
using namespace V;
#endif