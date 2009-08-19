// Copyright 2009 Roman Shterenzon
// Author: Roman Shterenzon
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <ruby.h>
#include <google/vcdecoder.h>
#include <google/vcencoder.h>
#include <string>

#ifdef __cplusplus
#  ifndef RUBY_METHOD_FUNC /* These definitions should work for Ruby 1.4.6 */
#    define PROTECTFUNC(f) ((VALUE (*)()) f)
#    define VALUEFUNC(f) ((VALUE (*)()) f)
#    define VOIDFUNC(f)  ((void (*)()) f)
#  else
#    ifndef ANYARGS /* These definitions should work for Ruby 1.6 */
#      define PROTECTFUNC(f) ((VALUE (*)()) f)
#      define VALUEFUNC(f) ((VALUE (*)()) f)
#      define VOIDFUNC(f)  ((RUBY_DATA_FUNC) f)
#    else /* These definitions should work for Ruby 1.7+ */
#      define PROTECTFUNC(f) ((VALUE (*)(VALUE)) f)
#      define VALUEFUNC(f) ((VALUE (*)(ANYARGS)) f)
#      define VOIDFUNC(f)  ((RUBY_DATA_FUNC) f)
#    endif
#  endif
#else
#  define VALUEFUNC(f) (f)
#  define VOIDFUNC(f) (f)
#endif

using namespace std;

// Destroys the class instance
static void VCDiffEncoder_destroy(open_vcdiff::VCDiffEncoder *encoder) {
	delete encoder;
}

// No allocation is performed, but a destroy function assigned.
static VALUE _wrap_VCDiffEncoder_allocate(VALUE klass) {
	VALUE obj;
	obj = Data_Wrap_Struct(klass, 0, VCDiffEncoder_destroy, 0);
	return obj;
}

static VALUE _wrap_new_VCDiffEncoder(VALUE self, VALUE arg) {
  open_vcdiff::VCDiffEncoder *result = 0 ;
  VALUE str;

  str = StringValue(arg);
  result = new open_vcdiff::VCDiffEncoder((char const *)RSTRING_PTR(str),RSTRING_LEN(str));
  DATA_PTR(self) = result;
  return self;
}

/* call-seq:
*	  encoder.format_flags=(Fixnum) => nil
*
*  By default, VCDiffEncoder uses standard VCDIFF format.  This function
*  can be used before calling encode(), to specify that interleaved format
*  and/or checksum format should be used.
*/
static VALUE _wrap_VCDiffEncoder_SetFormatFlags(VALUE self, VALUE arg) {
	open_vcdiff::VCDiffEncoder *encoder = NULL;
	Data_Get_Struct(self, open_vcdiff::VCDiffEncoder, encoder);

  	encoder->SetFormatFlags(FIX2INT(arg));
  	return Qnil;
}

/*
*  call-seq:
*     encoder.target_matching=(bool) => nil
*
*  By default, VCDiffEncoder looks for matches in the dictionary and also in
*  the previously encoded target data.  This function can be used before
*  calling encode(), to specify whether or not target matching should be
*  enabled.
*/
static VALUE _wrap_VCDiffEncoder_SetTargetMatching(VALUE self, VALUE arg) {
  	open_vcdiff::VCDiffEncoder *encoder = NULL;
	Data_Get_Struct(self, open_vcdiff::VCDiffEncoder, encoder);

	if ( arg == Qtrue ) {
  		encoder->SetTargetMatching(true);
	}
	else if ( arg == Qfalse || arg == Qnil ) {
		encoder->SetTargetMatching(false);
	}
	else {
		rb_raise(rb_eArgError, "boolean expected");
	}
  	return Qnil;
}

/*
*  call-seq:
*     encoder.encode(data) => str or nil
*
*  Performs encoding of the data, returning <i>str</i>,
*  or <code>nil</code> if the encoder fails.
*/
static VALUE _wrap_VCDiffEncoder_Encode(VALUE self, VALUE arg) {
  	open_vcdiff::VCDiffEncoder *encoder = NULL;
  	bool result;
  	VALUE str;
  	string output;

	Data_Get_Struct(self, open_vcdiff::VCDiffEncoder, encoder);
  	str = StringValue(arg);

  	result = encoder->Encode((char const *)RSTRING_PTR(str),RSTRING_LEN(str), &output);

  	if (result) {
		str = rb_str_new(output.data(), output.length());
		return str;
  	} else {
		return Qnil;
  	}
}

// Destroys the class instance
static void VCDiffDecoder_destroy(open_vcdiff::VCDiffDecoder *decoder) {
	delete decoder;
}

// Allocation is performed, a destroy function is assigned.
static VALUE _wrap_VCDiffDecoder_allocate(VALUE klass) {
	VALUE obj;
	obj = Data_Wrap_Struct(klass, 0, VCDiffDecoder_destroy, new open_vcdiff::VCDiffDecoder());
	return obj;
}

/*
*  call-seq:
*     decoder.decode(dictionary, delta) => str or nil
*
*  Performs decoding of the delta using the provided dictionary,
*  returning <i>str</i>, or <code>nil</code> if the decoder fails.
*/
static VALUE _wrap_VCDiffDecoder_Decode(VALUE self, VALUE dict, VALUE delta) {
  	open_vcdiff::VCDiffDecoder *decoder = NULL;
  	bool result;
  	VALUE str;
	VALUE d;
  	string output;
	string *c_delta;

	Data_Get_Struct(self, open_vcdiff::VCDiffDecoder, decoder);
  	str = StringValue(dict);

	d = StringValue(delta);
	c_delta = new string(RSTRING_PTR(d), RSTRING_LEN(d));
	
  	result = decoder->Decode((char const *)RSTRING_PTR(str), RSTRING_LEN(str), *c_delta , &output);
	delete c_delta;
	
  	if (result) {
		str = rb_str_new(output.data(), output.length());
		return str;
  	} else {
		return Qnil;
  	}
}


/* Wrapper around http://code.google.com/p/open-vcdiff library */

static VALUE mVCDiff;

#ifdef __cplusplus
extern "C"
#endif
void Init_VCDiff(void) {
	VALUE VCDiffEncoder;
	VALUE VCDiffDecoder;
	
	mVCDiff = rb_define_module("VCDiff");
	
	VCDiffEncoder = rb_define_class_under(mVCDiff, "Encoder", rb_cObject);
	rb_define_alloc_func(VCDiffEncoder, _wrap_VCDiffEncoder_allocate);
	
	rb_define_method(VCDiffEncoder, "initialize", VALUEFUNC(_wrap_new_VCDiffEncoder), 1);
  	rb_define_method(VCDiffEncoder, "format_flags=", VALUEFUNC(_wrap_VCDiffEncoder_SetFormatFlags), 1);
  	rb_define_method(VCDiffEncoder, "target_matching=", VALUEFUNC(_wrap_VCDiffEncoder_SetTargetMatching), 1);
  	rb_define_method(VCDiffEncoder, "encode", VALUEFUNC(_wrap_VCDiffEncoder_Encode), 1); 

	// No extensions: the encoded format will conform to the RFC
  	// draft standard for VCDIFF.
	rb_define_const(mVCDiff, "VCD_STANDARD_FORMAT", INT2FIX(0x00));
  	// If this flag is specified, then the encoder writes each delta file
  	// window by interleaving instructions and sizes with their corresponding
  	// addresses and data, rather than placing these elements
  	// into three separate sections.  This facilitates providing partially
  	// decoded results when only a portion of a delta file window is received
  	// (e.g. when HTTP over TCP is used as the transmission protocol.)
	rb_define_const(mVCDiff, "VCD_FORMAT_INTERLEAVED", INT2FIX(0x01));
  	// If this flag is specified, then an Adler32 checksum
  	// of the target window data is included in the delta window.
	rb_define_const(mVCDiff, "VCD_FORMAT_CHECKSUM", INT2FIX(0x02));

	VCDiffDecoder = rb_define_class_under(mVCDiff, "Decoder", rb_cObject);
	rb_define_alloc_func(VCDiffDecoder, _wrap_VCDiffDecoder_allocate);

	rb_define_method(VCDiffDecoder, "decode", VALUEFUNC(_wrap_VCDiffDecoder_Decode), 2); 
}
