#include "ruby.h"
#include "rtl-sdr.h"

VALUE m_turtleshell = Qnil;
static VALUE m_rtlsdr;

static VALUE turtleshell_count() {
  return INT2NUM(rtlsdr_get_device_count());
}

static VALUE turtleshell_new_device() {
  char *name;
  rtlsdr_dev_t *device = NULL;
  int count = rtlsdr_get_device_count();

  // ensure we have at least one device
  if (!count) { return Qnil; }

  name = rtlsdr_get_device_name(0);
  device = rtlsdr_open(&device, 0);

  VALUE device_hash = rb_hash_new();
  rb_hash_aset(device_hash, rb_str_new2("name"), rb_str_new2(rtlsdr_get_device_name(0)));
  rb_hash_aset(device_hash, rb_str_new2("device_handle"), device);

  return device_hash;
}

void Init_librtlsdr() {
  m_turtleshell = rb_define_module("TurtleShell");
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  rb_define_module_function(m_rtlsdr, "count", turtleshell_count, 0);
  rb_define_module_function(m_rtlsdr, "first_device", turtleshell_new_device, 0);
}
