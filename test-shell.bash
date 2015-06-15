#!/usr/bin/env basht

# Use https://github.com/progrium/basht to run these tests

export PARTITION_FILE="fat.test.part"

run() {
  echo -e $1 | ./pucrs-fat16 2>&1
  return $?
}

T_001_init_creates_partition_file() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  [[ -f $PARTITION_FILE ]] || $T_fail "Partition not created"
}

T_002_recognizes_all_valid_commands() {
  for cmd in load ls mkdir create unlink write append read; do
    # Not needed once init is only handled by the shell
    rm -f $PARTITION_FILE
    run "init" > /dev/null
    if run "${cmd}" | grep -q ': command not found$'; then
      $T_fail "'${cmd}' was not recognized as a valid command"
      return 1
    fi
  done
}

T_003_recognizes_invalid_commands() {
  rm -f $PARTITION_FILE
  if ! run "WAT" | grep -q ': command not found$'; then
    $T_fail "'WAT' was recognized as a valid command"
    return 1
  fi
}

T_005_init_recreates_partition_file() {
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

T_006_mkdir_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "mkdir" | grep -q '^$ USAGE: `mkdir '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if run "mkdir /foo/bar" | grep -q '^$ USAGE: `mkdir '; then
    $T_fail "Did not recognize the parameter provided"
    return 1
  fi
}

T_007_ls_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "ls" | grep -q '^$ USAGE: `ls '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if run "ls /foo/bar" | grep -q '^$ USAGE: `ls '; then
    $T_fail "Did not recognize the parameter provided"
    return 1
  fi
}

T_008_create_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "create" | grep -q '^$ USAGE: `create '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if run "create /foo/bar" | grep -q '^$ USAGE: `create '; then
    $T_fail "Did not recognize the parameter provided"
    return 1
  fi
}

T_009_unlink_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "unlink" | grep -q '^$ USAGE: `unlink '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if run "unlink /foo/bar" | grep -q '^$ USAGE: `unlink '; then
    $T_fail "Did not recognize the parameter provided"
    return 1
  fi
}

T_010_write_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "write" | grep -q '^$ USAGE: `write '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if ! run "write /file-name" | grep -q '^$ USAGE: `write '; then
    $T_fail "Allowed running with single argument"
    return 1
  fi

  if run "write \"some-text\" /foo/bar" | grep -q '^$ USAGE: `write '; then
    $T_fail "Did not recognize the parameters provided"
    return 1
  fi
}

T_010_append_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "append" | grep -q '^$ USAGE: `append '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if ! run "append /file-name" | grep -q '^$ USAGE: `append '; then
    $T_fail "Allowed running with single argument"
    return 1
  fi

  if run "append \"some-text\" /foo/bar" | grep -q '^$ USAGE: `append '; then
    $T_fail "Did not recognize the parameters provided"
    return 1
  fi
}

T_010_read_validates_its_arguments() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "read" | grep -q '^$ USAGE: `read '; then
    $T_fail "Did not validate empty path"
    return 1
  fi

  if run "read /foo/bar" | grep -q '^$ USAGE: `read '; then
    $T_fail "Did not recognize the parameters provided"
    return 1
  fi
}

T_011_basic_operations() {
  rm -f $PARTITION_FILE

  if ! run "init" > /dev/null; then
    $T_fail "Initialization failed"
    return 1
  fi

  if ! run "load" > /dev/null; then
    $T_fail "Load failed"
    return 1
  fi

  # ls /
  if ! run "load\nls /" | grep -q '^$ => Directory is empty'; then
    $T_fail "Was not able to list empty root dir"
    return 1
  fi

  if ! run "load\nmkdir /home\nls /" | grep -q '^\[D\] home'; then
    $T_fail "Was not able to create dir on root"
    return 1
  fi

  # create /home/file-a && create /home/file-b && ls /home
  if ! run "load\ncreate /home/file-a\ncreate /home/file-b\nls /home" > /tmp/pucrs-fat16-test ; then
    $T_fail "Was not able to create files \(1\)"
    return 1
  fi
  if ! (grep -q '^\[F\] file-a' /tmp/pucrs-fat16-test && grep -q '^\[F\] file-b' /tmp/pucrs-fat16-test); then
    $T_fail "Was not able to create files \(2\)"
    return 1
  fi

  # create /home/remove-file && unlink /home/remove-file && ls /home
  if ! run "load\ncreate /home/remove-file\nunlink /home/remove-file\nls /home" > /tmp/pucrs-fat16-test ; then
    $T_fail "Was not able to unlink file \(1\)"
    return 1
  fi
  if grep -q '^\[F\] remove-file' /tmp/pucrs-fat16-test; then
    $T_fail "Was not able to unlink file \(2\)"
    return 1
  fi
  if ! (grep -q '^\[F\] file-a' /tmp/pucrs-fat16-test && grep -q '^\[F\] file-b' /tmp/pucrs-fat16-test); then
    $T_fail "Unlink removed the wrong files"
    return 1
  fi

  # write foo-1234 /home/file-a
  if ! run "load\nwrite foo-1234 /home/file-a" > /tmp/pucrs-fat16-test ; then
    $T_fail "Was not able to write file \(1\)"
    return 1
  fi
}

T_012_does_not_allow_writing_on_top_of_a_dir() {
  rm -f $PARTITION_FILE

  if ! run "init\nload\nmkdir /a-dir\nwrite bla /a-dir" | grep -q " \[ERROR\] '/a-dir' is a directory!"; then
    $T_fail "Was able to write on top of a directory"
    return 1
  fi
}
