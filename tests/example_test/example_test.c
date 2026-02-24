#include "example_test.h"
#include "../testing.h"

uint8_t sub_test();

uint8_t sub_test() {
	return 1; // 1 is success
}

uint8_t test_example() {
	INIT_TEST();

	EXECUTE_SUBTEST(sub_test);

	EXIT_TEST();
}
