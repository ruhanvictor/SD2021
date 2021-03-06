/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/network_client.h"
#include "../include/client_stub-private.h"
#include "../include/sdmessage.pb-c.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/serialization.h"
#include "serialization.c"
#include "table.c"
#include "../include/extra/inet.h"

struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t *remote_table = malloc(sizeof(struct rtable_t));
    char *endpoint = malloc(sizeof(char)*strlen(address_port));
    strcpy(endpoint, address_port);
    strtok(endpoint, ":");
    remote_table->address = malloc(sizeof(char)); 
    
    memcpy(remote_table->address, endpoint, strlen(endpoint));
    remote_table->port = strtol(strtok(NULL,""), NULL, 10); 
    
    remote_table->server = malloc(sizeof(struct sockaddr_in ));

    if ((remote_table->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket TCP");
        return NULL;
    }
    struct sockaddr_in server_aux;
    server_aux.sin_family = AF_INET;
    server_aux.sin_port = htons(remote_table->port);
    if (inet_pton(AF_INET, remote_table->address, &server_aux.sin_addr) < 1) {
        printf("Erro ao converter IP\n");
        close(remote_table->sockfd);
        return NULL;
    }
    remote_table->server = &server_aux;
    free(endpoint);//look if this modify the remote_table
    network_connect(remote_table);
    return remote_table;
}

int rtable_disconnect(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->address != NULL){
            free(rtable->address);
        }
        network_close(rtable);
        return 0;
    }
    return -1;
}

int rtable_put(struct rtable_t *rtable, struct entry_t *entry){
    MessageT *msg, *msg_received;

    msg = malloc(sizeof(MessageT));

    message_t__init(msg); 

    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_ENTRY;

    msg->n_entries = 1;
    msg->entries = malloc(sizeof(MessageT__Entry *));

    MessageT__Entry *entry_tmp;
    entry_tmp = malloc(sizeof(MessageT__Entry));
    message_t__entry__init(entry_tmp);
    entry_tmp->key = entry->key;
    entry_tmp->data.len = entry->value->datasize;
    entry_tmp->data.data = malloc(entry->value->datasize);
    entry_tmp->data.data = entry->value->data;
    

    msg->entries[0] = entry_tmp;

    

    if((msg_received = network_send_receive(rtable, msg))!= NULL){
        free(msg);
        free(entry_tmp);
        message_t__free_unpacked(msg_received, NULL);
        return 0;
    }
    free(msg);
    return -1;
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){
    struct data_t *data;
    if(rtable != NULL && key != NULL){
        MessageT *msg, *msg_received;

        msg = malloc(sizeof(MessageT));

        message_t__init(msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_GET;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg-> n_keys = 1;
        msg->keys = malloc(sizeof(char*));
        msg->keys[0] = malloc(strlen(key)+1);
        strcpy(msg->keys[0], key);     
        msg_received = malloc(sizeof(MessageT));
        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            free(msg);
            if(msg_received->data.len != 0){
                data = data_create(msg_received->data.len);
                memcpy(data->data, msg_received->data.data, msg_received->data.len);
            }
            else{
                data = data_create(0);
            }
            message_t__free_unpacked(msg_received, NULL);
            return data;
        }
        free(msg);

    }
    return NULL;
}

int rtable_del(struct rtable_t *rtable, char *key){
    if(rtable != NULL && key != NULL){
        MessageT *msg, *msg_received;

        msg = malloc(sizeof(MessageT));

        message_t__init(msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg->n_keys = 1;
        msg->keys = malloc(sizeof(char*));
        msg->keys[0] = malloc(strlen(key)+1);
        strcpy(msg->keys[0], key);

        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            if(msg_received->opcode == MESSAGE_T__OPCODE__OP_DEL + 1){
                printf("chave %s apagada com sucesso.\n", key);
                return 0;
            }
            if(msg_received->opcode == MESSAGE_T__OPCODE__OP_ERROR){
                printf("chave %s não existe.\n", key);
            }
        }
        return -1;
    }
    return -1;
}

int rtable_size(struct rtable_t *rtable){
    if(rtable != NULL){
        MessageT *msg, *msg_received;
        msg = malloc(sizeof(MessageT));

        message_t__init(msg); 
        msg->opcode = MESSAGE_T__OPCODE__OP_SIZE;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            free(msg);
            
            if(msg_received->opcode == MESSAGE_T__OPCODE__OP_SIZE + 1){
                int result = msg_received->result;
                message_t__free_unpacked(msg_received, NULL);
                return result;
            }
            
        }
        free(msg);
    }
    printf("rtable = NULL\n");
    return -1;
}

char **rtable_get_keys(struct rtable_t *rtable){
    char **keys;
    if(rtable != NULL){
        MessageT *msg, *msg_received;

        msg = malloc(sizeof(MessageT));

        message_t__init(msg);

        msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

        msg_received = malloc(sizeof(MessageT));

        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            if(msg_received->c_type == MESSAGE_T__C_TYPE__CT_KEYS){
                keys = calloc((msg_received->n_keys+1), sizeof(char));
                if(msg_received->n_keys == 0){
                    return keys;
                }
                keys[0] = calloc(1, sizeof(char)); 
                //sprintf(keys[0], "%ld", msg_received->n_keys); 
                keys[0] = (char*) msg_received->n_keys;
                for(int i=1 ; i<(msg_received->n_keys+1) ; i++){
                    keys[i] = calloc(1, sizeof(char));
                    keys[i] = msg_received->keys[i-1];
                }
                return keys;
            }
        }
    }
    return NULL;
}

void rtable_free_keys(char **keys){ 
    if(keys != NULL){
        table_free_keys(keys);
    }
    printf("[ERROR] invalid argument keys");
}

void rtable_print(struct rtable_t *rtable){
    MessageT *msg, *msg_received;
    msg = malloc(sizeof(MessageT));
    message_t__init(msg); 
    
    msg->opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    msg_received = malloc(sizeof(MessageT));
    if((msg_received = network_send_receive(rtable, msg)) != NULL){
        if(msg_received->opcode == (MESSAGE_T__OPCODE__OP_PRINT + 1)){
            
            printf("remote_table: {\n");
            for(int i=0 ; i<msg_received->n_entries ; i++){
                printf("\tremote_entry_%d:[\n", i+1);
                printf("\t\tkey: %s,\n", (char*)msg_received->entries[i]->key);
                printf("\t\tdatasize: %d,\n",(int) msg_received->entries[i]->data.len);
                printf("\t\tdata: %s\n", (char*) msg_received->entries[i]->data.data);
                printf("\t]");
                if(i+1 != msg_received->n_entries){
                    printf(",\n");
                }
            }
            printf("}\n");
        }
    }
}

struct statistics *rtable_stats(struct rtable_t *rtable){
    MessageT *msg, *msg_received;
    struct statistics *statistics;
    msg = malloc(sizeof(MessageT));
    message_t__init(msg); 
    
    msg->opcode = MESSAGE_T__OPCODE__OP_STATS;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    
    if((msg_received = network_send_receive(rtable, msg)) != NULL){
        free(msg);
        if(msg_received->opcode == (MESSAGE_T__OPCODE__OP_STATS + 1)){
            statistics = malloc(sizeof(struct statistics));
            statistics->n_put = msg_received->stats[0];
            statistics->n_get = msg_received->stats[1];
            statistics->n_del = msg_received->stats[2];
            statistics->n_size = msg_received->stats[3];
            statistics->n_getkeys = msg_received->stats[4];
            statistics->n_table_print = msg_received->stats[5];
            statistics->avg_time = msg_received->avg_time;
            message_t__free_unpacked(msg_received, NULL);
            return statistics;
        }
        return NULL;
    }
    else{
        free(msg);
        return NULL; //TODO
    }
    
    
}

