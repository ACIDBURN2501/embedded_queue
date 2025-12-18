queue_test_overwrite_exe = executable('queue_test_overwrite', 'test_queue.c', 
                                    include_directories: inc,
                                    link_with: [queue_lib])
queue_test_fail_exe = executable('queue_test_fail', 'test_queue_fail.c', 
                                 include_directories: inc,
                                 link_with: [queue_lib], 
                                 c_args: ['-DQUEUE_OVERWRITE_ON_FULL=0'])

test('queue_test_overwrite', queue_test_overwrite_exe)
test('queue_test_fail', queue_test_fail_exe)
