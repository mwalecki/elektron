#define _GROUP(buf, grName, grNameLen)									if(strncmp((const char*)buf, grName, grNameLen)==0){
#define _ENDGROUP														}
#define _GET_SET_MEMBER(buf, varName, memName, memNameLen, grNameLen)	if(strncmp((const char*)(buf+grNameLen), memName, memNameLen)==0){if(buf[(grNameLen+memNameLen)]=='?'){sprintf((char*)buf,"%d",varName);USART1_SendString((char*)buf);USART1_SendString(USART_TxSuffix);}else{varName=atoi(buf+(grNameLen+memNameLen+1));}}
#define _IF_MEMBER_THEN(buf, memName, memNameLen, grNameLen)			if(strncmp((const char*)(buf+grNameLen), memName, memNameLen)==0)

