#!/usr/bin/env ruby
require 'mkmf'

have_library('rtlsdr')
extension_name = 'librtlsdr'

dir_config('librtlsdr')
create_makefile('librtlsdr/librtlsdr')
