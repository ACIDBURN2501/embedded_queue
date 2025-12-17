project('queue', 'c', default_options : ['c_std=c11'])

queue_lib = library('queue',
                    'src/queue.c',
                    include_directories: include_directories('src'))

subdir('test')
