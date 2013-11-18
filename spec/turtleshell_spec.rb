require 'turtleshell'

describe 'TurtleShell' do
  it 'tells you how many devices are connected' do
    TurtleShell::RTLSDR.
      should_receive(:count).
      and_return(886)

    expect(TurtleShell.count_of_devices).to eq(886)
  end

  it 'tells you which devices are at which index' do
    TurtleShell::RTLSDR.
      should_receive(:all_devices).
      and_return(['guilt-ridden-humpback-whale', 'four-ply-self-repellency', 'lute-backed-cross-stone'])

    names = TurtleShell.all_devices
    expect(names[0]).to eq('guilt-ridden-humpback-whale')
    expect(names[1]).to eq('four-ply-self-repellency')
    expect(names[2]).to eq('lute-backed-cross-stone')
  end
end
