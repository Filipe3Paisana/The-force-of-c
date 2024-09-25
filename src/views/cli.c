#include "cli.h"
#include "../controllers/controllers.h"

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void run_cli() {
    char* line = NULL;
    size_t len = 0;
    App app = new_app();
    while (true) {
        getline(&line, &len, stdin);
        line[strlen(line) - 1] = '\0';
        if (strlen(line) == 0) {
            break;
        }
        char* command = strtok(line, " ");
        if (strcmp(command, "RJ") == 0) {
            char* name = strtok(NULL, " ");
            if (has_user(app, name)) {
                printf("Utilizador existente.\n");
            } else {
                register_user(app, name);
                printf("Utilizador registado com sucesso.\n");
            }
        } else if (strcmp(command, "EJ") == 0) {
            char* name = strtok(NULL, " ");
            if(!has_user(app, name)){
                printf("Utilizador não existente.\n");
            } else if(no_sim_in_space(app, name) == false){
                printf("Utilizador tem espaços de simulação sem simulações realizadas.\n");
            } else {
                remove_user(app, name);
                printf("Utilizador removido com sucesso.\n"); 
            }
        
        } else if (strcmp(command, "LJ") == 0) {
            
            if(hash_table_is_empty(app->users)){ 
                printf("Não existem utilizadores registados.\n");
            } else {
                list_users(app);
            }       
        }
         else if (strcmp(command, "RE") == 0) {
            char* name = strtok(NULL, " ");
            if (!has_user(app, name)) {
                printf("Utilizador inexistente.\n");
            } else {
                char* space_id = register_space(app, name);
                printf("Espaço de simulação registado com identificador %s.\n", space_id);
            }
        
        } else if (strcmp(command, "EE") == 0) {
            char* name = strtok(NULL, " ");
            char* space_id = strtok(NULL, " ");
            
            if (!has_user(app, name)) {
                printf("Utilizador inexistente.\n");
            } else if (!has_space(app,name, space_id)) {
                printf("Espaço de simulação inexistente.\n");
            } else {
                remove_space(app, space_id, name);
                printf("Espaço de simulação removido com sucesso.\n");
            }
        } else if (strcmp(command, "RP") == 0) {
            char* name = strtok(NULL, " ");
            char* space_id = strtok(NULL, " ");


            char* second_line = NULL;
            //size_t second_len = 0;
            
            getline(&second_line, &len, stdin);
            //second_line[strlen(second_line) - 1] = '\0';
            
            double mass = atof(strtok(second_line, " "));
            double charge = atof(strtok(NULL, " "));

            char* third_line = NULL;
            //size_t third_len = 0;
            //bool delete_particle = false;
            
            getline(&third_line, &len, stdin);
            //third_line[strlen(third_line) - 1] = '\0';
            
            double px = atof(strtok(third_line, " "));
            double py = atof(strtok(NULL, " "));
            double pz = atof(strtok(NULL, " "));
            
            char* fourth_line = NULL;
            //size_t fourth_len = 0;
            getline(&fourth_line, &len, stdin);
            //fourth_line[strlen(fourth_line) - 1] = '\0';
            
            double vx = atof(strtok(fourth_line, " "));
            double vy = atof(strtok(NULL, " "));
            double vz = atof(strtok(NULL, " "));

            if (!has_user(app, name)) {
                printf("Utilizador inexistente.\n");
            
            } else if (!has_space(app, name, space_id)) {
                printf("Espaço de simulação inexistente.\n");
            
            } else if(!valid_mass(mass)){
                printf("Massa inválida.\n");
            
            } else{
                
                char* particle_id = register_particle(app, name, space_id, mass, charge, px, py, pz, vx, vy, vz);
                printf("Partícula registada com identificador Identificador %s.\n", particle_id);
            }
            
            free(second_line);
            free(third_line);
            free(fourth_line);

        } else if (strcmp(command, "AP") == 0) {
            char* name = strtok(NULL, " ");
            char* space_id = strtok(NULL, " ");
            char* particle_id = strtok(NULL, " ");

            char *second_line = NULL;
            getline(&second_line, &len, stdin);

            double mass = atof(strtok(second_line, " "));
            double charge = atof(strtok(NULL, " "));

            char *third_line = NULL;
            getline(&third_line, &len, stdin);
            
            bool delete_particle = false;
            double px, py, pz;
            
            if(strcmp(third_line, "\n") == 0) {
                delete_particle = true;
            }
            else {
                px = atof(strtok(third_line, " "));
                py = atof(strtok(NULL, " "));
                pz = atof(strtok(NULL, " "));
            }

            char *fourth_line = NULL;
            getline(&fourth_line, &len, stdin);
            
            double vx = 0;
            double vy = 0;
            double vz = 0;
            
            if(strcmp(fourth_line, "\n") != 0) {
                vx = atof(strtok(fourth_line, " "));
                vy = atof(strtok(NULL, " "));
                vz = atof(strtok(NULL, " "));
            }
            if (!has_user(app, name))
            {
                printf("Utilizador inexistente.\n");
            }
            else if (!has_space(app, name, space_id))
            {
                printf("Espaço de simulação inexistente.\n");
            }
            else if (!has_particle(app, name, space_id, particle_id))
            {
                printf("Partícula inexistente.\n");
            }
            else if (!valid_mass(mass)) {
                printf("Massa inválida.\n");
            }
            else {
                
                if (!delete_particle) { 
                    change_particle(app, name, space_id, particle_id, mass, charge, px, py, pz, vx, vy, vz);
                    printf("Partícula alterada com sucesso.\n");
                
                } else {
                    remove_particle(app, name, space_id, particle_id);
                    printf("Partícula removida com sucesso.\n");
                }
            }

            free(second_line);
            free(third_line);
            free(fourth_line);

        } else if (strcmp(command, "S") == 0) {
            char* name = strtok(NULL, " ");
            char* space_id = strtok(NULL, " ");
            char* moving_particle = strtok(NULL, " ");
            
            char* second_line = NULL;
            getline(&second_line, &len, stdin);
            
            double time = atof(strtok(second_line, " "));
            double step = atof(strtok(NULL, " "));
            
            char *third_line = NULL;
            getline(&third_line, &len, stdin);

            char* filename = strtok(third_line, " ");
            //fprintf(stderr, "output: %s\n", filename);
            
            
            if (!has_user(app, name)) {
                printf("Utilizador inexistente.\n");
            
            } else if (!has_space(app,name, space_id)) {
                printf("Espaço de simulação inexistente.\n");     
            
            } else if (!valid_time(time, step)) {
                
                printf("Parametros inválidos.\n");
            } else {
                simulate(app, name, space_id, moving_particle, time, step, filename);
            }

            free(second_line);
            free(third_line);


        } else {
            printf("Instrução inválida.\n");
        }
        free(line);
        line = NULL;
    }
    if (line != NULL) {
        free(line);
    }
    free_app(app);
}







 