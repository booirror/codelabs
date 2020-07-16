--[[
    炒股神器
]]
-- require("EngineCore/byui")
-- local ui = require("byui.basic")
require("EngineCore.animation")
local Anim = require("animation")
local HttpTest = {}

HttpTest.cmd = {
    pullwave = 1,
    yys = 2,
    wzry = 3,
}

local kScaleVal = 100

HttpTest.pos = {}
HttpTest.keyword = {}

function HttpTest:touch(finger_action,x,y,drawing_id)
    -- print_string("x=" .. self.sp.bbox.x .. ",w=" .. self.sp.bbox.w)
end

function HttpTest:run()
    local n = new(Node)
    n:addToRoot()
    self.root = n;

    local w = Widget()
    Window.instance().drawing_root:add(w)
    self.newNode = w
    
    local draw = new(Node)
    draw:addTo(self.root)
    self.drawNode = draw

    local lb = Label()
    lb:set_rich_text("<font size=30 color=#00ff00>社交网络讨论量</font><font size=14 color=#ffff00> (powered by babe3.0)</font>")
    Window.instance().drawing_root:add(lb)

    for y = 700, 0, -100 do
        self:addLabel("_" .. ((700-y)*kScaleVal), nil, 0, y, false, true, self.root)
    end

    
    local choiceBtn = UIFactory.createButton("lobby/battlebtn.png")
    choiceBtn:setSize(60, 30)
    choiceBtn:addTo(n)
    -- choiceBtn:setSize(296, 352)
    choiceBtn:setAlign(kAlignLeft)
    choiceBtn:setPos(1050, 20);
    choiceBtn:setOnClick(nil, function()
        local s1 = self.edit1:getText()
        local s2 = self.edit2:getText()
        print_string(string.format("[][%s][%s]", s1, s2))
        if s1 == ' ' then s1 =nil end
        if s2 == ' ' then s2 =nil end
        self.drawNode:removeAllChildren()
        self.newNode:remove_all()
        self:query(s1, s2)
    end)

    self:addLabel("查询", nil, 20, 20, false, true, choiceBtn)


    local t = {
        string = "",
        name = "edit",
        width = 100,
        height = 50,
        textAlign = kAlignLeft,
        fontSize = 16,
        colorRed = 0xff,
        colorGreen = 0,
        colorBlue = 0,
        x = 800,
        y = 20,
        nodeAlign = kAlignLeft,
        visible = 1,
    }
    local node = new(EditText,t.string,t.width,t.height,t.textAlign or t.align,"",t.fontSize,t.colorRed,t.colorGreen,t.colorBlue);
	node:setHintText("输入关键词1", 0xff, 0, 0xff)
    node:setName(t.name or "");
	node:setPos(t.x,t.y);
	node:setAlign(t.nodeAlign);
	node:setVisible(t.visible==1 and true or false);
    node:addTo(self.root)
    self.edit1 = node

    local t = {
        string = "",
        name = "edit",
        width = 100,
        height = 50,
        textAlign = kAlignLeft,
        fontSize = 16,
        colorRed = 0xff,
        colorGreen = 0,
        colorBlue = 0,
        x = 900,
        y = 20,
        nodeAlign = kAlignLeft,
        visible = 1,
    }
    local node = new(EditText,t.string,t.width,t.height,t.textAlign or t.align,"",t.fontSize,t.colorRed,t.colorGreen,t.colorBlue);
	node:setHintText("输入关键词2", 0xff, 0, 0xff)
    node:setName(t.name or "");
	node:setPos(t.x,t.y);
	node:setAlign(t.nodeAlign);
	node:setVisible(t.visible==1 and true or false);
    node:addTo(self.root)
    self.edit2 = node
    -- node:setOnTextChange(nil, function(obj, str)
    --     node:setText(str)
    -- end)

    -- local sp = Sprite()
    -- local unit = TextureUnit(TextureCache.instance():get('lobby/battlebtn.png'))
    -- sp.unit = unit
    -- sp.size = unit.size
    -- sp.pos = Point(100, 100)
    -- Window.instance().drawing_root:add(sp)
    -- self.sp = sp
    
    -- Window.instance().drawing_root:add(btn)

    HttpTest:register();
	HttpTest:query()
end

function HttpTest:register()
    EventDispatcher.getInstance():register(HttpManager.s_event,self,self.onHttpResponse);
    -- EventDispatcher.getInstance():register(Event.RawTouch,self,self.touch);
end

function HttpTest:unregister()
    EventDispatcher.getInstance():unregister(HttpManager.s_event,self,self.onHttpResponse);
end

function HttpTest:query(keyword1, keyword2)
    if keyword1 == nil and keyword2 == nil then
        keyword1 = "阴阳师"
        keyword2 = "王者荣耀"
    end
    if keyword1 then
        HttpTest:requestPullWave(HttpTest.cmd.yys, keyword1, nil, 0, 700)
    end
	if keyword2 then
        HttpTest:requestPullWave(HttpTest.cmd.wzry, keyword2, nil, 620, 700)
    end
end

function HttpTest:requestPullWave(cmd, keyword, time, x, y)

    if not cmd then cmd = HttpTest.cmd.pullwave end
    if x and y then
        self.pos[cmd] = {x = x, y = y}
    end
    self.keyword[cmd] = keyword
    if time == nil then
        local d = os.date("*t")
        time = string.format("%d-%d-%d", d.year, d.month, d.day)
    end
    local url = "http://www.pullwave.com/get.php?json=1&auth_usr=free_vip&w1=" .. keyword .. "&w2=" .. time;

    HttpModule.getInstance():executeDirectGet(cmd, url)
end

function HttpTest:onPullWave(cmd, errcode, data, resultStr)
    if resultStr == nil then return end
    print_string("onResponse, cmd=" .. cmd)
    resultStr = json.decode(resultStr)
    for k, item in pairs(resultStr) do
        print_string("k=" .. k)
    end
    local trend = resultStr.qushi
    local count = #trend

    local width = 600/count
    print_string("count=" .. count)
    -- for k, item in ipairs(trend) do
    --     print_string("k=" .. k .. "item=" ..json.encode(item))
    -- end
    local p = self.pos[cmd]
    local sx = p and p.x or 100
    local sy = p and p.y or 700
    for k, item in ipairs(trend) do
        dump(item)
        local date = item.date
        local v = tonumber(item.v)
        v = v / kScaleVal
        local w = self:addItem(width-1, v)
        -- w:align(kAlignLeftTop)        
        w.pos = Point(sx+k*width, sy-v)

        if k == 1 then
            self:addLabel(date, nil, sx+k*width, sy+2, false)
        elseif k == count then
            self:addLabel(date, nil, sx+k*width, sy+2, true)
        end
    end
    self:addLabel(self.keyword[cmd], 24, sx, 100, false)
end

function HttpTest:scaleEase(animType, duration, w, endCallback)
    local move = Anim.keyframes{
        {0, {scale = Point(1, 0)}, Anim.pow3_out},
        {1.0, {scale = Point(1, 1)}, nil},
    }
    local dt = duration / 1000
    local anim = Anim.Animator(Anim.scale_duration(dt, move), Anim.updator(w), animType)
    anim.on_stop = endCallback
    anim:start()
end

function HttpTest:addItem(width, height)
    
    local w = Widget()
    w.size = Point(width, height)
    w.background_color = Colorf(0xff, 0, 0)
    self.newNode:add(w)
    w.anchor = Point(0.5, 1)
    w.scale_at_anchor_point = true
    self:scaleEase(kAnimNormal, 1100, w)
    
    -- w.lua_on_enter = function()
    --     print_string("print enter")
    -- end
    -- local w = UIFactory.createImage("ui/a.png")
    -- w:setColor(0xff, 0, 0)
    -- w:setSize(width, height)
    -- w:addTo(self.root)
    return w
end

function HttpTest:addLabel(name, size, x, y, alignRight, alignBottom, root)
    -- local lb = Label()
    -- size = size or 12
    -- lb:set_rich_text("<font size=" .. size .. " color=#00ff00>" .. name .. "</font>")
    -- lb.x = x or 0
    -- lb.y = y or 0
    -- -- lb:set_color(Colorf(0xff, 0, 0))
    -- if alignRight then
    --     lb.align_h = Label.RIGHT
    -- else
    --     lb.align_h = Label.LEFT
    -- end
    -- if alignBottom then
    --     lb.align_v = Label.BOTTOM
    -- else
    --     lb.align_v = Label.TOP
    -- end
    -- Window.instance().drawing_root:add(lb)

    -- old


    local btnText = UIFactory.createText(name, size or 12, 0, 0, kAlignCenter, 0, 0xff, 0);
    local w, h = btnText:getSize()
    if alignRight then
        x = x - w 
    end
    if alignBottom then
        y = y - h
    end
    btnText:align(kAlignLeftTop)
    :pos(x, y)
    btnText:addTo(root or self.drawNode)
    return btnText
end

function HttpTest:onHttpResponse(cmd, errcode, data, resultStr)
    if self.cmdCallback[cmd] then
        self.cmdCallback[cmd](self, cmd, errcode, data, resultStr)
    end
end

HttpTest.cmdCallback = {
    [HttpTest.cmd.pullwave] = HttpTest.onPullWave,
    [HttpTest.cmd.yys] = HttpTest.onPullWave,
    [HttpTest.cmd.wzry] = HttpTest.onPullWave,
}

return HttpTest