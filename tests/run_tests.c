#include "example_test/example_test.h"
#include "testing.h"

int main(void) {
	start_testing();	

	EXECUTE_TEST(test_example);

	finish_testing();
}
