# Generated by jeweler
# DO NOT EDIT THIS FILE
# Instead, edit Jeweler::Tasks in Rakefile, and run `rake gemspec`
# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{vcdiff}
  s.version = "0.1.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Roman Shterenzon"]
  s.date = %q{2009-08-24}
  s.description = %q{An encoder and decoder for the format described in
      RFC 3284: "The VCDIFF Generic Differencing and Compression Data Format."
      The encoding strategy is largely based on Bentley-McIlroy 99:
      "Data Compression Using Long Common Strings.
      This is a wrapper aroung Google's open-vcdiff library.
      For more details visit: http://code.google.com/p/open-vcdiff}
  s.email = %q{romanbsd@yahoo.com}
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = [
    "README.markdown"
  ]
  s.files = [
    ".gitignore",
     "README.markdown",
     "Rakefile",
     "VERSION",
     "ext/extconf.rb",
     "ext/vcdiff_wrap.cxx",
     "vcdiff.gemspec"
  ]
  s.homepage = %q{http://github.com/romanbsd/vcdiff}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.3.4}
  s.summary = %q{Wrapper aroung open-vcdiff library}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
