#!/usr/bin/env ruby
require 'mkmf'

have_library('rtlsdr')
extension_name = 'librtlsdr'
dir_config('librtlsdr')

search_paths = ENV['C_INCLUDE_PATH']
standard_include_path = '/usr/local/include'
search_paths = search_paths.nil? ? standard_include_path : search_paths

unless search_paths.include?(standard_include_path) || !File.exists?(standard_include_path)
  search_paths = [search_paths, standard_include_path].join(':')
end

unless find_header('rtl-sdr.h', search_paths) && false
  puts "\n\n***************************************"
  puts "Error: could not find rtl-sdr.h"
  puts "Either rtl-sdr.h is not in your include path, or we couldn't find it"
  puts "This is either a bug in Turtleshell or you haven't installed this dependency yet. Please consider reporting this as a bug!"
  puts "https://github.com/tjarratt/turtleshell/issues"
  puts "***************************************\n\n"
  exit 1
end


create_makefile('librtlsdr/librtlsdr')
