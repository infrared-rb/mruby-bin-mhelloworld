##
## BinMhelloworld Test
##

assert("BinMhelloworld#hello") do
  t = BinMhelloworld.new "hello"
  assert_equal("hello", t.hello)
end

assert("BinMhelloworld#bye") do
  t = BinMhelloworld.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("BinMhelloworld.hi") do
  assert_equal("hi!!", BinMhelloworld.hi)
end
