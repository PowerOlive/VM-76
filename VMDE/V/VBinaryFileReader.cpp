//=============================================================================
// ■ VBinaryFileReader.cpp
//-----------------------------------------------------------------------------
//   在各计算机之间兼容的二进制文件读取类。（Little-endian）
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
	#define READ_CHAR(i) if ((c[i] = fgetc(f)) < 0) error(ERROR_MESSAGE);
	int32_t VBinaryFileReader::read_i32() {
		int c[4];
		READ_CHAR(0);
		READ_CHAR(1);
		READ_CHAR(2);
		READ_CHAR(3);
		return (int32_t) (c[0] | c[1] << 2 | c[2] << 4 | c[3] << 6);
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节int
	//-------------------------------------------------------------------------
	int64_t VBinaryFileReader::read_i64() {
		int c[8];
		READ_CHAR(0);
		READ_CHAR(1);
		READ_CHAR(2);
		READ_CHAR(3);
		READ_CHAR(4);
		READ_CHAR(5);
		READ_CHAR(6);
		READ_CHAR(7);
		return (int64_t) (
			c[0] | c[1] << 2 | c[2] << 4 | c[3] << 6
				| c[4] << 8 | c[5] << 10 | c[6] << 12 | c[7] << 14
		);
	}
	#undef READ_CHAR
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
		return (uint32_t) read_i32();
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节unsigned
	//-------------------------------------------------------------------------
	uint64_t VBinaryFileReader::read_u64() {
		return (uint64_t) read_i64();
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
