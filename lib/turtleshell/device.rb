module TurtleShell

  class DeviceNotFoundError < Exception; end

  class Device
    DEFAULT_ASYNC_BUFFER = 32
    DEFAULT_READ_SIZE = 1024

    attr_reader :name

    def initialize(n)
      raise ArgumentError.new('TurtleShell::Device.new expects a number') unless n.is_a? Fixnum

      unless raw_device_attrs = TurtleShell::RTLSDR.nth_device(n)
        raise TurtleShell::DeviceNotFoundError
      end

      @name = raw_device_attrs[:name]
      @device = raw_device_attrs[:device]
    end

    def self.nth_device(n)
      new(n)
    end

    def self.device_with_name(name)
      unless index = TurtleShell.all_devices.index(name)
        raise ArgumentError.new('No device with that name')
      end

      new(index)
    end

    def sample_rate
      TurtleShell::RTLSDR.get_sample_rate(@device)
    end

    def sample_rate=(rate)
      TurtleShell::RTLSDR.set_sample_rate(@device, rate)
    end

    def center_frequency
      TurtleShell::RTLSDR.get_center_freq(@device)
    end

    def center_frequency=(freq)
      TurtleShell::RTLSDR.set_center_freq(@device, freq)
    end

    def gain
      TurtleShell::RTLSDR.get_gain(@device)
    end

    def gain=(gain)
      TurtleShell::RTLSDR.set_gain(@device, gain)
    end

    # read specified number of complex samples from tuner
    # real and imaginary parts are normalized between [-1, 1]
    def read_samples(number_of_samples = RtlSDR::DEFAULT_READ_SIZE)
      number_of_bytes = 2 * number_of_samples
      raw_data = read_bytes(number_of_bytes)
      self.packed_bytes_to_complex(raw_data)
    end

    private
    def read_bytes(number_of_bytes)
      @buffer = Array.new(number_of_bytes)
      @bytes_read = 0

      result = TurtleShell::RTLSDR.read_sync(@device, @buffer, number_of_bytes, @bytes_read)

      if result != 0
        raise IOError.new("Error code #{result} reading #{number_of_bytes} bytes")
      elsif @bytes_read < number_of_bytes
        self.close()
        raise IOError.new("Short read, only read #{@bytes_read} of requested #{number_of_bytes} bytes")
      end

      @buffer
    end

    # converts an array of bytes to a list of complex numbers
    # and normalizes them within the range [-1, 1]
    def packed_bytes_to_complex(bytes)
      bytes.each_slice(2).map! do |i, r|
        Complex(i / 127.5 - 1, q / 127.5 - 1)
      end
    end
  end
end
