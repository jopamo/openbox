obt_sources = files(
  'display.c',
  'keyboard.c',
  'xml.c',
  'ddparse.c',
  'link.c',
  'paths.c',
  'prop.c',
  'signal.c',
  'xqueue.c',
)

obt_cargs = common_defines + feature_defines + ['-DG_LOG_DOMAIN="Obt"']
obt_deps = [glib_dep, xml_dep, x11_dep, xext_dep]
if have_xinerama
  obt_deps += xinerama_dep
endif
if have_xrandr
  obt_deps += xrandr_dep
endif

obt_version_conf = configuration_data()
obt_version_conf.set('OBT_MAJOR_VERSION', obt_major_version)
obt_version_conf.set('OBT_MINOR_VERSION', obt_minor_version)
obt_version_conf.set('OBT_MICRO_VERSION', obt_micro_version)
obt_version_h = configure_file(
  input: 'version.h.in',
  output: 'version.h',
  configuration: obt_version_conf)

libobt = library(
  'obt',
  obt_sources,
  include_directories: [common_includes],
  c_args: obt_cargs,
  dependencies: obt_deps,
  version: '@0@.@1@.@2@'.format(obt_current, obt_revision, obt_age),
  soversion: obt_current_minus_age,
  install: true)

obt_headers = files(
  'link.h',
  'display.h',
  'keyboard.h',
  'xml.h',
  'paths.h',
  'prop.h',
  'signal.h',
  'util.h',
  'xqueue.h',
)
install_headers(obt_headers + [obt_version_h], subdir: obt_api_subdir)

obt_unittests = executable(
  'obt_unittests',
  files('unittest_base.c', 'bsearch_unittest.c'),
  include_directories: [common_includes],
  c_args: common_defines + feature_defines + ['-DG_LOG_DOMAIN="Obt-Unittests"'],
  dependencies: [glib_dep],
  link_with: libobt,
  build_by_default: true,
  install: false)
