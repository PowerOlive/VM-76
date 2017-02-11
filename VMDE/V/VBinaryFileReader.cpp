//=============================================================================
// ■ VBinaryFileReader.cpp
//-----------------------------------------------------------------------------
//   在各计算机之间兼容的二进制文件读取类。
//=============================================================================

#include "../global.hpp"

namespace V {
	#define ERROR_MESSAGE "error: cannot find beer"
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	VBinaryFileReader::VBinaryFileReader(const char* filename) {
		f = fopen(filename, "rb");
		if (!f) error(ERROR_MESSAGE);
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	VBinaryFileReader::~VBinaryFileReader() {
		fclose(f);
	}
	//-------------------------------------------------------------------------
	// ● 读取1字节int
	//-------------------------------------------------------------------------
	int8_t VBinaryFileReader::read_i8() {
		return (int8_t) read_u8();
	}
	//-------------------------------------------------------------------------
	// ● 读取4字节int
	//-------------------------------------------------------------------------
	int32_t VBinaryFileReader::read_i32() {
		uint32_t c = fgetc(f);
		c = c | (fgetc(f) << 8);
		c = c | (fgetc(f) << 16);
		c = c | (fgetc(f) << 24);
		return (int32_t) c;
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节int
	//-------------------------------------------------------------------------
	int64_t VBinaryFileReader::read_i64() {
		uint64_t c1 = fgetc(f);
		c1 = c1 | (fgetc(f) << 8);
		c1 = c1 | (fgetc(f) << 16);
		c1 = c1 | (fgetc(f) << 24);
		uint64_t c2 = fgetc(f);
		c2 = c2 | (fgetc(f) << 8);
		c2 = c2 | (fgetc(f) << 16);
		c2 = c2 | (fgetc(f) << 24);
		uint64_t c = (c2 << 32) | (c1 & 0x00000000FFFFFFFF);
		return (int64_t) c;
	}
	//-------------------------------------------------------------------------
	// ● 读取1字节unsigned
	//-------------------------------------------------------------------------
	uint8_t VBinaryFileReader::read_u8() {
		int c = fgetc(f);
		if (c < 0) error(ERROR_MESSAGE);
		return (uint8_t) c;
	}
	//-------------------------------------------------------------------------
	// ● 读取4字节unsigned
	//-------------------------------------------------------------------------
	uint32_t VBinaryFileReader::read_u32() {
		uint32_t c = fgetc(f);
		c = c | (fgetc(f) << 8);
		c = c | (fgetc(f) << 16);
		c = c | (fgetc(f) << 24);
		return c;
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节unsigned
	//-------------------------------------------------------------------------
	uint64_t VBinaryFileReader::read_u64() {
		uint64_t c1 = fgetc(f);
		c1 = c1 | (fgetc(f) << 8);
		c1 = c1 | (fgetc(f) << 16);
		c1 = c1 | (fgetc(f) << 24);
		uint64_t c2 = fgetc(f);
		c2 = c2 | (fgetc(f) << 8);
		c2 = c2 | (fgetc(f) << 16);
		c2 = c2 | (fgetc(f) << 24);
		uint64_t c = (c2 << 32) | (c1 & 0x00000000FFFFFFFF);
		return c;
	}
	//-------------------------------------------------------------------------
	// ● 读取4字节float
	//-------------------------------------------------------------------------
	float VBinaryFileReader::read_float() {
		return read_directly<float>();
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节double
	//-------------------------------------------------------------------------
	double VBinaryFileReader::read_double() {
		return read_directly<double>();
	}
}
