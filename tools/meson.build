gnome_panel_control = executable(
  'gnome-panel-control',
  'gnome-panel-control/gnome-panel-control.c',
  include_directories: [common_includes],
  dependencies: [x11_dep],
  install: true)

obxprop = executable(
  'obxprop',
  'obxprop/obxprop.c',
  include_directories: [common_includes],
  dependencies: [glib_dep, x11_dep],
  install: true)

gdm_control = executable(
  'gdm-control',
  'gdm-control/gdm-control.c',
  include_directories: [common_includes],
  dependencies: [glib_dep, x11_dep, xau_dep],
  install: true)
