struct commandNode
{
	char *command;
	struct commandNode *next;
	int status;
};

struct commandNode *insertCommand(char *, int );
void printCommands(int);