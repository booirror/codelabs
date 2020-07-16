function value(t)
  local i = 0
  return function()
    i = i + 1
    if i <= #t then
      return i, t[i]
    else
      return nil 
    end
  end
end

local tb = {3, 4 , 6}

 
for key, val in value(tb) do
  print(key, val)
end

lib = {}
lib.foo = function(x, y) 
  return x + y
end

lib = {foo = function(x, y) return x + y end}

lib = {}

function lib.foo(x,y)
  return x + y
end

t = {3, 4, 22, 1, 9}

table.sort(t, function(x, y)
  return x < y
end)

print(table.concat(t, ", "))




