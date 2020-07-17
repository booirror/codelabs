-- setfenv.lua

function newEnv()
    local M = {}
    _G["hello"] = M
    setfenv(1, M)
    world = 3
end

print(hello)
newEnv()
print(hello)
print(hello.world)