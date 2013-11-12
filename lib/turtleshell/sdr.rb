class TurtleShell::SDR
  attr_reader :sample_rate, :center_frequency, :gain

  def initialize
    puts "found #{TurtleShell::RTLSDR::count()} devices"
    raw_device = TurtleShell::RTLSDR::first_device()
    puts raw_device.class.inspect

    puts raw_device['name'] if raw_device

    @device = TurtleShell::Device.new(raw_device)
  end

  def sample_rate=(rate)
    #find the closest value supported by the device
  end

  def center_frequency=(frequency)
    #find the closest value supported by the device
  end

  def gain=(decibels)
    #find the closest value supported by the device
  end

  def read_samples(count)
  end

  def read_samples_async(&block)
  end
end
