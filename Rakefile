require 'rake/clean'

MRUBY_ROOT = File.expand_path('mruby')
MRBC = "#{MRUBY_ROOT}/bin/mrbc"
LIB_MRUBY = "#{MRUBY_ROOT}/build/host/lib/libmruby.a"

LIB  = Dir['lib/*.rb']
SRC  = (Dir['*.c'] + ['lib.c']).uniq
OBJS = SRC.map { |file| file.gsub(/\.c$/,'.o') }
CC   = ENV['CC'] || 'cc'
CC_FLAGS = "-I#{MRUBY_ROOT}/include"
LD_FLAGS = "-static -L#{MRUBY_ROOT}/build/host/lib"

CLEAN.include OBJS + ['lib.c']

desc "Builds mruby"
task :mruby do
  sh 'git submodule init'
  sh 'git submodule update mruby'
  Dir.chdir('mruby') { sh 'rake MRUBY_CONFIG=../mruby_config.rb' }
end

file LIB_MRUBY => :mruby
file MRBC      => :mruby

desc "Compiles vm.rb into mruby ircode"
file 'lib.mrb' => [MRBC, *LIB] do
  sh "#{MRBC} -o lib.mrb #{LIB.join(' ')}"
end

desc "Embeds lib.mrb into vm.c"
file 'lib.c' => ['lib.mrb', __FILE__] do
  File.open('lib.c','w') do |f|
    ir = File.new('lib.mrb','rb').bytes.to_a

    f.puts %{
#include "lib.h"

#include <mruby.h>
#include <mruby/irep.h>

mrb_value lib_init(mrb_state *mrb)
{
\tconst uint8_t ircode[] = {#{ir.join(', ')}};

\treturn mrb_load_irep(mrb, ircode);
}
    }.strip
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
