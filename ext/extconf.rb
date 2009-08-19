#!/usr/bin/env ruby19
require 'mkmf'

$CFLAGS << ' -Wall'
$LDFLAGS << ' -lvcdcom -lvcdenc -lvcddec'

create_makefile('VCDiff')
