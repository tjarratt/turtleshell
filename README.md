[![Build Status](https://travis-ci.org/tjarratt/turtleshell.png)](https://travis-ci.org/tjarratt/turtleshell) turRTLshell
============

Overview
--------
Turtleshell is a ruby wrapper around librtlsdr, a C library for interfacing with Realtek RTL2832 DVB USB devices. More information about librtlsdr can be found [on their website](http://sdr.osmocom.org/trac/wiki/rtl-sdr).

Usage
-----
Turtleshell is available on rubygems. You can install it with `gem install turtleshell` or by adding it to your `Gemfile`.

Most of the key functions in librtlsdr are available, more will be added as development continues.

Examples
--------
Getting a reference to a device and reading from it could not be easier
```ruby
require 'turtleshell'
sdr = TurtleShell::Device.new

# configure device properties
sdr.sample_rate = 2.4e6
sdr.center_frequency = 100e6
sdr.gain = 10

# read samples from the device
samples = sdr.read_samples
puts "signal mean:   #{samples.inject(:+)/samples.length}"
```

More examples are available in the [examples directory](https://github.com/tjarratt/turtleshell/tree/master/examples).

Dependencies
------------
* ruby (version 1.9.2 or later)
* librtlsdr

License
-------
Turtleshell is released under the permissive MIT license.

Credit
------
The idea for this project came to me when I was watching [@0xabad1dea's](https://github.com/0xabad1dea) talk at Defcon 21. Without that initial idea and inspiration, this wouldn't exist!

Credit is due to [@roger-](https://github.com/roger-) for writing pyrtlsdr, which provided a great reference when working on Turtleshell.
