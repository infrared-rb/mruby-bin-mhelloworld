# mruby-bin-mhelloworld   [![Build Status](https://travis-ci.org/udzura/mruby-bin-mhelloworld.svg?branch=master)](https://travis-ci.org/udzura/mruby-bin-mhelloworld)
BinMhelloworld class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'udzura/mruby-bin-mhelloworld'
end
```
## example
```ruby
p BinMhelloworld.hi
#=> "hi!!"
t = BinMhelloworld.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
