# -*- coding: utf-8 -*-
require 'readline'
require 'rexml/document'

# variables
count = 0
proxy = ""
url = "http://api.wolframalpha.com/v2/query"
width = `tput cols`.chomp.to_i
path = File.expand_path(File.dirname(__FILE__))
appid_file = "#{path}/.wolfram_alpha_appid"

# methods
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
  yield.tap do
    i = -100
    ldAnim.join
  end
end

def printXML (xml, path, width)
  if (xml.elements['queryresult/futuretopic'] != nil)
    puts xml.elements['queryresult/futuretopic'].attributes['msg']
    return
  end
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

def wa_appid_input (appid_file)
  choice = ""
  appid_input = ""
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
  open(appid_file, "w") {|f| f.write appid_input}
end

def welcome_screen
  puts "\nWELCOME TO...\n\n"
  puts "██╗    ██╗ ██████╗ ██╗     ███████╗██████╗  █████╗ ███╗   ███╗"
  puts "██║    ██║██╔═══██╗██║     ██╔════╝██╔══██╗██╔══██╗████╗ ████║"
  puts "██║ █╗ ██║██║   ██║██║     █████╗  ██████╔╝███████║██╔████╔██║"
  puts "██║███╗██║██║   ██║██║     ██╔══╝  ██╔══██╗██╔══██║██║╚██╔╝██║"
  puts "╚███╔███╔╝╚██████╔╝███████╗██║     ██║  ██║██║  ██║██║ ╚═╝ ██║"
  puts " ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝"
  puts "\t █████╗ ██╗     ██████╗ ██╗  ██╗ █████╗"
  puts "\t██╔══██╗██║     ██╔══██╗██║  ██║██╔══██╗ (R)"
  puts "\t███████║██║     ██████╔╝███████║███████║"
  puts "\t██╔══██║██║     ██╔═══╝ ██╔══██║██╔══██║"
  puts "\t██║  ██║███████╗██║     ██║  ██║██║  ██║"
  puts "\t╚═╝  ╚═╝╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝\n\n"
  puts "\t\tCOMPUTATION[TM], KNOWLEDGE ENGINE"
  puts "\t\t(TYPE :q AND HIT RETURN FOR QUIT)\n\n"
end

# main routine
wa_appid_input(appid_file) unless (File.exist?(appid_file))
f = open(appid_file, "r")
appid = f.gets
f.close
welcome_screen
unless (ARGV[0].nil?)
  proxy = ARGV[0]
  system "export http_proxy=\"#{proxy}\""
  puts "Connecting via proxy(#{proxy})\n\n"
  proxy = "-x #{proxy}"
end

loop do
  query = Readline::readline("[#{count}] = ", true)
  if (query == ':q')
    puts "\nBYE...\n\n"
    break
  end

  query_title = query
  query = query.gsub("\\", "\\[Backslash]")
  query = query.gsub("\`", "\\[RawBackquote]")
  query = query.gsub("\"", "\\[RawDoubleQuote]")
  query = query.gsub("\'", "\\[OpenCurlyQuote]")

  print "Calculating \"#{query_title}\""
  param = "--data-urlencode \"input=#{query}\""
  param = "#{param} -d \"appid=#{appid}&format=plaintext&reinterpret=true\""
  loadingAnimation do
    system("curl -Ls #{param} #{url} #{proxy} -o #{path}/wolfram.xml")
  end

  print "\r"
  width.times {print " "}
  print "\r"

  xml = REXML::Document.new(open("#{path}/wolfram.xml"))

  # "chicag -> chicago"
  if (xml.elements['queryresult/warnings/spellcheck'] != nil)
    spellcheck = xml.elements['queryresult/warnings/spellcheck']
    query_title = spellcheck.attributes['suggestion']
    msg = "Interpreting \"#{spellcheck.attributes['word']}\""
    puts "> #{msg} as \"#{spellcheck.attributes['suggestion']}\"."
  end

  # "francee splat -> frances split"
  if (xml.elements['queryresult/didyoumeans/didyoumean'] != nil)
    didyoumean = xml.elements['queryresult/didyoumeans/didyoumean'].text
    puts "> Did you mean: \"#{didyoumean}\"?"
  end

  # "bear shoe -> bear"
  if (xml.elements['queryresult/warnings/reinterpret'] != nil)
    reinterpret = xml.elements['queryresult/warnings/reinterpret']
    query_title = reinterpret.attributes['new']
    puts "> #{reinterpret.attributes['text']} #{query_title}"
  end

  # "pi"
  if (xml.elements['queryresult/assumptions'] != nil)
    cnt = 0
    assumptions = xml.elements['queryresult/assumptions']
    assumptions.elements.each('assumption') do |assumption|
      if (assumptions.attributes['count'] == "1")
        assumption.elements.each('value') do |value|
          break if (cnt != 0)
          msg = "> Assuming \"#{assumption.attributes['word']}\" is"
          puts "#{msg} #{value.attributes['desc']}."
          cnt += 1
        end
      end
      if (assumption.attributes['type'] == "FormulaSelect")
        assumption.elements.each('value') do |value|
          break if (cnt != 0)
          puts "> Reinterpreted as #{value.attributes['desc']}."
          cnt += 1
        end
      end
      break if (cnt != 0)
    end
  end

  if (query_title.length > 24)
    query_title = query_title.slice!(0, 21)
    query_title = "#{query_title}..."
  else
    (24-query_title.length).times do |i|
      query_title = (i % 2 == 0) ? "#{query_title} " : " #{query_title}"
    end
  end

  print " "
  26.times{print "_"}
  (width-27).times{print " "}
  print "\n"
  print "/ #{query_title} \\"
  (width-28).times{print "_"}
  print "\n\n"
  printXML(xml, path, width)

  system("rm #{path}/wolfram.xml")
  width.times{print "-"}
  print "\n\n"

  count += 1

end

system "export http_proxy=\"\""
