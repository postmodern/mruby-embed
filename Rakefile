require 'rake/clean'
require 'erb'

MRUBY_ROOT = File.expand_path('mruby')
MRBC = "#{MRUBY_ROOT}/bin/mrbc"
LIB_MRUBY = "#{MRUBY_ROOT}/build/host/lib/libmruby.a"

LIB  = Dir['lib/*.rb']
SRC  = (Dir['src/*.c'] + ['src/lib.c']).uniq
OBJS = SRC.map { |file| file.gsub(/\.c$/,'.o') }
CC   = ENV['CC'] || 'cc'
CC_FLAGS = "-I#{MRUBY_ROOT}/include"
LD_FLAGS = "-static -L#{MRUBY_ROOT}/build/host/lib"

CLEAN.include OBJS + ['src/lib.c']

file 'mruby' do
  sh 'git submodule init'
end

namespace :mruby do
  desc 'Updates the mruby repository'
  task :update do
    cd('mruby') { sh 'git pull origin master' }
  end

  desc 'Cleans the mruby repository'
  task :clean do
    cd('mruby') { sh 'rake clean' }
  end

  desc 'Builds mruby'
  task :build do
    cd('mruby') { sh 'rake MRUBY_CONFIG=../mruby_config.rb' }
  end

  desc 'Starts the mruby console'
  task :console do
    system './mruby/bin/mirb'
  end
end

file LIB_MRUBY => 'mruby:build'
file MRBC      => 'mruby:build'

desc "Compiles vm.rb into mruby ircode"
file 'lib.mrb' => [MRBC, *LIB] do
  sh "#{MRBC} -o lib.mrb #{LIB.join(' ')}"
end

desc "Embeds lib.mrb into src/lib.c"
file 'src/lib.c' => ['lib.mrb', 'src/lib.c.erb'] do
  File.open('src/lib.c','w') do |f|
    ir  = File.new('lib.mrb','rb').each_byte.to_a
    erb = ERB.new(File.read('src/lib.c.erb'))

    f.write erb.result(binding)
  end
end

OBJS.zip(SRC).each do |obj,src|
  file obj => src do
    sh "#{CC} #{CC_FLAGS} -c -o #{obj} #{src}"
  end
end

desc "Builds the binary"
file 'bin' => [LIB_MRUBY, *OBJS] do
  sh "#{CC} #{LD_FLAGS} -o bin #{OBJS.join(' ')} -lmruby -lm"
end

task :default => 'bin'
