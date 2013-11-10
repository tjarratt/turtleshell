#include "ruby.h"

extern VALUE m_turtleshell;
static VALUE m_rtlsdr;

static void init_rtlsdr_module() {

}

void Init_lib_rtl_sdr_ext() {
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  rb_define_module_function(m_rtlsdr, "init", init_rtlsdr_module, 0);
}
