Gem::Specification.new do |s|
    s.name = "ssl_stat"
    s.version = "1.0.0"
    s.date = "2020-06-03"
    s.authors = ["Ferdinand E. Silva"]
    s.email = "ferdinandsilva@ferdinandsilva.com"
    s.summary = "Ruby C Language Extension To Get SSL Certificate Information From URL"
    s.homepage = "http://ferdinandsilva.com"
    s.description = "Ruby C Language Extension To Get SSL Certificate Information From URL"
    s.files = ["ext/ssl_stat/ssl_stat.c", "ext/ssl_stat/extconf.rb"]
    s.extensions = %w[ext/ssl_stat/extconf.rb]
end