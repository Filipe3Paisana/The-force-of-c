#include "controllers.h"
#include "../models/models.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//TESTE
App new_app() {
    App app = malloc(sizeof(tApp));
    app->users = hash_table_create(0, NULL, NULL, NULL, (void (*)(void*))free_user);
    return app;
}


void free_app(App app) {
    hash_table_destroy(app->users);
    
    free(app);
}

bool has_user(App app, char* name) {
    return hash_table_get(app->users, name) != NULL;
}

bool has_space(App app, char* name, char* space_id) {
    User user = hash_table_get(app->users, name);                       
    return (hash_table_get(user->simulation_spaces, space_id) != NULL);
}

//esta é nossa                                                         
void* get_user(App app, char* name ){                                 
    User user = hash_table_get(app->users, name);                         
    return user;                                                      
}                                                                     

bool no_sim_in_space(App app, char* name){
    User user = hash_table_get(app->users, name);
    if (user->no_sim_in_space == true){
        return true;
    }
    else{
        return false;
    }
}
/*
void remove_user(App app, char* name){
    hash_table_remove(app->users, name);
    free_user(app->users);
}
*/
void remove_user(App app, char* name){
    User user = hash_table_get(app->users, name);
    if (user != NULL) {
        hash_table_remove(app->users, name);
        free_user(user);
    }
}

void register_user(App app, char* name) {
    User user = new_user(name);
    hash_table_insert(app->users, user->name, user);
} 

int compare_users(const void* a, const void* b) {
    User user_a = *(User*)a;
    User user_b = *(User*)b;
    return strcmp(user_a->name, user_b->name);    
}

void list_users(App app) {
    
    List user_list = hash_table_values(app->users);
    int size = list_size(user_list);
    User* user_array = malloc(sizeof(User) * size);
    list_to_array(user_list, (void**)user_array);
    list_iterator_start(user_list);
    int i= 0;
    while(list_iterator_has_next(user_list)) {
        User user = list_iterator_get_next(user_list);
        user_array[i++] = user;
    }
    // Ordenar o array
    qsort(user_array, size, sizeof(char *), compare_users);
    // Mostrar o array
    for(int i=0; i<size; i++) {
        User user = user_array[i];
        printf("%s %d %d\n", user->name, user->num_spaces_total, user->num_sim);
    }
    for(int i=0; i<size; i++) {
        User user = user_array[i];
        free_user(user);
    }
    free(user_array);
    free(user_list);
    

}
// não esquecer de fazer free

void remove_space(App app, char* space_id, char* name){
    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    hash_table_remove(user->simulation_spaces, space_id);
    user->next_space_id-- ;
    user->num_spaces_total-- ;
    free_space(space);
}


char* register_space(App app, char* user_name) {
    User user = hash_table_get(app->users, user_name);
    // 0. Criar o identificador de espaço
    char space_id[80];
    sprintf(space_id, "%d", user->next_space_id);
    user->next_space_id++;
    user->num_spaces_total++;
    // 1. Criar um novo espaço
    Space space = new_space(space_id);
    // 2. Colocar o espaço na tabela de dispersão de espaços de simulação, do utilizador
    hash_table_insert(user->simulation_spaces, space->id, space);
    user->no_sim_in_space = true; 
    return space->id;
}

void* register_particle(App app, char* name, char* space_id, double mass, double charge, double px, double py, double pz, double vx, double vy, double vz) {
    
    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    // criar o identificador de partícula
    char particle_id[80]; //key
    space->next_particle_id++;
    sprintf(particle_id, "%d", space->next_particle_id);
    
    // criar uma nova partícula
    Vector position =  vector_new(px, py, pz);
    Vector velocity =  vector_new(vx, vy, vz);
    Particles particle = new_particle(particle_id);
    particle->mass = mass;
    particle->charge = charge;
    particle->position = position;
    particle->velocity = velocity;
    // colocar a partícula na tabela de dispersão de partículas
    hash_table_insert(space->particles, particle->id, particle);
  
    return particle->id;
}


bool has_particle(App app, char* name, char* space_id ,char* particle_id) {
    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);                       
    if(hash_table_get(space->particles, particle_id) != NULL){
        return true;
    } else {
        return false;
    }
}


/*
void simulate(App app, char* name, char* space_id, char* particle_id, double time, double step, char* filename) {

    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    Particles moving_particle = hash_table_get(space->particles, particle_id);
    List particles_list = hash_table_values(space->particles);

    double num_steps = time/step;
    int size = list_size(particles_list);
    Results* results_array = malloc(sizeof(Results) * (size - 1));
    
    list_iterator_start(particles_list);
    int i = 0;
    while(list_iterator_has_next(particles_list)) {
        
        Particles static_particle = list_iterator_get_next(particles_list);    
       
        if(moving_particle->id != static_particle->id){ 
            //contas para uma particula de cada vez
            results_array[i] = malloc(sizeof(Results));
            results_array[i]->position = malloc(sizeof(Vector));
            results_array[i]->velocity = malloc(sizeof(Vector));
            results_array[i]->acceleration = malloc(sizeof(Vector));
            results_array[i]->grav_force = malloc(sizeof(Vector));
            results_array[i]->eletric_force = malloc(sizeof(Vector));
            results_array[i]->total_force = malloc(sizeof(Vector));
    
            double const_gravity = 6674184 *10^-17;
            double const_eletric = 89875517923 *10^-1;
            
            double dx = static_particle->position->x - moving_particle->position->x;
            double dy = static_particle->position->y - moving_particle->position->y;
            double dz = static_particle->position->z - moving_particle->position->z;
            
            double fgx = const_gravity * moving_particle->mass * static_particle->mass / dx * dx;
            double fgy = const_gravity * moving_particle->mass * static_particle->mass / dy * dy;
            double fgz = const_gravity * moving_particle->mass * static_particle->mass / dz * dz;
            results_array[i]->grav_force->x = fgx;
            results_array[i]->grav_force->y = fgy;
            results_array[i]->grav_force->z = fgz;
            
            double fex = const_eletric * moving_particle->charge * static_particle->charge / dx * dx;
            double fey = const_eletric * moving_particle->charge * static_particle->charge / dy * dy;
            double fez = const_eletric * moving_particle->charge * static_particle->charge / dz * dz;
            results_array[i]->eletric_force->x = fex;
            results_array[i]->eletric_force->y = fey;
            results_array[i]->eletric_force->z = fez;

            double fx = fgx + fex;
            double fy = fgy + fey;
            double fz = fgz + fez;
            results_array[i]->total_force->x = fx;
            results_array[i]->total_force->y = fy;
            results_array[i]->total_force->z = fz;

            double ax = fx / moving_particle->mass;
            double ay = fy / moving_particle->mass;
            double az = fz / moving_particle->mass;
            results_array[i]->acceleration->x = ax;
            results_array[i]->acceleration->y = ay;
            results_array[i]->acceleration->z = az;
                 
            
            double vx = moving_particle->velocity->x + ax * step;
            double vy = moving_particle->velocity->y + ay * step;
            double vz = moving_particle->velocity->z + az * step;
            results_array[i]->velocity->x = vx;
            results_array[i]->velocity->y = vy;
            results_array[i]->velocity->z = vz;

            double px = moving_particle->position->x + vx * step + ax * step*step/2; 
            double py = moving_particle->position->y + vy * step + ay * step*step/2 ;
            double pz = moving_particle->position->z + vz * step + az * step*step/2 ;
            results_array[i]->position->x = px;
            results_array[i]->position->y = py;
            results_array[i]->position->z = pz;
            // aceleração, velocidade e posição vão ser calculadas com a força final (depois de ler o array)
            
            //malloc resultados para um array de structs de resultados
            i++;   
        }                    
    }    
    //User user1 = hash_table_get(app->users, name);
    //Space space1 = hash_table_get(user->simulation_spaces, space_id);
    //Particles moving_particle1 = hash_table_get(space->particles, particle_id);
    // printf ("%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", step, moving_particle1->position->x, moving_particle1->position->y, moving_particle1->position->z, moving_particle1->velocity->x, moving_particle1->velocity->y, moving_particle1->velocity->z, moving_particle1->acceleration->x, moving_particle1->acceleration->y, moving_particle1->acceleration->z, moving_particle1->grav_force->x, moving_particle1->grav_force->y, moving_particle1->grav_force->z, moving_particle1->eletric_force->x, moving_particle1->eletric_force->y, moving_particle1->eletric_force->z, moving_particle1->total_force->x, moving_particle1->total_force->y, moving_particle1->total_force->z);                           
    // isto é para ler os valores que estão no array
    // t,px,py,pz,vx,vy,vz,ax,ay,az,fgx,fgy,fgz,fex,fey,fez,fx,fy,fz
    // 0,px0,py0,pz0,vx0,vy0,vz0,ax0,ay0,az0,fgx0,fgy0,fgz0,fex0,fey0,fez0,fx0,fy0,fz0
    for(int i = 0; i < num_steps; i++){
        double fgx = results_array[i]->grav_force->x; 
        double fgy = results_array[i]->grav_force->y;
        double fgz = results_array[i]->grav_force->z;
        fgx += fgx;
        fgy += fgy;
        fgz += fgz;

        double fex = results_array[i]->eletric_force->x;
        double fey = results_array[i]->eletric_force->y;
        double fez = results_array[i]->eletric_force->z;
        fex += fex;
        fey += fey;
        fez += fez;   

        double fx = results_array[i]->total_force->x;
        double fy = results_array[i]->total_force->y;
        double fz = results_array[i]->total_force->z;
        fx += fx;
        fy += fy;
        fz += fz;            

        double ax = fx / moving_particle->mass;
        double ay = fy / moving_particle->mass;
        double az = fz / moving_particle->mass;

        double vx = moving_particle->velocity->x + ax * step;
        double vy = moving_particle->velocity->y + ay * step;
        double vz = moving_particle->velocity->z + az * step;
         
        results_array[i]->velocity->x = vx;
        results_array[i]->velocity->y = vy;
        results_array[i]->velocity->z = vz;
        results_array[i]->acceleration->x = ax;
        results_array[i]->acceleration->y = ay;
        results_array[i]->acceleration->z = az;

        double px = moving_particle->position->x + moving_particle->velocity->x * step + moving_particle->acceleration->x * step*step /2;
        double py = moving_particle->position->y + moving_particle->velocity->y * step + moving_particle->acceleration->y * step*step /2;
        double pz = moving_particle->position->z + moving_particle->velocity->z * step + moving_particle->acceleration->z * step*step /2;

        results_array[i]->position->x = px;
        results_array[i]->position->y = py;
        results_array[i]->position->z = pz;
        
        printf ("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            
        for (int j = 0; j < size - 1; j++) {

            Results* results = results_array[j];
            free(results[i]->grav_force);
            free(results[i]->eletric_force);
            free(results[i]->total_force);
            free(results[i]);
        }

        free(results_array);
        //list_destroy(particles_list, particle_id);
    }

    // TODO como chamar o fx, fy, fz calculado dentro do for?
    
    user->no_sim_in_space = false;
    
}
*/

char* change_particle(App app, char* name, char* space, char* particle, double mass, double charge, double px, double py, double pz, double vx, double vy, double vz) {
    User user = hash_table_get(app->users, name);
    Space space_id = hash_table_get(user->simulation_spaces, space);
    Particles particle_id = hash_table_get(space_id->particles, particle);
    if (!valid_mass(mass)) {
        particle_id->mass = mass;
    }
    particle_id->charge = charge;
    if (particle_id->position != NULL) {
        vector_free(particle_id->position);
    }
    particle_id->position = vector_new(px, py, pz);
    if (particle_id->velocity != NULL) {
        vector_free(particle_id->velocity);
    }
    particle_id->velocity = vector_new(vx, vy, vz);
    return particle;
}

 
char* remove_particle(App app, char* name, char* space_id, char* particle_id) {
    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    Particles particle = hash_table_get(space->particles, particle_id);
    hash_table_remove(space->particles, particle_id);
    space->next_particle_id--;
    free_particle(particle);
    return particle_id;
}
    
bool valid_mass(double mass) {
    if (mass > 0){
        return true;
    } else {
        return false;
    }    
}    

bool valid_time(double time, double step){
    if(step >= time || step <= 0){
        return false;
    }else{
        return true;
    }
}



/*
void contas(Particles static_particle, Particles moving_particle, double step){
    //contas para uma particula de cada vez
    double const_gravity = 6674184 *10^-17;
    double const_eletric = 89875517923 *10^-1;
    

    double dx = static_particle->position->x - moving_particle->position->x;
    double dy = static_particle->position->y - moving_particle->position->y;
    double dz = static_particle->position->z - moving_particle->position->z;
    
    double fgx = const_gravity * moving_particle->mass * static_particle->mass / dx * dx;
    double fgy = const_gravity * moving_particle->mass * static_particle->mass / dy * dy;
    double fgz = const_gravity * moving_particle->mass * static_particle->mass / dz * dz;
    
    double fex = const_eletric * moving_particle->charge * static_particle->charge / dx * dx;
    double fey = const_eletric * moving_particle->charge * static_particle->charge / dy * dy;
    double fez = const_eletric * moving_particle->charge * static_particle->charge / dz * dz;

    double fx = fgx + fex;
    double fy = fgy + fey;
    double fz = fgz + fez;
    // aceleração, velocidade e posição vão ser calculadas com a força final (depois de ler o array)
    double ax = fx / moving_particle->mass;
    double ay = fy / moving_particle->mass;
    double az = fz / moving_particle->mass;

    double vx = moving_particle->velocity->x + ax * step;
    double vy = moving_particle->velocity->y + ay * step;
    double vz = moving_particle->velocity->z + az * step;

    double px = moving_particle->position->x + moving_particle->velocity->x * step + 1/2 * moving_particle->acceleration->x * step*step;
    double py = moving_particle->position->y + moving_particle->velocity->y * step + 1/2 * moving_particle->acceleration->y * step*step;
    double pz = moving_particle->position->z + moving_particle->velocity->z * step + 1/2 * moving_particle->acceleration->z * step*step;

}
*/


/*

void simulate(App app, char* name, char* space_id, char* moving_particle, double time, double step, char* filename) {
    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    Particles particle = hash_table_get(space->particles, moving_particle);
    
    if (filename[0] == '-' && filename[1] == '\0') {
        // Mostrar a simulação na consola
        printf("t,px,py,pz,vx,vy,vz,ax,ay,az,fgx,fgy,fgz,fex,fey,fez,fx,fy,fz\n");
    } else {
        // Abrir o ficheiro para escrita
        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            printf("Erro ao abrir o ficheiro.\n");
            return;
        }
        // Escrever o cabeçalho no ficheiro
        fprintf(file, "t,px,py,pz,vx,vy,vz,ax,ay,az,fgx,fgy,fgz,fex,fey,fez,fx,fy,fz\n");
    }
    
    double t = 0;
    double px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz;
    
    // Realizar a simulação até atingir o tempo desejado
    while (t <= time) {
        // Obter os valores das variáveis para o instante de tempo atual
        px = particle->position->x;
        py = particle->position->y;
        pz = particle->position->z;
        vx = particle->velocity->x;
        vy = particle->velocity->y;
        vz = particle->velocity->z;
        
        ax = particle->acceleration->x;
        ay = particle->acceleration->y;
        az = particle->acceleration->z;
        
        fgx = particle->grav_force->x;
        fgy = particle->grav_force->y;
        fgz = particle->grav_force->z;
        fex = particle->eletric_force->x;
        fey = particle->eletric_force->y;
        fez = particle->eletric_force->z;
        fx = particle->total_force->x;
        fy = particle->total_force->y;
        fz = particle->total_force->z;
        
        // Escrever os valores das variáveis no local apropriado (consola ou ficheiro)
        if (filename[0] == '-' && filename[1] == '\0') {
            printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                   t, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
        } else {
            FILE* file = fopen(filename, "a");
            if (file == NULL) {
                printf("Erro ao abrir o ficheiro.\n");

                                return;
            }
            fprintf(file, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                    t, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            fclose(file);
        }
        
        // Atualizar as variáveis para o próximo passo de simulação
        update_particle(particle, step);
        t += step;
    }
}

void update_particle(Particles particle, double step) {
    // Atualizar a posição
    particle->position->x += particle->velocity->x * step;
    particle->position->y += particle->velocity->y * step;
    particle->position->z += particle->velocity->z * step;

    // Calcular as forças atuantes na partícula
    double fgx = 0.0, fgy = 0.0, fgz = 0.0; // Componentes da força gravitacional
    double fex = 0.0, fey = 0.0, fez = 0.0; // Componentes da força elétrica

    // Calcular as forças gravitacionais com base na massa da partícula e a aceleração da gravidade
    fgx = 0.0; // Cálculo da componente x da força gravitacional
    fgy = 0.0; // Cálculo da componente y da força gravitacional
    fgz = 0.0; // Cálculo da componente z da força gravitacional

    // Calcular as forças elétricas (se necessário) com base na carga da partícula e outras partículas presentes no espaço de simulação
    fex = 0.0; // Cálculo da componente x da força elétrica
    fey = 0.0; // Cálculo da componente y da força elétrica
    fez = 0.0; // Cálculo da componente z da força elétrica

    // Calcular a força resultante
    double fx = fgx + fex; // Componente x da força resultante
    double fy = fgy + fey; // Componente y da força resultante
    double fz = fgz + fez; // Componente z da força resultante

    // Calcular a aceleração com base na força resultante e na massa da partícula
    double ax = fx / particle->mass; // Componente x da aceleração
    double ay = fy / particle->mass; // Componente y da aceleração
    double az = fz / particle->mass; // Componente z da aceleração

    // Atualizar a velocidade com base na aceleração
    particle->velocity->x += ax * step;
    particle->velocity->y += ay * step;
    particle->velocity->z += az * step;

    //autalizar a aceleração
    particle->acceleration->x += ax * step;
    particle->acceleration->y += ay * step;
    particle->acceleration->z += az * step;

}

*/

void free_all_users(List user_list) {
    if (user_list != NULL) {
        // Liberar a memória alocada para cada usuário na lista
        list_iterator_start(user_list);
        while (list_iterator_has_next(user_list)) {
            User user = list_iterator_get_next(user_list);
            free_user(user);
        }
    }
}

/*

void simulate(App app, char* name, char* space_id, char* particle_id, double time, double step, char* filename) {

    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    Particles moving_particle = hash_table_get(space->particles, particle_id);
    List particles_list = hash_table_values(space->particles);

    //double num_steps = time/step;
    double aux_step = step;
    int size = list_size(particles_list);
    Results* results_array = malloc(sizeof(Results) * (size - 1));
    Results results = malloc(sizeof(tResults));
    
    list_iterator_start(particles_list);
    int i = 0;
    while(list_iterator_has_next(particles_list)) {
        
        Particles static_particle = list_iterator_get_next(particles_list);    
       
        if(moving_particle->id != static_particle->id){ 
            //contas para uma particula de cada vez
            //Results results;
            double const_gravity = 6674184 *10^-17;
            double const_eletric = 89875517923 *10^-1;
            
            double dx = static_particle->position->x - moving_particle->position->x;
            double dy = static_particle->position->y - moving_particle->position->y;
            double dz = static_particle->position->z - moving_particle->position->z;
            //Vector distance = vector_new(dx, dy, dz);

            
            double fgx = const_gravity * moving_particle->mass * static_particle->mass / dx * dx;
            double fgy = const_gravity * moving_particle->mass * static_particle->mass / dy * dy;
            double fgz = const_gravity * moving_particle->mass * static_particle->mass / dz * dz;
            Vector gravity = vector_new(fgx, fgy, fgz);
            results->grav_force = gravity;
            /*
            results_array[i]->grav_force->x = fgx;
            results_array[i]->grav_force->y = fgy;
            results_array[i]->grav_force->z = fgz;
            */

            //FIXME
            //em vez de atribuir os valores à particula ou aos results colocamos os valores num array (results_array[1]= fgx; results_array[2]= fgy...
            //depios lemos o array desde a posiçao 0 até à positçao 18. Depois fazermos um for para o passo que vai ler sempre de 19 em 19 posições o results_array e meter um \n para apresentar como deve der ser. 
            /*
            moving_particle->grav_force->x = fgx;
            moving_particle->grav_force->y = fgy;
            moving_particle->grav_force->z = fgz;

            double fex = const_eletric * moving_particle->charge * static_particle->charge / dx * dx;
            double fey = const_eletric * moving_particle->charge * static_particle->charge / dy * dy;
            double fez = const_eletric * moving_particle->charge * static_particle->charge / dz * dz;
            Vector eletric = vector_new(fex, fey, fez);
            results->eletric_force = eletric;
            /*
            moving_particle_array[i]->eletric_force->x = fex;
            moving_particle_array[i]->eletric_force->y = fey;
            moving_particle_array[i]->eletric_force->z = fez;
            moving_particle->eletric_force->x = fex;
            moving_particle->eletric_force->y = fey;
            moving_particle->eletric_force->z = fez;

            double fx = fgx + fex;
            double fy = fgy + fey;
            double fz = fgz + fez;
            Vector force = vector_new(fx, fy, fz);
            results->total_force = force;



            results_array[i] = results;
            /*
            moving_particle_array[i]->total_force->x = fx;
            moving_particle_array[i]->total_force->y = fy;
            moving_particle_array[i]->total_force->z = fz;
            moving_particle->total_force->x = fx;
            moving_particle->total_force->y = fy;
            
            moving_particle->total_force->z = fz;
            

            // aceleração, velocidade e posição vão ser calculadas com a força final (depois de ler o array)
            i++;   
        }                    
    }    
    
    for(int i = 0; i < time; i = i + step) {

        Results final_res = results_array[i];

        double fgx = final_res->grav_force->x; 
        double fgy = final_res->grav_force->y;
        double fgz = final_res->grav_force->z;

        fgx += fgx;
        fgy += fgy;
        fgz += fgz;


        double fex = final_res->eletric_force->x;
        double fey = final_res->eletric_force->y;
        double fez = final_res->eletric_force->z;
        fex += fex;
        fey += fey;
        fez += fez;   

        
        double fx = final_res->total_force->x;
        double fy = final_res->total_force->y;
        double fz = final_res->total_force->z;
        fx += fx;
        fy += fy;
        fz += fz;            
        
        

        /*
        double fgx = 1;
        double fgy = 1;
        double fgz = 1;
        double fex = 1;
        double fey = 1;
        double fez = 1;
        double fx = 1;
        double fy = 1;
        double fz = 1;
    

        double ax = fx / moving_particle->mass;
        double ay = fy / moving_particle->mass;
        double az = fz / moving_particle->mass;
        Vector acceleration = vector_new(ax, ay, az);
        final_res->acceleration = acceleration;
        /*
        results_array[i]->acceleration->x = ax;
        results_array[i]->acceleration->y = ay;
        results_array[i]->acceleration->z = az;
        results->acceleration->x = ax;
        results->acceleration->y = ay;
        results->acceleration->z = az;
        
               
        double vx = moving_particle->velocity->x + ax * step;
        double vy = moving_particle->velocity->y + ay * step;
        double vz = moving_particle->velocity->z + az * step;
        Vector velocity = vector_new(vx, vy, vz);
        final_res->velocity = velocity;
        /*
        results_array[i]->velocity->x = vx;
        results_array[i]->velocity->y = vy;
        results_array[i]->velocity->z = vz;
        results->velocity->x = vx;
        results->velocity->y = vy;
        results->velocity->z = vz;
        

        double px = moving_particle->position->x + vx * step + ax * step * step / 2; 
        double py = moving_particle->position->y + vy * step + ay * step * step / 2 ;
        double pz = moving_particle->position->z + vz * step + az * step * step / 2 ;
        Vector position = vector_new(px, py, pz);
        /*
        results_array[i]->position->x = px;
        results_array[i]->position->y = py;
        results_array[i]->position->z = pz;
        results->position->y = py;
        results->position->z = pz;
        
        final_res->position = position;

        results_array[i] = final_res;

        
        
        // Escrever os valores das variáveis no local apropriado (consola ou ficheiro)
        if (filename[0] == '-' && filename[1] == '\0') {
            printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", aux_step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            aux_step = step + aux_step;
        } else {
            FILE* file = fopen(filename, "a");
            printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", aux_step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            fprintf(file, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", aux_step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            fclose(file);
            aux_step = step + aux_step;
        }
    }
    
    user->no_sim_in_space = false;
    free(results_array);
    //list_destroy(particles_list, particle_id);
    
}
  */






void simulate(App app, char* name, char* space_id, char* particle_id, double time, double step, char* filename) {

    User user = hash_table_get(app->users, name);
    Space space = hash_table_get(user->simulation_spaces, space_id);
    Particles moving_particle = hash_table_get(space->particles, particle_id);
    List particles_list = hash_table_values(space->particles);

    //double num_steps = time/step;
    double aux_step = step;
    int size = list_size(particles_list);
    Results* results_array = malloc(sizeof(Results) * (size - 1));
    Results results = malloc(sizeof(tResults));
    
    list_iterator_start(particles_list);
    int i = 0;
    while(list_iterator_has_next(particles_list)) {
        
        Particles static_particle = list_iterator_get_next(particles_list);    
       
        if(moving_particle->id != static_particle->id){ 
            //contas para uma particula de cada vez
            //Results results;
            double const_gravity = 6674184 *10^-17;
            double const_eletric = 89875517923 *10^-1;
            
            double dx = static_particle->position->x - moving_particle->position->x;
            double dy = static_particle->position->y - moving_particle->position->y;
            double dz = static_particle->position->z - moving_particle->position->z;
            //Vector distance = vector_new(dx, dy, dz);

            
            double fgx = const_gravity * moving_particle->mass * static_particle->mass / dx * dx;
            double fgy = const_gravity * moving_particle->mass * static_particle->mass / dy * dy;
            double fgz = const_gravity * moving_particle->mass * static_particle->mass / dz * dz;
            Vector gravity = vector_new(fgx, fgy, fgz);
            results->grav_force = gravity;

            double fex = const_eletric * moving_particle->charge * static_particle->charge / dx * dx;
            double fey = const_eletric * moving_particle->charge * static_particle->charge / dy * dy;
            double fez = const_eletric * moving_particle->charge * static_particle->charge / dz * dz;
            Vector eletric = vector_new(fex, fey, fez);
            results->eletric_force = eletric;
           
            double fx = fgx + fex;
            double fy = fgy + fey;
            double fz = fgz + fez;
            Vector force = vector_new(fx, fy, fz);
            results->total_force = force;



            results_array[i] = results;

            printf("%f", results_array[i]->eletric_force->x);

            // aceleração, velocidade e posição vão ser calculadas com a força final (depois de ler o array)
            i++;   
        }                    
    }    
    
    for(int i = 0; i < time; i = i++) {

        Results final_res = results_array[i];
        /*

        Vector fg = final_res->grav_force; 
        
        fg->x += fg->x;
        fg->y += fg->y;
        fg->z += fg->z;


        double fex = final_res->eletric_force->x;
        double fey = final_res->eletric_force->y;
        double fez = final_res->eletric_force->z;
        fex += fex;
        fey += fey;
        fez += fez;   

        
        double fx = final_res->total_force->x;
        double fy = final_res->total_force->y;
        double fz = final_res->total_force->z;
        fx += fx;
        fy += fy;
        fz += fz;            
    */
        
        double fgx = 1;
        double fgy = 1;
        double fgz = 1;
        double fex = 1;
        double fey = 1;
        double fez = 1;
        double fx = 1;
        double fy = 1;
        double fz = 1;

        double ax = fx / moving_particle->mass;
        double ay = fy / moving_particle->mass;
        double az = fz / moving_particle->mass;
        Vector acceleration = vector_new(ax, ay, az);
        final_res->acceleration = acceleration;
       
               
        double vx = moving_particle->velocity->x + ax * step;
        double vy = moving_particle->velocity->y + ay * step;
        double vz = moving_particle->velocity->z + az * step;
        Vector velocity = vector_new(vx, vy, vz);
        final_res->velocity = velocity;
        

        double px = moving_particle->position->x + vx * step + ax * step * step / 2; 
        double py = moving_particle->position->y + vy * step + ay * step * step / 2 ;
        double pz = moving_particle->position->z + vz * step + az * step * step / 2 ;
        Vector position = vector_new(px, py, pz);
       
        final_res->position = position;

        results_array[i] = final_res;

        
        /*
        // Escrever os valores das variáveis no local apropriado (consola ou ficheiro)
        if (filename[0] == '-' && filename[1] == '\0') {
            printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", aux_step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            aux_step = step + aux_step;
        } else {
            FILE* file = fopen(filename, "a");
            printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", aux_step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            fprintf(file, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", aux_step, px, py, pz, vx, vy, vz, ax, ay, az, fgx, fgy, fgz, fex, fey, fez, fx, fy, fz);
            fclose(file);
            aux_step = step + aux_step;
        }
        */
    }
    
    user->no_sim_in_space = false;
    free(results_array);
    //list_destroy(particles_list, particle_id);
    
}
