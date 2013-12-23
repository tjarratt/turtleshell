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
  /* rtlsdr_read_sync(); */
  return Qnil;
}

typedef void *(turtleshell_callback)(void *);
static void turtleshell_read_asynchronous(VALUE self,
                                          VALUE device,
                                          VALUE buffer,
                                          VALUE bytes_to_read,
                                          VALUE bytes_read_return_value,
                                          turtleshell_callback callback) {
  /* rtlsdr_read_async(); */
}

static VALUE turtleshell_get_sample_rate(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return UINT2NUM(rtlsdr_get_sample_rate(device));
}

static void turtleshell_set_sample_rate(VALUE self,
                                        VALUE wrapped_device,
                                        VALUE value) {
  uint32_t rate = NUM2UINT(value);
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  rtlsdr_set_sample_rate(device, rate);
}

static VALUE turtleshell_get_center_frequency(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return UINT2NUM(rtlsdr_get_center_freq(device));
}

static void turtleshell_set_center_frequency(VALUE self,
                                             VALUE wrapped_device,
                                             VALUE value) {
  uint32_t freq = NUM2UINT(value);
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  rtlsdr_set_center_freq(device, freq);
}

static VALUE turtleshell_get_gain(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return INT2NUM(rtlsdr_get_tuner_gain(device));
}

static void turtleshell_set_gain(VALUE self, VALUE wrapped_device, VALUE value) {
  int gain = NUM2INT(value);
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  rtlsdr_set_tuner_gain(device, gain);
}

void Init_librtlsdr() {
  m_turtleshell = rb_define_module("TurtleShell");
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  c_device = rb_define_class_under(m_rtlsdr, "Device", rb_cObject);

  // count of devices
  rb_define_module_function(m_rtlsdr, "count", turtleshell_count, 0);

  // getting references to new devices
  rb_define_module_function(m_rtlsdr, "first_device", turtleshell_first_device, 0);
  rb_define_module_function(m_rtlsdr, "all_devices", turtleshell_all_devices, 0);
  rb_define_module_function(m_rtlsdr, "nth_device", turtleshell_nth_device, 1);

  // reading bytes
  rb_define_module_function(m_rtlsdr, "read_sync", turtleshell_read_synchronous, 4);
  rb_define_module_function(m_rtlsdr, "read_async", turtleshell_read_asynchronous, 5);

  // getters and setters
  rb_define_module_function(m_rtlsdr, "get_sample_rate", turtleshell_get_sample_rate, 1);
  rb_define_module_function(m_rtlsdr, "set_sample_rate", turtleshell_set_sample_rate, 2);
  rb_define_module_function(m_rtlsdr, "get_center_freq", turtleshell_get_center_frequency, 1);
  rb_define_module_function(m_rtlsdr, "set_center_freq", turtleshell_set_center_frequency, 2);
  rb_define_module_function(m_rtlsdr, "get_gain", turtleshell_get_gain, 1);
  rb_define_module_function(m_rtlsdr, "set_gain", turtleshell_set_gain, 2);
}
