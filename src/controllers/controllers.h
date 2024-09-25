#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "../utils/hash_table.h"
#include "../models/models.h"

typedef struct {
    HashTable users;
} tApp, *App;

App new_app();

void free_app(App app);

bool has_user(App app, char* name);

void register_user(App app, char* name);

char* register_space(App app, char* user_name);

bool no_sim_in_space(App app, char* name);

void remove_user(App app, char* name);

void list_users(App app); 

bool has_space(App app, char* name, char* space_id);

void remove_space(App app, char* space_id, char* name);

void* register_particle(App app, char* name, char* space_id, double mass, double charge, double px, double py, double pz, double vx, double vy, double vz);

bool has_particle(App App, char* name, char* space_id, char* particle_id);

char* change_particle(App app, char* name, char* space, char* particle, double mass, double charge, double px, double py, double pz, double vx, double vy, double vz);

char* remove_particle(App app, char* name, char* space, char* particle);

bool valid_mass(double mass);

void simulate(App app, char* name, char* space, char* particle, double time, double step, char* filename);

//void update_particle(Particles particle, double step);

bool valid_time(double time, double step);

void free_all_users(List list);

#endif