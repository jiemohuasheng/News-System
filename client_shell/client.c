//************************************************************************************************/
//This program makes a TCP connection with the server using ip and port number.
//After connecting the client can send different commands to get the news feeds of several categories.

//Program Name: feedmenews client
//File Name: client.c
//Created by: Project group 13
//Group members:
//              Venu Srinivasa Reddy (104699199, srinivav@uwindsor.ca)
//              Rahul Garg
//              Ashok Gurudayal
//              Mohammed Abdulwahab
//Created On: 25 March 2017
//Subject name and code: Advanced System Programming (03-60-656-1/2-2017W)
//Professor: Ziad Kobti
//***********************************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[])
{
	char message[2000];
	//char *servermessage;
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 3)
	{
		printf("Usage: %s <Ip Address> <Port#>\n",argv[0]);
		exit(0);
	}

	if((server = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<0)
	{
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}

	if(connect(server, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
	{
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}

	fprintf(stderr, "Connected successfully!\n");
	//while (1)
	{
		char buf[100];
		printf("Waiting to read()...\n");
		//read(server, buf, 100);
		//printf("buf: %s, len: %d\n", buf, (int)strlen(buf));
		
		//if (strcmp(buf, "EOS") != 0)
		//	strcat(message, buf);
		//else
		//{
		//	printf("Breaking first\n");
		//	break;
		//}
		if(read(server, message, 2000) < 0)
		{
			fprintf(stderr, "read() error\n");
			exit(3);
		}
	}
	//sleep(0.5);
	fprintf(stdout, "%s\n", message);

	while(1)
	{
		//memset(buf, 0, 100);
		//memset(message, 0, 2000);
		fprintf(stdout, "Enter the commands to send to the server. CTRL+D to Quit\n");
		if(fgets(message, 254, stdin) == NULL)
		{
			printf("Closing connection...\n");
			close(server);
			exit(0);
		}
		//printf("msg: %s\n", message);
		//if(message == NULL)
		//{
		//	close(server);
		//	exit(0);
		//}

		//Send the commands to the server for processing
		write(server, message, strlen(message) + 1);
		//printf("Wrote cmd to server\n");

		//memset(message, 0, 2000);
		char news[30000];
		printf("Waiting for response...\n");
                sleep(0.5);
                read(server, news, 30000);
		
		while (1)
        	{
			//char news[4096];
			//printf("Waiting for response...\n");
			//sleep(0.5);
			//read(server, temp, 4096);
			
			//if(read(server, temp, 100) < 0)
			//{
                	//	fprintf(stderr, "read() error\n");
                	//	exit(3);
			//}
	                
			/*printf("temp: %s, len: %d\n", temp, (int)strlen(temp));
			if (strcmp(temp, "EOS") != 0)
                	        strcat(message, temp);
			else
			{
				printf("Breaking\n");
				break;
			}*/

			//printf("Server Response:\n");

			/*
			char *str, cmd[15];
	                int argIndex = 0;
        	        str = strtok(message, " ");

			printf("You are subscribed to the channels: ");
                	while(str != NULL)
	                {
				if (argIndex > 0)
					printf("%s ", str);
				str = strtok(NULL, " ");
	                        argIndex++;
			}
			
//			printf("Item 1:\n");
			printf("Published Date: Fri, 31 Mar 2017 11:36:50 EDT\n");
			printf("Author: Cassie Williams\n");
			printf("More Details: http://www.cbc.ca/news/canada/nova-scotia/owen-gibson-skeir-sentenced-human-trafficking-1.4049642?cmp=rss\n");
			printf("Heading: Nova Scotia's 1st convicted human trafficker gets 7-year prison sentence\n");
			printf("Description: The first person convicted of human trafficking in Nova Scotia under federal legislation introduced more than a decade ago has been sentenced by a judge to seven years in prison.\n");
			printf("Image: https://i.cbc.ca/1.4049814.1490977215!/cpImage/httpImage/image.jpg_gen/derivatives/16x9_460/ns-human-trafficking-20170331.jpg\n");
			printf("=====================================================\n");

//			printf("Item 2:\n");
                        printf("Published Date: Fri, 31 Mar 2017 05:00:00 EDT\n");
                        printf("Author: Shanifa Nasser\n");
                        printf("More Details: http://www.cbc.ca/news/canada/toronto/muslim-students-praying-video-peel-1.4048991?cmp=rss\n");
                        printf("Heading: Video offering $1K reward for recordings of Muslim students praying ignites fears\n");
                        printf("Description: Staff with the Peel District School Board are being cautioned to be 'extra vigilant' about a video making the rounds online offering a $1,000 reward for recordings of Muslim students in any school in the region 'spewing hate speech during Friday Prayers.\n");
                        printf("Image: https://i.cbc.ca/1.4049001.1490927788!/fileImage/httpImage/image.png_gen/derivatives/16x9_460/kevin-johnston.png\n");
                        printf("=====================================================\n");

//			printf("Item 20:\n");
                        printf("Published Date: Fri, 30 Mar 2017 15:02:20 EDT\n");
                        printf("Author: David Burke\n");
                        printf("More Details: http://www.cbc.ca/news/canada/nova-scotia/passport-airlines-flights-air-canada-children-1.4047848?cmp=rss\n");
                        printf("Heading: Canadian family grounded because father signed child's passport\n");
                        printf("Description: A Halifax man wants airlines to warn families that parents are not allowed to sign the passports of their young children, after a U.S. customs agent ordered him to sign in order to enter the country.\n");
			printf("Image: https://i.cbc.ca/1.4047894.1490894786!/fileImage/httpImage/image.JPG_gen/derivatives/16x9_460/ahmad-hussein.JPG\n");
                        printf("=====================================================\n");

//			printf("Item 21:\n");
                        printf("Published Date: Fri, 31 Mar 2017 01:18:20 EDT\n");
                        printf("Author: Reuters\n");
                        printf("More Details: http://www.cbc.ca/news/world/venezuela-court-ruling-protests-1.4049076?cmp=rss\n");
                        printf("Heading: 'Maduro is now the National Assembly': Venezuela court ruling assailed across Americas\n");
                        printf("Description: Opposition leaders branded Venezuela's socialist President Nicolas Maduro a 'dictator' on Thursday after the Supreme Court took over the functions of Congress and pushed a lengthy political standoff to new heights.\n");
                        printf("Image: https://i.cbc.ca/1.4049080.1490936522!/fileImage/httpImage/image.jpg_gen/derivatives/16x9_460/venezuela-political-crisis.jpg\n");
                        printf("=====================================================\n");
			*/

			break;
		}//While loop

		//if(read(server, news, 2000) < 0)
        	//{
        	//}
		fprintf(stdout, "Server's reply: %s\n", news);
	}
}
