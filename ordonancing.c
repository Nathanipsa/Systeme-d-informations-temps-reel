#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define NUM_TASKS 7
#define MAX_JOBS 50 // The real max job is 29, 50 was taken to prevent buffer overflow in the worst case scenario.

typedef struct {
    char name[4];
    int C;
    int T;
} Task;

typedef struct {
    int id;
    char task_name[4];
    int release;
    int C;
    int deadline;
    int start_time;
    int end_time;
    bool is_completed;
    bool missed_deadline;
} Job;

Task tasks[NUM_TASKS] = {
    {"T1", 13, 100}, 
    {"T2", 30, 100},
    {"T3", 20, 200},
    {"T4", 20, 200},
    {"T5", 20, 400},
    {"T6", 20, 400},
    {"T7", 30, 800}
};

Job jobs[MAX_JOBS];
Job current_schedule[MAX_JOBS];
Job best_schedule[MAX_JOBS];

int total_jobs = 0;
int best_wait_time = INT_MAX;
long long explored_nodes = 0;

// Utilitaires de calcul d'hyperpériode
int gcd(int a, int b) { return (b == 0) ? a : gcd(b, a % b); }
int lcm(int a, int b) { return (a * b) / gcd(a, b); }

int calculate_hyperperiod() {
    int hp = tasks[0].T;
    for (int i = 1; i < NUM_TASKS; i++) hp = lcm(hp, tasks[i].T);
    return hp;
}

// Génération des jobs sur l'hyperpériode
void init_jobs(int hyperperiod) {
    total_jobs = 0;
    for (int i = 0; i < NUM_TASKS; i++) {
        int num_instances = hyperperiod / tasks[i].T;
        for (int j = 0; j < num_instances; j++) {
            jobs[total_jobs].id = total_jobs;
            strcpy(jobs[total_jobs].task_name, tasks[i].name);
            jobs[total_jobs].release = j * tasks[i].T;
            jobs[total_jobs].C = tasks[i].C;
            jobs[total_jobs].deadline = jobs[total_jobs].release + tasks[i].T;
            jobs[total_jobs].is_completed = false;
            jobs[total_jobs].missed_deadline = false;
            total_jobs++;
        }
    }
}

// Algorithme récursif Branch and Bound
void solve_branch_and_bound(int current_time, int completed_count, int current_wait, bool allow_t5_miss) {
    explored_nodes++;

    // 1. ELAGAGE (Bound) : Si on fait déjà pire ou égal au meilleur trouvé, on coupe.
    if (current_wait >= best_wait_time) return;

    // 2. CONDITION DE SUCCES : Tous les jobs sont placés
    if (completed_count == total_jobs) {
        best_wait_time = current_wait;
        for (int i = 0; i < total_jobs; i++) {
            best_schedule[i] = current_schedule[i];
        }
        // Affichage en temps réel des améliorations trouvées
        printf("Nouveau minimum trouve ! Temps d'attente = %d (Noeuds explores: %lld)\n", best_wait_time, explored_nodes);
        return;
    }

    bool job_available = false;
    int next_release_time = INT_MAX;

    // 3. EXPLORATION (Branch) : On essaie de placer tous les jobs restants
    for (int i = 0; i < total_jobs; i++) {
        if (!jobs[i].is_completed) {
            
            // Le job est-il arrivé ?
            if (jobs[i].release <= current_time) {
                job_available = true;
                
                // Vérification stricte des échéances
                bool is_t5 = (strcmp(jobs[i].task_name, "T5") == 0);
                if (!allow_t5_miss || !is_t5) {
                    if (current_time + jobs[i].C > jobs[i].deadline) {
                        continue; // Ce chemin rate une échéance, on l'abandonne
                    }
                }

                // Appliquer le choix
                jobs[i].is_completed = true;
                jobs[i].start_time = current_time;
                jobs[i].end_time = current_time + jobs[i].C;
                jobs[i].missed_deadline = (jobs[i].end_time > jobs[i].deadline);
                
                current_schedule[completed_count] = jobs[i];
                
                int wait = current_time - jobs[i].release;

                // Récursion
                solve_branch_and_bound(current_time + jobs[i].C, completed_count + 1, current_wait + wait, allow_t5_miss);

                // Annuler le choix (Backtracking)
                jobs[i].is_completed = false;
            } else {
                // Trouver le temps de la prochaine arrivée si le processeur est inactif
                if (jobs[i].release < next_release_time) {
                    next_release_time = jobs[i].release;
                }
            }
        }
    }

    // 4. AVANCEMENT DU TEMPS : Si aucun job n'est prêt, le processeur doit attendre la prochaine arrivée
    if (!job_available && next_release_time != INT_MAX) {
        solve_branch_and_bound(next_release_time, completed_count, current_wait, allow_t5_miss);
    }
}

void print_best_schedule() {
    printf("\n--- MEILLEUR PLANNING GLOBAL TROUVE ---\n");
    printf("Temps d'attente MINIMUM TOTAL : %d\n", best_wait_time);
    
    for (int i = 0; i < total_jobs; i++) {
        int wait_time = best_schedule[i].start_time - best_schedule[i].release;
        int resp_time = best_schedule[i].end_time - best_schedule[i].release;
        
        printf("[%02d -> %02d] %s | Rel: %02d | DL: %02d | Wait: %02d | Resp: %02d %s\n",
               best_schedule[i].start_time,
               best_schedule[i].end_time,
               best_schedule[i].task_name,
               best_schedule[i].release,
               best_schedule[i].deadline,
               wait_time,
               resp_time,
               best_schedule[i].missed_deadline ? "*** MISSED ***" : "");
    }
}

void run_scenario(bool allow_t5_miss) {
    // Reinitialisation des variables globales pour la recherche
    best_wait_time = INT_MAX;
    explored_nodes = 0;
    for (int i = 0; i < total_jobs; i++) {
        jobs[i].is_completed = false;
    }
    
    // Pour accélérer la recherche, on injecte un "pire cas acceptable" via une heuristique simple (EDF)
    // Ici, on fixe une borne initiale arbitraire mais raisonnable (ex: 150) pour forcer un élagage rapide.
    best_wait_time = 150; 
    
    printf("Debut de la recherche exhaustive (Cela peut prendre du temps...)\n");
    solve_branch_and_bound(0, 0, 0, allow_t5_miss);
    print_best_schedule();
}

int main() {
    int hp = calculate_hyperperiod();
    printf("Hyperperiode : %d\n", hp);
    
    init_jobs(hp);
    
    printf("\n=======================================================\n");
    printf("SCENARIO 1 : Strict (Aucune echeance ratee)\n");
    printf("=======================================================\n");
    run_scenario(false);
    
    printf("\n=======================================================\n");
    printf("SCENARIO 2 : Relache (T5 autorisee a rater son echeance)\n");
    printf("=======================================================\n");
    run_scenario(true);
    
    return 0;
}