//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数（即RubyWrapper），
//   还负责一些Ruby扩展所需的杂务和整个初始化系统的调用。
//=============================================================================

#include "global.hpp"

GLfloat vertices[] = {
	0.0f, 0.0f, 0.0f,
	435.0f, 0.0f, 0.0f,
	435.0f, 270.0f, 0.0f
};

namespace RubyWrapper {
	VALUE load_pic(VALUE self, VALUE path) {
		Check_Type(path, T_STRING);
		char* a = RSTRING_PTR(path);
		return Qnil;//RSTRING(load_img(a));
	}

	struct ptr_data {
		void* ptr;
		long size;
		type_free_function free;
		VALUE wrap[2];
	};

	static void rb_data_mark(void* ptr) {
		struct ptr_data* data = (struct ptr_data*) ptr;
		if (data->wrap[0]) rb_gc_mark(data->wrap[0]);
		if (data->wrap[1]) rb_gc_mark(data->wrap[1]);
	}

	static void rb_data_free(void* ptr) {
		struct ptr_data* data = (struct ptr_data*) ptr;
		if (data->ptr && data->free) (*(data->free))(data->ptr);
		RCN* n = (RCN*) data->ptr;
		if (n) {
			if (n->gd) {
				if (n->gd->vertices) {
					free(n->gd->vertices);
				}
				free(n->gd);
			}
		}
		xfree(ptr);
	}

	static size_t rb_data_memsize(const void* ptr) {
		const struct ptr_data* data = (const struct ptr_data*) ptr;
		return sizeof(*data) + data->size;
	}

	rb_data_type_t gdrawable_data_type = {
		"Drawable_C_Data",
		{rb_data_mark, rb_data_free, rb_data_memsize},
	};

	VALUE gdrawable_visible_get(VALUE val, VALUE cdata) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;

		return node->visible ? Qtrue : Qfalse;
	}

	VALUE gdrawable_visible_set(VALUE val, VALUE cdata, VALUE b) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;

		if (b == Qtrue && node->visible != true) {
			node->visible = true;
			node->prev = NULL;
			node->next = render_chain;
			if (node->next)
				node->next->prev = node;
			render_chain = node;
		}
		if (b == Qfalse && node->visible != false) {
			node->visible = false;
			if (node == render_chain)
				render_chain = node->next;
			if (node->prev)
				node->prev->next = node->next;
			if (node->next)
				node->next->prev = node->prev;
			node->prev = NULL;
			node->next = NULL;
		}

		return b;
	}

	size_t load_vertices(VALUE vertex_array, GDrawable::GDrawable* gd) {
		Check_Type(vertex_array, T_ARRAY);
		long i;
		long length = rb_array_len(vertex_array);
		for (i = 0; i < length; i++) {
			VALUE k = rb_ary_entry(vertex_array, i);
			Check_Type(k, T_FLOAT);
		}
		size_t size = sizeof(GLfloat) * length;
		gd->vertices = (GLfloat*) malloc(size);
		for (i = 0; i < length; i++) {
			gd->vertices[i] = (float) rb_float_value(rb_ary_entry(vertex_array, i));
		}
		gd->size_of_VBO = size;
	}

	VALUE gdrawable_bind_obj(VALUE self, VALUE ary) {
		RCN* node = (RCN*) malloc(sizeof(RCN));
		node->n = self;
		node->visible = false; node->prev = NULL; node->next = NULL;
		node->gd = GDrawable::create();
		load_vertices(ary, node->gd);
		GDrawable::update(node->gd);

		struct ptr_data* data;
		VALUE v = TypedData_Make_Struct(rb_cData, struct ptr_data, &gdrawable_data_type, data);
		data->ptr = node;
		data->free = free;
		data->size = sizeof(RCN);

		return v;
	}

	VALUE init_engine(VALUE self UNUSED, VALUE w, VALUE h) {
		Check_Type(w, T_FIXNUM);
		Check_Type(h, T_FIXNUM);
		::init_engine(FIX2INT(w), FIX2INT(h));
		return Qnil;
	}

	VALUE main_draw_loop() {
		::main_draw_loop();
		return Qnil;
	}

	VALUE main_get_frame_count() {
		return LONG2FIX(VMDE->frame_count);
	}

	VALUE main_get_fps() {
		return INT2FIX(VMDE->fps);
	}

	VALUE main_matrix2D() {
		::matrix2D();
		return Qtrue;
	}

	VALUE main_set_brightness(VALUE self UNUSED, VALUE value) {
		Check_Type(value, T_FLOAT);
		VMDE->state.brightness = rb_float_value(value);
		return Qnil;
	}

	VALUE audio_stop(VALUE self UNUSED) {
		Audio::stop();
		return Qnil;
	}

	VALUE audio_play_wave(VALUE self UNUSED, VALUE type, VALUE freq) {
		static const ID triangle = rb_intern("triangle");
		static const ID sine = rb_intern("sine");
		Check_Type(type, T_SYMBOL);
		Check_Type(freq, T_FLOAT);
		float f = rb_float_value(freq);
		if (f <= 0) rb_raise(rb_eRangeError, "frequency must be positive");
		switch (SYM2ID(type)) {
			case triangle:
				Audio::play_triangle(f);
				break;
			case sine:
				Audio::play_sine(f);
				break;
			default:
				rb_raise(rb_eArgError, "invalid wave type");
		}
		return Qnil;
	}
}

void init_ruby_modules() {
	#define RUBY_MODULE_API(module, ruby_name, wrapper_name, parameter_count) \
		rb_define_module_function(ruby_##module, #ruby_name, \
		(type_ruby_function) RubyWrapper::wrapper_name, parameter_count)
	ruby_VMDE = rb_define_module("VMDE");
	RUBY_MODULE_API(VMDE, init, init_engine, 2);
	RUBY_MODULE_API(VMDE, update, main_draw_loop, 0);
	RUBY_MODULE_API(VMDE, frame_count, main_get_frame_count, 0);
	RUBY_MODULE_API(VMDE, fps, main_get_fps, 0);
	RUBY_MODULE_API(VMDE, matrix2D, main_matrix2D, 0);
	RUBY_MODULE_API(VMDE, brightness=, main_set_brightness, 1);

	VALUE ruby_Audio = rb_define_module_under(ruby_VMDE, "Audio");
	RUBY_MODULE_API(Audio, stop, audio_stop, 0);
	RUBY_MODULE_API(Audio, play_wave, audio_play_wave, 2);
	#undef RUBY_MODULE_API
}

void init_ruby_classes() {
	ruby_GResPic = rb_define_class_under(ruby_VMDE, "GResPic", rb_cObject);
	rb_define_method(ruby_GResPic, "load_pic", (type_ruby_function) RubyWrapper::load_pic, 1);

	ruby_GDrawable = rb_define_class_under(ruby_VMDE, "GDrawable", rb_cObject);
	rb_define_method(ruby_GDrawable, "bind", (type_ruby_function) RubyWrapper::gdrawable_bind_obj, 1);
	rb_define_method(ruby_GDrawable, "set_visible", (type_ruby_function) RubyWrapper::gdrawable_visible_set, 2);
	rb_define_method(ruby_GDrawable, "get_visible", (type_ruby_function) RubyWrapper::gdrawable_visible_get, 1);
}

EXPORTED void Init_VMDE() {
	log("initializing the module");

	init_ruby_modules();
	init_ruby_classes();
}
