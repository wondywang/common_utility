#include <string>

int SelfNet2Ip(const string &net_name, unsigned int& net_ip)
{
	#define BUFSIZE 1024
	int _sock_fd;
	struct ifconf conf;
	struct ifreq *ifr;
	char buff[BUFSIZE];
	int num;
	int i;

	_sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
	if ( _sock_fd < 0 )	return -1;

	conf.ifc_len = BUFSIZE;
	conf.ifc_buf = buff;

	if ( ioctl(_sock_fd, SIOCGIFCONF, &conf) < 0 )
	{
		close(_sock_fd);
		return -1;
	}

	num = conf.ifc_len / sizeof(struct ifreq);
	ifr = conf.ifc_req;

	for( i=0; i<num;i++ )
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
		if ( ioctl(_sock_fd, SIOCGIFFLAGS, ifr) < 0 )
		{
			close(_sock_fd);
			return -1;
		}

		if ( (ifr->ifr_flags & IFF_UP)
            && strcmp(net_name.c_str(),ifr->ifr_name) == 0 )
		{
			net_ip = (unsigned)sin->sin_addr.s_addr;
			close(_sock_fd);
			return 0;
		}
		ifr++;
	}
	close(_sock_fd);

	return -1;
}
