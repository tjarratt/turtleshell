module TurtleShell

  class DeviceNotFoundError < Exception; end

  class Device
    DEFAULT_ASYNC_BUFFER = 32
    DEFAULT_READ_SIZE = 1024

    # initializers
    def initialize(n)
      raise ArgumentError.new('TurtleShell::Device.new expects a number') unless n.is_a? Fixnum

      unless @device = TurtleShell::RTLSDR::nth_device(n)
        raise TurtleShell::DeviceNotFoundError
      end
    end

    def self.nth_device(n)
      count = TurtleShell.count_of_devices
      unless n < count
        raise ArgumentError.new('Index out of bounds')
      end

      new(n)
    end

    def self.device_with_name(name)
      unless index = TurtleShell.all_devices.index(name)
        raise ArgumentError.new('No device with that name')
      end

      new(index)
    end

    # attributes
    def name
      @device ? @device[:name] : 'No device found'
    end

    def sample_rate(rate = nil)
      return @device[:sample_rate] if rate.nil?

      @device[:sample_rate] = rate
    end

    def center_frequency(freq = nil)
      return @device[:center_frequency] if freq.nil?

      @device[:center_frequency] = freq
    end

    def gain(value = nil)
      return @device[:gain] if value.nil?

      @device[:gain] = value
    end

    # read specified number of complex samples from tuner
    # real and imaginary parts are normalized between [-1, 1]
    def read_samples(number_of_samples = RtlSDR::DEFAULT_READ_SIZE)
      number_of_bytes = 2 * number_of_samples
      raw_data = self.read_bytes(number_of_bytes)
      self.packed_bytes_to_complex(raw_data)
    end

    private
    def read_bytes(number_of_bytes)
      @buffer = Array.new(number_of_bytes)
      @bytes_read = 0

      result = rtlsdr_read_sync(@device, @buffer, number_of_bytes, @bytes_read)

      if result < 0
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
