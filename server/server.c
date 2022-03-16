/*
** server.c -- a stream socket server demo
    Ian Havenaar, Simran Moolchandaney, Jacob Schwartz
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "../cJSON.h"
#include <time.h>
#include <sys/stat.h>



uint16_t rec_data_sz(int new_fd) {
    //receive data size
    uint16_t data_length;
    if(recv(new_fd, &data_length, sizeof(data_length), 0) == -1) {
        perror("recv");
        exit(1);
    }

    return data_length;
}
char *rec_data(int new_fd, uint16_t data_length) {
    // receive data
    char *data = malloc(data_length+1);
    data[data_length] = '\0';
    if(recv(new_fd, data, ntohs(data_length), 0) == -1) {
        perror("recv");
        exit(1);
    }

    return data;
}

#define BACKLOG 10   // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    fprintf(stdout, "line 43\n");
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    srand(time(NULL)); // for identifier

    if (argc != 2) {
        fprintf(stderr,"usage: server hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }
    fprintf(stdout, "line 92\n");

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    fprintf(stdout, "line 106\n");

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        fprintf(stdout, "line 119\n");
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        fprintf(stdout, "line 126\n");

        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
        printf("server: got connection from %s\n", s);
        fprintf(stdout, "line 129\n");

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener


            //check for errors when client asks for calendar stuff
            //char *error_message;
            char *data;
            
<<<<<<< HEAD
            char *calendar_name = rec_data(new_fd, rec_data_sz(new_fd));
=======
            //receive size of calendar name
            fprintf(stdout, "line 140\n");
            uint16_t calendar_length;
            if(recv(new_fd, &calendar_length, sizeof(calendar_length), 0) == -1) {
                fprintf(stdout, "line 143\n");
                perror("recv");
                exit(1);
            }

            fprintf(stdout, "line 148\n");
            // receive calendar name
            char calendar_name[calendar_length + 1];
            calendar_name[calendar_length] = '\0';
            if(recv(new_fd, calendar_name, ntohs(calendar_length), 0) == -1) {
                fprintf(stdout, "line 453\n");
                perror("recv");
                exit(1);
            }
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990

            fprintf(stdout, "line 158\n");
            //open calendar and put JSON in cal_JSON
<<<<<<< HEAD
            char file_name[BUFSIZ];
            strcat(file_name, "server/data/");
            strcat(file_name, calendar_name);
            strcat(file_name, ".json");

            printf("%s\n", file_name);
=======
            fprintf(stdout, calendar_name);
            char file_name[BUFSIZ];
            strcpy(file_name, "server/data/");
            strcat(file_name, calendar_name);
            strcat(file_name, ".json");
            fprintf(stdout, file_name);
            FILE *fp = fopen(file_name, "w+");
            fprintf(stdout, "post file 1\n");
            if (fp == NULL) {
                fprintf(stdout, "file check\n");
                fclose(fp);
                exit(1);
            }
            fprintf(stdout, "post file 2\n");

            struct stat st;
            stat(strcat(strcat("data/", calendar_name), ".json"), &st);
            uint16_t size = st.st_size;
            fprintf(stdout, "line 165\n");

            if(size == 0) {
                fprintf(stdout, "line 168\n");
                //need to create JSON file
                cJSON *date = NULL;
                cJSON *time = NULL;
                cJSON *duration = NULL;
                cJSON *name = NULL;
                cJSON *description = NULL;
                cJSON *location = NULL;
                cJSON *identifier_number = cJSON_CreateNumber((rand() % (999 - 100 + 1)) + 100); //create unique 3 digit identifier
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990

            FILE *fp = fopen(file_name, "r");

            /*
            fseek(fp, 0L, SEEK_END);
            int file_sz = ftell(fp);
            fseek(fp, 0L, SEEK_SET);

   
                cJSON *new_calendar = cJSON_CreateArray();
                cJSON *entry = cJSON_CreateObject();
                fprintf(stdout, "line 180\n");

                if(new_calendar == NULL || entry == NULL) {
                    perror("unable to write calendar.");
                    exit(1);
                }
                fprintf(stdout, "line 186\n");

                //cJSON_AddItemToObject(entry, "date", date);
                //cJSON_AddItemToObject(entry, "time", time);
                //cJSON_AddItemToObject(entry, "duration", duration);
                //cJSON_AddItemToObject(entry, "name", name);
                //cJSON_AddItemToObject(entry, "description", description);
                //cJSON_AddItemToObject(entry, "location", location);
                cJSON *identifier_number = cJSON_CreateNumber((rand() % (999 - 100 + 1)) + 100); //create unique 3 digit identifier
                cJSON_AddItemToObject(entry, "indetifier_number", identifier_number);
<<<<<<< HEAD
                
=======
                fprintf(stdout, "line 195\n");

>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990
                //add new null entry to new_calendar
                cJSON_AddItemToArray(new_calendar, entry);

                char *new_calendar_string = cJSON_Print(new_calendar);

<<<<<<< HEAD
                printf("%s\n", new_calendar_string);
                printf("%d\n", strlen(new_calendar_string));
                */

                /*
                if(fwrite(new_calendar_string, 1, strlen(new_calendar_string), fp) == -1) {
=======
                fprintf(stdout, "line 202\n");

                if(fwrite(new_calendar_string, sizeof(new_calendar_string), 1, fp) == -1) {
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990
                    perror("unable to write calendar");
                    exit(1);
                }

                fclose(fp);
                


            }*/

            fprintf(stdout, "line 210\n");
            //parse json file for calendar into cJSON object
            char calendar_buffer[BUFSIZ];
            if(fread(calendar_buffer, 1, BUFSIZ, fp) == -1) {
                perror("unable to read calendar");
                exit(1);
            }
            
            

            cJSON *calendar = cJSON_Parse(calendar_buffer);

<<<<<<< HEAD

            char *action = rec_data(new_fd, rec_data_sz(new_fd));
            printf("action: %s\n", action);
=======
            fprintf(stdout, "line 220\n");
            //receive size of action name
            uint16_t action_length;
            if(recv(new_fd, &action_length, sizeof(action_length), 0) == -1) {
                fprintf(stdout, "line 224\n");
                perror("recv");
                exit(1);
            }

            fprintf(stdout, "line 229\n");
            // receive action
            char action[action_length + 1];
            action[action_length] = '\0';
            if(recv(new_fd, action, ntohs(action_length), 0) == -1) {
                fprintf(stdout, "line 234\n");
                perror("recv");
                exit(1);
            }
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990

            if(!strcmp(action, "add")) {
                fprintf(stdout, "line 240\n");


                //receive number of fields to be added
<<<<<<< HEAD
                uint16_t num_fields = rec_data_sz(new_fd);

=======
                uint16_t num_fields;
                if(recv(new_fd, &num_fields, sizeof(num_fields), 0) == -1) {
                    fprintf(stdout, "line 245\n");
                    perror("recv");
                    exit(1);
                }
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990

                cJSON *entry = cJSON_CreateObject();
                fprintf(stdout, "line 251\n");

                for(int i = 0; i < num_fields/2; i++) {

<<<<<<< HEAD
                    char *field = rec_data(new_fd, rec_data_sz(new_fd));

                    char *field_value = rec_data(new_fd, rec_data_sz(new_fd));

=======
                    //receive size of field name
                    uint16_t field_length;
                    if(recv(new_fd, &field_length, sizeof(field_length), 0) == -1) {
                        fprintf(stdout, "line 258\n");
                        perror("recv");
                        exit(1);
                    }
                    fprintf(stdout, "line 262\n");
                    // receive field name
                    char field[field_length + 1];
                    field[field_length] = '\0';
                    if(recv(new_fd, field, ntohs(field_length), 0) == -1) {
                        fprintf(stdout, "line 267\n");
                        perror("recv");
                        exit(1);
                    }
                    fprintf(stdout, "line 271\n");
                    //receive size of field value
                    uint16_t field_value_length;
                    if(recv(new_fd, &field_value_length, sizeof(field_value_length), 0) == -1) {
                        fprintf(stdout, "line 275\n");
                        perror("recv");
                        exit(1);
                    }
                    fprintf(stdout, "line 279\n");
                    // receive field value
                    char field_value[field_value_length + 1];
                    field_value[field_value_length] = '\0';
                    if(recv(new_fd, field, ntohs(field_value_length), 0) == -1) {
                        fprintf(stdout, "line 284\n");
                        perror("recv");
                        exit(1);
                    }
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990

                    cJSON_AddStringToObject(entry, field, field_value);
                    fprintf(stdout, "line 291\n");
                }

                cJSON *identifier_number = cJSON_CreateNumber((rand() % (9999999 - 1000000 + 1)) + 1000000); //create unique 3 digit identifier
                cJSON_AddItemToObject(entry, "identifier", identifier_number);

                cJSON_AddItemToArray(calendar, entry);
                fprintf(stdout, "line 295\n");

                //write json object back to file
                char *calendar_string = cJSON_Print(calendar);
                fprintf(stdout, "line 299\n");

<<<<<<< HEAD
                fclose(fp);
                fopen(file_name, "w+");

                if(fwrite(calendar_string, 1, strlen(calendar_string), fp) == -1) {
=======
                if(fwrite(calendar_string, sizeof(calendar_string), 1, fp) == -1) {
                    fprintf(stdout, "line 302\n");
>>>>>>> ac53b8e695537a7b2939a1ebe84d8efb33ba0990
                    perror("unable to write calendar");
                    exit(1);
                }
                fprintf(stdout, "line 306\n");

            }
            else if(!strcmp(action, "remove")) {
               
                char *identifier = rec_data(new_fd, rec_data_sz(new_fd));

                //TODO PERFORM ACTION to remove event with identifier
                //TODO add error message if identifier does not exist
                int calendar_size = cJSON_GetArraySize(calendar);

                for(int i = 0; i < calendar_size; i++) {

                    cJSON *entry = cJSON_GetArrayItem(calendar, i);

                    int curr_identifier = cJSON_GetNumberValue(cJSON_GetObjectItem(entry, "identifier"));

                    if (atoi(identifier) == curr_identifier) {
                        printf("cya\n");
                       cJSON_DeleteItemFromArray(calendar, i);
                    }
                }

                //write json object back to file
                char *calendar_string = cJSON_Print(calendar);

                fclose(fp);
                fopen(file_name, "w+");

                if(fwrite(calendar_string, 1, strlen(calendar_string), fp) == -1) {
                    perror("unable to write calendar");
                    exit(1);
                }

            }
            else if(!strcmp(action, "update")) {
                
                char *identifier = rec_data(new_fd, rec_data_sz(new_fd));

                char *field = rec_data(new_fd, rec_data_sz(new_fd));

                //make sure identifier is not modified
                if(!strcmp(field, "identifier")) {
                    perror("identifier cannot be modifed");
                    exit(1);
                }

                char *field_value = rec_data(new_fd, rec_data_sz(new_fd));


                int calendar_size = cJSON_GetArraySize(calendar);
                for(int i = 0; i < calendar_size; i++) {
                    cJSON *entry = cJSON_GetArrayItem(calendar, i);
                    cJSON *curr_identifier = cJSON_GetObjectItem(entry, "identifier");
                    if (!strcmp(identifier, cJSON_Print(curr_identifier))) {
                        cJSON_ReplaceItemInObject(entry, field, cJSON_CreateString(field_value));
                    }
                }

                //write json object back to file
                char *calendar_string = cJSON_Print(calendar);

                fclose(fp);
                fopen(file_name, "w+");

                if(fwrite(calendar_string, sizeof(calendar_string), 1, fp) == -1) {
                    perror("unable to write calendar");
                    exit(1);
                }

            }
            else if(!strcmp(action, "get")) {
                
                char *date = rec_data(new_fd, rec_data_sz(new_fd));
                /*
                //TODO get events as json and send to client for particular date
                int calendar_size = cJSON_GetArraySize(calendar);
                cJSON *get_events = cJSON_CreateObject();
                cJSON *events = cJSON_CreateArray();

                for(int i = 0; i < calendar_size; i++) {
                    cJSON *entry = cJSON_GetArrayItem(calendar, i);
                    cJSON *curr_date = cJSON_GetObjectItem(entry, "date");
                    if (!strcmp(date, cJSON_Print(curr_date))) {
                        cJSON *event_name = cJSON_GetObjectItem(entry, "name");
                        cJSON_AddItemToArray(events, event_name);
                    }
                }

                cJSON *num_events = cJSON_CreateNumber(cJSON_GetArraySize(events));
                cJSON_AddItemToObject(get_events, "numevents", num_events);
                cJSON_AddItemToObject(get_events, "data", events);
                */
            }
            
            else if(!strcmp(action, "getrange")) {
                
                char *start_date = rec_data(new_fd, rec_data_sz(new_fd));

                char *end_date = rec_data(new_fd, rec_data_sz(new_fd));

                //TODO iterate through date range and get events

            }
            else if(!strcmp(action, "input")) {

                char *input = rec_data(new_fd, rec_data_sz(new_fd));

                cJSON *input_list = cJSON_Parse(input);

                
             
            }

            else {
                perror("not a valid action");
                exit(1);
            }

            
            /*

            //create response json
            cJSON *response = cJSON_CreateObject();
            if(response == NULL) {
                perror("JSON response could not be created");
                exit(1);
            }

            cJSON_AddItemToObject(response, "command", cJSON_CreateString(action));
            cJSON_AddItemToObject(response, "calendar", cJSON_CreateString(calendar_name));
            //cJSON_AddItemToObject(response, "identifier", success ? identifier_number: "XXXX");
            //cJSON_AddItemToObject(response, "success", success ? "True": "False");
            //cJSON_AddItemToObject(response, "error", error_message); //TODO get error messages
            //cJSON_AddItemToObject(response, "data", data);

            //TODO send response json object to client

            uint16_t response_size = htons(sizeof(response));//miracle if this is right

            //send size of JSON
            if((send(sockfd, &response_size, sizeof(response_size), 0)) == -1) {
                perror("recv");
                exit(1);
            }

            //send JSON, no way this actually works
            if((send(sockfd, response, sizeof(response), 0)) == -1) {
                perror("recv");
                exit(1);
            }
            */
			fclose(fp);
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    
    }
    return 0;
}