--圆形头像款(可以改成传递文件路径)
function display.createCircleSprite(srcFile, maskFile)
    -- local src = display.newSprite(srcFile)
    -- local mask = display.newSprite(maskFile)
    
    local src = srcFile
    local mask = maskFile
    --display.newSprite("ccbResources/common/DT_tiuxiangd.png")
    local size_src = src:getContentSize()
    local size_mask = mask:getContentSize()
    local canva = cc.RenderTexture:create(size_mask.width, size_mask.height, cc.TEXTURE2_D_PIXEL_FORMAT_RGB_A8888)      
    local ratiow = size_mask.width / size_src.width  
    local ratioh = size_mask.height / size_src.height  
    src:setScaleX(ratiow)  
    src:setScaleY(ratioh)  
    mask:setPosition(size_mask.width / 2, size_mask.height / 2)  
    src:setPosition(size_mask.width / 2, size_mask.height / 2)  
    local blendfunc_mask = cc.blendFunc(gl.ONE, gl.ZERO)  
    mask:setBlendFunc(blendfunc_mask)  
    local blendfunc_src = cc.blendFunc(gl.DST_ALPHA, gl.ZERO)  
    src:setBlendFunc(blendfunc_src)  
    canva:begin()  
    mask:visit()  
    src:visit()  
    canva:endToLua()  
    local masked_sprite = cc.Sprite:createWithTexture(canva:getSprite():getTexture())  
    masked_sprite:setFlippedY(true)  
    return masked_sprite  
end