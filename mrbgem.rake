MRuby::Gem::Specification.new('mruby-bin-mhelloworld') do |spec|
  spec.bins = ['mhelloworld']
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'

  path = File.expand_path('../mruby-libbpf', File.dirname(__FILE__))
  if File.exist? path
    spec.add_dependency 'mruby-libbpf', path: path
  else
    spec.add_dependency 'mruby-libbpf', github: 'intrared-rb/mruby-libbpf'
  end
  spec.add_dependency 'mruby-sleep'

  def spec.generate_bpf_files
    bpfdir = File.expand_path('./bpf', File.dirname(__FILE__))
    srcdir = File.expand_path('./src', File.dirname(__FILE__))

    file "#{bpfdir}/helloworld.bpf.c"

    file "#{bpfdir}/vmlinux.h" do
      puts "bpftool\tbtf dump file /sys/kernel/btf/vmlinux format c > #{bpfdir}/vmlinux.h"
      begin
        sh "bpftool btf dump file /sys/kernel/btf/vmlinux format c > #{bpfdir}/vmlinux.h"
      rescue => e
        sh "rm -f #{bpfdir}/vmlinux.h"
        raise e
      end
    end

    file "#{bpfdir}/helloworld.bpf.o" => ["#{bpfdir}/helloworld.bpf.c", "#{bpfdir}/vmlinux.h"] do
      puts "clang\t-O2 -c -g -target bpf #{bpfdir}/helloworld.bpf.c -o #{bpfdir}/helloworld.bpf.o"
      sh "clang -O1 -c -g -target bpf #{bpfdir}/helloworld.bpf.c -o #{bpfdir}/helloworld.bpf.o"
    end

    headers = []

    file "#{srcdir}/helloworld.bpf.h" => "#{bpfdir}/helloworld.bpf.o" do
      puts "bpftool\tgen skeleton #{bpfdir}/helloworld.bpf.o > #{srcdir}/helloworld.bpf.h"
      begin
        sh "bpftool gen skeleton #{bpfdir}/helloworld.bpf.o > #{srcdir}/helloworld.bpf.h"
      rescue => e
        sh "rm -f #{srcdir}/helloworld.bpf.h"
        raise e
      end
    end
    headers << "#{srcdir}/helloworld.bpf.h"

    if File.exist? "#{bpfdir}/types.h"
      file "#{srcdir}/types.h" => "#{bpfdir}/types.h" do
        sh "cp #{bpfdir}/types.h #{srcdir}/types.h"
      end
      headers << "#{srcdir}/types.h"
    end

    c_codes = Dir.glob("#{srcdir}/*.c")
    file c_codes[0] => headers
  end

  spec.generate_bpf_files
end
