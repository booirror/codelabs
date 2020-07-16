
t = {3, 4, 5}

local ipairsIter = function(t, i)
  i = i + 1
  local v = t[i]
  if v then
    return i, v
  end
end

local function myIpairs(t)
  return ipairsIter, t, 0
end

for k, v in myIpairs(t) do
  print(k, v)
end

print("++++++++++++++++++++++++++++")

local pairsIter = function(t, key)
  return next(t, key)
end

local myPairs = function(t)
  return pairsIter, t, nil
end

local t = {a = "hello", b = "world", c = "lua"}
for k, v in myPairs(t) do
  print(k, v)
end


