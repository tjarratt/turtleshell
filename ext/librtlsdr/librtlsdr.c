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
  int i, count;
  VALUE devices_array = rb_ary_new();
  count = rtlsdr_get_device_count();

  for (i = 0; i < count; ++i) {
    rb_ary_push(devices_array, rb_str_new2(rtlsdr_get_device_name(i)));
  }

  return devices_array;
}

static VALUE turtleshell_nth_device(VALUE self, VALUE n) {
  int open_success;
  uint32_t int_n = NUM2UINT(n);
  rtlsdr_dev_t *device = NULL;
  VALUE wrapped_device, hash = rb_hash_new();
  uint32_t device_count = (uint32_t)rtlsdr_get_device_count();

  if (int_n >= device_count) { return Qnil; }

  open_success = rtlsdr_open(&device, int_n);
  if (open_success != 0) {
    return Qnil;
  }

  wrapped_device = Data_Wrap_Struct(c_device, NULL, NULL, device);

  rb_hash_aset(hash, ID2SYM(rb_intern("name")), rb_str_new2(rtlsdr_get_device_name(0)));
  rb_hash_aset(hash, ID2SYM(rb_intern("device")), wrapped_device);

  return hash;
}

static VALUE turtleshell_close(VALUE self, VALUE device) {
  rtlsdr_dev_t *dev = NULL;
  Data_Get_Struct(device, rtlsdr_dev_t, dev);
  rtlsdr_close(dev);
  return Qnil;
}

static VALUE turtleshell_read_synchronous(VALUE self,
                                          VALUE device,
                                          VALUE bytes_to_read) {
  int success, i;
  int bytes_read;
  VALUE buffer = rb_ary_new();
  int length = NUM2INT(bytes_to_read);
  uint8_t *p_buffer = malloc(sizeof(uint8_t) * bytes_to_read);
  rtlsdr_dev_t *p_device;

  Data_Get_Struct(device, rtlsdr_dev_t, p_device);

  rtlsdr_reset_buffer(p_device);
  success = rtlsdr_read_sync(p_device, p_buffer, length, &bytes_read);
  if (success != 0) {
    printf("error reading bytes. read_sync returned %d\n", success);
    free(p_buffer);
    return buffer;
  }

  for (i = 0; i < bytes_read; ++i) {
    rb_ary_push(buffer, UINT2NUM((uint8_t)p_buffer[i]));
  }

  free(p_buffer);
  return buffer;
}

struct turtleshell_context {
  VALUE callback;
  rtlsdr_dev_t *device;
};

typedef struct turtleshell_context turtleshell_context;

static void turtleshell_callback(unsigned char *buffer, uint32_t length, void *context) {
  uint32_t i;
  rtlsdr_dev_t *device;
  VALUE buffer_array, callback, bool_value;
  struct turtleshell_context unwrapped_context;

  unwrapped_context = *(struct turtleshell_context *)context;

  if (!unwrapped_context.callback || !unwrapped_context.device) {
    printf("unexpected error: could not read callback / device from unwrapped context\n");
    return;
  }

  device = unwrapped_context.device;
  callback = unwrapped_context.callback;

  buffer_array = rb_ary_new();
  for (i = 0; i < length; ++i) {
    rb_ary_push(buffer_array, UINT2NUM((uint8_t)buffer[i]));
  }

  // TODO: do we need to free(buffer) or context ?
  bool_value = RTEST(rb_funcall(callback, rb_intern("call"), 1, buffer_array));

  if (!bool_value) {
    rtlsdr_cancel_async(device);
  }
}

static VALUE turtleshell_read_asynchronous(VALUE self,
                                          VALUE device,
                                          VALUE bytes_to_read,
                                          VALUE callback) {
  int success, length = NUM2INT(bytes_to_read);
  rtlsdr_dev_t *p_device;
  struct turtleshell_context context;
  void *wrapped_context;

  Data_Get_Struct(device, rtlsdr_dev_t, p_device);

  context.device = p_device;
  context.callback = callback;
  wrapped_context = (void *)&context;

  rtlsdr_reset_buffer(p_device);
  success = rtlsdr_read_async(p_device, turtleshell_callback, wrapped_context, 0, length);

  if (success != 0) {
    printf("error reading async: return code %d\n", success);
  }

  return Qnil;
}

static VALUE turtleshell_get_sample_rate(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return UINT2NUM(rtlsdr_get_sample_rate(device));
}

static VALUE turtleshell_set_sample_rate(VALUE self,
                                        VALUE wrapped_device,
                                        VALUE value) {
  uint32_t rate = NUM2UINT(value);
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  rtlsdr_set_sample_rate(device, rate);

  return Qnil;
}

static VALUE turtleshell_get_center_frequency(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return UINT2NUM(rtlsdr_get_center_freq(device));
}

static VALUE turtleshell_set_center_frequency(VALUE self,
                                             VALUE wrapped_device,
                                             VALUE value) {
  uint32_t freq = NUM2UINT(value);
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  rtlsdr_set_center_freq(device, freq);
  return Qnil;
}

static VALUE turtleshell_get_gain(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return INT2NUM(rtlsdr_get_tuner_gain(device));
}

static VALUE turtleshell_set_gain(VALUE self, VALUE wrapped_device, VALUE value) {
  int gain = NUM2INT(value);
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  rtlsdr_set_tuner_gain(device, gain);
  return Qnil;
}

static VALUE turtleshell_get_gains(VALUE self, VALUE wrapped_device) {
  int *gains = NULL;
  int length, i;
  rtlsdr_dev_t *device;
  VALUE buffer = rb_ary_new();
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);

  length = rtlsdr_get_tuner_gains(device, NULL);
  gains = malloc(sizeof(int) * length);
  rtlsdr_get_tuner_gains(device, gains);

  for (i = 0; i < length; ++i) {
    rb_ary_push(buffer, INT2NUM(gains[i]));
  }

  free(gains);
  return buffer;
}

static VALUE turtleshell_get_tuner_type(VALUE self, VALUE wrapped_device) {
  VALUE hash;
  int tuner_type;
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);

  hash = rb_const_get(m_rtlsdr, rb_intern("TunerTypes"));
  tuner_type = rtlsdr_get_tuner_type(device);
  return rb_hash_aref(hash, tuner_type);
}

static VALUE turtleshell_set_manual_gain(VALUE self, VALUE wrapped_device, VALUE enabled) {
  int result;
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);

  result = rtlsdr_set_tuner_gain_mode(device, RTEST(enabled));
  return INT2NUM(result);
}

static VALUE turtleshell_get_freq_correction(VALUE self, VALUE wrapped_device) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  return INT2NUM(rtlsdr_get_freq_correction(device));
}

static VALUE turtleshell_set_freq_correction(VALUE self, VALUE wrapped_device, VALUE v) {
  rtlsdr_dev_t *device;
  Data_Get_Struct(wrapped_device, rtlsdr_dev_t, device);
  if (rtlsdr_set_freq_correction(device, NUM2INT(v))) {
      rb_raise(rb_eRuntimeError, "couldn't set PPM");
  }

  return v;
}

void Init_librtlsdr() {
  VALUE tuner_hash;
  m_turtleshell = rb_define_module("TurtleShell");
  m_rtlsdr = rb_define_module_under(m_turtleshell, "RTLSDR");
  c_device = rb_define_class_under(m_rtlsdr, "Device", rb_cObject);

  // count of devices
  rb_define_module_function(m_rtlsdr, "count", turtleshell_count, 0);

  // life cycle of devices
  rb_define_module_function(m_rtlsdr, "first_device", turtleshell_first_device, 0);
  rb_define_module_function(m_rtlsdr, "all_devices", turtleshell_all_devices, 0);
  rb_define_module_function(m_rtlsdr, "nth_device", turtleshell_nth_device, 1);
  rb_define_module_function(m_rtlsdr, "close_device", turtleshell_close, 1);

  // reading bytes
  rb_define_module_function(m_rtlsdr, "read_sync", turtleshell_read_synchronous, 2);
  rb_define_module_function(m_rtlsdr, "read_async", turtleshell_read_asynchronous, 3);

  // getters and setters
  rb_define_module_function(m_rtlsdr, "get_sample_rate", turtleshell_get_sample_rate, 1);
  rb_define_module_function(m_rtlsdr, "set_sample_rate", turtleshell_set_sample_rate, 2);
  rb_define_module_function(m_rtlsdr, "get_center_freq", turtleshell_get_center_frequency, 1);
  rb_define_module_function(m_rtlsdr, "set_center_freq", turtleshell_set_center_frequency, 2);
  rb_define_module_function(m_rtlsdr, "get_gain", turtleshell_get_gain, 1);
  rb_define_module_function(m_rtlsdr, "set_gain", turtleshell_set_gain, 2);
  rb_define_module_function(m_rtlsdr, "get_tuner_gains", turtleshell_get_gains, 1);
  rb_define_module_function(m_rtlsdr, "get_tuner_type", turtleshell_get_tuner_type, 1);
  rb_define_module_function(m_rtlsdr, "set_manual_gain", turtleshell_set_manual_gain, 2);
  rb_define_module_function(m_rtlsdr, "get_freq_correction", turtleshell_get_freq_correction, 1);
  rb_define_module_function(m_rtlsdr, "set_freq_correction", turtleshell_set_freq_correction, 2);

  tuner_hash = rb_hash_new();
  rb_hash_aset(tuner_hash, RTLSDR_TUNER_UNKNOWN, ID2SYM(rb_intern("unknown")));
  rb_hash_aset(tuner_hash, RTLSDR_TUNER_E4000, ID2SYM(rb_intern("e4000")));
  rb_hash_aset(tuner_hash, RTLSDR_TUNER_FC0012, ID2SYM(rb_intern("fc0012")));
  rb_hash_aset(tuner_hash, RTLSDR_TUNER_FC0013, ID2SYM(rb_intern("fc0013")));
  rb_hash_aset(tuner_hash, RTLSDR_TUNER_R820T, ID2SYM(rb_intern("r820t")));
  rb_hash_aset(tuner_hash, RTLSDR_TUNER_R828D, ID2SYM(rb_intern("r828d")));
  rb_define_const(m_rtlsdr, "TunerTypes", tuner_hash);
}
