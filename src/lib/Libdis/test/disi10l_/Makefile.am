PROG_ROOT = ../..

AM_CFLAGS = -g -DTEST_FUNCTION -I${PROG_ROOT}/ -I${PROG_ROOT}/${PBS_MACH} --coverage

lib_LTLIBRARIES = libdisi10l_.la

AM_LDFLAGS = @CHECK_LIBS@ ${lib_LTLIBRARIES}

check_PROGRAMS = test_disi10l_

libdisi10l__la_SOURCES = scaffolding.c ${PROG_ROOT}/disi10l_.c
libdisi10l__la_LDFLAGS = @CHECK_LIBS@ -shared -lgcov

test_disi10l__SOURCES = test_disi10l_.c

check_SCRIPTS = ${PROG_ROOT}/../../test/coverage_run.sh

TESTS = ${check_PROGRAMS} ${check_SCRIPTS}

CLEANFILES = *.gcno *.gcda *.gcov core *.lo
