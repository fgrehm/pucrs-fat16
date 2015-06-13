#!/usr/bin/env basht

# Use https://github.com/progrium/basht to run these tests

export PARTITION_FILE="fat.test.part"

run() {
  echo $1 | ./pucrs-fat16 2>&1
  return $?
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

  [[ -f $PARTITION_FILE ]] || $T_fail "Partition not created"
}

T_init_recreates_partition_file() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi
  initial_time=$(stat -c %Y $PARTITION_FILE)

  # This sucks but time precision on my FS is 1second
  sleep 1.1

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  new_time=$(stat -c %Y $PARTITION_FILE)
  [ "${initial_time}" != "${new_time}" ] || $T_fail "Partition not recreated"

  fs_size=$(stat -c %s $PARTITION_FILE)
  [ $fs_size = '4194304' ] || $T_fail "Partition did not get wiped out"
}


T_ls_lists_dir_contents() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "ls" | grep -q '^$ USAGE: `ls '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  # TODO: Write more specs for success behavior
  if run "ls /foo/bar" | grep -q '^$ USAGE: `ls '; then
    $T_fail "Did not recognize the parameter provided"
    return 1
  fi
}
