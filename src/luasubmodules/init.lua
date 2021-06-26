local submoduledir = "./luasubmodules/"
json = require "json"
sql = require "luasql.postgres"

bacteria = require (submoduledir .. "bacteria")
bacteria_aes = require (submoduledir .. "bencdec")
bacteria.init("cryptocoins.ini",{'tgst','tdash'})
bacteria.dumpCryptocoins()

function extended (child, parent)
    setmetatable(child,{__index = parent}) 
end


function sleep (a) 
    local sec = tonumber(os.clock() + a); 
    while (os.clock() < sec) do 
    end 
end


local function check_leak_memory(fcoin,scoin) 
		tmp=bacteria.doRPCRequest(fcoin, "getbalance", {"*",6})
		tmp1=bacteria.doRPCRequest(scoin, "getbalance", {"*",6})

		local iter=65535 -- check to memory leak --
		--while 1 == 1 do
		while iter > 65535 do
			iter=iter-1
			bacteria.doRPCRequest(tgst, "getbalance", {"*",6})
			bacteria.doRPCRequest(sgst, "getbalance", {"*",6})
		end
		print(tmp1['result'])
		--print(tmp)
		print(tmp['result'])
end 

local function CryptocoinsTest()
	--tgst=cryptocoins.gettable(t,'tgst')
	--tdash=cryptocoins.gettable(t,'tdash')
	tgst = bacteria.coins['tgst']
	tdash = bacteria.coins['tdash']
	check_leak_memory(tgst,tdash)


	-- example in luasubmodules/bencdec.lua
	--key,iv=bacteria_aes.genKeyIV()
end


local function addChar(ch,time)
	ret=""
	while time > 0 do
		ret = ret .. ch
		time = time - 1
	end
	return ret
end

local function checkAllTypes(b,msg)
--	b:encrypt(msg)
--	b:decrypt( b:getAESData_rawEnc() )
--	b:clear()
	for index, data in pairs(AESENCType) do
	    --if data ~= AESENCType["t_chacha20"] then
	    	print(index,data)		
		b:encrypt(msg,data)
		b:decrypt( b:getAESData_rawEnc(),data )
		aesdata_enc,saesdata_enc=b:getAESData_enc()
		b:decrypt(b:getAESData_rawEnc(), data)
		aesdata_dec,saesdata_dec=b:getAESData_dec()
		b:clear()  
		print("Decrypt msg(from local aesdata, from C): ", aesdata_dec)
		print("SetAESData")
		b:setAESData_enc(aesdata_enc,saesdata_enc)
		print("Decrypt")
		b:decrypt(b:getAESData_rawEnc(), data)
		aesdata_dec,saesdata_dec=b:getAESData_dec()
		print("Decrypt msg(from created aesdata, from lua): ", aesdata_dec)
	   -- end

	end

	
end
local function EncDecTest()
	msg="Hello AES_ECB, AES_CBC, ChaCha20! "
	msg=msg .. addChar("s",666)

	b=bacteria_aes.new() --("mysmallkey") -- ("mykey","myiv") 32,16 bytes
	--print("key:", b:getKey(), "IV: ",b:getIV())
	print("AES check!")
	checkAllTypes(b,"is example message aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafasdjasdjkfsdfjasdfjiaodfasdfjiasdijfasidfadfiaojsdijfoasdfiaojsdfiojasdfijasdfuhasdufhasdiufhasidufashdfiasudhfiasudhfiuasdfihuSOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOFSDJKFASDJFASJDFQJAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
	print("\n\n\n\n\n\n\n\n\n\nx25519 + AES check!\n\n\n\n\n\n\n\n\n\n")
	k1 = bacteria_aes:newKeyPair()
	k2 = bacteria_aes:newKeyPair()
	k1:initKeyPair()
	k2:initKeyPair()
	pub1 = k1:getPubKey()
	pub2 = k2:getPubKey()
	priv2 = k2:getPrivKey()


	print("Pub1: ", string.tohex(pub1), string.fromhex(string.tohex(pub1)) == pub1 )
	print("Pub2: ", string.tohex(pub2), string.fromhex(string.tohex(pub2)) == pub2 )

	k3 = bacteria_aes:newKeyPair()
	k3:initKeyPair(pub2,priv2)
	--print("Pub1: ", pub1)
	--print("Priv2: ", priv2)
	--print("Pub2: ", pub2)

	pub3 = k3:getPubKey()
	priv3 = k3:getPrivKey()

	--print("Pub3: ", pub3)
	--print("Priv3: ", priv3)

	shared0=k1:getSharedKey(pub2)
	shared1=k2:getSharedKey(pub1)
	shared2=k3:getSharedKey(pub1)
	--print("Shared0:", shared0)
	--print("Shared1:", shared1)

	aes1=bacteria_aes:new(shared0, "123456789012345")
	aes=bacteria_aes:new(shared1, "123456789012345")
	aes1:encrypt("TestMsg W10013291825328197ASHFASDF8932ASDF8532BUSAFD893251BSDFA78532BFW783125HBSFAD789aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafasdjasdjkfsdfjasdfjiaodfasdfjiasdijfasidfadfiaojsdijfoasdfiaojsdfiojasdfijasdfuhasdufhasdiufhasidufashdfiasudhfiasudhfiuasdfihuSOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOFSDJKFASDJFASJDFQJAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
	--print("Encrypted")
	aes:decrypt(aes1:getAESData_rawEnc())
	aesdata_dec,saesdata_dec=aes:getAESData_dec()
	print("Decrypted: ", aesdata_dec)

	k1.clear()
	k2.clear()
	k3.clear()
end

local function checkGD(width,height,quality)
	width = width or 120
	height = height or 120
	quality = quality or 50

	print("GD")

	local color = {}
	color["red"] = 120
	color["green"] = 320
	color["blue"] = 50
	color["alpha"] = 255
	img=images.gdInitImage(width,height, color)
	local color2 = {}
	color2["blue"]=255
	color["red"]=100
	color["alpha"]=255
	color["green"]=0
	images.gdAddColor(img,"myColor",color)

	images.gdDrawRandomLines(img, 100)
	images.gdDrawRandomPixels(img, 100,300)
	local color1 = images.gdRandColor()
	print(color1['red'],' ',color1['green'], ' ', color1['blue'])

	myIntColor = images.gdInitColor(img, color1)
	myColor = images.gdFoundColor(img,"myColor")


	images.gdDrawRect(img, 0,0, 30, 30, myIntColor)
	images.gdDrawRect(img, 60,60, 30, 30, myColor)
	images.gdDrawRect(img, 60,60, 90, 90, myIntColor)
	images.gdDrawRect(img, 90,90, 120, 120, myColor)
	images.gdWriteText(img, 0,30, "./fonts/dummy.ttf", 15, 0.2, myIntColor, "TestTTF") 
	images.gdWriteText(img, 0,45, "./fonts/dummy.ttf", 15, -0.2, myIntColor, "TestTTF") 
	images.writeToFile(img, "Example.jpg", quality)
	print(images.gdGetRandStr(30))
	imgData = images.getImageData(img, quality)
	c = images.getImageDataRaw(imgData)
	s = images.getImageDataSize(imgData)
	--print((c), "size: ",s)
	images.gdFree(img)
end
--CryptocoinsTest()
--checkGD(120,120,100)
--EncDecTest()
print("GMP")
myZ = lgmp.mpz_init()
lgmp.mpz_add_ui(myZ,myZ, 1024)
lgmp.mpz_add_ui(myZ,myZ, 4201)
print("New value is:",lgmp.mpz_getstr(myZ, 128))
lgmp.mpz_clear(myZ)


