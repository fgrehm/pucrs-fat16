#!/usr/bin/env basht

# Requires https://github.com/progrium/basht to run these tests

###########################################################
# Simple helper to run the shell passing in some argument
###########################################################
run() {
  echo $1 | ./pucrs-fat16 2>&1
}

###########################################################
# Tests
###########################################################
T_recognizes_all_valid_commands() {
  for cmd in init load ls mkdir create unlink write append read; do
    if run "${cmd}" | grep -q ': command not found$'; then
      $T_fail "'${cmd}' was not recognized as a valid parameter"
      return 1
    fi
  done
}
