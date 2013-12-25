#!/usr/bin/env ruby
lib = File.expand_path('../lib', __FILE__)
$:.unshift lib unless $:.include? lib

require 'turtleshell'

puts "Found #{TurtleShell.count_of_devices} devices connected"
TurtleShell.all_devices.each_with_index do |device_name, index|
  puts "#{index} : #{device_name}"
end

begin
  sdr = TurtleShell::Device.new(0)
rescue TurtleShell::DeviceNotFoundError
  puts 'No compatible devices found. bailing!'
  exit 1
end

puts "\n\nConfiguring SDR device\n"

sdr.sample_rate = 2.4e6
sdr.center_frequency = 100e6
sdr.gain = 10

puts "\t sample rate:       #{sdr.sample_rate} MHz"
puts "\t center frequency:  #{sdr.center_frequency} MHz"
puts "\t gain:              #{sdr.gain} dB"

puts "\n\n"
puts "Reading Samples..."
samples = sdr.read_samples(1024)
print "\n\t signal mean:   #{samples.inject(:+)/samples.length}"


puts "\n\n"
puts "\tTesting async callbacks..."
sdr.read_samples_async do |buffer, rtlsdr|
  puts '\t in async callback'
  puts "\t signal mean:   #{buffer.inject(:+)/buffer.length}"
end

sdr.close
