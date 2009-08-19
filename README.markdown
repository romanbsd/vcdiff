# VCDiff
This is a wrapper around Google's open-vcdiff library, which provides
an encoder and decoder for the format described in RFC 3284:
"The VCDIFF Generic Differencing and Compression Data Format."
The encoding strategy is largely based on Bentley-McIlroy 99:
"Data Compression Using Long Common Strings.

For more details visit: http://code.google.com/p/open-vcdiff

## Installing
	# Install the gem:
	sudo gem install romanbsd-vcdiff

## Usage
	require 'VCDiff'
	
	# Encoding
	dictionary = IO.read('dictionary_file')
	modified = IO.read('modified_file')
	# Optional
	encoder.format_flags = VCDiff::VCD_FORMAT_INTERLEAVED | VCDiff::VCD_FORMAT_CHECKSUM
	encoder = VCDiff::Encoder.new(dictionary)
	res = encoder.encode(modified)
	
	# Decoding
	decoder = VCDiff::Decoder.new
	orig = decoder.decode(dictionary, res)
	
	orig == modified	# -> true
