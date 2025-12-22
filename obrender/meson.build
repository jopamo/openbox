obrender_sources = files(
  'button.c',
  'color.c',
  'font.c',
  'gradient.c',
  'image.c',
  'imagecache.c',
  'instance.c',
  'mask.c',
  'render.c',
  'theme.c',
)

obrender_cargs = [
  '-DG_LOG_DOMAIN="ObRender"',
  '-DDEFAULT_THEME="@0@"'.format(theme_name),
]
if have_imlib
  obrender_cargs += ['-DUSE_IMLIB2']
endif
if have_librsvg
  obrender_cargs += ['-DUSE_LIBRSVG']
endif

obrender_deps = [
  glib_dep, xml_dep, pango_dep, pangoxft_dep,
  xft_dep,
  x11_dep, xext_dep, xrender_dep,
  m_dep,
]
if have_imlib
  obrender_deps += imlib_dep
endif
if have_librsvg
  obrender_deps += librsvg_dep
endif

rr_version_conf = configuration_data()
rr_version_conf.set('RR_MAJOR_VERSION', rr_major_version)
rr_version_conf.set('RR_MINOR_VERSION', rr_minor_version)
rr_version_conf.set('RR_MICRO_VERSION', rr_micro_version)
rr_version_h = configure_file(
  input: 'version.h.in',
  output: 'version.h',
  configuration: rr_version_conf)

libobrender = library(
  'obrender',
  obrender_sources,
  include_directories: [common_includes],
  c_args: obrender_cargs,
  dependencies: obrender_deps,
  link_with: libobt,
  version: '@0@.@1@.@2@'.format(rr_current, rr_revision, rr_age),
  soversion: rr_current_minus_age,
  install: true)

obrender_headers = files(
  'color.h',
  'font.h',
  'geom.h',
  'gradient.h',
  'image.h',
  'instance.h',
  'mask.h',
  'render.h',
  'theme.h',
)
install_headers(obrender_headers + [rr_version_h], subdir: obrender_api_subdir)

if get_option('rendertest')
  executable(
    'obrender-rendertest',
    'test.c',
    include_directories: [common_includes],
    c_args: ['-DG_LOG_DOMAIN="RenderTest"'],
    dependencies: [glib_dep, pango_dep, pangoxft_dep, xml_dep, x11_dep, xext_dep, xrender_dep],
    link_with: [libobrender, libobt],
    build_by_default: true,
    install: false)
endif
