#include "testing.h"
#include "lib.h"

typedef struct {
	size_t refcount;
	uint32_t hash;
	String str;
} Bucket;

void test_StringGC_register(void) {
	StringBuffer str = StringBuffer_with_capacity(4);
	StringBuffer_push_str(&str, "foo");
	StringBuffer_transform_to_string(&str);
	GCString gcstr = StringGC_register(str);
	CU_ASSERT_PTR_NOT_NULL_FATAL(gcstr);
	Bucket *bucket = (void *)gcstr - offsetof(Bucket, str);
	CU_ASSERT_EQUAL_FATAL(bucket->refcount, 1);
	CU_ASSERT_EQUAL_FATAL(bucket->hash, String_hash(str));
	CU_ASSERT_EQUAL_FATAL(GCString_dispose(gcstr), 1);
}

int main(int argc, char **argv) {
	int status = 0;
	CU_TestInfo tests[] = {
		{ "StringGC_register", test_StringGC_register },
		CU_TEST_INFO_NULL
	};
	CU_SuiteInfo suites[] = {
		{ "GCString+StringGC", NULL, NULL, NULL, NULL, tests },
		CU_SUITE_INFO_NULL
	};
	CU_initialize_registry();
	if ((status = CU_register_suites(suites) == CUE_SUCCESS ? 0 : 1))
		goto cleanup;
	RUN_TESTS;
cleanup:
	CU_cleanup_registry();
	return status;
}
