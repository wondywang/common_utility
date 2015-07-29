#include <algorithm>
#include <cmath>
#include <arpa/inet.h>

using std::string;

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