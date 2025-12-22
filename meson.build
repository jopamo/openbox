project('openbox', 'c',
        version: '3.7',
        default_options: ['warning_level=2', 'c_std=gnu99'])

pkgconfig = import('pkgconfig')

cc = meson.get_compiler('c')

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
prefix = get_option('prefix')
bindir_rel = get_option('bindir')
libdir_rel = get_option('libdir')
datadir_rel = get_option('datadir')
libexecdir_rel = get_option('libexecdir')
sysconfdir_rel = get_option('sysconfdir')
mandir_rel = get_option('mandir')
docdir_rel = join_paths(datadir_rel, 'doc', meson.project_name())

configdir_rel = join_paths(sysconfdir_rel, 'xdg')
rcdir_rel = join_paths(configdir_rel, 'openbox')
themedir_rel = join_paths(datadir_rel, 'themes')
pixmapdir_rel = join_paths(datadir_rel, 'pixmaps')
appsdir_rel = join_paths(datadir_rel, 'applications')
xsessionsdir_rel = join_paths(datadir_rel, 'xsessions')
gnome_session_dir_rel = join_paths(datadir_rel, 'gnome-session', 'sessions')
gnome_wm_dir_rel = join_paths(datadir_rel, 'gnome', 'wm-properties')
docxbm_dir_rel = join_paths(docdir_rel, 'xbm')
xsddir_rel = join_paths(datadir_rel, 'openbox')

datadir_abs = join_paths(prefix, datadir_rel)
libexecdir_abs = join_paths(prefix, libexecdir_rel)
bindir_abs = join_paths(prefix, bindir_rel)
configdir_abs = join_paths(prefix, configdir_rel)
rcdir_abs = join_paths(prefix, rcdir_rel)
localedir_abs = join_paths(datadir_abs, 'locale')

# ---------------------------------------------------------------------------
# Versioning (mirrors Autotools setup)
# ---------------------------------------------------------------------------
rr_major_version = 3
rr_minor_version = 6
rr_micro_version = 33
rr_interface_age = 1
rr_binary_age = 1
rr_version = '@0@.@1@'.format(rr_major_version, rr_minor_version)
rr_current = rr_micro_version - rr_interface_age
rr_revision = rr_interface_age
rr_age = rr_binary_age - rr_interface_age
rr_current_minus_age = rr_current - rr_age

obt_major_version = 3
obt_minor_version = 6
obt_micro_version = 5
obt_interface_age = 3
obt_binary_age = 3
obt_version = '@0@.@1@'.format(obt_major_version, obt_minor_version)
obt_current = obt_micro_version - obt_interface_age
obt_revision = obt_interface_age
obt_age = obt_binary_age - obt_interface_age
obt_current_minus_age = obt_current - obt_age

obrender_api_subdir = 'openbox/@0@/obrender'.format(rr_version)
obt_api_subdir = 'openbox/@0@/obt'.format(obt_version)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
glib_dep = dependency('glib-2.0', version: '>=2.14.0')
pango_dep = dependency('pango', version: '>=1.8.0')
pangoxft_dep = dependency('pangoxft', version: '>=1.8.0')
xft_dep = dependency('xft')
xml_dep = dependency('libxml-2.0', version: '>=2.6.0')
x11_dep = dependency('x11')
xext_dep = dependency('xext')
xrender_dep = dependency('xrender')
xau_dep = dependency('xau')
m_dep = cc.find_library('m', required: false)
intl_dep = cc.find_library('intl', required: false)

# Optional dependencies / features
startup_opt = get_option('startup_notification')
libsn_dep = dependency('libstartup-notification-1.0',
                       required: startup_opt.enabled())

xcursor_opt = get_option('xcursor')
xcursor_dep = dependency('xcursor', required: xcursor_opt.enabled())

imlib_opt = get_option('imlib2')
imlib_dep = dependency('imlib2', required: imlib_opt.enabled())

librsvg_opt = get_option('librsvg')
librsvg_dep = dependency('librsvg-2.0', required: librsvg_opt.enabled())

xrandr_opt = get_option('xrandr')
xrandr_dep = dependency('xrandr', required: xrandr_opt.enabled())

xinerama_opt = get_option('xinerama')
xinerama_dep = dependency('xinerama', required: xinerama_opt.enabled())

xshape_opt = get_option('xshape')
have_xshape = false
if not xshape_opt.disabled()
  have_xshape = cc.has_header('X11/extensions/shape.h', dependencies: xext_dep)
  if not have_xshape and xshape_opt.enabled()
    error('XShape support requested but X11/extensions/shape.h not found')
  endif
endif

xsync_opt = get_option('xsync')
have_xsync = false
if not xsync_opt.disabled()
  have_xsync = cc.has_header('X11/extensions/sync.h', dependencies: xext_dep)
  if not have_xsync and xsync_opt.enabled()
    error('XSync support requested but X11/extensions/sync.h not found')
  endif
endif

xkb_opt = get_option('xkb')
have_xkb = false
if not xkb_opt.disabled()
  have_xkb = cc.has_function('XkbBell', dependencies: x11_dep)
  if not have_xkb and xkb_opt.enabled()
    error('XKB support requested but libX11 does not provide XkbBell()')
  endif
endif

session_opt = get_option('session_management')
sm_deps = []
have_session = false
if not session_opt.disabled()
  sm_lib = cc.find_library('SM', required: session_opt.enabled())
  ice_lib = cc.find_library('ICE', required: session_opt.enabled())
  have_session = sm_lib.found() and ice_lib.found()
  if have_session
    sm_deps = [sm_lib, ice_lib]
  elif session_opt.enabled()
    error('Session management requested but libSM/libICE not found')
  endif
endif

have_libsn = (not startup_opt.disabled()) and libsn_dep.found()
have_xcursor = (not xcursor_opt.disabled()) and xcursor_dep.found()
have_imlib = (not imlib_opt.disabled()) and imlib_dep.found()
have_librsvg = (not librsvg_opt.disabled()) and librsvg_dep.found()
have_xrandr = (not xrandr_opt.disabled()) and xrandr_dep.found()
have_xinerama = (not xinerama_opt.disabled()) and xinerama_dep.found()

# ---------------------------------------------------------------------------
# Common compile helpers
# ---------------------------------------------------------------------------
common_includes = include_directories('.')
theme_name = get_option('default_theme')

common_defines = [
  '-DLOCALEDIR="@0@"'.format(localedir_abs),
  '-DDATADIR="@0@"'.format(datadir_abs),
  '-DCONFIGDIR="@0@"'.format(configdir_abs),
  '-DPACKAGE_NAME="openbox"',
  '-DPACKAGE_TARNAME="openbox"',
  '-DPACKAGE_VERSION="@0@"'.format(meson.project_version()),
  '-DPACKAGE_STRING="openbox @0@"'.format(meson.project_version()),
  '-DPACKAGE_BUGREPORT="http://bugzilla.icculus.org"',
  '-DPACKAGE_URL=""',
  '-DPACKAGE="openbox"',
  '-DVERSION="@0@"'.format(meson.project_version()),
  '-DENABLE_NLS',
]

header_checks = {
  'ctype.h': 'HAVE_CTYPE_H',
  'dirent.h': 'HAVE_DIRENT_H',
  'errno.h': 'HAVE_ERRNO_H',
  'fcntl.h': 'HAVE_FCNTL_H',
  'grp.h': 'HAVE_GRP_H',
  'locale.h': 'HAVE_LOCALE_H',
  'pwd.h': 'HAVE_PWD_H',
  'signal.h': 'HAVE_SIGNAL_H',
  'stdio.h': 'HAVE_STDIO_H',
  'stdlib.h': 'HAVE_STDLIB_H',
  'string.h': 'HAVE_STRING_H',
  'strings.h': 'HAVE_STRINGS_H',
  'sys/select.h': 'HAVE_SYS_SELECT_H',
  'sys/socket.h': 'HAVE_SYS_SOCKET_H',
  'sys/stat.h': 'HAVE_SYS_STAT_H',
  'sys/time.h': 'HAVE_SYS_TIME_H',
  'sys/types.h': 'HAVE_SYS_TYPES_H',
  'sys/wait.h': 'HAVE_SYS_WAIT_H',
  'unistd.h': 'HAVE_UNISTD_H',
}
foreach header, define_name : header_checks
  if cc.has_header(header)
    common_defines += ['-D@0@'.format(define_name)]
  endif
endforeach

feature_defines = []
if have_libsn
  feature_defines += ['-DUSE_LIBSN=1']
endif
if have_xcursor
  feature_defines += ['-DUSE_XCURSOR=1']
endif
if have_imlib
  feature_defines += ['-DUSE_IMLIB2=1']
endif
if have_librsvg
  feature_defines += ['-DUSE_LIBRSVG=1']
endif
if have_session
  feature_defines += ['-DUSE_SM=1']
endif
if have_xkb
  feature_defines += ['-DXKB']
endif
if have_xrandr
  feature_defines += ['-DXRANDR']
endif
if have_xinerama
  feature_defines += ['-DXINERAMA']
endif
if have_xshape
  feature_defines += ['-DSHAPE']
endif
if have_xsync
  feature_defines += ['-DSYNC']
endif

# This header carries the project version string.
version_conf = configuration_data()
version_conf.set('OB_VERSION', meson.project_version())
top_version_h = configure_file(
  input: 'version.h.in',
  output: 'version.h',
  configuration: version_conf)

# ---------------------------------------------------------------------------
# Build subdirectories
# ---------------------------------------------------------------------------
subdir('obt')
subdir('obrender')
subdir('openbox')
subdir('tools')
subdir('data')
subdir('themes')
subdir('doc')
subdir('po')

# ---------------------------------------------------------------------------
# Install pkg-config files for the public libraries
# ---------------------------------------------------------------------------
obt_private_libs = [x11_dep, xext_dep, xrender_dep, xml_dep, glib_dep]
if have_xrandr
  obt_private_libs += xrandr_dep
endif
if have_xinerama
  obt_private_libs += xinerama_dep
endif

pkgconfig.generate(
  name: 'Obt',
  filebase: 'obt-3.5',
  description: 'Openbox Toolkit Library',
  version: obt_version,
  subdirs: obt_api_subdir,
  libraries: libobt,
  libraries_private: obt_private_libs,
  requires: ['glib-2.0', 'libxml-2.0'])

obrender_requires = ['obt-3.5', 'glib-2.0', 'xft', 'pangoxft']
if have_imlib
  obrender_requires += ['imlib2']
endif
if have_librsvg
  obrender_requires += ['librsvg-2.0']
endif

obrender_private_libs = [
  x11_dep, xext_dep, xrender_dep, glib_dep, xml_dep,
  pango_dep, pangoxft_dep, xft_dep,
]
if have_imlib
  obrender_private_libs += imlib_dep
endif
if have_librsvg
  obrender_private_libs += librsvg_dep
endif

pkgconfig.generate(
  name: 'ObRender',
  filebase: 'obrender-3.5',
  description: 'Openbox Render Library',
  version: rr_version,
  subdirs: obrender_api_subdir,
  libraries: libobrender,
  libraries_private: obrender_private_libs,
  requires: obrender_requires)

# ---------------------------------------------------------------------------
# Extra documentation that used to come from dist_doc_DATA
# ---------------------------------------------------------------------------
install_data(
  [
    'COMPLIANCE',
    'README',
    'AUTHORS',
    'CHANGELOG',
    'COPYING',
    'data/rc.xsd',
    'data/menu.xsd',
    'doc/rc-mouse-focus.xml',
  ],
  install_dir: docdir_rel)

# ---------------------------------------------------------------------------
# Summary output
# ---------------------------------------------------------------------------
summary({
  'Startup notification': have_libsn,
  'Xcursor': have_xcursor,
  'Imlib2 icons': have_imlib,
  'SVG icons (librsvg)': have_librsvg,
  'XRandR': have_xrandr,
  'Xinerama': have_xinerama,
  'XShape': have_xshape,
  'XSync': have_xsync,
  'XKB': have_xkb,
  'Session management': have_session,
}, bool_yn: true, section: 'Optional features')
