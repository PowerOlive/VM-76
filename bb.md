Bulletin Board
==============

这些规则即使不遵守也不会导致程序不能正常运行，但以后很有可能会这样坑到自己，因此还请仔细阅读。

C艹
---

C++有许多优秀的设计，但也有许多坑人的地方。因此，在本项目中：

* 禁止使用iostream
* 禁止使用C艹 string

printf
------

有些数据类型确实会让人不知道怎么在printf里表达。

数据类型 | printf
-------- | ------
void* | %p
size_t | %zu
ptrdiff_t | %td

内存
----

这些内存申请和释放函数需要这样配对使用。

申请 | 释放
---- | ----
`T* a = (T*) malloc(233);` | `free(a)`
`T* a = new T;` | `delete a;`
`T* a = new T[233];` | `delete[] a;`