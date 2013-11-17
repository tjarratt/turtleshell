#include "ruby.h"
#include "rtl-sdr.h"

VALUE m_turtleshell = Qnil;
VALUE c_device;
static VALUE m_rtlsdr;

static VALUE turtleshell_count() {
  return INT2NUM(rtlsdr_get_device_count());
}

static VALUE turtleshell_new_device() {
  rtlsdr_dev_t *device = NULL;
  VALUE wrapped_device;
  VALUE hash;
  int count = rtlsdr_get_device_count();

  // ensure we have at least one device
  if (!count) { return Qnil; }

  rtlsdr_open(&device, 0);
  wrapped_device = Data_Wrap_Struct(c_device, NULL, NULL, device);

  hash = rb_hash_new();
  rb_hash_aset(hash, rb_str_new2("name"), rb_str_new2(rtlsdr_get_device_name(0)));
  rb_hash_aset(hash, rb_str_new2("device_handle"), wrapped_device);

  return hash;
}

void Init_librtlsdr() {
  m_turtleshell = rb_define_module("TurtleShell");
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  c_device = rb_define_class_under(m_rtlsdr, "Device", rb_cObject);
  rb_define_module_function(m_rtlsdr, "count", turtleshell_count, 0);
  rb_define_module_function(m_rtlsdr, "first_device", turtleshell_new_device, 0);
}
