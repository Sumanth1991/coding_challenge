require 'mkmf-rice'
$LDFLAGS << " -lcryptopp"

HEADER_DIRS = [
  '/usr/local/include',
  '/usr/include',
  '/usr/local/include/cryptopp',
]

LIB_DIRS = [
  '/opt/local/lib',
  '/usr/local/lib',
  '/usr/lib',
]


create_makefile('encrypt')
