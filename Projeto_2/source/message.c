/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/message.h"
#include "../include/extra/inet.h"
//#include "sdmessage.pb-c.h"
#include <errno.h>

int write_all(int sock, void *buf, int len) {
	
	int bufsize = len; 
	while(len > 0) {
		int res = write(sock, buf, len); 
		
		if(res < 0) {
			if(errno==EINTR) 
				continue;
			perror("write failed");
			return res;
		} 
		buf+= res;
		len-= res;
	}
	return bufsize;
}


int read_all(int sock, void *buf, int len){
    int size = len;
	
  while(size>0) {
      int res = read(sock, buf, size);
	
      if(res == 0)
          return 0;
      if(res<0) {
          perror("falha na leitura:");
          return res;
      }
      buf += res;
      size -= res;
  }
  return len;
}
