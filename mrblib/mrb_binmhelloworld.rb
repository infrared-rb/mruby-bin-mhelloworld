class BinMhelloworld
  def bye
    self.hello + " bye"
  end
end

def __main__(argv)
  if ARGV.empty?
    pid = -1
  else
    pid = ARGV[0].to_i
  end
  builder = HelloworldBuilder.new(pid)
  builder.load
  builder.attach

  puts "*** collecting vfs_read() kprobe for 5 seconds..."
  sleep 5

  map = builder.hist_map
  key = nil
  while key = map.next_key(key)
    data_ = map.lookup(key)
    data  = data_.unpack("I15")
    puts "Pid: #{key.unpack("I")[0]}"
    data.each_with_index do |d, i|
      if i == 0
        bin = 0..0
      else
        bin = (2**(i-1))..(2**i - 1)
      end
      puts "  bytes%-14s = %3d %s" % [
             "[#{bin.inspect}]",
             d,
             '*' * [d, 16].min
           ]
    end
    puts
  end
end
