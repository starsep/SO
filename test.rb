#!/usr/bin/env ruby

PROGRAM = "ToONP"

bar = Proc.new { puts "====================" }

tests = [
    "123 + 432",
    "             ",
    "    123    +  432 ^ 1   ",
    "12 + a * ( b * c + d / e ) ",
    " ( ( 2 + 7 ) / 3 + ( 14 - 3 ) * 4 ) / 2    ",
    " ( ( ( ( ( ( ( ( ( ( ) ) ) ) ) ) ) ) ) ) ",
    "2 + 33333 - 33333 ^ 1 / 5 ^ ( 3 / 2 - 2 ) - 2",
    "3 ^ 4 / 5 ^ ( 6 / 7 - 8 )"
]

outputs = [
    "123 432 +",
    "",
    "123 432 1 ^ +",
    "12 a b c * d e / + * +",
    "2 7 + 3 / 14 3 - 4 * + 2 /",
    "",
    "2 33333 + 33333 1 ^ 5 3 2 / 2 - ^ / - 2 -",
    "3 4 ^ 5 6 7 / 8 - ^ /"
]

results = []

bar.call
                                                    
puts " _____     _____ _____ _____    _           _       "
puts "|_   _|___|     |   | |  _  |  | |_ ___ ___| |_ ___ " 
puts "  | | | . |  |  | | | |   __|  |  _| -_|_ -|  _|_ -|" 
puts "  |_| |___|_____|_|___|__|     |_| |___|___|_| |___|" 
puts "                                                    "  

bar.call

for i in 0...tests.size do
    puts
    puts
    puts
    puts "Test #{i + 1}."
    puts "Equation: #{tests[i]}"
    bar.call
    
    output = `./#{PROGRAM} \"#{tests[i]}\"`
    puts "#{output}"
    results[i] = output.chomp == outputs[i]
    if results[i]
        puts "OK"
    else
        puts "FAILURE! Output should look like this:"
        puts "#{outputs[i]}"
    end
    
    bar.call
end

puts
puts
puts "Report."
bar.call
for i in 0...results.size do
    print "Test: #{i + 1}\t"
    puts results[i] ? "OK" : "FAILURE!"
end
