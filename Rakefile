require 'rake/clean'

SRC  = ['vm.c', *Dir['*.c']].uniq
OBJS = SRC.map { |file| file.gsub(/\.c$/,'.o') }
CC   = ENV['CC'] || 'cc'

CC_FLAGS   = "-Imruby/include"
LINK_FLAGS = "-Lmruby/build/host/lib -lmruby"

MRBC = 'mruby/bin/mrbc'

CLEAN.include OBJS + ['vm.c']

desc "Builds mruby"
task :mruby do
  sh 'git submodule init'
  sh 'git submodule update mruby'
  sh 'make -C mruby'
end

file MRBC => :mruby

desc "Compiles vm.rb into mruby ircode"
file 'vm.mrb' => [MRBC, 'vm.rb'] do
  sh "#{MRBC} vm.rb"
end

desc "Embeds vm.mrb into vm.c"
file 'vm.c' => ['vm.mrb', __FILE__] do
  File.open('vm.c','w') do |f|
    ir = File.binread('vm.mrb').bytes.to_a

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

file 'vm' => [:mruby, *OBJS] do
  sh "#{CC} #{CC_FLAGS} #{LINK_FLAGS} -o vm #{OBJS.join(' ')}"
end

task :default => 'vm'
