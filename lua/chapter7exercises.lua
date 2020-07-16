print("-----exercise 7.1-----")
local function fromto(n, m)
  local i = n
  return function() local x = i; i = i + 1; if x <= m then return x end end 
end

for i in fromto(1, 4) do
  print(i)
end

print("-----exercise 7.2-----")

local function fromto2(n, m, step)
  local i = n
  return function() local x = i; i = i + step; if x <= m then return x end end
end

for i in fromto2(1, 9, 3) do
  print(i)
end

print("-----exercise 7.3-----")

local function uniquewords(filename)
  local file = io.open(filename)
  local line = file:read("*l")
  local pos = 1
  local tab = {}
  return function()
    while line do
      local s, e = string.find(line, "%w+", pos)
      if s then
        pos = e + 1
        local word = string.sub(line, s, e)
        if not tab[word] then
          tab[word] = true
          return word
        end
      else
        line = file:read("*l")
        pos =1
      end
    end
    file:close()
    return nil
  end
end

for word in uniquewords("d:/test.lua") do
  print(word)
end
print("-----exercise 7.4-----")

local function allsubstring(str)
  local s = 1
  local len = string.len(str)
  local e = 0
  return function()
    if s < len then
      e = e + 1
      if e <= len then
        return string.sub(str, s, e)
      else
        s = s + 1
        e = s
        return string.sub(str,s,e)
      end
    end
  end
end

for sub in allsubstring("word") do
  print(sub)
end


