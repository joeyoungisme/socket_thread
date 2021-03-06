#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "sft.h"
#include "pdu.h"
#include "err.h"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        call_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    SFT_DATA *client = sft_create(SFT_CLIENT);
    if(client == NULL) {
        fprintf(stderr, "client sft_create failed\n");
        exit(EXIT_FAILURE);
    }

    if(0 != client->action->init(client)) {
        fprintf(stderr, "client sft_init failed\n");
        exit(EXIT_FAILURE);
    }

    if(0 != client->action->connect(client, "127.0.0.1", 9420)) {
        fprintf(stderr, "client sft_connect failed\n");
        exit(EXIT_FAILURE);
    }

    char command[256];
    do {
        printf("Command >> ");
        if(!(fgets(command, 256, stdin))) {
            fprintf(stderr, "get command failed!\n");
            sft_destroy(client);
            exit(EXIT_FAILURE);
        }

        if(!strchr(command, '\n'))
            while(fgetc(stdin) != '\n');

        if(client->action->send(client, (void *)command, strlen(command)) <= 0) {
            fprintf(stderr, "client send command failed\n");
            sft_destroy(client);
            exit(EXIT_FAILURE);
        }

    }while(strncmp(command, "quit", 4) != 0);

    sft_destroy(client);

    return 0;
}

