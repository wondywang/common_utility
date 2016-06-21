#include <algorithm>
#include <cmath>
#include <arpa/inet.h>

using std::string;

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void myPrint(const char *argc, ...)
{
#ifdef DEBUG
	if(NULL == argc)	return;
	char buff[1024<<4] = {0};
	va_list args_ptr = NULL;
	va_start(args_ptr, argc);
	vsprintf(buff, argc, args_ptr);
	va_end(args_ptr);
	fputs(buff, stdout);
#else
	//do nothing
#endif
	return;
}

#define Api_Debug_Log(format, ...)\
	do {\
		myPrint("\x1B[33m<debug>\x1B[0m%s::%s[%d]: "#format"\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} while (0)

#define Api_Error_Log(format, ...)\
	do {\
		myPrint("\x1B[31m<error>%s::%s[%d]: "#format"\n\x1B[0m", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} while (0)

void hex2string(const unsigned char* hex, int hexLen, char* str, int *len)
{
	int i, o;
	for (i=0, o=0; i<hexLen; i++, o+=2)
	{
		snprintf(&str[o], sizeof(int), "%02X", hex[i]);
	}
	int length = 2*hexLen;
	len = &length;
}

void HexToStr(string &sDest, const string &sSrc)
{
	if (sSrc.empty())
	{
		sDest = "";
		return ;
	}
	unsigned int uLen = sSrc.length();
	char szDest[uLen*2 + 1];
	memset(szDest, 0, sizeof(szDest));
	int j = 0;
    	char l_byTmp;
    	for (unsigned int i = 0; i < uLen; i++)
    	{
        	l_byTmp = (sSrc[i] & 0xf0) >> 4;
        	if (l_byTmp < 10)
        	{
            		szDest[j++] = l_byTmp + '0';
        	}
        	else
        	{
            		szDest[j++] = l_byTmp - 10 + 'A';
        	}
        	l_byTmp = sSrc[i] & 0x0f;
        	if (l_byTmp < 10)
        	{
            		szDest[j++] = l_byTmp + '0';
        	}
        	else
        	{
            		szDest[j++] = l_byTmp - 10 + 'A';
        	}
    	}
    	szDest[j] = 0;
	sDest = szDest;
}

int Str2Hex(const char *sSrc, char *sDest, int nSrcLen )
{
	int i, nHighBits, nLowBits;
	for( i = 0; i < nSrcLen; i += 2 )
	{
		nHighBits = sSrc[i];
		nLowBits  = sSrc[i + 1];

		if( !isxdigit(nHighBits) )
			return -1;

		if( nHighBits > 0x39 )
			nHighBits -= 0x37;
		else
			nHighBits -= 0x30;

		if( i == nSrcLen - 1 )
			nLowBits = 0;
		else if( !isxdigit(nLowBits) )
			return -1;
		else if( nLowBits > 0x39 )
			nLowBits -= 0x37;
		else
			nLowBits -= 0x30;

		sDest[i / 2] = (nHighBits << 4) | (nLowBits & 0x0f);
	}
	return 0;
}

char *format_time( time_t tm)
{
    static char str_tm[1024];
    struct tm tmm;
    memset(&tmm, 0, sizeof(tmm) );
    localtime_r((time_t *)&tm, &tmm);

    snprintf(str_tm, sizeof(str_tm), "[%04d-%02d-%02d %02d:%02d:%02d]",
             tmm.tm_year + 1900, tmm.tm_mon + 1, tmm.tm_mday,
             tmm.tm_hour, tmm.tm_min, tmm.tm_sec);

    return str_tm;
}

inline char* Int2Str(unsigned int iValue)
{
	static char buf[64];
	snprintf(buf, sizeof(buf), "%u", iValue);
	return buf;
}

/*
   SplitStr字符串分割函数，可以同时使用多种字符分割，无返回值
   参数1：pOriginal待分割的字符串
   参数2：pSplitor 指定分隔符列表，里面的每个字符（或者任意多个里面的字符组成的字符串）都视为一个分隔符
   例如：pSplitor为",;" 那么字符','和';'都会被视为分割字符，
   则123;345,abc;,;cdf 会被分割为123、456、abc、cdf四个字符串通过result返回
   参数3：result 保存结果的容器
*/
void SplitStr(const char* pOriginal,const char* pSplitor, std::vector<std::string>& result) 
{
    size_t sHead = strspn(pOriginal, pSplitor);
    size_t sEnd = 0;
    const char * pHead = pOriginal + sHead;
    const char * pEnd = pHead;
    while( '\0' != *pHead) 
    {
        std::string strElement;
        sEnd = strcspn(pHead, pSplitor);
        pEnd = pHead + sEnd; 
        if( '\0' == *pEnd ) 
        {       
            strElement.assign(pHead, pEnd - pHead); 
            result.push_back(strElement);
            break;  
        }       
        strElement.assign(pHead, pEnd - pHead); 
        result.push_back(strElement);
        sHead = strspn(pEnd, pSplitor);
        pHead = pEnd + sHead;
    }
}

bool API_Zip(string& strXml)
{
     char   sTmpBuf[1024];
     size_t dwTmpLen = sizeof(sTmpBuf);
     int iRet = compress((Bytef *)sTmpBuf, (uLongf *)&dwTmpLen, (const Bytef *)strXml.c_str(), strXml.size());
     if(iRet != Z_OK)
     {
         return false;
     }
    strXml = "";
	strXml.resize(1, 1);
    strXml.append(sTmpBuf, dwTmpLen);
	return true;
}

inline string API_XmlEncode(const char *pSrc, unsigned int uSrcLen=0)
{
    if(uSrcLen == 0)
    {
        uSrcLen = strlen(pSrc);
    }
    if(uSrcLen == 0)
    {
        return string("");
    }
    char *pBuf = new char[uSrcLen*6+1];
    char *pDest = pBuf;
    for(unsigned int i=0; i<uSrcLen; ++i)
    {
        switch(pSrc[i])
        {
            case '&':
                memcpy(pDest, "&amp;", 5);
                pDest += 5;
                break;
            case '<':
                memcpy(pDest, "&lt;", 4);
                pDest += 4;
                break;
            case '>':
                memcpy(pDest, "&gt;", 4);
                pDest += 4;
                break;
            case '\"':
                memcpy(pDest, "&quot;", 6);
                pDest += 6;
                break;
            case '\'':
                memcpy(pDest, "&#39;", 5);
                pDest += 5;
                break;
            case '\r':
                break;
            case '\t':
                memcpy(pDest, "    ", 4);
                pDest += 4;
                break;
            default:
                *(pDest++) = pSrc[i];
        }
    }
    string strReturn(pBuf, pDest-pBuf);
    delete [] pBuf;

    return strReturn;
}

string API_UrlEncode( const string& input )
{
	unsigned char hexchars[] = "0123456789ABCDEF" ;
	char* s = (char*)input.c_str() ;
	int len = input.size() ;
	register int x, y ;
	unsigned char* str ;
	str = (unsigned char*)malloc(3 * len + 1) ;
	for( x=0, y=0; len--; x++, y++ )
	{
		str[y] = (unsigned char)s[x] ;
		if(str[y] == ' ')
		{
			str[y] = '+';
		}
//		else if( (str[y] < '0' && str[y] != '-' && str[y] != '.' && str[y] != '$' && str[y] != '+' && str[y] != '!' && str[y] != '*' && str[y] != '\'' && str[y] != '(' && str[y] != ')' && str[y] != ',') || (str[y] < 'A' && str[y] > '9') || (str[y] > 'Z' && str[y] < 'a' && str[y] != '_') || (str[y] > 'z') )
		else if( (str[y] < '0' && str[y] != '-' && str[y] != '.') || (str[y] < 'A' && str[y] > '9') || (str[y] > 'Z' && str[y] < 'a' && str[y] != '_') || (str[y] > 'z') )
		{
			str[y++] = '%' ;
			str[y++] = hexchars[(unsigned char) s[x] >> 4] ;
			str[y] = hexchars[(unsigned char) s[x] & 15] ;
		}
	}
	str[y] = '\0' ;
	string result((char*)str) ;
	free(str) ;
	return result ;
}
