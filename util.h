#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//struct declaration for IP address
typedef struct
{
    int sub[4][8];
} address;

address sub;

//file pointer declaration
FILE *fp;

//Function prototypes declaration

//buffer initialization
void mem_init();

//check txt file
int check_file();

//function to clean the txt file
void cleanfile();

//stampa a video la text art "sistemi"
void banner(int mod);

//string to integer conversion for input
void str2int(char *ip, int *ip1, int *ip2, int *ip3, int *ip4);

//ip conversion to binary
void bin(address *sub, int ip1, int ip2, int ip3, int ip4, int mod);

//sorting1 (is needed for vlsm)
void swap(int *a, int *b);

//sorting2 (is needed for vlsm)
void shakersort(int *a, int n);

//function to find the number of hosts in vlsm
void numhost(int nvlsm, int *host, int *vlsm, int *somma);

//function that prints the results of VLSM processing
void id_sub(char classe_ip, int nvlsm, int *host, int somma, int ip3, int ip4, int ip2, int ip1);

//function to determine the class of IP
char classe(int ip1);

//function for take the input
int input(int *ip1, int *ip2, int *ip3, int *ip4, int mod);

//function that checks if two IPs belong to the same network
int check_network(int ip1[], int ip2[], int ip3[], int ip4[]);

//checking the validity of the subnet mask in CIDR format
int check_cidr(int *cidr);

//function that finds the bits dedicated to hosts in the IP address
int bit_host(int cidr, int ip1);

//function that finds the number of hosts and the number of subnets that can be used by an IP address
void calc(int *nsubnet, int *nhost, int bit_host, int bit_sub);

//function that finds the block of hosts for subnetting (serves for calculating subnets)
int blocco(int bit_sub);

//function that prints the results of subnetting processing (network address, broadcast, gateway)
void print_ip(int ip1, int ip2, int ip3, int ip4, int block, int nsubnet);

void str2int(char *ip, int *ip1, int *ip2, int *ip3, int *ip4)
{
    sscanf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
}

// Buffer initialization to be more dynamic on standard peripherals
void mem_init()
{
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);
}

//function for conversion to binary
void bin(address *sub, int ip1, int ip2, int ip3, int ip4, int mod)
{
    FILE *fp;
    fp = fopen("ip.txt", "a");
    fprintf(fp, "\n");
    int j = 3;
    for (int p = 7; p > -1; p--)
    {
        sub->sub[j][p] = ip4 % 2;
        ip4 = (ip4 - sub->sub[j][p]) / 2;
    }
    j--;
    for (int p = 7; p > -1; p--)
    {
        sub->sub[j][p] = ip3 % 2;
        ip3 = (ip3 - sub->sub[j][p]) / 2;
    }
    j--;
    for (int p = 7; p > -1; p--)
    {
        sub->sub[j][p] = ip2 % 2;
        ip2 = (ip2 - sub->sub[j][p]) / 2;
    }
    j--;
    for (int p = 7; p > -1; p--)
    {
        sub->sub[j][p] = ip1 % 2;
        ip1 = (ip1 - sub->sub[j][p]) / 2;
    }
    if (mod == 1)
    {
        fprintf(fp, "Binary --> ");
        for (int j = 0; j < 4; j++)
        {
            for (int p = 0; p < 8; p++)
            {
                printf("%d", sub->sub[j][p]);
                fprintf(fp, "%d", sub->sub[j][p]);
            }
            if (j != 3)
            {
                printf(".");
                fprintf(fp, ".");
            }
            else
            {
                printf("\n");
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }
}

/*
to determine the class of the ip address
I make a control estimating the first octet in decimal and then 
I make a return of a character A, B or C that will serve for the
successive elaborations
*/
char classe(int ip1)
{
    if ((ip1 <= 127) && (ip1 > 0))
        return 'A';

    if ((ip1 >= 128) && (ip1 <= 191))
        return 'B';

    if ((ip1 >= 192) && (ip1 <= 223))
        return 'C';
}

//check if two IPs belong to the same network
int check_network(int ip1[], int ip2[], int ip3[], int ip4[])
{
    int a, b, c, d, e, f, g, h;
    //CLASS A
    //I do bit to bit AND between ip and subnet mask to extract the network address
    //and check if the two ip's belong to the same network
    if ((classe(ip1[0]) == 'A') && (classe(ip1[1]) == 'A'))
    {
        /*
        ip1[0] contains the first octet of the first IP
        ip1[1] contains the first octet of the second IP
        ip1[2] contains the first octet of the subnet mask
        - with the unary operator & I obtain the result of the and bit to bit between
        - the first octet of the first ip with the first octet of the subnet mask
        - the first octet of the second ip with the first octet of the subnet mask
        this procedure for all octets
        if the results between the first ip and the second ip are equal then belong to the same network
        */
        a = (ip1[0] & ip1[2]);
        b = (ip1[1] & ip1[2]);
        c = (ip2[0] & ip2[2]);
        d = (ip2[1] & ip2[2]);
        e = (ip3[0] & ip3[2]);
        f = (ip3[1] & ip3[2]);
        g = (ip4[0] & ip4[2]);
        h = (ip4[1] & ip4[2]);

        if ((a == b) && (c == d) && (e == f) && (g == h))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    //CLASS B
    //I do bit to bit AND between ip and subnet mask to extract the network address
    //and check if the two ip's belong to the same network
    if ((classe(ip1[0]) == 'B') && (classe(ip1[1]) == 'B'))
    {
        /*
        ip1[0] contains the first octet of the first IP
        ip1[1] contains the first octet of the second IP
        ip1[2] contains the first octet of the subnet mask
        - with the unary operator & I obtain the result of the and bit to bit between
        - the first octet of the first ip with the first octet of the subnet mask
        - the first octet of the second ip with the first octet of the subnet mask
        this procedure for all octets
        if the results between the first ip and the second ip are equal then belong to the same network
        */
        a = (ip1[0] & ip1[2]);
        b = (ip1[1] & ip1[2]);
        c = (ip2[0] & ip2[2]);
        d = (ip2[1] & ip2[2]);
        e = (ip3[0] & ip3[2]);
        f = (ip3[1] & ip3[2]);
        g = (ip4[0] & ip4[2]);
        h = (ip4[1] & ip4[2]);

        if ((a == b) && (c == d) && (e == f) && (g == h))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    //CLASS C
    //I do bit to bit AND between ip and subnet mask to extract the network address
    //and check if the two ip's belong to the same network
    if ((classe(ip1[0]) == 'C') && (classe(ip1[1]) == 'C'))
    {
        /*
        ip1[0] contains the first octet of the first IP
        ip1[1] contains the first octet of the second IP
        ip1[2] contains the first octet of the subnet mask
        - with the unary operator & I obtain the result of the and bit to bit between
        - the first octet of the first ip with the first octet of the subnet mask
        - the first octet of the second ip with the first octet of the subnet mask
        this procedure for all octets
        if the results between the first ip and the second ip are equal then belong to the same network
        */
        a = (ip1[0] & ip1[2]);
        b = (ip1[1] & ip1[2]);
        c = (ip2[0] & ip2[2]);
        d = (ip2[1] & ip2[2]);
        e = (ip3[0] & ip3[2]);
        f = (ip3[1] & ip3[2]);
        g = (ip4[0] & ip4[2]);
        h = (ip4[1] & ip4[2]);
        if ((a == b) && (c == d) && (e == f) && (g == h))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return -1;
}

int input(int *ip1, int *ip2, int *ip3, int *ip4, int mod)
{
    char ip[16];
    scanf("%s", ip);
    str2int(ip, ip1, ip2, ip3, ip4);
    //input subnet mask validity check
    /*
    the octets of the subnet mask in decimal can take only certain values:
    255, 254, 252, 248, 240, 224, 192, 128, 0
    with the control that follows I verify that the octets have these numbers
    */
    if (mod == 1)
    {
        if ((*ip1 != 255 && *ip1 != 254 && *ip1 != 252 && *ip1 != 248 && *ip1 != 240 && *ip1 != 224 && *ip1 != 192 &&
             *ip1 != 128 && *ip1 != 0) ||
            (*ip2 != 255 && *ip2 != 254 && *ip2 != 252 && *ip2 != 248 && *ip2 != 240 && *ip2 != 224 && *ip2 != 192 &&
             *ip2 != 128 && *ip2 != 0) ||
            (*ip3 != 255 && *ip3 != 254 && *ip3 != 252 && *ip3 != 248 && *ip3 != 240 && *ip3 != 224 && *ip3 != 192 &&
             *ip3 != 128 && *ip3 != 0) ||
            (*ip4 != 255 && *ip4 != 254 && *ip4 != 252 && *ip4 != 248 && *ip4 != 240 && *ip4 != 224 && *ip4 != 192 &&
             *ip4 != 128 && *ip4 != 0))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if ((*ip1 > 255 || *ip1 < 0) || (*ip2 > 255 || *ip2 < 0) || (*ip3 > 255 || *ip3 < 0) ||
            (*ip4 > 255 || *ip4 < 0))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//function that verifies that the subnet mask in CIDR format is valid
int check_cidr(int *cidr)
{
    scanf("%d", cidr);
    if (*cidr > 32 || *cidr == 0) //the Subnet Mask in CIDR format cannot be greater than 32
        return 1;
    else
        return 0;
}

/*
calculation of bits dedicated to subnetting (bits dedicated to the network)
I have used a switch instead of an if because more practical for this purpose 
*/
int bitsubnetting(int cidr, int ip1)
{
    switch (classe(ip1))
    {
    case 'A':
        if (cidr >= 8)
        {
            return cidr - 8;
        }
        else
        {
            return 8 - cidr;
        }

    case 'B':
        if (cidr >= 16)
        {
            return cidr - 16;
        }
        else
        {
            return 16 - cidr;
        }

    case 'C':
        if (cidr >= 24)
        {
            return cidr - 24;
        }
        else
        {
            return 24 - cidr;
        }
    }
}

int bit_host(int cidr, int ip1)
{
    //calculation of bits dedicated to hosts
    switch (classe(ip1))
    {
    case 'A':
        return 32 - cidr;
    case 'B':
        return 32 - cidr;
    case 'C':
        return 32 - cidr;
    }
}
/*
function that calculates the number of hosts that can be used and the number of subnets that can be obtained
*/
void calc(int *nsubnet, int *nhost, int bit_host, int bit_sub)
{
    *nhost = pow(2, bit_host) - 2;
    *nsubnet = pow(2, bit_sub);
}

/*
determine the block of hosts for subnetting
block will be used to calculate the network, broadcast and gateway addresses 
using FOR cycles
reasoning:
a network is composed by 256 hosts 
2^1 = 2 available networks --> 256/2 = 128 hosts (block)
2^2 = 4 available networks --> 256/4 = 64 hosts (block)
2^3 = 8 available networks --> 256/8 = 32 hosts (block)
2^4 = 16 available networks --> 256/16 = 16 hosts (block)
2^5 = 32 available networks --> 256/32 = 8 hosts (block)
2^6 = 64 available networks --> 256/64 = 4 hosts (block)
2^7 = 128 available networks --> 256/128 = 2 hosts (block)
2^8 = 256 available networks --> 256/256 = 1 host (block) 
in the last case hypothetically 256 networks from 1 host 
*/
int blocco(int bit_sub)
{
    switch (bit_sub)
    {
    case 1:
        return 128;
    case 2:
        return 64;
    case 3:
        return 32;
    case 4:
        return 16;
    case 5:
        return 8;
    case 6:
        return 4;
    case 7:
        return 2;
    case 8:
        return 1;
    }
}

//sorting for vlsm
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
//sorting for vlsm
void shakersort(int *a, int n)
{
    int p, i;
    for (p = 1; p <= n / 2; p++)
    {
        for (i = p - 1; i < n - p; i++)
            if (a[i] < a[i + 1])
            {
                swap(&a[i], &a[i + 1]);
            }
        for (i = n - p - 1; i >= p; i--)
            if (a[i] > a[i - 1])
            {
                swap(&a[i], &a[i - 1]);
            }
    }
}
/*
function for the calculation of VLSM
I need the variable "somma" to keep count of the number of hosts
and then decide whether it is possible or not to make the calculation (based on the capacity
of the octets dedicated to the hosts for class A, B, C)
the vector vlsm[i] contains the number of hosts requested by the client.
Example:
request 58 pc : check 64
request 25 pc : check 32 
and so on with this reasoning
I consider 3 waste hosts for net id, gateway and broadcast
*/
void numhost(int nvlsm, int *host, int *vlsm, int *somma)
{
    *somma = 0;
    for (int i = 0; i < nvlsm; i++)
    {
        switch (vlsm[i] + 3)
        {
        case 0 ... 2:
            host[i] = 2;
            *somma += host[i];
            break;
        case 3 ... 4:
            host[i] = 4;
            *somma += host[i];
            break;
        case 5 ... 8:
            host[i] = 8;
            *somma += host[i];
            break;
        case 9 ... 16:
            host[i] = 16;
            *somma += host[i];
            break;
        case 17 ... 32:
            host[i] = 32;
            *somma += host[i];
            break;
        case 33 ... 64:
            host[i] = 64;
            *somma += host[i];
            break;
        case 65 ... 128:
            host[i] = 128;
            *somma += host[i];
            break;
        case 129 ... 256:
            host[i] = 256;
            *somma += host[i];
            break;
        default:
            break;
        }
    }
}
/*
function that calculates net id, broadcast and gateway for vlsm
the reasoning is the same of subnetting
I use for cycles to add host[i] to octets (variable according to the class)
host[i] corresponds to the "block" of hosts in the case of subnetting
but this time is variable and not fixed
*/

void id_sub(char classe_ip, int nvlsm, int *host, int somma, int ip3, int ip4, int ip2, int ip1)
{
    int ip3v2 = ip3;
    int inc;
    switch (classe_ip)
    {
    case 'A':
        if (somma < 16777216)
        {
            ip4 = 0;
            for (int i = 0; i < nvlsm; ++i)
            {
                printf("net id %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4);
                bin(&sub, ip1, ip2, ip3, ip4, 1);
                fprintf(fp, "net id %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4);
                printf("first host of the network %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4 + 1);
                bin(&sub, ip1, ip2, ip3, ip4 + 1, 1);
                fprintf(fp, "first host of the network %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4 + 1);
                if ((ip4 + host[i]) >= 256)
                {
                    ip4 = 0;
                    inc = somma - 255;
                    ip3++;
                }
                else
                {
                    ip4 += host[i];
                }
            }
            printf("\n");
            break;
    case 'B':
        if (somma < 65536)
        {
            ip4 = 0;
            for (int i = 0; i < nvlsm; ++i)
            {
                printf("net id %d --> %d.%d.%d.%d ", i + 1, ip1, ip2, ip3, ip4);
                bin(&sub, ip1, ip2, ip3, ip4, 1);
                fprintf(fp, "net id %d --> %d.%d.%d.%d ", i + 1, ip1, ip2, ip3, ip4);
                printf("first host of network %d --> %d.%d.%d.%d ", i + 1, ip1, ip2, ip3, ip4 + 1);
                bin(&sub, ip1, ip2, ip3, ip4 + 1, 1);
                fprintf(fp, "first host of network %d --> %d.%d.%d.%d ", i + 1, ip1, ip2, ip3, ip4 + 1);
                if ((ip4 + host[i]) >= 256)
                {
                    ip4 = 0;
                    inc = somma - 255;
                    ip3++;
                }
                else
                {
                    ip4 += host[i];
                }
            }
            printf("\n");
            break;
    case 'C':
        if (somma < 255)
        {
            ip4 = 0;
            for (int i = 0; i < nvlsm; i++)
            {
                printf("net id %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4);
                bin(&sub, ip1, ip2, ip3, ip4, 1);
                fprintf(fp, "net id %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4);
                printf("first host of network %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4 + 1);
                bin(&sub, ip1, ip2, ip3, ip4 + 1, 1);
                fprintf(fp, "first host of network %d --> %d.%d.%d.%d\n ", i + 1, ip1, ip2, ip3, ip4 + 1);
                ip4 += host[i];
            }
            ip4 = 0;
            printf("\n");
        break;
    }
}

