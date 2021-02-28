#include "utilities.h"

typedef struct process // 
{
	char* path;
	int pid;
	struct process* next;
} process;

struct process* create_list(int, char*);
struct process* search_in_list(int, struct process **);
struct process* add_to_list(int, char*, bool);
void print_list(void);
void kill_list();
int delete_from_list(int);

