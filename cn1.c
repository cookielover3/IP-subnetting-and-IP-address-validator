#include "util.h" //inclusion of the "utils.h" library in which the functions used are contained
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int ipVerify(char* ipAddress, unsigned char* octetArray) {
	// Function verifies a valid IP has been entered, and then updates the octal array with the validated octets

	char* token;
	int i = 0;
	int j = 0;
	int periodCount = 0;

	// We will continue grabbing tokens whilst it isn't null
	token = strtok(ipAddress, ".");
	while (token != NULL) {

		// Loop through each character and check it's a digit
		// If it isn't break out. We use j to see if it looped the right amount of times
		for (j=0; j<strlen(token); j++) {
			if (isdigit(token[j]) == 0) {
				break;
			}
		}

		// If the right amount of digits have been entered, confirm octet as validated and add to array
		if (strlen(token) > 0 && strlen(token) < 4 && j == strlen(token)
				&& atoi(token) < 256 && atoi(token) >= 0) {
			periodCount++;
			octetArray[i] = atoi(token);
		} else {
			// No point in continuing if even one octet fails the test
			break;
		}

		i++;

		token = strtok(NULL, ".");
	}

	if (periodCount != 4) {
		return 0;
	} else {

		return 1;
	}
}

void printSubnetInfo(uint32_t* addressOctets, int* CIDR, int* subnetBits) {
	// Prints infomation about the given subnet.
	// Takes pointers to the required data, however does not change anything within them.
	// Any required manipulation is done with local variables

	uint32_t netAddress;
	uint32_t netMask;
	
	netMask = (0xFFFFFFFF << (32 - (*CIDR + *subnetBits)) & 0xFFFFFFFF);
	netAddress = *addressOctets & netMask;

	// Unpack and display the network address
	printf("\nNetwork address: %d.%d.%d.%d/%d\n", (netAddress >> 24) & 0xFF, (netAddress >> 16) & 0xFF,
						    (netAddress >> 8) & 0xFF, (netAddress) & 0xFF, *CIDR + *subnetBits);

	// Subtract the network address from the broadcast address and take one from the result for total hosts
	printf("Total hosts: %d\n", ((netAddress | ~netMask) - netAddress) - 1);

	// Display the first host address by adding to each of our unpacked octets
	printf("First host address: %d.%d.%d.%d\n", ((netAddress + 1) >> 24) & 0xFF, ((netAddress + 1) >> 16) & 0xFF,
						    ((netAddress + 1) >> 8) & 0xFF, (netAddress + 1) & 0xFF);
	
	// Bitwise OR the address int with the negated mask to get the broadcast address in the variable
	netAddress = netAddress | ~netMask;

	// Subtract from the from the broadcast address for the final host address
	printf("Last host address: %d.%d.%d.%d\n", ((netAddress - 1) >> 24) & 0xFF, ((netAddress - 1) >> 16) & 0xFF,
						   ((netAddress - 1) >> 8) & 0xFF, (netAddress - 1) & 0xFF);
	
	// Unpack and display the broadcast address
	printf("Broadcast address: %d.%d.%d.%d\n", (netAddress >> 24) & 0xFF, (netAddress >> 16) & 0xFF,
						   (netAddress >> 8) & 0xFF, (netAddress) & 0xFF);
}

void menu()
{
    printf("\n\n1 --> Check if two IPs belong to the same network\n");
    printf("2 --> Subnetting calculation\n");
    printf("3 --> VLSM calculation\n");
    printf("4 --> Exit\n");
}

int main()
{
    //variables declaration
    int ip1[3], ip2[3], ip3[3], ip4[3], cidr;
    int mod;
    int nhost, nsubnet;
    address sub;
    int numvlsm, vlsm[20], host[20], somma;
    mem_init(); //buffer initialization
    fclose(fp);
    do
    {
        menu();
        printf("--> ");
        scanf("%d", &mod);
        printf("\n\n");
        switch (mod)
        {
        case 1: //Check if two IPs belong to the same network
            do
            {
                printf("Enter the IP --> ");
            } while (input(&ip1[0], &ip2[0], &ip3[0], &ip4[0], 0));

            do
            {
                printf("Enter the second ip --> ");
            } while (input(&ip1[1], &ip2[1], &ip3[1], &ip4[1], 0));

            do
            {
                printf("Enter the subnet mask --> ");
            } while (input(&ip1[2], &ip2[2], &ip3[2], &ip4[2], 1));

            printf("First IP --> ");
            bin(&sub, ip1[0], ip2[0], ip3[0], ip4[0], 1);
            printf("Class --> %c\n", classe(ip1[0]));
            printf("Second IP --> ");
            bin(&sub, ip1[1], ip2[1], ip3[1], ip4[1], 1);
            printf("Class --> %c\n", classe(ip1[1]));
            printf("Subnet IP --> ");
            bin(&sub, ip1[2], ip2[2], ip3[2], ip4[2], 1);

            if ((classe(ip1[0])) != (classe(ip1[1])))
            {
                printf("The two IPs do not belong to the same network\n");
            }
            else if (check_network(ip1, ip2, ip3, ip4))
            {
                printf("The two IPs belong to the same network\n");
            }
            else
            {
                printf("The two Ip's do NOT belong to the same subnet.\n");
            }
            break;
        case 2: //Subnetting calculation
            char ipAddress[18];
	        char buffer[4];
	        int CIDR;
	        unsigned char* octetArray;
	        octetArray = calloc(4, sizeof(char));
	        uint32_t addressOctets;
	        int subnetNumber;
	        int subnetBits = 0;
	        int totalSubnets = 0;
	        uint32_t currentSubnet;
	        int i;

	        // Get the address
	        while (1) {
		        printf("Enter IPv4 address now: ");
		        fgets(ipAddress, 17, stdin);
		        ipAddress[strlen(ipAddress)-1] = '\0';

		        printf("Verifying: %s... ", ipAddress);

		        // Verify it
		        if (ipVerify(ipAddress, octetArray) == 0) {
			        printf("Invalid IP entered.\n");
		        } else {
			        printf("Address verified!\n");
			        break;
		        }
	        }

	        // Get the CIDR number
	        while (1) {
		    printf("Enter subnet mask in CIDR notation now: ");
		    fgets(buffer, 4, stdin);

		    CIDR = atoi(buffer);

		    if (CIDR > 0 && CIDR < 32) {
			    break;
		    } else {
			    printf("Invalid CIDR entered. Try again.\n");
		    }
	    }

	    printf("\n%d.%d.%d.%d/%d ", octetArray[0], octetArray[1], octetArray[2], octetArray[3], CIDR);

	    if (octetArray[0] > 239) {
		    printf("(Class E)\n");
	    } else if (octetArray[0] > 223) {
		    printf("(Class D)\n");
	    } else  if (octetArray[0] > 191) {
		    printf("(Class C)\n");
	    } else if (octetArray[0] > 127) {
		    printf("(Class B)\n");
	    } else {
		    printf("(Class A)\n");
	    }

	    // Pack bits of the IP address into an integer
	    addressOctets = (octetArray[0] << 24) | (octetArray[1] << 16) | (octetArray[2] << 8) | (octetArray[3]);

	    // Call the subnetinfo function for the network
	    printSubnetInfo(&addressOctets, &CIDR, &subnetBits);

	    do {
		    printf("Enter number of required networks, or q to quit: ");
		    fgets(buffer, 4, stdin);
		    subnetNumber = atoi(buffer);

		    if (subnetNumber == 0) {
			    printf("Exiting...\n");
			    exit(0);
		    }

		    // Determine the amount of bits required to contain the required networks
		    while (subnetNumber > totalSubnets) {
			    subnetBits++;
			    totalSubnets = pow(2, subnetBits);
		    }

		    // Check we have the required amount of bits to subnet successfully
		    if ((CIDR + subnetBits) > 31) {
			    printf("Amount of networks too large to be accommodated.\n");
		    }
	    } while ((CIDR + subnetBits) > 31);

	    printf("\nTotal subnets to be created: %d\n-------------------------------", totalSubnets); 

	    // Construct the subnet network bits, then print the information
	    for (i=0; i<totalSubnets; i++) {
		    currentSubnet = (addressOctets & ((0xFFFFFFFF << (32 - CIDR)) & 0xFFFFFFFF))
				| i << (32 - (CIDR + subnetBits));
		    printSubnetInfo(&currentSubnet, &CIDR, &subnetBits);
	    }

	    free(octetArray);
	
	
        break;
        case 3: //VLSM calculation
            printf("Enter the IP --> ");
            input(&ip1[0], &ip2[0], &ip3[0], &ip4[0], 0);
            printf("Enter number of subnets --> ");
            scanf("%d", &numvlsm);
            for (int i = 0; i < numvlsm; i++)
            {
                printf("Enter number of PCs for the network %d --> ", i + 1);
                scanf("%d", &vlsm[i]);
            }
            printf("\n");
            for (int i = 0; i < numvlsm; i++)
            {
                printf("Network number %d has %d host\n", i + 1, vlsm[i]);
            }
            printf("\nIP binary format --> ");
            bin(&sub, ip1[0], ip2[0], ip3[0], ip4[0], 1);
            printf("\n\n");
            printf("Hosts in descending order : \n");
            shakersort(vlsm, numvlsm);
            for (int i = 0; i < numvlsm; i++)
            {
                printf("%d\n", vlsm[i]);
            }
            printf("\n\n");
            numhost(numvlsm, host, vlsm, &somma);
            id_sub(classe(ip1[0]), numvlsm, host, somma, ip3[0], ip4[0], ip2[0], ip1[0]);
            break;
        case 4: //program exit
            return 0;
        default: //default case
            printf("Enter one of the options above\n");
        }
    } while (1);
}