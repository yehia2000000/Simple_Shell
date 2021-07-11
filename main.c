#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define MAX_CHAR        100
#define MAX_WORD        10
void SimpleShell_voidRemoveEnter (char * Copy_charCommandLine);										//Function to replace the Enter button (/n) with a NULL (/0) so that the command is received correctly
void SimpleShell_voidReadLine(char* Copy_charCommandLine , int size);								//Function to read the command from the user
int SimpleShell_voidSeparatedArray(char * Copy_charSeparatArray[] , char * Copy_charCommandLine);	/*Function to separate the words of the command (if the command is multiple words)
																									and returns a counter representing the length of the command*/
int SimpleShell_intFindElement (char* Copy_charCommandArray[]);										//Function that searches for the element '&' to either operate in blocked or unblocked mode
void SimpleShell_voidHandler();																		/*Function to keep a log file containing the line “Child process was terminated”
																								    number of times a child process was terminated*/

int SimpleShell_intNumberArray ;

int main()
{
int state ;
char SimpleCommandLine[MAX_CHAR]; 																	// command line is entered by terminal
char* SimpleCommandLineArray[MAX_WORD] ;
int SimpleShell_OperationResult ;
int SimpleShell_intBackGround;

  while (1)
  {

    SimpleShell_voidReadLine(SimpleCommandLine,MAX_CHAR);													//Command is recieved from the user
    SimpleShell_intNumberArray=SimpleShell_voidSeparatedArray(SimpleCommandLineArray,SimpleCommandLine);	//Calculating the length of the entered command for later use
    SimpleShell_intBackGround = SimpleShell_intFindElement (SimpleCommandLineArray);						//Deciding to operate either in blocked or unblocked mode

    signal (SIGCHLD , SimpleShell_voidHandler);
    pid_t pid = fork();
    if (pid == -1)
    {
        exit(0);
    }
    else if (pid == 0)
    {
        if(strcmp(SimpleCommandLineArray[0],"cd")==0)
        {
            chdir(SimpleCommandLineArray[1]);

        }
        else
        {
            SimpleShell_OperationResult = execvp(SimpleCommandLineArray[0],SimpleCommandLineArray);
            if (SimpleShell_OperationResult == -1)
            {
                printf ("please enter right command \n");
            }
        }
    }
    else
    {

        if (SimpleShell_intBackGround == 1)
        {

        }
        else {
            waitpid(pid,&state ,0 );

        }
    }

  }

}

void SimpleShell_voidReadLine(char* Copy_charCommandLine , int size)							//Function to read the command from the user
{
fgets(Copy_charCommandLine,size,stdin);
SimpleShell_voidRemoveEnter (Copy_charCommandLine);
if (strcmp(Copy_charCommandLine,"exit")==0)														//Check if the command is to terminate the process
{
exit(0);
}
else{

//nothing
}

}

void SimpleShell_voidRemoveEnter (char * Copy_charCommandLine)									//Function to replace the Enter button (/n) with a NULL (/0) so that the command is received correctly
{
int Local_intCounter =0  ;
while(Copy_charCommandLine[Local_intCounter] != '\n')
{
Local_intCounter++;
}
Copy_charCommandLine[Local_intCounter] = '\0';
}

int SimpleShell_voidSeparatedArray(char * Copy_charSeparatArray[] , char * Copy_charCommandLine)	/*Function to seperate the words of the command (if the command is multiple words)
																									and returns a counter representing the length of the command*/
{

int Local_intCounter=0;

Copy_charSeparatArray[Local_intCounter] = strtok(Copy_charCommandLine, " ");

while (Copy_charSeparatArray[Local_intCounter]!= NULL)
{
Local_intCounter++;
Copy_charSeparatArray[Local_intCounter]= strtok(NULL," ");

}
return Local_intCounter;
}

int SimpleShell_intFindElement ( char* Copy_charCommandArray[])										//Function that searches for the element '&' to either operate in blocked or unblocked mode
{
    int local_intCounter ;
    for (local_intCounter=0;local_intCounter<SimpleShell_intNumberArray;local_intCounter++)
    {
        if (strcmp(Copy_charCommandArray[local_intCounter],"&")==0)
        {
        return 1 ;

        }
        else
        {

        }

    }
    return 0 ;


}
void SimpleShell_voidHandler()																		/*Function to keep a log file containg the line “Child process was terminated”
																							       	number of times a child process was terminated*/
{
int Local_intCounter ;
static int  Local_intNumber=0;
FILE * SimpleShell_FilePointer ;

SimpleShell_FilePointer = fopen("log_file.txt","w");

for (Local_intCounter=0 ; Local_intCounter<=Local_intNumber;Local_intCounter++)
{

fprintf (SimpleShell_FilePointer,"child process was terminate\n");

}
fclose(SimpleShell_FilePointer);

Local_intNumber++;
return 0;

}
