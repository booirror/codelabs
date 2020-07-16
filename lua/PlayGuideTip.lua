PlayGuideTip = class(Node)
require("core/dict")
PlayGuideTip.m_Instance = nil;

PlayGuideType = {
    --四川
    OutCard = 1,           -- 出牌
    ExplicitGang = 2,      -- 明杠
    ImplicitGang = 3,      -- 暗杠
    Hu           = 4,      -- 胡牌
    Change3Card = 10,
    DingQue      = 11,     -- 定缺
    QiangGangHu = 12,      --抢杠胡
    ChaDaJiao    = 13,     --查大叫,查花猪

    --二人
    Enter = 30,            --入场
    Ting = 31,             --听牌
    Ming = 32,                --明牌

}

--global function
function g_triggerPlayGuide(guideType, isAutoHide, time)
    if PlayGuideTip then
		if isAutoHide then
			PlayGuideTip.getInstance():triggerWithAutoHide(guideType, time)
		else
			PlayGuideTip.getInstance():trigger(guideType)
		end
	end
end

function g_hidePlayGuide()
	if PlayGuideTip then
		PlayGuideTip.getInstance():hide()
	end
end

----------------------------------
function PlayGuideTip:ctor()
    PlayGuideTip.m_Instance = self

    self.guideValue = {}
    self.hiding = false
    self.m_anim = nil
    self.nextTip = nil

    self.m_teachLayer 		= new(Image , "room/playtip_bg.png",nil,nil,20,20,20,20);
	self.m_teachLayer:setSize(424,144)
    self.m_teachLayer:setVisible(false)
    self:addChild(self.m_teachLayer)

    local playerHeadIcon 	= new(Image , "room/cartoon.png");
    playerHeadIcon:setAlign(kAlignRight)
	playerHeadIcon:setPos(0 , 0);
	self.m_teachLayer:addChild(playerHeadIcon);

    self.m_teachTip = new(TextView , "", 285, 100, kAlignLeft, nil, 26, 0x55, 0x55, 0x55);
	self.m_teachTip:setPos(20,12);
	self.m_teachLayer:addChild(self.m_teachTip);

    self:readData()

    self.screenW = System.getScreenWidth() / System.getLayoutScale();
    self.screenH = System.getScreenHeight() / System.getLayoutScale();

    local w , h = self.m_teachLayer:getSize()

    self.centerX = self.screenW*0.75
    self.centerY = self.screenH*0.5 - h/2
end

function PlayGuideTip:dtor()
    self.hiding = false
    self.m_anim = nil
    self.nextTip = nil
end

function PlayGuideTip.getInstance()
    return PlayGuideTip.m_Instance
end

function PlayGuideTip:setCenterPos(x, y)
    self.centerX, self.centerY = x, y
end

function PlayGuideTip:saveGuided(guideType, val)
    self.guideValue[guideType] = val
    local set = new(Dict, "playGuideData")
	set:load()
    local id = MyUserData:getId()
    set:setBoolean("PGT" .. tostring(MyUserData:getId()) .. "_" .. guideType, val)
    set:save()
end

function PlayGuideTip:isGudie(guideType)
    return MyUserData:getLevel() >= 5 or self.guideValue[guideType]
end

function PlayGuideTip:readData()
    local set = new(Dict, "playGuideData")
	set:load()
    for k, v in pairs(PlayGuideType) do
        self.guideValue[v] = set:getBoolean("PGT" .. tostring(MyUserData:getId()) .. "_" .. v, false)
    end
end

function PlayGuideTip:setTipText(text)
    self.m_teachTip:setText(text)
end

function PlayGuideTip:onDefaultGuide(guideType)
    local text = self.m_GuideText and self.m_GuideText[guideType]
    if text == nil then return false end
    self:setTipText(text)
    return true
end

function PlayGuideTip:trigger(guideType)
    self:show(guideType, false, nil)
end

function PlayGuideTip:triggerWithAutoHide(guideType, duration)
    self:show(guideType, true, duration)
end

function PlayGuideTip:show(guideType, autoHide, duration)
    if self:isGudie(guideType) then
        -- return
    end
    self:saveGuided(guideType, true)

    if self.m_teachLayer == nil then return end
    if self.m_teachLayer:getVisible() or self.hiding then
        self.nextTip = {guideType, autoHide, duration}
        self:hide()
        return
    end
    -- setText
    if self.m_GuideFuncMap[guideType] then
        if not self.m_GuideFuncMap[guideType](self, guideType) then
            return
        end
    else
        if not self:onDefaultGuide(guideType) then
            return
        end
    end

    if autoHide then
        local anim = AnimFactory.createAnimDouble(kAnimNormal, 0, 1, duration or 3000, 0)
        anim:setEvent(nil, function()
            self:hide()
        end)
        if anim then
            self.m_anim = anim
        end
    end

    self.m_teachLayer:setVisible(true)
    local w, h = self.m_teachLayer:getSize();
    self.m_teachLayer:setPos(self.centerX - w/2, self.centerY - h/2)
    local px, py = self.m_teachLayer:getPos()
    local dx = self.screenW - px
    self.m_teachLayer:addAnimTranslateEase(kAnimNormal, 500, dx, 0, 0, 0)
end


function PlayGuideTip:hide()
    if self.m_teachLayer == nil or self.hiding or not self.m_teachLayer:getVisible() then
        return
    end
    self.hiding = true
    if self.m_anim then
        delete(self.m_anim)
        self.m_anim = nil
    end
    local offsetX = 20
    local px, py = self.m_teachLayer:getPos()
    local newpx = self.screenW + offsetX
    self.m_teachLayer:setPos(newpx, py)
    local dx = px - newpx
    self.m_teachLayer:addAnimTranslateEase(kAnimNormal, 500, dx, 0, 0, 0, function()
        printInfo("***--**self.m_teachLayer:setVisible(false)")
        self.m_teachLayer:setVisible(false)
        self.hiding = false
        if self.nextTip then
            self:show(unpack(self.nextTip))
            self.nextTip = nil
        end
    end)
end

PlayGuideTip.m_GuideFuncMap = {

}

return PlayGuideTip