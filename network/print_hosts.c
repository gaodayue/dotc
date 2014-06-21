#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXBUFLEN 128

int main(void)
{
	struct hostent *ent;
	char **aliases, **addrs;
	int domain;
	char addrbuf[MAXBUFLEN];

	while ((ent = gethostent()) != NULL)
	{
		printf("---------------------\n");
		printf("hostname: %s\n", ent->h_name);

		// print aliases
		for (aliases = ent->h_aliases; *aliases != NULL; aliases++)
		{
			printf("  %s\n", *aliases);
		}

		// print domain
		domain = ent->h_addrtype;
		if (domain == AF_INET)
			printf("domain: IPV4\n");
		else if (domain == AF_INET6)
			printf("domain: IPV6\n");
		else
			printf("domain: unknown %d\n", domain);

		// print addresses
		if (domain == AF_INET || 
			domain == AF_INET6)
		{
			for (addrs = ent->h_addr_list; *addrs != NULL; addrs++)
			{
				inet_ntop(domain, *addrs, addrbuf, MAXBUFLEN);
				printf("  address:%s\n", addrbuf);
			}
		}

		printf("\n");
	}
	endhostent();
}
