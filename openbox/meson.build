openbox_sources = files(
  'actions.c',
  'actions/addremovedesktop.c',
  'actions/all.c',
  'actions/breakchroot.c',
  'actions/close.c',
  'actions/cyclewindows.c',
  'actions/debug.c',
  'actions/decorations.c',
  'actions/desktop.c',
  'actions/directionalwindows.c',
  'actions/dock.c',
  'actions/dockautohide.c',
  'actions/execute.c',
  'actions/exit.c',
  'actions/focus.c',
  'actions/focusfallback.c',
  'actions/focustobottom.c',
  'actions/fullscreen.c',
  'actions/growtoedge.c',
  'actions/iconify.c',
  'actions/if.c',
  'actions/kill.c',
  'actions/layer.c',
  'actions/lower.c',
  'actions/maximize.c',
  'actions/move.c',
  'actions/moverelative.c',
  'actions/moveresizeto.c',
  'actions/movetoedge.c',
  'actions/omnipresent.c',
  'actions/raise.c',
  'actions/raiselower.c',
  'actions/reconfigure.c',
  'actions/resize.c',
  'actions/resizerelative.c',
  'actions/restart.c',
  'actions/shade.c',
  'actions/shadelowerraise.c',
  'actions/showdesktop.c',
  'actions/showmenu.c',
  'actions/unfocus.c',
  'client.c',
  'client_list_combined_menu.c',
  'client_list_menu.c',
  'client_menu.c',
  'config.c',
  'debug.c',
  'dock.c',
  'event.c',
  'focus.c',
  'focus_cycle.c',
  'focus_cycle_indicator.c',
  'focus_cycle_popup.c',
  'frame.c',
  'framerender.c',
  'grab.c',
  'group.c',
  'keyboard.c',
  'keytree.c',
  'menu.c',
  'menuframe.c',
  'mouse.c',
  'moveresize.c',
  'openbox.c',
  'ping.c',
  'place.c',
  'place_overlap.c',
  'popup.c',
  'prompt.c',
  'resist.c',
  'screen.c',
  'session.c',
  'stacking.c',
  'startupnotify.c',
  'translate.c',
  'window.c',
)

openbox_deps = [glib_dep, xml_dep, x11_dep, xext_dep, pango_dep, pangoxft_dep]
if have_libsn
  openbox_deps += libsn_dep
endif
if have_xcursor
  openbox_deps += xcursor_dep
endif
if have_xrandr
  openbox_deps += xrandr_dep
endif
if have_xinerama
  openbox_deps += xinerama_dep
endif
if have_session
  openbox_deps += sm_deps
endif
if intl_dep.found()
  openbox_deps += intl_dep
endif

openbox = executable(
  'openbox',
  openbox_sources + [top_version_h],
  include_directories: [common_includes],
  c_args: common_defines + feature_defines + ['-DG_LOG_DOMAIN="Openbox"'],
  dependencies: openbox_deps,
  link_with: [libobrender, libobt],
  install: true)
