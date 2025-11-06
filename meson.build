project(
    'cz-core',
    'c','cpp',
    version : run_command('cat', files('VERSION'), check : false).stdout().strip(),
    meson_version: '>= 0.62.0',
    default_options: [
        'warning_level=2',
        'buildtype=release',
        'cpp_std=c++20'])

NAME = 'Core'
VERSION_MAJOR = meson.project_version().split('.')[0]
VERSION_MINOR = meson.project_version().split('.')[1]
VERSION_PATCH = meson.project_version().split('.')[2]
VERSION_BUILD = run_command('cat', './BUILD', check : false).stdout()
HEADERS_INSTALL_DIR = join_paths(get_option('prefix'), get_option('includedir'))
CONF_FILE_NAME = 'CZ' + NAME + 'Version.h'

conf = configuration_data()
conf.set('VERSION_MAJOR', VERSION_MAJOR)
conf.set('VERSION_MINOR', VERSION_MINOR)
conf.set('VERSION_PATCH', VERSION_PATCH)
conf_file = configure_file(
    input : 'src/CZ/Core/' + CONF_FILE_NAME +'.in',
    output : CONF_FILE_NAME,
    configuration : conf)

install_data(
    conf_file,
    install_dir: HEADERS_INSTALL_DIR)

# -------------- DEPENDENCIES --------------

cpp  = meson.get_compiler('cpp')
pkg  = import('pkgconfig')

cz_skia_dep = dependency('cz-skia', fallback:['cz-skia', 'cz_skia_dep'])

deps = [
    cz_skia_dep,
    dependency('xkbcommon')
]

# -------------- SOURCES --------------

include_paths = [include_directories('src/')]

# All headers
headers = run_command('find', 'src/CZ', '-type', 'f', '-name', '*.h', check : false).stdout().strip().split('\n')

# Public and private API headers
header_dirs = [
    ['src/CZ/Core', 'CZ/Core'],
    ['src/CZ/Core/Events', 'CZ/Core/Events'],
    ['src/CZ/Core/Utils', 'CZ/Core/Utils']
]

foreach header_dir : header_dirs
    header_files = run_command('find', header_dir[0], '-type', 'f', '-name', '*.h', '-maxdepth', '1', check : false).stdout().strip().split('\n')
    install_headers(header_files, install_dir : join_paths(HEADERS_INSTALL_DIR, header_dir[1]))
    include_paths += include_directories(header_dir[0])
endforeach

# -------------- LIBRARY --------------

cz_core = library(
    'cz-core',
    sources : run_command('find', 'src/CZ', '-type', 'f', '-name', '*.cpp', check : false).stdout().strip().split('\n'),
    include_directories : [include_directories('src')],
    dependencies : deps,
    soversion: VERSION_MAJOR,
    install : true)

cz_core_dep = declare_dependency(
    dependencies: deps,
    include_directories : ['src', 'src/CZ'],
    link_with : cz_core)

pkg.generate(
    cz_core,
    name: 'cz-core',
    description: 'Cuarzo core C++ library.',
    version: meson.project_version(),
    subdirs: ['CZ/', 'CZ/Core', 'CZ/Core/Events', 'CZ/Core/Utils'],
    libraries: deps)

# -------------- TESTS --------------

subdir('src/tests/cz-core-timers')
