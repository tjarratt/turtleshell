#!/usr/bin/env ruby
require 'mkmf'
require 'rake/extensiontask'

have_library('rtlsdr')
extension_name = 'librtlsdr'

dir_config(extension_name)
create_makefile(extension_name)

Rake::ExtensionTask.new(extension_name)
