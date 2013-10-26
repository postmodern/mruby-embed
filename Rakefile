require 'rake/clean'

MRUBY_ROOT = File.expand_path('mruby')
MRBC = "#{MRUBY_ROOT}/bin/mrbc"
LIB_MRUBY = "#{MRUBY_ROOT}/build/host/lib/libmruby.a"

LIB  = Dir['lib/*.rb']
SRC  = ['vm.c', *Dir['*.c']].uniq
OBJS = SRC.map { |file| file.gsub(/\.c$/,'.o') }
CC   = ENV['CC'] || 'cc'
CC_FLAGS = "-I#{MRUBY_ROOT}/include"
LD_FLAGS = "-static -L#{MRUBY_ROOT}/build/host/lib"

CLEAN.include OBJS + ['vm.c']

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
file 'vm.c' => ['lib.mrb', __FILE__] do
  File.open('vm.c','w') do |f|
    ir = File.new('lib.mrb','rb').bytes.to_a

    f.puts %{
#include "vm.h"

#include <mruby.h>
#include <mruby/irep.h>

mrb_state * vm_load()
{
\tconst uint8_t ircode[] = {#{ir.join(', ')}};
\tmrb_state *mrb;

\tif (!(mrb = mrb_open()))
\t{
\t\treturn NULL;
\t}

\tmrb_load_irep(mrb, ircode);
\treturn mrb;
}
    }.strip
  end
end

OBJS.zip(SRC).each do |obj,src|
  file obj => src do
    sh "#{CC} #{CC_FLAGS} -c -o #{obj} #{src}"
  end
end

file 'vm' => [LIB_MRUBY, *OBJS] do
  sh "#{CC} #{LD_FLAGS} -o vm #{OBJS.join(' ')} -lmruby -lm"
end

task :default => 'vm'
