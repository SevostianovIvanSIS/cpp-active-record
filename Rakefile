require 'rake/builder'

Rake::Builder.new do | builder |
  builder.task_namespace       = :i386
  builder.target               = 'libactive_record_sqlite_i386.a'
  builder.architecture         = 'i386'
  builder.source_search_paths  = [ 'src' ]
  builder.header_search_paths  = [ 'include/**/*.h' ]
  builder.objects_path         = 'objects_i386'
  builder.include_paths        = [ 'include' ]
  builder.library_dependencies = [ 'sqlite3' ]
end

Rake::Builder.new do | builder |
  builder.task_namespace       = :x86_64
  builder.target               = 'libactive_record_sqlite_x86_64.a'
  builder.architecture         = 'x86_64'
  builder.source_search_paths  = [ 'src' ]
  builder.header_search_paths  = [ 'include/**/*.h' ]
  builder.objects_path         = 'objects_x86_64'
  builder.include_paths        = [ 'include' ]
  builder.library_dependencies = [ 'sqlite3' ]
end

Rake::Builder.new do | builder |
  builder.task_namespace       = :test
  builder.target               = 'active_record_test'
  builder.architecture         = 'x86_64'
  builder.source_search_paths  = [ 'test' ]
  builder.header_search_paths  = [ 'test' ]
  builder.objects_path         = 'test/lib_objects'
  builder.include_paths        = [ 'include', 'test' ]
  builder.library_dependencies = [ 'sqlite3', 'gtest', 'active_record_sqlite_x86_64' ]
  builder.library_paths        = [ 'objects_x86_64' ]
  builder.target_prerequisites = [ :'rake:x86_64' ]
  builder.default_task         = [ :run ]
end
