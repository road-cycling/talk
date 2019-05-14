#define MAX_DATA 1
#define WAIT_QUEUE 10
#define num_rows 25
#define num_cols 80

#include <stdlib.h>

#include <unistd.h>
#include<curses.h>

void startup( void )
{
     initscr();	    /* activate curses                                            */
     curs_set(0);   /* prevent the cursor to be displayed                         */
     clear();	    /* clear the screen that curses provides                      */           
     noecho();	    /* prevent the input chars to be echoed to the screen         */      
     cbreak();	    /* change the stty so that characters are delivered to the
		       program as they are typed--no need to hit the return key!  */
	move(0, 0);
    addstr("--------------------------------------------------------------");

    move(20, 0);
    addstr("--------------------------------------------------------------");


     move(1, 0); 
     refresh();
}

void terminate( void )
{
     mvcur( 0, COLS - 1, LINES - 1, 0 );
     clear();
     refresh();
     endwin();
}

void sender(int cli_fd) {

    int recvCharOnScreen = 1300;

    while ( 1 ) {
        char c = getchar();

        if (c == '\n') continue;

        if (write(cli_fd, (char *)&c, 1) < 0) {
            perror("Write\n");
        }

        mvaddch( recvCharOnScreen / 65 + 1, recvCharOnScreen % 65, c );
        recvCharOnScreen++;
        refresh();
    }

}

void receiver(int cli_fd) {
    char data[MAX_DATA];

    int data_len = 1;
    int sentCharOnScreen = 65;


    while ( 1 ) {
        data_len = read(cli_fd, data, MAX_DATA);
        if ( data_len == -1 ) {
                printf("Error Recv \n");
                exit(-1);
        }

        if ( data_len == 2 ) {
            printf("Client is trying to exit\n");
            break;
        }

        if ( data_len ) {
            mvaddch( sentCharOnScreen / 65, sentCharOnScreen % 65, *data );
            sentCharOnScreen ++;
            refresh();

        }

    }

    printf("im done\n");

}