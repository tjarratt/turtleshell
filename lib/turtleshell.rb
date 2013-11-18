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

module TurtleShell
  def self.all_devices
    TurtleShell::RTLSDR.all_devices
  end

  def self.count_of_devices
    TurtleShell::RTLSDR.count
  end
end
