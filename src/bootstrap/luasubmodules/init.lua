print("bootstrap server inited")
--int sock, const char * uIp, uint16_t uPort, char* buf
function event1(sock, ip, port, buf)
	print("Message: ", buf, " from ", ip, " on port: ", port)
	return "Hewwo?"
end
