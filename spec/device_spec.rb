require 'turtleshell'

describe 'Turtleshell:Device interfaces' do
  it 'should return the name' do
    TurtleShell::RTLSDR.
      should_receive(:first_device).
      and_return({:name => 'slippery-slope'})

    expect(TurtleShell::Device.new.name).to eq('slippery-slope')
  end
end
