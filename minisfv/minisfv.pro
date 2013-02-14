CONFIG += warn_on \
          qt \
          thread \
CONFIG -= debug_and_release_target -console -debug
TEMPLATE = subdirs
DESTDIR = dist

SUBDIRS += model \
  src tests

