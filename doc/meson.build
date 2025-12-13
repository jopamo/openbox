doc_conf = configuration_data()
doc_conf.set('configdir', configdir_abs)

man_in_files = [
  'openbox.1.in',
  'openbox-session.1.in',
  'openbox-gnome-session.1.in',
  'openbox-kde-session.1.in',
  'obxprop.1.in',
]

man_pages = []
foreach man_in : man_in_files
  man_pages += configure_file(
    input: man_in,
    output: man_in.replace('.in', ''),
    configuration: doc_conf)
endforeach

install_man(man_pages)
