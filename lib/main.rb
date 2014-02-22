def main(argv=[])
  p argv

  loop do
    print '>> '

    unless (line = gets)
      return
    end

    begin
      result = eval(line.chomp)

      puts "=> #{result.inspect}"
    rescue => error
      puts error.message
    end
  end
end
