#!/usr/bin/env basht

# Use https://github.com/progrium/basht to run these tests

export PARTITION_FILE="fat.test.part"

run() {
  echo $1 | ./pucrs-fat16 2>&1
}

T_recognizes_all_valid_commands() {
  for cmd in init load ls mkdir create unlink write append read; do
    # Not needed once init is only handled by the shell
    rm -f $PARTITION_FILE
    if run "${cmd}" | grep -q ': command not found$'; then
      $T_fail "'${cmd}' was not recognized as a valid command"
      return 1
    fi
  done
}

T_recognizes_invalid_commands() {
  rm -f $PARTITION_FILE
  if ! run "WAT" | grep -q ': command not found$'; then
    $T_fail "'WAT' was recognized as a valid command"
    return 1
  fi
}

T_init_creates_partition_file() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi
}
