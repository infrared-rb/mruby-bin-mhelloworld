MRuby::Gem::Specification.new('mruby-bin-mhelloworld') do |spec|
  spec.bins = ['mhelloworld']
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'

  spec.linker.libraries << 'bpf'
  spec.add_dependency 'mruby-sleep'
end
