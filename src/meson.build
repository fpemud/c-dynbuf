#
# target: libcdynbuf.so
#

libcdynbuf_symfile = join_paths(meson.current_source_dir(), 'libcdynbuf.sym')

libcdynbuf_private = static_library(
	'cdynbuf-private',
	[
		'c-dynbuf.c',
	],
	c_args: [
		'-fvisibility=hidden',
		'-fno-common',
	],
	pic: true,
)

libcdynbuf_shared = shared_library(
	'cdynbuf',
	objects: libcdynbuf_private.extract_all_objects(),
	install: not meson.is_subproject(),
	soversion: 0,
	link_depends: libcdynbuf_symfile,
	link_args: [
		'-Wl,--no-undefined',
		'-Wl,--version-script=@0@'.format(libcdynbuf_symfile),
	],
)

libcdynbuf_dep = declare_dependency(
        include_directories: include_directories('.'),
        link_with: libcdynbuf_private,
        version: meson.project_version(),
)

if not meson.is_subproject()
        install_headers('c-dynbuf.h')

        mod_pkgconfig.generate(
        		libraries: libcdynbuf_shared,
                version: meson.project_version(),
                name: 'libcdynbuf',
                filebase: 'libcdynbuf',
                description: project_description,
        )
endif

#
# target: cdynbuf_demo
#

cdynbuf_demo = executable('cdynbuf_demo', ['demo.c'], dependencies: libcdynbuf_dep)
test('Demo Program', cdynbuf_demo)
