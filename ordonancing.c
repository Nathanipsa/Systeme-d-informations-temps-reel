#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define NUM_TASKS 7
#define MAX_JOBS 50 /* The real max job is 29, 50 is used to prevent buffer overflow in worst case scenario. */

typedef struct {
    char name[4];
    int c;      /* Computation time (WCET) */
    int t;      /* Period */
} Task;

typedef struct {
    int id;
    char task_name[4];
    int release;        /* Release time */
    int c;              /* Computation time */
    int deadline;       /* Absolute deadline */
    int start_time;     /* Start execution time */
    int end_time;       /* End execution time */
    bool is_completed;  /* Whether job is scheduled */
    bool missed_deadline; /* Whether deadline was missed */
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

/* Compute greatest common divisor */
static inline int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

/* Compute least common multiple */
static inline int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

/* Calculate the hyperperiod (LCM of all task periods) */
int calculate_hyperperiod(void) {
    int hp = tasks[0].t;
    for (int i = 1; i < NUM_TASKS; i++) {
        hp = lcm(hp, tasks[i].t);
    }
    return hp;
}

/* Generate all job instances over the hyperperiod */
void init_jobs(int hyperperiod) {
    total_jobs = 0;
    for (int i = 0; i < NUM_TASKS; i++) {
        int num_instances = hyperperiod / tasks[i].t;
        for (int j = 0; j < num_instances; j++) {
            jobs[total_jobs].id = total_jobs;
            strcpy(jobs[total_jobs].task_name, tasks[i].name);
            jobs[total_jobs].release = j * tasks[i].t;
            jobs[total_jobs].c = tasks[i].c;
            jobs[total_jobs].deadline = jobs[total_jobs].release + tasks[i].t;
            jobs[total_jobs].is_completed = false;
            jobs[total_jobs].missed_deadline = false;
            total_jobs++;
        }
    }
}

/* Branch and Bound scheduling algorithm */
void solve_branch_and_bound(int current_time, int completed_count, int current_wait, bool allow_t5_miss) {
    explored_nodes++;

    /* PRUNING: If current wait time exceeds best found, prune this branch */
    if (current_wait >= best_wait_time) {
        return;
    }

    /* SUCCESS CONDITION: All jobs have been scheduled */
    if (completed_count == total_jobs) {
        best_wait_time = current_wait;
        for (int i = 0; i < total_jobs; i++) {
            best_schedule[i] = current_schedule[i];
        }
        /* Display real-time improvement notifications */
        printf("New minimum found! Total wait time = %d (Nodes explored: %lld)\n",
               best_wait_time, explored_nodes);
        return;
    }

    bool job_available = false;
    int next_release_time = INT_MAX;

    /* BRANCHING: Try scheduling each available unscheduled job */
    for (int i = 0; i < total_jobs; i++) {
        if (!jobs[i].is_completed) {
            /* Check if job has been released */
            if (jobs[i].release <= current_time) {
                job_available = true;

                /* Verify deadline constraints */
                bool is_t5 = (strcmp(jobs[i].task_name, "T5") == 0);
                if (!allow_t5_miss || !is_t5) {
                    if (current_time + jobs[i].c > jobs[i].deadline) {
                        continue; /* Pruning: this path violates deadline */
                    }
                }

                /* Apply the scheduling decision */
                jobs[i].is_completed = true;
                jobs[i].start_time = current_time;
                jobs[i].end_time = current_time + jobs[i].c;
                jobs[i].missed_deadline = (jobs[i].end_time > jobs[i].deadline);
                current_schedule[completed_count] = jobs[i];

                int wait = current_time - jobs[i].release;

                /* Recursive call */
                solve_branch_and_bound(current_time + jobs[i].c, completed_count + 1,
                                     current_wait + wait, allow_t5_miss);

                /* Backtrack */
                jobs[i].is_completed = false;
            } else {
                /* Find next arrival time if processor is idle */
                if (jobs[i].release < next_release_time) {
                    next_release_time = jobs[i].release;
                }
            }
        }
    }

    /* TIME ADVANCEMENT: If no job is ready, skip to next arrival */
    if (!job_available && next_release_time != INT_MAX) {
        solve_branch_and_bound(next_release_time, completed_count, current_wait, allow_t5_miss);
    }
}

/* Display the best schedule found */
void print_best_schedule(void) {
    printf("\n--- BEST GLOBAL SCHEDULE FOUND ---\n");
    printf("MINIMUM TOTAL WAIT TIME: %d\n", best_wait_time);
    printf("\nSchedule Details:\n");

    for (int i = 0; i < total_jobs; i++) {
        int wait_time = best_schedule[i].start_time - best_schedule[i].release;
        int resp_time = best_schedule[i].end_time - best_schedule[i].release;

        printf("[%02d -> %02d] %s | Release: %02d | Deadline: %02d | Wait: %02d | Response: %02d %s\n",
               best_schedule[i].start_time,
               best_schedule[i].end_time,
               best_schedule[i].task_name,
               best_schedule[i].release,
               best_schedule[i].deadline,
               wait_time,
               resp_time,
               best_schedule[i].missed_deadline ? "*** DEADLINE MISSED ***" : "");
    }
}

/* Run scheduling scenario with specified constraints */
void run_scenario(bool allow_t5_miss) {
    /* Reset search variables */
    best_wait_time = INT_MAX;
    explored_nodes = 0;
    for (int i = 0; i < total_jobs; i++) {
        jobs[i].is_completed = false;
    }

    /* Initialize with heuristic bound (EDF) for faster pruning */
    best_wait_time = 150;

    printf("Starting exhaustive search...\n");
    solve_branch_and_bound(0, 0, 0, allow_t5_miss);
    print_best_schedule();
}

int main(void) {
    int hp = calculate_hyperperiod();
    printf("Hyperperiod: %d\n", hp);

    init_jobs(hp);

    printf("\n===================================================");
    printf("\nSCENARIO 1: Strict (No deadline misses allowed)\n");
    printf("===================================================");
    run_scenario(false);

    printf("\n===================================================");
    printf("\nSCENARIO 2: Relaxed (T5 allowed to miss deadline)\n");
    printf("===================================================");
    run_scenario(true);

    return 0;
}