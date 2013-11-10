#!/usr/bin/env ruby

require 'mkmf'
extension_name = 'librtlsdr'

dir_config(extension_name)
have_library('librtlsdr')
create_makefile(extension_name)
