--big number multiplication
-- Karatsuba algorithm

local bignum = {}

function bignum.len(num)
    return string.len(num)
end

function bignum.splitAt(num, pos)
    local low = string.sub(num, -pos)
    local high = string.sub(num, 1, -pos - 1)
    if high == '' then high = '0' end
    return high, low
end

function bignum.plus(str1, str2)
    local increase = 0
    local t = {}
    local max = math.max(string.len(str1), string.len(str2))
    for i = 0, max-1 do
        local ch1 = string.sub(str1, -1 - i, -1 - i)
        local ch2 = string.sub(str2, -1-i, -1-i)
        if ch1 == "" then ch1 = "0" end
        if ch2 == "" then ch2 = "0" end
        local newv = increase + ch1 + ch2
        increase = (newv - newv % 10) / 10
        table.insert(t, newv%10)
    end
    if increase > 0 then
        table.insert(t, increase)
    end
    local len = #t
    local tmp
    for i = 1, len/2 do
        tmp = t[i]
        t[i] = t[len-i+1]
        t[len-i+1] = tmp
    end
    return table.concat(t)
end

function bignum.plusEx(...)
    local t = {...}
    local result = t[1]
    for key=2, #t do
        result = bignum.plus(result, t[key])
    end
    return result
end


function bignum.multi(num1, num2)
    local increase = 0
    local t = {}
    local len1, len2 = string.len(num1),  string.len(num2)
    local min = len1 > len2 and len2 or len1
    local minstr = min == len1 and num1 or num2
    local maxstr = minstr == num1 and num2 or num1

    for i = 0, min-1 do
        local val = string.sub(minstr, -1 - i,  -1 - i)
        val = tonumber(val)
        local tb = {}
        for j = 1, i do
            table.insert(tb, 0)
        end
        increase = 0
        for k = 0, string.len(maxstr)-1 do
            local v = string.sub(maxstr, -1-k, -1-k)
            local m = val * v + increase
            local rem = m % 10
            increase = (m - rem) / 10
            table.insert(tb, rem)
        end
        if increase > 0 then
            table.insert(tb, increase)
        end
        table.insert(t, tb)
    end
    
    local tb2reverseStr = function(tbl)
        local len = #tbl
        local tmp
        for i = 1, len/2 do
            tmp = tbl[i]
            tbl[i] = tbl[len-i+1]
            tbl[len-i+1] = tmp
        end
        return table.concat(tbl)
    end
    for k, v in ipairs(t) do
        repeat
            if #v == 1 or v[#v] ~= 0 then
                break
            end
            table.remove(v)
        until #v == 1
        t[k] = tb2reverseStr(v)
    end
    return bignum.plusEx(unpack(t))
end

function bignum.minus(m1, m2)
    local borrow = 0
    local t = {}
    local max = math.max(string.len(m1), string.len(m2))
    for i = 0, max-1 do
        local ch1 = string.sub(m1, -1 -i, -1-i)
        local ch2 = string.sub(m2, -1-i, -1-i)
        if ch2 == "" then ch2 = '0' end
        if ch1 - ch2 - borrow >= 0 then
            table.insert(t, ch1-ch2-borrow)
            borrow = 0
        else
            table.insert(t, ch1+10 - ch2 - borrow)
            borrow = 1
        end
    end
    repeat
        if #t == 0 or t[#t] ~= 0 then
            break
        end
        table.remove(t)
    until #t == 0
    local len = #t
    local tmp
    for i = 1, len/2 do
        tmp = t[i]
        t[i] = t[len-i+1]
        t[len-i+1] = tmp
    end
    return table.concat(t)
end

function bignum.pow(base, val)
    local str = {1}
    for i = 1, val do
        table.insert(str, 0)
    end
    return table.concat(str)
end

function bignum.karatsuba(num1, num2)
    local l1 = bignum.len(num1)
    local l2 = bignum.len(num2)
    if l1 < 10 or l2 < 10 then
        return bignum.multi(num1, num2)
    end
    local m = math.max(l1, l2)
    local m2 = math.floor(m/2)
    local high1, low1 = bignum.splitAt(num1, m2)
    local high2, low2 = bignum.splitAt(num2, m2)
    local z0 = bignum.karatsuba(low1, low2)
    local z1 = bignum.karatsuba(bignum.plus(high1, low1), bignum.plus(high2, low2))
    local z2 = bignum.karatsuba(high1, high2)
    local result = bignum.plusEx(bignum.multi(z2, bignum.pow(10, 2*m2)), bignum.multi(bignum.minus(bignum.minus(z1, z2), z0), bignum.pow(10, m2)), z0)
    return result
end

return bignum

