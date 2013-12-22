#include "ruby.h"
#include "rtl-sdr.h"

VALUE m_turtleshell = Qnil;
VALUE c_device;
static VALUE m_rtlsdr;

static VALUE turtleshell_count() {
  return INT2NUM(rtlsdr_get_device_count());
}

static VALUE turtleshell_first_device() {
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

static VALUE turtleshell_all_devices() {
  VALUE devices_array = rb_ary_new();
  int count = rtlsdr_get_device_count();

  for (int i = 0; i < count; ++i) {
    rb_ary_push(devices_array, rb_str_new2(rtlsdr_get_device_name(i)));
  }

  return devices_array;
}

static VALUE turtleshell_nth_device(VALUE self, VALUE n) {
  uint32_t int_n = NUM2UINT(n);
  rtlsdr_dev_t *device = NULL;
  VALUE hash, wrapped_device;

  if (n < rtlsdr_get_device_count()) { return Qnil; }

  rtlsdr_open(&device, int_n);
  wrapped_device = Data_Wrap_Struct(c_device, NULL, NULL, device);

  hash = rb_hash_new();
  rb_hash_aset(hash, rb_str_new2("name"), rb_str_new2(rtlsdr_get_device_name(0)));
  rb_hash_aset(hash, rb_str_new2("device_handle"), wrapped_device);

  return hash;
}

static VALUE turtleshell_read_synchronous(VALUE self,
                                          VALUE device,
                                          VALUE buffer,
                                          VALUE bytes_to_read,
                                          VALUE bytes_read_return_value) {

}

typedef void *(turtleshell_callback)(void *);
static void turtleshell_read_asynchronous(VALUE self,
                                          VALUE device,
                                          VALUE buffer,
                                          VALUE bytes_to_read,
                                          VALUE bytes_read_return_value,
                                          turtleshell_callback callback) {

}

void Init_librtlsdr() {
  m_turtleshell = rb_define_module("TurtleShell");
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  c_device = rb_define_class_under(m_rtlsdr, "Device", rb_cObject);
  rb_define_module_function(m_rtlsdr, "count", turtleshell_count, 0);
  rb_define_module_function(m_rtlsdr, "first_device", turtleshell_first_device, 0);
  rb_define_module_function(m_rtlsdr, "all_devices", turtleshell_all_devices, 0);
  rb_define_module_function(m_rtlsdr, "nth_device", turtleshell_nth_device, 1);
  rb_define_module_function(m_rtlsdr, "read_sync", turtleshell_read_synchronous, 4);
  rb_define_module_function(m_rtlsdr, "read_async", turtleshell_read_asynchronous, 5);
}
