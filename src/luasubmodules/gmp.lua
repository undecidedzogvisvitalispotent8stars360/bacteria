gmp = {}
--
--	myZ = lgmp.mpz_init()
--	lgmp.mpz_add_ui(myZ,myZ, 5)
--	lgmp.mpz_add_ui(myZ,myZ, 1)
	--6
--	lgmp.mpz_mul(myZ,myZ, myZ)
--	--6*6 = 36
--	lgmp.mpz_mul_ui(myZ,myZ, 5)
--	--36*5 = 180
--	myZ1 = lgmp.mpz_init_str("5")
--	lgmp.mpz_div(myZ,myZ,myZ1)
--	print("New value is:(will be 36)",lgmp.mpz_getstr(myZ, 128))
--	lgmp.mpz_sub_ui(myZ,myZ, 40)
--	print("New value is:(will be -4)",lgmp.mpz_getstr(myZ, 128))
--	lgmp.mpz_clear(myZ)

--	myF = lgmp.mpf_init()
--	lgmp.mpf_add_ui(myF, myF, 10)
--	lgmp.mpf_add_ui(myF, myF, 10)

--	myF1 = lgmp.mpf_init_str("321.0234")
--	lgmp.mpf_add(myF, myF1,myF1)
--	print("New value is:",lgmp.mpf_getstr(myF1, 128))
--	lgmp.mpf_mul(myF1, myF1,myF1)
--	print("New value is:",lgmp.mpf_getstr(myF1, 128))
--	lgmp.mpf_div(myF1, myF1,myF)

--	print("New value is:",lgmp.mpf_getstr(myF1, 128))
--	lgmp.mpf_clear(myF)
--	lgmp.mpf_clear(myF1)
--gmp.types =  { ["mpz"] = 1,["mpf"] = 2} 
function gmp:newMPZ()
	--if not m_type == gmp.types.mpz and not m_type == gmp.types.mpf then
	--	error("Unknown gmp type")
	--end
	local obj = {}
		obj.m_num = {}
	function obj:init(num_str)
		num = num or nil
		if not obj.m_num == nil then
			print("WARNING: GMP try reinit without clear. ignoring.")
			return nil
		end
		if num == nil then
			obj.m_num = lgmf.mpz_init()
		else
			obj.m_num = lgmp.mpz_init_str(num_str)
		end
	end
	function obj:clear()
		if obj.m_num == nil then return nil end
		lgmp.mpz_clear(obj.m_num)
		obj.m_num = nil
	end
	
end

function gmp:newMPF()
	--if not m_type == gmp.types.mpz and not m_type == gmp.types.mpf then
	--	error("Unknown gmp type")
	--end
	local obj = {}
		obj.m_num = {}
	function obj:init(num_str)
		num = num or nil
		if not obj.m_num == nil then
			print("WARNING: GMP try reinit without clear. ignoring.")
			return nil
		end
		if num == nil then
			obj.m_num = lgmf.mpf_init()
		else
			obj.m_num = lgmp.mpf_init_str(num_str)
		end
	end
	function obj:clear()
		if obj.m_num == nil then return nil end
		lgmp.mpf_clear(obj.m_num)
		obj.m_num = nil
	end
	
end

return gmp
