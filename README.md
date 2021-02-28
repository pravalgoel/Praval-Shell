# Praval-Shell
This is a shell which takes input from command line and using system calls processes it and gives final result.
Working- 

When shell runs ,main fuunction is callled which 'll when child process is executed, run the signal function and call sighandler which 'll store data of how process terminated and send to bradcasttermination, which 'll print how the precess terminated and call delete_from_list function. Now delete_from_child function 'll remive this child process' data and thus free upp space.
now, it will take command usinf=g shell_read_line and will send and split argument in shell_split_line using breaking points defined and 'll return to main. Now shell_execute function 'll take this array as argument and 'll check the first element of the list and check if it is in our builtin string and if yes 'll return builtin function corresponding to it and 'll send it to shell_launch function and it 'll execute the command using execvp function. Now, condition 'll repeat till either the command is competed or any signal interrupts the process.  

To use, open terminal and compile all the files using command.

gcc acmsh.c linkedlist.c linkedlist.h utilities.h utilities.c ./a.out 

Shell 'll open in the terminal.  
