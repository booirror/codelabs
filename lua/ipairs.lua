
local function test()
  local t = { [0] = "123",  [1] = "12", [4] = "1", ["0"] = "ab"}
  
  for k,v in pairs(t) do
    print(k, v)
  end
  
  print("------------")
  
  for k, v in ipairs(t) do
    print(k, v)
  end
end

local function main()
  test()
end

main()

define 'abc' {key=3}

