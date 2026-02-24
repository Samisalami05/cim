#include "testing.h"

static testenv g_test_env = {0};

void execute_test(testfunc test, char* name) {
	printf("\n--< %s >--\n", name); 
	g_test_env.total++; 
	if (!test()) { 
		fprintf(stderr, "--< %s%s: failed%s >--\n", COL_FAIL, name, COL_ENDC); 
		g_test_env.failed++; 
	} 
	else { 
		printf("--< %s%s: passed%s >--\n", COL_GREEN, name, COL_ENDC); 
	}
}

void start_testing() {
	printf("\n===== STARTED TESTING =====\n");
	testenv_init(&g_test_env);
}

void finish_testing() {
	if (g_test_env.failed == 0) 
		printf("\n%s===== TESTS PASSED =====%s\n", COL_GREEN, COL_ENDC);
	else 
		printf("\n%s===== TESTS FAILED =====%s\n", COL_FAIL, COL_ENDC);
	printf("time: %ld ms , passed %d/%d\n\n", testenv_time(g_test_env), g_test_env.total - g_test_env.failed, g_test_env.total);
}

void testenv_init(testenv* env) {
	env->total = 0;
	env->failed = 0;
	clock_gettime(CLOCK_MONOTONIC, &env->start);
}

long testenv_time(testenv env) {
	struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    time_t sec = now.tv_sec - env.start.tv_sec;
    long nsec  = now.tv_nsec - env.start.tv_nsec;

    if (nsec < 0) {
        sec--;
        nsec += 1000000000L;
    }

    return sec * 1000L + nsec / 1000000L;
}
