//===- DiagCommon.inc -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Format:
//   DIAG(Identity, Severity, Message)
//
// Identity:
//   a. The validate characters are all in lower case: a-z
//   b. The prefix must be the name of libraries or tools
//   c. The separator is `_`
//
// Severity:
//   a. Fatal
//   b. Error
//   c. Warning
//   d. Note
//   e. Unreachable
//===----------------------------------------------------------------------===//
DIAG(generic_note, Note, "%0")
DIAG(warning_pthread_create,      Warning, "Thread::start: The pthread_create error: %0")
DIAG(warning_pthread_join,        Warning, "Unable to join the thread. Code: %0")
DIAG(warning_cond_wait_error,     Warning, "WaitCondition::wait: unable to wait the mutex. Code: %0")
DIAG(warning_cond_signal_error,   Warning, "WaitCondition::signal: unable to wake up a thread. Code: %0")
DIAG(warning_cond_broadcast_error,Warning, "WaitCondition::broadcast: unable to wake up all threads. Code: %0")
DIAG(fatal_no_such_value,         Fatal,   "Object::operator[](\"%0\"): cannot find value by key %0")
DIAG(error_json_incorrect_grammar,Error,   "Error parsing JSON file: %0")
DIAG(error_json_parsing,          Error,   "Parsing JSON file: %0: %1.%2-%3.%4: %5")
DIAG(fatal_json_scaning,          Fatal,   "Scaning JSON file: %0: %1")
DIAG(fatal_json_open,             Fatal,   "can not open JSON file: %0")
DIAG(error_json_syntax,           Error,   "JSON syntax error: %0")
DIAG(fatal_json_read,             Fatal,   "can not read strings in JSON format:\n%0")
DIAG(fatal_multi_option_defs,     Fatal,   "The option -%0 has already been registered!")
DIAG(illegal_option_help,         Error,   "For '%0' option: %1\n%2")
DIAG(illegal_option,              Error,   "For '%0' option: %1")
DIAG(fatal_open_random,           Fatal,   "Cannot open random device '%0'")
DIAG(error_pipe_cannot_open,      Error,   "can not open pipe for task %0. (Code: %1)")
DIAG(unreach_pipe_access,         Unreachable, "can not access both sides")
DIAG(error_cannot_fork,           Error,   "can not fork child process for task %0. (Code: %1)")
DIAG(error_cannot_exec,           Error,   "can not execute the script %0. (%1)")
DIAG(cannot_set_group_leader,     Error,   "can not set process %0 (pid: %1) as a group leader (Code: %2)")
DIAG(error_child_process,         Error,   "an unknown error occurs in child process (Script %0).")
DIAG(cannot_set_sid,              Fatal,   "cannot be session leader (Code: %0)")
DIAG(cannot_open_socket,          Error,   "cannot open socket (code: %0)")
DIAG(cannot_bind_socket,          Error,   "cannot bind socket (code: %0)")
DIAG(wait_fail,                   Error,   "wait() system call fail and return immediately. (Code: %0)")
DIAG(fatal_invalid_iterator,      Fatal,   "can not derefer trivial iterator.")
DIAG(fatal_out_of_range,          Fatal,   "index %0 is out of range (size is %1).")
DIAG(fatal_open_folder,           Fatal,   "cannot open the folder `%0`. (Code: %1)")
DIAG(fatal_invalid_mutation_name, Fatal,   "invalid option name for mutation: %0")
DIAG(opt_multi_enum,              Fatal,   "Multiple definitions of cl::Enum(%0...)")
DIAG(opt_no_pos_def,              Fatal,   "Illegal argument '%0'.\nNo kPositional option was defined.")
DIAG(opt_illegal_bare,            Fatal,   "Invalid bare option '%0'")
DIAG(opt_undefined_naked_dash,    Fatal,   "Illegal argument '-'")
DIAG(opt_unknown,                 Fatal,   "Unknown command line argument: '%0'.\nDid you mean '%1%2'?")
DIAG(opt_unknown_not_guess,       Fatal,   "Unknown command line argument: '%0'.\nDid you mean '--'?")
DIAG(fatal_bad_unsafe_cast,       Fatal,   "cdl::OptStorage::unsafe_cast: failed conversion using unsafe_cast")
DIAG(duplicated_tool,             Warning, "tool %0 has been added twice.\nPlease check the configuration of toolchain %0.")
DIAG(dupliated_tool_objects,      Unreachable, "two different tool objects with the same quadruple.\n\ttool: %0\n\ttoolchain: %1")
DIAG(error_hire,                  Error,   "cannot hire a new worker. It's a rara case, means your OS is out of free memory.")
DIAG(unreach_barrier_wait_cond,   Unreachable, "Unreadable barrier wait condition: pipe %0")
DIAG(unreach_unimplement_virtual, Unreachable, "unimplemented virtual function")
DIAG(null_argument,               Warning, "Argument is NULL")
DIAG(illegal_positional,          Warning, "A positional option without definition: %0")
DIAG(no_naked_dash_def,           Warning, "A naked dash without definition: %0")
DIAG(no_option_def,               Warning, "A option without definition: %0")
DIAG(no_group_def,                Warning, "A grouped option without definition: %0")
DIAG(invalid_option,              Unreachable, "Invalid option: %0")
DIAG(no_double_naked_dash_def,    Warning, "A double naked dash without definition: %0")
DIAG(no_param_def,                Warning, "A parameter option without definition: --param %0")
DIAG(cannot_getcwd,               Error,   "getcwd() failed (code: %0)")
DIAG(cannot_drop_group_previliges, Fatal,  "unable to drop group privileges: %0")
DIAG(cannot_drop_user_previliges, Fatal,   "unable to drop user privileges: %0")
DIAG(cannot_add_event,            Unreachable, "cannot add event %0 in the event loop: %1")
DIAG(cassette_object_dup_key,     Warning, "key `%0` has been inserted in cassette Object")
DIAG(core_pass_no_print,          Error,   "Pass::print not implemented for pass: '%0'!")
DIAG(core_pass_no_name,           Error,   "Pass::getPassName not implemented for pass!")
DIAG(pass_registered,             Error,   "Pass %0 registered multiple times!")
DIAG(pass_not_registered,         Error,   "Pass %0 is not registered in global registry")
DIAG(onnx_cannot_parsed,          Error,   "Cannot parse onnx input file `%0`")
DIAG(onnx_graph_alive,            Error,   "onnx::Graph still alive after Module dead.")
DIAG(use_out_of_range,            Fatal,   "`use` out of range (%0): operator %1 contains only %2 input values")
DIAG(input_out_of_range,          Fatal,   "`input` out of range (%0): operator %1 contains only %2 input values")
