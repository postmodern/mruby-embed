require 'rake/clean'
require 'erb'

MRUBY_ROOT = File.expand_path('mruby')
MRBC       = "#{MRUBY_ROOT}/bin/mrbc"
MRBC_FLAGS = %W[]
LIB_MRUBY  = "#{MRUBY_ROOT}/build/host/lib/libmruby.a"

ENV['MRUBY_CONFIG'] ||= File.expand_path('mruby_config.rb')

CC       = ENV['CC'] || 'cc'
CC_FLAGS = %W[-I#{MRUBY_ROOT}/include]
LD_FLAGS = %W[-static -L#{MRUBY_ROOT}/build/host/lib]

if ENV['DEBUG']
  MRBC_FLAGS << '-g'
  CC_FLAGS   << '-ggdb'
end

RUBY_SRC = Rake::FileList['lib/{**/}*.rb']
SRC      = Rake::FileList['src/*.c', 'src/lib.c'].uniq
OBJS     = SRC.ext('.o')
BIN      = 'bin'

CLEAN.include OBJS, 'lib.mrb', 'src/lib.c', BIN

file 'mruby' do
  sh 'git submodule init'
  sh 'git submodule update'
end

namespace :mruby do
  desc 'Updates the mruby repository'
  task :update do
    cd('mruby') { sh 'git pull origin master' }
  end

  desc 'Cleans the mruby repository'
  task :clean do
    cd('mruby') { sh 'make clean' }
  end

  desc 'Builds mruby'
  task :build do
    cd('mruby') { sh 'make' }
  end

  desc 'Starts the mruby console'
  task :console do
    system './mruby/bin/mirb'
  end
end

task :clean => 'mruby:clean'

file LIB_MRUBY => 'mruby:build'
file MRBC      => 'mruby:build'

desc "Compiles lib/*.rb into lib.mrb"
file 'lib.mrb' => [MRBC, *RUBY_SRC] do
  sh MRBC, *MRBC_FLAGS, '-o', 'lib.mrb', *RUBY_SRC
end

desc "Embeds lib.mrb into src/lib.c"
file 'src/lib.c' => ['lib.mrb', 'src/lib.c.erb'] do
  File.open('src/lib.c','w') do |f|
    ir  = File.new('lib.mrb','rb').each_byte.to_a
    erb = ERB.new(File.read('src/lib.c.erb'))

    f.write erb.result(binding)
  end
end

rule '.o' => '.c' do |t|
  sh CC, *CC_FLAGS, '-c', '-o', t.name, t.source
end

desc "Builds the binary"
file BIN => [LIB_MRUBY, *OBJS] do
  sh CC, *LD_FLAGS, '-o', BIN, *OBJS, '-lmruby', '-lm'
end

task :default => BIN
