#include "license_pbs.h" /* See here for the software license */
#include "qstat.h"
#include "test_qstat.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

#include "pbs_error.h"
#include "qstat.h"
#include "cmds.h"

using namespace std;

extern bool connect_success;
extern char *pbs_server;

int time_to_string(char *time_string, int total_seconds);

START_TEST(time_to_string_test)
  {
  int  total_seconds = 3666;
  char buf[1024];

  time_to_string(buf, total_seconds);
  fail_unless(!strcmp("01:01:06", buf));

  total_seconds += 3600;
  total_seconds += 120;

  time_to_string(buf, total_seconds);
  fail_unless(!strcmp("02:03:06", buf));

  total_seconds += 120;
  total_seconds += 5;
  time_to_string(buf, total_seconds);
  fail_unless(!strcmp("02:05:11", buf));
  }
END_TEST

START_TEST(test_istrue)
  {
  char  some_string[20];
  int   rc;

  strcpy(some_string, "TRUE");
  rc = istrue(some_string);
  fail_unless(rc == TRUE, "TRUE string failed in istrue");

  strcpy(some_string, "1");
  rc = istrue(some_string);
  fail_unless(rc == TRUE, "'1' string failed in istrue");

  strcpy(some_string, "junk");
  rc = istrue(some_string);
  fail_unless(rc == FALSE, "invalid  string failed in istrue");

  }
END_TEST

START_TEST(test_isjobid)
  {
  int result;
  char some_string[20];

  strcpy(some_string, "1979");
  result = isjobid(some_string);
  fail_unless(result == TRUE, "isjobid failed for valid string");

  strcpy(some_string, "classof1979");
  result = isjobid(some_string);
  fail_unless(result == FALSE, "isjobid failed for valid string");

  strcpy(some_string, "-classof1979");
  result = isjobid(some_string);
  fail_unless(result == FALSE, "isjobid failed for valid string");


  }
END_TEST

START_TEST(test_timestring_to_int)
  {
  int rc;
  char some_string[20];
  int  walltime;

  strcpy(some_string, "01:01:01");
  rc  = timestring_to_int(some_string, &walltime);
  fail_unless(walltime == 3661, "timestring_to_int failed");
  fail_unless(rc == PBSE_NONE, "bad return code from timestring_to_int 1 ", rc);

  strcpy(some_string, "01:01");
  rc  = timestring_to_int(some_string, &walltime);
  fail_unless(rc == PBSE_BAD_PARAMETER, "bad return code from timestring_to_int 2 ", rc);

  strcpy(some_string, "01:01:");
  rc  = timestring_to_int(some_string, &walltime);
  fail_unless(rc == PBSE_NONE, "bad return code from timestring_to_int 3 ", rc);

  strcpy(some_string, "300");
  rc  = timestring_to_int(some_string, &walltime);
  fail_unless(rc == PBSE_BAD_PARAMETER, "bad return code from timestring_to_int 4 ", rc);


  }
END_TEST

START_TEST(test_process_commandline_opts)
  {
  int  rc;
  int  argc = 1;
  int  exec_only;
  int  errflg = 0;
  char *argv[10];

  for(int i = 0; i < 10; i++)
    argv[i] = NULL;

  /* no arguments */
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with no options ", rc);

  argv[1] = (char *)malloc(10);
  fail_unless(argv[1] != NULL, "failed to allocate memory for argv[1]");
  strcpy(argv[1], "-q");
  argc = 2;
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -q option ", rc);

  strcpy(argv[1], "-Q");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -Q option ", rc);

  strcpy(argv[1], "-B");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -B option ", rc);

  strcpy(argv[1], "-a");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -a option ", rc);

  strcpy(argv[1], "-i");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -i option ", rc);

  strcpy(argv[1], "-r");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -r option ", rc);

  strcpy(argv[1], "-e");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -e option ", rc);

  strcpy(argv[1], "-l");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -l option ", rc);

  strcpy(argv[1], "-s");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -s option ", rc);

  strcpy(argv[1], "-R");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -R option ", rc);

  strcpy(argv[1], "-R");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -R option ", rc);

  strcpy(argv[1], "-c");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -c option ", rc);

  optind = 1;
  strcpy(argv[1], "123");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with job id argument ", rc);

  argv[2] = (char *)malloc(10);
  fail_unless(argv[1] != NULL, "failed to allocate memory for argv[1]");
  argc = 3;
  optind = 1;
  strcpy(argv[1], "-B");
  strcpy(argv[2], "-Q");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_IVALREQ, "parse_commandline_opts failed with illegal -B and -Q option ", rc);

  optind = 1;
  strcpy(argv[1], "-q");
  strcpy(argv[2], "-B");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_IVALREQ, "parse_commandline_opts failed with illegal -B and -q option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-Q");
  strcpy(argv[2], "-q");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -Q and -q option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-n");
  strcpy(argv[2], "-l");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -n and -l option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-n");
  strcpy(argv[2], "-1");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -n and -1 option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-a");
  strcpy(argv[2], "-s");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -a and -s option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-a");
  strcpy(argv[2], "-R");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -a and -R option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-a");
  strcpy(argv[2], "-l");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -a and -l option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-i");
  strcpy(argv[2], "-l");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -i and -l option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-r");
  strcpy(argv[2], "-l");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -r and -l option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-e");
  strcpy(argv[2], "-l");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with illegal -e and -l option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-Q");
  strcpy(argv[2], "batch");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -Q destination option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-q");
  strcpy(argv[2], "batch");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -q destination option ", rc);

  optind = 1;
  argc = 3;
  strcpy(argv[1], "-q");
  strcpy(argv[2], "-M");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -q -M  option ", rc);

  optind = 1;
  argv[3] = (char *)malloc(10);
  fail_unless(argv[3] != NULL, "failed to allocate memory for argv[1]");
  argc = 4;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-x");
  strcpy(argv[3], "1234");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-1");
  strcpy(argv[3], "1234");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-l");
  strcpy(argv[3], "1234");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "1234");
  strcpy(argv[3], "batch");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-G");
  strcpy(argv[3], "1234");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-M");
  strcpy(argv[3], "1234");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  strcpy(argv[1], "-Q");
  strcpy(argv[2], "-f");
  strcpy(argv[3], "batch");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -Q,-f and destination options ", rc);

  argv[4] = (char *)malloc(10);
  fail_unless(argv[1] != NULL, "failed to allocate memory for argv[1]");
  optind = 1;
  argc = 4;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-W");
  strcpy(argv[3], "site_stuff");
  strcpy(argv[4], "123");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  argc = 4;
  strcpy(argv[1], "-f");
  strcpy(argv[2], "-G");
  strcpy(argv[3], "-M");
  strcpy(argv[4], "123");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_IVALREQ, "parse_commandline_opts failed with -f and -x options ", rc);

  optind = 1;
  argc = 4;
  strcpy(argv[1], "-Q");
  strcpy(argv[2], "-f");
  strcpy(argv[3], "-1");
  strcpy(argv[4], "second");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -Q, -f, -1 and destination options ", rc);

  optind = 1;
  argc = 4;
  strcpy(argv[1], "-Q");
  strcpy(argv[2], "-f");
  strcpy(argv[3], "-l");
  strcpy(argv[4], "second");
  rc = process_commandline_opts(argc, argv, &exec_only, &errflg);
  fail_unless(rc == PBSE_NONE, "parse_commandline_opts failed with -Q, -f, -l and destination options ", rc);

  }
END_TEST

START_TEST(test_get_ct)
  {
  int  jque = 0;
  int  jrun = 0;
  string state_string = "Transit:2 Queued:98 Held:2 Waiting:0 Running:10 Exiting:0 Complete:0 ";

  get_ct(state_string.c_str(), &jque, &jrun);
  fail_unless(jque == 100, "get_ct failed for 'Q'");
  fail_unless(jrun == 10, "get_ct failed for 'Q'");
  }
END_TEST

START_TEST(test_run_job_mode)
  {
  int                  rc;
  int                  located;
  char                 job_id_out[PBS_MAXCLTJOBID];
  char                 server_out[MAXSERVERNAME] = "";
  char                 server_old[MAXSERVERNAME] = "";
  char                *queue_name_out = NULL;
  char                *server_name_out = NULL;
  string               operand;
  bool                 have_args = false;
  size_t               len = PBS_MAXSERVERNAME;

  connect_success = true;
  pbs_server = (char *)malloc(PBS_MAXSERVERNAME + 1);
  fail_unless(pbs_server != NULL);
  
  rc = gethostname(pbs_server, len);
  fail_unless(rc == 0);

  rc = run_job_mode(have_args, operand.c_str(), &located, server_out, server_old, queue_name_out, server_name_out, job_id_out);
  fail_unless(rc == PBSE_NONE, "run_job_mode failed for no operand");

  have_args = true;
  operand = "(null)";
  rc = run_job_mode(have_args, operand.c_str(), &located, server_out, server_old, queue_name_out, server_name_out, job_id_out);
  fail_unless(rc == PBSE_NONE, "run_job_mode failed for '(null)' operand");

  operand = "1234";
  rc = run_job_mode(have_args, operand.c_str(), &located, server_out, server_old, queue_name_out, server_name_out, job_id_out);
  fail_unless(rc == PBSE_NONE, "run_job_mode failed for jobid 1234 operand");

  operand = "1234.hosta";
  rc = run_job_mode(have_args, operand.c_str(), &located, server_out, server_old, queue_name_out, server_name_out, job_id_out);
  fail_unless(rc == PBSE_NONE, "run_job_mode failed for jobid 1234.kmn operand");

  connect_success = false;
  rc = run_job_mode(have_args, operand.c_str(), &located, server_out, server_old, queue_name_out, server_name_out, job_id_out);
  fail_unless(rc != PBSE_NONE, "run_job_mode failed for jobid 1234.kmn operand, bad connection");

  free(pbs_server);
  }
END_TEST

START_TEST(test_run_queue_mode)
  {
  int     rc;
  bool    have_args = false;
  char    server_out[MAXSERVERNAME] = "";
  char   *queue_name_out = NULL;
  char    *server_name_out = NULL;
  string  operand;
  size_t  len = MAXSERVERNAME;
  
  connect_success = true;
  pbs_server = (char *)malloc(PBS_MAXSERVERNAME + 1);
  fail_unless(pbs_server != NULL);
  
  rc = gethostname(pbs_server, len);
  fail_unless(rc == 0);

  rc = run_queue_mode(have_args, operand.c_str(), server_out, queue_name_out, server_name_out);
  fail_unless(rc == PBSE_NONE);

  have_args = true;
  operand = "batch";
  rc = run_queue_mode(have_args, operand.c_str(), server_out, queue_name_out, server_name_out);
  fail_unless(rc == PBSE_NONE);

  operand = "(null)";
  rc = run_queue_mode(have_args, operand.c_str(), server_out, queue_name_out, server_name_out);
  fail_unless(rc == PBSE_NONE);

  connect_success = false;
  operand = "batch";
  rc = run_queue_mode(have_args, operand.c_str(), server_out, queue_name_out, server_name_out);
  fail_unless(rc != PBSE_NONE);

  free(pbs_server);

  }
END_TEST

START_TEST(test_run_server_mode)
  {
  int     rc;
  char    server_out[MAXSERVERNAME + 1] = "";
  string  operand;
  bool    have_args = false;
  size_t  len = MAXSERVERNAME;

  connect_success = true;
  pbs_server = (char *)malloc(PBS_MAXSERVERNAME + 1);
  fail_unless(pbs_server != NULL);
  
  rc = gethostname(pbs_server, len);
  fail_unless(rc == 0);

  rc = run_server_mode(have_args, operand.c_str(), server_out);
  fail_unless(rc == PBSE_NONE, "run_server_mode failed with no arguments");

  have_args = true;
  operand = "(null)";
  rc = run_server_mode(have_args, operand.c_str(), server_out);
  fail_unless(rc == PBSE_NONE, "run_server_mode failed with no (null) arguments");

  operand = "hosta";
  rc = run_server_mode(have_args, operand.c_str(), server_out);
  fail_unless(rc == PBSE_NONE, "run_server_mode failed with arguments");

  connect_success = false;
  rc = run_server_mode(have_args, operand.c_str(), server_out);
  fail_unless(rc != PBSE_NONE, "run_server_mode failed bad connect");

  }
END_TEST



START_TEST(test_two)
  {


  }
END_TEST


Suite *qstat_suite(void)
  {
  Suite *s = suite_create("qstat_suite methods");
  TCase *tc_core = tcase_create("time_to_string_test");
  tcase_add_test(tc_core, time_to_string_test);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_istrue");
  tcase_add_test(tc_core, test_istrue);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_isjobid");
  tcase_add_test(tc_core, test_isjobid);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_timestring_to_int");
  tcase_add_test(tc_core, test_timestring_to_int);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_process_commandline_opts");
  tcase_add_test(tc_core, test_process_commandline_opts);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_get_ct");
  tcase_add_test(tc_core, test_get_ct);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_run_job_mode");
  tcase_add_test(tc_core, test_run_job_mode);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_run_queue_mode");
  tcase_add_test(tc_core, test_run_queue_mode);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_run_server_mode");
  tcase_add_test(tc_core, test_run_server_mode);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("test_two");
  tcase_add_test(tc_core, test_two);
  suite_add_tcase(s, tc_core);

  return s;
  }

void rundebug()
  {
  }

int main(void)
  {
  int number_failed = 0;
  SRunner *sr = NULL;
  rundebug();
  sr = srunner_create(qstat_suite());
  srunner_set_log(sr, "qstat_suite.log");
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
  }
