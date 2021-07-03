gd={}


function gd:new(width, height,quality,color)
	local obj = {}
		obj.width = width or 120
		obj.height = height or 120
		obj.quality = quality or 50
		obj.gd = nil
		obj.defFont = {}
		obj.background = color or images.gdRandColor()


--
	function obj:getSize()
		return obj.width,obj.height
	end
	function obj:getQuality()
		return obj.quality
	end
--
	function obj:getImageData(q)
			q = q or obj.quality
			return images.getImageData(obj.gd, q)
	end
	function obj:getRawImage(imgdata,q)
			q = q or obj.quality
			imgdata = imgdata or obj:getImageData(q)
			c = images.getImageDataRaw(imgdata)
			s = images.getImageDataSize(imgdata)
			return c,s
	end
	function obj:saveToFile(path)
		images.writeToFile(obj.gd, path, obj.quality)
	end
--
	function obj:setQuality(q)
		obj.quality = q
	end
	function obj:setGD(gd)
		obj:clear()
		obj.gd = gd
	end
--constructor
	function obj:reopenFromFile(width,height,path)
		obj:clear()
		--todo: get with and height from file
		obj.width=width
		obj.height=height
		obj.gd = obj:initImageFromFile(path,width,height)
		return obj
	end
--font
	function obj:setDefFont(path)
		obj.defFont = path
	end
--init functions
	function obj:initImage(background)
		background = background or obj.background
		obj.gd=images.gdInitImage( obj.width, obj.height, background)
		return true
	end
	function obj:initImageFromFile(path, width, height)
		if not obj.gd == nil then
			print("obj: Firstly free old image")
			return nil
		end
		obj.gd = images.gdInitImageFromFile(path,width,height)
		return true
	end
--colors
	function obj:getColorFromArray(color)
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		--("Color: ", color['red'], ' ', color['green'], ' ', color['blue'], ' ', color['alpha'])
		return images.gdInitColor(obj.gd, color)
	end

	function obj:getRandColor()
		local color1 = images.gdRandColor()
		return obj:getColorFromArray(color1)
	end

	function obj:getColorArray(red,green,blue,alpha)
		local color = {}
		color["red"] = red or 0
		color["green"] = green or 0
		color["blue"] = blue or 0
		color["alpha"] = alpha or 255	
		return color
	end

	function obj:getColor(red,green,blua,alpha)
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		color = obj:getColorArray(red,green,blue,alpha)
		
		return obj:getColorFromArray(color)
	end

	function obj:addColor(name,red,green,blue,alpha)
		name = name or nil
		if name == nil then return nil end
		red = red or 0
		green = green or 0
		blue = blue or 0
		alpha = alpha or 255
		--print("obj.getColor")
		local c = obj:getColorArray(red,green,blue,alpha)
		--print("gdAddColor")
		images.gdAddColor(obj.gd,name,c)
		return true
	end
	function obj:foundColor(name)
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		return images.gdFoundColor(obj.gd,name)
	end	
	-- need color from obj.getColor(...)
	function obj:setColor(name, n, color)
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		if n == nil or name == nil then return nil end
		images.gdSetColor( obj.gd, name, n, color )
	end
--draw
	function obj:drawPixel(x,y,color)
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		images.gdSetPixel(obj.gd, x,y, color)
		return true
	end
	function obj:drawRect(x,y,x1,y1,color)
		x = x or 0
		y = y or 0
		x1 = x1 or obj.width
		y1 = y1 or obj.height
		color = color or obj:getColor()
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		images.gdDrawRect(obj.gd, x,y, x1, y1, color)
		return true
	end
	function obj:drawLine(x,y,x1,y1,color)
		x = x or 0
		y = y or 0
		x1 = x1 or obj.width
		y1 = y1 or obj.height
		color = color or obj:getColor()
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		images.gdDrawLine(obj.gd, x,y, x1,y1, color)
		return true
	end
	function obj:drawText(text, x,y, color, ptSize, angle, fontPath)
		x = x or 0
		y = y or 0
		fontPath = fontPath or obj.defFont
		angle = angle or 0.0
		color = color or obj:getColor()
		text = text or ""
		if obj.gd == nil then
			print("obj: Firstly init image")
			return nil
		end
		images.gdWriteText(obj.gd, x,y, fontPath, ptSize, angle, color, text)
	end
--random draw
	function obj:drawRandomPixels(min,max)
		if obj.gd == nil then return nil end
		min = min or 100
		max = max or 300
		images.gdDrawRandomPixels(obj.gd, min, max)
	end
	function obj:drawRandomLines(count)
		count = count or {}
		if obj.gd == nil then return nil end
		images.gdDrawRandomLines(obj.gd, count)
		return true
	end
--random text
	function obj:getRandStr(s)
		s = s or 5
		return images.gdGetRandStr(s)
	end
--clear

	function obj:clear()
		if not obj.gd == nil then
			images.gdFree(obj.gd)
			obj.gd=nil
			return true
		end 
		return nil
	end
	

	return obj
end



return gd
