#!/usr/bin/env ruby
lib = File.expand_path('../../lib', __FILE__)
$:.unshift lib unless $:.include? lib

require 'turtleshell'

puts "Found #{TurtleShell.count_of_devices} devices connected"

TurtleShell.all_devices.each_with_index do |device_name, index|
  puts "#{index} : #{device_name}"
end
