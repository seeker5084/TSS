# -*- coding: utf-8 -*-
require 'readline'
require 'rexml/document'

count = 0
choice = ""
proxy = ""
appid_input = ""
width = `tput cols`.chomp.to_i
path = File.expand_path(File.dirname(__FILE__))

def loadingAnimation
  i = 0
  frames = ["...", " ..", ". .", ".. "]
  ldAnim = Thread.new do
    while (i > -1) do
      print frames[(i%4)]
      sleep 0.1
      i += 1
      print "\b\b\b"
    end
  end
  yield.tap {
    i = -100
    ldAnim.join
  }
end

def printXML (path, width)
  xml = REXML::Document.new(open("#{path}/wolfram.xml"))
  xml.elements.each('queryresult/pod') do |pod|
    next if (pod.elements['subpod/plaintext'].text == nil)
    puts "[#{pod.attributes['title']}]"
    pod.elements.each('subpod') do |subpod|
      next if (subpod.elements['plaintext'].text == nil)
      ptext = subpod.elements['plaintext'].text
      ptext.each_line do |pline|
        next if (pline == "\n")
        if (pline.length > (width-6))
          puts "    • #{pline[0, (width-6)]}"
          pline = pline[(width-6), pline.length-(width-6)]
          while (pline != nil)
            puts "      #{pline[0, (width-6)]}"
            pline = pline[(width-6), pline.length-(width-6)]
          end
        else
          puts "    • #{pline}"
        end
      end
    end
    print "\n"
  end
end

if (!File.exist?("#{path}/wa-appid"))
  puts "\nTo activate this application, you need to get your own App-ID."
  print "You can get it from \"http://developer.wolframalpha.com\".\n"
  loop do
    print "\nType(or Copy and Paste) your App-ID here :"
    appid_input = gets.chomp.to_s
    puts "\nYour App-ID is \"#{appid_input}\""
    loop do
      print "Confirm[y/N] :"
      choice = gets.to_s.slice(0,1)
      puts ""
      break if (choice =~ /[yn\n]/i)
    end
    break if (choice =~ /[y]/i)
  end
  open("#{path}/wa-appid", "w") {|f| f.write appid_input}
end

f = open("#{path}/wa-appid", "r")
appid = f.gets
f.close

puts "\nWELCOME TO...\n\n"
puts "██╗    ██╗ ██████╗ ██╗     ███████╗██████╗  █████╗ ███╗   ███╗"
puts "██║    ██║██╔═══██╗██║     ██╔════╝██╔══██╗██╔══██╗████╗ ████║"
puts "██║ █╗ ██║██║   ██║██║     █████╗  ██████╔╝███████║██╔████╔██║"
puts "██║███╗██║██║   ██║██║     ██╔══╝  ██╔══██╗██╔══██║██║╚██╔╝██║"
puts "╚███╔███╔╝╚██████╔╝███████╗██║     ██║  ██║██║  ██║██║ ╚═╝ ██║"
puts " ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝\n"
puts "\t █████╗ ██╗     ██████╗ ██╗  ██╗ █████╗"
puts "\t██╔══██╗██║     ██╔══██╗██║  ██║██╔══██╗ (R)"
puts "\t███████║██║     ██████╔╝███████║███████║"
puts "\t██╔══██║██║     ██╔═══╝ ██╔══██║██╔══██║"
puts "\t██║  ██║███████╗██║     ██║  ██║██║  ██║"
puts "\t╚═╝  ╚═╝╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝\n\n"
puts "\t\tCOMPUTATION[TM], KNOWLEDGE ENGINE"
puts "\t\t(TYPE :q AND HIT RETURN FOR QUIT)\n\n"

unless (ARGV[0].nil?)
  proxy = "-x #{ARGV[0]}"
  puts "Connecting via proxy(#{ARGV[0]})\n\n"
end

while true do
  
  query = Readline::readline("[#{count}] = ", true)
  if (query == ':q')
    puts "\nBYE...\n\n"
    break
  end
  
  query.gsub!("\`", "\\\`")
  query.gsub!("\\", "\\\\\\")
  query.gsub!("\"", "\\\"")
  query.gsub!("\'", "\\\'")
  
  query_title = query
  if (query_title.length > 24)
    query_title = query_title.slice!(0, 21)
    query_title = "#{query_title}..."
  else
    (24-query_title.length).times do |i|
      query_title = (i % 2 == 0) ? "#{query_title} " : " #{query_title}"
    end
  end

  print "Calculating \"#{query}\""
  loadingAnimation {
    system("curl -Ls --data-urlencode \"input=#{query}\" -d \"appid=#{appid}&format=plaintext\" \"http://api.wolframalpha.com/v2/query\" #{proxy} -o #{path}/wolfram.xml")
  }
  
  print "\r "
  26.times{print "_"}
  (width-27).times{print " "}
  print "\n"
  print "/ #{query_title} \\"
  (width-28).times{print "_"}
  print "\n\n"
  
  printXML(path, width)
  system("rm #{path}/wolfram.xml")
  
  width.times{print "-"}
  print "\n\n"
  
  count += 1
  
end
