#!/usr/bin/env ruby
lib = File.expand_path('../../lib', __FILE__)
$:.unshift lib unless $:.include? lib

require 'turtleshell'

begin
  sdr = TurtleShell::Device.new
rescue TurtleShell::DeviceNotFoundError
  puts 'No compatible devices found. bailing!'
  exit 1
end


sdr.sample_rate = 2.4e6
sdr.center_frequency = 100e6
sdr.gain = 8

count = 0
puts "\n\nTesting async callbacks..."
sdr.read_samples_async(512) do |buffer|
  count += 1
  puts "\t in async callback with buffer of length #{buffer.length}"
  puts "\t signal mean:   #{buffer.inject(:+)/buffer.length}"

  count == 3 # exit when this has been called three times
end

sdr.close_device
