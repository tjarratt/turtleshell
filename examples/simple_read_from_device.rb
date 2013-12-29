#!/usr/bin/env ruby
lib = File.expand_path('../lib', __FILE__)
$:.unshift lib unless $:.include? lib

require 'turtleshell'

puts "Found #{TurtleShell.count_of_devices} devices connected"
TurtleShell.all_devices.each_with_index do |device_name, index|
  puts "#{index} : #{device_name}"
end

begin
  sdr = TurtleShell::Device.new
rescue TurtleShell::DeviceNotFoundError
  puts 'No compatible devices found. bailing!'
  exit 1
end

puts "\n\nConfiguring SDR device\n"

sdr.sample_rate = 2.4e6
sdr.center_frequency = 100e6
sdr.gain = 10

puts "\t sample rate:       #{sdr.sample_rate / 1e6} MHz"
puts "\t center frequency:  #{sdr.center_frequency / 1e6} MHz"
puts "\t gain:              #{sdr.gain} dB"

puts "\n\n"
puts "Reading Samples..."
samples = sdr.read_samples(1024)
print "\t signal mean:   #{samples.inject(:+)/samples.length}"

count = 0
puts "\n\nTesting async callbacks..."
sdr.read_samples_async do |buffer|
  count += 1
  puts "\t in async callback with buffer of length #{buffer.length}"
  puts "\t signal mean:   #{buffer.inject(:+)/buffer.length}"

  count == 3 # exit when this has been called three times
end

sdr.close_device
