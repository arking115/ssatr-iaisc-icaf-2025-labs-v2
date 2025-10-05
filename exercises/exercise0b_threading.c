/*
 * Exercise 0B: Simple Threading with pthreads
 *
 * This exercise introduces POSIX threads (pthreads), which are essential
 * for real-time systems that need concurrent tasks.
 *
 * Learning Objectives:
 * 1. Create threads with pthread_create()
 * 2. Wait for thread completion with pthread_join()
 * 3. Pass data to threads using pointers and structs
 * 4. Collect results from threads
 *
 * Key Concepts:
 * - Thread function signature: void* function_name(void* arg)
 * - pthread_t: Thread handle/identifier
 * - Passing structs to share data between main and threads
 * - Thread lifecycle: create -> execute -> join
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

/*
 * Structure to pass data to/from threads
 * This pattern is commonly used to configure thread behavior and collect results
 */
typedef struct {
    int thread_id;           // Input: thread identifier
    int work_amount;         // Input: how much work to do
    long result;             // Output: result from computation
    int status;              // Output: completion status
} thread_data_t;

/*
 * Thread function - this runs in parallel for each thread
 *
 * Key points:
 * - Must have signature: void* function_name(void* arg)
 * - arg is a pointer to data passed during pthread_create()
 * - Return value is retrieved by pthread_join()
 */
void* worker_function(void* arg) {
    // Cast the void* argument back to our struct type
    thread_data_t *data = (thread_data_t *)arg;

    printf("  [Thread %d] Started with work_amount = %d\n",
           data->thread_id, data->work_amount);

    // Simulate some work (computation)
    long sum = 0;
    for (int i = 0; i < data->work_amount; i++) {
        sum += i;
    }

    // Simulate variable execution time
    usleep(data->thread_id * 50000);  // Sleep 50ms * thread_id

    // Store results back in the struct
    data->result = sum;
    data->status = 0;  // 0 = success

    printf("  [Thread %d] Completed with result = %ld\n",
           data->thread_id, data->result);

    // Return value (can be retrieved by pthread_join)
    return data;
}

int main(void) {
    pthread_t threads[NUM_THREADS];      // Thread handles
    thread_data_t thread_data[NUM_THREADS];  // Data for each thread

    printf("=== Exercise 0B: Simple Threading ===\n\n");

    /* Step 1: Create threads */
    printf("Step 1: Creating %d threads\n", NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        // Initialize thread data
        thread_data[i].thread_id = i;
        thread_data[i].work_amount = (i + 1) * 1000000;  // Different work per thread
        thread_data[i].result = 0;
        thread_data[i].status = -1;

        // Create thread
        int ret = pthread_create(&threads[i],     // Thread handle (output)
                                 NULL,             // Default attributes
                                 worker_function,  // Function to run
                                 &thread_data[i]); // Argument to pass

        if (ret != 0) {
            fprintf(stderr, "Error: pthread_create failed for thread %d\n", i);
            return 1;
        }

        printf("  Main: Created thread %d\n", i);
    }

    printf("\n");

    /* Step 2: Wait for threads to complete */
    printf("Step 2: Waiting for threads to complete (pthread_join)\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        void *return_value;

        // pthread_join blocks until the thread completes
        int ret = pthread_join(threads[i], &return_value);

        if (ret != 0) {
            fprintf(stderr, "Error: pthread_join failed for thread %d\n", i);
            return 1;
        }

        printf("  Main: Thread %d joined successfully\n", i);
    }

    printf("\n");

    /* Step 3: Display results */
    printf("Step 3: Displaying results collected from threads\n\n");

    printf("Thread ID | Work Amount | Result      | Status\n");
    printf("----------|-------------|-------------|-------\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("    %d     | %11d | %11ld | %s\n",
               thread_data[i].thread_id,
               thread_data[i].work_amount,
               thread_data[i].result,
               thread_data[i].status == 0 ? "OK" : "FAIL");
    }

    /* Step 4: Demonstrating thread function return value */
    printf("\n\nBonus: Thread Return Values\n");
    printf("Creating a single thread and retrieving its return value...\n");

    thread_data_t bonus_data = {
        .thread_id = 99,
        .work_amount = 5000000,
        .result = 0,
        .status = -1
    };

    pthread_t bonus_thread;
    pthread_create(&bonus_thread, NULL, worker_function, &bonus_data);

    void *returned_ptr;
    pthread_join(bonus_thread, &returned_ptr);

    // The returned pointer is the same as what we passed in (modified)
    thread_data_t *returned_data = (thread_data_t *)returned_ptr;
    printf("Returned data: thread_id=%d, result=%ld\n",
           returned_data->thread_id, returned_data->result);

    /* Summary */
    printf("\n=== Key Takeaways ===\n");
    printf("✓ pthread_create() starts a new thread running a specified function\n");
    printf("✓ pthread_join() waits for thread completion (like 'wait' for processes)\n");
    printf("✓ Use structs to pass multiple parameters to threads\n");
    printf("✓ Threads can modify the struct to return results\n");
    printf("✓ Thread function signature: void* func(void* arg)\n");
    printf("✓ Cast void* to actual type inside the thread function\n");

    printf("\n=== Threading Pattern Used in RT Labs ===\n");
    printf("Exercise 2 uses this same pattern to run tasks with different scheduling policies.\n");
    printf("Each thread represents a separate RT task with its own priority.\n");

    printf("\n=== Next Steps ===\n");
    printf("Continue to Exercise 0C to learn about file I/O for storing results.\n");

    return 0;
}
