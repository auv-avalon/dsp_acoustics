begin
    require "../build/bindings/rdsp_acoustics1"
rescue LoadError => e
    puts "Cannot find Ruby Binding for the library"
end


