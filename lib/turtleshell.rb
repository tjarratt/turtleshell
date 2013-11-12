module TurtleShell
end

Dir.entries(Dir.pwd).select do |f|
  File.directory?(f) && f[0] != '.'
end.each do |dir|
  lib = File.expand_path("../#{dir}", __FILE__)
  $:.unshift lib unless $:.include? lib
end

require 'librtlsdr/librtlsdr'
require 'turtleshell/device'
require 'turtleshell/sdr'
