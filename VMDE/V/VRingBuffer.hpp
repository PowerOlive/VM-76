//=============================================================================
// ■ VRingBuffer.hpp
//-----------------------------------------------------------------------------
//   环状队列数据结构模板类。
//=============================================================================

#include <cstddef>
#include <cassert>

template <class T, size_t size> class VRingBuffer {
private:
	//-------------------------------------------------------------------------
	// ● 一堆指针
	//-------------------------------------------------------------------------
	// [  ][**][**][**][**][  ][  ][  ]
	//  BS  Fr              Ba          BE
	T* buf_start;
	T* buf_end;
	T* front;
	T* back;
public:
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	VRingBuffer() {
		buf_start = front = back = new T[size];
		buf_end = buf_start + size;
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	~VRingBuffer() {
		delete[] buf_start;
	}
	//-------------------------------------------------------------------------
	// ● 队列是否为空？
	//-------------------------------------------------------------------------
	bool is_empty() {
		return front == back;
	}
	//-------------------------------------------------------------------------
	// ● 队列是否为满？
	//-------------------------------------------------------------------------
	bool is_full() {
		return back == front - 1 || (front == buf_start && back == buf_end);
	}
	//-------------------------------------------------------------------------
	// ● 清空
	//-------------------------------------------------------------------------
	void clear() {
		front = back = buf_start;
	}
	//-------------------------------------------------------------------------
	// ● 入队
	//-------------------------------------------------------------------------
	bool enqueue(T x) {
		if (is_full()) return false;
		*back = x;
		back++;
		if (back >= buf_end) back = buf_start;
		return true;
	}
	//-------------------------------------------------------------------------
	// ● 出队
	//   需要手动在调用前查询队列是否为空。
	//-------------------------------------------------------------------------
	T dequeue() {
		T* r;
		assert(!is_empty());
		r = front;
		front++;
		if (front >= buf_end) front = buf_start;
		return *r;
	}
	//-------------------------------------------------------------------------
	// ● 查询元素个数
	//-------------------------------------------------------------------------
	size_t length() {
		ptrdiff_t r = back - front;
		if (r < 0) r += size;
		assert(r >= 0);
		return (size_t) r;
	}
};
