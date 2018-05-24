//************************************************************************************************/
//This program waits for TCP connections from clients, on accepting the client's connection,
//It forks a child process and lets the child process handle the commands sent by the client.

//Program Name: feedmenews
//File Name: feedmenews.c
//Created by: Project group 13
//Group members:
//		Venu Srinivasa Reddy (104699199, srinivav@uwindsor.ca)
//		Rahul Garg
//		Ashok Gurudayal
//		Mohammed Abdulwahab
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
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

struct item
{
	char *pubTime;
	char *heading;
	char *url;
	char *author;
	char *description;
};
typedef struct item ITEM;

struct itemGroup
{
	char *category;
	struct item items[10];
};
typedef struct itemGroup ITEMGROUP;

struct responseData
{
	struct itemGroup groups[10];
};
typedef struct responseData RESPONSE;

//Global variable
static char *categories[11] = { "top", "canada", "sports", "world", "politics", "business", "tech", "windsor", "nba", "soccer", NULL };

//Method prototypes
static void read_elements(xmlDocPtr doc, xmlNode *a_node, char *news);
void child(int sd);
static char* getUrlForCategory(char *category);
void readItem(xmlDocPtr doc, xmlNode *item_node, char *news);
void subscriptionHandler();
xmlXPathObjectPtr GetNodeSet(xmlDocPtr doc, xmlChar *xpath);

int main(int argc, char *argv[])
{
	int sd, client, portNumber;
	//socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 2)
	{
		printf("Call model: %s <Port #>\n", argv[0]);
		exit(0);
	}

	if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
	listen(sd, 5);

	while(1)
	{
		printf("Server: Waiting for a client asynchronously...\n");
		client = accept(sd,(struct sockaddr*)NULL,NULL);
		printf("Server: Got a client!\n");
		pid_t pid;
		pid = fork();
		
		//printf("pid %d\n", (int)pid);
		if(pid == 0)
			child(client);
		close(client);
	}
}

/*
 * Serves one client. Called by child process.
 * Input: sd - Socket descripter of the client
 */
void child(int sd)
{
	char welcome_msg[2000];
	char message[2000];
	char *subscription_cats[10];
	char news[30000];

	//printf("\nChild[%d]: %s\n", sd, "Welcome");
	strcat(welcome_msg, "-*-*-*-*-*-*-*-*-Welcome to FEEDMENEWS service by Group-13-*-*-*-*-*-*-*-*-*-\n\nA command interpreter which provides latest news on the required categories.\n\n=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=\n\n");
	strcat(welcome_msg, "Commands available:\n1 - feedmenews <categories_list>\n2 - subscribe <categories_list>\n\n");
	strcat(welcome_msg, "Following are the categories available to check news or to subscribe:\n");

	int i;
	for (i=0; i < 11; i++)
	{
		if (categories[i] != NULL)
		{
			//printf("cat: %s\n", categories[i]);
			strcat(welcome_msg, "-");
			strcat(welcome_msg, categories[i]);
			strcat(welcome_msg, "\n");
		}
	}

	//printf("Welcome msg: %s\n", welcome_msg);
	
	write(sd, welcome_msg, strlen(welcome_msg) + 1);

	//Write the response to the client socket
	//ITEM *newItem = (ITEM *)malloc(sizeof(ITEM));
	ITEM newItem;
	//if(0)//newItem == NULL)
	//	perror("Error malloc\n");
	//else
	//{
		newItem.pubTime = "This is the publish date";
		newItem.author = "I'm the author";
		newItem.url = "This is the URL";
		newItem.heading = "This is heading";
	//}
	//memset(welcome_msg, 0, strlen(welcome_msg));
	//printf("After memset(): %s, len: %d\n", welcome_msg, (int)strlen(welcome_msg));

	//printf("%s\n", newItem.pubTime);
	//printf("%s\n", newItem.author);

	//write(sd, "This is another line", 21);
	//sleep(1);
	//write(sd, "EOS", 4);
	//printf("Wrote EOF to client...\n");

	while(1)
	{
		memset(message, 0, 2000);
		printf("Waiting for input...\n");
		if(!read(sd, message, 2000))
		{
			close(sd);
			fprintf(stderr,"Bye, client [%d] is dead\n", sd);
			exit(0);
		}
		//TO-DO: Get the commands from the client and execute them on the server and send back the results
		fprintf(stdout, "Client's command: %s\n", message);
		//sleep(2);

		//Print the command and the arguments sent by the client
		char *str, cmd[15];
		int argIndex = 0;
		str = strtok(message, " ");
		
		while(str != NULL)
		{
			//printf("starting loop...");
			//printf("%d\n", argIndex);
			//Remove the new line char from the token
			char *pos;
			if ((pos=strchr(str, '\n')) != NULL)
				*pos = '\0';

			//Store the command
			//printf("Before copy. Command: %s, len: %d\n", str, (int)strlen(str));
			//strcpy(cmd, str);
			//printf("Command: %s, len: %d\n", cmd, (int)strlen(cmd));

			if(argIndex == 0)
			{
				strcpy(cmd, str);
				//printf("Command: %s, len: %d\n", str, (int)strlen(str));
                                if (strcmp(str, "feedmenews") != 0 && strcmp(str, "subscribe") != 0)
                                {
					//printf("Client gave invalid command\n");
                                        write(sd, "Invalid Command", 16);
                                        break;
                                }
				//if(strcmp(str, "subscribe") == 0)
				//	printf("Client is subscribed to the following channels: ");
				if(strcmp(str, "feedmenews") == 0)
					strcat(news, "<responseData>");
			}
			else
			{
				if (strcmp(cmd, "feedmenews") == 0)
				{
					strcat(news, "<itemGroup>");
					strcat(news, "<category>");
					strcat(news, str);
					strcat(news, "</category>");

					//printf("feedmenews command with arg[%d]: %s. len=%d\n", argIndex, str, (int)strlen(str));

					//Get the rss feed for the categories
			                xmlNode *root_element = NULL;
                
		                	xmlDocPtr xmlDoc = NULL;
 
        	        		char* url = getUrlForCategory(str);
					printf("url: %s\n", url);
					if(strcmp(url, "") != 0)
					{
						xmlDoc = xmlReadFile(url, NULL, XML_PARSE_NOCDATA);
						root_element = xmlDocGetRootElement(xmlDoc);
						read_elements(xmlDoc, root_element, news);
		                		xmlFreeDoc(xmlDoc);
					}
					else
					{
						write(sd, "Invalid categories", 20);
						break;
					}
					strcat(news, "</itemGroup>");
				}
				else if(strcmp(cmd, "subscribe") == 0)
				{
					//printf("Client entered subscribe command\n");
					char* url = getUrlForCategory(str);
					if(strcmp(url, "") != 0)
                                        {
						//Add the url to the array
						subscription_cats[argIndex-1] = url;
						printf("%s ", str);
                                        }
                                        else
                                        {
                                                write(sd, "Invalid categories", 20);
                                                break;
                                        }
				}
			}
			str = strtok(NULL, " ");
			argIndex++;
		}

		//printf("Command: %s\n", cmd);
		if (strcmp(cmd, "subscribe") == 0)
		{
			printf("Calling subscription handler...\n");
			//subscriptionHandler(subscription_cats);
		}
		else
			strcat(news, "</responseData>");

		//printf("\n\nNews string:\n %s\n", news);

		write(sd, news, strlen(news)+1);

		//printf("Response has been sent to the client!\n");

		printf("\n\nNews len: %d\n", strlen(news));
		//Write the response to the client socket
		//printf("Check msg\n");
		if (news != NULL && !strcmp(news, "") && !strcmp(news, "\n"))
		{
			printf("Check msg2\n");
			write(sd, news, strlen(news)+1);
		}
		//printf("Check msg end\n");
		//write(sd, "EOS", 4);
		//printf("Wrote EOS to client. End\n");
	}
}

/*
 * read_elements:
 * @doc: xml document pointer
 * @a_node: the initial xml node to consider.
 *
 * Reads the child nodes of the given xml element
 */
static void read_elements(xmlDocPtr doc, xmlNode *a_node, char *news)
{
        //xmlNode *cur_node = NULL;

	int i;
        xmlChar *keyword;
	xmlChar *xpath = (xmlChar*) "//item";
	xmlXPathObjectPtr result = GetNodeSet(doc, xpath);
	xmlNodeSetPtr nodeset;

	if (result)
	{
		nodeset = result->nodesetval;
		for (i=0; i < 5 /*nodeset->nodeNr*/; i++)
		{
			//keyword = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
			//printf("keyword: %s\n", keyword);
			//xmlFree(keyword);

			//printf("%d\n", nodeset->nodeNr);
			strcat(news, "<item>");
			readItem(doc, nodeset->nodeTab[i], news);
			strcat(news, "</item>");
		}
		xmlXPathFreeObject(result);
	}
	//xmlFreeDoc(doc);
	//xmlCleanupParser();

	/*
	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
        {
                if (cur_node->type == XML_ELEMENT_NODE)
                {
			//printf("CurNode: %s\n", cur_node->name);
			//printf("=%d\n", strcmp(cur_node->name, "item"));
			if (strcmp(cur_node->name, "item") == 0)
			{
				strcat(news, "<item>");

				//printf("node type: Element, name: %s, %s\n", cur_node->name, cur_node->content);
				//printf("String 'item' found!\n");
				//key = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
				//printf("Node: %s, Content: %s\n", cur_node->name, key);
				//xmlFree(key);

				//Parse one 'item' node
				//readItem(doc, cur_node, news);
				strcat(news, "</item>");
                        }  
                }
                else
		{

                }
		//read_elements(doc, cur_node->children, news);
        }
	*/
}

void readItem(xmlDocPtr doc, xmlNode *cur, char* news)
{
	xmlChar *key = NULL;

	//Read 'title'
	//Read 'link'
	//Read 'pubDate'
	//Read 'author'
	//Read 'description'
	
/*	strcat(news, "<pubTime></pubTime>");
	strcat(news, "<author></author>");
	strcat(news, "<url></url>");
	strcat(news, "<heading></heading>");
	strcat(news, "<description></description>");
	strcat(news, "<img></img>");
*/
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if (strcmp(cur->name, "title") == 0)
		{
			key = xmlNodeGetContent(cur);
			//printf("title: %s\n", key);
			strcat(news, "<heading>");
			strcat(news, key);
			strcat(news, "</heading>");
			xmlFree(key);
		}
		else if (strcmp(cur->name, "link") == 0)
                {
                        key = xmlNodeGetContent(cur);
			//printf("link: %s\n", key);
			strcat(news, "<url>");
			strcat(news, key);
			strcat(news, "</url>");
                        xmlFree(key);
                }
		else if (strcmp(cur->name, "pubDate") == 0)
                {
                        key = xmlNodeGetContent(cur);
			//printf("pubDate: %s\n", key);
			strcat(news, "<pubTime>");
			strcat(news, key);
			strcat(news, "</pubTime>");
                        xmlFree(key);
                }
		else if (strcmp(cur->name, "author") == 0)
                {
                        key = xmlNodeGetContent(cur);
			//printf("author: %s\n", key);
			strcat(news, "<author>");
			strcat(news, key);
			strcat(news, "</author>");
                        xmlFree(key);
                }
		else if (strcmp(cur->name, "description") == 0)
                {
			key = xmlNodeGetContent(cur);
			//printf("description: %s\n", key);
			strcat(news, "<description>");
			strcat(news, key);
			strcat(news, "</description>");
			xmlFree(key);
                }
		cur = cur->next;
	}//while loop

	/*
	while (cur != NULL)
	{
		printf("%s\n", cur->name);
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"title")))
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("title: %s\n", key);
			xmlFree(key);
 		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"link")))
		{
                	key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                	printf("link: %s\n", key);
                	xmlFree(key);
        	}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"pubDate")))
                {
                        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                        printf("pubdate: %s\n", key);
                        xmlFree(key);
                }
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"author")))
                {
                        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                        printf("author: %s\n", key);
                        xmlFree(key);
                }
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"description")))
                {
                        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                        printf("description: %s\n", key);
                        xmlFree(key);
                }
		cur = cur->next;
	}*/
}

/*
 * Returns the RSS feed url for ta particular category
 * Input: category name
 * Output: RSS feed url
 */
static char* getUrlForCategory(char *category)
{
        char* url;
	if (strcmp(category, "top") == 0)
                url = "http://rss.cbc.ca/lineup/topstories.xml";

	else if (strcmp(category, "canada") == 0)
                url = "http://rss.cbc.ca/lineup/canada.xml";

	else if (strcmp(category, "sports") == 0)
                url = "http://rss.cbc.ca/lineup/sports.xml";

	else if (strcmp(category, "world") == 0)
		url = "http://rss.cbc.ca/lineup/world.xml";

	else if (strcmp(category, "politics") == 0)
		url = "http://rss.cbc.ca/lineup/politics.xml";

	else if (strcmp(category, "business") == 0)
		url = "http://rss.cbc.ca/lineup/business.xml";

	else if (strcmp(category, "tech") == 0)
		url = "http://rss.cbc.ca/lineup/technology.xml";

	else if (strcmp(category, "windsor") == 0)
		url = "http://rss.cbc.ca/lineup/canada-windsor.xml";

	else if (strcmp(category, "nba") == 0)
		url = "http://rss.cbc.ca/lineup/sports-nba.xml";

	else if (strcmp(category, "soccer") == 0)
		url = "http://rss.cbc.ca/lineup/sports-soccer.xml";
	else
		url = "";
	//{ "top", "canada", "sports", "world", "politics", "business", "tech", "windsor", "nba", "soccer", NULL };
	return url;
}

void subscriptionHandler(char *subscription_cats[10])
{
	//int i = 0;
	//Wait for 5 minutes
	sleep(2);
	//Call the CBC server to get the xml
	/*
	for (i=0; i<10; i++)
	{
		if(subscription_cats[i] != NULL)
			printf("Calling subscription... %s\n", subscription_cats[i]);
	}
	*/
}

xmlXPathObjectPtr GetNodeSet(xmlDocPtr doc, xmlChar *xpath)
{
        xmlXPathContextPtr context;
        xmlXPathObjectPtr result;

        context = xmlXPathNewContext(doc);
        result = xmlXPathEvalExpression(xpath, context);
        if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
                xmlXPathFreeObject(result);
                printf("No result\n");
                return NULL;
	}
	return result;
}
