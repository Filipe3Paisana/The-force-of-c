#define _POSIX_C_SOURCE 200809L

#include "models.h"
#include <stdlib.h>
#include <string.h>

User new_user(char* name) {
    User user = malloc(sizeof(tUser));
    user->name = strdup(name);
    user->next_space_id = 1;
    user->simulation_spaces = hash_table_create(0, NULL, NULL, NULL, free_space);
    user->no_sim_in_space = true; 
    user->num_sim = 0;
    user->num_space = 0;
    return user;
}

void free_user(User user) {
    free(user->name);
    hash_table_destroy(user->simulation_spaces);
    free(user);
    // TODO Falta atualizar para os novos campos de User.
}

Space new_space(char* space_id) {
    Space space = malloc(sizeof(tSpace));
    space->id = strdup(space_id);
    space->particles = hash_table_create(0, NULL, NULL, NULL, free_particle);
    return space;
}

Particles new_particle(char* particle_id){
    Particles particle = malloc(sizeof(tParticles));
    particle->id = strdup(particle_id);
    return particle;
}


void free_space(void* space) {
    Space s = (Space)space;
    hash_table_destroy(s->particles);
    free(s->id);
    free(s);
}

void free_particle(void* particle) {
    Particles p = (Particles)particle;
    vector_free(p->position);
    vector_free(p->velocity);
    free(p->id);
    
    free(p);
}

