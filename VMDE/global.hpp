//=============================================================================
// ■ VMDE/global.hpp
//-----------------------------------------------------------------------------
//   在VMDE的内部，包容一切的头。
//   关于只有一个头：
//   我完全无法理解为什么C要采用有头文件的设计。当然这很可能是历史遗留问题，
//   但这导致了C程序员总是需要花费大量时间整理头文件，且这时间花得莫名其妙。
//   而这在其他任何语系中都不是问题：即使是采用C语法的衍生语言，也不会保留
//   这个“特性”。因此我们需要解决它。
//   采用#include管理多文件编译是最好的解决方案。在现今编译单个文件的速度有极
//   大提升的时代，一个个编译小文件却慢得多。然而这个解决方案并不能在像VMDE这
//   样的合作项目中进行，因为它会让很多已建立的条件反射、编译器、IDE日狗。
//   单一头文件也是一个可行的方案。它保留了头文件，但只有一个——这意味着所有
//   你需要编辑的头都在固定的位置，给身体和手带来了放松的空间，同时也保证编辑
//   器的标签栏（如果有的话）不会因为头文件而缩小为原来的一半。
//   当然，还有一个解决办法就是只用一个.c文件。结果：从此这个项目就没落了。
//   综上所述，我选择狗带，砍了其他所有头。
//=============================================================================

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <ctime>
#include <thread>
#include <stdarg.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <portaudio.h>
#include <vorbis/vorbisfile.h>

#ifndef _INCLUDE_GLOBAL_H
	#define _INCLUDE_GLOBAL_H
	using namespace std;
	//-------------------------------------------------------------------------
	// ● PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	//Becuase it;s Windoges,I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	//-------------------------------------------------------------------------
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif
	#ifdef __CYGWIN__
		#warning This will not work in Cygwin. Try at your own risk.
	#endif
	//-------------------------------------------------------------------------
	// ● 全局变量表
	//-------------------------------------------------------------------------
	// API
	EXPORTED void init_engine(int w, int h, const char* title);
	// 全局事件
	EXPORTED void (*on_terminate)();
	//-------------------------------------------------------------------------
	// ● 定义宏魔法
	//-------------------------------------------------------------------------
	// ARRAY_SIZE - 获取定义的数组大小
	//     int a[56]; → ARRAY_SIZE(a) == 56
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))
	// MLVN - Macro-Local Variable Name
	// 这是为了不让明文变量名被莫名其妙使用而设计的。
	//     #define TEST do { int MLVN() = 0, MLVN(another) = 0; } while(0)
	#define MLVN(name) My_Lovely_##name##___
	// NTIMES - 执行多次
	//     NTIMES(3) puts("Three or more, use a for");
	#define NTIMES(n) \
		for (unsigned short MLVN(index) = 0; MLVN(index) < (n); MLVN(index)++)
	// TWICE - 执行两次
	//     TWICE a++;
	//     TWICE { a++; b++; }
	#define TWICE NTIMES(2)
	// EXPLOSION!
	// 可以安全地在用户级代码中调用并获得SIGSEGV。
	// 在退出程序时装逼。
	#define EXPLOSION EXP_ASSIGNMENT
	// EXP_ASSIGNMENT - EXPLOSION的赋值实现
	#define EXP_ASSIGNMENT ((void) (*((float*) NULL) = INFINITY))
	// EXP_MEMORY - EXPLOSION的标准C库内存操作实现
	#define EXP_MEMORY ((void) memset(NULL, 233, 1))
	// EXP_SIGNAL - EXPLOSION的标准C库信号实现
	// * 需要#include <csignal>，但本项目没有包含。
	#define EXP_SIGNAL ((void) ((raise(SIGSEGV) == 0) || abort()))
	// EXP_CONSTSTR - EXPLOSION的只读字符串实现
	#define EXP_CONSTSTR ((void) *"" = 0;)
	// EXP_ASM - EXPLOSION的Pentium Pro指令实现
	// * 这个实现不会导致SIGSEGV。
	#define EXP_ASM (__asm__("UD2"))
	// xeFree - 释放内存黑魔法
	#define xefree(pointer) \
		do { if (pointer) { \
		free(pointer); pointer = NULL; \
		} } while (false)

	#define VMDE_Dispose(x) xefree(x->dispose());

	#ifdef __GNUC__
		#define UNUSED(x) x __attribute__((__unused__))
	#else
		#define UNUSED(x) x
	#endif
	//-------------------------------------------------------------------------
	// ● shaders.cpp
	//-------------------------------------------------------------------------
	class Shaders {
	public:
		GLuint basic_2D_vsh;
		GLuint basic_2D_fsh;
		GLuint shaderProgram;

		GLuint UBO_matrix;
		glm::mat4* mat = new glm::mat4[3];

	public:
		Shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr);
		void link_program();
		void use();

		void set_float(const char* identifier, GLfloat value);
		void set_int(const char* identifier, GLint value);

		void ProjectionView(glm::mat4 projection, glm::mat4 view);
	};
	extern Shaders* _shaders_in_use;
	//-------------------------------------------------------------------------
	// ● init.cpp
	//-------------------------------------------------------------------------
	void setup_viewport();
	void init_vmde(int w, int h);
	//-------------------------------------------------------------------------
	// ● main.cpp
	//-------------------------------------------------------------------------
	void glfw_error_callback(int error, const char* description);
	void main_draw_start();
	void main_draw_end();
	void main_set_brightness(float b);
	extern glm::mat4 projection, view;
	//-------------------------------------------------------------------------
	// ● resources.cpp
	//-------------------------------------------------------------------------
	namespace Res {
		class Texture {
		public:
			GLuint texture, index;
			int width, height;
			Texture(const char* file, GLuint index);
			Texture* dispose();
		};
		extern Texture* tex_unit[16];
	}
	//-------------------------------------------------------------------------
	// ● util.cpp
	//-------------------------------------------------------------------------
	#define DEBUG_ENVIRONMENT "VMDE"
	// log - Util::log_internal的方便缩写，可以直接得到当前函数名
	//     log("%p", log);
	#define log(...) Util::log_internal(DEBUG_ENVIRONMENT, __func__, __VA_ARGS__)
	namespace Util {
		extern const float PIf;
		extern const double PI;
		#define PId PI
		void log_internal(const char*, const char*, const char*, ...);
		char* read_file(const char* filename);
	}
	//-------------------------------------------------------------------------
	// ● VMDE
	//-------------------------------------------------------------------------
	struct VMDEState {
		bool frozen;
		float brightness;
	};

	enum GL_VER {
		GL_33,
		GL_43,
		VULKAN // Not used
	};

	struct VMDE {
		VMDEState state;
		long frame_count;
		long millisecond;
		int width, height;
		int fps;
		double frame_time;
		bool done;

		int gl_ver;
	};

	// VMDE操控的全局变量
	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	//-------------------------------------------------------------------------
	// ● Audio
	//-------------------------------------------------------------------------
	namespace Audio {
		struct triangle_data {
			float value;
			float delta;
		};
		struct sine_data {
			float index;
			float index_delta;
			bool minus;
			float value; // for convenience only
		};
		struct wave_callback_data {
			double sample_rate;
			// type = 0……静音；1……三角波；2……正弦波；3……白噪音
			// 为啥不用枚举？因为太麻烦了！
			int type;
			union {
				struct triangle_data triangle;
				struct sine_data sine;
			} data;
		};
		struct active_sound {
			PaStream* stream;
			FILE* file;
			OggVorbis_File vf;
			#define AUDIO_VF_BUFFER_SIZE ((size_t) 4096)
			float vf_buffer[2][AUDIO_VF_BUFFER_SIZE];
			size_t play_head;
			size_t load_head;
			bool eof;
			bool loop;
			int bitstream;
			thread* decode_thread;
		};
		extern PaStream* wave_stream;
		extern struct wave_callback_data wave_data;
		#define AUDIO_ACTIVE_SOUND_SIZE ((size_t) 16)
		extern struct active_sound* active_sounds[AUDIO_ACTIVE_SOUND_SIZE];
		#define AUDIO_SINE_TABLE_SIZE ((size_t) 256)
		extern float sine_table[AUDIO_SINE_TABLE_SIZE];
		void init();
		void init_waves();
		void wobuzhidaozhegefangfayinggaijiaoshenmemingzi();
		void ensure_no_error(PaError err);
		int play_wave_callback(
			UNUSED(const void* input_buffer),
			void* output_buffer,
			unsigned long frames_per_buffer,
			UNUSED(const PaStreamCallbackTimeInfo* time_info),
			UNUSED(PaStreamCallbackFlags status_flags),
			void* user_data
		);
		void stop();
		void stop_waves();
		void play_triangle(float freq);
		void get_next_triangle_value(struct triangle_data* data);
		void play_sine(float freq);
		void populate_sine_table();
		void get_next_sine_value(struct sine_data* data);
		void compact_active_sounds_array();
		void play_sound(const char* filename, bool loop);
		int play_sound_callback(
			UNUSED(const void* input_buffer),
			void* output_buffer,
			unsigned long frame_count,
			UNUSED(const PaStreamCallbackTimeInfo* time_info),
			UNUSED(PaStreamCallbackFlags status_flags),
			void* user_data
		);
		void decode_vorbis(struct active_sound* sound);
		void decode_vorbis_thread(struct active_sound* sound);
	}
	//-------------------------------------------------------------------------
	// ● GDrawable
	//-------------------------------------------------------------------------
	class GDrawable {
	public:
		struct Data {
			GLfloat* vertices;
			int vtx_c;
			GLuint* indices;
			int ind_c;
			GLuint* mat;
			int mat_c;
			GLuint MBO;
			GLuint VAO;
			GLuint VBO;
			GLuint EBO;
			GLuint tri_mesh_count;
			glm::mat4 model;
		} data;

		void draw(GLuint start, GLuint end);
		void draw();
		void fbind();
		void update();
		void update_instance();
		GDrawable* dispose();

		GDrawable();
	};
#endif
