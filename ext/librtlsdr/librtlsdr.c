#include "ruby.h"

VALUE m_turtleshell = Qnil;
static VALUE m_rtlsdr;

static void init_rtlsdr_module() {
}

void Init_librtlsdr() {
  m_turtleshell = rb_define_module("TurtleShell");
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  rb_define_module_function(m_rtlsdr, "init", init_rtlsdr_module, 0);
}
