#!/usr/bin/ruby

require 'time'

$stdout << "Generating new class prefix for Objective-C classes\n"
File.open("#{ENV['PROJECT_DIR']}/aucocoaclassprefix.h", "w+") do |stream|
    
  		t = Time.now.to_i
        t.round
        id = t.to_s
        stream << "#define SMTG_AU_NAMESPACE\t"
        stream << "SMTGAUCocoa#{id}_\n"
        
end