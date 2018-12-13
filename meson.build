project(
        'c-dynbuf',
        'c',
        version: '1',
        license: 'LGPL-3.0',
        default_options: [
                'c_std=c11',
        ],
)
project_description = 'Dynamic Buffer Implementation in Standard ISO-C11'

add_project_arguments('-D_GNU_SOURCE', language: 'c')
mod_pkgconfig = import('pkgconfig')

subdir('src')
