#include <stdio.h>
#include <stdlib.h> // rand srand malloc free strtol
#include <time.h> // time(NULL)
#include <ctype.h> // tolower, isspace
#include <string.h> // fgets, strlen, strcspn
#include <stdbool.h> // bool
#include <limits.h> // INT_MIN INT_MAX

void seed_rng_once(void);
int  random_in_range(int min, int max);

bool read_int_safe(const char *prompt, int *out_value);
bool ask_yes_no(const char *prompt);

int  play_single_round(int min, int max, int max_attempts, FILE *logf);

void log_result(FILE *logf, int min, int max, int target, 
int attempts, bool won, double seconds);

void clear_stdin_if_needed(void); // i left this as a optional choice. 
//if fgets being used there is no need for this but, if scanf gets involved it can be necessary.




int main(void)
{
    seed_rng_once();

    // Trying to open the log file in append mode
    FILE *logf = fopen("game_log.txt","a");
    if (logf == NULL){
        printf("Warning: could not open log file, continuing without logging");
        // Continue without logging
    }
    bool keep_playing=true;

    while(keep_playing){
        int min, max, limit;

        // Asking for minimum bound
        if (!read_int_safe("Enter minimum: ", &min)){
            printf("Invalid input. Try again.\n");
            continue;
        }
        // Asking for maximum bound
        if (!read_int_safe("Enter maximum: ", &max)){
            printf("Invalid input. Try again.\n");
            continue;
        }
        if(min >= max){
            printf("Minimum must be less than maximum.\n");
            continue;
        }
        // Ask for max attempts (0 means unlimited)
        if (!read_int_safe("Max attempts (0 = unlimited): ", &limit)){
            limit = 0;
        }
        // Play one game round
        (void)play_single_round(min, max, limit, logf);
        // Asking the user if they want to play again
        keep_playing= ask_yes_no("Play again? (y/n): ");
    }

    if (logf !=NULL){
        fclose(logf);
    }
    
    return 0;
}

// Seed the random number generator once using system time
void seed_rng_once(void)
{
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }
}

// Generate a random number in range [min, max]
int random_in_range(int min, int max)
{
    if (max < min) {
        int tmp = min; min = max; max = tmp;
    }
    int range = max - min + 1;
    if (range <= 0) { // overflow guard
        return min;
    }
    return min + (rand() % range);
}

// Remove trailing newline from fgets
static void trim_newline(char *s)
{
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

// Safely read an integer from stdin using fgets + strtol
bool read_int_safe(const char *prompt, int *out_value)
{
    if (!out_value) return false;

    printf("%s", prompt);
    char buf[128];
    if (!fgets(buf, sizeof buf, stdin)) {
        return false;
    }
    trim_newline(buf);

    // Skip leading spaces
    char *p = buf;
    while (*p && isspace((unsigned char)*p)) p++;

    if (*p == '\0') return false;

    char *endptr = NULL;
    long v = strtol(p, &endptr, 10);

    if (endptr == p) return false; // no number found

    // Skip trailing spaces
    while (endptr && *endptr && isspace((unsigned char)*endptr)) endptr++;
    if (endptr && *endptr != '\0') return false;

    if (v < INT_MIN || v > INT_MAX) return false;

    *out_value = (int)v;
    return true;
}

// Ask yes/no question, return true if answer starts with 'y' or 'Y'
bool ask_yes_no(const char *prompt)
{
    char buf[32];
    printf("%s", prompt);
    if (!fgets(buf, sizeof buf, stdin)) {
        return false;
    }
    // Find first non-space character
    char *p = buf;
    while (*p && isspace((unsigned char)*p)) p++;
    if (*p == '\0') return false;

    char c = (char)tolower((unsigned char)*p);
    return (c == 'y');
}

// Play one round of the game
int play_single_round(int min, int max, int max_attempts, FILE *logf)
{
    int target   = random_in_range(min, max);
    int attempts = 0;

    clock_t start = clock();

    for (;;) {
        // If we reached attempt limit
        if (max_attempts > 0 && attempts >= max_attempts) {
            double seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
            printf("Out of attempts! You lose. The number was %d.\n", target);
            log_result(logf, min, max, target, attempts, false, seconds);
            return 0;
        }

        int guess;
        if (!read_int_safe("Your guess: ", &guess)) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (guess < min || guess > max) {
            printf("Out of range (%d..%d). Try again.\n", min, max);
            continue;
        }

        attempts++;

        if (guess < target) {
            printf("Higher!\n");
        } else if (guess > target) {
            printf("Lower!\n");
        } else {
            double seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
            printf("You win! Attempts: %d, Time: %.2f s\n", attempts, seconds);
            log_result(logf, min, max, target, attempts, true, seconds);
            return 1;
        }
    }
}

// Log the result to file in CSV-like format
void log_result(FILE *logf, int min, int max, int target,
                int attempts, bool won, double seconds)
{
    if (logf == NULL) return;

    time_t t = time(NULL);
    struct tm lt;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&lt, &t);   // Windows safe version
#else
    localtime_r(&t, &lt);   // POSIX safe version
#endif
    char ts[32];
    strftime(ts, sizeof ts, "%Y-%m-%d %H:%M:%S", &lt);

    fprintf(logf, "%s, %d-%d, %d, %d, %s, %.2f\n",
            ts, min, max, target, attempts, won ? "WIN" : "LOSE", seconds);
    fflush(logf);
}
