/*
 * Copyright (c) 2013, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CLI_H
#define _CLI_H

#if ARDUINO >= 100
# include <Arduino.h>
#else
# include <WProgram.h>
#endif

#include <stdlib.h>

#define CLI_BUFFER 80 

class CLIServer;

class CLIClient : public Print {
    private:
        Stream *dev;
        char input[CLI_BUFFER];
        int pos;

    public:
        CLIClient(Stream *d);
        int readline();
        int parseCommand();

#if (ARDUINO >= 100) 
        size_t write(uint8_t);
#else
        void write(uint8_t);
#endif
        
    friend class CLIServer;
};

#define CLI_COMMAND(X) int X(CLIClient *dev, int argc, char **argv)

typedef struct _CLIClientList {
    CLIClient *client;
    struct _CLIClientList *next;
} CLIClientList;

typedef struct _CLICommand {
    char *command;
    int (*function)(CLIClient *, int, char **);
    struct _CLICommand *next;
} CLICommand;

class CLIServer : public Print {
    private:
        CLIClientList *clients;
        CLICommand *commands;

    public:
        CLIServer();
        void addCommand(const char *command, int (*function)(CLIClient *, int, char **));
        void addClient(Stream *dev);
        void addClient(Stream &dev);
        void process();
        void broadcast(char *);
#if (ARDUINO >= 100) 
        size_t write(uint8_t);
#else
        void write(uint8_t);
#endif

    friend class CLIClient;
};

extern CLIServer CLI;

#endif
