
local LabelTest = {}

function LabelTest:simpleText(text)
    local lb = Label()
    lb:set_simple_text(text)
    Window.instance().drawing_root:add(lbl)
end

function LabelTest:richText()
    local lbl = Label()
    lbl.x = 100
    lbl.y = 200
    lbl:set_rich_text('<b><i><font size=60 color=#000000 weight=1 glow=#ff0000>Hello </font></i></b><font color=#0000ff>富文本! </font>')
    Window.instance().drawing_root:add(lbl)
end

return LabelTest