SUBDIRS += imp \
 model \
 model_proxies \
 widgets \
 storage \
 document \
 settings \
 processor \
 client \
 dbcleaner \
 config
TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread
INCLUDEPATH += c:/build/include c:/projects/imageimp

TRANSLATIONS = imageimp_he.ts \
imageimp_en.ts
