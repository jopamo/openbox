data_conf = configuration_data()
data_conf.set('version', meson.project_version())
data_conf.set('configdir', configdir_abs)
data_conf.set('rcdir', rcdir_abs)
data_conf.set('libexecdir', libexecdir_abs)
data_conf.set('bindir', bindir_abs)

# Session launcher scripts
session_scripts = [
  ['xsession/openbox-session.in', 'openbox-session'],
  ['xsession/openbox-gnome-session.in', 'openbox-gnome-session'],
  ['xsession/openbox-kde-session.in', 'openbox-kde-session'],
]
foreach script : session_scripts
  configure_file(
    input: script[0],
    output: script[1],
    configuration: data_conf,
    install: true,
    install_dir: bindir_rel,
    install_mode: 'rwxr-xr-x')
endforeach

# Desktop entries for display managers
desktop_entries = [
  ['xsession/openbox.desktop.in', 'openbox.desktop'],
  ['xsession/openbox-gnome.desktop.in', 'openbox-gnome.desktop'],
  ['xsession/openbox-kde.desktop.in', 'openbox-kde.desktop'],
]
foreach entry : desktop_entries
  configure_file(
    input: entry[0],
    output: entry[1],
    configuration: data_conf,
    install: true,
    install_dir: xsessionsdir_rel)
endforeach

# Default autostart helpers
configure_file(
  input: 'autostart/autostart.in',
  output: 'autostart',
  configuration: data_conf,
  install: true,
  install_dir: rcdir_rel,
  install_mode: 'rwxr-xr-x')

configure_file(
  input: 'autostart/openbox-autostart.in',
  output: 'openbox-autostart',
  configuration: data_conf,
  install: true,
  install_dir: libexecdir_rel,
  install_mode: 'rwxr-xr-x')

# Static data files
install_data('openbox.desktop', install_dir: appsdir_rel)
install_data('openbox.png', install_dir: pixmapdir_rel)
install_data(['rc.xml', 'menu.xml'], install_dir: rcdir_rel)
install_data('environment', install_dir: rcdir_rel, install_mode: 'rwxr-xr-x')

install_data(
  [
    'gnome-session/openbox-gnome.session',
    'gnome-session/openbox-gnome-fallback.session',
  ],
  install_dir: gnome_session_dir_rel)

install_data('gnome-wm-properties/openbox.desktop',
             install_dir: gnome_wm_dir_rel)

install_data('autostart/openbox-xdg-autostart',
             install_dir: libexecdir_rel,
             install_mode: 'rwxr-xr-x')

# Default button masks
xbm_files = files(
  'xbm/bullet.xbm',
  'xbm/close.xbm',
  'xbm/desk_toggled.xbm',
  'xbm/desk.xbm',
  'xbm/iconify.xbm',
  'xbm/max_toggled.xbm',
  'xbm/max.xbm',
  'xbm/shade_toggled.xbm',
  'xbm/shade.xbm',
)
install_data(xbm_files, install_dir: docxbm_dir_rel)
