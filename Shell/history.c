#include "history.h"

struct commandNode *commandList;
int commandIndex;

struct commandNode *insertCommand(char *command, int status)
{
	struct commandNode *newNode = (struct commandNode *)malloc(sizeof(struct commandNode)); 
	newNode->command = (char *)malloc(sizeof(command));
	strcpy(newNode->command, command);
	newNode->next = NULL;

	if (commandIndex == 10)
	{
		struct commandNode *p = commandList;
		for (int i = 0; i < 9; i++)
		{
			p = p->next;
		}
		free(p->next);
		p->next = NULL;
		commandIndex = 9;
	}

	if (commandIndex++ == 0)
		commandList = newNode;
	else
	{
		newNode->next = commandList;
		commandList = newNode;
	}
	return newNode;
}

void printCommands(int sig)
{
	struct commandNode *p = commandList;
	printf("\n");
	while (p != NULL)
	{
		printf("\n%s Status: %d", p->command, p->status);
		p = p->next;
	}
	printf("\n");
}