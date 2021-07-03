#include<string.h>
#include<string>
#include<vector>
namespace split{
//constexpr auto bufsize=2056;
char ** 
split_msg(char * buf, const char schar, size_t * splitted_size, size_t msg_len);

void 
free_splitted(char  ** what, size_t n);

std::vector<std::string> split(char * buf, const char schar, size_t bufsize=0);

};
