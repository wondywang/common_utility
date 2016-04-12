#include <algorithm>
#include <cmath>
#include <arpa/inet.h>

using std::string;

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
