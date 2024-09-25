#ifndef MODELS_H
#define MODELS_H

#include "../utils/hash_table.h"
#include "../utils/vectors.h"

typedef struct {
    char* name;
    int num_space;
    int num_sim;
    HashTable simulation_spaces;
    int num_spaces_total;
    int num_sim_total;
    int next_space_id;
    bool no_sim_in_space;
} tUser, *User;

typedef struct {
    char* id;
    int next_particle_id;
    bool was_simulated; 
    HashTable particles;
} tSpace, *Space;

typedef struct {
    char* id;
    double mass;
    double charge;
    double step;
    double time;
    Vector position;
    Vector velocity;
    Vector direction;
    Vector acceleration;
    Vector grav_force;
    Vector eletric_force;
    Vector total_force;
} tParticles, *Particles;


typedef struct {
    double step;
    double time; 
    Vector position;
    Vector velocity;
    Vector acceleration;
    Vector grav_force;
    Vector eletric_force;
    Vector total_force;
} tResults, *Results;


typedef struct {
    Vector p;
    Vector v;
    Vector a;
    Vector fg;
    Vector fe;
    Vector f;
}tFinal_Results, *Final_Results;

User new_user(char* name);

void free_user(User user);

Space new_space(char* space_id);

void free_space(void* space);

Particles new_particle(char* particle_id);

void free_particle(void* particle);

#endif