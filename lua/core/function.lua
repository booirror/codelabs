
handler = function(obj, func)
  return function(...) func(obj, ...) end
end