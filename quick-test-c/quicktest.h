#ifndef QUICKTEST_HPP
#define QUICKTEST_HPP
#include <stdio.h>
/* colors */
#define ERR_COLOR "\033[0;31m"
#define PASS_COLOR "\033[0;32m"
#define NORM_COLOR "\033[0;0m"

#define TEST_RESULT quicktest_exit_code

/* initialize tests */
#define TestInit()                                                             \
    int quicktest_nb_passed = 0;                                               \
    int quicktest_nb_test = 0;                                                 \
    int quicktest_exit_code = 0;

/* initialize tests */
#define MPI_TestInit() TestInit()

/* end tests */
#define TestEnd()                                                              \
    printf("%d tests ran.\n", quicktest_nb_test);                              \
    printf(PASS_COLOR "passed: %d\n" NORM_COLOR, quicktest_nb_passed);         \
    if (quicktest_nb_passed < quicktest_nb_test)                               \
        printf(ERR_COLOR "failed: %d\n" NORM_COLOR,                            \
               quicktest_nb_test - quicktest_nb_passed);

/* end tests */
#define MPI_TestEnd()                                                          \
    int rank = -1;                                                             \
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                      \
    if (rank == 0) {                                                           \
        printf("%d tests ran.\n", quicktest_nb_test);                          \
        printf(PASS_COLOR "passed: %d\n" NORM_COLOR, quicktest_nb_passed);     \
        if (quicktest_nb_passed < quicktest_nb_test)                           \
            printf(ERR_COLOR "failed: %d\n" NORM_COLOR,                        \
                   quicktest_nb_test - quicktest_nb_passed);                   \
    }

/* run a test name */
#define TestRun(name, desc)                                                    \
    quicktest_nb_test++;                                                       \
    if (0 == quicktest_test_##name(__LINE__, #name, desc))                     \
        quicktest_nb_passed++;                                                 \
    else                                                                       \
        quicktest_exit_code = 1;

/* create a test */
#define Test(name)                                                             \
    int quicktest_test_##name(int quicktest_line, const char *quicktest_name,  \
                              const char *quicktest_desc)

#define assert(cond)                                                           \
    if (!(cond)) {                                                             \
        printf(ERR_COLOR "Test %s (%s) at line %d failed.\n" NORM_COLOR,       \
               quicktest_name, quicktest_desc, quicktest_line);                \
        printf(ERR_COLOR "assertion failed at line %d.\n" NORM_COLOR,          \
               __LINE__);                                                      \
        return 1;                                                              \
    }

/* assert that is ran only for the rank 0 */
#define MPI_assert(cond)                                                       \
    int rank = -1;                                                             \
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                      \
    if (rank == 0 && !(cond)) {                                                \
        printf(ERR_COLOR "Test %s (%s) at line %d failed.\n" NORM_COLOR,       \
               quicktest_name, quicktest_desc, quicktest_line);                \
        printf(ERR_COLOR "assertion failed at line %d.\n" NORM_COLOR,          \
               __LINE__);                                                      \
        return 1;                                                              \
    }

/* assert that is ran only for the rank `rank` */
#define MPI_assert_rank(cond, rank)                                            \
    if (rank == 0 && !(cond)) {                                                \
        printf(ERR_COLOR "Test %s (%s) at line %d failed.\n" NORM_COLOR,       \
               quicktest_name, quicktest_desc, quicktest_line);                \
        printf(ERR_COLOR "assertion failed at line %d.\n" NORM_COLOR,          \
               __LINE__);                                                      \
        return 1;                                                              \
    }

#endif
