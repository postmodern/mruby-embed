def main(argv=[])
  loop do
    print '>> '
    line = gets.chomp
    begin
      result = eval(line)

      puts "=> #{result.inspect}"
    rescue => error
      puts error.message
    end
  end
end
