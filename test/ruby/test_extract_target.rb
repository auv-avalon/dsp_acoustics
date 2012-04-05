require 'vizkit'
require 'yaml'

begin
    require "../../build/bindings/rdsp_acoustics"
rescue LoadError => e
    puts "Cannot find Ruby Binding for the library"
end


result = File.open("target_sonar3.yml","r") do |io|
            YAML.load(io.read)
         end
beam = if result.is_a?(Hash)
           result["beam"]
       else
           result
       end

0.upto(15) do |i|
    beam[i] = 0
end

plot = Vizkit.default_loader.Plot2d
plot.getXAxis.setLabel "Bin Number"

a = Rdsp_acoustics::extractTarget(beam,25)
filtered = a[2].map do |v|
                v / 50
           end

target = Array.new(filtered.size,0)
a[0].upto(a[0]+a[1]) do |i|
    target[i] = 10
end

plot.update_vector(filtered,"filtered")
plot.update_vector(target,"target")
plot.update_vector(beam,"beam")

puts a[0]
puts a[1]

plot.show
Vizkit.exec
