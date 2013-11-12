#!/usr/bin/env ruby
lib = File.expand_path('../lib', __FILE__)
$:.unshift lib unless $:.include? lib

require 'turtleshell'

sdr = TurtleShell::SDR.new

puts 'Configuring SDR device'

sdr.sample_rate = 2.4e6
sdr.center_frequency = 100e6
sdr.gain = 10

puts "\n\n"
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
