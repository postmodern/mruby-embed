require 'rake/clean'
require 'erb'

MRUBY_VERSION = ENV['MRUBY_VERSION'] || "3.0.0"
MRUBY_ARCHIVE = "mruby-#{MRUBY_VERSION}.zip"
MRUBY_URL     = "https://github.com/mruby/mruby/archive/#{MRUBY_VERSION}.zip"

MRUBY_ROOT = "vendor/mruby"
MRBC       = "#{MRUBY_ROOT}/bin/mrbc"
MRBC_FLAGS = %W[]
LIB_MRUBY  = "#{MRUBY_ROOT}/build/host/lib/libmruby.a"

ENV['MRUBY_CONFIG'] ||= File.expand_path('mruby_config.rb')

CC       = ENV['CC'] || 'cc'
CC_FLAGS = %W[-I#{MRUBY_ROOT}/include]
LD_FLAGS = %W[-static -L#{MRUBY_ROOT}/build/host/lib]
LIBS     = %W[mruby m]

if ENV['DEBUG']
  MRBC_FLAGS << '-g'
  CC_FLAGS   << '-ggdb'
end

RUBY_SRC = Rake::FileList['lib/{**/}*.rb']
SRC      = Rake::FileList['src/*.c', 'src/lib.c'].uniq
OBJS     = SRC.ext('.o')
BIN      = 'bin'

CLEAN.include OBJS, 'lib.mrb', 'src/lib.c', BIN

directory 'vendor'
file "vendor/#{MRUBY_ARCHIVE}" => 'vendor' do
  sh 'wget', '-O', "vendor/#{MRUBY_ARCHIVE}", MRUBY_URL
end

file "vendor/mruby-#{MRUBY_VERSION}" => "vendor/#{MRUBY_ARCHIVE}" do
  sh 'unzip', '-d', 'vendor', "vendor/#{MRUBY_ARCHIVE}"
end

file "vendor/mruby" => "vendor/mruby-#{MRUBY_VERSION}" do
  sh 'ln', '-sf', "mruby-#{MRUBY_VERSION}", 'vendor/mruby'
end

namespace :mruby do
  desc 'Cleans the mruby repository'
  task :clean => 'vendor/mruby' do
    cd('vendor/mruby') { sh 'rake clean' }
  end

  desc 'Builds mruby'
  task :build => 'vendor/mruby' do
    cd('vendor/mruby') { sh 'rake' }
  end

  desc 'Tests mruby'
  task :test => 'vendor/mruby' do
    cd('mruby') { sh 'rake test' }
  end

  desc 'Starts the mruby console'
  task :console => 'vendor/mruby' do
    system './vendor/mruby/bin/mirb'
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
  sh CC, *LD_FLAGS, '-o', BIN, *OBJS, *LIBS.map { |lib| "-l#{lib}" }
end

task :default => BIN
