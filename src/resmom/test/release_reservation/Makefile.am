
PROG_ROOT = ../..

AM_CFLAGS = -g -DTEST_FUNCTION `xml2-config --cflags` -I${PROG_ROOT}/ -I${PROG_ROOT}/${PBS_MACH} -I$(PROG_ROOT)/../include --coverage -I$(top_srcdir)/src/resmom/@PBS_MACH@ -DPBS_MOM -DDEMUX=\"$(program_prefix)$(DEMUX_PATH)$(program_suffix)\" -DRCP_PATH=\"$(program_prefix)$(RCP_PATH)$(program_suffix)\" -DRCP_ARGS=\"$(RCP_ARGS)\" -DPBS_SERVER_HOME=\"$(PBS_SERVER_HOME)\" -DPBS_ENVIRON=\"$(PBS_ENVIRON)\"

AM_LIBS = `xml2-config --libs`

lib_LTLIBRARIES = libtest_release_reservation.la

AM_LDFLAGS = @CHECK_LIBS@ $(lib_LTLIBRARIES) $(AM_LIBS)

check_PROGRAMS = test_release_reservation

libtest_release_reservation_la_SOURCES = scaffolding.c $(PROG_ROOT)/release_reservation.c $(PROG_ROOT)/../lib/Libutils/u_constants.c
libtest_release_reservation_la_LDFLAGS = @CHECK_LIBS@ $(AM_LIBS) -shared -lgcov

test_release_reservation_SOURCES = test_release_reservation.c

check_SCRIPTS = ${PROG_ROOT}/../test/coverage_run.sh

TESTS = $(check_PROGRAMS) ${check_SCRIPTS}

CLEANFILES = *.gcno *.gcda *.gcov core *.lo
