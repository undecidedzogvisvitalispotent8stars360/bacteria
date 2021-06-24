baes = {}

function string.fromhex(str)
    return (str:gsub('..', function (cc)
        return string.char(tonumber(cc, 16))
    end))
end

function string.tohex(str)
    return (str:gsub('.', function (c)
        return string.format('%02X', string.byte(c))
    end))
end

function baes:newKeyPair(keyPair)
	local obj = {}
		obj.keyPair=keyPair or {}
	function obj:initKeyPair(pub,priv)
	 	--print("init")
		if not obj.keyPair == {} then return false end 
		keyPair = {}
		p = priv or nil
		pb = pub or nil
		if p == nil or pb == nil then
		--	print("geKeyPair()")
			keyPair = encdec.getKeyPair();
		else
		--	print("p != {} and pb != {}")
		--	print("Pub: ", pub, "priv: ",priv)
			keyPair = encdec.createKeyPair(pub,priv)
		end 
		obj.keyPair = keyPair
		return true;
	end
	function obj:getKeyPair()
		if obj.keyPair == {} then return nil end 
		return obj.keyPair
	end
	function obj:getPubKey()
		if obj.keyPair == nil then
			 print("PubKey is nil")
			 return nil 
		end 
		--print("ret pub key",encdec.getPubKey(obj.keyPair))
		return encdec.getPubKey(obj.keyPair)
	end
	function obj:getPrivKey()
		if obj.keyPair == nil then
			 print("PubKey is nil")
			 return nil 
		end 
		--print("ret pub key",encdec.getPubKey(obj.keyPair))
		return encdec.getPrivKey(obj.keyPair)
	end
	function obj:getSharedKey(key)
		--print("getSharedKey and pub is: ", key) 
		if obj.keyPair == {} then return nil end
		if key == nil then error("PubKey not will be nil") end
		return encdec.getSharedKey(obj.keyPair, key)
	end
	function obj:clear()
		if not obj.keyPair == nil then
			encdec.freeKeyPair(obj.keyPair)
			obj.keyPair = nil
		end
	end
	return obj
end
function baes:new(key,iv)
	local obj = {}
		obj.key=key or encdec.genRandBytes(32)
		obj.iv=iv   or encdec.genRandBytes(16)
		obj.aesdata_enc={}
		obj.aesdata_dec={}
		

		function obj:getKey()
			return self.key
		end

		function obj:getIV()
			return self.iv
		end
		function obj:getAESData_rawEnc()
			return self.aesdata_enc
		end
		function obj:getAESData_rawDec()
			return self.aesdata_dec
		end
		function obj:getAESData_enc()
			return encdec.getAESData(self.aesdata_enc),encdec.getAESData_len(self.aesdata_enc)
		end

		function obj:getAESData_dec()
			return encdec.getAESData(self.aesdata_dec),encdec.getAESData_len(self.aesdata_dec)
		end

		function obj:setAESData_enc(msg,size)
			msg = msg or ""
			size = size or 0
			encdec.createAESData(msg,size)
		end
		function obj:encrypt(msg,t)
			t = t or AESENCType['t_chacha20']
			self.aesdata_enc=encdec.AESenc(self.key,self.iv,msg,t)
		end
		function obj:decrypt(aesdata,t)
			t = t or AESENCType['t_chacha20']
			self.aesdata_dec=encdec.AESdec(self.key,self.iv,aesdata,t)
		end

		function obj:clear()
				if not obj.aesdata_enc == nil then 
					encdec.freeAESData(self.aesdata_enc) 
					self.aesdata_enc = nil
				end
				if not obj.aesdata_dec == nil then 
					encdec.freeAESData(self.aesdata_dec) 
					self.aesdata_dec = nil
				end
		end
		return obj
end


--checkAllTypes("")
--checkAllTypes("is example message")
--checkAllTypes("a")
--checkAllTypes("ab")
--checkAllTypes("abc")
--checkAllTypes("abcd")
-- end test stuff



return baes
